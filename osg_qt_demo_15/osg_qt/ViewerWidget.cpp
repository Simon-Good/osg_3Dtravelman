#include "ViewerWidget.h"
//#ifdef _DEBUG
//#define MODELBASE "../../models/"
//#else
//#define MODELBASE "./models/"
//#endif
ViewerWidget::ViewerWidget(QWidget* parent):   QWidget(parent){
	mparent = parent;
	loadFinished = false;
	swt = new osg::Switch();

	osg::Camera* camera = createCamera( 50, 50, 640, 480 );
	osgViewer::StatsHandler *sh = new osgViewer::StatsHandler();
	sh->setKeyEventTogglesOnScreenStats('t');
	sh->setKeyEventPrintsOutStats('T');
	mainView = new osgViewer::Viewer();
    mainView->setCamera( camera );
    mainView->setSceneData( swt );
    mainView->addEventHandler( sh);
    mainView->setCameraManipulator( TravelManipulator::Instance());
	mainView->addEventHandler(GeneralEventHandler::Instance());
    mainView->setThreadingModel( osgViewer::Viewer::SingleThreaded );
	mainView->setKeyEventSetsDone(0);
        
    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
	gw->getTraits();
    if ( gw )
    {
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget( gw->getGLWidget() );
        setLayout( layout );
    }
	rThread = new RenderThread(this);
	rThread->viewerPtr = mainView;
	rThread->rootswt = swt;
	connect(this, SIGNAL(loadOneSwt(osg::Switch*, int)), rThread, SLOT(loadNewSwt(osg::Switch*, int)));
	connect(rThread, SIGNAL(loadSwtFinished()), this, SLOT(loadNext()));
	rThread->start();	
}

osg::Camera* ViewerWidget::createCamera( int x, int y, int w, int h )
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowDecoration = false;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
	traits->alpha = ds->getMinimumNumAlphaBits();
	traits->stencil = ds->getMinimumNumStencilBits();
	traits->sampleBuffers = ds->getMultiSamples();
	traits->samples = 16;
    
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );
    camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
    camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
    return camera.release();
}

void ViewerWidget::reloadModel(int index){
	osg::Switch::ValueList vl = swt->getValueList();
	int vllen = vl.size(), mark = 0;
	for(mark = 0; mark< vllen; mark++)
		if(vl[mark] == true)
			break;
	cameraContextList[mark] = *TravelManipulator::Instance()->getCameraContext();
	swt->setSingleChildOn(index);
	GeneralEventHandler::Instance()->setDBMap(generateDBMap(index));
	GeneralEventHandler::Instance()->setCurrentScene(swt->getChild(index)->asSwitch(), index);
	TravelManipulator::Instance()->setCameraContext(&cameraContextList[index]);
}

