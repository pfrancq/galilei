/*

	GALILEI Research Project

	GQuery.cpp

	Find the group most relevant with a query .Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
// include file for Galilei Project
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <langs/glang.h>
#include <langs/gdict.h>
#include <docs/gdoc.h>
#include <sessions/gsession.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <profiles/gprofile.h>
#include <groups/ggroupvector.h>
#include <groups/ggroupcalcgravitation.h>
#include <tests/gsimdocquerygroup.h>
#include <tests/gquery.h>
using namespace GALILEI;

//-----------------------------------------------------------------------------
// Include file for R Project
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>



//-----------------------------------------------------------------------------
//
// class GQuery
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GQuery::GQuery(GIWordsWeights* query,GSession* session,GLang* lang,double minsim,bool modify)
	:QueryWord(query),ses(session),Lang(lang),MinSim(minsim),Modify(modify)
{
	ses->LoadIdealGroupmentInGroups();
	Groups=new RContainer<GGroup,unsigned int,false,false>(20,10);
	CalcSimGroup();
}

//-----------------------------------------------------------------------------
void GALILEI::GQuery::CalcSimGroup()
{

	GGroupCalcGravitation Gravitation(ses);
	Gravitation.SetMaxNonZero(500);
	if(Modify) QueryWord->ModifyQueryGroups(otDoc,Lang);
	GGroupsCursor idealgroup=ses->GetGroupsCursor();

	for(idealgroup.Start();!idealgroup.End();idealgroup.Next())
	{
		GGroupCursor Group;
		Group=idealgroup()->GetGroupCursor();
		bool endtest=false;

		while(!endtest)
		{
			double minsim=0;
			for(Group.Start();!Group.End();Group.Next())
			{
				GGroup* Grp=Group();
				Gravitation.Compute(Grp);
				GGroupVector* GrpV=static_cast<GGroupVector*>(Group());
				bool end=true;
				const char* name;
				for(Grp->Start();end;Grp->Next())
				{
					name=(*Grp)()->GetProfile()->GetName();
					end=false;
				}
				double simidf=QueryWord->Similarity(GrpV->GetVector());
				if((simidf>minsim)&&(!Groups->IsIn(GrpV)))
				{
					minsim=simidf;
				}
			}
			if(minsim<=MinSim) endtest=true;
			if(!endtest)
			{
				for(Group.Start();!Group.End();Group.Next())
				{
					GGroup* Grp=Group();
					GGroupVector* GrpV=static_cast<GGroupVector*>(Group());
					double simidf=QueryWord->Similarity(GrpV->GetVector());
					if(simidf==minsim) Groups->InsertPtr(Grp);
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GQuery::ResStart(void)
{
	Groups->Start(); 
}


//-----------------------------------------------------------------------------
void GALILEI::GQuery::ResNext(void)
{
	Groups->Next();  
}


//-----------------------------------------------------------------------------
bool GALILEI::GQuery:: ResEnd(void)
{
	return(Groups->End()); 
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GQuery::GetCurrentGroup(void)
{
	return((*Groups)());
}


//-----------------------------------------------------------------------------
double GALILEI::GQuery::GetCurrentSim(void)
{
	GGroupCalcGravitation Gravitation(ses);
	Gravitation.SetMaxNonZero(500);
	GGroup* Grp=(*Groups)();
	Gravitation.Compute(Grp);
	GGroupVector* GrpV=static_cast<GGroupVector*>(Grp);
	return(QueryWord->Similarity(GrpV->GetVector()));
}


//-----------------------------------------------------------------------------
GALILEI::GQuery::~GQuery(void)
{
}
