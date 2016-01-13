#pragma once
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgText/Text>
#include <string>
#include <iostream>
#include "Global.h"

using namespace std;
class TextPanel:public osg::MatrixTransform
{
public:
	TextPanel(void);
	~TextPanel(void);
	//void updateContent(string str);
	void updateContent(map<string, string, MyCompRule>* m);
	void addYZContent(const osg::Vec3& pos, float widht, float height, float size ,bool minus = false, string fontpath = "simhei.ttf");
	void addXZContent(const osg::Vec3& pos, float widht, float height, float size ,bool minus = false, string fontpath = "simhei.ttf");
	
private:
	void addContent(const osg::Vec3& pos, osg::Vec3Array* nodeList, osgText::TextBase::AxisAlignment align, float& width, float& height, float& size ,bool minus = false);
	void setupProperties(osgText::Text& textObject,osgText::Font* font, osgText::TextBase::AxisAlignment align, float size,const osg::Vec3& pos );

	osg::ref_ptr<osg::Geode> geode;
	osg::ref_ptr<osgText::Text> content;
	osg::ref_ptr<osgText::Font> font;
	string textString; 
	string timeString;
	//float width;
	//float height;
};