void ViewerWidget::loadModels(int size){
	genRangeNodeList(size);
	cameraContextList = vector<CameraContext>(size);//cameraContextList初始化的语句必须在线程开始之前执行
	thread = boost::thread(&ViewerWidget::loadModleThread, this, size);
	osg::ref_ptr<osg::Node> node;
	osg::ref_ptr<osg::Switch> underswt = new osg::Switch();
	node = osgDB::readNodeFile(MODELPATH+"0.ive");
	underswt->insertChild(0, node, true);
	GeneralEventHandler::Instance()->setCurrentScene(underswt, 0);
	CameraContext *cc = new CameraContext();
	cc->m_fMoveSpeed = 80.0f;
	cc->m_vPosition = osg::Vec3(62373.6, -35979.1, 900.0f);
	cc->m_vRotation = osg::Vec3(1.61923,0.0f,-4.50296);
	cc->max_height = 2500;
	cc->min_height = 400;
	cc->keepout  = getKeepOutBorder(0);
	cc->keepin = getKeepInBorder(0);
	cc->flymode = false;
	cc->lowmode = true;
	cc->peng = false;
	cc->m_fAngle = 0.5f;
	cameraContextList[0] = *cc;
	TravelManipulator::Instance()->setCameraContext(cc);
	GeneralEventHandler::Instance()->setDBMap(generateDBMap(0));
	swt->insertChild(0, underswt, true);	
}
void ViewerWidget::loadModleThread(int modelnum){
	char num[10];
	TextPanel* textnode;
	string namehead;
	osg::Vec4 keypoint;
	for(int i = 1; i< modelnum; i++){
		while(loadFinished == true)
			boost::this_thread::sleep(boost::posix_time::millisec(50));
		_itoa(i, num, 10);
		cout<<"inthread"<<MODELPATH+string(num)+".ive"<<endl;
		threadNode = osgDB::readNodeFile(MODELPATH+string(num)+".ive");
		threadSwt = new osg::Switch();
		threadSwt->insertChild(0, threadNode, true);
		osg::Vec3 centerpos = threadNode->getBound().center();
		CameraContext *cc = new CameraContext();
		cc->keepout  = getKeepOutBorder(i);
		cc->keepin = getKeepInBorder(i);
		if(i == 1){//送水闸
			cc->m_fMoveSpeed = 5.0f;
			cc->m_vPosition = osg::Vec3(380, -7272.73f, -30.0f);
			cc->m_vRotation = osg::Vec3(osg::PI_2,0.0f,-6.26486f);
			cc->max_height = 30;
			cc->min_height = -80;

			namehead = "SONGSHUI_#";
			for(int j = 0; j< 2; j++){
				keypoint = cc->keepout->at(j).range;
				textnode = new TextPanel();
				float xpos = keypoint.x() + abs(keypoint.y()-keypoint.x())/2;
				float ypos = keypoint.z() + abs(keypoint.w()-keypoint.z())/2-50;
				textnode->setMatrix(osg::Matrix::translate(osg::Vec3(xpos, ypos, -20.0)));
				textnode->addContent(30, 15, 3.0);
				textnode->setName(namehead + to_string((long long)j));
				threadSwt->insertChild(j+1, textnode, true);
			}
		}else if(i == 2){//泵房
			cc->m_fMoveSpeed = 15.0f;
			cc->m_vPosition = osg::Vec3(-174.813f, -1986.09f, 180.0f);
			cc->m_vRotation = osg::Vec3(osg::PI_2,0.0f,-6.329f);
			cc->max_height = 1000;
			cc->min_height = 45;
			
			namehead = "BENGFANG_#";
			for(int j = 0; j< 18; j++){
				keypoint = cc->keepout->at(j).range;
				textnode = new TextPanel();
				float xpos = keypoint.x() + abs(keypoint.y()-keypoint.x())/2;
				if(j >= 0 && j< 9){
					float ypos = keypoint.z()+ 150;
					textnode->setMatrix(osg::Matrix::rotate(osg::PI, osg::Vec3(0, 0, 1)) * osg::Matrix::translate(osg::Vec3(xpos, ypos, 300.0)));
					textnode->addContent(300, 135, 12.0);
				}else if(j >= 9 && j< 18){
					float ypos = keypoint.z() - 100;
					textnode->setMatrix(osg::Matrix::translate(osg::Vec3(xpos, ypos, 210.0)));
					textnode->addContent(90, 40, 12.0);
				}
				textnode->setName(namehead + to_string((long long)j));
				threadSwt->insertChild(j+1, textnode, true);
			}

		}else if(i == 3){//连轴层
			cc->m_fMoveSpeed = 5.0f;
			cc->m_vPosition = osg::Vec3(-324.813f, -1086.09f, -240.0f);
			cc->m_vRotation = osg::Vec3(osg::PI_2,0.0f,-6.329f);
			cc->max_height = 900;
			cc->min_height = -350;

			namehead = "LIANZHOU_#";
			keypoint = cc->keepout->at(0).range;
			textnode = new TextPanel();
			float xpos = keypoint.y()-50;
			float ypos = keypoint.w()-50;
			textnode->setMatrix(osg::Matrix::rotate(osg::PI_2, osg::Vec3(0, 0, 1)) * osg::Matrix::translate(osg::Vec3(xpos, ypos, -200.0)));
			textnode->addContent(100, 50, 8.0);
			textnode->setName(namehead + to_string((long long)0));
			threadSwt->insertChild(1, textnode, true);
		}else if(i == 4){//调度闸
			cc->m_fMoveSpeed = 5.0f;
			cc->m_vPosition = osg::Vec3(40.813f, -550.09f, -30.0f);
			cc->m_vRotation = osg::Vec3(1.5407,0.0f,0.0f);
			cc->max_height = 40;
			cc->min_height = 0;

			namehead = "DIAODU_#";
			for(int j = 0; j< 2; j++){
				keypoint = cc->keepout->at(j).range;
				textnode = new TextPanel();
				float xpos = keypoint.x() + abs(keypoint.y()-keypoint.x())/2;
				float ypos = keypoint.z() + abs(keypoint.w()-keypoint.z())/2 -50;
				textnode->setMatrix(osg::Matrix::translate(osg::Vec3(xpos, ypos, -5.0)));
				textnode->addContent(25, 10, 2.0);
				textnode->setName(namehead + to_string((long long)j));
				threadSwt->insertChild(j+1, textnode, true);
			}
		}else if(i == 5){//节制闸
			cc->m_fMoveSpeed = 8.0f;
			cc->m_vPosition = osg::Vec3(83.3197, -1841.9, 0.0f);
			cc->m_vRotation = osg::Vec3(1.5407,0.0f,0.0f);
			cc->max_height = 70;
			cc->min_height = 0;

			namehead = "JIEZHI_#";
			for(int j = 0; j< 3; j++){
				keypoint = cc->keepout->at(j).range;
				textnode = new TextPanel();
				float xpos = keypoint.x() + abs(keypoint.y()-keypoint.x())/2;
				float ypos = keypoint.z() - 22;
				textnode->setMatrix(osg::Matrix::translate(osg::Vec3(xpos, ypos, -5.0)));
				textnode->addContent(20, 12, 2.0);
				textnode->setName(namehead + to_string((long long)j));
				threadSwt->insertChild(j+1, textnode, true);
			}
		}else if(i == 6){
			cc->m_fMoveSpeed = 10.0f;
			cc->m_vPosition = osg::Vec3(-778.774, -2709.9, 890.0f);
			cc->m_vRotation = osg::Vec3(1.5407,0.0f,0.0f);
			cc->max_height = 1700;
			cc->min_height = 20;

			namehead = "ZHANBIANSUOBIAN_#";
			for(int j = 0; j< 4; j++){
				float wid = 200;

				keypoint = cc->keepout->at(j).range;
				textnode = new TextPanel();
				float xpos = keypoint.x()+(keypoint.y() - keypoint.x() - wid);
				float ypos = keypoint.z();
				if(j< 2){
					textnode->setMatrix(osg::Matrix::rotate((-1)*osg::PI_2, osg::Vec3(0, 0 , 1)) * osg::Matrix::translate(osg::Vec3(xpos, ypos, 930.0)));
					textnode->addContent(wid, 60, 12.0);
				}else{
					textnode->setMatrix(osg::Matrix::rotate((-1)*osg::PI_2, osg::Vec3(0, 0 , 1)) * osg::Matrix::translate(osg::Vec3(xpos, ypos, 780.0)));
					textnode->addContent(wid, 40, 12.0);
				}
				textnode->setName(namehead + to_string((long long)j));
				threadSwt->insertChild(j+1, textnode, true);
			}
		}

		cc->flymode = false;
		cc->lowmode = true;
		cc->peng = false;
		cc->m_fAngle = 0.5f;
		cameraContextList[i] = *cc;
		emit loadOneSwt(threadSwt, i);
		loadFinished = true;
	}
	emit modelLoadFinished();
}

