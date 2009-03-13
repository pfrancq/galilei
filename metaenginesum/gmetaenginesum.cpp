/*

	GALILEI Research Project

	GMetaEngineSum.cpp

	Meta Engine for extraction of results from different search engines - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
// include files for ANSI C/C++
#include <ctype.h>
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R
#include <rdownload.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gmetaenginesum.h>
#include <gengine.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
//
// class GMetaEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngineSum::GMetaEngineSum(GFactoryMetaEngine* fac)
	:GMetaEngine(fac),Results(100,50),KeyWords(10,5),QueryWords(10,5),UseWeight(false),MaxResNb(-1),NValue(10),
		SingleQuery(true),RanchQuery(false),PercentValue(true),PLowerPC(50),PUpperPC(70),PLowerAbs(5),PUpperAbs(7)
{
}


//-----------------------------------------------------------------------------
void GMetaEngineSum::ApplyConfig(void)
{

//	UseWeight=Factory->GetBool("UseWeight");
//	if(Factory->GetBool("LimitRes"))
//	{
//		MaxResNb=Factory->GetUInt("MaxResNb");
//	}
//	else
//		MaxResNb=-1;
//	NValue=Factory->GetUInt("NValue");
//	unsigned int mode=Factory->GetUInt("QueryMode");
//	switch(mode)
//	{
//		case 1:
//			SingleQuery=true;
//			RanchQuery=false;
//			break;
//		case 2:
//			SingleQuery=false;
//			RanchQuery=false;
//			break;
//		case 0:
//			SingleQuery=false;
//			RanchQuery=true;
//			break;
//	}
//	PercentValue=Factory->GetBool("PercentValue");
//	PLowerPC=Factory->GetUInt("PLowerPC");
//	PUpperPC=Factory->GetUInt("PUpperPC");
//	PLowerAbs=Factory->GetUInt("PLowerAbs");
//	PUpperAbs=Factory->GetUInt("PUpperAbs");
}


//-----------------------------------------------------------------------------
void GMetaEngineSum::Connect(GSession* session)
{
	// First init
	if(Session) return;
	GMetaEngine::Connect(session);
}


//-----------------------------------------------------------------------------
void GMetaEngineSum::Disconnect(GSession* session)
{

	if(!Session) return;
	GMetaEngine::Disconnect(session);
}

//------------------------------------------------------------------------------
void GMetaEngineSum::QueryEngines(RContainer<RString,false,false>& keyWords)
{
	try
	{
		R::RCursor<GFactoryEngine> engCur=GALILEIApp->GetManager<GEngineManager>("Engine")->GetFactories();
		for(engCur.Start();!engCur.End();engCur.Next())
		{
			if(engCur()->GetPlugin())
			{
				engCur()->GetPlugin()->Process(keyWords);
			}
		}
	}
	catch(GException e)
	{
		cout<<"Error: "<<e.GetMsg()<<endl;
	}
	catch(RException e)
	{
		cout<<"Error: "<<e.GetMsg()<<endl;
	}
	catch(std::bad_alloc e)
	{
		cout<<"Error: bad_alloc"<<endl;
	}
	catch(...)
	{
		cout<<"Error"<<endl;
	}

}


//------------------------------------------------------------------------------
void GMetaEngineSum::Query(RContainer<RString,true,false> &keyWords, bool useAllKwds)
{
	unsigned int n,p,p_up;

	//Init
	R::RCursor<RString> cur;
	QueryWords.Clear();
	//KeyWords.Clear();

	if((!useAllKwds)&&(keyWords.GetNb()>NValue))
	{
		//RStringCursor cur(keyWords);
		cur.Set(keyWords);
		for (cur.Start();!cur.End();cur.Next())
		{
			KeyWords.InsertPtr(new RString(*cur()));
			if(KeyWords.GetNb()>=NValue)
				break;
		}
	}
	else
	{
		KeyWords=keyWords;
	}

	n=KeyWords.GetNb();
	cur.Set(KeyWords);

	//Clear the set of results
	Results.Clear();

	//if no keywords ->don't search
	if(n==0)
		return;

	//Check the working mode of the meta engine
	if(SingleQuery)
	{	//Use a single query containing all the keywords
		for(cur.Start();!cur.End();cur.Next())
		{
			QueryWords.InsertPtr(cur());
			//if((!useAllKwds)&&(QueryWords.NbPtr >=NValue))
			//	break;
		}
		QueryEngines(QueryWords);
	}
	else
		if(RanchQuery)
		{
			//Use a ranch of combinaison to crete the queries
			//Compute P:
			if(PercentValue)
			{
				p=round(static_cast<double>(n)*PLowerPC/100 );
				p_up=round(static_cast<double>(n)*PUpperPC/100 );
			}
			else
			{
				if(PLowerAbs>=n)
					p=n;
				else
					p=PLowerAbs;

				if(PUpperAbs>=n)
					p_up=n;
				else
					p_up=PLowerAbs;
			}
			if(p==0)
				p=1;
			if(p_up==0)
				p_up=1;
			if(p>p_up)
				p_up=p;
			for(/*p*/; p<=p_up; p++)
			{
				CombineQuery(0,p);
			}
		}
		else
		{	//Use a combinaison of the keywords to create the query
			//Compute P:
			if(PercentValue)
			{
				p=round(static_cast<double>(n)*PLowerPC/100);
			}
			else
			{
				if(PLowerAbs>=n)
					p=n;
				else
					p=PLowerAbs;
			}
			if(p==0)
				p=1;
			CombineQuery(0,p);
		}
	KeyWords.Clear();
}

