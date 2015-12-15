#include "GeneralEventHandler.h"


GeneralEventHandler* GeneralEventHandler::instance = 0;
QWidget* GeneralEventHandler::mparent = 0;
bool GeneralEventHandler::infoenable = true;
GeneralEventHandler* GeneralEventHandler::Instance(QWidget* parent){
	if(instance == 0){
		mparent = parent;
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
			if(curSwt->getValueList().size() > 1){
				//osg::Switch::ValueList vl = curSwt->getValueList();
				int numchildren = curSwt->getNumChildren();
				TextPanel* tp;
				//sql command execute here//
				//split sql result into map<field,value> form//
				//setTempData(dbMap);
				for(int i = 1; i< numchildren; i++){
					//rawtime = time(NULL);
					//struct tm* ucttime = localtime(&rawtime);
					//timestr = asctime(ucttime);
					//string stime = string(timestr);
					//size_t p = stime.rfind(" ");
					//stime = stime.substr(0, p);
					//p = stime.rfind(" ");
					//stime = stime.substr(p, stime.length());
					tp = dynamic_cast<TextPanel*>(curSwt->getChild(i));
					tp->updateContent(dbMap->at(i));
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
					if(node!= 0)
						emit selectedPosition(node->getBound().center(), node->getBound().radius());
					
					node=  (nodepath.size() >= 2)?dynamic_cast<osg::Node*>(nodepath[nodepath.size()-3]):0;
					if(node!= 0)
						emit selectedPosition(node->getBound().center(), node->getBound().radius());

					node=  (nodepath.size() >= 1)?dynamic_cast<osg::Node*>(nodepath[nodepath.size()-2]):0;
					if(node!= 0)
						emit selectedPosition(node->getBound().center(), node->getBound().radius());
				}
				emit resetDoshow();
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

void GeneralEventHandler::setDBMap(vector<map<string, string>*>* dbmap){
	if(dbMap != NULL){
		for(int i = 0; i< dbmap->size(); i++){
			delete dbmap->at(i);
		}
		delete dbMap;
	}
	this->dbMap = dbmap;
}

void GeneralEventHandler::setTempData(vector<map<string, string>*>* dbmap){
	for(int i = 0; i< dbmap->size(); i++){
		dbmap->at(i)->at("��ѹ") = "DY";
		dbmap->at(i)->at("����") = "DL";
		dbmap->at(i)->at("�й�����") = "YGGL";
		dbmap->at(i)->at("�޹�����") = "WGGL";
		dbmap->at(i)->at("���ŵ���") = "LCDL";
		dbmap->at(i)->at("���ŵ�ѹ") = "LCDY";
		dbmap->at(i)->at	("����ת��") = "ZJZS";
		dbmap->at(i)->at	("�����¶�") = "DZWD";
		dbmap->at(i)->at	("�ϵ��¶�") = "SDWD";
		dbmap->at(i)->at	("�µ��¶�") = "XDWD";
		dbmap->at(i)->at	("���͸��¶�") = "SYGWD";
		dbmap->at(i)->at	("���͸��¶�") = "XYGWD";
		dbmap->at(i)->at	("�������¶�") = "TLWWD";
		dbmap->at(i)->at	("����") = "PL";
		dbmap->at(i)->at	("���") = "GG";
		dbmap->at(i)->at	("ҶƬ�Ƕ�") = "YPJD";
		dbmap->at(i)->at	("բ��ˮλ") = "ZSSW";
		dbmap->at(i)->at	("������ˮλ") = "DDQSW";
		dbmap->at(i)->at	("բ��ˮλ") = "ZXSW";
	}
}