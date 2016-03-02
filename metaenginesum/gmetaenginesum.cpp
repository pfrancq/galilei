/*

	GALILEI Research Project

	GMetaEngineSum.cpp

	Meta-engine based on a Weighted Sum - Implementation.

   Copyright 2003-2015 by Pascal Francq.
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
#include <gdocref.h>
#include <gdocfragmentranks.h>
#include <gdocfragmentrank.h>
#include <gsearchquery.h>



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
	:GMetaEngine(session,fac), QueryWords(10,5)
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
			mThrowGException(RString::Number(type)+" is not a valid query type");
	}
}


//------------------------------------------------------------------------------
void GMetaEngineSum::FragmentRankAdded(GDocFragmentRank* rank,GEngine* engine)
{
	if(UseWeight)
		rank->SetRanking(rank->GetRanking()*engine->GetWeight());
}


//------------------------------------------------------------------------------
void GMetaEngineSum::RequestEngines(GSearchQuery* query,size_t caller)
{
	switch(Type)
	{
		case Single:
			GMetaEngine::RequestEngines(query,caller);
			break;
		case kCombinations:
			StartCombinations(query,caller);
	}
}


//------------------------------------------------------------------------------
void GMetaEngineSum::PostRequest(size_t caller)
{
	if(Debug)
	{
		// Print the results.
		R::RCursor<GDocFragmentRanks> Ranks(GetRankings(caller));
		for(Ranks.Start();!Ranks.End();Ranks.Next())
		{
			cout<<Ranks()->GetFragment()->GetDoc()->GetName()<<endl;
			RCursor<GDocFragmentRank> Cur(Ranks()->GetRankings());
			for(Cur.Start();!Cur.End();Cur.Next())
				cout<<"\t\t"<<Cur()->GetRanking()<<" : "<<Cur()->GetInfo()<<"  ";
		}
	}
}


//------------------------------------------------------------------------------
void GMetaEngineSum::StartCombinations(GSearchQuery* query,size_t caller)
{
	if(!query->UseOnlyAND())
		mThrowGException("Query does not contain only AND operators.");


	// if No keywords -> don't search for this language.
	size_t Nb(query->GetNbTokens());
	if(!Nb)
		return;

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
		CombineKeywords(query,0,mink,caller);
}



//------------------------------------------------------------------------------
void GMetaEngineSum::CombineKeywords(GSearchQuery* query,size_t pos,size_t k,size_t caller)
{
	if(QueryWords.GetNb()==k)
	{
		RString Query;
		Query.Concat(QueryWords,' ');
		GSearchQuery TheQuery(GetSession(),GetExtractor(),caller);
		TheQuery.Build(Query);
		GMetaEngine::RequestEngines(&TheQuery,caller);
	}
	else
	{
		R::RCursor<RString> cur(query->GetTokens());
		for(cur.GoTo(pos);!cur.End();cur.Next())
		{
			QueryWords.InsertPtr(cur());
			CombineKeywords(query,pos+1,k,caller);
			QueryWords.DeletePtr(*cur());
			if(query->GetNbTokens()-pos-1<k-QueryWords.GetNb())
				break;
			pos++;
		}
	}
}


//------------------------------------------------------------------------------
void GMetaEngineSum::ComputeGlobalRanking(size_t caller)
{
	// Go trough each document fragment retrieved
	RCursor<GDocFragmentRanks> Ranks(GetRankings(caller));
	for(Ranks.Start();!Ranks.End();Ranks.Next())
	{
		double Rank(0.0);
		R::RCursor<GDocFragmentRank> Cur(Ranks()->GetRankings());
		for(Cur.Start();!Cur.End();Cur.Next())
			Rank+=Cur()->GetRanking();
		Ranks()->GetFragment()->SetRanking(Rank);
	}
}


//------------------------------------------------------------------------------
int GMetaEngineSum::sortOrder(const void* a, const void* b)
{
	double ar=(*((GDocFragment**)(a)))->GetRanking();
	double br=(*((GDocFragment**)(b)))->GetRanking();
	if(ar>br)
		return(-1);
	else if(ar<br)
		return(1);
	return(0);
}



//------------------------------------------------------------------------------
CREATE_METAENGINE_FACTORY("MetaEngineSum","Weighted Sum Meta-Engine",GMetaEngineSum)
