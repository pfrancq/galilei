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
//
// class GrWord
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class KViewStems::GrWord : public RString
{
public:
	RContainer<Word,unsigned int,false,false> Words;
	double Precision;
	double Recall;

	GrWord(const char* s) : RString(s),Words(30,20) {}
	GrWord(const RString& s) : RString(s),Words(30,20) {}
};



//-----------------------------------------------------------------------------
//
//  class GrWordId
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class KViewStems::GrWordId
{
public:
	GrWord* Grp;
	unsigned int Pos;

	GrWordId(GrWord* grp,unsigned int pos) : Grp(grp), Pos(pos) {}
	int Compare(const GrWord* grp) const {return(Grp->Compare(grp));}
	int Compare(const GrWordId* grp) const {return(Grp->Compare(grp->Grp));}
	int Compare(const GrWordId& grp) const {return(Grp->Compare(grp.Grp));}
};



//-----------------------------------------------------------------------------
//
// class KViewStems
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewStems::KViewStems(const char* code,const char* filename,KDoc* doc, QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Roots(0), Stems(0), Words(0),
	  Precision(0.0), Recall(0.0)
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
		if(wptr->Stem) continue; // Word alreay exists

		// Compute Stem
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

	// Compute Stats
	ComputeRecallPrecision();
	ComputeTotal();

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
		sprintf(word,"Stem: '%s'  -  Precision=%1.3f - Recall=%1.3f",(*gptr)(),gptr->Precision,gptr->Recall);
		QListViewItem* grsitem = new QListViewItem(prGroups,word);
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
unsigned int KViewStems::GetNbWords(GrWord* grp1,GrWord* grp2)
{
	unsigned int tot=0;
	unsigned int i;
	Word** tab;

	for(i=grp1->Words.NbPtr+1,tab=grp1->Words.Tab;--i;tab++)
		if(grp2->Words.IsIn<const Word*>(*tab))
			tot++;
	return(tot);
}


//-----------------------------------------------------------------------------
void KViewStems::ComputeRecallPrecision(void)
{
	GrWord* root;
	GrWord* stem;
	unsigned int NbStem;
	unsigned int InStem;
	unsigned int InRoot;
	double NbWords;
	char tmp[100];
	RContainer<GrWord,unsigned int,true,true>*** ptr;
	RContainer<GrWord,unsigned int,true,true>** ptr2;
	unsigned int i,j;

	Precision=Recall=0.0;
	NbWords=0.0;
	for(i=27+1,ptr=Stems->Hash;--i;ptr++)
	{
		for(j=27+1,ptr2=*ptr;--j;ptr2++)
		{
			for((*ptr2)->Start();!(*ptr2)->End();(*ptr2)->Next())
			{
				stem=(**ptr2)();
				NbStem=stem->Words.NbPtr;
				NbWords+=NbStem;
				stem->Precision=stem->Recall=0.0;
				if(!NbStem) continue;
				if(NbStem==1)
				{
					root=stem->Words.Tab[0]->Root;
					if(!root) continue;
					stem->Precision=1.0;
					if(root->Words.NbPtr==1);
						stem->Recall=1.0;
					Precision+=stem->Precision;
					Recall+=stem->Recall;
				}
				else
				{
					for(stem->Words.Start();!stem->Words.End();stem->Words.Next())
					{
						root=stem->Words()->Root;
						if(!root) continue;
						if(root->Words.NbPtr==1)
						{
							stem->Recall+=1.0;
						}
						else
						{
							InRoot=GetNbWords(root,stem)-1;
							if(InRoot)
								stem->Precision+=((double)(InRoot))/((double)(NbStem-1));
							InStem=GetNbWords(stem,root)-1;
							if(InStem)
								stem->Recall+=((double)(InStem))/((double)(root->Words.NbPtr-1));
						}
					}
					Precision+=stem->Precision;
					Recall+=stem->Recall;
					stem->Precision/=NbStem;
					stem->Recall/=NbStem;
				}
			}
		}
	}
	if(NbWords)
	{
		Precision/=NbWords;
		Recall/=NbWords;
	}
	sprintf(tmp," - Precision=%1.3f - Recall=%1.3f",Precision,Recall);
	setCaption(caption()+tmp);
}


//-----------------------------------------------------------------------------
void KViewStems::ComputeTotal(void)
{
	GrWord* root;
	GrWord* stem;
	unsigned int** matrix;                        // Matrix representing all assignation ideal/computed
	unsigned int NbRows,NbCols;                   // Rows and Cols for matrix
	unsigned int NbWords;                         // Number of Words.
	unsigned int** mat;
	unsigned int* mat2;
	unsigned int row,col;
	RContainer<GrWord,unsigned int,true,true>*** ptr;
	RContainer<GrWord,unsigned int,true,true>** ptr2;
	unsigned int i,j;
	char tmp[100];
	
	// Init part
	NbWords=0;
	Total=0.0;
	NbRows=Roots->GetNb();
	NbCols=Stems->GetNb();
	if((!NbRows)||(!NbCols)) return;
	matrix=new unsigned int*[NbRows];
	for(row=NbRows+1,mat=matrix;--row;mat++)
	{
		(*mat)=new unsigned int[NbCols];
		memset((*mat),0,NbCols*sizeof(int));
	}

	// Construction of the container for relation between root and rows in the matrix.
	RContainer<GrWordId,unsigned int,true,true> RootsId(NbRows,NbRows/2);
	for(i=27+1,row=0,ptr=Roots->Hash;--i;ptr++)
	{
		for(j=27+1,ptr2=*ptr;--j;ptr2++)
		{
			for((*ptr2)->Start();!(*ptr2)->End();(*ptr2)->Next())
			{
				root=(**ptr2)();
				RootsId.InsertPtr(new GrWordId(root,row));
				row++;
			}
		}
	}

	// Construction of the container for relation between stem and column in the matrix.
	RContainer<GrWordId,unsigned int,true,true> StemsId(NbCols,NbCols/2);
	for(i=27+1,col=0,ptr=Stems->Hash;--i;ptr++)
	{
		for(j=27+1,ptr2=*ptr;--j;ptr2++)
		{
			for((*ptr2)->Start();!(*ptr2)->End();(*ptr2)->Next())
			{
				stem=(**ptr2)();
				StemsId.InsertPtr(new GrWordId(stem,col));
				col++;
			}
		}
	}

	// Element i,j of the matrix is the number of words who are in the ith root
	// and jth stem.
	for(i=27+1,col=0,ptr=Roots->Hash;--i;ptr++)
	{
		for(j=27+1,ptr2=*ptr;--j;ptr2++)
		{
			for((*ptr2)->Start();!(*ptr2)->End();(*ptr2)->Next())
			{
				root=(**ptr2)();
				row=RootsId.GetPtr<const GrWord*>(root)->Pos;
				// for each word in this root add 1 in the case corresponding to the
				// id of the computedgroup where the subprofile is.
				for(root->Words.Start();!root->Words.End();root->Words.Next())
				{
					col=StemsId.GetPtr<const GrWord*>(root->Words()->Stem)->Pos;
					matrix[row][col]++;
					NbWords++;
				}
			}
		}
	}

	//Calculation of the different terms of the total = a-(b*c)/d)/((1/2*(b+c))-(b*c)/d)
	double a,b,c,d,num,den;
	a=b=c=0.0;

	for(row=NbRows+1,mat=matrix;--row;mat++)
	{
		int sum=0;
		for(col=NbCols+1,mat2=(*mat);--col;mat2++)
		{
			a+=(((*mat2)*((*mat2)-1))/2);
			sum+=(*mat2);
		}
		b+=((sum*(sum-1))/2);
	}

	for(col=NbCols+1;--col;)
	{
		int sum=0;
		for(row=NbRows+1,mat=matrix;--row;mat++)
		{
			sum+=(*mat)[col-1];
		}
		c+=((sum*(sum-1))/2);
	}

	d=(NbWords*(NbWords-1))/2;
	num=a-((b*c)/d);
	den=(0.5*(b+c))-(b*c/d);
	Total=num/(den*NbWords);

	// Delete the matrix
 	for(row=NbRows+1,mat=matrix;--row;mat++)
	{
		delete[] (*mat);
	}
	delete[] matrix;
	sprintf(tmp," - Total=%1.3f",Total);
	setCaption(caption()+tmp);
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
KViewStems::~KViewStems(void)
{
	if(Roots) delete Roots;
	if(Stems) delete Stems;
	if(Words) delete Words;
}
