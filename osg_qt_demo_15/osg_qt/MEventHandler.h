#pragma once
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgViewer/Viewer>
#include <osgText/Text>
#include <time.h>
#include <string>
#include <stdio.h>

using namespace std;
class MEventHandler:public osgGA::GUIEventHandler
{
public:
	MEventHandler(osgText::Text* txt = NULL){
		text = txt;
	}
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa){
		osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
		if(!viewer)return false;
		
		switch(ea.getEventType()){
		case osgGA::GUIEventAdapter::FRAME:
			{
				/*rawtime = time(NULL);
				struct tm* ucttime = localtime(&rawtime);
				timestr = asctime(ucttime);
				string stime = string(timestr);
				size_t p = stime.rfind(" ");
				stime = stime.substr(0, p);
				p = stime.rfind(" ");
				stime = stime.substr(p, stime.length());
				text->setText(stime);*/
			}
			break;
		default:
			break;
		}
		return false;
	}
private:
	osgText::Text* text;
	char* timestr;
	time_t rawtime;
};

