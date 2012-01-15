/*

	GALILEI Research Project

	LangAR_KDE.cpp

	Arabic Language (KDE Part) - Implementation.

	Copyright 2004-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2004 by David Wartel.
	Copyright 2004 by Sarah Rolfo.
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R Project
#include <rqt.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>
using namespace GALILEI;



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "ar", 0, ki18n("Arabic Language"),
		"1.5", ki18n("This is the Arabic language plug-in using a Porter-based stemming algorithm"), KAboutData::License_GPL,
		ki18n("(C) 2004-2012 by Pascal Francq\n(C) 2004 by David Wartel and Sarah Rolfo\n(c) 2004-2008 by the Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("David Wartel"),ki18n("Developer"));
	aboutData.addAuthor(ki18n("Sarah Rolfo"),ki18n("Linguist"));
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
