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
	setWindowTitle(tr("�߸�բվ������ά���ӻ��ۺϹ��������ϵͳ"));
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
	fileMenu = new MyMenu(tr("&�ļ�"),this);
	fileMenu->addAction(openpathAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
	
	editMenu = new MyMenu(tr("&�༭"),this);
	editMenu->addAction(recordpathAct);
	
	viewMenu = new MyMenu(tr("&��ͼ"),this);
	viewMenu->addAction(toolBarViewAct);
	settingMenu = new MyMenu(tr("&����"), this);
    settingMenu->addAction(interecAct);
	settingMenu->addAction(cameraSettingAct);
	settingMenu->addAction(cameraLowModeAct);
	settingMenu->addAction(cameraFlyModeAct);
	settingMenu->addAction(textInfoAct);

	helpMenu = new MyMenu(tr("&����"), this);
	helpMenu->addAction(howtoAct);
    helpMenu->addAction(aboutAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(editMenu);
	menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(settingMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createAction(){
	openpathActTB = new QAction(QIcon(string(RESOURCESPATH +"open.png").c_str()),tr("&��·��"), this);
	openpathAct = new QAction(tr("&��·��"), this);
	connect(openpathActTB, SIGNAL(triggered()), this, SLOT(playCameraPath()));
	connect(openpathAct, SIGNAL(triggered()), this, SLOT(playCameraPath()));
	recordpathActTB = new QAction(QIcon(string(RESOURCESPATH +"recordpath.png").c_str()), tr("&��ʼ��¼·��"), this);
	recordpathAct = new QAction(tr("&��ʼ��¼·��"), this);
	connect(recordpathAct, SIGNAL(triggered()), this, SLOT(recordCameraPath()));
	connect(recordpathActTB, SIGNAL(triggered()), this, SLOT(recordCameraPath()));
	
	cameraSettingActTB = new QAction(QIcon(string(RESOURCESPATH +"camera_setting.png").c_str()),tr("&�ӵ��������"), this);
	cameraSettingAct = new QAction(tr("&�ӵ��������"), this);
	connect(cameraSettingActTB, SIGNAL(triggered()), this, SLOT(showCameraSettingDialog()));
	connect(cameraSettingAct, SIGNAL(triggered()), this, SLOT(showCameraSettingDialog()));

	cameraLowModeAct = new QAction(tr("&����ģʽ�ӵ�"), this);
	cameraLowModeActTB = new QAction(QIcon(string(RESOURCESPATH + "lowmode.png").c_str()),tr("&����ģʽ�ӵ�"), this);
	cameraLowModeAct->setCheckable(true);
	cameraLowModeAct->setChecked(true);
	cameraLowModeActTB->setCheckable(true);
	cameraLowModeActTB->setChecked(true);
	connect(cameraLowModeAct, SIGNAL(triggered()), this, SLOT(setCameraLowMode()));
	connect(cameraLowModeActTB, SIGNAL(triggered()), this, SLOT(setCameraLowMode()));

	cameraFlyModeAct = new QAction(tr("&���ģʽ�ӵ�"), this);
	cameraFlyModeActTB = new QAction(QIcon(string(RESOURCESPATH + "flymode.png").c_str()),tr("&���ģʽ�ӵ�"), this);
	cameraFlyModeAct->setCheckable(true);
	cameraFlyModeAct->setChecked(false);
	cameraFlyModeActTB->setCheckable(true);
	cameraFlyModeActTB->setChecked(false);
	connect(cameraFlyModeAct, SIGNAL(triggered()), this, SLOT(setCameraFlyMode()));
	connect(cameraFlyModeActTB, SIGNAL(triggered()), this, SLOT(setCameraFlyMode()));
	
	exitAct = new QAction(tr("&�˳�"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	interecActTB = new QAction(QIcon(string(RESOURCESPATH + "peng_closed.png").c_str()), tr("&��ײ���"), this);
	interecAct = new QAction(tr("&��ײ���"), this);
    interecAct->setShortcut(tr("Ctrl+I"));
	interecAct->setCheckable(true);
	interecActTB->setCheckable(true);
    connect(interecAct, SIGNAL(triggered()), this, SLOT(open_peng()));
	connect(interecActTB, SIGNAL(triggered()), this, SLOT(open_peng()));

	toolBarViewAct = new QAction(tr("������"), this);
	toolBarViewAct->setCheckable(true);
	toolBarViewAct->setChecked(true);
	connect(toolBarViewAct, SIGNAL(triggered()), this, SLOT(toolbarSwitch()));

	textInfoAct = new QAction(tr("ʵʱ����"), this);
	textInfoAct->setCheckable(false);
	textInfoAct->setChecked(false);
	textInfoAct->setEnabled(false);
	connect(textInfoAct, SIGNAL(triggered()), this, SLOT(textInfoSwitch()));

    aboutAct = new QAction(tr("&����"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	howtoAct = new QAction(tr("&ʹ�÷���"), this);
	connect(howtoAct, SIGNAL(triggered()), this, SLOT(control()));
}

void MainWindow::createToolBar(){
	 editToolBar=addToolBar(tr("�༭"));
	 editToolBar->addAction(openpathActTB);
	 editToolBar->addAction(recordpathActTB);

	 settingToolBar=addToolBar(tr("����"));
	 settingToolBar->addAction(interecActTB);
	 settingToolBar->addAction(cameraSettingActTB);
	 settingToolBar->addAction(cameraLowModeActTB);
	 settingToolBar->addAction(cameraFlyModeActTB);
}

void MainWindow::createDockWindow(){
	 QDockWidget *dock = new QDockWidget(tr("��������"), this);
	 dock->setMaximumWidth(320);
	 dock->setMinimumWidth(300);
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     customerList = new QListWidget(dock);
     customerList->addItems(QStringList()
             << ">�߸�բվ"
             << ">��ˮբ"
			 << ">�÷�"
			 << ">�����"
			 << ">����բ"
			 << ">����բ"
			 << ">վ����");
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

	 mapdock = new QDockWidget(tr("��ͼ����"), this);
	 mapdock->setMaximumHeight(450);
	 mapdock->setMinimumHeight(400);
	 mapdock->setMaximumWidth(350);
	 mapdock->setMinimumWidth(300);
     mapdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	 QGraphicsScene *scene = new QGraphicsScene(0, 0, 300, 400);

	 MapItem* mapitem1 = new MapItem(QPixmap(string(RESOURCESPATH + "kongzhilou.png").c_str()).scaled(36.5,29));
	 mapitem1->setData(0, 0);
	 mapitem1->setPos(83, 171);
	 mapitem1->setToolTip(QString(tr("����¥")));
	 scene->addItem(mapitem1);

	 MapItem* mapitem2 = new MapItem(QPixmap(string(RESOURCESPATH + "diaoduzha.png").c_str()).scaled(13,33));
	 mapitem2->setData(0, 1);
	 mapitem2->setPos(226,122);
	 mapitem2->setToolTip(QString(tr("����բ")));
	 scene->addItem(mapitem2);
	 
	 MapItem* mapitem3 =new MapItem(QPixmap(string(RESOURCESPATH + "bengzhan.png").c_str()).scaled(36,105));
	 mapitem3->setData(0, 2);
	 mapitem3->setPos(82,66);
	 mapitem3->setToolTip(QString(tr("��վ")));
	 scene->addItem(mapitem3);
	  
	 MapItem* mapitem4=new MapItem(QPixmap(string(RESOURCESPATH + "biandianzhan.png").c_str()).scaled(68,67.5));
	 mapitem4->setData(0, 3);
	 mapitem4->setPos(134,316.5);
	 mapitem4->setToolTip(QString(tr("���վ")));
	 scene->addItem(mapitem4);

	 MapItem* mapitem5=new MapItem(QPixmap(string(RESOURCESPATH + "jiezhizha.png").c_str()).scaled(25,51));
	 mapitem5->setData(0, 4);
	 mapitem5->setPos(91,14);
	 mapitem5->setToolTip(QString(tr("����բ")));
	 scene->addItem(mapitem5);

	 MapItem* mapitem6=new MapItem(QPixmap(string(RESOURCESPATH + "songshuizha.png").c_str()).scaled(43.5,39.5));
	 mapitem6->setData(0, 5);
	 mapitem6->setPos(180.5,187.5);
	 mapitem6->setToolTip(QString(tr("��ˮբ")));
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
	msgBox.setWindowTitle(tr("<b>����</b>"));
	msgBox.setText(tr("<b>��������ϵͳ</b>"));
	msgBox.setInformativeText(tr("	��������������ĳ����͹�����֮������Ѳ����<br> "
								"  �汾�ţ�V1.0<br>"
								  "������λ���Ӻ���ѧ<br>"));
	msgBox.addButton(tr("ȷ��"), QMessageBox::AcceptRole);
	int ret = msgBox.exec();
}

void MainWindow::control(){
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("<b>ʹ�÷���</b>"));
	msgBox.setText(tr("<b>����˵��</b>"));
	msgBox.setInformativeText(tr("�����ǰ������ƽ�ƣ�W��S��A��D��<br> "
					 "���������ƽ�ƣ�Q��E��<br> "
					 "�����ת�����϶�����Ҽ�<br> "
					 "ѡ������鿴���ܣ����������<br> "
					 "�Ŵ�/��С�������ֹ���<br> "));
	msgBox.addButton(tr("ȷ��"), QMessageBox::AcceptRole);
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
									"װ��9̨��ʽ����ʽ�����ã�Ҷ��ֱ��3�ף���������34������ÿ�롣<br>"
									"���̨2000ǧ����ʽͬ�����������4��ƽ���բ�ţ���������ˮ300 m3/s��");
						title = tr("��վ");
					}
					break;
				case 1:
					{
						content = tr(
									"�ܾ���20�׷�5�ף����������440 m3/s��ÿ�׾���10�ס�<br>"
									"�仡�͸�բ�ţ�������̨225kN����ʽբ�����ջ�����3̨�������ƹ���ơ�");
						title = tr("����բ");
					}
					break;
				case 2:
					{
						content = tr(
									"բ�ҳ�196�ף�����16�ף�����ˮ��3.5�ף�<br>"
									"��ͨ��ǧ�ּ����ӡ�<br>");
						title = tr("��բ");
					}
					break;
				case 3:
					{
						content = tr(
									"բ�ҳ�196�ף�����16�ף�����ˮ��3.5�ף�<br>"
									"��ͨ��ǧ�ּ����ӡ�<br>");
						title = tr("��բ");
					}
					break;
				case 4:
					{
						content = tr(
									"�ܾ���20�׷�4�ף��������100 m3/s��ÿ�׾���5m��<br>"
									"բ��Ϊ�ֽṹƽ��������ֱ���ţ�������̨160kN����ʽ���ջ����ֱ�����̨�������ƹ���ơ�");
						title = tr("����բ");
					}
					break;
				case 5:
					{
						content = tr(
								    "�ܾ���16.5�׷����ף��������100m3/s��ÿ�׾���5.5m��<br>"
								    "բ��Ϊ�ֽṹƽ��ֱ���ţ�������̨80kN����ʽ���ջ����ֱ�����̨�������ƹ���ơ�");
						title = tr("��ˮբ");
					}
					break;
				case 11:
					{
						content= tr(
							"�����������һ̨��SF6��·��1̨����������������ѹ��������3ֻ�����뵶բ4��");
						title=tr("�����");
					}
					break;
				default: return;
				}
			}else if(customerList->row(currentItem) == 2){
				if(index > 8 && index < 18){//���բ��
					content = tr(
								"ÿ̨ˮ�û����ϡ��������²����������ڸ���һ������ƽ�����<br>"
								"բ�ţ�����QPK�;���ʽ�������ջ����ƣ�ÿ̨���鹲4�ȡ�");
					QString num = QString::number(index-8, 10);
					title = num + tr("#բ��");
				}else if(index > -1 && index < 3){
					QString num = QString::number(index+1, 10);
					title = num + tr("#ˮ�û���");
					content = tr(
								"ˮ���ͺţ�3000ZLB35-4    �������ң�����ˮ�ó�<br>"
								"���������35m3/s    �����̣�4m    ��Ч�ʣ�89%<br>"
								"���ù��ʣ�2000kW    ˮ��ת�٣�150r/min<br>"
								"ˮ��Ҷ��ֱ����3m    ҶƬ����3ö<br>"
								"���ΪTL2000��40/3250��2000 kW��ʽͬ���綯����");
				}else if(index > 2 && index < 9){
					QString num = QString::number(index+1, 10);
					title = num + tr("#ˮ�û���");
					content = tr(
								"ˮ���ͺţ�3000ZLQ35-4    �������ң�����ˮ�ó�<br>"
								"������Χ��26��42 m3/s    ��̷�Χ��0��5.2m<br>"
								"��Ч�ʷ�Χ��75��89%<br>"
								"���ù��ʣ�2000kW     ˮ��ת�٣�150r/min <br>"
								"ˮ��Ҷ��ֱ����3m   ҶƬ����3ö<br>"
								"���ΪTL2000��40/3250��2000 kW��ʽͬ���綯����");
				}else
					return;
			}else if(customerList->row(currentItem) == 3){
				if(index == 0){
					title = tr("����ϵͳ");
					content = tr(
								"��ȴˮϵͳ��Ҫ����ˮ����С����Ϻ��������͸ס�ҶƬ����ϵͳ�ȷ��棬<br>"
								"��̨��ˮ��ͨ��һ��400�����׵���ˮ���ṩˮԴ��");
				}
			}else if(customerList->row(currentItem) == 6){
				if(index == 2){
					title = tr("վ��");
					content = tr(
								"������֬��Ե��ʽ������ѹ��<br>"
								"�ͺţ�SC9-800/10<br>"
								"�����̣���ͨ�Ѱ��ѹ�����޹�˾<br>"
								"�������800kVA<br>"
								"���������ѹ�� 46.2A����ѹ�� 1154.7A<br>");
				}else if(index == 3){
					title = tr("����");
					content = tr(
								"������֬��Ե��ʽ������ѹ��<br>"
								"�ͺţ�SC9-800/10<br>"
								"�����̣���ͨ�Ѱ��ѹ�����޹�˾<br>"
								"�������800kVA<br>"
								"���������ѹ�� 46.2A����ѹ�� 1154.7A<br>");
				}
			}

			msgBox.setWindowTitle(title);
			msgBox.setText(content);
			msgBox.addButton(tr("ȷ��"), QMessageBox::AcceptRole);
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
		string openpath = QFileDialog::getOpenFileName(this, tr("��·��"),"./paths", tr("*.path")).toStdString();
		TravelManipulator::Instance()->setPlayPath(true, openpath);
	}
}

void MainWindow::recordCameraPath(){
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("��¼·��"));
	if(TravelManipulator::Instance()->getRecordPath() == false){
		msgBox.setText(tr("�Ƿ�ʼ��¼·����"));
		msgBox.addButton(tr("��"), QMessageBox::AcceptRole);//ret = 0
		msgBox.addButton(tr("��"), QMessageBox::RejectRole);//ret = 1
		int ret = msgBox.exec();
		if(ret == 0){
			this->recordpathActTB->setIcon(QIcon(string(RESOURCESPATH + "pause.png").c_str()));
			this->recordpathActTB->setText(tr("��ͣ��¼·��"));
			this->recordpathAct->setText(tr("��ͣ��¼·��"));
			TravelManipulator::Instance()->setRecordPath(true);
		}
	}else if(TravelManipulator::Instance()->getRecordPath() == true){
		msgBox.setText(tr("�Ƿ�ֹͣ��¼·����"));
		msgBox.addButton(tr("��"), QMessageBox::AcceptRole);//ret = 0
		msgBox.addButton(tr("��"), QMessageBox::RejectRole);//ret = 1
		int ret = msgBox.exec();
		if(ret == 0){
			string savepath = QFileDialog::getSaveFileName(this, tr("����·��"),"./paths", tr("*.path")).toStdString();
			string ext = savepath.substr(savepath.rfind('.')+1, savepath.length());
			if(ext.compare("path") !=0){//useless, only for safe
				QMessageBox::information(this, tr("������ѡ�񱣴�����"), tr("·���ļ�ֻ����\".path\"��չ���������±���·����"));
			}else{
				recordpathActTB->setIcon(QIcon(string(RESOURCESPATH + "recordpath.png").c_str()));
				this->recordpathActTB->setText(tr("��ʼ��¼·��"));
				this->recordpathAct->setText(tr("��ʼ��¼·��"));
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