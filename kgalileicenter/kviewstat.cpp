/*

	GALILEI Research Project

	KViewStat.cpp

	Basic Window for the application - Implementation.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// application specific includes
#include "kviewstat.h"



//-----------------------------------------------------------------------------
//
// class KViewStat
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewStat::KViewStat(KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
}


//-----------------------------------------------------------------------------
void KViewStat::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
void KViewStat::resizeEvent(QResizeEvent *)
{
//	Docs->setGeometry(0,0,this->width(),this->height());
//	Docs->setColumnWidth(0,(this->width()/2)-2);
//	Docs->setColumnWidth(1,(this->width()/2)-2);
}


//-----------------------------------------------------------------------------
KViewStat::~KViewStat(void)
{
}
