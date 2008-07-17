/*

	GALILEI Research Project

	GCommunity.cpp

	Group - Implementation.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gcommunity.h>
#include <gdoc.h>
#include <gsubject.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GFdbkRef
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFdbkRef
{
public:
	GFdbk* Doc;
	double Sim;

	GFdbkRef(GFdbk* d,double s) : Doc(d), Sim(s) {}
	int Compare(const GFdbkRef&) const {return(-1);}
	int Compare(const GFdbk* d) const
	{
		return(Doc->GetDocId()-d->GetDocId());
	}
};



//------------------------------------------------------------------------------
//
//  GCommunity
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GCommunity::GCommunity(size_t id,const R::RDate& u,const R::RDate& c)
	: GGroup<GProfile,GCommunity,otCommunity>(id,u,c)
{
}


//------------------------------------------------------------------------------
int GCommunity::sortOrder(const void *a,const void *b)
{
	double af=(*((GFdbkRef**)(a)))->Sim;
	double bf=(*((GFdbkRef**)(b)))->Sim;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
size_t GCommunity::GetNbProfiles(const GSubject* subject) const
{
	size_t tot;
	RCursor<GProfile> sub(*this);
	for(sub.Start(),tot=0;!sub.End();sub.Next())
		if(subject->IsIn(sub()))
			tot++;
	return(tot);
}



//------------------------------------------------------------------------------
void GCommunity::NotJudgedDocsList(RContainer<GFdbk,false,true>& docs, GProfile* prof) const
{
	// Clear container.
	docs.Clear();

	RCursor<GProfile> Prof(*this);
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(Prof()==prof) continue;

		// Go through the assessments of the profile grouped together through this subprofile
		RCursor<GFdbk> Fdbks(Prof()->GetFdbks());
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Verify that it was not assessed by the corresponding profile
			if(prof->GetFdbk(Fdbks()->GetDocId())) continue;

			// Verify if already inserted:
			// If not -> insert it in docs.
			// If yes -> Verify judgement
			GFdbk* ptr=docs.GetPtr<const GFdbk*>(Fdbks());
			if(ptr)
			{
				tDocAssessment j=ptr->GetFdbk();
				switch( j & djMaskJudg)
				{
					case djKO:
						if(Fdbks()->GetFdbk() & djOK)
						{
							docs.DeletePtr(ptr);
							docs.InsertPtr(Fdbks());
						}
						break;
					case djOutScope:
						if((Fdbks()->GetFdbk() & djOK)&&(Fdbks()->GetFdbk() & djKO))
						{
							docs.DeletePtr(ptr);
							docs.InsertPtr(Fdbks());
						}
						break;
				}
			}
			else
				docs.InsertPtr(Fdbks());
		}
	}
}


//------------------------------------------------------------------------------
void GCommunity::NotJudgedDocsRelList(GMeasure* measure,RContainer<GFdbk,false,false>& docs, GProfile* prof,GSession*) const
{
	if(!measure)
		throw GException("No documents/profiles similarities");

	// Clear container.
	docs.Clear();
	RContainer<GFdbkRef,true,false> Docs(50,25);

	// Go through the subprofiles
	RCursor<GProfile> Prof(*this);
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		//If current treated profile is the profile "prof" ->Then only add links docs
		if(Prof()==prof)
		{
			// Go through the assessments of the profile grouped together through this profile
			RCursor<GFdbk> Fdbks(Prof()->GetFdbks());
			for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
			{
				// Verify if the document is a relevant hub or authority.
				tDocAssessment j=Fdbks()->GetFdbk();
				if(!( (j & (djOK & djHub)) || (j & (djOK & djAutority)))) continue;

				// Verify if already inserted in Docs.
				if(Docs.GetPtr<const GFdbk*>(Fdbks())) continue;
				// Insert it.
				double res;
				measure->Measure(0,Fdbks()->GetDocId(),prof->GetId(),&res);
				Docs.InsertPtr(new GFdbkRef(Fdbks(),res));
			}
			continue;
		}

		// Go through the assessments of the profile grouped together through this profile
		RCursor<GFdbk> Fdbks(Prof()->GetFdbks());
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Verify if the document is relevant.
			tDocAssessment j=Fdbks()->GetFdbk();
			if(!(j & djOK)) continue;

			// Verify if already inserted in Docs or if it was not assessed by the
			// corresponding profile.
			if((Docs.GetPtr<const GFdbk*>(Fdbks()))||(prof->GetFdbk(Fdbks()->GetDocId()))) continue;

			// Insert it.
			double res;
			measure->Measure(0,prof->GetId(),Fdbks()->GetDocId(),&res);
			Docs.InsertPtr(new GFdbkRef(Fdbks(),res));
		}
	}

	// Sort the container by similarity
	if(docs.GetNb())
		docs.ReOrder(sortOrder);

	// Copy the result in docs
	RCursor<GFdbkRef> Cur(Docs);
	for(Cur.Start();!Cur.End();Cur.Next())
		docs.InsertPtr(Cur()->Doc);
}


//------------------------------------------------------------------------------
GCommunity::~GCommunity(void)
{
}
