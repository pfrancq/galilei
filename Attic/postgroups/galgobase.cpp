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
#include <postgroups/galgobase.h>
#include <postgroups/gcpov.h>
#include <postgroups/gexpertkwds.h>
// #include <groups/ggroups.h>
// #include <groups/ggroup.h>
// #include <infos/giwordweight.h>
// #include <infos/giwordsweights.h>
 #include <profiles/gprofdoc.h>
 #include <profiles/gprofile.h>
 #include <profiles/gsubprofilevector.h>
 #include <langs/glang.h>
 #include <langs/gdict.h>
 #include <docs/gdocvector.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for R Project
// #include <rstd/rcontainer.h>
// #include <rstd/rrecfile.h>
// #include <rstd/rcursor.h>
// #include <rdb/rmysql.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class GAlgobase
//
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
GALILEI::GAlgobase::GAlgobase() throw(bad_alloc)
      : GGroup()
{
	// ChainPov = new RContainer<GExpertKwds,unsigned,false,true>(10,5);
	Pov =  new RContainer<GCpov,unsigned,false,true>(10,5);
}


// ----------------------------------------------------------------------------
unsigned int GALILEI::GAlgobase::GetId()
{
	return Id;
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::SetCurPrf(GSubProfile * cprf)
{
	CurrentPrf = cprf;
}


// ----------------------------------------------------------------------------
GSubProfile * GALILEI::GAlgobase::GetCurPrf(void)
{
	return CurrentPrf;
}


// ----------------------------------------------------------------------------
GGroup * GALILEI::GAlgobase::GetCurGrp()
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
void GALILEI::GAlgobase::BuildPoVGroup(GGroup * grp, GSession *s)
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

	Sp=grp->GetSubProfilesCursor();
     
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
			Ccur = Vect->GetVectorCursor();    			// ici
			AnalysePoVProfile(s);            
			FormatOutData(s);
		}
	}
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::AnalysePoVProfile(GSession *s)
{
	double poid = 0.0;
	double max = 0.0;
	GSubProfileVector *Vect;
	GIWordsWeights* GIwws;
	GDict *dic;
	GLang *lang;
	double TotalRef = Lng->GetRef(otSubProfile,tWordList);
  char theword[128];
  
  	//Vect = (GSubProfileVector*)Sp();
    Vect = (GSubProfileVector *)GetCurPrf();
  	GIwws=Vect->GetVector();
  	max = GIwws->GetMaxWeight();
                    
	for(Ccur.Start(); !Ccur.End(); Ccur.Next())     // traite les kwds des docs ok
	{
		poid=Ccur()->GetWeight();                 // frequence calculee

		Data = new GCpov(Ccur()->GetId());
              
		Data->InsertIdw(  Ccur()->GetId() );
		
		Data->InsertIdfFactor( (poid/max)*log(TotalRef/Lng->GetRef(Data->GetIdw(),otSubProfile)) );

		lang = Vect->GetLang();
		dic = lang->GetDict();
		dic->GetWord(Ccur()->GetId());
		memset(theword,0,128);
		strcpy(theword,dic->GetWord(Ccur()->GetId()));
		Data->SetWord(theword);

		InsertNewExpertKwds();
	}

	
	CalcHierarchy(s);

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
void GALILEI::GAlgobase::CalcHierarchy(GSession* /*s*/)
{
	GDocVector* docu;
	GProfDocCursor MyDoc;
	bool contains = 0;
	int trouve = 0;
	int pastrouve = 0;
	int nbdoc = 0;
	//int i = 0;
	GExpertKwds *occdoc;
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
				occdoc = new GExpertKwds(MyDoc()->GetDoc()->GetId());

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
int GALILEI::GAlgobase::CalcLevel(GStandardInOutPut *Clink)
{
	GCpovCursor curs1 = GetGCpovCursor();
	int  i,tmp=0;
	int stop = 0;

	for(curs1.Start(); !curs1.End(); curs1.Next())
	{
		if( curs1()->GetIdw() == Clink->GetIdw())
			continue;
		for( i = 0, stop = curs1()->GetNbParent(); i < stop; i++)
		{
			if(curs1()->GetIdw() == Clink->Parent[i][0])
			{
				tmp = Clink->GetLevel() + 1;
				Clink->SetLevel(tmp);
			}  
		}
	}
	if(tmp == 0) tmp = 1;
  return tmp;
//		for(curs1.Start(); !curs1.End(); curs1.Next())
//	{
//		if( curs1()->GetIdw() == Clink->GetIdw())
//			continue;
//		for( i = 0, stop = curs1()->GetNbParent(); i < stop; i++)
//		{
//			if(curs1()->GetIdw() == Clink->Parent[i][0])
//			{
//				tmp = Clink->GetLevel();
//				Clink->SetLevel(tmp++);
//			}
//		}
//	} 
}


// ----------------------------------------------------------------------------
void GALILEI::GAlgobase::FormatOutData(GSession *s)
{
	int i = 0;
	int par= 0;
	int freq = 0;
//	char tmp[100];

	GCpovCursor cursor = GetGCpovCursor();
	Out = new GInOutputBase();
	for(cursor.Start(); !cursor.End(); cursor.Next())
	{
		DataOut = new GStandardInOutPut();
		DataOut->CleanFreq();
		DataOut->CleanParent();
		DataOut->SetProfile(GetCurPrf());
		DataOut->SetGroup(GetCurGrp());
		DataOut->SetIdw(cursor()->GetIdw());
		DataOut->SetWord(cursor()->GetWord());
		for( i = 0; i < cursor()->GetNbParent(); i++)
		{
			par = cursor()->GetOneParent(i);
			freq = cursor()->GetFreqParent(i);
			DataOut->SetParent(i,par,freq);
			DataOut->AddFreq(freq);
			DataOut->AddParent(par);
		}
		DataOut->SetLevel(CalcLevel(DataOut));
		Out->StdInOutPut->InsertPtr(DataOut);
		
//		SaveDocsList(cursor()->IsInDocs);						// save le list of attatched documents
	}
	 s->SavePointOfView(Out->StdInOutPut);
}





