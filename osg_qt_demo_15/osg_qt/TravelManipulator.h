#pragma once
#include <osgViewer/Viewer>
#include <osg/Matrix>
#include <osg/Vec3>
#include <osgGA/CameraManipulator>
#include <osgUtil/IntersectVisitor>
#include <osg/LineSegment>
#include <osg/MatrixTransform>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>
#include <osg/AnimationPath>
#include <cmath>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;
struct RangeNode{
	//imagine as a rectangle range, range[0] is the minmum x value, range[1] max x value
	//range[2] is the minmum y value, range[3] max y  value
	RangeNode(int indx, osg::Vec4 rng){
		index = indx;
		range._v[0] = rng._v[0];
		range._v[1] = rng._v[1];
		range._v[2] = rng._v[2];
		range._v[3] = rng._v[3];
	}
	int index;
	osg::Vec4 range;
};

struct CameraContext{
	osg::Vec3 m_vPosition;
	osg::Vec3 m_vRotation;
	float m_fMoveSpeed;
	float m_fAngle;
	float min_height;
	float max_height;
	bool flymode;
	bool lowmode;
	bool peng;
	vector<RangeNode>* keepout;
	vector<RangeNode>* keepin;
	double fov;
};

class TravelManipulator :
	public osgGA::CameraManipulator
{
public:
	static TravelManipulator* Instance();
	
protected:
	TravelManipulator();
	~TravelManipulator(void);
	bool inRange(const osg::Vec3& pos);
	bool outRange(const osg::Vec3& pos);
	
public:
	CameraContext *cc;

	float dragDelta;
	float m_fpushX;//������ʱ��Ļ���
	float m_fpushY;//�Ҽ����ʱ��Ļ���
	bool m_bRightButtonDown;//����Ƿ���
	osg::ref_ptr<osgViewer::View> view; 

public:
	virtual void setByMatrix(const osg::Matrixd& matrix);
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	virtual osg::Matrixd getMatrix(void) const;
	virtual osg::Matrixd getInverseMatrix(void) const;
	void ChangePosition(osg::Vec3 delta);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	CameraContext* getCameraContext();
	void setCameraContext(CameraContext* context);
	void setRecordPath(bool rec, string savePath = "");
	bool getRecordPath(){return recordPath;}
	void setPlayPath(bool ply, string playPath = "");
	bool getPlayPath(){return playPath;}
	void resetStateBits();
	void switchForbidMove();
private:
	static TravelManipulator* instance;
	static float rightDragDistX;
	static float rightDragDistY;
	static int frameCount;
	char stateBits;
	bool recordPath;
	bool playPath;
	bool forbidMove;
	clock_t timeRecord;
	const char moveForward;
	const char moveBackward;
	const char moveLeft;
	const char moveRight;
	const char moveUpward;
	const char moveDownward;
	osg::ref_ptr<osg::AnimationPath> animationPath;
	osg::AnimationPath::TimeControlPointMap pointMap;
	osg::AnimationPath::TimeControlPointMap::iterator pointMapIt;
};
