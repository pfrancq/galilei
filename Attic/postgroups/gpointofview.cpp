/*

	GALILEI Research Project

	PointOfView.cpp

	Compare a ideal groupement with a computed one - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Stphane Rideau (srideau@ulb.ac.be).

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



// ----------------------------------------------------------------------------
//include file for GALILEI
#include <postgroups/gpointofview.h>
//#include <sessions/gsessionmysql.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
#include <profiles/gprofdoc.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofilevector.h>
#include <langs/glang.h>
#include <docs/gdocvector.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rio/rrecfile.h>
#include <rstd/rcursor.h>
#include <rmysql/rmysql.h>
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class ExpertKwds
//
//-----------------------------------------------------------------------------

                       
// ----------------------------------------------------------------------------
GALILEI::ExpertKwds::ExpertKwds(void) throw(bad_alloc)
{
 //  GCpovs = new RContainer<GCpov,unsigned int,false,true>(10,5);
 // Id = 0;
}


// ----------------------------------------------------------------------------
GALILEI::ExpertKwds::ExpertKwds(unsigned ident) throw(bad_alloc)
{
	Id = ident;
}  



// ----------------------------------------------------------------------------
int GALILEI::ExpertKwds::Compare(const ExpertKwds* n) const
{
	return(Id-(n->GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::ExpertKwds::Compare(const ExpertKwds& n) const
{
	return(Id-(n.GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::ExpertKwds::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
unsigned GALILEI::ExpertKwds::GetId() const
{
	  return(Id);
}


//-----------------------------------------------------------------------------
void GALILEI::ExpertKwds::SetId(unsigned int ident)
{
	Id = ident;
}


//-----------------------------------------------------------------------------
//void GALILEI::ExpertKwds::AddPov(GCpov* pov)
//{
//	  GCpovs->InsertPtr(pov);
//}

//-----------------------------------------------------------------------------
//
// class GCpov
//
//-----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
GALILEI::GCpov::GCpov()
{
	Id = GetId();
	IdfFactor = 0.0;
	Idw = 0;
	Rdist = 0.0;
	NbParent = 5;    // a parametrer dans input
       
	IsInDocs = new RContainer<ExpertKwds,unsigned int,false,true>(10,5);
}


// ----------------------------------------------------------------------------
GALILEI::GCpov::GCpov(unsigned int id)
   : Id(id)
{
	IdfFactor = 0.0;
	Idw = 0;
	Rdist = 0.0;
	NbParent = 5;    // a parametrer dans input
          
	IsInDocs = new RContainer<ExpertKwds,unsigned int,false,true>(10,5);
}



   
// ----------------------------------------------------------------------------
void GALILEI::GCpov::InsertIdfFactor(double idfvalue)
{
	IdfFactor = idfvalue;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::InsertIdw(int kwdsvalue)
{
	Idw = kwdsvalue;
}


// ----------------------------------------------------------------------------
double GALILEI::GCpov::Getidfac()
{
	return(IdfFactor);
}


// ----------------------------------------------------------------------------
unsigned int GALILEI::GCpov::GetIdw()
{
	return(Idw);
}


// ----------------------------------------------------------------------------
unsigned int GALILEI::GCpov::GetId() const
{
	return(Id);
}

// ----------------------------------------------------------------------------
void GALILEI::GCpov::SetId(unsigned int id)
{
	Id = id;
}

// ----------------------------------------------------------------------------
int GALILEI::GCpov::Compare(const GCpov* n) const
{
	return(Id-(n->GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::Compare(const GCpov& n) const
{
	return(Id-(n.GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::Compare(const unsigned id) const
{
	return(Id-id);
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::SetRdist(double dr)
{
	Rdist = dr;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::IncrId()
{
	Id++;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::SetNbParent(int NbParents)
{
	NbParent = NbParents;
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::GetNbParent(void)
{
	return NbParent;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::CleanParent()
{
	int i,j;

	for(i = 0; i < NbParent; i++)
		for(j = 0; j < 2; j++)
			Parent[i][j] = 0;
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::AddParent(int value, int freq)
{
	int i;

	for(i = 0; i < NbParent; i++)
	{
		if(Parent[i][0]==0)
		{
			Parent[i][0] = value;
			Parent[i][1] = freq;
			return 1;
		}
	}
	return 0;     
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::RemplaceParent(int position, int value, int freq)
{
	Parent[position][0] = value;
	Parent[position][1] = freq;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::AddRemplaceParent(int IdParent, int freq )
{
	int i;

	for( i = 0; i < NbParent; i++)
	{
		if(Parent[i][1] < freq)
		{
			Parent[i][0] = IdParent;
			Parent[i][1] = freq;
			break;
		}
	}
}

// ----------------------------------------------------------------------------
int GALILEI::GCpov::GetOneParent(int position)
{
	return Parent[position][0];
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::GetFreqParent(int position)
{
	return Parent[position][1];
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::GetAllParent(void)
{
	return 1;
}


//-----------------------------------------------------------------------------
//
// class GDispatchpov
//
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void GALILEI::GDispatchpov::GeneratePoV(RContainer<GGroups,unsigned int,true,true>* /*groups*/)
{

}


