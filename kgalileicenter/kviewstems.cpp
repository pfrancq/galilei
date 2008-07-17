/*

	GALILEI Research Project

	KViewStems.cpp

	Basic Window for the application - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

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
#include <rtextfile.h>
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qtabwidget.h>
#include <qlistview.h>
#include <qlabel.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


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
	RContainer<Word,false,false> Words;
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
	size_t Pos;

	GrWordId(GrWord* grp,size_t pos) : Grp(grp), Pos(pos) {}
	int Compare(const GrWord* grp) const {return(Grp->Compare(*grp));}
	int Compare(const GrWordId& grp) const {return(Grp->Compare(*grp.Grp));}
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
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("kmultiple.png",KIcon::Small)));
	char tmp[10];

	GLangManager* langMng=GALILEIApp->GetManager<GLangManager>("Lang");
	Lang=langMng->GetPlugIn(code);

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
	Roots=new RDblHashContainer<GrWord,true>(27,27,200,100);
	Stems=new RDblHashContainer<GrWord,true>(27,27,200,100);
	Words=new RDblHashContainer<Word,true>(27,27,300,150);
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
	RString line;
	const RChar* ptr;
	size_t pos;
	size_t len;
	RString word;
	Word* wptr;
	GrWord* gptr;

	// Read the File
	f.Open(RIO::Read);
	while(!f.End())
	{
		// Read a line
		line=f.GetLine();
		ptr=line();

		// Read Word

		ptr++;
		len=0;
		while((!ptr->IsNull())&&((*ptr)!=RChar('"')))
		{
			ptr++;
			len++;
		}
		word=line.Mid(1,len);
		wptr=Words->GetInsertPtr<RString>(word);
		if(wptr->Stem) continue; // Word alreay exists

		// Compute Stem
		gptr=Stems->GetInsertPtr<const RString>(Lang->GetStemming(word));
		wptr->Stem=gptr;
		gptr->Words.InsertPtr(wptr);

		// Skip '" = "'
		ptr+=5;
		pos=len+6;

		// Read Root
		len=0;
		while((!ptr->IsNull())&&((*ptr)!=RChar('"')))
		{
			ptr++;
			len++;
		}
		gptr=Roots->GetInsertPtr<RString>(line.Mid(pos,len));
		wptr->Root=gptr;
		gptr->Words.InsertPtr(wptr);
	}

	// Set the caption
	setCaption(ToQString(Lang->GetName())+" - Words: "+QString::number(Words->GetNb())+" - Roots: - "+QString::number(Roots->GetNb())+" - Stems: "+QString::number(Stems->GetNb()));

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
	RContainer<GrWord,true,true>* ptr2;
	char word[200];
	Word* wptr;
	GrWord* gptr;

	thGroups->clear();
	ptr2=(*(*Roots)[index])[index2];
	RCursor<GrWord> cGr(*ptr2);
	for(cGr.Start();!cGr.End();cGr.Next())
	{
		gptr=cGr();
		QListViewItem* grsitem = new QListViewItem(thGroups,ToQString(*gptr));
		RCursor<Word> cWord(cGr()->Words);
		for(cWord.Start();!cWord.End();cWord.Next())
		{
			wptr=cWord();
			sprintf(word,"Word: '%s'  -  Stem: '%s'",wptr->Latin1(),wptr->Stem->Latin1());
			new QListViewItem(grsitem,word);
		}
	}
}


//-----------------------------------------------------------------------------
void KViewStems::ConstructPr(char index,char index2)
{
	RContainer<GrWord,true,true>* ptr2;
//	char word[200];
	QString word;
	Word* wptr;
	GrWord* gptr;

	prGroups->clear();
	ptr2=(*(*Stems)[index])[index2];
	RCursor<GrWord> cGr(*ptr2);
	for(cGr.Start();!cGr.End();cGr.Next())
	{
		gptr=cGr();
		word="Stem: '"+ToQString(*gptr)+"'  -  Precision="+QString::number(gptr->Precision)+"  -  Recall="+QString::number(gptr->Recall);
		QListViewItem* grsitem = new QListViewItem(prGroups,word);
		RCursor<Word> cWord(cGr()->Words);
		for(cWord.Start();!cWord.End();cWord.Next())
		{
			wptr=cWord();
			word="Word: '"+ToQString(*wptr)+"'  -  Root: '"+ToQString(*wptr->Root)+"'";
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
size_t KViewStems::GetNbWords(GrWord* grp1,GrWord* grp2)
{
	size_t tot=0;

	RCursor<Word> cWord(grp1->Words);
	for(cWord.Start();!cWord.End();cWord.Next())
		if(grp2->Words.IsIn<Word>(*cWord()))
			tot++;
	return(tot);
}


//-----------------------------------------------------------------------------
void KViewStems::ComputeRecallPrecision(void)
{
// 	GrWord* root;
// 	GrWord* stem;
// 	size_t NbStem;
// 	size_t InStem;
// 	size_t InRoot;
// 	double NbWords;
// 	char tmp[100];
// 	RContainer<GrWord,true,true>*** ptr;
// 	RContainer<GrWord,true,true>** ptr2;
// 	size_t i,j;
//
// 	Precision=Recall=0.0;
// 	NbWords=0.0;
// 	for(i=27+1,ptr=Stems->Hash;--i;ptr++)
// 	{
// 		for(j=27+1,ptr2=*ptr;--j;ptr2++)
// 		{
// 			RCursor<GrWord> cWord(**ptr2);
// 			for(cWord.Start();!cWord.End();cWord.Next())
// 			{
// 				stem=cWord();
// 				NbStem=stem->Words.GetNb();
// 				NbWords+=NbStem;
// 				stem->Precision=stem->Recall=0.0;
// 				if(!NbStem) continue;
// 				if(NbStem==1)
// 				{
// 					root=stem->Words[0]->Root;
// 					if(!root) continue;
// 					stem->Precision=1.0;
// 					if(root->Words.GetNb()==1);
// 						stem->Recall=1.0;
// 					Precision+=stem->Precision;
// 					Recall+=stem->Recall;
// 				}
// 				else
// 				{
// 					RCursor<Word> cWord2(stem->Words);
// 					for(cWord2.Start();!cWord2.End();cWord2.Next())
// 					{
// 						root=cWord2()->Root;
// 						if(!root) continue;
// 						if(root->Words.GetNb()==1)
// 						{
// 							stem->Recall+=1.0;
// 						}
// 						else
// 						{
// 							InRoot=GetNbWords(root,stem)-1;
// 							if(InRoot)
// 								stem->Precision+=((double)(InRoot))/((double)(NbStem-1));
// 							InStem=GetNbWords(stem,root)-1;
// 							if(InStem)
// 								stem->Recall+=((double)(InStem))/((double)(root->Words.GetNb()-1));
// 						}
// 					}
// 					Precision+=stem->Precision;
// 					Recall+=stem->Recall;
// 					stem->Precision/=NbStem;
// 					stem->Recall/=NbStem;
// 				}
// 			}
// 		}
// 	}
// 	if(NbWords)
// 	{
// 		Precision/=NbWords;
// 		Recall/=NbWords;
// 	}
// 	sprintf(tmp," - Precision=%1.3f - Recall=%1.3f",Precision,Recall);
// 	setCaption(caption()+tmp);
}


//-----------------------------------------------------------------------------
void KViewStems::ComputeTotal(void)
{
// 	GrWord* root;
// 	size_t NbRows,NbCols;                   // Rows and Cols for matrix
// 	size_t NbWords;                         // Number of Words.
// 	double* VectorRows;                           // Sum of the rows of the matrix
// 	double* VectorCols;                           // Sum of the columns of the matrix
// 	double* VectorColsTemp;                       // temp sum of the columns of the matrix
// 	double* vec;
// 	size_t row,col;
// 	double a,b,c,d,num,den;
// 	RContainer<GrWord,true,true>*** ptr;
// 	RContainer<GrWord,true,true>** ptr2;
// 	size_t i,j;
// 	char tmp[100];
//
// 	// Init part
// 	NbWords=0;
// 	Total=0.0;
// 	NbRows=Roots->GetNb();
// 	NbCols=Stems->GetNb();
// 	if((!NbRows)||(!NbCols)) return;
// 	VectorRows=new double[NbRows];
// 	VectorCols=new double[NbCols];
// 	VectorColsTemp=new double[NbCols];
// 	memset(VectorRows,0,NbRows*sizeof(double));
// 	memset(VectorCols,0,NbCols*sizeof(double));
// 	a=b=c=d=0.0;
//
// 	// Construction of the container for relation between root and rows in the matrix.
// 	RContainer<GrWordId,true,true> RootsId(NbRows,NbRows/2);
// 	for(i=27+1,row=0,ptr=Roots->Hash;--i;ptr++)
// 		for(j=27+1,ptr2=*ptr;--j;ptr2++)
// 		{
// 			RCursor<GrWord> cGr(**ptr2);
// 			for(cGr.Start();!cGr.End();cGr.Next())
// 				RootsId.InsertPtr(new GrWordId(cGr(),row++));
// 		}
//
// 	// Construction of the container for relation between stem and column in the matrix.
// 	RContainer<GrWordId,true,true> StemsId(NbCols,NbCols/2);
// 	for(i=27+1,col=0,ptr=Stems->Hash;--i;ptr++)
// 		for(j=27+1,ptr2=*ptr;--j;ptr2++)
// 		{
// 			RCursor<GrWord> cGr(**ptr2);
// 			for(cGr.Start();!cGr.End();cGr.Next())
// 				StemsId.InsertPtr(new GrWordId(cGr(),col++));
// 		}
//
// 	// Element i,j of the matrix is the number of words who are in the ith root
// 	// and jth stem.
// 	for(i=27+1,ptr=Roots->Hash;--i;ptr++)
// 	{
// 		for(j=27+1,ptr2=*ptr;--j;ptr2++)
// 		{
// 			RCursor<GrWord> cGr(**ptr2);
// 			for(cGr.Start();!cGr.End();cGr.Next())
// 			{
// 				memset(VectorColsTemp,0,NbCols*sizeof(double));
// 				root=cGr();
// 				row=RootsId.GetPtr<const GrWord*>(root)->Pos;
// 				// for each word in this root add 1 in the case corresponding to the
// 				// id of the computedgroup where the subprofile is.
// 				RCursor<Word> cWord(root->Words);
// 				for(cWord.Start();!cWord.End();cWord.Next())
// 				{
// 					col=StemsId.GetPtr<const GrWord*>(cWord()->Stem)->Pos;
// 					VectorRows[row]++;
// 					VectorCols[col]++;
// 					VectorColsTemp[col]++;
// 					NbWords++;
// 				}
// 				for(col=NbCols+1,vec=VectorColsTemp;--col;vec++)
// 					a+=(((*vec)*((*vec)-1))/2);
// 			}
// 		}
// 	}
//
// 	// Calculation of the different terms of the total = a-(b*c)/d)/((1/2*(b+c))-(b*c)/d)
// 	for(col=NbCols+1,vec=VectorCols;--col;vec++)
// 		b+=(((*vec)*((*vec)-1))/2);
// 	for(row=NbRows+1,vec=VectorRows;--row;vec++)
// 		c+=(((*vec)*((*vec)-1))/2);
// 	d=(NbWords*(NbWords-1))/2;
// 	num=a-((b*c)/d);
// 	den=(0.5*(b+c))-(b*c/d);
// 	if(den)
// 		Total=num/den;
// 	else
// 		Total=1.0;
//
// 	//delete the vectors
// 	if (VectorRows) delete[] VectorRows;
// 	if (VectorCols) delete[] VectorCols;
// 	if (VectorColsTemp) delete[] VectorColsTemp;
//
// 	sprintf(tmp," - Total=%1.3f",Total);
// 	setCaption(caption()+tmp);
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
