/*

	GALILEI Research Project

	GProfilesBehaviour.cpp

	Similarities between Profiles - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gprofilesbehaviour.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <profiles/gusers.h>
#include <langs/glang.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GBehaviours
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GBehaviours::GBehaviours(unsigned int id,unsigned int max) throw(bad_alloc)
	: RContainer<GBehaviour,unsigned int,true,false>(max,max/2), Id(id)
{
}


//-----------------------------------------------------------------------------
//
// class GProfilesBehaviour
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfilesBehaviour::GProfilesBehaviour(RContainer<GSubProfile,unsigned int,false,true>* s,GLang* l) throw(bad_alloc)
	: Behaviours(s->NbPtr,s->NbPtr<50?50:s->NbPtr/2),Lang(l)
{
	GSubProfileCursor Cur1, Cur2;
	unsigned int i,j, pos;
	GBehaviours* behaviours;

	//initializes table of modified subprofiles;
	ModifiedProfs=new unsigned int [s->NbPtr];
	NbModified=0;

	//builds the left inferior triangular matrix.
	Cur1.Set(s);
	Cur2.Set(s);
	if(!Cur1.GetNb()) return;
	for(Cur1.Start(), i=0; !Cur1.End();Cur1.Next(),i++)
	{
		pos=Cur1()->GetProfile()->GetId();
		behaviours=new GBehaviours(Cur1()->GetProfile()->GetId(),pos-1);
		Behaviours.InsertPtrAt(behaviours,pos);
		for(Cur2.Start(), j=0;j<i;Cur2.Next(),j++)
			 AnalyseBehaviour(behaviours,Cur1(),Cur2());
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfilesBehaviour::AnalyseBehaviour(GBehaviours* behaviours,GSubProfile* sub1,GSubProfile* sub2)
{
	unsigned int pos, nbcommon, nbsame, nbdiff;
	double pcsame, pcdiff;

	pcsame=pcdiff=0.0;
	nbsame=sub1->GetCommonOKDocs(sub2);
	nbdiff=sub1->GetCommonDiffDocs(sub2);
	nbcommon=sub1->GetCommonDocs(sub2);
	if (nbcommon)
	{
		pcdiff=100.0*(nbdiff/nbcommon);
		pcsame=100.0*(nbsame/nbcommon);
	}

	pos=sub2->GetProfile()->GetId();
	behaviours->InsertPtrAt(new GBehaviour(pos,pcsame, pcdiff,osUpdated), pos);
}


//-----------------------------------------------------------------------------
double GALILEI::GProfilesBehaviour::GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2, unsigned int threshold)
{
	GBehaviours* b;
	GBehaviour* b2;
	int i,j,tmp;
	double okratio, diffratio, nbcommon;

	i = sub1->GetProfile()->GetId();
	j = sub2->GetProfile()->GetId();

	if(i==j) return (100.0);
	if (i<j)
	{
		tmp=i;
		i=j;
		j=tmp;
	}
	b=Behaviours.GetPtr<unsigned int>(i);
	if(!b) return(0.0);
	b2=b->GetPtr<unsigned int>(j);
	if(!b2) return(0.0);

	if( (b2->GetState() == osUpdated) || (b2->GetState() == osUpToDate))
		return b2->GetAgreementRatio();

	if(b2->GetState() == osModified)
	{
		okratio=diffratio=0.0;
		b2->SetState(osUpToDate);
		nbcommon=sub1->GetCommonDocs(sub2);
		if(nbcommon>=threshold)
		{
			okratio=100.0*(sub1->GetCommonOKDocs(sub2)/nbcommon);
			diffratio=100.0*(sub1->GetCommonDiffDocs(sub2)/nbcommon);
		}
		 b2->SetAgreementRatio(okratio);
		 b2->SetDisAgreementRatio(diffratio);
 		 return (okratio);
	}

	if (b2->GetState()==osDelete)
		return (0.0);   //-------------------------A MODIFIER

	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GProfilesBehaviour::GetDisAgreementRatio(GSubProfile* sub1,GSubProfile* sub2, unsigned int threshold)
{
	GBehaviours* b;
	GBehaviour* b2;
	int i,j,tmp;
	double diffratio, okratio, nbcommon;

	i = sub1->GetProfile()->GetId();
	j = sub2->GetProfile()->GetId();

	if(i==j) return (0.0);
	if (i<j)
	{
		tmp=i;
		i=j;
		j=tmp;
	}
	b=Behaviours.GetPtr<unsigned int>(i);
	if(!b) return(0.0);
	b2=b->GetPtr<unsigned int>(j);
	if(!b2) return(0.0);

	if( (b2->GetState() == osUpdated) || (b2->GetState() == osUpToDate))
		return b2->GetDisAgreementRatio();

	if(b2->GetState() == osModified)
	{
		cout << "disagreement - prof modified"<<endl;
		okratio=diffratio=0.0;
		b2->SetState(osUpToDate);
		nbcommon=sub1->GetCommonDocs(sub2);
		if(nbcommon>=threshold)
		{
			okratio=100.0*(sub1->GetCommonOKDocs(sub2)/nbcommon);
			diffratio=100.0*(sub1->GetCommonDiffDocs(sub2)/nbcommon);
		}
		 b2->SetAgreementRatio(okratio);
		 b2->SetDisAgreementRatio(diffratio);
 		 return (diffratio);
	}

	if (b2->GetState()==osDelete)
		return (0.0);   //-------------------------A MODIFIER

	return(0.0);
}


//-----------------------------------------------------------------------------
int GALILEI::GProfilesBehaviour::Compare(const GLang* l) const
{
	return(Lang->Compare(l));
}


//-----------------------------------------------------------------------------
int GALILEI::GProfilesBehaviour::Compare(const GProfilesBehaviour& profilesBehaviour) const
{
	return(Lang->Compare(profilesBehaviour.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GProfilesBehaviour::Compare(const GProfilesBehaviour* profilesBehaviour) const
{
	return(Lang->Compare(profilesBehaviour->Lang));
}


//-----------------------------------------------------------------------------
tObjState GALILEI::GProfilesBehaviour::GetState(unsigned int id1, unsigned int id2)
{
	GBehaviours* behaviours;
	GBehaviour* behaviour;
	int tmp;

	if (id1>id2)
	{
		tmp=id1;
		id1=id2;
		id2=tmp;
	}

	behaviours = Behaviours.GetPtr<unsigned int>(id1);
	if (!behaviours) return osUnknow;
	behaviour = behaviours->GetPtr<unsigned int>(id2);
	if (!behaviour) return osUnknow;

	return behaviour->GetState() ;
}



//-----------------------------------------------------------------------------
void GALILEI::GProfilesBehaviour::AddModifiedProfile(unsigned int id)
{
	ModifiedProfs[NbModified]=id;
	NbModified++;
}


//-----------------------------------------------------------------------------
void  GALILEI::GProfilesBehaviour::UpdateProfBehaviour(void) throw(bad_alloc)
{
	unsigned int i;
	GBehaviours* behaviours;

	// change status of modified subprofiles and add Behaviours of created subprofiles
	for (i=NbModified; i; i--)
	{
		behaviours = Behaviours.GetPtrAt(ModifiedProfs[i-1]);
		if(!behaviours)
			Behaviours.InsertPtrAt(behaviours = new GBehaviours(ModifiedProfs[i-1] , ModifiedProfs[i-1]-1), ModifiedProfs[i-1]);
		for (behaviours->Start(); !behaviours->End(); behaviours->Next())
			(*behaviours)()->SetState(osModified);
	}

	//reset the number of modified subprofiles.
	NbModified=0;
	#warning osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}



//-----------------------------------------------------------------------------
GALILEI::GProfilesBehaviour::~GProfilesBehaviour(void)
{
}



