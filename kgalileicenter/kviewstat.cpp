/*

	GALILEI Research Project

	KViewStat.cpp

	Window for the running some instruction to the session - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

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
// include file for R Project
#include <rstd/rstring.h>
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewstat.h"


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlineedit.h>
#include <qlabel.h>
#include <qmultilineedit.h>



//-----------------------------------------------------------------------------
//
// class KStatInst
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class KStatInst
{
public:

	/**
	* Name of the Instruction.
	*/
	RString Name;

	KStatInst(const char* n) : Name(n) {}
	int Compare(const KStatInst* i) const {return(Name.Compare(i->Name));}
	int Compare(const char* i) const {return(Name.Compare(i));}
	int Compare(const RString& i) const {return(Name.Compare(i));}
	virtual void Help(QMultiLineEdit* o)
	{
		o->insertLine(Name());
	}
	virtual void Exec(char* /*param*/,KDoc* /*doc*/,QMultiLineEdit* o,RTextFile* /*f*/)
	{
		o->insertLine(Name());
	}
	virtual ~KStatInst(void) {}
};



//-----------------------------------------------------------------------------
//
// class KStatInstClear
//
//-----------------------------------------------------------------------------
class KStatInstClear : public KStatInst
{
public:
	KStatInstClear(void) : KStatInst("CLEAR") {}
	virtual void Help(QMultiLineEdit* o)
	{
		o->insertLine("CLEAR: Clear the result widget.");
	}
	virtual void Exec(char* /*param*/,KDoc* /*doc*/,QMultiLineEdit* o,RTextFile* /*f*/)
	{
		o->clear();
	}
};



//-----------------------------------------------------------------------------
//
// class KStatInstListUser
//
//-----------------------------------------------------------------------------
class KStatInstUser : public KStatInst
{
public:
	KStatInstUser(void) : KStatInst("USER") {}
	virtual void Help(QMultiLineEdit* o)
	{
		o->insertLine("USER: Show information about a user");
	}
	virtual void Exec(char* param,KDoc* /*doc*/,QMultiLineEdit* o,RTextFile* /*f*/)
	{
		o->insertLine(param);
	}
};



//-----------------------------------------------------------------------------
//
// class KViewStat
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewStat::KViewStat(KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), RContainer<KStatInst,unsigned int,true,true>(20,5)
{
	// Init part
	setCaption("Run Instructions");
	QLabel* l=new QLabel(this,"Label");
	l->setText("Instruction:");
	LabelWidth=l->width()+5;
	l->setGeometry(5,0,l->width(),l->height());
	Cmd=new QLineEdit(this,"Instructions");
	Result=new QMultiLineEdit(this,"Command");
	Result->setReadOnly(true);

	// Connections
	connect(Cmd,SIGNAL(returnPressed()),this,SLOT(slotNewCmd()));

	// Instructions
	InsertPtr(new KStatInstUser());
	InsertPtr(new KStatInstClear());
}


//-----------------------------------------------------------------------------
void KViewStat::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
void KViewStat::slotNewCmd(void)
{
	KStatInst* i;
	RString cmd;
	char* ptr;

	// Extract command and parameters
	sprintf(Buffer,Cmd->text().latin1());
	ptr=Buffer;
	while((*ptr)&&((*ptr)!=' '))
		ptr++;
	if(*ptr) (*(ptr++))=0;
	cmd.StrUpr(Buffer);

	// It is help or about
	if(cmd=="HELP")
	{
		for(Start();!End();Next())
			(*this)()->Help(Result);
		return;
	}
	if(cmd=="ABOUT")
	{
		Result->insertLine("GALILEI Internal Language Version 1.00");
		return;
	}

	// Try to find the instruction and run it
	i=GetPtr<const RString>(cmd);
	if(!i)
	{
		Cmd->clear();
		Result->insertLine(RString("'"+cmd+"' Unknown command")());
		return;
	}
	i->Exec(ptr,Doc,Result,0);
}


//-----------------------------------------------------------------------------
void KViewStat::resizeEvent(QResizeEvent *)
{
	Cmd->setGeometry(LabelWidth,0,width()-LabelWidth,Cmd->height());
	Result->setGeometry(0,Cmd->height(),width(),height()-Cmd->height());
}


//-----------------------------------------------------------------------------
KViewStat::~KViewStat(void)
{
}
