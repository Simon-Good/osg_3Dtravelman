#pragma once
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgText/Text>
#include <string>
#include <iostream>

using namespace std;
class TextPanel:public osg::MatrixTransform
{
public:
	TextPanel(void);
	~TextPanel(void);
	void updateContent(string str);
	void updateContent(map<string, string>* m);
	//void updateTime(string stime);
	void setupProperties(osgText::Text& textObject,osgText::Font* font,float size,const osg::Vec3& pos );
	void addYZContent(const osg::Vec3& pos, float widht, float height, bool minus = false, string fontpath = "simhei.ttf");
private:
	osg::ref_ptr<osg::Geode> geode;
	osg::ref_ptr<osgText::Text> content;
	osg::ref_ptr<osgText::Font> font;
	string textString; 
	string timeString;
	float width;
	float height;
};
