/*

	GALILEI Research Project

	KViewChromos.cpp

	Window to manipulate a set of chromosomes - Implementation.

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
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <groups/ggroupvector.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <galilei/qlistviewitemtype.h>
#include <infos/giwordweight.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdoc.h"
#include "kviewchromos.h"
#include "qsessionprogress.h"
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class KViewChromos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewChromos::KViewChromos(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	setCaption("Stored Chromosomes");
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/actions/window_new.png"));

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
void KViewChromos::update(unsigned int)
{
}


//-----------------------------------------------------------------------------
void KViewChromos::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}
