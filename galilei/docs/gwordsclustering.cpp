/*

	GALILEI Research Project

  gwordsclustering.cpp

  Create concepts - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Nicolas Kumps (nkumps@ulb.ac.be).

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>
#include <math.h>


//---------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gwordsclustering.h>
#include <docs/gdocvector.h>
#include <groups/gsubject.h>
#include <infos/giwordlist.h>
#include <infos/giwordweight.h>
#include <langs/gdict.h>
#include <langs/glang.h>
#include <langs/gwordlist.h>
using namespace GALILEI;
using namespace R;
//-----------------------------------------------------------------------------
//
// class GWordDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GWordsClustering::GWordDocs::GWordDocs()
	: GIWordsWeights(200)
{
	Docs=new RContainer<GDoc,unsigned,false,true>(10);
}


//-----------------------------------------------------------------------------
GALILEI::GWordsClustering::GWordDocs::GWordDocs(unsigned id)
	: GIWordsWeights(200),Id(id)
{
	Docs=new RContainer<GDoc,unsigned,false,true>(10);
}


//-----------------------------------------------------------------------------
int GALILEI::GWordsClustering::GWordDocs::Compare(const GWordDocs* n) const
{
	return(Id-(n->GetId()));
}


//-----------------------------------------------------------------------------
int GALILEI::GWordsClustering::GWordDocs::Compare(const GWordDocs& n) const
{
	return(Id-(n.GetId()));
}


//-----------------------------------------------------------------------------
int GALILEI::GWordsClustering::GWordDocs::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
unsigned GALILEI::GWordsClustering::GWordDocs::GetId() const
{
	return(Id);
}


//-----------------------------------------------------------------------------
double GALILEI::GWordsClustering::GWordDocs::GetIdf() const
{
	return(Idf);
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::GWordDocs::SetIdf(double idf) 
{
	Idf=idf;
}


//-----------------------------------------------------------------------------
int GALILEI::GWordsClustering::GWordDocs::sortOrder(const void *a,const void *b)
{
	double af=(*((GWordDocs**)(a)))->GetIdf();
	double bf=(*((GWordDocs**)(b)))->GetIdf();

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::GWordDocs::AddDoc(GDoc* doc)
{
	Docs->InsertPtr(doc);
}


//-----------------------------------------------------------------------------
GALILEI::GWordsClustering::GWordDocs::~GWordDocs()
{
	delete(Docs);
}


//-----------------------------------------------------------------------------
//
// class GWordsClustering
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GWordsClustering::GWordsClustering(GDict* dic,unsigned mindocs,unsigned maxdocs,unsigned minocc,unsigned s,double c,unsigned ndm,RString name)
  : NbMinDocs(mindocs), NbMaxDocs(maxdocs), NbMinOcc(minocc), WindowSize(s),
    MinConf(c), NbDocsMin(ndm), Dic(dic), DbName(name)
{
	Words=new RContainer<GWordDocs,unsigned,true,true>(1000);
	Docs=new RContainer<GDoc,unsigned,false,false>(100,50);
	Groups=new RContainer<GIWordList,unsigned,true,true>(1000);
	Order =new GWordDocs*[5000];
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::AddDoc(GDocVector* doc)
{
	GWordDocs* worddocs;
	Docs->InsertPtr(doc);
	for (doc->Start();!doc->End();doc->Next())
	{
		if(!Dic->GroupsList.IsIn((*doc)()->GetId()))
		{
			if(Words->IsIn((*doc)()->GetId()))
			{
//				if((*doc)()->GetWeight()>=NbMinOcc)
				{
//					if(Words->GetPtr((*doc)()->GetId())->Docs->NbPtr<NbMaxDocs)
//	        {
						Words->GetPtr((*doc)()->GetId())->AddDoc(doc);
//    	    }
				}
			}
			else
				if((*doc)()->GetWeight()>=NbMinOcc)
				{
					worddocs=new GWordDocs((*doc)()->GetId());
					worddocs->SetIdf(log(double((doc->GetLang()->GetRef(otDoc,tWord))/doc->GetLang()->GetRef((*doc)()->GetId(),otDoc))));
					worddocs->AddDoc(doc);
					Words->InsertPtr(worddocs);
				}
		}
	}
}


//-----------------------------------------------------------------------------
RString GALILEI::GWordsClustering::GetFileName(GDoc *doc)
{
	RString name;
	name="/var/galilei/bin/";
	name+=DbName;
	name+="/Doc";
	name+=itoa(doc->GetId());
	return(name);
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::InsertNeighbor()
{
	unsigned int a[50];
	unsigned int i,k;
	for(Docs->Start();!Docs->End();Docs->Next())
	{
		k=int(WindowSize/2);
		if(k>=50) k=49;
		RRecFile<GWord,sizeof(unsigned int),false> f(GetFileName((*Docs)()),Read);
		cout<<GetFileName((*Docs)())<<endl;
		unsigned int tot=(*Docs)()->GetV()-k;
		if(k>=(*Docs)()->GetV()) k=(*Docs)()->GetV();
		for(i=0;i<k;i++)
			f>>a[i];
		if(k>=(*Docs)()->GetV())
			for(i=0;i<k;i++)
			{
				if(k<2)k=2;
				if(Words->IsIn(a[i]))
					for(unsigned int j=i+1;j<=k-2;j++)
					{
						if((Words->IsIn(a[j]))&&(a[i]!=a[j]))
						{
							if((Words->GetPtr(a[i])->Docs->IsIn((*Docs)()->GetId()))&&(Words->GetPtr(a[j])->Docs->IsIn((*Docs)()->GetId())))
							{
								Words->GetPtr(a[i])->GetInsertPtr(a[j])->AddWeight(1.0);
								Words->GetPtr(a[j])->GetInsertPtr(a[i])->AddWeight(1.0);
							}
						}
					}
			}
		else
		{
			do
			{
				if(Words->IsIn(a[0]))
				{
					for(i=1;i<k;i++)
					{
						if((Words->IsIn(a[i]))&&(a[0]!=a[i]))
						{
							if((Words->GetPtr(a[0])->Docs->IsIn((*Docs)()->GetId()))&&(Words->GetPtr(a[i])->Docs->IsIn((*Docs)()->GetId())))
							{
								Words->GetPtr(a[0])->GetInsertPtr(a[i])->AddWeight(1.0);
								Words->GetPtr(a[i])->GetInsertPtr(a[0])->AddWeight(1.0);
							}
						}
					}
				}
				for(i=0;i<k-1;i++)
					a[i]=a[i+1];
				if(tot)
				{
					f>>a[k-1];
					tot--;
				}
			} while(tot);
			if(k<2) k=2;
			for(i=0;i<k-2;i++)
				if(Words->IsIn(a[i]))
					for(unsigned int j=i+1;j<=k-2;j++)
					{
						if((Words->IsIn(a[j]))&&(a[i]!=a[j]))
						{
							if((Words->GetPtr(a[i])->Docs->IsIn((*Docs)()->GetId()))&&(Words->GetPtr(a[j])->Docs->IsIn((*Docs)()->GetId())))
							{
								Words->GetPtr(a[i])->GetInsertPtr(a[j])->AddWeight(1.0);
								Words->GetPtr(a[j])->GetInsertPtr(a[i])->AddWeight(1.0);
							}
						}
					}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::OrderByNeighbor()
{
	bool change;
	unsigned int lastmax,max,i,l,id,subid,memsubid,nbword;
	double norm1,norm2,meanconf,conf,minconf,newprodscal,confidence1,confidence2,minid,lim;
	GIWordList *toins;
	GWordDocs *tocompute,*tocompute2;
	GIWordWeight **ptr;
	RContainer<GDoc,unsigned,false,true> *docslist,*mem;
	GLang *lang=Dic->GetLang();
	meanconf=0.5;
	Sort();
	unsigned int tmp[100];
	GSubjectCursor sub;
	GDocVector *doc;
	lastmax=id=subid=memsubid=nbword=0;
	conf=0.0;
	for(i=0;i<100;i++)
		tmp[i]=0;

	//variable de test.
	double A,AB,Aconf,Amean,Apds,AE,AP,B,BA,Bconf,Bmean,Bpds,BE,BP,ApBA,ApBB,ApBconf,ApBmean,ApBpds,ApBE,ApBP;
	unsigned bestdoc,Adoc,Bdoc,ApBdoc;
	unsigned nbinsub[60];
	RTextFile f("/home/nkumps/StatClustering"+itoa(WindowSize)+".txt",Create);
	RTextFile g("/home/nkumps/Graphe"+itoa(WindowSize)+".txt",Create);
	f<<"C		A		B		nbdoc		conf		meanconf		prodscal		E	"<<endl;
	g<<"nbdoc	confidence	meanconfidence	prodscal	moyenne harmonique"<<endl;
	RTextFile best("/home/nkumps/Best"+itoa(WindowSize)+".txt",Create);

	//nbr de docs par categorie pour ziff.
	if(DbName=="ziff")
	{
		nbinsub[2]=816;
		nbinsub[4]=63;
		nbinsub[6]=256;
		nbinsub[8]=2878;
		nbinsub[10]=194;
		nbinsub[12]=61;
		nbinsub[14]=4220;
		nbinsub[16]=532;
		nbinsub[18]=209;
		nbinsub[20]=210;
		nbinsub[22]=1718;
		nbinsub[24]=3136;
		nbinsub[26]=476;
		nbinsub[28]=333;
		nbinsub[30]=52;
		nbinsub[32]=3144;
		nbinsub[34]=6449;
		nbinsub[36]=453;
		nbinsub[38]=127;
		nbinsub[40]=586;
		nbinsub[42]=2026;
		nbinsub[44]=1433;
		nbinsub[46]=96;
		nbinsub[48]=673;
		nbinsub[50]=827;
		nbinsub[52]=648;
	}

	//nbr de docs par topics pour lesoir.
	if(DbName=="lesoir")
	{
		nbinsub[2]=1929;
		nbinsub[4]=793;
		nbinsub[6]=1941;
		nbinsub[8]=1965;
		nbinsub[10]=1949;
		nbinsub[12]=1471;
		nbinsub[14]=687;
		nbinsub[16]=511;
		nbinsub[18]=1988;
	}

	//nbr de docs par topics pour news1500.
	if(DbName=="news1500")
	{
		nbinsub[3]=91;
		nbinsub[4]=121;
		nbinsub[6]=100;
		nbinsub[8]=126;
		nbinsub[9]=126;
		nbinsub[11]=59;
		nbinsub[12]=55;
		nbinsub[14]=155;
		nbinsub[15]=134;
		nbinsub[17]=62;
		nbinsub[19]=129;
		nbinsub[21]=203;
		nbinsub[22]=119;
	}

	double tA,tB,tC,tconf,tmeanconf,tprodscal,tnbdoc,tE;
	double btA,btB,btC,btconf,btmeanconf,btprodscal,btnbdoc,btE,tmeannb;
	double nC,bA,bB,bC,bconf,bmeanconf,bprodscal,bnbdoc,bE;
	double monC,mA,mB,mC,mconf,mmeanconf,mprodscal,mnbdoc,mE,meannb;
	tA=tB=tC=tconf=tmeanconf=tprodscal=tnbdoc=tE=0.0;
	btA=btB=btC=btconf=btmeanconf=btprodscal=btnbdoc=btE=tmeannb=0.0;

	//variable necessaire aux differents graphes.
	double fctconf[101][4],fctmean[101][4],fctpds[101][4],fctA[101][4],fctB[101][4];

	//initialisation
	for(i=0;i<101;i++)
		for(l=0;l<4;l++)
		{
			fctconf[i][l]=0.0;
			fctmean[i][l]=0.0;
			fctpds[i][l]=0.0;
			fctA[i][l]=0.0;
			fctB[i][l]=0.0;
		}

	/*Creation du vecteur des groupements encore disponibles*/
	for(i=0;i<Words->NbPtr;i++)
	{
		toins=new GIWordList();
		toins->SetId(Order[i]->GetId());
		toins->InsertPtr(new GIWord(Order[i]->GetId()));
		Groups->InsertPtr(toins);
	}
	for(i=0;i<Words->NbPtr;i++)
	{
		if(Groups->GetPtr(Order[i]->GetId()))
		{
			tocompute=Words->GetPtr(Order[i]->GetId());
			double prodscal=0.4;
			minid=0.8;
			meanconf=0.3;
			norm1=0.0;
			lim=0.3;
			bestdoc=Adoc=Bdoc=ApBdoc=0;
			A=AB=Aconf=Amean=Apds=AE=AP=B=BA=Bconf=Bmean=Bpds=BE=BP=ApBA=ApBB=ApBconf=ApBmean=ApBpds=ApBE=ApBP=0.0;
			nC=bA=bB=bC=bconf=bmeanconf=bprodscal=bnbdoc=bE=0.0;
			monC=mA=mB=mC=mconf=mmeanconf=mprodscal=mnbdoc=meannb=mE=0.0;

			//Compute norm1
			for (tocompute->Start();!tocompute->End();tocompute->Next())
				norm1+=((*tocompute)()->GetWeight()*(*tocompute)()->GetWeight());

			change=false;
			for(ptr=tocompute->Tab,l=tocompute->NbPtr;(--l+1);ptr++)
			{

				if(Groups->GetPtr((*ptr)->GetId()))
				{
					tocompute2=Words->GetPtr((*ptr)->GetId());

					//Compute prodscal
					newprodscal=0.0;
					norm2=0.0;
					for(tocompute2->Start();!tocompute2->End();tocompute2->Next())
					{

						//Compute norm2
						norm2+=((*tocompute2)()->GetWeight()*(*tocompute2)()->GetWeight());

//						if((*tocompute2)()->GetId()!=tocompute->GetId())
						if(tocompute->IsIn((*tocompute2)()->GetId()))
							newprodscal+=(*tocompute2)()->GetWeight()*tocompute->GetPtr((*tocompute2)()->GetId())->GetWeight();
					}
					newprodscal=newprodscal/(sqrt(norm1)*sqrt(norm2));

//					nb=0;
//					k=0;
					RContainer<GDoc,unsigned,false,true> *cur1,*cur2;
					docslist=new RContainer<GDoc,unsigned,false,true>(10);
					cur1=tocompute->Docs;
					cur2=tocompute2->Docs;
					cur2->Start();

					//Compute nbdocs.
					for(cur1->Start();!cur1->End();cur1->Next())
					{
						while((!cur2->End())&&((*cur2)()->GetId()<(*cur1)()->GetId()))
							cur2->Next();
						if((!cur2->End())&&((*cur2)()->GetId()==(*cur1)()->GetId()))
						{
							docslist->InsertPtr((*cur1)());
						}
					}

					//Mesure de pertinence du groupement.
					for(unsigned k=0;k<100;k++)
						tmp[k]=0;
					for (docslist->Start();!docslist->End();docslist->Next())
					{
						doc= dynamic_cast<GDocVector*>((*docslist)());
						sub=doc->GetSubjectCursor();
						sub.Start();
						tmp[(sub)()->GetId()]++;
					}
					max=0;
					for(unsigned k=0;k<100;k++)
					{
						if(tmp[k]>0)
							nC+=double(tmp[k])/double(nbinsub[k]);
						if(tmp[k]>max)
						{
							max=tmp[k];
							subid=k;
						}
					}

					//Compute meanconfidence and minconfidance.
					confidence1=0.0;
					confidence2=0.0;
					double newmeanconf,newminconf;
					confidence1=double(docslist->NbPtr)/double(tocompute->Docs->NbPtr);
					confidence2=double(docslist->NbPtr)/double(tocompute2->Docs->NbPtr);//lang->GetRef(tocompute2->GetId(),otDoc)
					if(confidence1>confidence2) newminconf=confidence2;
					else newminconf=confidence1;
					newmeanconf=(confidence1+confidence2)/2;


					if((docslist->NbPtr>=5)&&(confidence1>0.2)&&(newmeanconf>0.2)&&(newprodscal>0.1)) // g<<docslist->NbPtr<<"	"<<confidence1<<"	"<<newmeanconf<<"	"<<newprodscal<<"	"<<(2.0/((1.0/(double(max)/double(nbinsub[subid])))+(1.0/(double(max)/double(docslist->NbPtr)))))<<"	"<<(double(max)/double(docslist->NbPtr))<<"	"<<(double(max)/double(nbinsub[subid]))<<endl;
					g<<docslist->NbPtr<<"	"<<confidence1+newmeanconf+newprodscal<<"	"<<(2.0/((1.0/(double(max)/double(nbinsub[subid])))+(1.0/(double(max)/double(docslist->NbPtr)))))<<"	"<<(double(max)/double(docslist->NbPtr))<<"	"<<(double(max)/double(nbinsub[subid]))<<endl;


					//assignation des valeurs de tests
					if(nC>bC)
					{
					bA=double(max)/double(nbinsub[subid]);
					bB=double(max)/double(docslist->NbPtr);
					bconf=confidence1;
					bmeanconf=newmeanconf;
					bprodscal=newprodscal;
					bE=confidence1+newmeanconf+newprodscal;
					bnbdoc=docslist->NbPtr;
					bC=nC;
					}
					mA+=double(max)/double(nbinsub[subid]);
					mB+=double(max)/double(docslist->NbPtr);
					mC+=nC;
					mconf+=confidence1;
					mmeanconf+=newmeanconf;
					mprodscal+=newprodscal;
					mE+=confidence1+newmeanconf+newprodscal;
					mnbdoc+=docslist->NbPtr;
					meannb++;


					//Compare
					if((confidence1+newmeanconf+newprodscal+(confidence1*newmeanconf*newprodscal)>lim)&&(docslist->NbPtr>=NbDocsMin)&&(confidence1>MinConf))
					{
						lim=confidence1+newmeanconf+newprodscal+(confidence1*newmeanconf*newprodscal);
						change=true;
						meanconf=newmeanconf;
						prodscal=newprodscal;
						minconf=newminconf;
						id=tocompute2->GetId();
						mem=new RContainer<GDoc,unsigned,false,true>(10);
						for(docslist->Start();!docslist->End();docslist->Next())
							mem->InsertPtr((*docslist)());
						minid=double(max)/double(docslist->NbPtr);
						memsubid=subid;
						conf=confidence1;
						lastmax=max;
						bestdoc=docslist->NbPtr;
						monC=nC;
					}

					delete(docslist);
				}
			}
			//affichage des valeurs test.
			f<<"Valeur pour le meilleur couple :"<<endl;
			f<<bC<<"	"<<bA<<"	"<<bB<<"	"<<bnbdoc<<"	"<<bconf<<"	"<<bmeanconf<<"	"<<bprodscal<<"	"<<bE<<"	"<<endl;
			f<<"Valeur moyennes des couples :"<<endl;
			f<<mC/meannb<<"	"<<mA/meannb<<"	"<<mB/meannb<<"	"<<mnbdoc/meannb<<"	"<<mconf/meannb<<"	"<<mmeanconf/meannb<<"	"<<mprodscal/meannb<<"	"<<mE/meannb<<"	"<<endl;
			tC+=mC/meannb;
			tA+=mA/meannb;
			tB+=mB/meannb;
			tnbdoc+=mnbdoc/meannb;
			tconf+=mconf/meannb;
			tmeanconf+=mmeanconf/meannb;
			tprodscal+=mprodscal/meannb;
			tE+=mE/meannb;
			btC+=bC;
			btA+=bA;
			btB+=bB;
			btnbdoc+=bnbdoc;
			btconf+=bconf;
			btmeanconf+=bmeanconf;
			btprodscal+=bprodscal;
			btE+=bE;
			tmeannb++;


			if(change)
			{
				Groups->GetPtr(Order[i]->GetId())->InsertPtr(new GIWord(id));
				Groups->DeletePtr(id);
/*				//affichage des groupements et de leur caractristique.
				f<<"meilleur B : "<<"Conf = "<<Bconf<<" , Mean = "<<Bmean<<" , Pds = "<<Bpds<<" , A = "<<BA<<" , B = "<<B<<" , E = "<<BE<<" , P = "<<BP<<" , nbdocs = "<<Bdoc<<endl;
				f<<"meilleur A : "<<"Conf = "<<Aconf<<" , Mean = "<<Amean<<" , Pds = "<<Apds<<" , A = "<<A<<" , B = "<<AB<<" , E = "<<AE<<" , P = "<<AP<<" , nbdocs = "<<Adoc<<endl;
				f<<"meilleur A+B : "<<"Conf = "<<ApBconf<<" , Mean = "<<ApBmean<<" , Pds = "<<ApBpds<<" , A = "<<ApBA<<" , B = "<<ApBB<<" , E = "<<ApBE<<" , P = "<<ApBP<<" , nbdocs = "<<ApBdoc<<endl;
*/				f<<"Groupe = ("<<Words->GetPtr(Order[i]->GetId())->GetId()<<","<<Words->GetPtr(id)->GetId()<<") : "<<" , C = "<<monC<<" , A = "<<double(lastmax)/double(nbinsub[memsubid])<<" , B = "<<minid<<" , nbdocs = "<<bestdoc<<"Conf = "<<conf<<" , Mean = "<<meanconf<<" , Pds = "<<prodscal<<" , E = "<<conf+meanconf+prodscal<<" , P = "<<conf*meanconf*prodscal<<endl;
				best<<bestdoc<<"	"<<conf<<"	"<<meanconf<<"	"<<prodscal<<"	"<<(2.0/((1.0/(double(lastmax)/double(nbinsub[memsubid])))+(1.0/(double(lastmax)/double(bestdoc)))))<<endl;
/*				f<<"--------------------------------------------------------------------------------------------------------------"<<endl;*/
//				cout<<Dic->GetWord(Words->GetPtr(Order[i]->GetId())->GetId())<<" / "<<Dic->GetWord(Words->GetPtr(id)->GetId())<<" : Confidence = "<<minconf<<", ";
//				cout<<" / Nombre de docs = "<<double(Words->GetPtr(Order[i]->GetId())->Docs->NbPtr)<<" , support = "<<double(Words->GetPtr(Order[i]->GetId())->Docs->NbPtr)/double(lang->GetRef(otDoc,tWordList))<<endl;
			}
			f<<"------------------------------------------------------------------------------------------------------------------"<<endl;
		}
	}

	f<<"Valeur moyenne pour les meilleurs couples :"<<endl;
	f<<btC/tmeannb<<"	"<<btA/tmeannb<<"	"<<btB/tmeannb<<"	"<<btnbdoc/tmeannb<<"	"<<btconf/tmeannb<<"	"<<btmeanconf/tmeannb<<"	"<<btprodscal/tmeannb<<"	"<<btE/tmeannb<<"	"<<endl;
	f<<"Valeur moyennes de l'ensemble couples :"<<endl;
	f<<tC/tmeannb<<"	"<<tA/tmeannb<<"	"<<tB/tmeannb<<"	"<<tnbdoc/tmeannb<<"	"<<tconf/tmeannb<<"	"<<tmeanconf/tmeannb<<"	"<<tprodscal/tmeannb<<"	"<<tE/tmeannb<<"	"<<endl;

	double conftot;
	for(Groups->Start();!Groups->End();Groups->Next())
	{
		if((*Groups)()->NbPtr>1)
		{
		conftot=double(Words->GetPtr((*Groups)()->GetId())->Docs->NbPtr);
		for(((*Groups)())->Start();!((*Groups)())->End();((*Groups)())->Next())
			cout<<Dic->GetWord(Words->GetPtr((*((*Groups)()))()->GetId())->GetId())<<" : Confidence = "<<double(conftot)/double(Words->GetPtr((*((*Groups)()))()->GetId())->Docs->NbPtr)/*(lang->GetRef((*((*Groups)()))()->GetId(),otDoc))*/<<", ";
		cout<<" / Nombre de docs = "<<conftot<<" , support = "<<double(conftot)/double(lang->GetRef(otDoc,tWordList))<<endl;
		}
	}
	cout<<nbword<<endl;

}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::Sort()
{
	unsigned int MaxtmpSize;
	MaxtmpSize=static_cast<unsigned int>((Words->NbPtr+1)*1.1);
	Order =new GWordDocs*[MaxtmpSize];
	memcpy(Order,Words->Tab,Words->NbPtr*sizeof(GWordDocs*));
	qsort(static_cast<void*>(Order),Words->NbPtr,sizeof(GWordDocs*),GWordDocs::sortOrder);
	Order[Words->NbPtr]=0;
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::CleanWords()
{
	GWordDocs* del,**cur;
	cur=Words->Tab;
	while((*cur))
	{
		if(((unsigned((*cur)->Docs->NbPtr))<NbMinDocs)||((unsigned((*cur)->Docs->NbPtr))>NbMaxDocs))
		{
			del=(*cur);
			Words->DeletePtr(del);
		}
		else
			cur++;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::ConceptSpace()
{
	unsigned int i,j,nbdocs,nbgroups,id,bestnbdocs;
	double confidence1,confidence2,norm1,norm2,prodscal,bestprodscal,bestmeanconf,bestminconf;
	bool change,find,find2;
	GIWordList *toins;
	GWordDocs *tocompute,*tocomputewith;
	GIWordWeight **ptr;
	nbgroups=0;
	Sort();
	cout<<"Nombre de mots  traiter : "<<Words->NbPtr<<endl;
	for(i=0;i<Words->NbPtr;i++)
	{
		tocompute=Words->GetPtr(Order[i]->GetId());
		id=0;
		bestnbdocs=NbDocsMin;
		bestmeanconf=0.0;
		bestprodscal=0.0;
		bestminconf=0.0;
		norm1=0.0;
		for (tocompute->Start();!tocompute->End();tocompute->Next())
			norm1+=((*tocompute)()->GetWeight()*(*tocompute)()->GetWeight());
		change=false;
		for/*(j=i+1;j<Words->NbPtr;j++)*/(ptr=tocompute->Tab,j=tocompute->NbPtr;(--j+1);ptr++)
		{
			tocomputewith=Words->GetPtr((*ptr)->GetId());
			prodscal=0.0;
			norm2=0.0;
			for(tocomputewith->Start();!tocomputewith->End();tocomputewith->Next())
			{
				norm2+=((*tocomputewith)()->GetWeight()*(*tocomputewith)()->GetWeight());
				if(tocompute->IsIn((*tocomputewith)()->GetId()))
					prodscal+=(*tocomputewith)()->GetWeight()*tocompute->GetPtr((*tocomputewith)()->GetId())->GetWeight();
			}
			prodscal=prodscal/(sqrt(norm1)*sqrt(norm2));
			nbdocs=0;
			RContainer<GDoc,unsigned,false,true> *cur1,*cur2;
			cur1=tocompute->Docs;
			cur2=tocomputewith->Docs;
			cur2->Start();

			//Compute nbdocs.
			for(cur1->Start();!cur1->End();cur1->Next())
			{
				while((!cur2->End())&&((*cur2)()->GetId()<(*cur1)()->GetId()))
					cur2->Next();
				if((!cur2->End())&&((*cur2)()->GetId()==(*cur1)()->GetId()))
					nbdocs++;
			}
			//Compute meanconfidence and minconfidence.
			confidence1=0.0;
			confidence2=0.0;
			double meanconf,minconf;
			confidence1=double(nbdocs)/double(tocompute->Docs->NbPtr);
			confidence2=double(nbdocs)/double(tocomputewith->Docs->NbPtr);
			if(confidence1>confidence2) minconf=confidence2;
			else minconf=confidence1;
			meanconf=(confidence1+confidence2)/2;
			//Compare
			if(/*condition*/((minconf+meanconf+prodscal+(minconf*meanconf*prodscal))>(bestminconf+bestmeanconf+bestprodscal+(bestminconf*bestmeanconf*bestprodscal)))&&(nbdocs>=bestnbdocs)&&(minconf>MinConf)  )
			{
				change=true;
				id=(*ptr)->GetId();
				bestnbdocs=nbdocs;
				bestmeanconf=meanconf;
				bestprodscal=prodscal;
				bestminconf=minconf;
			}

		}
		if(change)
		{
			Groups->Start();
			find=false;
			find2=false;
			while((!Groups->End())&&(!find))
			{
				if((*Groups)()->IsIn(Order[i]->GetId())) find=true;
				else Groups->Next();
			}
			Groups->Start();
			while((!Groups->End())&&(!find2))
			{
				if((*Groups)()->IsIn(id)) find2=true;
				else Groups->Next();
			}
			if((find)&&(!find2))
			{
				Groups->Start();
				find=false;
				while((!Groups->End())&&(!find))
				{
					if((*Groups)()->IsIn(Order[i]->GetId())) find=true;
					else Groups->Next();
				}
				(*Groups)()->InsertPtr(new GIWord(id));
			}
			if((find2)&&(!find))
			{
				Groups->Start();
				find2=false;
				while((!Groups->End())&&(!find2))
				{
					if((*Groups)()->IsIn(id)) find2=true;
					else Groups->Next();
				}
				(*Groups)()->InsertPtr(new GIWord(Order[i]->GetId()));
			}
			if((!find)&&(!find2))
			{
				toins=new GIWordList();
				toins->SetId(nbgroups);
				toins->InsertPtr(new GIWord(Order[i]->GetId()));
				toins->InsertPtr(new GIWord(id));
				Groups->InsertPtr(toins);
				nbgroups++;

			}
		}
	}
	cout<<"Nombre de groupes crs : "<<nbgroups<<endl;
}


//-----------------------------------------------------------------------------
bool GALILEI::GWordsClustering::OrdreByDocs(unsigned n)
{
	GIWordList* tocompute,*toins,*tocompute2;
	unsigned i,k,l,nb,maxptr,id,it;
	RContainer<GDoc,unsigned,false,true>* docslist,*mem;
	bool change,end;
	double confidence,conftot,nbmax,nbmin,minconf;
	GLang *lang=Dic->GetLang();
	nbmax=0.1;
	nbmin=0.1;
	mem=new RContainer<GDoc,unsigned,false,true>(10);
	it=1;
	id=0;
	change=false;
	end=false;
	Sort();
	if(n==0)
	{
		for(i=0;i<Words->NbPtr;i++)
		{
			toins=new GIWordList();
			toins->SetId(Order[i]->GetId());
			toins->InsertPtr(new GIWord(Order[i]->GetId()));
			Groups->InsertPtr(toins);
		}
	}
	end=true;
	maxptr=Groups->NbPtr;
	for(i=0;i<maxptr;i++)
	{
		if(Groups->GetPtrAt(i))
		{
			tocompute=Groups->GetPtrAt(i);
			change=false;
			for(l=i+1;l<maxptr;l++)
			{
				if(Groups->GetPtrAt(l))
				{
					tocompute2=Groups->GetPtrAt(l);
					nb=0;
					k=0;
					RContainer<GDoc,unsigned,false,true> *cur1,*cur2;
					docslist=new RContainer<GDoc,unsigned,false,true>(10);
					cur1=Words->GetPtr(tocompute->GetId())->Docs;
					cur2=Words->GetPtr(tocompute2->GetId())->Docs;
					cur2->Start();
					for(cur1->Start();!cur1->End();cur1->Next())
						{
							while((!cur2->End())&&((*cur2)()->GetId()<(*cur1)()->GetId()))
								cur2->Next();
							if((!cur2->End())&&((*cur2)()->GetId()==(*cur1)()->GetId()))
							{
								docslist->InsertPtr((*cur1)());
							}
						}
						conftot=0.0;
						minconf=0.15;
						confidence=0.0;
						for(tocompute->Start();!tocompute->End();tocompute->Next())
						{
							if((double(docslist->NbPtr)/double(lang->GetRef((*tocompute)()->GetId(),otDoc)))<minconf) minconf=(double(docslist->NbPtr)/double(lang->GetRef((*tocompute)()->GetId(),otDoc)));
							confidence+=(double(docslist->NbPtr)/double(lang->GetRef((*tocompute)()->GetId(),otDoc)));
							conftot++;
						}
						for(tocompute2->Start();!tocompute2->End();tocompute2->Next())
						{
							if((double(docslist->NbPtr)/double(lang->GetRef((*tocompute2)()->GetId(),otDoc)))<minconf) minconf=(double(docslist->NbPtr)/double(lang->GetRef((*tocompute2)()->GetId(),otDoc)));
							confidence+=(double(docslist->NbPtr)/double(lang->GetRef((*tocompute2)()->GetId(),otDoc)));
							conftot++;
						}
						confidence=confidence/conftot;
						if((docslist->NbPtr>5)&&(confidence>nbmax)&&(minconf>nbmin))
						{
							change=true;
							nbmax=/*docslist->NbPtr*/confidence;
							nbmin=minconf;
							id=tocompute2->GetId();
							mem=new RContainer<GDoc,unsigned,false,true>(10);
							for(docslist->Start();!docslist->End();docslist->Next())
								mem->InsertPtr((*docslist)());
						}
					delete(docslist);
				}
			}
			if(change)
			{
				toins=Groups->GetPtr(id);
				for(toins->Start();!toins->End();toins->Next())
					tocompute->InsertPtr(new GIWord((*toins)()->GetId()));
				delete(Words->GetPtr(tocompute->GetId())->Docs);
				Words->GetPtr(tocompute->GetId())->Docs=new RContainer<GDoc,unsigned,false,true>(10);
				for(mem->Start();!mem->End();mem->Next())
					Words->GetPtr(tocompute->GetId())->Docs->InsertPtr((*mem)());
				end=false;
				change=false;
				Groups->DeletePtr(toins);
				nbmin=0.1;
				nbmax=0.1;
			}
		}
	}
	for(Groups->Start();!Groups->End();Groups->Next())
	{
		if((*Groups)()->NbPtr>1)
		{
		conftot=double(Words->GetPtr((*Groups)()->GetId())->Docs->NbPtr);
		for(((*Groups)())->Start();!((*Groups)())->End();((*Groups)())->Next())
			cout<<Dic->GetWord(Words->GetPtr((*((*Groups)()))()->GetId())->GetId())<<" : Confidence = "<<double(conftot)/double(lang->GetRef((*((*Groups)()))()->GetId(),otDoc))<<", ";
		cout<<" / Nombre de docs = "<<conftot<<" , support = "<<double(conftot)/double(lang->GetRef(otDoc,tWordList))<<endl;
		}
	}
	return(end);
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::View()
{
	GLang *lang=Dic->GetLang();
	double TotalRefWL=lang->GetRef(otDoc,tWordList);
	for(Dic->GroupsList.Start();!Dic->GroupsList.End();Dic->GroupsList.Next())
	{
		cout<<(Dic->GroupsList)()->GetId()<<" = ";
		for((Dic->GroupsList)()->List->Start();!(Dic->GroupsList)()->List->End();(Dic->GroupsList)()->List->Next())
			cout<<(*(Dic->GroupsList)()->List)()->GetId()<<" / ";
		cout<<" Idf = "<<log(TotalRefWL/lang->GetRef((Dic->GroupsList)()->GetId(),otDoc))<<" ";
		cout<<endl;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::UpdateDoc(GDocVector* doc)
{
	unsigned mult;
	double nbocc;
	bool isin;
	doc->RemoveRefs();
	for(Dic->GroupsList.Start();!Dic->GroupsList.End();Dic->GroupsList.Next())
	{
		if(!doc->IsIn((Dic->GroupsList)()->GetId()))
		{
			nbocc=1000.0;
			isin=true;
			mult=0;
			for((Dic->GroupsList)()->List->Start();!(Dic->GroupsList)()->List->End();(Dic->GroupsList)()->List->Next())
			{
				mult++;
				if(doc->IsIn((*(Dic->GroupsList)()->List)()->GetId()))
				{
					if(nbocc>doc->GetPtr((*(Dic->GroupsList)()->List)()->GetId())->GetWeight())
						nbocc=doc->GetPtr((*(Dic->GroupsList)()->List)()->GetId())->GetWeight();
				}
				else isin=false;
			}
			if((isin)&&(nbocc!=0)&&(mult!=0))
			{
				doc->AddWordList((Dic->GroupsList)()->GetId(),nbocc*mult*4);
			}
		}
	}
	doc->UpdateRefs();
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::UpdateDocbis(GDocVector* doc)
{
	unsigned mult;
	double nbocc;
	bool isin;
	doc->RemoveRefs();
	for(Dic->GroupsList.Start();!Dic->GroupsList.End();Dic->GroupsList.Next())
	{
		if(!doc->IsIn((Dic->GroupsList)()->GetId()))
		{
			nbocc=0.0;
			isin=false;
			mult=0;
			for((Dic->GroupsList)()->List->Start();!(Dic->GroupsList)()->List->End();(Dic->GroupsList)()->List->Next())
			{
				mult++;
				if(doc->IsIn((*(Dic->GroupsList)()->List)()->GetId()))
				{
					isin=true;
					if(nbocc<doc->GetPtr((*(Dic->GroupsList)()->List)()->GetId())->GetWeight())
						nbocc+=doc->GetPtr((*(Dic->GroupsList)()->List)()->GetId())->GetWeight();
				}
			}
			if((isin)&&(nbocc!=0)&&(mult!=0))
			{
				doc->AddWordList((Dic->GroupsList)()->GetId(),nbocc);
			}
		}
	}
	doc->UpdateRefs();
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::ReverseUpdateDoc(GDocVector* doc)
{
	for(Dic->GroupsList.Start();!Dic->GroupsList.End();Dic->GroupsList.Next())
	{
		if(doc->IsIn((Dic->GroupsList)()->GetId()))
			doc->DeletePtr((Dic->GroupsList)()->GetId());
	}
	doc->UpdateRefs();
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::UpdateDocter(GDocVector* doc)
{
	unsigned mult;
	double nbocc;
	bool isin;
	doc->RemoveRefs();
	for(Dic->GroupsList.Start();!Dic->GroupsList.End();Dic->GroupsList.Next())
	{
		if(!doc->IsIn((Dic->GroupsList)()->GetId()))
		{
			nbocc=1000.0;
			isin=true;
			mult=0;
			for((Dic->GroupsList)()->List->Start();!(Dic->GroupsList)()->List->End();(Dic->GroupsList)()->List->Next())
			{
				mult++;
				if(doc->IsIn((*(Dic->GroupsList)()->List)()->GetId()))
				{
					if(nbocc>doc->GetPtr((*(Dic->GroupsList)()->List)()->GetId())->GetWeight())
						nbocc=doc->GetPtr((*(Dic->GroupsList)()->List)()->GetId())->GetWeight();
				}
				else isin=false;
			}
			if(isin)
			{
				doc->AddWordList((Dic->GroupsList)()->GetId(),nbocc*mult);
				for((Dic->GroupsList)()->List->Start();!(Dic->GroupsList)()->List->End();(Dic->GroupsList)()->List->Next())
					doc->DeletePtr((*(Dic->GroupsList)()->List)()->GetId());
			}
		}
	}
	doc->UpdateRefs();
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::SaveAssociations(bool save)
{
	GWordList *tmp;
	bool tosave;
	unsigned mult;
	for(Groups->Start();!Groups->End();Groups->Next())
	{
		tosave=true;
		if((tosave)&&((*Groups)()->NbPtr>1))
		{
			mult=0;
			tmp=new GWordList(cNoRef,"grouplist"+itoa(Dic->GetNbGroupsList()));
			((*Groups)())->Start();
			while(!((*Groups)())->End())
			{
				tmp->InsertWord(new GWord((*((*Groups)()))()->GetId(),Dic->GetWord((*((*Groups)()))()->GetId())));
				mult++;
				((*Groups)())->Next();
			}
			if(mult>1) Dic->InsertNewWordList((*tmp),save);
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::SaveConceptSpace(bool save)
{
	GWordList *tmp;
	bool tosave;
	unsigned mult;
	for(Groups->Start();!Groups->End();Groups->Next())
	{
		tosave=true;
		if((tosave)&&((*Groups)()->NbPtr>1))
		{
			mult=0;
			tmp=new GWordList(cNoRef,"grouplist"+itoa(Dic->GetNbGroupsList()));
			((*Groups)())->Start();
			while(!((*Groups)())->End())
			{
				tmp->InsertWord(new GWord((*((*Groups)()))()->GetId(),Dic->GetWord((*((*Groups)()))()->GetId())));
				mult++;
				((*Groups)())->Next();
			}
			if(mult>1) Dic->InsertNewWordList((*tmp),save);
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GWordsClustering::Clear()
{
//	if(Doc)
//	{
//		Doc->Clear();
//	}
	if(Words)
	{
		Words->Clear();
	}
}


//-----------------------------------------------------------------------------
GWordsClustering::~GWordsClustering()
{
	if (Order) delete[] Order;
	if (Docs) delete(Docs);
	if (Groups) delete(Groups);
	if (Words) delete(Words);
}