void ViewerWidget::loadNext(){
	loadFinished = false;
}

CameraContext* ViewerWidget::getCameraContext(int index){
	return &cameraContextList.at(index);
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
	return keepInList.at(modelindex);
}

vector<RangeNode>* ViewerWidget::getKeepOutBorder(int modelindex){
	return keepOutList.at(modelindex);
}

vector<map<string, string, MyCompRule>*>* ViewerWidget::generateDBMap(int index){
	map<string, string, MyCompRule>* retMap;
	vector<map<string, string, MyCompRule>*>* retVec = new vector<map<string, string, MyCompRule>*>();
	if(index == 0){
		retMap = new map<string, string, MyCompRule>();
		retMap->insert(pair<string, string>("电压","dianya0"));
		retVec->push_back(retMap);
	}else if(index == 1){//送水闸//retVec[map(kaigao1,kaigao2),map(kaigao3)]
		retMap = new map<string, string, MyCompRule>();
		retMap->insert(pair<string, string>("1#闸门开高",""));//*
		retMap->insert(pair<string, string>("2#闸门开高",""));//*
		retMap->insert(pair<string, string>("送水河水位",""));
		retMap->insert(pair<string, string>("调度区水位", ""));
		retVec->push_back(retMap);

		retMap = new map<string, string, MyCompRule>();
		retMap->insert(pair<string, string>("3#闸门开高",""));//*
		retMap->insert(pair<string, string>("送水河水位",""));
		retMap->insert(pair<string, string>("调度区水位", ""));
		retVec->push_back(retMap);
	}else if(index == 2){//泵站
		for(int i = 0; i< 18; i++){
			retMap = new map<string, string, MyCompRule>();
			if(i>=0 && i< 9){
				retMap->insert(pair<string, string>("电压",""));
				retMap->insert(pair<string, string>("电流",""));
				retMap->insert(pair<string, string>("有功功率",""));
				retMap->insert(pair<string, string>("无功功率",""));
				retMap->insert(pair<string, string>("励磁电流",""));

				retMap->insert(pair<string, string>("励磁电压",""));
				retMap->insert(pair<string, string>("主机转速",""));
				retMap->insert(pair<string, string>("定子温度",""));
				retMap->insert(pair<string, string>("上导温度",""));
				retMap->insert(pair<string, string>("下导温度",""));

				retMap->insert(pair<string, string>("上油缸温度",""));
				retMap->insert(pair<string, string>("下油缸温度",""));
				retMap->insert(pair<string, string>("推力瓦温度",""));
				retMap->insert(pair<string, string>("瞬时流量", ""));//*
				retMap->insert(pair<string, string>("实际流量", ""));//*
				retMap->insert(pair<string, string>("瞬时流量类型", ""));//*
				retMap->insert(pair<string, string>("叶片角度",""));
				retMap->insert(pair<string, string>("闸上水位",""));
				if(i< 3)
					retMap->insert(pair<string, string>("调度区水位",""));
				else if(i > 2 && i< 9)
					retMap->insert(pair<string, string>("闸下水位", ""));
			}else if(i>=9 && i< 18){
				retMap->insert(pair<string, string>("A门闸门高度", ""));//*
				retMap->insert(pair<string, string>("A门电机电流", ""));//*
				retMap->insert(pair<string, string>("B门闸门高度", ""));//*
				retMap->insert(pair<string, string>("B门电机电流", ""));//*
			}
			retVec->push_back(retMap);
		}
	}else if(index == 3){//连轴层
		retMap = new map<string, string, MyCompRule>();
		retMap->insert(pair<string, string>("1#供水母管压力",""));//*
		retMap->insert(pair<string, string>("2#供水母管压力",""));//*
		retMap->insert(pair<string, string>("1#供泵状态",""));//*
		retMap->insert(pair<string, string>("2#供泵状态",""));//*
		retMap->insert(pair<string, string>("3#供泵状态",""));//*
		retVec->push_back(retMap);
	}else if(index == 4){//调度闸//retVec[map(kaigao1,kaigao2),map(kaigao3, kaigao4)]
		for(int i = 0; i< 2; i++){
			retMap = new map<string, string, MyCompRule>();
			retMap->insert(pair<string, string>(to_string((long long)(2*i + 1)) + "#闸门开高",""));//*
			retMap->insert(pair<string, string>(to_string((long long)(2*i + 2)) + "#闸门开高",""));//*
			retMap->insert(pair<string, string>(to_string((long long)(2*i + 1)) + "#闸门电机电流",""));//*
			retMap->insert(pair<string, string>(to_string((long long)(2*i + 2)) + "#闸门电机电流",""));//*
			retMap->insert(pair<string, string>("闸下水位",""));
			retMap->insert(pair<string, string>("调度区水位",""));
			retVec->push_back(retMap);
		}
	}else if(index == 5){//节制闸//retVec[map(kaigao1, kaigao2),map(kaigao3,kaigao4), map(kaigao5)]
		for(int i = 0; i< 2; i++){
			retMap = new map<string, string, MyCompRule>();
			retMap->insert(pair<string, string>(to_string((long long)(2*i + 1)) + "#闸门开高",""));//*
			retMap->insert(pair<string, string>(to_string((long long)(2*i + 2)) + "#闸门开高",""));//*
			retMap->insert(pair<string, string>(to_string((long long)(2*i + 1)) + "#闸门电机电流",""));//*
			retMap->insert(pair<string, string>(to_string((long long)(2*i + 2)) + "#闸门电机电流",""));//*
			retMap->insert(pair<string, string>("内河侧水位",""));
			retMap->insert(pair<string, string>("长江侧水位",""));
			retMap->insert(pair<string, string>("实际流量",""));
			retVec->push_back(retMap);
		}
		retMap = new map<string, string, MyCompRule>();
		retMap->insert(pair<string, string>("5#闸门开高",""));//*
		retMap->insert(pair<string, string>("5#闸门电机电流",""));//*
		retMap->insert(pair<string, string>("内河侧水位",""));
		retMap->insert(pair<string, string>("长江侧水位",""));
		retMap->insert(pair<string, string>("实际流量",""));
		retVec->push_back(retMap);
	}else if(index == 6){
		retMap = new map<string, string, MyCompRule>();
		retMap->insert(pair<string, string>("所变高压侧电压",""));
		retMap->insert(pair<string, string>("所变高压侧电流",""));
		retMap->insert(pair<string, string>("所变低压侧电压",""));
		retMap->insert(pair<string, string>("所变低压侧电流",""));
		retVec->push_back(retMap);

		retMap = new map<string, string, MyCompRule>();
		retMap->insert(pair<string, string>("站变高压侧电压",""));
		retMap->insert(pair<string, string>("站变高压侧电流",""));
		retMap->insert(pair<string, string>("站变低压侧电压",""));
		retMap->insert(pair<string, string>("站变低压侧电流",""));
		retVec->push_back(retMap);

		retMap = new map<string, string, MyCompRule>();
		retMap->insert(pair<string, string>("站变铁芯温度",""));
		retVec->push_back(retMap);

		retMap = new map<string, string, MyCompRule>();
		retMap->insert(pair<string, string>("所变铁芯温度",""));
		retVec->push_back(retMap);
	}

	return retVec;
}

