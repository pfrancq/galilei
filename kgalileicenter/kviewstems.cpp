/*

	GALILEI Research Project

	KViewStems.cpp

	Basic Window for the application - Implementation.

	(C) 2001 by Pascal Francq

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
// include files for R Project
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <sessions/gsession.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qtabwidget.h>
#include <qlistview.h>
#include <qlabel.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kdoc.h"
#include "kviewstems.h"


//-----------------------------------------------------------------------------
//
// class Word
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class KViewStems::Word : public RString
{
public:
	GrWord* Root;
	GrWord* Stem;

	Word(const char* s) : RString(s),Root(0),Stem(0) {}
	Word(const RString& s) : RString(s),Root(0),Stem(0) {}
};


//-----------------------------------------------------------------------------
class KViewStems::GrWord : public RString
{
public:
	RContainer<Word,unsigned int,false,false> Words;

	GrWord(const char* s) : RString(s),Words(30,20) {}
	GrWord(const RString& s) : RString(s),Words(30,20) {}
};


//-----------------------------------------------------------------------------
//
// class KViewStems
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewStems::KViewStems(const char* code,const char* filename,KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Roots(0), Stems(0), Words(0)
{
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/mimetypes/kmultiple.png"));
	char tmp[10];

	Lang=doc->GetSession()->GetLang(code);

	// initialisation of the tab widget
	Indexes=new QLabel(this,"Indexes");
	Indexes->setText("Words beginning with:");
	Indexes->setGeometry(0,0,width()-120,30);
	Index=new QComboBox(false,this,"First Index");
	Index->setGeometry(width()-120,0,60,30);
	Index2=new QComboBox(false,this,"First Index");
	Index2->setGeometry(width()-60,0,60,30);
	Infos=new QTabWidget(this);
	Infos->setGeometry(0,30,width(),height()-30);
	for(char c='a';c<='z';c++)
	{
		sprintf(tmp,"%c",c);
		Index->insertItem(tmp,c-'a');
		Index2->insertItem(tmp,c-'a');
	}
	Index->insertItem("Other",26);
	Index2->insertItem("Other",26);

	// Theoritic groupement
	thGroups = new QListView(this);
	Infos->insertTab(thGroups,"Theoritic Groups of words - Roots");
	thGroups->resize(Infos->size());
	thGroups->addColumn(QString("Words/Stems"));
	thGroups->setRootIsDecorated(true);
	thGroups->setSorting(-1);

	// Theoritic groupement
	prGroups = new QListView(this);
	Infos->insertTab(prGroups,"Computed Groups of words - Stems");
	prGroups->resize(size());
	prGroups->addColumn(QString("Words/Roots"));
	prGroups->addColumn(QString("Stats"));
	prGroups->setRootIsDecorated(true);
	prGroups->setSorting(-1);

	// connect
	connect(Index,SIGNAL(activated(int)),this,SLOT(slotIndex(int)));
	connect(Index2,SIGNAL(activated(int)),this,SLOT(slotIndex2(int)));

	// Loads and Computes
	Roots=new RDblHashContainer<GrWord,unsigned,27,27,true>(200,100);
	Stems=new RDblHashContainer<GrWord,unsigned,27,27,true>(200,100);
	Words=new RDblHashContainer<Word,unsigned,27,27,true>(300,150);
	LoadFile(filename);
}


//-----------------------------------------------------------------------------
void KViewStems::update(unsigned int /*cmd*/)
{
}


