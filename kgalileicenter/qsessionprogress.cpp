/*

	GALILEI Research Project

	QSessionProgressDlg.cpp

	Dialog Box to show the progress of the something done on a session  - Implementation.

	(C) 2001 by P. Francq.

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
// include files for GALILEI
#include <docs/gdoc.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocxml.h>
#include <profiles/guser.h>
#include <sessions/gsession.h>
#include <profiles/gprofilecalc.h>
#include <groups/ggrouping.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for current project
#include "qsessionprogress.h"




//-----------------------------------------------------------------------------
//
// class QSessionProgressDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QSessionProgressDlg::QSessionProgressDlg( QWidget* parent,GSession* s,const char* c)
    : QSemiModal(parent,"QSessionProgressDlg",true), GGroupingSignalsReceiver(),
      GSessionSignalsReceiver(), Session(s)
{
	resize( 311, 78 );
	setCaption(i18n(c));

	btnOk = new QPushButton( this, "buttonOk_2" );
	btnOk->setGeometry( QRect( 110, 50, 80, 22 ) );
	btnOk->setText( i18n( "&OK" ) );
	btnOk->setAutoDefault( TRUE );
	btnOk->setDefault( TRUE );

	Line = new QFrame( this, "Line1" );
	Line->setGeometry( QRect( 0, 30, 311, 20 ) );
	Line->setFrameStyle( QFrame::HLine | QFrame::Sunken );

	txtRem = new QLabel( this, "txtRem" );
	txtRem->setGeometry( QRect( 10, 10, 281, 20 ) );

	connect(btnOk,SIGNAL(clicked()),this,SLOT(close()));
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::LoadSession(unsigned int cmd)
{
	btnOk->setEnabled(false);
	show();

	// Look if dics must be loaded)
	if((cmd==0)||(cmd==1))
	{
		txtRem->setText("Loading Dicionnaries/Stoplists ...");
		KApplication::kApplication()->processEvents();
		Session->InitDics();
	}

	// Look if docs must be loaded)
	if((cmd==0)||(cmd==1)||(cmd==2)||(cmd==4))
	{
		txtRem->setText("Loading Documents ...");
		KApplication::kApplication()->processEvents();
		Session->InitDocs();
	}

	// Look if groups must be loaded
	if((cmd==0)||(cmd==2)||(cmd==3)||(cmd==4))
	{
		txtRem->setText("Load Groups ...");
		KApplication::kApplication()->processEvents();
		Session->InitGroups();
	}

	// Look if users must be loaded
	if((cmd==0)||(cmd==2)||(cmd==3))
	{
		txtRem->setText("Load Users/Profiles/SubProfiles ...");
		KApplication::kApplication()->processEvents();
		Session->InitUsers();
	}

	// Look if users' feedback must be loaded
	if((cmd==0)||(cmd==2))
	{
		txtRem->setText("Load Users' Feedbacks ...");
		KApplication::kApplication()->processEvents();
		Session->InitFdbks();
	}

	txtRem->setText("Finish");
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::CreateDocXML(GDocXML* &xml,GDoc* doc)
{
	btnOk->setEnabled(false);
	show();
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
		txtRem->setText(QString("Error: ")+e.GetMsg());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::AnalyseXML(GDocXML* &xml,GDoc* doc)
{
	btnOk->setEnabled(false);
	show();
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
		Session->GetDocAnalyse()->Analyse(xml,doc);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText(QString("Error: ")+e.GetMsg());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::AnalyseDocs(bool modified)
{
	btnOk->setEnabled(false);
	show();
	try
	{
		txtRem->setText("Analyse Documents ...");
		KApplication::kApplication()->processEvents();
		Session->AnalyseDocs(this,modified);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText(QString("Error: ")+e.GetMsg());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::ComputeProfile(GProfile* prof,GProfileCalc* method)
{
	btnOk->setEnabled(false);
	show();
	try
	{
		txtRem->setText(QString("Compute Profile'")+prof->GetName()+"' ...");
		KApplication::kApplication()->processEvents();
		method->Compute(prof);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText(QString("Error: ")+e.GetMsg());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::ComputeProfiles(GProfileCalc* method,bool modified)
{
	btnOk->setEnabled(false);
	show();
	try
	{
		txtRem->setText("Compute Profiles ...");
		KApplication::kApplication()->processEvents();
		Session->CalcProfiles(this,method,modified);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText(QString("Error: ")+e.GetMsg());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::GroupProfiles(GGrouping* method,bool modified)
{
	btnOk->setEnabled(false);
	show();
	try
	{
		txtRem->setText("Groups Profiles ...");
		KApplication::kApplication()->processEvents();
		method->Grouping(this,modified);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText(QString("Error: ")+e.GetMsg());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::ComputeAll(GProfileCalc* method,GGrouping* grpmethod,bool modified)
{
	btnOk->setEnabled(false);
	show();
	try
	{
		txtRem->setText("Analyse Documents ...");
		KApplication::kApplication()->processEvents();
		Session->AnalyseDocs(this,modified);
		txtRem->setText("Compute Profiles ...");
		KApplication::kApplication()->processEvents();
		Session->CalcProfiles(this,method,modified);
		txtRem->setText("Groups Profiles ...");
		KApplication::kApplication()->processEvents();
		grpmethod->Grouping(this,modified);
		txtRem->setText("Finish");
	}
	catch(GException& e)
	{
		txtRem->setText(QString("Error: ")+e.GetMsg());
	}
	btnOk->setEnabled(true);
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::NextGroupLang(GLang* lang)
{
	txtRem->setText(QString("Groups Profiles for '")+lang->GetName()+"' ...");
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextDoc(const GDoc* doc)
{
	txtRem->setText(QString("Analyse Doc '")+doc->GetName()+"' ...");
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
void QSessionProgressDlg::receiveNextProfile(const GProfile* prof)
{
	txtRem->setText(QString("Analyse Profile '")+prof->GetName()+"' of User '"+prof->GetUser()->GetName()+"' ...");
	KApplication::kApplication()->processEvents();
}


//-----------------------------------------------------------------------------
QSessionProgressDlg::~QSessionProgressDlg(void)
{
}
