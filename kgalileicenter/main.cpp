/*

	main.cpp

	Main program - Implementation.

	Copyright 2001 by the Université libre de Bruxelles.

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
#include <cstdlib>
#include <stdexcept>
#include <iostream>
using namespace std;


//-----------------------------------------------------------------------------
// include files for R
#include <rstd/rdate.h>

//-----------------------------------------------------------------------------
// include files for KDE
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kapp.h>
#include <qmessagebox.h>

//-----------------------------------------------------------------------------
// include files for current application
#include "kgalileicenter.h"


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This application provides a control center for the GALILEI project.");



//-----------------------------------------------------------------------------
// Arguments to add at the command prompt
static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { 0, 0, 0 }
};


//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE,"");
	QString year=QString("(c) 1998-")+QString::number(RDate::GetToday().GetYear())+QString(", Université Libre de Bruxelles\nCAD/CAM Department");

	KAboutData aboutData( "kgalileicenter", I18N_NOOP("KGALILEICenter"),
		VERSION, description, KAboutData::License_GPL,year, 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Project Manager"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Nicolas Kumps",I18N_NOOP("Researcher"), "nkumps@ulb.ac.be");
	aboutData.addAuthor("Marjorie Paternostre",I18N_NOOP("Researcher"), "Marjorie.Paternostre@ulb.ac.be");
	aboutData.addAuthor("Stéphane Rideau",I18N_NOOP("Researcher"), 0);
	aboutData.addAuthor("Valery Vandaele",I18N_NOOP("Researcher"), "vvandaele@ulb.ac.be");
	aboutData.addAuthor("David Wartel",I18N_NOOP("Researcher"), "dwartel@ulb.ac.be");
	aboutData.addCredit("Marco Saerens",I18N_NOOP("Professor"), "saerens@isys.ucl.ac.be");

	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

	try
	{
		KApplication app;
		if(app.isRestored())
		{
			RESTORE(KGALILEICenterApp());
		}
		else
		{
			KGALILEICenterApp *testmdi = new KGALILEICenterApp();
			testmdi->show();
		}
		return app.exec();
	}
	catch(GALILEI::GException& e)
	{
		cout<<e.GetMsg()<<endl;;
	}
	catch(...)
	{
		cout<<"Unknown problem"<<endl;
	}
 	return(0);
}
