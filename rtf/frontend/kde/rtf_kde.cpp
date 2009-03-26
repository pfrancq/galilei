/*

	GALILEI Research Project

	RTF_KDE.cpp

	A RTF filter (KDE Part) - Implementation.

	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by Nicolas Kumps.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
	KAboutData aboutData("rtf", 0, ki18n("RTF Filter"),
		"1.0", ki18n("This is the RTF filter for GALILEI."), KAboutData::License_GPL,
		ki18n("(C) 2001-2004 by Nicolas Kumps and Valery Vandaele\n(c) 2001-2008 by the Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Nicolas Kumps"),ki18n("developer"));
	aboutData.addAuthor(ki18n("Valery Vandaele"),ki18n("developer"));
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------


