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
			 if(d == 0){//kongzhilou
				 emit itemClicked(osg::Vec3(28332.1, -40417.7, 900), osg::Vec3(1.5132, 0, -4.78175));
			 }else if(d == 1){//diaoduzha
				 emit itemClicked(osg::Vec3(17458.7, -25214.2, 900), osg::Vec3(1.48702, 0, -4.36944));
			 }else if(d == 2){//bengzhan
				 emit itemClicked(osg::Vec3(19640.7, -38137.6, 900), osg::Vec3(1.63756, 0, -4.49249));
			 }else if(d == 3){//biandianzhan
				 emit itemClicked(osg::Vec3(30467.3, -27406.9, 900), osg::Vec3(1.49488, 0, -2.11273));
			 }else if(d == 4){//jiezhizha
				 emit itemClicked(osg::Vec3(6641.6, -38446.6, 900), osg::Vec3(1.46346, 0, -4.32625));
			 }else if(d == 5){//songshuizha
				 emit itemClicked(osg::Vec3(21120.8, -34238.3, 900), osg::Vec3(1.47917, 0, -6.21512));
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
	DBHandler::Get_dbH_Instance();
	wp->setValue(15);
	createAction();
	createMenu();
	createDockWindow();
	createToolBar();
	wp->setValue(35);
	viewWidget = new ViewerWidget(this);
	//doshow = true;
	wp->setValue(65);
	viewWidget->loadModels(customerList->count());
	wp->setValue(85);
	setCentralWidget(viewWidget);
	connect(viewWidget, SIGNAL(modelLoadFinished()), this, SLOT(enableItems()));
	connect(GeneralEventHandler::Instance(), SIGNAL(selectedPosition(float, float, float)), this, SLOT(showPos(float, float, float)));
	//connect(GeneralEventHandler::Instance(), SIGNAL(resetDoshow()), this, SLOT(resetDoshowFlag()));
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
	openpathActTB = new QAction(QIcon(string(RESOURCESPATH +"open.png").c_str()),tr("&打开路径"), this);
	openpathAct = new QAction(tr("&打开路径"), this);
	connect(openpathActTB, SIGNAL(triggered()), this, SLOT(playCameraPath()));
	connect(openpathAct, SIGNAL(triggered()), this, SLOT(playCameraPath()));
	recordpathActTB = new QAction(QIcon(string(RESOURCESPATH +"recordpath.png").c_str()), tr("&开始记录路径"), this);
	recordpathAct = new QAction(tr("&开始记录路径"), this);
	connect(recordpathAct, SIGNAL(triggered()), this, SLOT(recordCameraPath()));
	connect(recordpathActTB, SIGNAL(triggered()), this, SLOT(recordCameraPath()));
	
	cameraSettingActTB = new QAction(QIcon(string(RESOURCESPATH +"camera_setting.png").c_str()),tr("&视点参数设置"), this);
	cameraSettingAct = new QAction(tr("&视点参数设置"), this);
	connect(cameraSettingActTB, SIGNAL(triggered()), this, SLOT(showCameraSettingDialog()));
	connect(cameraSettingAct, SIGNAL(triggered()), this, SLOT(showCameraSettingDialog()));

	cameraLowModeAct = new QAction(tr("&贴地模式视点"), this);
	cameraLowModeActTB = new QAction(QIcon(string(RESOURCESPATH + "lowmode.png").c_str()),tr("&贴地模式视点"), this);
	cameraLowModeAct->setCheckable(true);
	cameraLowModeAct->setChecked(true);
	cameraLowModeActTB->setCheckable(true);
	cameraLowModeActTB->setChecked(true);
	connect(cameraLowModeAct, SIGNAL(triggered()), this, SLOT(setCameraLowMode()));
	connect(cameraLowModeActTB, SIGNAL(triggered()), this, SLOT(setCameraLowMode()));

	cameraFlyModeAct = new QAction(tr("&鸟瞰模式视点"), this);
	cameraFlyModeActTB = new QAction(QIcon(string(RESOURCESPATH + "flymode.png").c_str()),tr("&鸟瞰模式视点"), this);
	cameraFlyModeAct->setCheckable(true);
	cameraFlyModeAct->setChecked(false);
	cameraFlyModeActTB->setCheckable(true);
	cameraFlyModeActTB->setChecked(false);
	connect(cameraFlyModeAct, SIGNAL(triggered()), this, SLOT(setCameraFlyMode()));
	connect(cameraFlyModeActTB, SIGNAL(triggered()), this, SLOT(setCameraFlyMode()));
	
	exitAct = new QAction(tr("&退出"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	interecActTB = new QAction(QIcon(string(RESOURCESPATH + "peng_closed.png").c_str()), tr("&碰撞检测"), this);
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
	 dock->setMaximumWidth(320);
	 dock->setMinimumWidth(300);
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     customerList = new QListWidget(dock);
     customerList->addItems(QStringList()
             << ">高港闸站"
             << ">送水闸"
			 << ">泵房"
			 << ">连轴层"
			 << ">调度闸"
			 << ">节制闸"
			 << ">站变室");
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
	 mapdock->setMaximumHeight(450);
	 mapdock->setMinimumHeight(400);
	 mapdock->setMaximumWidth(350);
	 mapdock->setMinimumWidth(300);
     mapdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	 QGraphicsScene *scene = new QGraphicsScene(0, 0, 300, 400);

	 MapItem* mapitem1 = new MapItem(QPixmap(string(RESOURCESPATH + "kongzhilou.png").c_str()).scaled(36.5,29));
	 mapitem1->setData(0, 0);
	 mapitem1->setPos(83, 171);
	 mapitem1->setToolTip(QString(tr("控制楼")));
	 scene->addItem(mapitem1);

	 MapItem* mapitem2 = new MapItem(QPixmap(string(RESOURCESPATH + "diaoduzha.png").c_str()).scaled(13,33));
	 mapitem2->setData(0, 1);
	 mapitem2->setPos(226,122);
	 mapitem2->setToolTip(QString(tr("调度闸")));
	 scene->addItem(mapitem2);
	 
	 MapItem* mapitem3 =new MapItem(QPixmap(string(RESOURCESPATH + "bengzhan.png").c_str()).scaled(36,105));
	 mapitem3->setData(0, 2);
	 mapitem3->setPos(82,66);
	 mapitem3->setToolTip(QString(tr("泵站")));
	 scene->addItem(mapitem3);
	  
	 MapItem* mapitem4=new MapItem(QPixmap(string(RESOURCESPATH + "biandianzhan.png").c_str()).scaled(68,67.5));
	 mapitem4->setData(0, 3);
	 mapitem4->setPos(134,316.5);
	 mapitem4->setToolTip(QString(tr("变电站")));
	 scene->addItem(mapitem4);

	 MapItem* mapitem5=new MapItem(QPixmap(string(RESOURCESPATH + "jiezhizha.png").c_str()).scaled(25,51));
	 mapitem5->setData(0, 4);
	 mapitem5->setPos(91,14);
	 mapitem5->setToolTip(QString(tr("节制闸")));
	 scene->addItem(mapitem5);

	 MapItem* mapitem6=new MapItem(QPixmap(string(RESOURCESPATH + "songshuizha.png").c_str()).scaled(43.5,39.5));
	 mapitem6->setData(0, 5);
	 mapitem6->setPos(180.5,187.5);
	 mapitem6->setToolTip(QString(tr("送水闸")));
	 scene->addItem(mapitem6);

	 QGraphicsPixmapItem* backgrounditem = new QGraphicsPixmapItem(QPixmap(string(RESOURCESPATH + "map.png").c_str()).scaled(300,400));
	 backgrounditem->setPos(0,0);
	 backgrounditem->setZValue(-1);
	 scene->addItem(backgrounditem);
	 MyGraphicsView *view = new MyGraphicsView(scene);
	 view->setRenderHint(QPainter::Antialiasing);
	 view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

	 connect(view, SIGNAL(itemClicked(osg::Vec3&, osg::Vec3&)), this, SLOT(setCameraToPosition(osg::Vec3&, osg::Vec3&)));
	 mapdock->setWidget(view);
	 addDockWidget(Qt::LeftDockWidgetArea, mapdock);
	 viewMenu->addAction(mapdock->toggleViewAction());
}

void  MainWindow::open_peng(){
	CameraContext *cc = TravelManipulator::Instance()->getCameraContext();
	if(cc->peng == true && (!interecAct->isChecked())){
		interecActTB->setIcon(QIcon(string(RESOURCESPATH + "peng_closed.png").c_str()));
		interecActTB->setChecked(false);
		cc->peng = false;
	}else if(cc->peng == true && (!interecActTB->isChecked())){
		interecActTB->setIcon(QIcon(string(RESOURCESPATH + "peng_closed.png").c_str()));
		interecAct->setChecked(false);
		cc->peng = false;
	}else if(cc->peng == false && (interecAct->isChecked())){
		interecActTB->setIcon(QIcon(string(RESOURCESPATH + "peng_open.png").c_str()));
		interecActTB->setChecked(true);
		cc->peng = true;
	}else if(cc->peng == false && (interecActTB->isChecked())){
		interecActTB->setIcon(QIcon(string(RESOURCESPATH + "peng_open.png").c_str()));
		interecAct->setChecked(true);
		cc->peng = true;
	}
	TravelManipulator::Instance()->setCameraContext(cc);
}

void MainWindow::toolbarSwitch(){
	if(toolBarViewAct->isChecked()){
		editToolBar->show();
		settingToolBar->show();
	}else{
		editToolBar->hide();
		settingToolBar->hide();
	}
}

void MainWindow::about(){
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("<b>关于</b>"));
	msgBox.setText(tr("<b>厂区漫游系统</b>"));
	msgBox.setInformativeText(tr("	可以让你在虚拟的厂区和工作间之中自由巡览。<br> "
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
					 "选中物体查看接受：点击鼠标左键<br> "
					 "放大/缩小：鼠标滚轮滚动<br> "));
	msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
	int ret = msgBox.exec();
}

void MainWindow::changeModel(QListWidgetItem* item){
	currentItem = item;
	viewWidget->reloadModel(customerList->row(item));
	CameraContext *cc = viewWidget->getCameraContext(customerList->row(item));
	if(customerList->row(item) == 0){
		cameraFlyModeAct->setEnabled(true);
		cameraFlyModeAct->setCheckable(true);
		cameraFlyModeAct->setChecked(cc->flymode);

		cameraLowModeAct->setEnabled(true);
		cameraLowModeAct->setCheckable(true);
		cameraLowModeAct->setChecked(cc->lowmode);

		cameraFlyModeActTB->setEnabled(true);
		cameraFlyModeActTB->setChecked(cc->flymode);
		cameraLowModeActTB->setEnabled(true);
		cameraLowModeActTB->setChecked(cc->lowmode);
		
		GeneralEventHandler::Instance()->infoEnable(true);
		textInfoAct->setDisabled(false);
		textInfoAct->setCheckable(true);
		textInfoAct->setChecked(true);

		mapdock->toggleViewAction()->setDisabled(false);
		mapdock->toggleViewAction()->setCheckable(true);
		mapdock->toggleViewAction()->setChecked(true);
		mapdock->show();
	}else{
		cameraFlyModeAct->setEnabled(false);
		cameraFlyModeAct->setChecked(false);
		cameraFlyModeAct->setCheckable(false);

		cameraLowModeAct->setEnabled(true);
		cameraLowModeAct->setChecked(true);
		cameraLowModeAct->setCheckable(true);

		cameraFlyModeActTB->setEnabled(false);
		cameraFlyModeActTB->setChecked(false);
		cameraFlyModeActTB->setCheckable(false);

		cameraLowModeActTB->setEnabled(true);
		cameraLowModeActTB->setChecked(true);
		cameraLowModeActTB->setCheckable(true);
		if(customerList->row(item) == 2 ||customerList->row(item) == 1 
			|| customerList->row(item) == 4 ||customerList->row(item) == 5
			|| customerList->row(item) == 6){
			textInfoAct->setDisabled(false);
			textInfoAct->setCheckable(true);
			textInfoAct->setChecked(true);
		}else{
			textInfoAct->setDisabled(true);
			textInfoAct->setCheckable(false);
			textInfoAct->setChecked(false);
		}
		GeneralEventHandler::Instance()->infoEnable(true);

		mapdock->toggleViewAction()->setDisabled(true);
		mapdock->toggleViewAction()->setCheckable(false);
		mapdock->toggleViewAction()->setChecked(false);
		mapdock->hide();
	}
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
	if(cameraFlyModeAct->isEnabled()){
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

void MainWindow::showPos(float x, float y, float radius){
	vector<RangeNode>::iterator itr;
	vector<RangeNode>* vec = TravelManipulator::Instance()->cc->keepout;
	int index = -1;
	if(vec->size() > 0){
		for(itr = vec->begin(); itr != vec->end(); itr++){
			if( (itr->range.x() < x&& x < itr->range.y())&&
				(itr->range.z() < y&& y < itr->range.w())&&
				(radius <= caculateRadius(itr->range))){
					index = itr->index;
					break;
			}
		}
		//if(index != -1 && doshow == true){
		if(index != -1){
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
									"总净宽20米分5孔，，设计流量440 m3/s，每孔净宽10米。<br>"
									"配弧型钢闸门，采用两台225kN卷扬式闸门启闭机，由3台电气控制柜控制。");
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
									"总净宽20米分4孔，设计流量100 m3/s，每孔净宽5m。<br>"
									"闸门为钢结构平面上下扉直升门，采用两台160kN卷扬式启闭机，分别由两台电气控制柜控制。");
						title = tr("调度闸");
					}
					break;
				case 5:
					{
						content = tr(
								    "总净宽16.5米分三孔，设计流量100m3/s，每孔净宽5.5m。<br>"
								    "闸门为钢结构平面直升门，采用两台80kN卷扬式启闭机，分别由两台电气控制柜控制。");
						title = tr("送水闸");
					}
					break;
				case 11:
					{
						content= tr(
							"变电所有主变一台，SF6继路器1台，电流互感器、电压互感器各3只，隔离刀闸4组");
						title=tr("变电所");
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
			}else if(customerList->row(currentItem) == 3){
				if(index == 0){
					title = tr("辅机系统");
					content = tr(
								"冷却水系统主要用于水导轴承、填料函、上下油缸、叶片调节系统等方面，<br>"
								"三台供水泵通过一个400立方米的蓄水池提供水源。");
				}
			}else if(customerList->row(currentItem) == 6){
				if(index == 2){
					title = tr("站变");
					content = tr(
								"三相树脂绝缘干式电力变压器<br>"
								"型号：SC9-800/10<br>"
								"制造商：南通友邦变压器有限公司<br>"
								"额定容量：800kVA<br>"
								"额定电流：高压侧 46.2A、低压侧 1154.7A<br>");
				}else if(index == 3){
					title = tr("所变");
					content = tr(
								"三相树脂绝缘干式电力变压器<br>"
								"型号：SC9-800/10<br>"
								"制造商：南通友邦变压器有限公司<br>"
								"额定容量：800kVA<br>"
								"额定电流：高压侧 46.2A、低压侧 1154.7A<br>");
				}
			}

			msgBox.setWindowTitle(title);
			msgBox.setText(content);
			msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
			int ret = msgBox.exec();
			//doshow = false;
		}
	}

}

//void MainWindow::resetDoshowFlag(){
//	doshow = true;
//}

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
		string openpath = QFileDialog::getOpenFileName(this, tr("打开路径"),"./paths", tr("*.path")).toStdString();
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
			this->recordpathActTB->setIcon(QIcon(string(RESOURCESPATH + "pause.png").c_str()));
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
			string savepath = QFileDialog::getSaveFileName(this, tr("保存路径"),"./paths", tr("*.path")).toStdString();
			string ext = savepath.substr(savepath.rfind('.')+1, savepath.length());
			if(ext.compare("path") !=0){//useless, only for safe
				QMessageBox::information(this, tr("请重新选择保存名称"), tr("路径文件只接受\".path\"扩展名，请重新保存路径。"));
			}else{
				recordpathActTB->setIcon(QIcon(string(RESOURCESPATH + "recordpath.png").c_str()));
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