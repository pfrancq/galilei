/*

	GALILEI Research Project

	LangHU_KDE.cpp

	Hugarian language (KDE Part) - Implementation.

	Copyright 2001-2009 by the Snowball Project.
	Copyright 2008-2009 by Pascal Francq.

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
#include <glang.h>
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
	KAboutData aboutData("hu", 0, ki18n("Hungarian Language"),
		"1.0", ki18n("This is the swedish language plugin using a stemming algorithm based on snowball algorithms."), KAboutData::License_GPL,
		ki18n("(C) 2001-2009 by the Snowball Project\n(C) 2008-2009 by Pascal Francq"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("David Wartel"),ki18n("Developer"));
	aboutData.addAuthor(ki18n("Snowball Project"),ki18n("Stemming algorithm"));
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
