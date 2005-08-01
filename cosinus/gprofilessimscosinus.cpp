/*

	GALILEI Research Project

	GSubProfiles.cpp

	List of SubProfiles for a given Language - Implementation.

	Copyright 2003-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

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
// include standard api files
#include <math.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gprofilessimscosinus.h>
#include <gsubprofile.h>
#include <gprofile.h>
#include <glang.h>
#include <gsession.h>
#include <gpluginmanagers.h>
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
	double AgreementRatio;     // Agreement ratio.
	double DisagreementRatio;  // Disagreement ratio.
	tObjState State;         // State of the similarity.


	GSim(unsigned int id,double s,double agree,double disagree, tObjState state = osUpToDate)
		: SubId(id), Sim(s), AgreementRatio(agree), DisagreementRatio(disagree), State(state)
		  {
		  }
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
class GSims : public RContainer<GSim,true,false>
{
public:
	unsigned int SubId;         // Identifier of the first profile

	GSims(unsigned int id,unsigned int max)
		: RContainer<GSim,true,false>(max,max/2), SubId(id) {}
	int Compare(const GSims* s) const {return(SubId-s->SubId);}
	int Compare(const unsigned int id) const {return(SubId-id);}
};



//------------------------------------------------------------------------------
//
// class GProfilesSimsCosinus::GProfilesSim
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities between subprofiles of a given
// language.
class GProfilesSimsCosinus::GProfilesSim
{
public:

	R::RContainer<GSims,true,false>* Sims;  // Similarities
	GLang* Lang;                                          // Language
	double MeanSim;                                       // Mean of similarities
	double Deviation;                                     // Standart deviation of similarities
	unsigned int OldNbComp;                               // Old number of comparisons
	GProfilesSimsCosinus* Manager;                               //manger of the gprofsim
	// Identificators of modified (and defined) )profiles
	RContainer<GSubProfile,false,true>* ModifiedProfs;

	// Constructor and Compare methods.
	GProfilesSim(GProfilesSimsCosinus* manager,GLang* lang);
	int Compare(const GLang& l) const {return(Lang->Compare(l));}
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GProfilesSim& profilesSim) const {return(Lang->Compare(*profilesSim.Lang));}

	// compute the similarity betwwen tow subprofiles
	// param nullsim: if true, the sim is set to 0.0 (but we know that the sim is not null and has to exist!)
	void Analyse(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2,bool isnull=false);
	void Compute(const GSubProfile* sub1,const GSubProfile* sub2,double& sim,double& agree,double& disagree) const;

	GSim* GetRatio(const GSubProfile* sub1,const GSubProfile* sub2) const;

	// Get the similarities between two profiles.
	double GetSim(const GSubProfile* s1,const GSubProfile* s2) const;
	double GetAgreementRatio(const GSubProfile* s1,const GSubProfile* s2) const;
	double GetDisagreementRatio(const GSubProfile* s1,const GSubProfile* s2) const;

	// Update the state of the profiles sims : If the subprofile has changed
	// the corresponding sim will be set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element
	// is created but not computed ( -> state to osModified )
	void Update(void) throw(std::bad_alloc);

	// Set all the sims to osModified if the iff bolean has changed.
	void Update(bool iff)throw(std::bad_alloc);

	// add a new GSims fo a new subprofile subprof
	GSims* AddNewSims(GSubProfile* subprof);


	// Update the deviation od similarities.
	double GetMinSimilarity(void);

	// Add a subprofile to the listof the modified one.
	void AddModifiedProfile(const GSubProfile* s);

	// Destructor.
	virtual ~GProfilesSim(void);
};


//------------------------------------------------------------------------------
GProfilesSimsCosinus::GProfilesSim::GProfilesSim(GProfilesSimsCosinus* manager,GLang* lang)
	:  Sims(0), Lang(lang), MeanSim(0.0), Deviation(0.0), OldNbComp(0), Manager(manager), ModifiedProfs(0)
{

	RCursor<GSubProfile> Cur1, Cur2;
	unsigned int i,j, pos, nbcomp;
	double simssum, deviation, tmpsim;
	GSims* sim;
	RCursor<GSubProfile> s2;

	//initialize table of modified subprofiles;
	ModifiedProfs=new RContainer<GSubProfile,false,true>(5000,5000);

	// if memory is false, we don't stock a container of similarities.
	// sims will be re-calculated eacht time.
	if((!Manager->Memory)||(!Manager->Session))
		return;

	//initialize the container of GSims (calculate size)
	RCursor<GSubProfile> s(Manager->Session->GetSubProfilesCursor(Lang));
	for(s.Start(),i=0; !s.End(); s.Next())
		if(s()->GetProfile()->GetId()>i)
			i=s()->GetProfile()->GetId();
	Sims=new RContainer<GSims,true,false>(i+1);

	//builds the left inferior triangular matrix.
	Cur1=s;
	Cur2=s;
	if(!Cur1.GetNb()) return;
	for(Cur1.Start(), i=0; !Cur1.End();Cur1.Next(),i++)
	{
		// if the profile is the first one -> no need to create sim
		if (Cur1()->GetProfile()->GetId()==1) continue;
		pos=Cur1()->GetProfile()->GetId();
		sim=new GSims(pos,pos);
		Sims->InsertPtrAt(sim,pos);
		//if the sub is not defined, no need to analyse sim (but the simS must be created!!)
		if (!Cur1()->IsDefined()) continue;
		for(Cur2.Start(); !Cur2.End(); Cur2.Next())
		{
			if (!Cur2()->IsDefined()) continue;
			if (Cur2()->GetId()==Cur1()->GetId()) continue;
			if(Cur1()->GetProfile()->GetId()>Cur2()->GetProfile()->GetId())
		 		Analyse(sim,Cur1(),Cur2());
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
	{
		char tmp[250];
		sprintf(tmp,"Negative Deviation (%f) in creating profiles similarities !",Deviation);
		throw GException(tmp);
	}
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::GProfilesSim::Compute(const GSubProfile* sub1,const GSubProfile* sub2,double& sim,double& agree,double& disagree) const
{
	sim=Manager->ComputeSim(sub1,sub2);
	disagree=Manager->ComputeDisagree(sub1,sub2);
	agree=Manager->ComputeAgree(sub1,sub2);
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::GProfilesSim::Analyse(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2,bool isnull)
{
	double tmp, pcsame, pcdiff;
	unsigned int pos;

	pos=sub2->GetProfile()->GetId();
	if(pos>=sub1->GetProfile()->GetId())
		throw (GException("Out of Range in Analyse similarities !"));
	Compute(sub1,sub2,tmp,pcsame,pcdiff);
	if(isnull)
		sim->InsertPtrAt(new GSim(pos,0.0,pcsame,pcdiff,osUpdated),pos);
	else
		if((tmp!=0.0)||(pcsame!=0.0)||(pcdiff!=0.0))
			sim->InsertPtrAt(new GSim(pos,tmp,pcsame,pcdiff,osUpdated), pos);
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GProfilesSim::GetSim(const GSubProfile* sub1,const GSubProfile* sub2) const
{

	if(sub1->GetId()==sub2->GetId())
		return (1.0);
	GSim* s2=GetRatio(sub1,sub2);
	if(s2)
		return(s2->Sim);
	return(0.0);
}


//------------------------------------------------------------------------------
GSim* GProfilesSimsCosinus::GProfilesSim::GetRatio(const GSubProfile* sub1,const GSubProfile* sub2) const
{
	unsigned int i,j,tmp;

	// get the position of the subprofiles in the double container
	i = sub1->GetProfile()->GetId();
	j = sub2->GetProfile()->GetId();

	// the first indice i must be the highest one
	if (i<j)
	{
		tmp=i;
		i=j;
		j=tmp;
	}

	if(Sims->GetMaxPos()<i)
		return(0);
	GSims* s=(*Sims)[i];
	if(!s) return(0);
	if(s->GetMaxPos()<j)
		return(0);
	GSim* s2=(*s)[j];
	if(!s2) return(0);

	if( (s2->State == osUpdated) || (s2->State == osUpToDate))
		return(s2);

	if(s2->State == osModified)
	{
		double pcsame, pcdiff, tmp;

		Compute(sub1,sub2,tmp,pcsame,pcdiff);
		s2->State = osUpToDate ;
		s2->Sim=tmp;
		s2->AgreementRatio=pcsame;
		s2->DisagreementRatio=pcdiff;
		return(s2);
	}

	if (s2->State == osDelete)  return(0);   //-------------------------A MODIFIER


	return(0);
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GProfilesSim::GetAgreementRatio(const GSubProfile* sub1,const GSubProfile* sub2) const
{
	GSim* b;

	if(sub1->GetId()==sub2->GetId()) return(1.0);
	b=GetRatio(sub1,sub2);
	if(!b) return(0.0);
	return(b->AgreementRatio);
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GProfilesSim::GetDisagreementRatio(const GSubProfile* sub1,const GSubProfile* sub2) const
{
	GSim* b;

	if(sub1->GetId()==sub2->GetId()) return(0.0);
	b=GetRatio(sub1,sub2);
	if(!b) return(0.0);
	return(b->DisagreementRatio);
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::GProfilesSim::Update(void) throw(std::bad_alloc)
{
	GSims* sims;
	GSim* sim;
	RCursor<GSubProfile> subscur;
	RCursor<GSubProfile> subscur2;
	subscur.Set(*ModifiedProfs);

	// if memory is false, no update is needed
	// since sims are claulctaed each time
	if((!Manager->Memory)||(!Manager->Session))
		return;

	//initialize the container of GSims (calculate size)
	if(!Sims)
	{
		unsigned int i;
		RCursor<GSubProfile> s(Manager->Session->GetSubProfilesCursor(Lang));
		for(s.Start(),i=0; !s.End(); s.Next())
			if(s()->GetProfile()->GetId()>i)
				i=s()->GetProfile()->GetId();
		Sims=new RContainer<GSims,true,false>(i+1);
	}

	subscur2=Manager->Session->GetSubProfilesCursor(Lang);
	if(!subscur.GetNb()) return;

	// change status of modified subprofiles and add sims of created subprofiles
	for(subscur.Start(); !subscur.End(); subscur.Next())
	{
		if(!subscur()->IsDefined())
			continue;
		if(Sims->GetMaxPos()<subscur()->GetProfile()->GetId())
			sims=AddNewSims(subscur());
		else
		{
			sims = (*Sims)[subscur()->GetProfile()->GetId()];
			if(!sims)
				sims=AddNewSims(subscur());
			else
			{
				for(subscur2.Start(); !subscur2.End(); subscur2.Next())
				{
					if(!subscur2()->IsDefined()) continue;
					//take only less ID
					if(!(subscur()->GetProfile()->GetId()>subscur2()->GetProfile()->GetId())) continue;
					if(sims->GetMaxPos()<subscur2()->GetProfile()->GetId())
						Analyse(sims, subscur(), subscur2(),true);
					else
					{
						sim=(*sims)[subscur2()->GetProfile()->GetId()];
						if(!sim)
							Analyse(sims, subscur(), subscur2(),true);
					}
				}
			}
		}
		if(Manager->ISF) continue; //if IFF all the sims will be set to osModified later.
		RCursor<GSim> Cur2(*sims);
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			Cur2()->State=osModified;
	}

	if(Manager->ISF) //if IFF all sims must be set to osModified since at least one sub is modified.
	{
		RCursor<GSims> Cur(*Sims);
		for (Cur.Start();!Cur.End();Cur.Next())
		{
			RCursor<GSim> Cur2(*Cur());
			for(Cur2.Start();!Cur2.End();Cur2.Next())
				Cur2()->State=osModified;
		}
	}

	//reset the number of modified subprofiles.
	ModifiedProfs->Clear();
}


//------------------------------------------------------------------------------
GSims* GProfilesSimsCosinus::GProfilesSim::AddNewSims(GSubProfile* sub)
{
	GSims* sims, *tmpsims;
	RCursor<GSubProfile> subcur;

	if((!sub->IsDefined())||(!Manager->Session))
		return(0);
	sims=new GSims(sub->GetProfile()->GetId(),sub->GetProfile()->GetId());
	Sims->InsertPtrAt(sims, sub->GetProfile()->GetId());
	subcur=Manager->Session->GetSubProfilesCursor(Lang);
	for (subcur.Start(); !subcur.End(); subcur.Next())
	{
		if (subcur()->GetProfile()->GetId()<sub->GetProfile()->GetId())
		{
			// if the sim exists, it hs to be set to 0.0 not to influence deviation!
			Analyse(sims, sub,subcur(), true);
		}
		if (subcur()->GetProfile()->GetId()>sub->GetProfile()->GetId())
		{
			if(Sims->GetMaxPos()<subcur()->GetProfile()->GetId())
				continue;
			tmpsims=(*Sims)[subcur()->GetProfile()->GetId()];
			if(tmpsims)
			{
				// if the sim exists, it hs to be set to 0.0 not to influene devaition!
				Analyse(tmpsims, subcur(), sub,true);
			}
		}
	}
	return sims;
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GProfilesSim::GetMinSimilarity(void)
{
	GSim* sim;
	GSims* sims;
	unsigned int nbcomp; //number of comparison
	unsigned i,j;
	double oldsim,newsim;
	double newmean, oldmean, newdev, olddev;
	RCursor<GSubProfile> subprofiles2;
	double deviationrate=1.5;

	nbcomp=0;
	oldmean=MeanSim; olddev=Deviation;
	newmean=OldNbComp*oldmean;
	newdev=OldNbComp*(olddev+(oldmean*oldmean));

	RCursor<GSubProfile> subprofiles(Manager->Session->GetSubProfilesCursor(Lang));
	subprofiles2=subprofiles;

	for(subprofiles.Start(),i=0;!subprofiles.End();subprofiles.Next(),i++)
	{
		if(!(subprofiles)()->IsDefined()) continue;
		nbcomp++;
		if(subprofiles()->GetProfile()->GetId()==1) continue;
		sims=(*Sims)[subprofiles()->GetProfile()->GetId()];
		if (!sims)
			throw(GException("Sims not present in Update Mean similarities"));
		for(subprofiles2.Start(),j=0;(j<i)&&(!subprofiles2.End());subprofiles2.Next(),j++)
		{
			if(!(subprofiles2)()->IsDefined()) continue;
			if(sims->GetMaxPos()<subprofiles2()->GetProfile()->GetId())
				continue;
			sim=(*sims)[subprofiles2()->GetProfile()->GetId()];
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
	if(Deviation<0)
	{
		char tmp[250];
		sprintf(tmp,"Negative Deviation (%f) in creating profiles similarities !",Deviation);
		throw GException(tmp);
	}
	OldNbComp=nbcomp;

	return(MeanSim+deviationrate*sqrt(Deviation));
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::GProfilesSim::AddModifiedProfile(const GSubProfile* s)
{
	if(Manager->Memory)
	{
		if(!ModifiedProfs->GetPtr(s))
			ModifiedProfs->InsertPtr(s);
	}
}


//------------------------------------------------------------------------------
GProfilesSimsCosinus::GProfilesSim::~GProfilesSim(void)
{
	if (Sims)
		delete Sims;
	if(ModifiedProfs)
		delete ModifiedProfs;
}



//------------------------------------------------------------------------------
//
//  GProfilesSimsCosinus
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesSimsCosinus::GProfilesSimsCosinus(GFactoryProfilesSims* fac)
	: GProfilesSims(fac), GSignalHandler(), Sims(10,5), IDF(true), ISF(true), Memory(true),
	  NullSimLevel(0.000001), MinSameDocs(7), MinDiffDocs(4), Debug(false),
	  DebugMinSim(false), MinSim(0.5), AutomaticMinSim(true), NeedUpdate(false)
{
	GSession::AddHandler(this);
	if(!Memory) return;
	R::RCursor<GLang> Langs((dynamic_cast<GLangManager*>(GPluginManagers::PluginManagers.GetManager("Lang")))->GetPlugIns());
	for(Langs.Start();!Langs.End();Langs.Next())
		Sims.InsertPtr(new GProfilesSim(this,Langs()));
}


//-----------------------------------------------------------------------------
void GProfilesSimsCosinus::ApplyConfig(void)
{
	MinSameDocs=Factory->GetUInt("MinSameDocs");
	MinDiffDocs=Factory->GetUInt("MinDiffDocs");
	NullSimLevel=Factory->GetDouble("NullSimLevel");
	ISF=Factory->GetBool("ISF");
	IDF=Factory->GetBool("IDF");
	Memory=Factory->GetBool("Memory");
	Debug=Factory->GetBool("DebugSim");
	DebugMinSim=Factory->GetBool("DebugMinSim");
	MinSim=Factory->GetDouble("MinSim");
	MinAgreement=Factory->GetDouble("MinAgreement");
	MinDisagreement=Factory->GetDouble("MinDisagreement");
	AutomaticMinSim=Factory->GetBool("AutomaticMinSim");
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::Update(void)
{
	RCursor<GProfilesSim> Cur(Sims);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Update();
	NeedUpdate=false;
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::ComputeSim(const GSubProfile* sub1,const GSubProfile* sub2) const
{
	double sim;

	if((!ISF)&&(!IDF))
		sim=sub1->Similarity(*sub2);
	else
	{
		if(ISF)
		{
			if(IDF)
				sim=sub1->SimilarityIFF2(*sub2,otDoc,otSubProfile,sub1->GetLang());
			else
				sim=sub1->SimilarityIFF(*sub2,otSubProfile,sub1->GetLang());
		}
		else
			sim=sub1->SimilarityIFF(*sub2,otDoc,sub1->GetLang());
	}
	if(fabs(sim)<NullSimLevel)
		sim=0.0;
	return(sim);
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetSimilarity(const GSubProfile* sub1,const GSubProfile* sub2)
{
	if(sub1->GetLang()!=sub2->GetLang())
		throw GException("Cannot compare two subprofiles of a different language");

	//if memory is false, re-calculate similarity
	if(!Memory)
		return(ComputeSim(sub1,sub2));

	if(NeedUpdate)
		Update();
	GProfilesSim* ProfSim = Sims.GetPtr<GLang*>(sub1->GetLang());
	return(ProfSim->GetSim(sub1,sub2));
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::ComputeMinSim(const GLang* lang)
{
	unsigned int i,j, nbcomp;
	double simssum, deviation, tmpsim,Deviation,MeanSim;
	RCursor<GSubProfile> s2;
	double deviationrate=1.5;

	simssum=deviation=0.0;
	nbcomp=0;

	RCursor<GSubProfile> subprofiles(Session->GetSubProfilesCursor(lang));
	s2=subprofiles;
	for(subprofiles.Start(),i=0,j=subprofiles.GetNb();--j;subprofiles.Next(),i++)
	{
		if (!subprofiles()->IsDefined()) continue;
		for(s2.GoTo(i+1);!s2.End();s2.Next())
		{
			if (!s2()->IsDefined()) continue;
			tmpsim=GetSimilarity(subprofiles(),s2());
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

	return(MeanSim+deviationrate*sqrt(Deviation));
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetMinSimilarity(const GLang* lang)
{
	if(!AutomaticMinSim)
		return(MinSim);

	if(!Memory)
		return(ComputeMinSim(lang));

	if(NeedUpdate)
		Update();
	GProfilesSim* profSim = Sims.GetPtr<const GLang*>(lang);
	if(!profSim)
		throw GException("Language not defined");
	return(profSim->GetMinSimilarity());
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::ComputeAgree(const GSubProfile* sub1,const GSubProfile* sub2) const
{
	double agree=0.0;
	unsigned int nbsame;
	double nbcommon;

	nbsame=sub1->GetCommonOKDocs(sub2);
	nbcommon=double(sub1->GetCommonDocs(sub2));
	if(nbcommon>=MinSameDocs)
		agree=nbsame/nbcommon;
	if(fabs(agree)<NullSimLevel)
		agree=0.0;
	return(agree);
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetAgreementRatio(const GSubProfile* sub1,const GSubProfile* sub2)
{
	if(sub1->GetLang()!=sub2->GetLang())
		throw GException("Cannot compare two subprofiles of a different language");

	//if memory is false, re-calculate
	if(!Memory)
		return(ComputeAgree(sub1,sub2));

	if(NeedUpdate)
		Update();
	GProfilesSim* ProfSim = Sims.GetPtr<const GLang*>(sub1->GetLang());
	return(ProfSim->GetAgreementRatio(sub1,sub2));
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetMinAgreementRatio(const GLang*)
{
	return(MinAgreement);
}

//------------------------------------------------------------------------------
double GProfilesSimsCosinus::ComputeDisagree(const GSubProfile* sub1,const GSubProfile* sub2) const
{
	unsigned int nbdiff;
	double nbcommon;
	double disagree=0.0;

	nbdiff=sub1->GetCommonDiffDocs(sub2);
	nbcommon=double(sub1->GetCommonDocs(sub2));
	if(nbcommon>=MinDiffDocs)
		disagree=nbdiff/nbcommon;
	if(fabs(disagree)<NullSimLevel)
		disagree=0.0;
	return(disagree);
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetDisagreementRatio(const GSubProfile* sub1,const GSubProfile* sub2)
{
	if(sub1->GetLang()!=sub2->GetLang())
		throw GException("Cannot compare two subprofiles of a different language");

	//if memory is false, re-calculate
	if(!Memory)
		return(ComputeDisagree(sub1,sub2));

	if(NeedUpdate)
		Update();
	GProfilesSim* ProfSim = Sims.GetPtr<const GLang*>(sub1->GetLang());
	return(ProfSim->GetDisagreementRatio(sub1,sub2));
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetMinDisagreementRatio(const GLang*)
{
	return(MinDisagreement);
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::Event(GLang* lang, tEvent event)
{
	if(!Memory) return;
	switch(event)
	{
		case eObjCreated:
			Sims.InsertPtr(new GProfilesSim(this,lang));
			break;
		case eObjDeleted:
			Sims.DeletePtr(*lang);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::Event(GSubProfile* sub, tEvent event)
{
	if(!Memory) return;
	GProfilesSim* profSim;
	RCursor<GSims> Sub;

	switch(event)
	{
		case eObjCreated:
		case eObjModified:
			profSim=Sims.GetPtr<GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			profSim->AddModifiedProfile(sub);
			NeedUpdate=true;
			break;
		case eObjDeleted:
			profSim = Sims.GetPtr<const GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			if(profSim->Sims)
			{
				Sub.Set(*profSim->Sims);
				for(Sub.Start();!Sub.End();Sub.Next())
				{
					if(Sub()->SubId>sub->GetProfile()->GetId())
						continue;
					if(sub->GetProfile()->GetId()<Sub()->GetMaxPos())
						Sub()->DeletePtrAt(sub->GetProfile()->GetId(),false);
				}
				if(sub->GetProfile()->GetId()<profSim->Sims->GetMaxPos())
					profSim->Sims->DeletePtrAt(sub->GetProfile()->GetId(),false);
			}
			profSim->ModifiedProfs->DeletePtr(sub);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamUInt("MinSameDocs",7));
	params->InsertPtr(new GParamUInt("MinDiffDocs",4));
	params->InsertPtr(new GParamDouble("NullSimLevel",0.00001));
	params->InsertPtr(new GParamBool("Memory",true));
	params->InsertPtr(new GParamBool("ISF",true));
	params->InsertPtr(new GParamBool("IDF",true));
	params->InsertPtr(new GParamBool("DebugSim",false));
	params->InsertPtr(new GParamBool("DebugMinSim",false));
	params->InsertPtr(new GParamDouble("MinSim",0.05));
	params->InsertPtr(new GParamDouble("MinAgreement",0.6));
	params->InsertPtr(new GParamDouble("MinDisagreement",0.6));
	params->InsertPtr(new GParamBool("AutomaticMinSim",true));
}


//------------------------------------------------------------------------------
GProfilesSimsCosinus::~GProfilesSimsCosinus(void)
{
	GSession::DeleteHandler(this);
}


//------------------------------------------------------------------------------
CREATE_PROFILESSIMS_FACTORY("Cosinus Method",GProfilesSimsCosinus)
