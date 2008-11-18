/*

	GALILEI Research Project

	GLangNL_KDE.cpp

	A KDE about box for HTML filter - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Nicolas Kumps (nkumps@ulb.ac.be).

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
	KAboutData aboutData( "nl", 0, ki18n("Dutch Language"),
		"1.0", ki18n("This is the dutch language plugin using the Porter stemming algorithm."), KAboutData::License_GPL,
		ki18n("(c) 1998-2003, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Nicolas Kumps"),ki18n("Maintainer"), "nkumps@ulb.ac.be");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Contributor"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Julien Lamoral"),ki18n("Contributor"), "jlamoral@ulb.ac.be");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