void ViewerWidget::genRangeNodeList(int listSize){

	vector<RangeNode>* in ;
	vector<RangeNode>* out;
	for (int i=0;i<listSize;i++){
		if (i==0){
			in = new vector<RangeNode>();
			in->push_back(RangeNode(0, osg::Vec4(-421529.25, 397938.25, -339227.56, 256748.81)));

			out = new vector<RangeNode>();
			out->push_back(RangeNode(0, osg::Vec4(4647.066, 19877.461, -42102.832, -39354.855)));
			out->push_back(RangeNode(1, osg::Vec4(-901.902, 4774.132, -41881.059, -39449.918)));
			out->push_back(RangeNode(2, osg::Vec4(-27511.979, -21784.602, -67398.156, -39283.802)));
			out->push_back(RangeNode(3, osg::Vec4(-20822.3, -14822.3, -64349.04, -39274.7)));
			out->push_back(RangeNode(4, osg::Vec4(11176.188, 14578.176, -26729.436, -26052.115)));
			out->push_back(RangeNode(5, osg::Vec4(19420.225, 22092.951, -31058.322, -27451.885)));
			out->push_back(RangeNode(6, osg::Vec4(20499.781, 28487.51, -14153.42, -3149.199)));
			out->push_back(RangeNode(7, osg::Vec4(47000.5, 98848.5, -1926.9, 40826.9)));
			out->push_back(RangeNode(8, osg::Vec4(25834.207, 37383.184, -63846.973, -53276.723)));
			out->push_back(RangeNode(9, osg::Vec4(18182.531, 22549.695, -73950.375, -64837.469)));
			out->push_back(RangeNode(10, osg::Vec4(-37689.965, -29035.395, -68235.962, -64327.445)));
			out->push_back(RangeNode(11, osg::Vec4(3338.7,42143.2 , -38250.4, -30315.8)));
		}else if(i==1){
			in = new vector<RangeNode>();
			in->push_back(RangeNode(0, osg::Vec4(176.417, 394.14, -7278.08, -6160.67)));

			out = new vector<RangeNode>();
			out->push_back(RangeNode(0, osg::Vec4(160.031, 198.88, -6878.99, -6834.36)));
			out->push_back(RangeNode(1, osg::Vec4(160.031, 198.88, -6529.84, -6487.92)));
		}else if(i==2){
			in = new vector<RangeNode>();
			in->push_back(RangeNode(0, osg::Vec4(-1266.5, 306.5, -2485.69, 6885.69)));

			out = new vector<RangeNode>();
			out->push_back(RangeNode(0, osg::Vec4(-256.911, 280.151, -272.057, 300.0)));//由门至远
			out->push_back(RangeNode(1, osg::Vec4(-256.911, 280.151, 500.0, 1080.0)));
			out->push_back(RangeNode(2, osg::Vec4(-256.911, 280.151, 1280.0, 1900.0)));
			out->push_back(RangeNode(3, osg::Vec4(-256.911, 280.151, 2150.0, 2700.54)));
			out->push_back(RangeNode(4, osg::Vec4(-256.911, 280.151, 2900.54, 3500.54)));
			out->push_back(RangeNode(5, osg::Vec4(-256.911, 280.151, 3700.54, 4300.54)));
			out->push_back(RangeNode(6, osg::Vec4(-256.911, 280.151, 4600.54, 5150.54)));
			out->push_back(RangeNode(7, osg::Vec4(-256.911, 280.151, 5400.54, 5950.54)));
			out->push_back(RangeNode(8, osg::Vec4(-256.911, 280.151, 6200.54, 6800.54)));//右侧 电机
			out->push_back(RangeNode(9, osg::Vec4(-1250.6, -750.6, -430.5, 330.5)));//左侧 闸门 由门至远
			out->push_back(RangeNode(10, osg::Vec4(-1250.6, -750.6, 400.5, 1150.5)));
			out->push_back(RangeNode(11, osg::Vec4(-1250.6, -750.6, 1200.5, 1950.5)));
			out->push_back(RangeNode(12, osg::Vec4(-1250.6, -750.6, 2020.5, 2790.5)));
			out->push_back(RangeNode(13, osg::Vec4(-1250.6, -750.6, 2830.5, 3590.5)));
			out->push_back(RangeNode(14, osg::Vec4(-1250.6, -750.6, 3620.5, 4390.5)));
			out->push_back(RangeNode(15, osg::Vec4(-1250.6, -750.6, 4480.5, 5250.5)));
			out->push_back(RangeNode(16, osg::Vec4(-1250.6, -750.6, 5270.5, 6050.5)));
			out->push_back(RangeNode(17, osg::Vec4(-1250.6, -750.6, 6060.5, 6850.5)));
		}else if (i==3){
			in = new vector<RangeNode>();
			in->push_back(RangeNode(0, osg::Vec4(-501.837, 306.5, -1247.69, 6885.69)));

			out = new vector<RangeNode>();
			out->push_back(RangeNode(0, osg::Vec4(-136.528, 284.223, -1128, -930.987)));
		}else if (i==4){
			in = new vector<RangeNode>();
			in->push_back(RangeNode(0, osg::Vec4(36.8103, 76.6053, -561.673, 851.025)));

			out = new vector<RangeNode>();
			out->push_back(RangeNode(0, osg::Vec4(-159.0, -121.0, -205.634, -154.634)));
			out->push_back(RangeNode(1, osg::Vec4(-159.0, -121.0, 492.634, 536.634)));
		}else if (i==5){
			in = new vector<RangeNode>();
			in->push_back(RangeNode(0, osg::Vec4(-107.807, 121.029, -1842.43, 1801.22)));

			out = new vector<RangeNode>();
			out->push_back(RangeNode(0, osg::Vec4(-120.0, -81.575, -972.88, -928.773)));
			out->push_back(RangeNode(1, osg::Vec4(-120.0, -81.575, 406.407, 447.844)));
			out->push_back(RangeNode(2, osg::Vec4(-120.0, -81.575, 1102.98, 1146.21)));
			out->push_back(RangeNode(3, osg::Vec4(-67.3615, 75.7484, -1553.51, -1049.4)));
			out->push_back(RangeNode(4, osg::Vec4(-107.513, -77.9218, -977.048, -922.796)));
			out->push_back(RangeNode(5, osg::Vec4(-67.3615, 71.7206, -871.702, -365.465)));
			out->push_back(RangeNode(6, osg::Vec4(-67.3615, 80.257, -182.804, 336.603)));
			out->push_back(RangeNode(7, osg::Vec4(-106.9, -80.2308, 404.539, 450.121)));
			out->push_back(RangeNode(8, osg::Vec4(-67.3615, 69.1049, 511.798, 1011.59)));
			out->push_back(RangeNode(9, osg::Vec4(-107.802, -81.398, 1102.56, 1148.05)));
			out->push_back(RangeNode(10, osg::Vec4(-67.3615, 71.5234, 1202.85, 1703.84)));

		}else if (i==6){
			in = new vector<RangeNode>();
			in->push_back(RangeNode(0, osg::Vec4(-1628.09, 514.386, -2881.55, 3273.53)));

			out = new vector<RangeNode>();
			out->push_back(RangeNode(0, osg::Vec4(-986.176, -560.574, -1723.67, -1373.82)));
			out->push_back(RangeNode(1, osg::Vec4(-550.176, -140.834, -1723.67, -1373.82)));
			out->push_back(RangeNode(2, osg::Vec4(-1112.15, -21.4336, -24.3993,678.568)));
			out->push_back(RangeNode(3, osg::Vec4(-1112.15, -21.4336, 1466.82, 2174.4)));

		}else{
			in=NULL;
			out=NULL;
		}

		if (in!=NULL)
			keepInList.push_back(in);
		if (out!=NULL)
			keepOutList.push_back(out);
	}
}
