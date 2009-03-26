/*

	GALILEI Research Project

	PDF_KDE.cpp

	A PDF filter (KDE Part) - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).
	Copyright 1996-2005 by Glyph & Cog, LLC.

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
	KAboutData aboutData( "pdf",0, ki18n("PDF Filter"),
		"1.1",ki18n("This is the PDF filter for GALILEI.  It is highly based on the Xpdf project 3.01."), KAboutData::License_GPL,
		ki18n("(C) 2001-2009 by Pascal Francq\n(C) 2001-2008 by the Université Libre de Bruxelles (ULB)\n(C) 1996-2005 by Glyph & Cog, LLC"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	aboutData.addAuthor(ki18n("Glyph & Cog, LLC"),ki18n("Xpdf Project"), "derekn@foolabs.com");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------
