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
#include <rstd/rtextfile.h>
#include <frontend/kde/rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <sessions/gsession.h>
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
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("kmultiple.png",KIcon::Small)));
	char tmp[10];

	GLangManager* langMng=dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang"));
	Lang=langMng->GetLang(code);

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
	Roots=new RDblHashContainer<GrWord,27,27,true>(200,100);
	Stems=new RDblHashContainer<GrWord,27,27,true>(200,100);
	Words=new RDblHashContainer<Word,27,27,true>(300,150);
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
	unsigned int pos;
	unsigned int len;
	RString word;
	Word* wptr;
	GrWord* gptr;

	// Read the File
	while(!f.Eof())
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
	ptr2=Roots->Hash[index][index2];
	for(ptr2->Start();!ptr2->End();ptr2->Next())
	{
		gptr=(*ptr2)();
		QListViewItem* grsitem = new QListViewItem(thGroups,ToQString(*gptr));
		for(gptr->Words.Start();!gptr->Words.End();gptr->Words.Next())
		{
			wptr=gptr->Words();
			sprintf(word,"Word: '%s'  -  Stem: '%s'",wptr,wptr->Stem);
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
	ptr2=Stems->Hash[index][index2];
	for(ptr2->Start();!ptr2->End();ptr2->Next())
	{
		gptr=(*ptr2)();
		word="Stem: '"+ToQString(*gptr)+"'  -  Precision="+QString::number(gptr->Precision)+"  -  Recall="+QString::number(gptr->Recall);
		QListViewItem* grsitem = new QListViewItem(prGroups,word);
		for(gptr->Words.Start();!gptr->Words.End();gptr->Words.Next())
		{
			wptr=gptr->Words();
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
	RContainer<GrWord,true,true>*** ptr;
	RContainer<GrWord,true,true>** ptr2;
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
	unsigned int NbRows,NbCols;                   // Rows and Cols for matrix
	unsigned int NbWords;                         // Number of Words.
	double* VectorRows;                           // Sum of the rows of the matrix
	double* VectorCols;                           // Sum of the columns of the matrix
	double* VectorColsTemp;                       // temp sum of the columns of the matrix
	double* vec;
	unsigned int row,col;
	double a,b,c,d,num,den;
	RContainer<GrWord,true,true>*** ptr;
	RContainer<GrWord,true,true>** ptr2;
	unsigned int i,j;
	char tmp[100];
	
	// Init part
	NbWords=0;
	Total=0.0;
	NbRows=Roots->GetNb();
	NbCols=Stems->GetNb();
	if((!NbRows)||(!NbCols)) return;
	VectorRows=new double[NbRows];
	VectorCols=new double[NbCols];
	VectorColsTemp=new double[NbCols];
	memset(VectorRows,0,NbRows*sizeof(double));
	memset(VectorCols,0,NbCols*sizeof(double));
	a=b=c=d=0.0;

	// Construction of the container for relation between root and rows in the matrix.
	RContainer<GrWordId,true,true> RootsId(NbRows,NbRows/2);
	for(i=27+1,row=0,ptr=Roots->Hash;--i;ptr++)
		for(j=27+1,ptr2=*ptr;--j;ptr2++)
			for((*ptr2)->Start();!(*ptr2)->End();(*ptr2)->Next())
				RootsId.InsertPtr(new GrWordId((**ptr2)(),row++));

	// Construction of the container for relation between stem and column in the matrix.
	RContainer<GrWordId,true,true> StemsId(NbCols,NbCols/2);
	for(i=27+1,col=0,ptr=Stems->Hash;--i;ptr++)
		for(j=27+1,ptr2=*ptr;--j;ptr2++)
			for((*ptr2)->Start();!(*ptr2)->End();(*ptr2)->Next())
				StemsId.InsertPtr(new GrWordId((**ptr2)(),col++));

	// Element i,j of the matrix is the number of words who are in the ith root
	// and jth stem.
	for(i=27+1,ptr=Roots->Hash;--i;ptr++)
	{
		for(j=27+1,ptr2=*ptr;--j;ptr2++)
		{
			for((*ptr2)->Start();!(*ptr2)->End();(*ptr2)->Next())
			{
				memset(VectorColsTemp,0,NbCols*sizeof(double));
				root=(**ptr2)();
				row=RootsId.GetPtr<const GrWord*>(root)->Pos;
				// for each word in this root add 1 in the case corresponding to the
				// id of the computedgroup where the subprofile is.
				for(root->Words.Start();!root->Words.End();root->Words.Next())
				{
					col=StemsId.GetPtr<const GrWord*>(root->Words()->Stem)->Pos;
					VectorRows[row]++;
					VectorCols[col]++;
					VectorColsTemp[col]++;
					NbWords++;
				}
				for(col=NbCols+1,vec=VectorColsTemp;--col;vec++)
					a+=(((*vec)*((*vec)-1))/2);
			}
		}
	}

	// Calculation of the different terms of the total = a-(b*c)/d)/((1/2*(b+c))-(b*c)/d)
	for(col=NbCols+1,vec=VectorCols;--col;vec++)
		b+=(((*vec)*((*vec)-1))/2);
	for(row=NbRows+1,vec=VectorRows;--row;vec++)
		c+=(((*vec)*((*vec)-1))/2);
	d=(NbWords*(NbWords-1))/2;
	cout<<"NbWords="<<NbWords<<endl<<"a="<<a<<endl<<"b="<<b<<endl<<"c="<<c<<endl<<"d="<<d<<endl;
	num=a-((b*c)/d);
	den=(0.5*(b+c))-(b*c/d);
	if(den)
		Total=num/den;
	else
		Total=1.0;

	//delete the vectors
	if (VectorRows) delete[] VectorRows;
	if (VectorCols) delete[] VectorCols;
	if (VectorColsTemp) delete[] VectorColsTemp;

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
