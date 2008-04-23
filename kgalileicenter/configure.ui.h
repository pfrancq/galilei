/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

//-----------------------------------------------------------------------------
#include <iostream>
using namespace std;

//-----------------------------------------------------------------------------
#include <kurlrequester.h>
#include <kfiledialog.h>
#include <qmessagebox.h>

//-----------------------------------------------------------------------------
#include <kgalileicenter.h>
#include <kdoc.h>
#include <rqt.h>

void ConfigureDlg::Init(void* param)
{
	KGALILEICenterApp* App=static_cast<KGALILEICenterApp*>(param);

	// General
	PlugInsConfig->setShowLocalProtocol(false);
	PlugInsConfig->setURL(ToQString(App->GetPlugInsConfigName()));
	LogFile->setShowLocalProtocol(false);
	LogFile->setURL(ToQString(App->GetLogFileName()));
	DebugFile->setShowLocalProtocol(false);
	DebugFile->setURL(ToQString(App->GetDebugFileName()));
	PrgPath->setShowLocalProtocol(false);
	PrgPath->setURL(ToQString(App->GetPrgPath()));
	
	// Directories
	R::RCursor<RString> Cur(App->GetPlugInsPath());
	for(Cur.Start();!Cur.End();Cur.Next())
		Dirs->insertItem(ToQString(*Cur()));
}


void ConfigureDlg::Done(void* param)
{
	KGALILEICenterApp* App=static_cast<KGALILEICenterApp*>(param);

	// General
	App->SetPlugInsConfigName(FromQString(PlugInsConfig->url()));
	App->SetLogFileName(FromQString(LogFile->url()));
	App->SetDebugFileName(FromQString(DebugFile->url()));	
	App->SetPrgPath(FromQString(PrgPath->url()));	
	QString debug=DebugFile->url();
	delete App->Debug;
	App->Debug=0;
	if(!debug.isEmpty())
	{
	try
	{
		App->Debug=new RDebugXML(FromQString(debug));
		if(App->Doc)
			App->Doc->GetSession()->SetDebug(App->Debug);
	}
	catch(...)
	{
		QMessageBox::critical(this,"KGALILEICenter","Error while creating '"+debug+"'");
	}
	}
	// Directories
	App->ClearPlugInsPath();
	for(int i=0;i<Dirs->count();i++)
		App->AddPlugInsPath(FromQString(Dirs->text(i)));
}


void ConfigureDlg::newAddDir( void )
{
	QString newdir=KFileDialog::getExistingDirectory(QString::null,this,"Add new directory for plug-ins");
	if((!newdir.isEmpty())&&(!Dirs->findItem(newdir,ExactMatch)))
		Dirs->insertItem(newdir);
}


void ConfigureDlg::newRemoveDir(void)
{
	Dirs->removeItem(Dirs->currentItem());
}
