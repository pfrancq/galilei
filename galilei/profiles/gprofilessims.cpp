/*

	GALILEI Research Project

	GSubProfiles.cpp

	List of SubProfiles for a given Language - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gprofilessims.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GSim
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing a similarity with a subprofile.
class GSim
{
public:
	unsigned int SubId;      // identifier of the second profile
	double Sim;              // Similarity between the profiles.
	tObjState State;         // State of the similarity.


	GSim(unsigned int id,double s,tObjState state = osUpToDate) : SubId(id), Sim(s),State(state)  {}
	int Compare(const GSim* s) const {return(SubId-s->SubId);}
	int Compare(const unsigned int id) const {return(SubId-id);}
};



//------------------------------------------------------------------------------
//
// class GSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities for a given subprofile.
class GSims : public RContainer<GSim,unsigned int,true,false>
{
public:
	unsigned int SubId;         // Identifier of the first profile

	GSims(unsigned int id,unsigned int max) throw(bad_alloc)
		: RContainer<GSim,unsigned int,true,false>(max,max/2), SubId(id) {}
	int Compare(const GSims* s) const {return(SubId-s->SubId);}
	int Compare(const unsigned int id) const {return(SubId-id);}
};



//------------------------------------------------------------------------------
//
// class GProfilesSims::GProfilesSim
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities between subprofiles of a given
// language.
class GProfilesSims::GProfilesSim
{
public:

	R::RContainer<GSims,unsigned int,true,false>* Sims;  // Similarities
	bool IFF;                                             // Inverse Frequency Factor
	GLang* Lang;                                          // Language
	double MeanSim;                                       // Mean of similarities
	double Deviation;                                     // Standart deviation of similarities
	unsigned int OldNbComp;                               // Old number of comparisons
	GProfilesSims* Manager;                               //manger of the gprofsim
	// Identificators of modified profiles
	RContainer<GSubProfile,unsigned int,false,true>* ModifiedProfs;

	// Constructor and Compare methods.
	GProfilesSim(GProfilesSims* manager,GSubProfileCursor& s,bool iff,GLang* lang) throw(bad_alloc, GException);
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GProfilesSim* profilesSim) const {return(Lang->Compare(profilesSim->Lang));}

	// compute the similarity betwwen tow subprofiles
	void AnalyseSim(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2) throw (GException);

	// Get the similarities between two profiles.
	double GetSim(const GSubProfile* s1,const GSubProfile* s2);

	// Update the state of the profiles sims : If the subprofile has changed
	// the corresponding sim will be set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element
	// is created but not computed ( -> state to osModified )
	void Update(void) throw(bad_alloc);

	// Set all the sims to osModified if the iff bolean has changed.
	void Update(bool iff)throw(bad_alloc);

	// add a new GSims fo a new subprofile subprof
	GSims* AddNewSims(GSubProfile* subprof);


	// Update the deviation od similarities.
	void UpdateDeviationAndMeanSim(GSubProfileCursor& subprofiles) throw (GException); // general function
	void UpdateDevMeanSim(GSubProfileCursor& subprofiles) throw (GException); // memory on
	void RecomputeDevMeanSim(GSubProfileCursor& subprofiles) throw (GException); //memory off

	// Add a subprofile to the listof the modified one.
	void AddModifiedProfile(GSubProfile* s);

	// Destructor.
	virtual ~GProfilesSim(void);
};


//------------------------------------------------------------------------------
GProfilesSims::GProfilesSim::GProfilesSim(GProfilesSims* manager, GSubProfileCursor& s,bool iff,GLang* l) throw(bad_alloc, GException)
	:  IFF(iff),Lang(l),Manager(manager)
{
	GSubProfileCursor Cur1, Cur2;
	unsigned int i,j, pos, nbcomp;
	double simssum, deviation, tmpsim;
	GSims* sim;
	GSubProfileCursor s2;

	// if memory is false, we don't stock a container of similarities.
	// sims will be re-calculated eacht time.
	if (!Manager->GetMemory())
		return;

	//initialize the container of GSims (calculate size)
	for (s.Start(), i=0; !s.End(); s.Next())
		if (s()->GetProfile()->GetId()>i)
			i=s()->GetProfile()->GetId();
	Sims=new RContainer<GSims,unsigned int,true,false>(i+1,1);

	//initialize table of modified subprofiles;
	ModifiedProfs=new RContainer<GSubProfile,unsigned int,false,true>(5,1);

	//builds the left inferior triangular matrix.
	Cur1=s;
	Cur2=s;
	if(!Cur1.GetNb()) return;
	for(Cur1.Start(), i=0; !Cur1.End();Cur1.Next(),i++)
	{
		if ((!Cur1()->IsDefined())||(Cur1()->GetProfile()->GetId()==1)) continue;
		pos=Cur1()->GetProfile()->GetId();
		sim=new GSims(pos,pos);
		Sims->InsertPtrAt(sim,pos);
		for(Cur2.Start(), j=0;j<i;Cur2.Next(),j++)
		{
			if (!Cur2()->IsDefined()) continue;
		 	AnalyseSim(sim,Cur1(),Cur2());
		}
	}


	//mean calculation & deviation calculation
	simssum=deviation=0.0;
	nbcomp=0;
	s2=s;
	for(s.Start(),i=0,j=s.GetNb();--j;s.Next(),i++)
	{
		if (!s()->IsDefined()) continue;
		for(s2.GoTo(i+1);!s2.End();s2.Next())
		{
			if (!s2()->IsDefined()) continue;
			tmpsim=GetSim(s(),s2());
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
	if(Deviation<0)
		throw("Negative Deviation in creating profiles similarities !");
}


//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::AnalyseSim(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2) throw (GException)
{
	double tmp;
	unsigned int pos;

	if(IFF)
		tmp=sub1->SimilarityIFF(sub2);
	else
		tmp=sub1->Similarity(sub2);
	if(fabs(tmp)<Manager->GetNullSimLevel()) return;
	pos=sub2->GetProfile()->GetId();
	if (pos >=sub1->GetProfile()->GetId())
		throw (GException("Out of Range in Analyse similarities !"));
	sim->InsertPtrAt(new GSim(pos,tmp,osUpdated), pos);

}

//------------------------------------------------------------------------------
double GProfilesSims::GProfilesSim::GetSim(const GSubProfile* sub1,const GSubProfile* sub2)
{
	GSims* s;
	GSim* s2;
	int i,j,tmp;

	//if memory is false, re-calculate similarity
	if (!Manager->GetMemory())
	{
		if (IFF)
			return (sub1->SimilarityIFF(sub2));
		else
			 return (sub1->Similarity(sub2));
	}

	// get the position of the subprofiles in the double container
	i = sub1->GetProfile()->GetId();
	j = sub2->GetProfile()->GetId();

	if(i==j) return (1.0);
	// the first indice i must be the highest one
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
		return(s2->Sim);

	if(s2->State == osModified)
	{
		s2->State = osUpToDate ;
		if (IFF)
		{
			s2->Sim=sub1->SimilarityIFF(sub2);
			return (s2->Sim);
		}
		else
		{
			 s2->Sim=sub1->Similarity(sub2);
			 return (s2->Sim);
		}
	}
	if (s2->State == osDelete)  return(0.0);   //-------------------------A MODIFIER

	return(0.0);
}


//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::Update(void) throw(bad_alloc)
{
	GSims* sims;

	// if memory is false, no update is needed
	// since sims are claulctaed each time
	if(!Manager->GetMemory())
		return;

	// change status of modified subprofiles and add sims of created subprofiles
	for (ModifiedProfs->Start(); !ModifiedProfs->End(); ModifiedProfs->Next())
	{
		if(!(*ModifiedProfs)()->IsDefined()) continue;
		sims = Sims->GetPtrAt((*ModifiedProfs)()->GetProfile()->GetId());
		if(!sims)
			sims=AddNewSims((*ModifiedProfs)());
		for (sims->Start(); !sims->End(); sims->Next())
			(*sims)()->State=osModified;
	}

	//reset the number of modified subprofiles.
	ModifiedProfs->Clear();
	#warning osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}


//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::Update(bool iff) throw(bad_alloc)
{
	// the type of similarity has changed => All the sims values must be updated.
	if(iff!=IFF)
	{
		IFF=iff;
		// if memory is false, no update is needed
		// since sims are calculated each time
		if(!Manager->GetMemory())
			return;
		 for (Sims->Start(); !Sims->End(); Sims->Next())
		 	for ((*Sims)()->Start(); !(*Sims)()->End(); (*Sims)()->Next())
				(*(*Sims)())()->State=osModified;
	}
}


//------------------------------------------------------------------------------
GSims*  GProfilesSims::GProfilesSim::AddNewSims(GSubProfile* sub)
{
	GSims* sims, *tmpsims;
	GSubProfileCursor subcur;

	if (!sub->IsDefined()) return 0;
	sims=new GSims(sub->GetProfile()->GetId(),sub->GetProfile()->GetId());
	Sims->InsertPtrAt(sims, sub->GetProfile()->GetId());
        subcur=Manager->GetSession()->GetSubProfilesCursor(Lang);

	for (subcur.Start(); !subcur.End(); subcur.Next())
	{
		if (subcur()->GetProfile()->GetId()<sub->GetProfile()->GetId())
		{
			AnalyseSim(sims, sub,subcur());
		}
		if (subcur()->GetProfile()->GetId()>sub->GetProfile()->GetId())
		{
			tmpsims=Sims->GetPtrAt(subcur()->GetProfile()->GetId());
			if(tmpsims)
				AnalyseSim(tmpsims, subcur(), sub);
		}
	}
	return sims;
}


//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::UpdateDeviationAndMeanSim(GSubProfileCursor& subprofiles) throw (GException)
{
	if (Manager->GetMemory())
		UpdateDevMeanSim(subprofiles);
	else
		RecomputeDevMeanSim(subprofiles);
}

//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::UpdateDevMeanSim(GSubProfileCursor& subprofiles) throw (GException)
{
	GSim* sim;
	GSims* sims;
	unsigned int nbcomp; //number of comparison
	unsigned i,j;
	double oldsim,newsim;
	double newmean, oldmean, newdev, olddev;
	GSubProfileCursor subprofiles2;

	nbcomp=0;
	oldmean=MeanSim; olddev=Deviation;
	newmean=OldNbComp*oldmean;
	newdev=OldNbComp*(olddev+(oldmean*oldmean));
	subprofiles2=subprofiles;

	for(subprofiles.Start(),i=0;!subprofiles.End();subprofiles.Next(),i++)
	{
		if(!(subprofiles)()->IsDefined()) continue;
		nbcomp++;
		if(subprofiles()->GetProfile()->GetId()==1) continue;
		sims=Sims->GetPtrAt(subprofiles()->GetProfile()->GetId());
		if (!sims)
			throw(GException("Sims not present in Update Mean similarities"));
		for(subprofiles2.Start(),j=0;(j<i)&&(!subprofiles2.End());subprofiles2.Next(),j++)
		{
			if(!(subprofiles2)()->IsDefined()) continue;
			sim=sims->GetPtrAt(subprofiles2()->GetProfile()->GetId());
			// if the similarity isn't in profsim, continue
			if(!sim) continue;
			// if the similarity is in "modified" state :
			if (sim->State==osModified)
			{
				oldsim=sim->Sim;
				newsim=GetSim(subprofiles(),subprofiles2());
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
	nbcomp=nbcomp*(nbcomp-1)/2;
	newmean/=nbcomp;
	newdev/=nbcomp;
	newdev-=(newmean*newmean);

	//re-affect thhe global parameters
	MeanSim=newmean;
	Deviation=newdev;
	if (fabs(Deviation)<0.00001)
		Deviation=0.0;
	if (Deviation <0.0)
	{
		cout <<"Negative Deviation in profiles similarities : "<< Deviation<<endl;
		throw(GException("Negative Deviation in profiles similarities"));
	}
	OldNbComp=nbcomp;
}


//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::RecomputeDevMeanSim(GSubProfileCursor& subprofiles) throw (GException)
{
	unsigned int i,j, pos, nbcomp;
	double simssum, deviation, tmpsim;
	GSubProfileCursor s2;

	simssum=deviation=0.0;
	nbcomp=0;
	s2=subprofiles;

	for(subprofiles.Start(),i=0,j=subprofiles.GetNb();--j;subprofiles.Next(),i++)
	{
		if (!subprofiles()->IsDefined()) continue;
		for(s2.GoTo(i+1);!s2.End();s2.Next())
		{
			if (!s2()->IsDefined()) continue;
			tmpsim=GetSim(subprofiles(),s2());
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
	}
	else
		MeanSim=Deviation=0.0;
	if (Deviation <0.0)
		throw(GException("Negative Deviation in profiles similarities"));
}

//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::AddModifiedProfile(GSubProfile* s)
{
	if (Manager->GetMemory())
		ModifiedProfs->InsertPtr(s);
}


//------------------------------------------------------------------------------
GProfilesSims::GProfilesSim::~GProfilesSim(void)
{
	if (Sims)
		delete Sims;
	if(ModifiedProfs)
		delete ModifiedProfs;
}


//------------------------------------------------------------------------------
//
//  GProfilesSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesSims::GProfilesSims(GSession* session,bool iff, bool memory) throw(bad_alloc)
	: Sims(10,5), Session(session), IFF(iff), Memory(memory),
	  NullSimLevel(session->GetSessionParams()->GetDouble("NullSimLevel"))
{
	GFactoryLangCursor Langs;
	GLang* Lang;


	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		Sims.InsertPtr(new GProfilesSim(this, Session->GetSubProfilesCursor(Lang),IFF,Lang));
	}
}

//------------------------------------------------------------------------------
void GProfilesSims::ReInit(void) throw(bad_alloc)
{
	GFactoryLangCursor Langs;
	GLang* Lang;

	if (!GetMemory()) return;
	Sims.Clear();
	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		Sims.InsertPtr(new GProfilesSim(this, Session->GetSubProfilesCursor(Lang),IFF,Lang));
	}
}


//------------------------------------------------------------------------------
void GProfilesSims::UseIFF(bool iff) throw(bad_alloc)
{
	GFactoryLangCursor Langs;
	GLang* Lang;

	IFF=iff;
	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		GProfilesSim* ProfSim = Sims.GetPtr<GLang*>(Lang);
		ProfSim->Update(iff);
	}
}


//------------------------------------------------------------------------------
void GProfilesSims::Update(void)
{
	for (Sims.Start(); !Sims.End(); Sims.Next())
		Sims()->Update();
}

//------------------------------------------------------------------------------
double GProfilesSims::GetSim(const GSubProfile* sub1,const GSubProfile* sub2) throw(GException)
{
	if(sub1->GetLang()!=sub2->GetLang())
		throw GException("Cannot compare two subprofiles of a different language");
	GProfilesSim* ProfSim = Sims.GetPtr<GLang*>(sub1->GetLang());
	return(ProfSim->GetSim(sub1,sub2));
}


//------------------------------------------------------------------------------
double GProfilesSims::GetMinimumOfSimilarity(GLang* lang, double deviationrate) throw(GException)
{
	double minSim;

	GProfilesSim* profSim = Sims.GetPtr<const GLang*>(lang);
	if(!profSim)
		throw GException("Language not defined");
	profSim->UpdateDeviationAndMeanSim(Session->GetSubProfilesCursor(lang));
	minSim=profSim->MeanSim+deviationrate*sqrt(profSim->Deviation);

	return(minSim);
}


//------------------------------------------------------------------------------
void GProfilesSims::AddModifiedProfile(GSubProfile* sub) throw(bad_alloc,GException)
{
	GProfilesSim* profSim;

	profSim = Sims.GetPtr<GLang*>(sub->GetLang());
	if(!profSim)
		throw GException("Language not defined");
	profSim->AddModifiedProfile(sub);
}


//------------------------------------------------------------------------------
GProfilesSims::~GProfilesSims(void)
{
}
