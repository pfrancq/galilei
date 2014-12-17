/*

	GALILEI Project.

	QGALILEI.h

	Application - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).

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
// include files for R
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QTranslator>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QLocale>


//-----------------------------------------------------------------------------
// include files for current project
#include <qgalilei.h>
#include <qgalileiwin.h>
using namespace R;
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class QGALILEI
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QGALILEI::QGALILEI(int argc, char *argv[])
	: /*QApplication(argc,argv),**/ GGALILEIApp("QGALILEI",argc,argv,true), Main(0)
{
	// Qt translation for default dialogs (QFileDialog) and so on
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
	QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	QApplication::instance()->installTranslator(&qtTranslator);
}


//-----------------------------------------------------------------------------
void QGALILEI::CreateConfig(void)
{
	GGALILEIApp::CreateConfig();
	Config.InsertParam(new RParamValue("ScriptPath",""));
}


//-----------------------------------------------------------------------------
void QGALILEI::Init(void)
{
	GGALILEIApp::Init();
	ScriptPath=Config.Get("ScriptPath");
}


//-----------------------------------------------------------------------------
void QGALILEI::Run(void)
{
 	Main=new QGALILEIWin(this);
	Main->show();
	QApplication::instance()->exec();
}


//-----------------------------------------------------------------------------
void QGALILEI::SetScriptPath(const QString& path)
{
	ScriptPath=FromQString(path);
	Config.Set("ScriptPath",ScriptPath);
}


//-----------------------------------------------------------------------------
QGALILEI::~QGALILEI(void)
{
	if(Main)
	{
		delete Main;
		Main=0;
	}
}

