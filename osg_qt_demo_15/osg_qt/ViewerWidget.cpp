#include "ViewerWidget.h"
#ifdef _DEBUG
#define MODELBASE "../../models/"
#else
#define MODELBASE "./models/"
#endif
ViewerWidget::ViewerWidget(QWidget* parent){
	mparent = parent;

	setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
	setKeyEventSetsDone(0);	// disable default setting of viewer.done() by pressing Escape.

	root = new osg::Group();
	swt = new osg::Switch();
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	createTraits(traits, "OSG_DEMO", 0, 0, 200, 200);
	qgw = new osgQt::GraphicsWindowQt(traits);

	mainView = generateMainView(qgw);
	addView(mainView);
	osg::Camera* camera = mainView->getCamera();
	camera->setGraphicsContext( qgw );
	camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
	camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );

	QHBoxLayout *boxlay = new QHBoxLayout;
	boxlay->addWidget( qgw->getGLWidget());
	setLayout( boxlay);

	connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
	_timer.start( 10 );	
}


void ViewerWidget::createTraits(osg::GraphicsContext::Traits* traits, string name, int x, int y, int H, int W){
	osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
	traits->windowName = name;
	traits->windowDecoration = false;
	traits->x = x;
	traits->y = y;
	traits->width = W;
	traits->height = H;
	traits->doubleBuffer = true;
	traits->alpha = ds->getMinimumNumAlphaBits();
	traits->stencil = ds->getMinimumNumStencilBits();
	traits->sampleBuffers = ds->getMultiSamples();
	traits->samples = 16;
	//traits->samples = ds->getNumMultiSamples();
}

osg::ref_ptr<osgViewer::View> ViewerWidget::generateMainView(osgQt::GraphicsWindowQt*  qgw){
	setlocale(LC_ALL,".936");
	osg::ref_ptr<osgViewer::Viewer> view = new osgViewer::Viewer();
	view->setDataVariance(osg::Object::DYNAMIC);
	view->setSceneData(root);
	//event handler
	osgViewer::StatsHandler *sh = new osgViewer::StatsHandler();
	sh->setKeyEventTogglesOnScreenStats('t');
	sh->setKeyEventPrintsOutStats('T');
	view->addEventHandler(sh);
	view->addEventHandler(GeneralEventHandler::Instance(mparent));
	view->setCameraManipulator(TravelManipulator::Instance());
	return view;
}

void ViewerWidget::reloadModel(int index){
	osg::Switch::ValueList vl = swt->getValueList();
	int vllen = vl.size(), mark = 0;
	for(mark = 0; mark< vllen; mark++)
		if(vl[mark] == true)
			break;
	cameraContextList[mark] = TravelManipulator::Instance()->getCameraContext();
	swt->setSingleChildOn(index);
	GeneralEventHandler::Instance(this)->setDBMap(generateDBMap(index));
	GeneralEventHandler::Instance(this)->setCurrentScene(swt->getChild(index)->asSwitch(), index);
	TravelManipulator::Instance()->setCameraContext(cameraContextList[index]);
}

