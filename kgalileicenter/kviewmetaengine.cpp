/*

	GALILEI Research Project

	KViewMetaEngine.cpp

	Window to manipulate a specific group - Implementation.

	Copyright 2008-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2001 by Valery Vandaele.
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
// include files for R Project
#include <rstring.h>
#include <rtextfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <rqt.h>
#include <ggalileiapp.h>
#include <gmetaengine.h>
#include <gsession.h>


//-----------------------------------------------------------------------------
// includes files for Qt/KDE
#include <kapplication.h>


//-----------------------------------------------------------------------------
// include files for current application
#include <kviewmetaengine.h>
#include <qsessionprogress.h>



//-----------------------------------------------------------------------------
//
// class QQuery
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class QQuery : public QSessionThread
{
	RString Query;

public:
	QQuery(KGALILEICenter* app,const QString& query) : QSessionThread(app), Query(FromQString(query)) {}
	virtual void DoIt(void)
	{
		App->getSession()->RequestMetaEngine(Query);
	}
};



//-----------------------------------------------------------------------------
//
// class KViewMetaEngine
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewMetaEngine::KViewMetaEngine(KGALILEICenter* app)
	:  QMdiSubWindow(), Ui_KViewMetaEngine(), App(app)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("Query meta engine");
	NbRes->setValue(100);
	connect(Search,SIGNAL(clicked()),this,SLOT(QueryEngine()));
}


//-----------------------------------------------------------------------------
void KViewMetaEngine::showResults(void)
{
	GMetaEngine* Meta(GALILEIApp->GetCurrentPlugIn<GMetaEngine>("MetaEngine"));
	Results->Set(QGObjectsList::Docs,Meta,NbRes->value());
	ResLabel->setText("<b>"+QString::number(Meta->GetNbResults())+" Results displayed.</b>");
}


//-----------------------------------------------------------------------------
void KViewMetaEngine::QueryEngine(void)
{
	// If no keywords specified -->Error
	if(TxtQuery->text().isEmpty())
	{
		ResLabel->setText("<b> Enter first a query!</b>");
		return;
	}
	QSessionProgressDlg Dlg(App,"Search: "+TxtQuery->text());
	QQuery* Task(new QQuery(App,TxtQuery->text()));
	connect(Task,SIGNAL(finish()),this,SLOT(showResults()));
	Dlg.Run(Task);
}
