/*

	GALILEI Research Project

	SGGAGroupProfiles_KDE.cpp

	SGGA Plug-in for Profiles (KDE Part) - Implementation.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <sggaconfig_kde.h>



//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "sgga_profiles", 0, ki18n("SGGA Plug-in for Profiles"),
		"1.0",ki18n("This is a genetic algorithm to cluster profiles."), KAboutData::License_GPL,
		ki18n("(C) 2003-2012 by Pascal Francq\n(C) 2003-2008 by the Université Libre de Bruxelles (ULB)"),
		KLocalizedString(), "http://www.imrdp.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Maintainer"), "pascal@francq.info");
	KAboutApplicationDialog dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
bool Configure(GPlugIn* fac)
{
	GAConfigDlg dlg;
	return(dlg.Configure(fac));
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------

