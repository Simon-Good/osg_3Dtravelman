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
	setWindowTitle(tr("�߸�բվ������ά���ӻ��ۺϹ��������ϵͳ"));
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
	openpathActTB = new QAction(QIcon("./open.png"),tr("&��·��"), this);
	openpathAct = new QAction(tr("&��·��"), this);
	connect(openpathActTB, SIGNAL(triggered()), this, SLOT(playCameraPath()));
	connect(openpathAct, SIGNAL(triggered()), this, SLOT(playCameraPath()));
	recordpathActTB = new QAction(QIcon("./recordpath.png"), tr("&��ʼ��¼·��"), this);
	recordpathAct = new QAction(tr("&��ʼ��¼·��"), this);
	connect(recordpathAct, SIGNAL(triggered()), this, SLOT(recordCameraPath()));
	connect(recordpathActTB, SIGNAL(triggered()), this, SLOT(recordCameraPath()));
	
	cameraSettingActTB = new QAction(QIcon("./camera_setting.png"),tr("&�ӵ��������"), this);
	cameraSettingAct = new QAction(tr("&�ӵ��������"), this);
	connect(cameraSettingActTB, SIGNAL(triggered()), this, SLOT(showCameraSettingDialog()));
	connect(cameraSettingAct, SIGNAL(triggered()), this, SLOT(showCameraSettingDialog()));

	cameraLowModeAct = new QAction(tr("&����ģʽ�ӵ�"), this);
	cameraLowModeActTB = new QAction(QIcon("./lowmode.png"),tr("&����ģʽ�ӵ�"), this);
	cameraLowModeAct->setCheckable(true);
	cameraLowModeAct->setChecked(true);
	cameraLowModeActTB->setCheckable(true);
	cameraLowModeActTB->setChecked(true);
	connect(cameraLowModeAct, SIGNAL(triggered()), this, SLOT(setCameraLowMode()));
	connect(cameraLowModeActTB, SIGNAL(triggered()), this, SLOT(setCameraLowMode()));

	cameraFlyModeAct = new QAction(tr("&���ģʽ�ӵ�"), this);
	cameraFlyModeActTB = new QAction(QIcon("./flymode.png"),tr("&���ģʽ�ӵ�"), this);
	cameraFlyModeAct->setCheckable(true);
	cameraFlyModeAct->setChecked(false);
	cameraFlyModeActTB->setCheckable(true);
	cameraFlyModeActTB->setChecked(false);
	connect(cameraFlyModeAct, SIGNAL(triggered()), this, SLOT(setCameraFlyMode()));
	connect(cameraFlyModeActTB, SIGNAL(triggered()), this, SLOT(setCameraFlyMode()));
	
	exitAct = new QAction(tr("&�˳�"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	interecActTB = new QAction(QIcon("./peng_closed.png"), tr("&��ײ���"), this);
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
	 //fileToolBar = addToolBar(tr("�ļ�"));
	 //fileToolBar->addAction(openpathActTB);

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
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     customerList = new QListWidget(dock);
     customerList->addItems(QStringList()
             << ">�߸�բվ"
             << ">��ˮբ"
			 << ">�÷�"
			 << ">�����"
			 << ">����բ"
			 << ">����բ");
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
     mapdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	 QGraphicsScene *scene = new QGraphicsScene(0, 0, 400, 300);
	 MapItem* mapitem = new MapItem(QPixmap("./open.png"));
	 mapitem->setData(0, 0);
	 mapitem->setPos(20, 10);
	 mapitem->setToolTip(QString(tr("����վ")));
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
	msgBox.setWindowTitle(tr("<b>����</b>"));
	msgBox.setText(tr("<b>��������ϵͳ</b>"));
	msgBox.setInformativeText(tr("	��������������ĳ����͹�����֮�����������<br> "
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
					 "ѡ�н�����϶�������<br> "
					 "�Ŵ�/��С�������ֹ���<br> "));
	msgBox.addButton(tr("ȷ��"), QMessageBox::AcceptRole);
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
									"װ��9̨��ʽ����ʽ�����ã�Ҷ��ֱ��3�ף���������34������ÿ�롣<br>"
									"���̨2000ǧ����ʽͬ�����������4��ƽ���բ�ţ���������ˮ300 m3/s��");
						title = tr("��վ");
					}
					break;
				case 1:
					{
						content = tr(
									"��5�ף�ÿ�׾���10�ף��仡�͸�բ�ţ��������440 m3/s");
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
									"��4�ף��ܾ���20�ף��������100 m3/s����ƽ���բ�š�");
						title = tr("����բ");
					}
					break;
				case 5:
					{
						content = tr(
									"��3�ף��ܾ���16.5�ף��������100m3/s��");
						title = tr("��ˮբ");
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
			}
			QMessageBox msgBox;
			msgBox.setWindowTitle(title);
			msgBox.setText(content);
			msgBox.addButton(tr("ȷ��"), QMessageBox::AcceptRole);
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
		string openpath = QFileDialog::getOpenFileName(this, tr("��·��"),"./path", tr("*.path")).toStdString();
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
			this->recordpathActTB->setIcon(QIcon("./pause.png"));
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
			string savepath = QFileDialog::getSaveFileName(this, tr("����·��"),"./path", tr("*.path")).toStdString();
			string ext = savepath.substr(savepath.rfind('.')+1, savepath.length());
			if(ext.compare("path") !=0){//useless, only for safe
				QMessageBox::information(this, tr("������ѡ�񱣴�����"), tr("·���ļ�ֻ����\".path\"��չ���������±���·����"));
			}else{
				recordpathActTB->setIcon(QIcon("./recordpath.png"));
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