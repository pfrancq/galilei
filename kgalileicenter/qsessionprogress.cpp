/*

	GALILEI Research Project

	QSessionProgressDlg.cpp

	Dialog Box to show the progress of the something done on a session  - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <klocale.h>
#include <kapplication.h>

//-----------------------------------------------------------------------------
// include files for Qt
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <sessions/gstorage.h>
#include <docs/gdoc.h>
#include <infos/glang.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocxml.h>
#include <docs/gpostdoc.h>
#include <profiles/guser.h>
#include <sessions/gsession.h>
#include <profiles/gprofile.h>
#include <profiles/gprofilecalc.h>
#include <groups/ggrouping.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for current project
#include "qsessionprogress.h"






//-----------------------------------------------------------------------------
//
// class QSessionProgressDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionProgressDlg::QSessionProgressDlg(QWidget* parent,GSession* s,const char* c) throw(std::bad_alloc,RException)
    : QSemiModal(parent,"QSessionProgressDlg",true), GSlot(), Session(s)
{
	resize(600, 78 );
	setCaption(i18n(c));

	btnOk = new QPushButton( this, "buttonOk_2" );
	btnOk->setGeometry( QRect( 260, 50, 80, 22 ) );
	btnOk->setText( i18n( "&OK" ) );
	btnOk->setAutoDefault( TRUE );
	btnOk->setDefault( TRUE );

	Line = new QFrame( this, "Line1" );
	Line->setGeometry( QRect( 0, 30, 600, 20 ) );
	Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );

	txtRem = new QLabel( this, "txtRem" );
	txtRem->setGeometry( QRect( 10, 10, 580, 20 ) );
	txtRem->setText(c+QString(" ..."));

	connect(btnOk,SIGNAL(clicked()),this,SLOT(close()));
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::LoadSession(GLangManager* langs,GFilterManager* umng, GDocAnalyseManager* dmng,GProfileCalcManager* pmng, GGroupingManager* gmng, GGroupCalcManager* gcmng,
		GStatsCalcManager* smng, GLinkCalcManager* lmng, GPostDocManager* pdmng, GPostGroupManager* pgmng) throw(GException,std::bad_alloc)
{
	btnOk->setEnabled(false);
	show();
	KApplication::kApplication()->processEvents();

	txtRem->setText("Connect (Loading Dicionnaries/Stoplists) ...");
	KApplication::kApplication()->processEvents();
	Session->Connect(langs,umng,dmng,pmng,gmng,gcmng,smng,pdmng, pgmng);

	Session->GetStorage()->LoadSubjectTree(Session);

	txtRem->setText("Loading Documents ...");
	KApplication::kApplication()->processEvents();
	Session->GetStorage()->LoadDocs(Session);

	txtRem->setText("Load Groups ...");
	KApplication::kApplication()->processEvents();
	Session->GetStorage()->LoadGroups(Session);

	txtRem->setText("Load Users/Profiles/SubProfiles ...");
	KApplication::kApplication()->processEvents();
	Session->GetStorage()->LoadUsers(Session);
	Session->GetStorage()->LoadIdealGroupment(Session);

	txtRem->setText("Load Users Feedbacks ...");
	KApplication::kApplication()->processEvents();
	Session->GetStorage()->LoadFdbks(Session);

	txtRem->setText("Post Connect ...");
	KApplication::kApplication()->processEvents();
	Session->PostConnect(lmng);

	txtRem->setText("Finish");
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::CreateDocXML(GDocXML* &xml,GDoc* doc)
{
	btnOk->setEnabled(false);
	show();
	KApplication::kApplication()->processEvents();

	txtRem->setText("Creating XML Structure ...");
	KApplication::kApplication()->processEvents();
	if(xml)
	{
		delete xml;
		xml=0;
	}
	try
	{
		xml=Session->CreateDocXML(doc);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText((RString("Error: ")+e.GetMsg()).Latin1());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::AnalyseXML(GDocXML* &xml,GDoc* doc)
{
	btnOk->setEnabled(false);
	show();
	KApplication::kApplication()->processEvents();

	try
	{
		if(!xml)
		{
			txtRem->setText("Creating XML Structure ...");
			KApplication::kApplication()->processEvents();
			xml=Session->CreateDocXML(doc);
		}
		txtRem->setText("Analysing XML Structure ...");
		KApplication::kApplication()->processEvents();
		//doc->Analyse(xml,Session);
		if(!Session->GetDocAnalyseMng()->GetCurrentMethod())
			 txtRem->setText( RString("Error: No Text Analyse method chosen.").Latin1());
		else
		{
			Session->GetDocAnalyseMng()->GetCurrentMethod()->Analyse(xml,doc);
			txtRem->setText("Finish");
		}
	}
	catch(GException& e)
	{
		txtRem->setText((RString("Error: ")+e.GetMsg()).Latin1());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::AnalyseDocs(bool modified)
{
	btnOk->setEnabled(false);
	show();
	KApplication::kApplication()->processEvents();

	try
	{
		txtRem->setText("Analyse Documents ...");
		KApplication::kApplication()->processEvents();
		Session->AnalyseDocs(this,modified);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText((RString("Error: ")+e.GetMsg()).Latin1());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::ComputeProfiles(bool modified,bool save)
{
	btnOk->setEnabled(false);
	show();
	KApplication::kApplication()->processEvents();

	try
	{
		txtRem->setText("Compute Profiles ...");
		KApplication::kApplication()->processEvents();
		Session->CalcProfiles(this,modified,save);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText((RString("Error: ")+e.GetMsg()).Latin1());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::GroupProfiles(bool modified,bool save)
{
	btnOk->setEnabled(false);
	show();
	KApplication::kApplication()->processEvents();

	try
	{
		txtRem->setText("Groups Profiles ...");
		KApplication::kApplication()->processEvents();
		Session->GroupingProfiles(this,modified,save);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText((RString("Error: ")+e.GetMsg()).Latin1());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::ComputeAll(bool modified,bool save)
{
	btnOk->setEnabled(false);
	show();
	KApplication::kApplication()->processEvents();

	try
	{
		txtRem->setText("Analyse Documents ...");
		KApplication::kApplication()->processEvents();
		Session->AnalyseDocs(this,modified);
		txtRem->setText("Compute Profiles ...");
		KApplication::kApplication()->processEvents();
		Session->CalcProfiles(this,modified,save);
		txtRem->setText("Groups Profiles ...");
		KApplication::kApplication()->processEvents();
		Session->GroupingProfiles(this,modified,save);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText((RString("Error: ")+e.GetMsg()).Latin1());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextGroupLang(const GLang* lang) throw(std::bad_alloc,RException)
{
	txtRem->setText(QString("Groups Profiles for '")+lang->GetName()+"' ...");
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextDoc(const GDoc* doc) throw(std::bad_alloc,RException)
{
	txtRem->setText(QString("Analyse Doc '")+doc->GetName().Latin1()+"' ...");
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextProfile(const GProfile* prof) throw(std::bad_alloc,RException)
{
	txtRem->setText(QString("Analyse Profile '")+prof->GetName().Latin1()+"' of User '"+prof->GetUser()->GetName().Latin1()+"' ...");
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextChromosome(unsigned int id) throw(std::bad_alloc,RException)
{
	char tmp[50];

	sprintf(tmp,"Analyse Chromosome n°%u ...",id);
	txtRem->setText(tmp);
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::PutText(const char* text)
{
	txtRem->setText(text);
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::Begin(void)
{
	txtRem->setText("Finish");
	btnOk->setEnabled(false);
	show();
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::Finish(void)
{
	txtRem->setText("Finish");
	btnOk->setEnabled(true);
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
QSessionProgressDlg::~QSessionProgressDlg(void)
{
//	hide();
}
