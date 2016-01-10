#include "TravelManipulator.h"

TravelManipulator* TravelManipulator::instance = 0;
float TravelManipulator::rightDragDistX = 0.0;
float TravelManipulator::rightDragDistY = 0.0;
int TravelManipulator::frameCount = 0;
TravelManipulator* TravelManipulator::Instance(){
	if(instance == 0){
		instance = new TravelManipulator();
	}
	return instance;
}

TravelManipulator::TravelManipulator():
	moveForward(1),moveBackward(1<<1),moveLeft(1<<2),
	moveRight(1<<3),moveUpward(1<<4),moveDownward(1<<5)
	/*m_fMoveSpeed(150.0f),m_fAngle(0.5f),moveForward(1),moveBackward(1<<1),
	moveLeft(1<<2),moveRight(1<<3),moveUpward(1<<4),moveDownward(1<<5)*/
{
	//m_vPosition = osg::Vec3(0.0f, 0.0f, 0.0f);//��ʼλ��
	//m_vRotation = osg::Vec3(osg::PI_2,0.0f,0.0f);//��ʼ�Ƕȣ��ɸ��ӣ���x��ת��ʮ�ȣ�����y������
	m_fpushX= 0;
	m_fpushY= 0;
	//min_height = 450;
	//max_height = 2500;
	dragDelta = 0.05;
	//flymode = false;
	//lowmode = true;
	//peng=false;
	//fov = 30.0;
	cc = NULL;
	m_bRightButtonDown= false;
	view= new osgViewer::View();
	stateBits = 0x0;
	recordPath = false;
	playPath = false;
	forbidMove = true;
	timeRecord = 0;
	animationPath = NULL;
	animationPath = new osg::AnimationPath();
	animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
}

TravelManipulator::~TravelManipulator(void)
{
}

void TravelManipulator::setByMatrix(const osg::Matrixd& matrix)
{
}

void TravelManipulator::setByInverseMatrix(const osg::Matrixd& matrix)
{
}

osg::Matrixd TravelManipulator::getMatrix(void) const
{
	if(cc != NULL){
		osg::Matrixd mat;

		mat.makeRotate(cc->m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f),

			cc->m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f),

			cc->m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f));

		return mat * osg::Matrixd::translate(cc->m_vPosition);
	}else
		return osg::Matrix::rotate(osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f),
														0, osg::Vec3(0.0f, 1.0f, 0.0f),
														0, osg::Vec3(0.0f, 0.0f, 1.0f))
									* osg::Matrix::translate(osg::Vec3(0.0f, 0.0f, 0.0f));
}

osg::Matrixd TravelManipulator::getInverseMatrix(void) const
{
	if(cc!=NULL){
		osg::Matrixd mat;

		mat.makeRotate(cc->m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f),

			cc->m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f),

			cc->m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f));

		return osg::Matrixd::inverse(mat * osg::Matrixd::translate(cc->m_vPosition));
	}else
		return osg::Matrix::inverse(osg::Matrix::rotate(osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f),
														0, osg::Vec3(0.0f, 1.0f, 0.0f),
														0, osg::Vec3(0.0f, 0.0f, 1.0f))
									* osg::Matrix::translate(osg::Vec3(0.0f, 0.0f, 0.0f)));
}

void TravelManipulator::ChangePosition(osg::Vec3 delta)
{
	if(inRange(cc->m_vPosition+delta))
		if (cc->peng && cc->lowmode){
			if(outRange(cc->m_vPosition+delta))
				cc->m_vPosition += delta;
		}else 
			cc->m_vPosition += delta; 
}

bool TravelManipulator::inRange(const osg::Vec3& pos){
	bool ret = true;
	if(cc->keepin!= NULL){
		for(vector<RangeNode>::iterator itr = cc->keepin->begin(); 
			itr != cc->keepin->end(); 
			itr++){
			if( !((itr->range.x() < pos.x()&& pos.x() < itr->range.y())&&
				(itr->range.z() < pos.y()&& pos.y() < itr->range.w()))){
					ret = false;
					break;
			}
		}
	}
	return ret;
}

bool TravelManipulator::outRange(const osg::Vec3& pos){
	bool ret = true;
	if(cc->keepout != NULL){
		for(vector<RangeNode>::iterator itr = cc->keepout->begin();
			itr != cc->keepout->end(); itr++){
			if( (itr->range.x() < pos.x()&& pos.x() < itr->range.y())&&
				(itr->range.z() < pos.y()&& pos.y() < itr->range.w())){
					ret = false;
					break;
			}
		}
	}
	return ret;
}

