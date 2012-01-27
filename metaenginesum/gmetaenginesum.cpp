/*

	GALILEI Research Project

	GMetaEngineSum.cpp

	Meta-engine based on a Weighted Sum - Implementation.

   Copyright 2003-2012 by Pascal Francq.
	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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



//-----------------------------------------------------------------------------
// DEBUG Mode
const bool Debug=false;



//------------------------------------------------------------------------------
//
// class GMetaEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngineSum::GMetaEngineSum(GSession* session,GPlugInFactory* fac)
	:GMetaEngine(session,fac), Keywords(10,5), QueryWords(10,5)
{
}


//------------------------------------------------------------------------------
void GMetaEngineSum::CreateConfig(void)
{
	InsertParam(new RParamValue("UseWeight",false));
	InsertParam(new RParamValue("Type",Single));
	InsertParam(new RParamValue("PercentValue",true));
	InsertParam(new RParamValue("kMin",50));
	InsertParam(new RParamValue("kMax",70));
}


//-----------------------------------------------------------------------------
void GMetaEngineSum::ApplyConfig(void)
{
	PercentValue=FindParam<RParamValue>("PercentValue")->GetBool();
	UseWeight=FindParam<RParamValue>("UseWeight")->GetBool();
	kMin=FindParam<RParamValue>("kMin")->GetUInt();
	kMax=FindParam<RParamValue>("kMax")->GetUInt();
	size_t type(FindParam<RParamValue>("Type")->GetUInt());
	switch(type)
	{
		case Single:
			Type=Single;
			break;
		case kCombinations:
			Type=kCombinations;
			break;
		default:
			ThrowGException(RString::Number(type)+" is not a valid query type");
	}
}

//------------------------------------------------------------------------------
void GMetaEngineSum::AddResult(size_t docid,const RString desc,double ranking,const GEngine* engine)
{
	if(UseWeight)
		ranking*=engine->GetWeight();
	GMetaEngine::AddResult(docid,desc,ranking,engine);
}


//------------------------------------------------------------------------------
void GMetaEngineSum::AddResult(const RString& uri,const RString& title,const RString desc,double ranking,const GEngine* engine)
{
	if(UseWeight)
		ranking*=engine->GetWeight();
	GMetaEngine::AddResult(uri,title,desc,ranking,engine);

}


//------------------------------------------------------------------------------
void GMetaEngineSum::Request(const R::RString query)
{
	// Initialise
	Query=query;
	Results.Clear();
	Keywords.Clear();
	query.Split(Keywords,' ');

	//if No keywords -> don't search.
	if(!Keywords.GetNb())
		return;

	// Does the work.
	BuildRequests();
	ComputeGlobalRanking();
	if(Debug)
	{
		// Print the results.
		R::RCursor<GDocRetrieved> Doc(GetDocs());
		for(Doc.Start();!Doc.End();Doc.Next())
		{
			cout<<Doc()->GetRanking()<<" URL : "<<Doc()->GetURI()<<endl;
			RCursor<GDocRanking> Cur(Doc()->GetRankings());
			for(Cur.Start();!Cur.End();Cur.Next())
				cout<<"  "<<Cur()->GetRanking()<<" : "<<Cur()->GetInfo()<<"  ";
		}
	}
}

//------------------------------------------------------------------------------
void GMetaEngineSum::BuildRequests(void)
{
	size_t Nb(Keywords.GetNb());

	switch(Type)
	{
		case Single:
		{
			RequestEngines(Query);
			break;
		}
		case kCombinations:
		{
			size_t mink,maxk;
			if(PercentValue)
			{
				mink=round(static_cast<double>(Nb)*kMin/100 );
				maxk=round(static_cast<double>(Nb)*kMax/100 );
			}
			else
			{
				if(kMin>Nb)
					mink=Nb;
				else
					mink=kMin;

				if(kMax>Nb)
					maxk=Nb;
				else
					maxk=kMax;
			}
			if(mink==0)
				mink=1;
			if(maxk==0)
				maxk=1;
			if(mink>maxk)
				maxk=mink;
			for(;mink<=maxk;mink++)
				CombineKeywords(0,mink);
			break;
		}
	}
}

//------------------------------------------------------------------------------
void GMetaEngineSum::CombineKeywords(size_t pos,size_t k)
{
	if(QueryWords.GetNb()==k)
	{
		RString Query;
		Query.Concat(QueryWords,' ');
		RequestEngines(Query);
	}
	else
	{
		R::RCursor<RString> cur(Keywords);
		for(cur.GoTo(pos);!cur.End();cur.Next())
		{
			QueryWords.InsertPtr(cur());
			CombineKeywords(pos+1,k);
			QueryWords.DeletePtr(*cur());
			if(Keywords.GetNb()-pos-1<k-QueryWords.GetNb())
				break;
			pos++;
		}
	}
}


//------------------------------------------------------------------------------
void GMetaEngineSum::RequestEngines(const R::RString& query)
{
	try
	{
		RCastCursor<GPlugIn,GEngine> Cur(GALILEIApp->GetPlugIns<GEngine>("Engine"));
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Request(this,query);
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
void GMetaEngineSum::ComputeGlobalRanking(void)
{
	// Go trough each document retrieved
	RCursor<GDocRetrieved> Doc(GetDocs());
	for(Doc.Start();!Doc.End();Doc.Next())
	{
		double Rank(0.0);
		R::RCursor<GDocRanking> Cur(Doc()->GetRankings());
		for(Cur.Start();!Cur.End();Cur.Next())
			Rank+=Cur()->GetRanking();
		SetRanking(Doc(),Rank);
	}

	// Sort results
	if(Results.GetNb())
		Results.ReOrder(sortOrder);
}


//------------------------------------------------------------------------------
int GMetaEngineSum::sortOrder(const void* a, const void* b)
{
	int ar=(*((GDocRetrieved**)(a)))->GetRanking();
	int br=(*((GDocRetrieved**)(b)))->GetRanking();
	if(ar==br)
		return(0);
	if(ar>br)
		return(1);
	else
		return(-1);
}



//------------------------------------------------------------------------------
CREATE_METAENGINE_FACTORY("MetaEngineSum","Weighted Sum Meta-Engine",GMetaEngineSum)
