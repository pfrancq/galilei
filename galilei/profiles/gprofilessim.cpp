/*

	GALILEI Research Project

	GProfilesSim.cpp

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
// include files for GALILEI
#include <profiles/gprofilessim.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <infos/glang.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GSim
{
public:
	unsigned int Id;         // identifier of the second profile
	double Sim;              // Similarity between the profiles.
	tObjState State;         //  State of the similarity.


	GSim(unsigned int id,double s,tObjState state = osUpToDate) : Id(id), Sim(s),State(state)  {}
	int Compare(const GSim* s) const {return(Id-s->Id);}
	int Compare(const GSim& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
	void changeValue(double sim){Sim = sim;}
	
};



//-----------------------------------------------------------------------------
//
// class GSims
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GProfilesSim::GSims : public RContainer<GSim,unsigned int,true,false>
{
public:
	unsigned int Id;         // Identifier of the first profile

	GSims(unsigned int id,unsigned int max) throw(bad_alloc);
	int Compare(const GSims* s) const {return(Id-s->Id);}
	int Compare(const GSims& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
};


//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::GSims::GSims(unsigned int id,unsigned int max) throw(bad_alloc)
	: RContainer<GSim,unsigned int,true,false>(max,max/2), Id(id)
{
}


//-----------------------------------------------------------------------------
//
// class GProfilesSim
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::GProfilesSim(RContainer<GSubProfile,unsigned int,false,true>* s,bool global,GLang* l) throw(bad_alloc)
	: GlobalSim(global), Lang(l)
{
	GSubProfileCursor Cur1, Cur2;
	unsigned int i,j, pos;
	GSims* sim;

	//initialize the container of GSims (calculate size)
	for (s->Start(), i=0; !s->End(); s->Next())
		if ((*s)()->GetProfile()->GetId()>i)
			i=(*s)()->GetProfile()->GetId();
	Sims=new RContainer<GSims,unsigned int,true,false>(i+1,1);

	//initialize table of modified subprofiles;
	ModifiedProfs=new unsigned int [s->NbPtr];
	NbModified=0;

	//builds the left inferior triangular matrix.
	Cur1.Set(s);
	Cur2.Set(s);
	if(!Cur1.GetNb()) return;
	for(Cur1.Start(), i=0; !Cur1.End();Cur1.Next(),i++)
	{
		pos=Cur1()->GetProfile()->GetId();
		sim=new GSims(Cur1()->GetProfile()->GetId(),pos-1);
		Sims->InsertPtrAt(sim,pos);
		for(Cur2.Start(), j=0;j<i;Cur2.Next(),j++)
		{
			 AnalyseSim(sim,Cur1(),Cur2());
		}
	}

	//mean calculation & deviation calculation
	double simssum, deviation, tmpsim;;
	simssum=deviation=0.0;
	unsigned int nbcomp;
	nbcomp=0;
	GSubProfile **sub1, **sub2;

	for (sub1=s->Tab, i=s->NbPtr; i--; sub1++)
	{
		if (!(*sub1)->IsDefined()) continue;
		for (sub2=sub1+1, j=0; j<i; sub2++,j++)
		{
			 if (!(*sub2)->IsDefined()) continue;
			 tmpsim=GetSim((*sub1),(*sub2));
			simssum+=tmpsim;
			deviation+=tmpsim*tmpsim;
			nbcomp++;
		}
	}
	if (nbcomp)
	{
		MeanSim=simssum/double(nbcomp);
		deviation/=double(nbcomp);
		deviation-=MeanSim*MeanSim;
		Deviation=deviation;
		OldNbComp=nbcomp;
	}
	else
		MeanSim=Deviation=0.0;
}


//-----------------------------------------------------------------------------
void GALILEI::GProfilesSim::AnalyseSim(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2)
{
	double tmp;
	unsigned int pos;

	if(GlobalSim)
		tmp=sub1->SimilarityIFF(sub2);
	else
		tmp=sub1->Similarity(sub2);
//	if(fabs(tmp)<1e-10) return;
	pos=sub2->GetProfile()->GetId();
	sim->InsertPtrAt(new GSim(pos,tmp,osUpdated), pos);

}



//-----------------------------------------------------------------------------
double GALILEI::GProfilesSim::GetSim(const GSubProfile* sub1,const GSubProfile* sub2)
{
	GSims* s;
	GSim* s2;
	int i,j,tmp;

	// get the position of the subprofiles in the double container
	i = sub1->GetProfile()->GetId();
	j = sub2->GetProfile()->GetId();

	if(i==j) return (1.0);
	if (i<j)
	{
		tmp=i;
		i=j;
		j=tmp;
	}
	s=Sims->GetPtrAt(i);
	if(!s) return(0.0);
	s2=s->GetPtrAt(j);
	if(!s2) return(0.0);

	if( (s2->State == osUpdated) || (s2->State == osUpToDate))
		return s2->Sim;

	if(s2->State == osModified)
	{
		s2->State = osUpToDate ;
		if (GlobalSim)
		{
			s2->Sim=sub1->SimilarityIFF(sub2);
			return ( s2->Sim);
		}
		else
		{
			 s2->Sim=sub1->Similarity(sub2);
			 return ( s2->Sim);
		}
	}
	if (s2->State == osDelete)  return (0.0);   //-------------------------A MODIFIER

	return(0.0);
}


//-----------------------------------------------------------------------------
int GALILEI::GProfilesSim::Compare(const GLang* l) const
{
	return(Lang->Compare(l));
}


//-----------------------------------------------------------------------------
int GALILEI::GProfilesSim::Compare(const GProfilesSim& profilesSim) const
{
	return(Lang->Compare(profilesSim.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GProfilesSim::Compare(const GProfilesSim* profilesSim) const
{
	return(Lang->Compare(profilesSim->Lang));
}


//-----------------------------------------------------------------------------
tObjState GALILEI::GProfilesSim::GetState(unsigned int id1, unsigned int id2)
{
	GSims* sims;
	GSim* sim;
	int tmp;
	
	if (id1<id2)
	{
		tmp=id1;
		id1=id2;
		id2=tmp;
	}
	
	sims = Sims->GetPtr<unsigned int>(id1);
	if (!sims) return osUnknow;
	sim = sims->GetPtr<unsigned int>(id2);
	if (!sim) return osUnknow;

	return sim->State ;
	
}

//-----------------------------------------------------------------------------
void  GALILEI::GProfilesSim::UpdateProfSim(bool global)throw(bad_alloc)
{
	unsigned int i;
	GSims* sims;

	// change status of modified subprofiles and add sims of created subprofiles
	for (i=NbModified; i; i--)
	{
		sims = Sims->GetPtrAt(ModifiedProfs[i-1]);
		if(!sims)
			Sims->InsertPtrAt(sims = new GSims(ModifiedProfs[i-1] , ModifiedProfs[i-1]-1), ModifiedProfs[i-1]);
		for (sims->Start(); !sims->End(); sims->Next())
			(*sims)()->State=osModified;
	}

	if(global!=GlobalSim) // the type of similarity has changed => All the sims values must be updated.
	{
		 for (Sims->Start(); !Sims->End(); Sims->Next())
		 	for ((*Sims)()->Start(); !(*Sims)()->End(); (*Sims)()->Next())
      				(*(*Sims)())()->State=osModified;
	}

	//reset the number of modified subprofiles.
	NbModified=0;
	#warning osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}


//-----------------------------------------------------------------------------
void GALILEI::GProfilesSim::UpdateDeviationAndMeanSim(RContainer<GSubProfile,unsigned int,false,true>* subprofiles)
{
	GSim* sim;
	GSims* sims;
	unsigned int nbcomp,i;
	double oldsim,newsim;
	GSubProfile** sub1, **sub2;
	double newmean, oldmean, newdev, olddev;

	oldmean=MeanSim; olddev=Deviation;

	//manage number of comparisons.
	nbcomp=(subprofiles->NbPtr)*(subprofiles->NbPtr-1)/2;

	newmean=OldNbComp*oldmean;
	newdev=OldNbComp*(olddev+(oldmean*oldmean));
	
	for (sub1=subprofiles->Tab, i=subprofiles->NbPtr; i--; sub1++)
	{
		sims=Sims->GetPtrAt((*sub1)->GetProfile()->GetId());
		for (sub2=subprofiles->Tab; sub2<sub1; sub2++)
		{
			sim=sims->GetPtrAt((*sub2)->GetProfile()->GetId());
			// if the similarity isn't in profsim, continue
			if(!sim) continue;
			//if ths similarity is in "modified" state :
			if (sim->State==osModified)
			{
				oldsim=sim->Sim;
				newsim=GetSim((*sub1),(*sub2));
				// deviation is calculated as follow : {sum (xi"2/n)} - mean"2
				//removing the modified element
				newdev-=(oldsim*oldsim);
				//add the new value of the modified element
				newdev+=(newsim*newsim);
				//update the mean of similarities
				newmean=newmean-oldsim+newsim;
			}
		}
	}
	newmean/=nbcomp;
	newdev/=nbcomp;
	newdev-=(newmean*newmean);

	//re-affect thhe global parameters
	MeanSim=newmean;
	Deviation=newdev;
	if (Deviation <0)
		cout << " ************ WARNING ! BUG in Deviation !!!!!!!!!!!! ************"<<endl;
	OldNbComp=nbcomp;
}


//-----------------------------------------------------------------------------
void GALILEI::GProfilesSim::AddModifiedProfile(unsigned int id)
{
	ModifiedProfs[NbModified]=id;
	NbModified++;
}

//-----------------------------------------------------------------------------
GALILEI::GProfilesSim::~GProfilesSim(void)
{

}