// ----------------------------------------------------------------------------
void GALILEI::GDispatchpov::GeneratePoVfromScratch(GGroup* Grp)
{
	//InputBase *fomatIn;
	OutputBase *fomatOut;
	GAlgobase *Calcul;

	cout << "GeneratePoVfromScratch" << endl;   
	// selon algorithme choisi
  
	//fomatIn->FormatInBase();   
	Calcul = new GAlgobase();
	Calcul->BuildPoVGroup(Grp);

	// fomatOut = new OutputBase();
	fomatOut->FormatOutBase(); 

	// selon algo choisi end
}


// ----------------------------------------------------------------------------
void GALILEI::GDispatchpov::UpdatePovForAGrp()
{
	InputBase *fomatIn;
	OutputBase *fomatOut;

	cout << "GeneratePoVfromScratch" << endl;
	// selon algorithme choisi
	fomatIn->FormatInBase();   // formatage entree
	fomatOut->FormatOutBase(); // formatage sortie
}


//-----------------------------------------------------------------------------
//
// class GAlgobase
//
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
GALILEI::GAlgobase::GAlgobase() throw(bad_alloc)
      : GGroup()
{
	// ChainPov = new RContainer<ExpertKwds,unsigned,false,true>(10,5);
	Pov =  new RContainer<GCpov,unsigned,false,true>(10,5);
}


