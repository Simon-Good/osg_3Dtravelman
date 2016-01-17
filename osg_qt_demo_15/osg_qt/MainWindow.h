#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <qdockwidget.h>
#include <qlistwidget.h>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "MapItem.h"
#include "ViewerWidget.h"
#include "TravelManipulator.h"
#include "CameraSetting.h"
#include "WelcomePage.h"
#include "Global.h"
#include "DBHandler.h"
#include "GeneralEventHandler.h"
#include <osg/Matrix>
#include <osg/Vec3>
#include <osg/MatrixTransform>
#include <QProgressDialog>
#include <string>
#include <iostream>
#include <QToolBar>
#include <QIcon>

using namespace std;
class MyMenu: public QMenu{
	Q_OBJECT
public:
	MyMenu(QWidget *parent = 0);
	MyMenu(const QString &title, QWidget *parent = 0);
	~MyMenu();
	void mouseReleaseEvent(QMouseEvent *e);
};

class MyGraphicsView: public QGraphicsView{
	Q_OBJECT
public:
	MyGraphicsView(QGraphicsScene* scene, QWidget *parent = 0):QGraphicsView(scene, parent){}
	~MyGraphicsView(){}
protected:
    void mouseReleaseEvent(QMouseEvent *event);
signals:
	void itemClicked(osg::Vec3& trans, osg::Vec3& rot);
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(WelcomePage* wp, QWidget *parent = 0);
	~MainWindow();
	void createMenu();
	void createAction();
	void createDockWindow();
	void createToolBar();
	void updateCameraSetting(CameraContext *cc);
	virtual bool winEvent(MSG * message, long * result);
	float caculateRadius(osg::Vec4& points);
public slots:
	void open_peng();
	void about();
	void control();
	void changeModel(QListWidgetItem* item);
	void showCameraSettingDialog();
	void setCameraLowMode();
	void setCameraFlyMode();
	void setCameraToPosition(osg::Vec3& trans, osg::Vec3& rot);
	void showPos(float x, float y, float radius);
	void enableItems();
	void toolbarSwitch();
	//void resetDoshowFlag();
	void playCameraPath();
	void recordCameraPath();
	void textInfoSwitch();
public:
	QProgressDialog* progress;
	vector<string>* names;
private:
	QToolBar *editToolBar;
	QToolBar *settingToolBar;
	QAction *toolBarViewAct;
	QAction *textInfoAct;

	QAction *openpathAct;
	QAction *openpathActTB;
	QAction *recordpathAct;
	QAction *recordpathActTB;
    QAction *exitAct;
	QAction *cameraSettingAct;
	QAction *cameraLowModeAct;
	QAction *cameraFlyModeAct;
	QAction *cameraSettingActTB;
	QAction *cameraLowModeActTB;
	QAction *cameraFlyModeActTB;

	QAction *howtoAct;
    QAction *aboutAct;
	QAction *interecAct;
	QAction *interecActTB;
	QDockWidget *mapdock;

	MyMenu *fileMenu;
	MyMenu *editMenu;
	MyMenu *viewMenu;
    MyMenu *settingMenu;
	MyMenu *helpMenu;
	QListWidget *customerList;
	ViewerWidget* viewWidget;
	CameraSetting* cs;
	QListWidgetItem * currentItem;
	QMessageBox msgBox;
	//bool doshow;
};

#endif // MAINWINDOW_H
