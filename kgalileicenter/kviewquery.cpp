/*

	GALILEI Research Project

	KViewQuery.cpp

	Window for the running some instruction to the session - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
// include file for Galilei Project
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <langs/glang.h>
#include <langs/gdict.h>
#include <docs/gdoc.h>
#include <sessions/gsession.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <profiles/gprofile.h>
#include <groups/ggroupvector.h>
#include <groups/ggroupcalcgravitation.h>
#include <tests/gsimdocquerygroup.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// Include file for R Project
#include <rstd/rstring.h>
#include <rstd/rtextfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// Application specific includes
#include "kviewquery.h"
#include "kdoc.h"


//-----------------------------------------------------------------------------
// Include files for Qt
#include <qlineedit.h>
#include <qlabel.h>
#include <qmultilineedit.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
//
// class KViewQuery
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewQuery::KViewQuery(KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	// Init part
	QueryWord = new GIWordsWeights(20);
	ses=doc->GetSession();
	setCaption("Run Query");
	QLabel* l=new QLabel(this,"Label");
	l->setText("Query: (ex: +jazz -opera)");
	LabelWidth=l->width()+26;
	l->setGeometry(5,0,l->width(),l->height());
	Cmd=new QLineEdit(this,"Query2");
	Result=new QListView(this);
	Result->addColumn("Group name");
	Result->addColumn("Sim Type");
	Result->addColumn("similarity");
	// Connections
	GSimDocQueryGroup julien(ses);
	connect(Cmd,SIGNAL(returnPressed()),this,SLOT(slotNewQuery()));
}


//-----------------------------------------------------------------------------
void KViewQuery::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
void KViewQuery::CalcSimGroup()
{
	Result->clear();
//	QueryWord->ModifyQuery(otGroup,lang);

	GGroupsCursor idealgroup=ses->GetGroupsCursor();
	for(idealgroup.Start();!idealgroup.End();idealgroup.Next())
	{
		GGroupCursor Group;
		Group=idealgroup()->GetGroupCursor();
		for(Group.Start();!Group.End();Group.Next())
		{
			GGroup* Grp=Group();
			GGroupVector* GrpV=static_cast<GGroupVector*>(Group());
			bool end=true;	
			const char* name;
			for(Grp->Start();end;Grp->Next())
			{
				name=(*Grp)()->GetProfile()->GetName();
				end=false;
			}
			new QListViewItem(Result,QString(name),"Idf",QString::number(QueryWord->SimilarityIdf(GrpV->GetVector(),otGroup,lang)));
			new QListViewItem(Result,QString(name),"Norm",QString::number(QueryWord->Similarity(GrpV->GetVector())));
		}
	}
}


//-----------------------------------------------------------------------------
void KViewQuery::AddWord(char* word,bool weight)
{
	RString Stem;
	RString NoStem(word);
	lang=Doc->GetSession()->GetDoc(1)->GetLang();
	GDict* dict=Doc->GetSession()->GetDic(lang);
	//passer ds le stemming ...
	Stem=lang->GetStemming(NoStem);
	//prendre le dictionnaire et lrechercher l id du mot ds le dictionnaire
	int id=dict->GetId(Stem);
	//ajouter dans le container de giwordweights..
	if(weight)	QueryWord->InsertPtr(new GIWordWeight(id,1));
	else QueryWord->InsertPtr(new GIWordWeight(id,-1));
}


//-----------------------------------------------------------------------------
void KViewQuery::slotNewQuery(void)
{
	QueryWord->Clear();
	char* ptr;
	char temp;
	char* ptrtemp;

	// Extract command and parameters
	sprintf(Buffer,Cmd->text().latin1());
	ptr=Buffer;
	while(*ptr)
	{
		if((*ptr)=='+')
		{
			ptr++;
			ptrtemp=ptr;
			while((*ptr)&&((*ptr)!=' ')&&((*ptr)!='-')&&((*ptr)!='+'))
			{
				ptr++;
				temp=(*ptr);
			}
			(*ptr)=0;
			AddWord(ptrtemp,true);
			(*ptr)=temp;
		}
		else if((*ptr)=='-')
		{
			ptr++;
			ptrtemp=ptr;
			while((*ptr)&&((*ptr)!=' ')&&((*ptr)!='-')&&((*ptr)!='+'))
			{
				ptr++;
				temp=(*ptr);
			}
			(*ptr)=0;
			AddWord(ptrtemp,false);
			(*ptr)=temp;
		}
		else ptr++;
	}
	CalcSimGroup();
}


//-----------------------------------------------------------------------------
void KViewQuery::resizeEvent(QResizeEvent *)
{
	Cmd->setGeometry(LabelWidth,0,width()-LabelWidth,Cmd->height());
	Result->setGeometry(0,Cmd->height(),width(),height()-Cmd->height());
}


//-----------------------------------------------------------------------------
KViewQuery::~KViewQuery(void)
{
if (QueryWord) delete(QueryWord);
}