void ViewerWidget::loadModels(int size){
	cameraContextList = vector<CameraContext>(size);//cameraContextList初始化的语句必须在线程开始之前执行
	thread = boost::thread(&ViewerWidget::loadModleThread, this, size);
	osg::ref_ptr<osg::Node> node;
	osg::ref_ptr<osg::Switch> underswt = new osg::Switch();
	node = osgDB::readNodeFile(string(MODELBASE)+"0.ive");
	underswt->addChild(node, true);
	GeneralEventHandler::Instance(this)->setCurrentScene(underswt);
	CameraContext cc;
	cc.m_fMoveSpeed = 50.0f;
	cc.m_vPosition = osg::Vec3(40315.8f, -78755.8f, 900.0f);
	cc.m_vRotation = osg::Vec3(osg::PI_2,0.0f,-6.33946);
	cc.max_height = 2500;
	cc.min_height = 400;
	cc.keepout  = getKeepOutBorder(0);
	cc.keepin = getKeepInBorder(0);
	cc.flymode = false;
	cc.lowmode = true;
	cc.peng = false;
	cc.m_fAngle = 0.5f;
	cameraContextList[0] = cc;
	TravelManipulator::Instance()->setCameraContext(cameraContextList[0]);

	//LightContext lc;
	//swt->insertChild(0, node, true);
	swt->insertChild(0, underswt, true);

	root->addChild(swt);
}
void ViewerWidget::loadModleThread(int modelnum){
	char num[10];
	for(int i = 1; i< modelnum; i++){
		while(loadFinished == true)
			boost::this_thread::sleep(boost::posix_time::millisec(50));
		_itoa(i, num, 10);
		cout<<"inthread"<<MODELBASE+string(num)+".ive"<<endl;
		threadNode = osgDB::readNodeFile(MODELBASE+string(num)+".ive");
		threadSwt = new osg::Switch();
		threadSwt->insertChild(0, threadNode, true);
		osg::Vec3 centerpos = threadNode->getBound().center();
		CameraContext cc;
		currentIndex = i;
		cc.keepout  = getKeepOutBorder(i);
		cc.keepin = getKeepInBorder(i);
		if(i == 1){
			cc.m_fMoveSpeed = 15.0f;
			cc.m_vPosition = osg::Vec3(380, -7272.73f, -30.0f);
			cc.m_vRotation = osg::Vec3(osg::PI_2,0.0f,-6.26486f);
			cc.max_height = 200;
			cc.min_height = -80;
		}else if(i == 2){//泵站一层
			cc.m_fMoveSpeed = 5.0f;
			cc.m_vPosition = osg::Vec3(-174.813f, -1986.09f, 180.0f);
			cc.m_vRotation = osg::Vec3(osg::PI_2,0.0f,-6.329f);
			cc.max_height = 1000;
			cc.min_height = 45;
			TextPanel* textnode;

			string namehead = "INFO_#";
			osg::Vec4 keypoint;
			for(int i = 0; i< 9; i++){
				namehead = namehead + to_string((long long)i);
				keypoint = cc.keepout->at(i).range;
				textnode = new TextPanel();
				textnode->setDataVariance(osg::Object::DYNAMIC);
				float xpos = keypoint.x() + abs(keypoint.y()-keypoint.x())/2;
				float ypos = (-1)*(keypoint.z() + abs(keypoint.w()-keypoint.z())/2)+50;
				textnode->addYZContent(namehead + "\n", osg::Vec3(xpos, ypos, 200.0), 300, 100);
				textnode->setName(namehead);
				threadSwt->insertChild(i+1, textnode, true);
			}

		}else if(i == 3){
			cc.m_fMoveSpeed = 5.0f;
			cc.m_vPosition = osg::Vec3(-324.813f, -1086.09f, -240.0f);
			cc.m_vRotation = osg::Vec3(osg::PI_2,0.0f,-6.329f);
			cc.max_height = -90;
			cc.min_height = -350;
			//cc.light_Position = osg::Vec4(-500, -3557, -240, 0.0);
		}else if(i == 4){
			cc.m_fMoveSpeed = 5.0f;
			cc.m_vPosition = osg::Vec3(40.813f, -550.09f, -30.0f);
			cc.m_vRotation = osg::Vec3(1.5407,0.0f,0.0f);
			cc.max_height = 70;
			cc.min_height = 0;
		}else if(i == 5){
			cc.m_fMoveSpeed = 2.0f;
			cc.m_vPosition = osg::Vec3(83.3197, -1841.9, 0.0f);
			cc.m_vRotation = osg::Vec3(1.5407,0.0f,0.0f);
			cc.max_height = 70;
			cc.min_height = 0;
		}

		cc.flymode = false;
		cc.lowmode = true;
		cc.peng = false;
		cc.m_fAngle = 0.5f;
		//cc.light_Position = osg::Vec4(centerpos.x(), centerpos.y(), centerpos.z(), 1.0);
		cameraContextList[i] = cc;
		loadFinished = true;
	}
	emit modelLoadFinished();
}


