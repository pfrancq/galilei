/*

	GALILEI Research Project

	GLangFR_KDE.cpp

	A KDE about box for Danish language - Implementation.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gfilter.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This is the danish language plugin using a stemming algorithm based on snowball algorithms.");



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "dk", I18N_NOOP("Danish Language"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("David Wartel",I18N_NOOP("Maintainer"), "dwartel@ulb.ac.be");
	aboutData.addAuthor("Blake Madden",I18N_NOOP("C++ stemming algorithm writter"), "madden_blake@hotmail.com");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
