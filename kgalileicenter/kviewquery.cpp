/*

	GALILEI Research Project

	KViewQuery.cpp

	Window for the running some instruction to the session - Implementation.

	(C) 2002 by Julien Lamoral

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
#include <langs/glangen.h>
#include <sessions/gsession.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include file for R Project
#include <rstd/rstring.h>
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewquery.h"
#include "kdoc.h"


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlineedit.h>
#include <qlabel.h>
#include <qmultilineedit.h>



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
	setCaption("Run Query");
	QLabel* l=new QLabel(this,"Label");
	l->setText("Query : (ex : +jazz -opera)");
	LabelWidth=l->width()+5;
	l->setGeometry(5,0,l->width(),l->height());
	Cmd=new QLineEdit(this,"Query2");
	Result=new QMultiLineEdit(this,"Query2");
	Result->setReadOnly(true);
	Result->insertLine("dlkjljdlsqjl");
	// Connections
	connect(Cmd,SIGNAL(returnPressed()),this,SLOT(slotNewQuery()));

	// lancer le calcul de similarité avec les groupes...
	// afficher le résultat...
	delete(QueryWord);
}


//-----------------------------------------------------------------------------
void KViewQuery::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
void KViewQuery::AddWord(char* word,bool weight)
{
	GLang* lang=Doc->GetSession()->GetLang("en");
	GDict* dict=Doc->GetSession()->GetDic(lang);
	GLangEN Stem (GDict* dict);
	if(weight) cout<<word<<" est passé par addword"<<endl;
	//passer ds le stemming ...
	
	//prendre le dictionnaire et lrechercher l id du mot ds le dictionnaire

	//ajouter dans le container de giwordweights..
}


//-----------------------------------------------------------------------------
void KViewQuery::slotNewQuery(void)
{
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
}