CameraContext ViewerWidget::getCameraContext(int index){
	return cameraContextList.at(index);
}

void ViewerWidget::textInfoSwt(int index, bool showTxt){
	osg::Switch* subswt = swt->getChild(index)->asSwitch();
	int numChd = subswt->getNumChildren();
	if(numChd > 1){
		if(showTxt == true)
			subswt->setAllChildrenOn();
		else{
			subswt->setSingleChildOn(0);
		}
	}
}

vector<RangeNode>* ViewerWidget::getKeepInBorder(int modelindex){
	vector<RangeNode>* ret = new vector<RangeNode>();
	if(modelindex == 0){
		ret->push_back(RangeNode(0, osg::Vec4(-421529.25, 397938.25, -339227.56, 256748.81)));
	}else if(modelindex == 1){
		ret->push_back(RangeNode(0, osg::Vec4(176.417, 394.14, -7278.08, -6160.67)));
	}else if(modelindex == 2){
		ret->push_back(RangeNode(0, osg::Vec4(-1266.5, 306.5, -2485.69, 6885.69)));
	}else if(modelindex == 3){
		ret->push_back(RangeNode(0, osg::Vec4(-501.837, 306.5, -1247.69, 6885.69)));
	}else if(modelindex == 4){
		ret->push_back(RangeNode(0, osg::Vec4(36.8103, 76.6053, -561.673, 851.025)));
	}else if(modelindex == 5){
		ret->push_back(RangeNode(0, osg::Vec4(-107.807, 121.029, -1842.43, 1801.22)));
	}
	return ret;
}

