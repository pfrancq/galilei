/*

	GALILEI Research Project

	GLangFR_KDE.cpp

	A KDE about box for Danish language - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#include <gfilter.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "dk", 0, ki18n("Danish Language"),
		"1.0", ki18n("This is the danish language plugin using a stemming algorithm based on snowball algorithms."), KAboutData::License_GPL,
		ki18n("(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("David Wartel"),ki18n("Maintainer"), "dwartel@ulb.ac.be");
	aboutData.addAuthor(ki18n("Blake Madden"),ki18n("C++ stemming algorithm writter"), "madden_blake@hotmail.com");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
