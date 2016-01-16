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
	void updateContent(map<string, string, MyCompRule>* m);
	void TextPanel::addContent(float width, float height, float size, string fontpath = "simhei.ttf");
	
private:
	void setupProperties(osgText::Text& textObject,osgText::Font* font, float size,const osg::Vec3& pos );

	osg::ref_ptr<osg::Geode> geode;
	osg::ref_ptr<osgText::Text> content;
	osg::ref_ptr<osgText::Font> font;
	static string standardWidth;
};
