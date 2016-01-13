#include "TextPanel.h"


TextPanel::TextPanel(void):osg::MatrixTransform()
{
	content = new osgText::Text;
	font = new osgText::Font();
	geode = new osg::Geode();
	content->setDataVariance(osg::Object::DYNAMIC); 
	this->addChild(geode);
	timeString = "";
	textString = "床前明月光；\n疑是地上霜；\n举头望明月；\n低头思故乡；\n";
	//width = 1.0;
	//height = 1.0;
}


TextPanel::~TextPanel(void)
{
}

void TextPanel::addYZContent(const osg::Vec3& leftupCorner, 
							 float width, float height, 
							 float size, bool minus, 
							 string fontpath){
	osg::Vec3Array* nodeList = new osg::Vec3Array();
	nodeList->push_back(leftupCorner);
	nodeList->push_back(leftupCorner + osg::Vec3(0.0f, width, 0.0f));
	nodeList->push_back(leftupCorner + osg::Vec3(0.0f, width, height));
	nodeList->push_back(leftupCorner + osg::Vec3(0.0f, 0.0f, height));

	font = osgText::readFontFile(fontpath);
	addContent(leftupCorner, nodeList,  osgText::Text::YZ_PLANE, width, height,size, minus);
}

void TextPanel::addXZContent(const osg::Vec3& leftupCorner, 
							 float width, float height, 
							 float size, bool minus, 
							 string fontpath){
	osg::Vec3Array* nodeList = new osg::Vec3Array();
	nodeList->push_back(leftupCorner);
	nodeList->push_back(leftupCorner + osg::Vec3(width, 0.0f, 0.0f));
	nodeList->push_back(leftupCorner + osg::Vec3(width,0.0f, height));
	nodeList->push_back(leftupCorner + osg::Vec3(0.0f, 0.0f, height));

	font = osgText::readFontFile(fontpath);
	addContent(leftupCorner, nodeList, osgText::Text::XZ_PLANE, width, height,  size, minus);
}

void TextPanel::addContent(const osg::Vec3& leftupCorner, 
						   osg::Vec3Array* nodeList, 
						   osgText::TextBase::AxisAlignment align, 
						   float& width, float& height, 
						   float& size, bool minus){
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.4, 0.8, 0.4, 0.6));

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
	geom->setVertexArray(nodeList);
	geom->setColorArray(colors);
	geom->setColorBinding( osg::Geometry::BIND_OVERALL);
	osg::StateSet* stateset = geom->getOrCreateStateSet(); 
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	geode->addDrawable(geom.get());

	
	setupProperties(*content, font, osgText::Text::YZ_PLANE, size, leftupCorner + osg::Vec3(1.0f, 0.0f, height-5));//5 margin to up border

	geode->addDrawable(content.get());
	if(minus == false)
		this->setMatrix(osg::Matrix::rotate(3.141592657, osg::Vec3(0, 0, 1)));
}

//void TextPanel::updateContent(string cont){
//	cont = cont+textString;
//	int requiredSize = mbstowcs(NULL, cont.c_str(), 0);
//	wchar_t * wtext = new wchar_t[requiredSize+1];
//	mbstowcs(wtext , cont.c_str(), requiredSize+1);
//	content->setText(wtext);
//	delete wtext;
//}

void TextPanel::updateContent(map<string,string, MyCompRule>* dbmap){
	string contentString = "";
	for(map<string, string, MyCompRule>::iterator it = dbmap->begin();
		it != dbmap->end();
		it++)
	{
		if(dbmap->size() < 6){
			contentString += it->first + ":"+ it->second + "\n";
		}else{
			contentString += it->first + ":"+ it->second + "        ";
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

void TextPanel::setupProperties(osgText::Text& textObject,
								osgText::Font* font, 
								osgText::TextBase::AxisAlignment align, 
								float size,const osg::Vec3& pos ){
   	textObject.setFont(font);//
	textObject.setCharacterSize(size);//字体大小
	textObject.setPosition(pos);
	textObject.setColor(osg::Vec4(0.8, 1.0, 0.9, 1.0));
	//textObject.setAlignment(osgText::Text::LEFT_BOTTOM);//文字显示方向
	textObject.setAlignment(osgText::Text::LEFT_TOP);
	textObject.setAxisAlignment(osgText::Text::SCREEN);//获取文字对称成方式正对屏幕方向
	textObject.setAutoRotateToScreen(true);//跟随视角不断变化，但离物体越远，文字越小，和现实当中像类似
    textObject.setAxisAlignment(align);//获取文字对称成方式
}
