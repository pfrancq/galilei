/*

	main.cpp

	Main program - Implementation.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

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
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kapp.h>


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
	KAboutData aboutData( "kgalileicenter", I18N_NOOP("KGALILEICenter"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 1998-2001, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://www.ulb.ac.be/polytech/cfao", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Project Manager"), "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Julien Lamoral",I18N_NOOP("Researcher"), "jlamoral@ulb.ac.be");
	aboutData.addAuthor("Marjorie Paternostre",I18N_NOOP("Researcher"), "Marjorie.Paternostre@ulb.ac.be");
	aboutData.addAuthor("David Wartel",I18N_NOOP("Researcher"), "dwartel@ulb.ac.be");
	aboutData.addCredit("Marco Saerens",I18N_NOOP("Consultant"), "marco.saerens@smals-mvm.be");

	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

	KApplication app;
 
	if(app.isRestored())
	{
		RESTORE(KGALILEICenterApp);
	}
	else
	{
		KGALILEICenterApp *testmdi = new KGALILEICenterApp();
		testmdi->show();
	}
	return app.exec();
}  
