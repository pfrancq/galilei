/*

	Main.cpp

	Main program - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <stdexcept>
#include <iostream>
using namespace std;


//-----------------------------------------------------------------------------
// include files for R
#include <rstd.h>
#include <rdate.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>


//-----------------------------------------------------------------------------
// include files for current application
#include <kgalileicenter.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	"This application provides a control center for the GALILEI project.";


//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE,"");

    // Information about the application
	KAboutData aboutData("kgalileicenter",0,ki18n("KGALILEICenter"),"1.89",ki18n(description),
			KAboutData::License_GPL,ki18n("(C) 1998-2010 by Pascal Francq\n(C) 1998-2008 by the Université Libre de Bruxelles (ULB)"),
			KLocalizedString(),"http://www.otlet-institute.org", "pascal@francq.info");
	aboutData.addAuthor(ki18n("Pascal Francq"),ki18n("Project Manager"),"pascal@francq.info");
	aboutData.addCredit(ki18n("Faiza Gaultier"),ki18n("Senior Researcher"),"fabbaci@ulb.ac.be");
	aboutData.addCredit(ki18n("Nicolas Kumps"),ki18n("Past Researcher"));
	aboutData.addCredit(ki18n("Stéphane Rideau"),ki18n("Past Researcher"));
	aboutData.addCredit(ki18n("Jean-Baptiste Valsamis"),ki18n("Past Researcher"),"jvalsami@ulb.ac.be");
	aboutData.addCredit(ki18n("Valery Vandaele"),ki18n("Past Researcher"));
	aboutData.addCredit(ki18n("David Wartel"),ki18n("Past Researcher"));
	aboutData.addCredit(ki18n("Xavier Sacré"),ki18n("CMake Support"));
	aboutData.addCredit(ki18n("Marco Saerens"),ki18n("Professor"),"saerens@isys.ucl.ac.be");
	aboutData.addCredit(ki18n("Marjorie Paternostre"),ki18n("Past Researcher"));
	aboutData.addCredit(ki18n("Sarah Rolfo"),ki18n("Past Researcher"));

	// Init
	KCmdLineArgs::init( argc, argv, &aboutData );
    KCmdLineOptions options;
	KCmdLineArgs::addCmdLineOptions(options);

	// Run
	try
	{
		KApplication app;
		KGALILEICenter* Center=new KGALILEICenter(argc,argv);
		if(app.isSessionRestored())
		{
//			RESTORE(Center);
		}
		else
		{
			Center->Execute();
		}
		return(app.exec());
	}
	catch(GException& e)
	{
		cout<<e.GetMsg()<<endl;
	}
	catch(RException& e)
	{
		cout<<e.GetMsg()<<endl;
	}
	catch(exception& e)
	{
		cout<<e.what()<<endl;
	}
	catch(...)
	{
		cout<<"Unknown problem"<<endl;
	}
 	return(0);
}
