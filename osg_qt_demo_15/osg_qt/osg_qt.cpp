#include "osg_qt.h"

osg_qt::osg_qt(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
}

osg_qt::~osg_qt()
{

}