//------------------------------------------------------------------------------
void GMetaEngineSum::CombineQuery(unsigned int pos,unsigned int nbTot)
{
	if(QueryWords.GetNb()==nbTot)
	{
		QueryEngines(QueryWords);
	}
	else
	{
		R::RCursor<RString> cur(KeyWords);
		for(cur.GoTo(pos);!cur.End();cur.Next())
		{
			QueryWords.InsertPtr(cur());
			CombineQuery(pos+1,nbTot);
			QueryWords.DeletePtr(*cur());
			if(KeyWords.GetNb()-pos-1<nbTot-QueryWords.GetNb())
				break;
			pos++;
		}
	}
}


//------------------------------------------------------------------------------
void GMetaEngineSum::Process()
{
	//Find global ranking and sort results
	ComputeGlobalRanking();
	//--------------------------------------

/*	R::RCursor<GEngineDoc>  docCur =GetEngineDocsCursor();
	for(docCur.Start();!docCur.End();docCur.Next())
	{
		cout<<docCur()->GetGlobalRanking()<<" URL : "<<docCur()->GetUrl().Latin1()<<endl;
		R::RCursor<GRanking> ranCur=docCur()->GetRankingsCursor();
		for(ranCur.Start();!ranCur.End();ranCur.Next())
		{
			cout<<ranCur()->GetRank()<<" : "<<ranCur()->GetEngine().Latin1()<<"  ";
		}
	}*/
}


//------------------------------------------------------------------------------
void GMetaEngineSum::AddResult(RString url,RString title, RString desc, int rank,RString engine)
{
	//if url ends with a '/' remove it.
	if(url[url.GetLen()-1]=='/')
	{
		url=url.Mid(0,url.GetLen()-1);
	}

	//Test if URl already in the container
	if(Results.GetPtr(url))
	{
		//add new ranking to the existing GEngineDoc
		Results.GetPtr(url)->AddRanking(rank,engine);
	}
	else
	{
		Results.InsertPtr(new GEngineDoc(url,title,desc,rank,engine));
	}
}


//------------------------------------------------------------------------------
int GMetaEngineSum::sortOrder(const void* a, const void* b)
{
	int ar=(*((GEngineDoc**)(a)))->GetGlobalRanking();
	int br=(*((GEngineDoc**)(b)))->GetGlobalRanking();
	if(ar==br)
		return(0);
	if(ar<br)
		return(1);
	else
		return(-1);
}


//------------------------------------------------------------------------------
void GMetaEngineSum::ComputeGlobalRanking()
{
	GEngineManager* mng=GALILEIApp->GetManager<GEngineManager>("Engine");
	R::RCursor<GEngineDoc> docCur =GetEngineDocs();
	GEngine* eng;
	unsigned int globalRank;
	unsigned int tmp;
	unsigned int weight;
	for(docCur.Start();!docCur.End();docCur.Next())
	{
		globalRank=0;
		R::RCursor<GRanking> rankCur=docCur()->GetRankings();
		for(rankCur.Start();!rankCur.End();rankCur.Next())
		{
			tmp=(1000 - rankCur()->GetRank() +1);
			//check wether results must multiplie by the engine weight
			if(UseWeight)
			{
				eng=mng->GetPlugIn(rankCur()->GetEngine());
				if(eng)
				{
					weight=round(eng->GetEngineWeight()*10);
					tmp*=weight;
				}
			}
			globalRank+= tmp;
		}
		docCur()->SetGlobalRanking(globalRank);
	}

	//Sort results
	if(Results.GetNb())
		Results.ReOrder(sortOrder);
}


//-----------------------------------------------------------------------------
void GMetaEngineSum::Clear(void)
{
	Results.Clear();
}


//-----------------------------------------------------------------------------
R::RCursor<GEngineDoc> GMetaEngineSum::GetEngineDocs(void)
{
	R::RCursor<GEngineDoc> cur(Results);
	return(cur);
}


//------------------------------------------------------------------------------
void GMetaEngineSum::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("UseWeight",false));
	params->InsertParam(new RParamValue("LimitRes",false));
	params->InsertParam(new RParamValue("MaxResNb",100));
	params->InsertParam(new RParamValue("NValue",10));
	params->InsertParam(new RParamValue("QueryMode",1));
	params->InsertParam(new RParamValue("PercentValue",true));
	params->InsertParam(new RParamValue("PLowerPC",50));
	params->InsertParam(new RParamValue("PUpperPC",70));
	params->InsertParam(new RParamValue("PLowerAbs",5));
	params->InsertParam(new RParamValue("PUpperAbs",7));
}


//-----------------------------------------------------------------------------
GMetaEngineSum::~GMetaEngineSum(void)
{
}


//------------------------------------------------------------------------------
CREATE_METAENGINE_FACTORY("Meta engine ranked with sum",GMetaEngineSum)
