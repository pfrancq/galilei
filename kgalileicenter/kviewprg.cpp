/*

	GALILEI Research Project

	KViewPrg.cpp

	Window to run a program - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewprg.h"



//-----------------------------------------------------------------------------
//
// class KViewPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewPrg::KViewPrg(KDoc* doc, QWidget* parent,const char* name,int wflags) throw(std::bad_alloc,RException)
	: KView(doc,parent,name,wflags), GSlot()
{
	// Window proprieties
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("make.png",KIcon::Small)));
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
void KViewPrg::receiveNextDoc(const GDoc*) throw(std::bad_alloc,RException)
{
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::receiveNextProfile(const GProfile*) throw(std::bad_alloc,RException)
{
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::NextGroupLang(const GLang*) throw(std::bad_alloc,RException)
{
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::WriteStr(const char* str) throw(std::bad_alloc,RException)
{
	Output->insertLine(str);
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void KViewPrg::resizeEvent(QResizeEvent *)
{
	Output->resize(size());
}


//-----------------------------------------------------------------------------
KViewPrg::~KViewPrg(void)
{
}