//-----------------------------------------------------------------------------
void KViewStems::LoadFile(const char* filename)
{
	RTextFile f(filename);
	char* line;
	char* ptr;
	char word[200];
	char root[200];
	Word* wptr;
	GrWord* gptr;
//	RContainer<RString,unsigned,true,true> Genres(20,10);

	// Read the File
	while(!f.Eof())
	{
		line=f.GetLine();
		line++;  // Skip ".

		// Read Word
		ptr=word;
		while((*line)&&((*line)!='"'))
			(*(ptr++))=(*(line++));
		(*ptr)=0;
		wptr=Words->GetInsertPtr<const char*>(word);
		gptr=Stems->GetInsertPtr<const RString>(Lang->GetStemming(word));
		wptr->Stem=gptr;
		gptr->Words.InsertPtr(wptr);

		// Skip '" = "'
		line+=5;

		// Read Root
		ptr=root;
		while((*line)&&((*line)!='"'))
			(*(ptr++))=(*(line++));
		(*ptr)=0;
		gptr=Roots->GetInsertPtr<const char*>(root);
		wptr->Root=gptr;
		gptr->Words.InsertPtr(wptr);
		
		// Skip '" '
		line+=2;

		// Read Genres
//		ptr=root;
//		while((*line)&&((*line)!='['))
//			(*(ptr++))=(*(line++));
//		(*ptr)=0;
//		Genres.GetInsertPtr<const char*>(root);
	}

	// Set the caption
	sprintf(word,"%s - Words: %u - Roots: %u - Stems: %u",Lang->GetName(),Words->GetNb(),Roots->GetNb(),Stems->GetNb());
	setCaption(word);

	// Fill the list 'Roots'
	ConstructTh(0,0);
	ConstructPr(0,0);
}


//-----------------------------------------------------------------------------
void KViewStems::ConstructTh(char index,char index2)
{
	RContainer<GrWord,unsigned,true,true>* ptr2;
	char word[200];
	Word* wptr;
	GrWord* gptr;

	thGroups->clear();
	ptr2=Roots->Hash[index][index2];
	for(ptr2->Start();!ptr2->End();ptr2->Next())
	{
		gptr=(*ptr2)();
		QListViewItem* grsitem = new QListViewItem(thGroups,(*gptr)());
		for(gptr->Words.Start();!gptr->Words.End();gptr->Words.Next())
		{
			wptr=gptr->Words();
			sprintf(word,"Word: '%s'  -  Stem: '%s'",(*wptr)(),(*wptr->Stem)());
			new QListViewItem(grsitem,word);
		}
	}
}


//-----------------------------------------------------------------------------
void KViewStems::ConstructPr(char index,char index2)
{
	RContainer<GrWord,unsigned,true,true>* ptr2;
	char word[200];
	Word* wptr;
	GrWord* gptr;

	prGroups->clear();
	ptr2=Stems->Hash[index][index2];
	for(ptr2->Start();!ptr2->End();ptr2->Next())
	{
		gptr=(*ptr2)();
		QListViewItem* grsitem = new QListViewItem(prGroups,(*gptr)());
		for(gptr->Words.Start();!gptr->Words.End();gptr->Words.Next())
		{
			wptr=gptr->Words();
			sprintf(word,"Word: '%s'  -  Root: '%s'",(*wptr)(),(*wptr->Root)());
			new QListViewItem(grsitem,word);
		}
	}
}


//-----------------------------------------------------------------------------
void KViewStems::resizeEvent(QResizeEvent *)
{
	Indexes->setGeometry(0,0,width()-120,30);
	Index->setGeometry(width()-120,0,60,30);
	Index2->setGeometry(width()-60,0,60,30);
	Infos->setGeometry(0,30,width(),height()-30);
}


//-----------------------------------------------------------------------------
void KViewStems::slotIndex(int index)
{
	ConstructTh(index,Index2->currentItem());
	ConstructPr(index,Index2->currentItem());
}


//-----------------------------------------------------------------------------
void KViewStems::slotIndex2(int index)
{
	ConstructTh(Index->currentItem(),index);
	ConstructPr(Index->currentItem(),index);
}


//-----------------------------------------------------------------------------
bool KViewStems::close(void)
{
	delete this;
	return(true);
}


//-----------------------------------------------------------------------------
KViewStems::~KViewStems(void)
{
	if(Roots) delete Roots;
	if(Stems) delete Stems;
	if(Words) delete Words;
}