// ----------------------------------------------------------------------------
unsigned int GALILEI::GAlgobase::GetId()
{
	return Id;
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::SetCurPrf(GSubProfile *cprf)
{
	CurrentPrf = cprf;
}


// ----------------------------------------------------------------------------
GSubProfile * GALILEI::GAlgobase::GetCurPrf()
{
	return CurrentPrf;
}


// ----------------------------------------------------------------------------
GGroup *GALILEI::GAlgobase::GetCurGrp()
{
	return CurGrp;
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::SetCurGrp(GGroup* grp)
{
	CurGrp = grp;
}


// ----------------------------------------------------------------------------
GCpovCursor& GALILEI::GAlgobase::GetGCpovCursor()
{
	GCpovCursor *cur = GCpovCursor::GetTmpCursor();
	cur->Set(Pov);
	return(*cur);
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::BuildPoVGroup(GGroup * grp)
{
	//InputBase *fomatIn;
	//OutputBase *fomatOut;
	GSubProfileVector *Vect;
	GSubProfile *Subp;
	GIWordsWeights* GIwws;
	GIWordWeight** GIww;
	int nbp = 0;

	//GSubProfile *cprf;

	if(grp->IsEmpty())
		return;
	
	Lng = grp->GetLang();
	SetCurGrp(grp);         
	/*     if(!fomatIn->FormatInBase() )       
        	return;
	*/

	Sp=grp->GetSubProfileCursor();       
     
	EvaluateMaxExpertKwds( );
	for(Sp.Start();!Sp.End();Sp.Next(),nbp++)
	{
		cout<<"scan profile "<<nbp<<endl;
         
		if( (GSubProfileVector*)Sp()->IsDefined() )    
		{
			Subp = Sp();
			SetCurPrf(Subp);
			Vect = (GSubProfileVector*)Sp();
			GIwws=Vect->GetVector();
			GIww=GIwws->Tab;
			Ccur = Vect->GetVectorCursor();
			AnalysePoVProfile();            
			FormatOutData();
		}
	}
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::AnalysePoVProfile()
{
	double poid = 0.0;
	double max = 0.0;
	GSubProfileVector *Vect;
	GIWordsWeights* GIwws;
	//GIWordWeight** GIww;
	double TotalRef = Lng->GetRef(otSubProfile,tWordList);


	Vect = (GSubProfileVector*)Sp();
	GIwws=Vect->GetVector();
	max = GIwws->GetMaxWeight();
            
	for(Ccur.Start(); !Ccur.End(); Ccur.Next())     // traite les kwds des docs ok
	{
		poid=Ccur()->GetWeight();                 // frequence calculee

		Data = new GCpov(Ccur()->GetId());
              
		Data->InsertIdw(  Ccur()->GetId() );    

		Data->InsertIdfFactor( (poid/max)*log(TotalRef/Lng->GetRef(Data->GetIdw(),otSubProfile)) );

		InsertNewExpertKwds();
	}

	CalcHierarchy();

}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::InsertNewExpertKwds()
{
	//	int currentpt;
	//	int nbe = 0;
	//	double IdfPrec = 0.0;
	//	double Maxdist = 1000000;
	double smallest = 1000000;
	GCpov *tmp = new GCpov();
	bool add = 0;

   
	if(Pov->NbPtr < MaxExpertKwds) 
	{
		// Data->IncrId();
		Pov->InsertPtr(Data);
	}
	else
	{
		for(Pov->Start(); !Pov->End(); Pov->Next())
		{
			if( ((*Pov)()->Getidfac() < Data->Getidfac() ) && ((*Pov)()->Getidfac() < smallest) )
			{
				smallest = (*Pov)()->Getidfac();
				tmp = (*Pov)();
				add = 1;
			}
		}

		if(add)
		{
			Pov->DeletePtr(tmp);
			Pov->InsertPtr(Data);
			add = 0;
		}
	}

	delete(tmp);

//    for(Pov->Start(); !Pov->End(); Pov->Next())
//    {
//       IdfPrec = (*Pov)()->Getidfac();
//       nbe++;
//		}
//		Data->Rdist = Data->idffac - (IdfPrec/nbe);
//		for(Pov->Start(); !Pov->End(); Pov->Next())
//    {
//       if(Data->Rdist - (*Pov)()->Getidfac()  < Maxdist )
//       {
//         	Maxdist =  Data->Rdist - (*Pov)()->Getidfac();
//          Data->Parent = (*Pov)()->GetId();
//			 }
//		}
//		cout<<"Data->Rdist : "<<Data->Rdist<<" Data->idffac : "<<Data->idffac<<"Data->Parent : "<<Data->Parent<<endl;
}
 

// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::EvaluateMaxExpertKwds()   // mettre dans prepare input
{
	double TotalRef = Lng->GetRef(otSubProfile,tWordList);
	// selon parametrage
	MaxExpertKwds = int(abs(log(TotalRef)*10)); 
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::CalcHierarchy()
{
	GDocVector* docu;
	GProfDocCursor MyDoc;
	bool contains = 0;	
	int trouve = 0;
	int pastrouve = 0;
	int nbdoc = 0;
	//int i = 0;
	ExpertKwds *occdoc;
	GCpovCursor curs1 = GetGCpovCursor();
	GCpovCursor curs2 = GetGCpovCursor();
	//GCpov *tmp;
   
	MyDoc = (GetCurPrf())->GetProfile()->GetProfDocCursor();
  
	for(curs1.Start(); !curs1.End(); curs1.Next())
	{ 
		for(MyDoc.Start(); !MyDoc.End(); MyDoc.Next())
		{
			docu = dynamic_cast<GDocVector*>(MyDoc()->GetDoc());
    
			if(contains = docu->IsIn(curs1()->GetId()) )
			{
				trouve++;  // insert dans container
  				occdoc = new ExpertKwds(MyDoc()->GetDoc()->GetId());
				curs1()->IsInDocs->InsertPtr(occdoc);
   			}
			else
				pastrouve++;
		}
		trouve = pastrouve = 0; 
	}
// ***************$ debug
//	 for(curs1.Start(); !curs1.End(); curs1.Next())
//   {
//      cout<<"curs : "<<curs1()->GetId()<<" NBe : "<<curs1()->IsInDocs->NbPtr<<endl;
//   }
//********************    


	for(curs1.Start(); !curs1.End(); curs1.Next())
	{
		curs1()->CleanParent();
		curs1()->SetNbParent(5);         // warning ! get the right parameter
		for(curs2.Start(); !curs2.End(); curs2.Next())
		{
			if(curs1() == curs2())
				continue;
			for(curs1()->IsInDocs->Start(); !curs1()->IsInDocs->End(); curs1()->IsInDocs->Next())
			{
				for(curs2()->IsInDocs->Start(); !curs2()->IsInDocs->End(); curs2()->IsInDocs->Next())
				{
					if((*curs1()->IsInDocs)()->GetId() == (*curs2()->IsInDocs)()->GetId() )
					{
						nbdoc++;
						continue;
					}
				}
			}
			if(nbdoc > 0)
				if(!curs1()->AddParent(curs2()->GetIdw(), nbdoc) )
					curs1()->AddRemplaceParent(curs2()->GetIdw(),nbdoc);
			nbdoc = 0;
		}
	}

//   	 for(curs1.Start(); !curs1.End(); curs1.Next())
//    {
//			 for(i = 0; i < 5; i++)
//        cout<<curs1()->Parent[i][0]<<" "<<curs1()->Parent[i][1]<<endl;
//        cout<<"---------"<<endl;
//		}
//		cout<<"ay"<<endl;
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::CalcLevel(StandardOutPut* Clink)
{
	GCpovCursor curs1 = GetGCpovCursor();
	int  i,tmp;
	int stop = 0;

	for(curs1.Start(); !curs1.End(); curs1.Next())
	{
		if( curs1()->GetIdw() == Clink->GetIdw())
			continue;
		for( i = 0, stop = curs1()->GetNbParent(); i < stop; i++)
		{
			if(curs1()->GetIdw() == Clink->Parent[i][0])
			{
				tmp = Clink->GetLevel();
				Clink->SetLevel(tmp++);
			}  
		}
	} 
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::FormatOutData()
{
	int i = 0;
	int par= 0;
	int freq = 0;
	GCpovCursor cursor = GetGCpovCursor();
	Out = new OutputBase();

	for(cursor.Start(); !cursor.End(); cursor.Next())
	{
		DataOut = new StandardOutPut();
		DataOut->SetIdw(cursor()->GetIdw());
		for( i = 0; i < cursor()->GetNbParent(); i++)
		{
			 par = cursor()->GetOneParent(i);
			 freq = cursor()->GetFreqParent(i);
			DataOut->SetParent(i,par,freq);
		}
		DataOut->SetProfile(GetCurPrf());
		DataOut->SetGroup(GetCurGrp());
		CalcLevel(DataOut);
		Out->StdOutPut->InsertPtr(DataOut);
	}
}



//-----------------------------------------------------------------------------
//
// class InputBase
//
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
int GALILEI::InputBase::FormatInBase(void)
{
	return 1;  // data accessible coherente
}


//-----------------------------------------------------------------------------
//
// class StandardOutPut
//
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
GALILEI::StandardOutPut::StandardOutPut(void)
{
	GroupId = 0;
	SProfile = NULL;
	Idw = 0;
	memset(Parent,0,sizeof(Parent));
	Level = 0; 
}


// ----------------------------------------------------------------------------
int GALILEI::StandardOutPut::Compare(const StandardOutPut* n) const
{
	return(Id-(n->GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::StandardOutPut::Compare(const StandardOutPut& n) const
{
	return(Id-(n.GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::StandardOutPut::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
unsigned GALILEI::StandardOutPut::GetId() const
{
	return(Id);
}

// ----------------------------------------------------------------------------
void GALILEI::StandardOutPut::SetIdw(unsigned int id)
{
	Idw = id;
}


// ----------------------------------------------------------------------------
unsigned int GALILEI::StandardOutPut::GetIdw(void)
{
	return Idw;
}

// ----------------------------------------------------------------------------
void GALILEI::StandardOutPut::SetParent(int position, int value, int freq)
{
	Parent[position][0] = value;
	Parent[position][1] = freq;
}

// ----------------------------------------------------------------------------
void GALILEI::StandardOutPut::SetProfile(GSubProfile *sprofile)
{
	SProfile = sprofile;
}


// ----------------------------------------------------------------------------
void GALILEI::StandardOutPut::SetGroup(GGroup *grp)
{
	Grp = grp;
}

// ----------------------------------------------------------------------------
void GALILEI::StandardOutPut::SetLevel(int level)
{
	Level = level;
}


// ----------------------------------------------------------------------------
int GALILEI::StandardOutPut::GetLevel(void)
{
	return Level;
}

// ----------------------------------------------------------------------------
GALILEI::StandardOutPut::~StandardOutPut(void)
{
}


//-----------------------------------------------------------------------------
//
// class OutputBase
//
//-----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
GALILEI::OutputBase::OutputBase()
{
	StdOutPut = new RContainer<StandardOutPut,unsigned int,false,true>(10,5);   //**** debug
}


// ----------------------------------------------------------------------------
GOutPutCursor& GALILEI::OutputBase::GetGOutPutCursor()
{
	GOutPutCursor *cur = GOutPutCursor::GetTmpCursor();
	cur->Set(StdOutPut);
	return(*cur);
}


// ----------------------------------------------------------------------------
int GALILEI::OutputBase::FormatOutBase(void)
{

	GOutPutCursor Outpt = GetGOutPutCursor();	

	for(Outpt.Start(); !Outpt.End(); Outpt.Next())
	{
		// save datas   ;
	}
//	GAlgobase *final;
//
//	StdOutPut = new RContainer<StandardOutPut,unsigned int,false,true>(10,5);   //**** debug
//	final = new GAlgobase();
//	final->FormatOutData();

	// Save final->dataOut
	return 1;  // dat a accessible coherente
}                          