bool TravelManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	float mouseX=ea.getX();
	float mouseY=ea.getY();
	view = dynamic_cast<osgViewer::Viewer*> (&us);
	
	switch(ea.getEventType())
	{
		case (osgGA::GUIEventAdapter::FRAME):
			{
				//////////////////////////update camera position and rotation//////////////////////
				if(forbidMove != true){
					if((stateBits&moveForward) != 0 && (stateBits&moveBackward) == 0){
						ChangePosition(osg::Vec3 (0, cc->m_fMoveSpeed * sinf(osg::PI_2+cc->m_vRotation._v[2]), 0)) ; 
						ChangePosition(osg::Vec3 (cc->m_fMoveSpeed * cosf(osg::PI_2+cc->m_vRotation._v[2]), 0, 0)) ;
					}else if((stateBits&moveBackward) != 0 && (stateBits&moveForward) == 0){
						ChangePosition(osg::Vec3 (0, -cc->m_fMoveSpeed * sinf(osg::PI_2+cc->m_vRotation._v[2]), 0)) ; 
						ChangePosition(osg::Vec3(-cc->m_fMoveSpeed * cosf(osg::PI_2+cc->m_vRotation._v[2]), 0, 0)) ;
					}

					if((stateBits&moveLeft)!= 0 && (stateBits&moveRight) == 0){
						ChangePosition(osg::Vec3 (0, cc->m_fMoveSpeed * cosf(osg::PI_2+cc->m_vRotation._v[2]), 0)) ; 
						ChangePosition(osg::Vec3 (-cc->m_fMoveSpeed * sinf(osg::PI_2+cc->m_vRotation._v[2]), 0, 0)) ;
					}else if((stateBits&moveRight) != 0 && (stateBits&moveLeft)== 0){
						ChangePosition(osg::Vec3 (0,-cc->m_fMoveSpeed * cosf(osg::PI_2+cc->m_vRotation._v[2]), 0)) ; 
						ChangePosition(osg::Vec3 (cc->m_fMoveSpeed * sinf(osg::PI_2+cc->m_vRotation._v[2]), 0, 0)) ;
					}

					if((stateBits&moveUpward) != 0 && (stateBits&moveDownward) == 0){
						if(cc->m_vPosition.z() < cc->max_height)
							ChangePosition(osg::Vec3 (0, 0, cc->m_fMoveSpeed));
					}else if((stateBits&moveDownward) != 0 && (stateBits&moveUpward) == 0){
						if(cc->m_vPosition.z() > cc->min_height)
							ChangePosition(osg::Vec3 (0, 0, -cc->m_fMoveSpeed));
					}
				}else{
					this->resetStateBits();
				}
				
				////////////////////////////////record path if user want to//////////////////
				if(recordPath == true){
					if(frameCount % 5 == 0){
						osg::Camera* mainCam = view->getCamera();
						osg::Matrix camMat = mainCam->getViewMatrix();
						osg::Vec3 pos = camMat.getTrans();
						osg::Quat rot = camMat.getRotate();
						float time = ((float)(clock() - timeRecord))/CLOCKS_PER_SEC;
						animationPath->insert(time, osg::AnimationPath::ControlPoint(pos, rot));
					}
					frameCount++;
				}
				///////////////////////////////play path if user want to/////////////////////
				if(playPath == true){
					if(frameCount %4 == 0){
						if(pointMapIt != pointMap.end()){
							osg::Camera* mainCam = view->getCamera();
							osg::Matrix mt;
							pointMapIt->second.getMatrix(mt);
							mt = osg::Matrix::inverse(mt);
							cc->m_vPosition = mt.getTrans();
							pointMapIt++;
						}else{
							playPath = false;
							frameCount = 0;
						}
					}
					frameCount++;
				}
			}
			return false;
		case(osgGA::GUIEventAdapter::KEYDOWN):
			{
				if(ea.getKey() == 'q')//upwards
					stateBits |= moveUpward;
				if(ea.getKey () == 'e')//downwards
					stateBits |= moveDownward;
				if (ea.getKey () == 0x41 || ea.getKey () == 0x61)//'a'move to left
					stateBits |= moveLeft;
				if (ea.getKey () == 0x44||ea.getKey () == 0x64)//'d'move to right
					stateBits |= moveRight;
				if(ea.getKey()==0X57 || ea.getKey()==0X77)//'w'move to front
					stateBits |= moveForward;
				if(ea.getKey()==0X53 || ea.getKey()==0X73 )//'s' move to back
					stateBits |= moveBackward;
				if (ea.getKey() == 0xFF53)//right arrow,Right roll
					cc->m_vRotation._v[1] += osg::DegreesToRadians(cc->m_fAngle);
				if (ea.getKey()== 0xFF51)//left arrow, Left roll
					cc->m_vRotation._v[1] -= osg::DegreesToRadians(cc->m_fAngle);
				break;
				//return false;
			}
			case(osgGA::GUIEventAdapter::KEYUP):
			{
				if(ea.getKey() == 'q')//upwards
					stateBits &= (~moveUpward);
				if(ea.getKey () == 'e')//downwards
					stateBits &= (~moveDownward);
				if (ea.getKey () == 0x41 || ea.getKey () == 0x61)//'a'move to left
					stateBits &= (~moveLeft);
				if (ea.getKey () == 0x44||ea.getKey () == 0x64)//'d'move to right
					stateBits &= (~moveRight);
				if(ea.getKey()==0X57 || ea.getKey()==0X77)//'w'move to front
					stateBits &= (~moveForward);
				if(ea.getKey()==0X53 || ea.getKey()==0X73 )//'s' move to back
					stateBits &= (~moveBackward);
				break;
			}
		case(osgGA::GUIEventAdapter::SCROLL):
			{
				osg::Camera* cm = view->getCamera();
				double mfov = 0, ratio = 0, far = 0, near = 0;
				cm->getProjectionMatrixAsPerspective(mfov, ratio, near, far);
				if(ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN && mfov < 45){
					mfov += 2.0;
					cm->setProjectionMatrixAsPerspective(mfov, ratio, near, far);
					//view->setCamera(cm);
				}else if(ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP && mfov > 25){
					mfov -= 2.0;
					cm->setProjectionMatrixAsPerspective(mfov, ratio, near, far);
					//view->setCamera(cm);
				}
				cc->fov = mfov;
				return false;//Ĭ�Ϸ���ֵ
			}
		case(osgGA::GUIEventAdapter::PUSH):
			if(ea.getButton()==4)
			{
				m_fpushX=mouseX;
				m_fpushY=mouseY;
				m_bRightButtonDown=true;
				dragDelta = 0.15;
				rightDragDistX = 0.0;
				rightDragDistY = 0.0;
			}
			return false;
		case(osgGA::GUIEventAdapter::DRAG):
			if(m_bRightButtonDown)
			{	
				//in one drag process, mouseX alwasy change, while m_fpushX do not change until next PUSH
				cc->m_vRotation._v[2]-=osg::DegreesToRadians(cc->m_fAngle*(mouseX-m_fpushX - rightDragDistX)*dragDelta);
				cc->m_vRotation._v[0]+=osg::DegreesToRadians(cc->m_fAngle*(mouseY-m_fpushY - rightDragDistY)*dragDelta);

				rightDragDistX = mouseX - m_fpushX;
				rightDragDistY = mouseY - m_fpushY;

				if(cc->m_vRotation._v[0]>=3.14)
					cc->m_vRotation._v[0]=3.14;
				if(cc->m_vRotation._v[0]<=0)
					cc->m_vRotation._v[0]=0;
			}
			return false;
		case(osgGA::GUIEventAdapter::RELEASE):
			if(ea.getButton()==4)
			{
				m_bRightButtonDown=false;
			}
			return false;
		default:
			{
				
			return false;
			}
	}
}
CameraContext* TravelManipulator::getCameraContext(){
	/*CameraContext ret;
	ret.m_fAngle = m_fAngle;
	ret.m_fMoveSpeed = m_fMoveSpeed;
	ret.m_vPosition = m_vPosition;
	ret.m_vRotation = m_vRotation;
	ret.flymode = flymode;
	ret.lowmode = lowmode;
	ret.peng = peng;
	ret.max_height = max_height;
	ret.min_height = min_height;
	ret.keepin = keepInBorder;
	ret.keepout = keepOutBorder;
	ret.fov = fov;*/
	return cc;
}

