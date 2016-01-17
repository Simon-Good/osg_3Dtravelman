#include "GeneralEventHandler.h"
#include "DBHandler.h"

GeneralEventHandler* GeneralEventHandler::instance = 0;
bool GeneralEventHandler::infoenable = true;
GeneralEventHandler* GeneralEventHandler::Instance(){
	if(instance == 0){
		instance = new GeneralEventHandler();
	}
	return instance;
}

bool GeneralEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa){
	osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*> (&aa);

	if(viewer==NULL)
		return false;
	
	static int screenCaptureSequence=0;
	switch(ea.getEventType()){
		case osgGA::GUIEventAdapter::FRAME:
		{
			frameCount = frameCount<<1;
			if((frameCount | 0x0) == 0x0){
				frameCount = 0x1;
				if(curSwt!=NULL && curSwt->getValueList().size() > 1){
					int numchildren = curSwt->getNumChildren();
					TextPanel* tp;
					if(DBHandler::Get_dbH_Instance()->get_dbMessage(curSwtIndex,dbMap)==true){
						if(dbMap->size() > 0){
							for(int i = 1; i< numchildren; i++){
								tp = dynamic_cast<TextPanel*>(curSwt->getChild(i));
								tp->updateContent(dbMap->at(i-1));
							}
						}
					}
				}
			}
		}
		break;
		case osgGA::GUIEventAdapter::PUSH:
		{
			if(ea.getButton() == 1 && infoenable == true)
			{
				osgUtil::LineSegmentIntersector::Intersections hits;
				if(viewer->computeIntersections(ea.getX(), ea.getY(), hits)){
					osgUtil::LineSegmentIntersector::Intersection intersection = *hits.begin();
					osg::NodePath& nodepath = intersection.nodePath;
					osg::ref_ptr<osg::Node> node=  (nodepath.size() >= 3)?dynamic_cast<osg::Node*>(nodepath[nodepath.size()-4]):0;
					if(node!= 0){
						cout<<"got click 1"<<endl;
						emit selectedPosition(node->getBound().center().x(), node->getBound().center().y(), node->getBound().radius());
					}
					
					node=  (nodepath.size() >= 2)?dynamic_cast<osg::Node*>(nodepath[nodepath.size()-3]):0;
					if(node!= 0){
						cout<<"got click 2"<<endl;
						emit selectedPosition(node->getBound().center().x(), node->getBound().center().y(), node->getBound().radius());
					}

					node=  (nodepath.size() >= 1)?dynamic_cast<osg::Node*>(nodepath[nodepath.size()-2]):0;
					if(node!= 0){
						cout<<"got click 3"<<endl;
						emit selectedPosition(node->getBound().center().x(), node->getBound().center().y(), node->getBound().radius());
					}
				}
				//emit resetDoshow();
			}
		}
		break;	
		default:
			return false;
	}
	return true;
}

void GeneralEventHandler::infoEnable(bool enable){
	infoenable = enable;
}

void GeneralEventHandler::setCurrentScene(osg::Switch* swt, int index){
	curSwt = swt;
	curSwtIndex = index;
}

void GeneralEventHandler::setDBMap(vector<map<string, string, MyCompRule>*>* dbmap){
	if(dbMap != NULL){
		for(int i = 0; i< dbMap->size(); i++){
			delete dbMap->at(i);
		}
		delete dbMap;
	}
	this->dbMap = dbmap;
}
