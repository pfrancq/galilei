/*

	GALILEI Project.

	QGALILEI.h

	Application - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).

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
#include <configure.h>
#include <kprgconsole.h>
#include <kviewclass.h>
#include <kviewclasses.h>
#include <kviewdicts.h>
#include <kviewdoc.h>
#include <kviewdocs.h>
#include <kviewgroup.h>
#include <kviewgroups.h>
#include <kviewidealgroups.h>
#include <kviewmetaengine.h>
#include <kviewprg.h>
#include <kviewprofile.h>
#include <kviewstats.h>
#include <kviewusers.h>
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
	Config.InsertParam(new RParamValue("MainWindowWidth",300));
	Config.InsertParam(new RParamValue("MainWindowHeight",300));
	Configure::createOptions(Config);
	KPrgConsole::createOptions(Config);
	KViewClass::createOptions(Config);
	KViewClasses::createOptions(Config);
	KViewDicts::createOptions(Config);
	KViewDoc::createOptions(Config);
	KViewDocs::createOptions(Config);
	KViewTopic::createOptions(Config);
	KViewCommunity::createOptions(Config);
	KViewTopics::createOptions(Config);
	KViewCommunities::createOptions(Config);
	KViewIdealTopics::createOptions(Config);
	KViewIdealCommunities::createOptions(Config);
	KViewIdealClasses::createOptions(Config);
	KViewMetaEngine::createOptions(Config);
	KViewPrg::createOptions(Config);
	KViewProfile::createOptions(Config);
	KViewStats::createOptions(Config);
	KViewUsers::createOptions(Config);
}


//-----------------------------------------------------------------------------
void QGALILEI::Init(void)
{
	GGALILEIApp::Init();
	ScriptPath=Config.Get("ScriptPath");
	Configure::readOptions(Config);
	KPrgConsole::readOptions(Config);
	KViewClass::readOptions(Config);
	KViewClasses::readOptions(Config);
	KViewDicts::readOptions(Config);
	KViewDoc::readOptions(Config);
	KViewDocs::readOptions(Config);
	KViewTopic::readOptions(Config);
	KViewCommunity::readOptions(Config);
	KViewTopics::readOptions(Config);
	KViewCommunities::readOptions(Config);
	KViewIdealTopics::readOptions(Config);
	KViewIdealCommunities::readOptions(Config);
	KViewIdealClasses::readOptions(Config);
	KViewMetaEngine::readOptions(Config);
	KViewPrg::readOptions(Config);
	KViewProfile::readOptions(Config);
	KViewStats::readOptions(Config);
	KViewUsers::readOptions(Config);
}


//-----------------------------------------------------------------------------
void QGALILEI::Run(void)
{
 	Main=new QGALILEIWin(this);
	Main->resize(Config.GetInt("MainWindowWidth"),Config.GetInt("MainWindowHeight"));
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
	Configure::saveOptions(Config);
	KPrgConsole::saveOptions(Config);
	KViewClass::saveOptions(Config);
	KViewClasses::saveOptions(Config);
	KViewDicts::saveOptions(Config);
	KViewDoc::saveOptions(Config);
	KViewDocs::saveOptions(Config);
	KViewTopic::saveOptions(Config);
	KViewCommunity::saveOptions(Config);
	KViewTopics::saveOptions(Config);
	KViewCommunities::saveOptions(Config);
	KViewIdealTopics::saveOptions(Config);
	KViewIdealCommunities::saveOptions(Config);
	KViewIdealClasses::saveOptions(Config);
	KViewMetaEngine::saveOptions(Config);
	KViewPrg::saveOptions(Config);
	KViewProfile::saveOptions(Config);
	KViewStats::saveOptions(Config);
	KViewUsers::saveOptions(Config);
	if(Main)
	{
		Config.SetInt("MainWindowWidth",Main->width());
		Config.SetInt("MainWindowHeight",Main->height());
		delete Main;
		Main=0;
	}
}

