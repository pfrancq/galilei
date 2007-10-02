/*

	GALILEI Research Project

	KViewDebug.cpp

	Window to manipulate debugging information - Implementation.

	Copyright 2007 by the Universite Libre de Bruxelles.

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
// include files for R Project
#include <rstring.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <rqt.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt/KDE
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdoc.h"
#include "kviewdebug.h"
#include "qsessionprogress.h"



//-----------------------------------------------------------------------------
//
// class KViewDebug
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDebug::KViewDebug(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	setCaption("Debug Information");
    Form1Layout = new QVBoxLayout( this, 11, 6, "Form1Layout");

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1");

    textLabel1 = new QLabel( this, "textLabel1" );
    layout1->addWidget( textLabel1 );

    Object = new QLineEdit( this, "Object" );
    layout1->addWidget( Object );

    Run = new QPushButton( this, "pushButton1" );
    layout1->addWidget(Run);
    Form1Layout->addLayout( layout1 );

    layout2 = new QHBoxLayout( 0, 0, 6, "layout2");

    textLabel2 = new QLabel( this, "textLabel2" );
    layout2->addWidget( textLabel2 );

    Info = new QLineEdit( this, "Info" );
    layout2->addWidget( Info );
    Form1Layout->addLayout( layout2 );

    Results = new QTextEdit( this, "Results" );
    Results->setTextFormat( QTextEdit::RichText );
    Results->setWordWrap( QTextEdit::FixedColumnWidth );
    Results->setReadOnly(true);
    Results->setTextFormat(Qt::PlainText);
    Form1Layout->addWidget( Results );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    connect(Run,SIGNAL(clicked(void)),this,SLOT(run(void)));
}


//-----------------------------------------------------------------------------
void KViewDebug::update(unsigned int)
{
}


//-----------------------------------------------------------------------------
void KViewDebug::languageChange(void)
{
    setCaption( tr( "Debug Information" ) );
    textLabel1->setText( tr( "Object" ) );
    Run->setText( tr( "&Run" ) );
    Run->setAccel( QKeySequence( tr( "Alt+R" ) ) );
    textLabel2->setText( tr( "Information" ) );
}


//-----------------------------------------------------------------------------
void KViewDebug::run(void)
{
	try
	{
		RString res=Doc->GetSession()->GetDebugInfo(FromQString(Object->text()),FromQString(Info->text()));
		Results->append(">Object(\""+Object->text()+"\").Info(\""+Info->text()+"\")");
		Results->append(ToQString(res));
	}
	catch(GException& e)
	{
		Results->append(QString(">")+e.GetMsg());
	}
}


//-----------------------------------------------------------------------------
KViewDebug::~KViewDebug(void)
{
}
