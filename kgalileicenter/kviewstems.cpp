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

	GrWord(const char* s) : RString(s),Words(100,25) {}
	GrWord(const RString& s) : RString(s),Words(100,25) {}
};


//-----------------------------------------------------------------------------
//
// class KViewStems
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewStems::KViewStems(const char* code,const char* filename,KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/mimetypes/kmultiple.png"));

	Lang=doc->GetSession()->GetLang(code);

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Theoritic groupement
	thGroups = new QListView(this);
	Infos->insertTab(thGroups,"Theoritic Groups of words - Roots");
	thGroups->resize(size());
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

	// Loads and Computes
	LoadFile(filename);
	Compute();
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
	RDblHashContainer<GrWord,unsigned,27,27,true> Roots(2000,1500);
	RDblHashContainer<GrWord,unsigned,27,27,true> Stems(2000,1500);
	RDblHashContainer<Word,unsigned,27,27,true> Words(2000,1500);
	RContainer<GrWord,unsigned,true,true>*** ptr1;
	RContainer<GrWord,unsigned,true,true>** ptr2;
	unsigned i,j;
	RContainer<RString,unsigned,true,true> Genres(20,10);

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
		wptr=Words.GetInsertPtr<const char*>(word);
		gptr=Stems.GetInsertPtr<const RString>(Lang->GetStemming(word));
		wptr->Stem=gptr;
		gptr->Words.InsertPtr(wptr);

		// Skip '" = "'
		line+=5;

		// Read Root
		ptr=root;
		while((*line)&&((*line)!='"'))
			(*(ptr++))=(*(line++));
		(*ptr)=0;
		gptr=Roots.GetInsertPtr<const char*>(root);
		wptr->Root=gptr;
		gptr->Words.InsertPtr(wptr);
		
		// Skip '" '
		line+=2;

		// Read Genres
		ptr=root;
		while((*line)&&((*line)!='['))
			(*(ptr++))=(*(line++));
		(*ptr)=0;
//		Genres.GetInsertPtr<const char*>(root);
	}

	// Set the caption
	sprintf(word,"%s - Words: %u - Roots: %u - Stems: %u",Lang->GetName(),Words.GetNb(),Roots.GetNb(),Stems.GetNb());
	setCaption(word);

	// Fill the list 'Roots'
	for(i=27+1,ptr1=Roots.Hash;--i;ptr1++)
	{
		for(j=27+1,ptr2=*ptr1;--j;ptr2++)
		{
			for((*ptr2)->Start();!(*ptr2)->End();(*ptr2)->Next())
			{
				gptr=(*(*ptr2))();
				QListViewItem* grsitem = new QListViewItem(thGroups,(*gptr)());
				for(gptr->Words.Start();!gptr->Words.End();gptr->Words.Next())
				{
					wptr=gptr->Words();
					sprintf(word,"Word: '%s'  -  Stem: '%s'",(*wptr)(),(*wptr->Stem)());
					new QListViewItem(grsitem,word);
				}
			}
		}
	}

	// Fill the list 'Stems'
	for(i=27+1,ptr1=Stems.Hash;--i;ptr1++)
	{
		for(j=27+1,ptr2=*ptr1;--j;ptr2++)
		{
			for((*ptr2)->Start();!(*ptr2)->End();(*ptr2)->Next())
			{
				gptr=(*(*ptr2))();
				QListViewItem* grsitem = new QListViewItem(prGroups,(*gptr)());
				for(gptr->Words.Start();!gptr->Words.End();gptr->Words.Next())
				{
					wptr=gptr->Words();
					sprintf(word,"Word: '%s'  -  Root: '%s'",(*wptr)(),(*wptr->Root)());
					new QListViewItem(grsitem,word);
				}
			}
		}
	}
}




//-----------------------------------------------------------------------------
void KViewStems::Compute(void)
{
}


//-----------------------------------------------------------------------------
void KViewStems::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
KViewStems::~KViewStems(void)
{
}
