/*

	GALILEI Research Project

	KViewPrg.cpp

	Window to run a program - Implementation.

	(C) 2002 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for Qt
#include<qmultilineedit.h>
#include<qpixmap.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapplication.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewprg.h"



//-----------------------------------------------------------------------------
//
// class KViewPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewPrg::KViewPrg(KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), GSlot()
{
	// Window proprieties
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/mimetypes/make.png"));
	setCaption(name);

	Output=new QMultiLineEdit(this,"Output");
	Output->setReadOnly(true);
	Output->resize(size());
}


//-----------------------------------------------------------------------------
void KViewPrg::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
void KViewPrg::WriteStr(const char* str)
{
	Output->insertLine(str);
	KApplication::kApplication()->processEvents(1000);
}


//-----------------------------------------------------------------------------
void KViewPrg::resizeEvent(QResizeEvent *)
{
	Output->resize(size());
}


//-----------------------------------------------------------------------------
bool KViewPrg::close(void)
{
	delete this;
	return(true);
}


//-----------------------------------------------------------------------------
KViewPrg::~KViewPrg(void)
{
}
