#include "TextPanel.h"

/*
textpanel(MatrixTransform)-geode-geom
			                    -content

in default draw textpanel on YZ plain
*/
string TextPanel::standardWidth = "                              ";
TextPanel::TextPanel(void):osg::MatrixTransform()
{
	content = new osgText::Text;
	font = new osgText::Font();
	geode = new osg::Geode();
	content->setDataVariance(osg::Object::DYNAMIC); 
	this->addChild(geode);
}


TextPanel::~TextPanel(void)
{
}

void TextPanel::addContent(float width, float height, float size,  
							 string fontpath){
	osg::Vec3Array* nodeList = new osg::Vec3Array();
	nodeList->push_back(osg::Vec3(0, 0, 0));//make zero point on the left top
	nodeList->push_back(osg::Vec3(0.0f, width, 0.0f));
	nodeList->push_back(osg::Vec3(0.0f, width, height*(-1)));
	nodeList->push_back(osg::Vec3(0.0f, 0.0f, height*(-1)));

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.4, 0.8, 0.4, 0.6));

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
	geom->setVertexArray(nodeList);
	geom->setColorArray(colors);
	geom->setColorBinding( osg::Geometry::BIND_OVERALL);
	osg::StateSet* stateset = geom->getOrCreateStateSet(); 
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->addDrawable(geom.get());

	font = osgText::readFontFile(fontpath);
	setupProperties(*content, font, size, osg::Vec3(0.2, size*0.5, (-1)*size*0.5));

	geode->addDrawable(content.get());
}

void TextPanel::setupProperties(osgText::Text& textObject,
								osgText::Font* font, 
								float size,const osg::Vec3& offset ){
   	textObject.setFont(font);
	textObject.setCharacterSize(size);//字体大小
	textObject.setPosition(offset);
	textObject.setColor(osg::Vec4(0.8, 1.0, 0.9, 1.0));
	textObject.setAlignment(osgText::Text::LEFT_TOP);
	textObject.setAutoRotateToScreen(true);//跟随视角不断变化，但离物体越远，文字越小，和现实当中像类似
	textObject.setAxisAlignment(osgText::Text::YZ_PLANE);
}

void TextPanel::updateContent(map<string,string, MyCompRule>* dbmap){
	string contentString = "";
	for(map<string, string, MyCompRule>::iterator it = dbmap->begin();
		it != dbmap->end();
		it++)
	{
		if(dbmap->size() < 6){
			contentString += it->first + ":"+ it->second + "\n";
		}else{
			string temp = it->first + ":"+ it->second;
			contentString +=temp + standardWidth.substr(temp.length(), standardWidth.length() - temp.length());
			it++;
			if(it == dbmap->end())
				break;
			contentString += it->first + ":"+ it->second + "\n";
		}

	}
	int requiredSize = mbstowcs(NULL, contentString.c_str(), 0);
	wchar_t * wtext = new wchar_t[requiredSize+1];
	mbstowcs(wtext , contentString.c_str(), requiredSize+1);
	content->setText(wtext);
	delete wtext;
}