void TravelManipulator::setCameraContext(CameraContext* context){
	/*m_fAngle = context.m_fAngle;
	m_fMoveSpeed = context.m_fMoveSpeed;
	m_vPosition = context.m_vPosition;
	m_vRotation = context.m_vRotation;
	flymode = context.flymode;
	lowmode = context.lowmode;
	peng = context.peng;
	min_height = context.min_height;
	max_height = context.max_height;
	keepInBorder = context.keepin;
	keepOutBorder = context.keepout;
	fov = context.fov;*/
	cc = context;
}

void TravelManipulator::resetStateBits(){
	stateBits = 0x0;
}

void TravelManipulator::setRecordPath(bool rec, string savepath){
	recordPath = rec;
	if(recordPath == false){
		if(savepath.compare("") != 0){
			ofstream out(savepath.c_str());
			animationPath->write(out);
		}
		frameCount = 0;
		timeRecord = 0;
		animationPath->clear();
	}else if(recordPath == true){
		animationPath->clear();
		timeRecord = clock();
	}
}

void TravelManipulator::setPlayPath(bool ply, string strPlayPath){
	playPath = ply;
	if(playPath == true){
		animationPath->clear();
		animationPath->read(ifstream(strPlayPath.c_str()));
		pointMap = animationPath->getTimeControlPointMap();
		pointMapIt = pointMap.begin();
		//osg::ref_ptr<osg::AnimationPathCallback>apcb = dynamic_cast<osg::AnimationPathCallback*>(view->getCamera()->getUpdateCallback());
		//view->getc->setUpdateCallback(new osg::AnimationPathCallback(animationPath.get(), 0.0f, 1.0f));
	}
}

void TravelManipulator::switchForbidMove(){
	if(forbidMove == true)
		forbidMove = false;
	else
		forbidMove = true;
}

