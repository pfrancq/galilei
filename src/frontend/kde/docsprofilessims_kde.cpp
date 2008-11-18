/*

	GALILEI Research Project

	GCommunityCalcGravitation_KDE.cpp

	A KDE about box for the statistical method. - Implementation.

	Copyright 2003-2008 by the Université Libre de Bruxelles.

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
// include files for KDE
#include <kaboutdata.h>
#include <kaboutapplicationdialog.h>
#include <KDE/KLocale>


//------------------------------------------------------------------------------
// include files for GALILEI
#include "genericsims_kde.h"



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "docsprofilessims", 0, ki18n("Cosinus Method"),
		"1.0", ki18n("The cosinus method is used to computed the similarities between the profiles and documents."), KAboutData::License_GPL,
		ki18n("(c) 2005, Université Libre de Bruxelles\nCAD/CAM Department"), KLocalizedString(), "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pfrancq@ulb.ac.be");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}

//------------------------------------------------------------------------------
void Configure(GFactoryMeasure* params)
{
	GGenericSimsDlg dlg("Similarities between documents and profiles");
	dlg.Configure(params);
}


//------------------------------------------------------------------------------
}     // end of extren
//------------------------------------------------------------------------------
