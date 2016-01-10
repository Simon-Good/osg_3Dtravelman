#include "mainwindow.h"
MyMenu::MyMenu(QWidget* parent): QMenu(parent){}
MyMenu::MyMenu(const QString &title, QWidget *parent): QMenu(title, parent){}
MyMenu::~MyMenu(){}

void MyMenu::mouseReleaseEvent(QMouseEvent *e){
	QAction *action = this->actionAt(e->pos());
    if(action)
    {
        QString strCanHide = action->property("canHideMenu").toString();
        if(strCanHide == "true")
        {
            QMenu::mouseReleaseEvent(e);
            return;
        }
        else
        {
            action->activate(QAction::Trigger);
            return;
        }
    }
    QMenu::mouseReleaseEvent(e);
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event){
	if (QGraphicsItem *item = itemAt(event->pos())) {
         if (MapItem *image = qgraphicsitem_cast<MapItem *>(item)){
			 int d = image->data(0).toInt();
			 if(d == 0){
				 emit itemClicked(osg::Vec3(28332.1, -40417.7, 900), osg::Vec3(1.5132, 0, -4.78175));
			 }
		 }
     }
     QGraphicsView::mouseReleaseEvent(event);
}
MainWindow::MainWindow(WelcomePage* wp, QWidget *parent)
	: QMainWindow(parent)
{
	cs = new CameraSetting(this);
	wp->setValue(5);
	setGeometry(100, 100, 800, 600);
	setWindowTitle(tr("高港闸站工程三维可视化综合管理与决策系统"));
	dbHandler::Get_dbH_Instance();
	wp->setValue(15);
	createAction();
	createMenu();
	createDockWindow();
	createToolBar();
	wp->setValue(35);
	viewWidget = new ViewerWidget(this);
	//viewWidget->setFocusPolicy(Qt::StrongFocus);
	doshow = true;
	//setFocusPolicy(Qt::StrongFocus);
	//connect(this,SIGNAL(QApplication::focusChanged()), this, SLOT(tempfocuschange()));
	wp->setValue(65);
	viewWidget->loadModels(customerList->count());
	wp->setValue(85);
	setCentralWidget(viewWidget);
	connect(viewWidget, SIGNAL(modelLoadFinished()), this, SLOT(enableItems()));
	connect(GeneralEventHandler::Instance(), SIGNAL(selectedPosition(osg::Vec3, float)), this, SLOT(showPos(osg::Vec3, float)));
	connect(GeneralEventHandler::Instance(), SIGNAL(resetDoshow()), this, SLOT(resetDoshowFlag()));
	wp->setValue(95);
	currentItem = customerList->currentItem();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenu(){
	fileMenu = new MyMenu(tr("&文件"),this);
	fileMenu->addAction(openpathAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
	
	editMenu = new MyMenu(tr("&编辑"),this);
	editMenu->addAction(recordpathAct);
	
	viewMenu = new MyMenu(tr("&视图"),this);
	viewMenu->addAction(toolBarViewAct);
	settingMenu = new MyMenu(tr("&设置"), this);
    settingMenu->addAction(interecAct);
	settingMenu->addAction(cameraSettingAct);
	settingMenu->addAction(cameraLowModeAct);
	settingMenu->addAction(cameraFlyModeAct);
	settingMenu->addAction(textInfoAct);

	helpMenu = new MyMenu(tr("&帮助"), this);
	helpMenu->addAction(howtoAct);
    helpMenu->addAction(aboutAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(editMenu);
	menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(settingMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createAction(){
	openpathActTB = new QAction(QIcon("./open.png"),tr("&打开路径"), this);
	openpathAct = new QAction(tr("&打开路径"), this);
	connect(openpathActTB, SIGNAL(triggered()), this, SLOT(playCameraPath()));
	connect(openpathAct, SIGNAL(triggered()), this, SLOT(playCameraPath()));
	recordpathActTB = new QAction(QIcon("./recordpath.png"), tr("&开始记录路径"), this);
	recordpathAct = new QAction(tr("&开始记录路径"), this);
	connect(recordpathAct, SIGNAL(triggered()), this, SLOT(recordCameraPath()));
	connect(recordpathActTB, SIGNAL(triggered()), this, SLOT(recordCameraPath()));
	
	cameraSettingActTB = new QAction(QIcon("./camera_setting.png"),tr("&视点参数设置"), this);
	cameraSettingAct = new QAction(tr("&视点参数设置"), this);
	connect(cameraSettingActTB, SIGNAL(triggered()), this, SLOT(showCameraSettingDialog()));
	connect(cameraSettingAct, SIGNAL(triggered()), this, SLOT(showCameraSettingDialog()));

	cameraLowModeAct = new QAction(tr("&贴地模式视点"), this);
	cameraLowModeActTB = new QAction(QIcon("./lowmode.png"),tr("&贴地模式视点"), this);
	cameraLowModeAct->setCheckable(true);
	cameraLowModeAct->setChecked(true);
	cameraLowModeActTB->setCheckable(true);
	cameraLowModeActTB->setChecked(true);
	connect(cameraLowModeAct, SIGNAL(triggered()), this, SLOT(setCameraLowMode()));
	connect(cameraLowModeActTB, SIGNAL(triggered()), this, SLOT(setCameraLowMode()));

	cameraFlyModeAct = new QAction(tr("&鸟瞰模式视点"), this);
	cameraFlyModeActTB = new QAction(QIcon("./flymode.png"),tr("&鸟瞰模式视点"), this);
	cameraFlyModeAct->setCheckable(true);
	cameraFlyModeAct->setChecked(false);
	cameraFlyModeActTB->setCheckable(true);
	cameraFlyModeActTB->setChecked(false);
	connect(cameraFlyModeAct, SIGNAL(triggered()), this, SLOT(setCameraFlyMode()));
	connect(cameraFlyModeActTB, SIGNAL(triggered()), this, SLOT(setCameraFlyMode()));
	
	exitAct = new QAction(tr("&退出"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	interecActTB = new QAction(QIcon("./peng_closed.png"), tr("&碰撞检测"), this);
	interecAct = new QAction(tr("&碰撞检测"), this);
    interecAct->setShortcut(tr("Ctrl+I"));
	interecAct->setCheckable(true);
	interecActTB->setCheckable(true);
    connect(interecAct, SIGNAL(triggered()), this, SLOT(open_peng()));
	connect(interecActTB, SIGNAL(triggered()), this, SLOT(open_peng()));

	toolBarViewAct = new QAction(tr("工具栏"), this);
	toolBarViewAct->setCheckable(true);
	toolBarViewAct->setChecked(true);
	connect(toolBarViewAct, SIGNAL(triggered()), this, SLOT(toolbarSwitch()));

	textInfoAct = new QAction(tr("实时数据"), this);
	textInfoAct->setCheckable(false);
	textInfoAct->setChecked(false);
	textInfoAct->setEnabled(false);
	connect(textInfoAct, SIGNAL(triggered()), this, SLOT(textInfoSwitch()));

    aboutAct = new QAction(tr("&关于"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	howtoAct = new QAction(tr("&使用方法"), this);
	connect(howtoAct, SIGNAL(triggered()), this, SLOT(control()));
}

void MainWindow::createToolBar(){
	 //fileToolBar = addToolBar(tr("文件"));
	 //fileToolBar->addAction(openpathActTB);

	 editToolBar=addToolBar(tr("编辑"));
	 editToolBar->addAction(openpathActTB);
	 editToolBar->addAction(recordpathActTB);

	 settingToolBar=addToolBar(tr("设置"));
	 settingToolBar->addAction(interecActTB);
	 settingToolBar->addAction(cameraSettingActTB);
	 settingToolBar->addAction(cameraLowModeActTB);
	 settingToolBar->addAction(cameraFlyModeActTB);
}

void MainWindow::createDockWindow(){
	 QDockWidget *dock = new QDockWidget(tr("导航窗口"), this);
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     customerList = new QListWidget(dock);
     customerList->addItems(QStringList()
             << ">高港闸站"
             << ">送水闸"
			 << ">泵房"
			 << ">连轴层"
			 << ">调度闸"
			 << ">节制闸");
	 customerList->setCurrentRow(0);
	 int num = customerList->count();
	 for(int i = 0; i< num; i++){
		 QListWidgetItem* item = customerList->item(i);
		 item->setFlags(Qt::NoItemFlags);
	 }
	
     dock->setWidget(customerList);
	 this->setContextMenuPolicy(Qt::NoContextMenu);
     addDockWidget(Qt::LeftDockWidgetArea, dock);
     viewMenu->addAction(dock->toggleViewAction());

	 mapdock = new QDockWidget(tr("地图窗口"), this);
     mapdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	 QGraphicsScene *scene = new QGraphicsScene(0, 0, 400, 300);
	 MapItem* mapitem = new MapItem(QPixmap("./open.png"));
	 mapitem->setData(0, 0);
	 mapitem->setPos(20, 10);
	 mapitem->setToolTip(QString(tr("管理站")));
	 scene->addItem(mapitem);
	 MyGraphicsView *view = new MyGraphicsView(scene);
	 view->setRenderHint(QPainter::Antialiasing);
	 view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	 QPixmap pixmap = QPixmap("./background2.bmp").scaled(400, 300);
	 view->setBackgroundBrush(pixmap);
	 connect(view, SIGNAL(itemClicked(osg::Vec3&, osg::Vec3&)), this, SLOT(setCameraToPosition(osg::Vec3&, osg::Vec3&)));
	 mapdock->setWidget(view);
	 addDockWidget(Qt::LeftDockWidgetArea, mapdock);
	 viewMenu->addAction(mapdock->toggleViewAction());
}

void  MainWindow::open_peng(){
	CameraContext *cc = TravelManipulator::Instance()->getCameraContext();
	if(cc->peng == true && (!interecAct->isChecked())){
		interecActTB->setIcon(QIcon("./peng_closed.png"));
		interecActTB->setChecked(false);
		cc->peng = false;
	}else if(cc->peng == true && (!interecActTB->isChecked())){
		interecActTB->setIcon(QIcon("./peng_closed.png"));
		interecAct->setChecked(false);
		cc->peng = false;
	}else if(cc->peng == false && (interecAct->isChecked())){
		interecActTB->setIcon(QIcon("./peng_open.png"));
		interecActTB->setChecked(true);
		cc->peng = true;
	}else if(cc->peng == false && (interecActTB->isChecked())){
		interecActTB->setIcon(QIcon("./peng_open.png"));
		interecAct->setChecked(true);
		cc->peng = true;
	}
	TravelManipulator::Instance()->setCameraContext(cc);
}

void MainWindow::toolbarSwitch(){
	if(toolBarViewAct->isChecked()){
		//fileToolBar->show();
		editToolBar->show();
		settingToolBar->show();
	}else{
		//fileToolBar->hide();
		editToolBar->hide();
		settingToolBar->hide();
	}
}

void MainWindow::about(){
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("<b>关于</b>"));
	msgBox.setText(tr("<b>厂区漫游系统</b>"));
	msgBox.setInformativeText(tr("	可以让你在虚拟的厂区和工作间之中自由浏览。<br> "
								"  版本号：V1.0<br>"
								  "开发单位：河海大学<br>"));
	msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
	int ret = msgBox.exec();
}

void MainWindow::control(){
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("<b>使用方法</b>"));
	msgBox.setText(tr("<b>操作说明</b>"));
	msgBox.setInformativeText(tr("摄像机前后左右平移：W、S、A、D键<br> "
					 "摄像机上下平移：Q、E键<br> "
					 "摄像机转动：拖动鼠标右键<br> "
					 "选中建筑物：拖动鼠标左键<br> "
					 "放大/缩小：鼠标滚轮滚动<br> "));
	msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
	int ret = msgBox.exec();
}

void MainWindow::changeModel(QListWidgetItem* item){
	currentItem = item;
	viewWidget->reloadModel(customerList->row(item));
	CameraContext *cc = viewWidget->getCameraContext(customerList->row(item));
	if(customerList->row(item) == 0){
		cameraFlyModeAct->setDisabled(false);
		cameraFlyModeAct->setChecked(cc->flymode);
		cameraLowModeAct->setChecked(cc->lowmode);
		cameraFlyModeActTB->setDisabled(false);
		cameraFlyModeActTB->setChecked(cc->flymode);
		cameraLowModeActTB->setChecked(cc->lowmode);
		GeneralEventHandler::Instance(this)->infoEnable(true);
		textInfoAct->setDisabled(true);
		textInfoAct->setCheckable(false);
		textInfoAct->setChecked(false);
		mapdock->show();
	}else{
		cameraFlyModeAct->setDisabled(true);
		cameraFlyModeAct->setChecked(false);
		cameraLowModeAct->setChecked(true);
		cameraFlyModeActTB->setDisabled(true);
		cameraFlyModeActTB->setChecked(false);
		cameraLowModeActTB->setChecked(true);
		if(customerList->row(item) == 2){
			GeneralEventHandler::Instance(this)->infoEnable(true);
			textInfoAct->setDisabled(false);
			textInfoAct->setCheckable(true);
			textInfoAct->setChecked(true);
		}else{
			GeneralEventHandler::Instance(this)->infoEnable(false);
			textInfoAct->setDisabled(true);
			textInfoAct->setCheckable(false);
			textInfoAct->setChecked(false);
		}
		mapdock->hide();
	}
	//if(textInfoAct->isChecked() == true)
	//	cout<<"change model and textinfo is checked"<<endl;
	//else
	//	cout<<"change model and textinfo is unchecked"<<endl;
	if(cs->isVisible())
		updateCameraSetting(cc);
}

void MainWindow::showCameraSettingDialog(){
	cs->showWithContext();
}

void MainWindow::updateCameraSetting(CameraContext* cc){
	cs->showWithContext(cc);
}

void MainWindow::setCameraFlyMode(){
	CameraContext* cc = TravelManipulator::Instance()->getCameraContext();
	if(cc->flymode == false && cc->lowmode ==true){
		cc->flymode = true;
		cc->lowmode = false;
		cameraLowModeAct->setChecked(false);
		cameraLowModeActTB->setChecked(false);
		if(cameraFlyModeAct->isChecked() != cc->flymode){
			cameraFlyModeAct->setChecked(cc->flymode);
		}else{
			cameraFlyModeActTB->setChecked(cc->flymode);
		}
		cc->m_vPosition += osg::Vec3(0.0f, 0.0f, 10000.0f);
		cc->m_vRotation._v[0] = osg::DegreesToRadians(75.0f);
	}else if(cc->flymode == true){
		cameraFlyModeAct->setChecked(true);
		cameraFlyModeActTB->setChecked(true);
	}

	cc->max_height = 18000;
	cc->min_height = 8000;
	TravelManipulator::Instance()->setCameraContext(cc);
}

void MainWindow::setCameraLowMode(){
	CameraContext *cc = TravelManipulator::Instance()->getCameraContext();
	int index = customerList->row(currentItem);
	if(cc->lowmode == false && cc->flymode ==true){
		cc->flymode = false;
		cc->lowmode = true;
		cameraFlyModeAct->setChecked(false);
		cameraFlyModeActTB->setChecked(false);
		if(cameraLowModeAct->isChecked() != cc->lowmode){
			cameraLowModeAct->setChecked(cc->lowmode);
		}else{
			cameraLowModeActTB->setChecked(cc->lowmode);
		}
		cc->m_vPosition += osg::Vec3(0.0f, 0.0f, 900.0f-cc->m_vPosition.z());
		cc->m_vRotation._v[0] = osg::DegreesToRadians(90.0f);
	}else if(cc->lowmode == true){
		cameraLowModeAct->setChecked(true);
		cameraLowModeActTB->setChecked(true);
	}
	
	if(index == 0){
		cc->max_height = 2500;
		cc->min_height = 450;
	}else if(index == 1){
		cc->max_height = 500;
		cc->min_height = -20;
	}else if(index == 2){
		cc->max_height = 1000;
		cc->min_height = 45;
	}else if(index == 3){
		cc->max_height = -90;
		cc->min_height = -300;
	}else if(index == 4){
		cc->max_height = 70;
		cc->min_height = 0;
	}else if(index == 5){
		cc->max_height = 70;
		cc->min_height = 0;
	}
	TravelManipulator::Instance()->setCameraContext(cc);
}

void MainWindow::showPos(osg::Vec3 pos, float radius){
	vector<RangeNode>::iterator itr;
	vector<RangeNode>* vec = TravelManipulator::Instance()->cc->keepout;
	int index = -1;
	if(vec->size() > 0){
		for(itr = vec->begin(); itr != vec->end(); itr++){
			if( (itr->range.x() < pos.x()&& pos.x() < itr->range.y())&&
				(itr->range.z() < pos.y()&& pos.y() < itr->range.w())&&
				(radius <= caculateRadius(itr->range))){
					index = itr->index;
					break;
			}
		}
		if(index != -1 && doshow == true){
			QString content;
			QString title;
			if(customerList->row(currentItem) == 0){
				switch (index){
				case 0:
					{
						content = tr(
									"装有9台立式开敞式轴流泵，叶轮直径3米，单机流量34立方米每秒。<br>"
									"配九台2000千瓦立式同步电机，设置4扇平面钢闸门，可正反抽水300 m3/s。");
						title = tr("泵站");
					}
					break;
				case 1:
					{
						content = tr(
									"共5孔，每孔净宽10米，配弧型钢闸门，设计流量440 m3/s");
						title = tr("节制闸");
					}
					break;
				case 2:
					{
						content = tr(
									"闸室长196米，净宽16米，坎上水深3.5米，<br>"
									"可通行千吨级船队。<br>");
						title = tr("船闸");
					}
					break;
				case 3:
					{
						content = tr(
									"闸室长196米，净宽16米，坎上水深3.5米，<br>"
									"可通行千吨级船队。<br>");
						title = tr("船闸");
					}
					break;
				case 4:
					{
						content = tr(
									"共4孔，总净宽20米，设计流量100 m3/s，配平面钢闸门。");
						title = tr("调度闸");
					}
					break;
				case 5:
					{
						content = tr(
									"共3孔，总净宽16.5米，设计流量100m3/s。");
						title = tr("送水闸");
					}
					break;
				default: return;
				}
			}else if(customerList->row(currentItem) == 2){
				if(index > 8 && index < 18){//左侧闸门
					content = tr(
								"每台水泵机组上、下游上下层流道进出口各设一道钢质平面快速<br>"
								"闸门，采用QPK型卷扬式快速启闭机控制，每台机组共4扇。");
					QString num = QString::number(index-8, 10);
					title = num + tr("#闸门");
				}else if(index > -1 && index < 3){
					QString num = QString::number(index+1, 10);
					title = num + tr("#水泵机组");
					content = tr(
								"水泵型号：3000ZLB35-4    生产厂家：无锡水泵厂<br>"
								"设计流量：35m3/s    设计扬程：4m    泵效率：89%<br>"
								"配用功率：2000kW    水泵转速：150r/min<br>"
								"水泵叶轮直径：3m    叶片数：3枚<br>"
								"电机为TL2000—40/3250型2000 kW立式同步电动机。");
				}else if(index > 2 && index < 9){
					QString num = QString::number(index+1, 10);
					title = num + tr("#水泵机组");
					content = tr(
								"水泵型号：3000ZLQ35-4    生产厂家：无锡水泵厂<br>"
								"流量范围：26～42 m3/s    扬程范围：0～5.2m<br>"
								"泵效率范围：75～89%<br>"
								"配用功率：2000kW     水泵转速：150r/min <br>"
								"水泵叶轮直径：3m   叶片数：3枚<br>"
								"电机为TL2000—40/3250型2000 kW立式同步电动机。");
				}else
					return;
			}
			QMessageBox msgBox;
			msgBox.setWindowTitle(title);
			msgBox.setText(content);
			msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
			int ret = msgBox.exec();
			doshow = false;
		}
	}

}

void MainWindow::resetDoshowFlag(){
	doshow = true;
}

float MainWindow::caculateRadius(osg::Vec4& points){
	return sqrt(pow((double)(points.w() - points.z()), 2.0) + pow((double)(points.y() - points.x()), 2.0));
}

void MainWindow::enableItems(){
	QListWidgetItem* item;
	for(int i = 0; i< customerList->count(); i++){
		item = customerList->item(i);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	connect(customerList, SIGNAL(itemClicked( QListWidgetItem *)),this, SLOT(changeModel(QListWidgetItem*)));
}

void MainWindow::playCameraPath(){
	if(TravelManipulator::Instance()->getPlayPath() == false){
		string openpath = QFileDialog::getOpenFileName(this, tr("打开路径"),"./path", tr("*.path")).toStdString();
		TravelManipulator::Instance()->setPlayPath(true, openpath);
	}
}

void MainWindow::recordCameraPath(){
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("记录路径"));
	if(TravelManipulator::Instance()->getRecordPath() == false){
		msgBox.setText(tr("是否开始记录路径？"));
		msgBox.addButton(tr("是"), QMessageBox::AcceptRole);//ret = 0
		msgBox.addButton(tr("否"), QMessageBox::RejectRole);//ret = 1
		int ret = msgBox.exec();
		if(ret == 0){
			this->recordpathActTB->setIcon(QIcon("./pause.png"));
			this->recordpathActTB->setText(tr("暂停记录路径"));
			this->recordpathAct->setText(tr("暂停记录路径"));
			TravelManipulator::Instance()->setRecordPath(true);
		}
	}else if(TravelManipulator::Instance()->getRecordPath() == true){
		msgBox.setText(tr("是否停止记录路径？"));
		msgBox.addButton(tr("是"), QMessageBox::AcceptRole);//ret = 0
		msgBox.addButton(tr("否"), QMessageBox::RejectRole);//ret = 1
		int ret = msgBox.exec();
		if(ret == 0){
			string savepath = QFileDialog::getSaveFileName(this, tr("保存路径"),"./path", tr("*.path")).toStdString();
			string ext = savepath.substr(savepath.rfind('.')+1, savepath.length());
			if(ext.compare("path") !=0){//useless, only for safe
				QMessageBox::information(this, tr("请重新选择保存名称"), tr("路径文件只接受\".path\"扩展名，请重新保存路径。"));
			}else{
				recordpathActTB->setIcon(QIcon("./recordpath.png"));
				this->recordpathActTB->setText(tr("开始记录路径"));
				this->recordpathAct->setText(tr("开始记录路径"));
				TravelManipulator::Instance()->setRecordPath(false, savepath);
			}
		}
	}
}

void MainWindow::textInfoSwitch(){
	if(textInfoAct->isEnabled() == true){
		if(textInfoAct->isChecked() == true){
			viewWidget->textInfoSwt(customerList->row(currentItem), true);
			textInfoAct->setChecked(true);
		}else if(textInfoAct->isChecked() == false){
			viewWidget->textInfoSwt(customerList->row(currentItem), false);
			textInfoAct->setChecked(false);
		}
	}
}

void MainWindow::setCameraToPosition(osg::Vec3& trans, osg::Vec3& rot){
	CameraContext *cc = TravelManipulator::Instance()->getCameraContext();
	cc->m_vPosition = trans;
	cc->m_vRotation = rot;
	TravelManipulator::Instance()->setCameraContext(cc);
}

bool MainWindow::winEvent(MSG * message, long * result){
	if(message->message == WM_ACTIVATE){
		TravelManipulator::Instance()->switchForbidMove();
	}
	return false;
}