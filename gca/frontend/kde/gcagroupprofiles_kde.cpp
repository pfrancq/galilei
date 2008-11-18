/*

	Genetic Community Algorithm

	GCAGrouping_KDE.cpp

	Config and about dialogs - Implementation.

	Copyright 2003-2007 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <ggroupprofiles.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <gaconfig_kde.h>



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "GCA", 0, ki18n("Genetic Community Algorithm"),
		"1.0",ki18n("This is the Genetic Community Algorithm for GALILEI."), KAboutData::License_GPL,
		ki18n("(c) 1998-2005, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pfrancq@ulb.ac.be");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryGroupProfiles* params)
{
	GAConfigDlg dlg;
	if(dlg.Configure(params))
		params->Apply();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------