vector<RangeNode>* ViewerWidget::getKeepOutBorder(int modelindex){
	vector<RangeNode>* ret = new vector<RangeNode>();
	if(modelindex == 0){
		ret->push_back(RangeNode(0, osg::Vec4(4647.066, 19877.461, -42102.832, -39354.855)));
		ret->push_back(RangeNode(1, osg::Vec4(-901.902, 4774.132, -41881.059, -39449.918)));
		ret->push_back(RangeNode(2, osg::Vec4(-27511.979, -21784.602, -67398.156, -39283.802)));
		ret->push_back(RangeNode(3, osg::Vec4(-20822.3, -14822.3, -64349.04, -39274.7)));
		ret->push_back(RangeNode(4, osg::Vec4(11176.188, 14578.176, -26729.436, -26052.115)));
		ret->push_back(RangeNode(5, osg::Vec4(19420.225, 22092.951, -31058.322, -27451.885)));
		ret->push_back(RangeNode(6, osg::Vec4(20499.781, 28487.51, -14153.42, -3149.199)));
		ret->push_back(RangeNode(7, osg::Vec4(47000.5, 98848.5, -1926.9, 40826.9)));
		ret->push_back(RangeNode(8, osg::Vec4(25834.207, 37383.184, -63846.973, -53276.723)));
		ret->push_back(RangeNode(9, osg::Vec4(18182.531, 22549.695, -73950.375, -64837.469)));
		ret->push_back(RangeNode(10, osg::Vec4(-37689.965, -29035.395, -68235.962, -64327.445)));
	}else if(modelindex == 1){
	}else if(modelindex == 2){
		ret->push_back(RangeNode(0, osg::Vec4(-256.911, 280.151, -272.057, 300.0)));//由门至远
		ret->push_back(RangeNode(1, osg::Vec4(-256.911, 280.151, 500.0, 1080.0)));
		ret->push_back(RangeNode(2, osg::Vec4(-256.911, 280.151, 1280.0, 1900.0)));
		ret->push_back(RangeNode(3, osg::Vec4(-256.911, 280.151, 2150.0, 2700.54)));
		ret->push_back(RangeNode(4, osg::Vec4(-256.911, 280.151, 2900.54, 3500.54)));
		ret->push_back(RangeNode(5, osg::Vec4(-256.911, 280.151, 3700.54, 4300.54)));
		ret->push_back(RangeNode(6, osg::Vec4(-256.911, 280.151, 4600.54, 5150.54)));
		ret->push_back(RangeNode(7, osg::Vec4(-256.911, 280.151, 5400.54, 5950.54)));
		ret->push_back(RangeNode(8, osg::Vec4(-256.911, 280.151, 6200.54, 6800.54)));//右侧 电机
		ret->push_back(RangeNode(9, osg::Vec4(-1250.6, -750.6, -430.5, 330.5)));//左侧 闸门 由门至远
		ret->push_back(RangeNode(10, osg::Vec4(-1250.6, -750.6, 400.5, 1150.5)));
		ret->push_back(RangeNode(11, osg::Vec4(-1250.6, -750.6, 1200.5, 1950.5)));
		ret->push_back(RangeNode(12, osg::Vec4(-1250.6, -750.6, 2020.5, 2790.5)));
		ret->push_back(RangeNode(13, osg::Vec4(-1250.6, -750.6, 2830.5, 3590.5)));
		ret->push_back(RangeNode(14, osg::Vec4(-1250.6, -750.6, 3620.5, 4390.5)));
		ret->push_back(RangeNode(15, osg::Vec4(-1250.6, -750.6, 4480.5, 5250.5)));
		ret->push_back(RangeNode(16, osg::Vec4(-1250.6, -750.6, 5270.5, 6050.5)));
		ret->push_back(RangeNode(17, osg::Vec4(-1250.6, -750.6, 6060.5, 6850.5)));
	}else if(modelindex == 3){
		//ret = NULL;
	}else if(modelindex == 4){
		//ret = NULL;
	}else if(modelindex == 5){
		ret->push_back(RangeNode(0, osg::Vec4(-67.3615, 75.7484, -1553.51, -1049.4)));
		ret->push_back(RangeNode(1, osg::Vec4(-107.513, -77.9218, -977.048, -922.796)));
		ret->push_back(RangeNode(2, osg::Vec4(-67.3615, 71.7206, -871.702, -365.465)));
		ret->push_back(RangeNode(3, osg::Vec4(-67.3615, 80.257, -182.804, 336.603)));
		ret->push_back(RangeNode(4, osg::Vec4(-106.9, -80.2308, 404.539, 450.121)));
		ret->push_back(RangeNode(5, osg::Vec4(-67.3615, 69.1049, 511.798, 1011.59)));
		ret->push_back(RangeNode(6, osg::Vec4(-107.802, -81.398, 1102.56, 1148.05)));
		ret->push_back(RangeNode(7, osg::Vec4(-67.3615, 71.5234, 1202.85, 1703.84)));
	}
	return ret;
}

map<string, string>* ViewerWidget::generateDBMap(int index){
	map<string, string>* retMap;
	list<map<string, string>*> 
	if(index == 0){
	}else if(index == 1){
	}else if(index == 2){
		retMap = new map<string, string>();
		retMap->insert(pair<string, string>("电压",""));
		retMap->insert(pair<string, string>("电流",""));
		retMap->insert(pair<string, string>("有功功率",""));
	}else if(index == 3){
	}else if(index == 4){
	}else if(index == 5){
	}
	return retMap;
}

void ViewerWidget::paintEvent(QPaintEvent* event){
	frame();
	if(loadFinished == true){
		//swt->insertChild(currentIndex, threadNode, false);
		swt->insertChild(currentIndex, threadSwt, false);
		loadFinished = false;
	}
	if(isActiveWindow() != true){
		TravelManipulator::Instance()->resetStateBits();
		//cout<<"viewerWidget paintevent not active window"<<endl;
	}
}

//bool ViewerWidget::winEvent(MSG * message, long * result){
//	if(message->message == WM_ACTIVATE){
//		TravelManipulator::Instance()->resetStateBits();
//		cout<<"viewervidget activate"<<endl;
//	}
//	if(message->message == WM_KILLFOCUS){
//		TravelManipulator::Instance()->resetStateBits();
//		cout<<"viewervidget killfocus"<<endl;
//	}
//	return false;
//}

