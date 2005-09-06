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
#include <gprofilesagree.h>
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
	double AgreementRatio;     // Agreement ratio.
	tObjState State;         // State of the similarity.


	GSim(unsigned int id,double agree,tObjState state = osUpToDate)
		: SubId(id), AgreementRatio(agree), State(state)
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
	void Compute(const GSubProfile* sub1,const GSubProfile* sub2,double& agree) const;

	GSim* GetRatio(const GSubProfile* sub1,const GSubProfile* sub2) const;

	// Get the similarities between two profiles.
	double GetAgreementRatio(const GSubProfile* s1,const GSubProfile* s2) const;

	// Update the state of the profiles sims : If the subprofile has changed
	// the corresponding sim will be set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element
	// is created but not computed ( -> state to osModified )
	void Update(void) throw(std::bad_alloc);

	// add a new GSims fo a new subprofile subprof
	GSims* AddNewSims(GSubProfile* subprof);

	// Add a subprofile to the listof the modified one.
	void AddModifiedProfile(const GSubProfile* s);

	// Destructor.
	virtual ~GProfilesSim(void);
};


//------------------------------------------------------------------------------
GProfilesSimsCosinus::GProfilesSim::GProfilesSim(GProfilesSimsCosinus* manager,GLang* lang)
	:  Sims(0), Lang(lang), Manager(manager), ModifiedProfs(0)
{

	RCursor<GSubProfile> Cur1, Cur2;
	unsigned int i,pos;
	GSims* sim;
	RCursor<GSubProfile> s2;

	//initialize table of modified subprofiles;
	ModifiedProfs=new RContainer<GSubProfile,false,true>(5000,5000);

	// if memory is false, we don't stock a container of similarities.
	// sims will be re-calculated eacht time.
	if((!Manager->Memory)||(!Manager->Session))
		return;

	//initialize the container of GSims (calculate size)
	RCursor<GSubProfile> s(Manager->Session->GetSubProfiles(Lang));
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
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::GProfilesSim::Compute(const GSubProfile* sub1,const GSubProfile* sub2,double& agree) const
{
	agree=Manager->ComputeAgree(sub1,sub2);
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::GProfilesSim::Analyse(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2,bool isnull)
{
	double pcsame;
	unsigned int pos;

	pos=sub2->GetProfile()->GetId();
	if(pos>=sub1->GetProfile()->GetId())
		throw (GException("Out of Range in Analyse similarities !"));
	Compute(sub1,sub2,pcsame);
	if(isnull)
		sim->InsertPtrAt(new GSim(pos,pcsame,osUpdated),pos);
	else
		if(pcsame!=0.0)
			sim->InsertPtrAt(new GSim(pos,pcsame,osUpdated), pos);
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
		double pcsame;

		Compute(sub1,sub2,pcsame);
		s2->State = osUpToDate ;
		s2->AgreementRatio=pcsame;
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
		RCursor<GSubProfile> s(Manager->Session->GetSubProfiles(Lang));
		for(s.Start(),i=0; !s.End(); s.Next())
			if(s()->GetProfile()->GetId()>i)
				i=s()->GetProfile()->GetId();
		Sims=new RContainer<GSims,true,false>(i+1);
	}

	subscur2=Manager->Session->GetSubProfiles(Lang);
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
	}

	RCursor<GSims> Cur(*Sims);
	for (Cur.Start();!Cur.End();Cur.Next())
	{
		RCursor<GSim> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			Cur2()->State=osModified;
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
	subcur=Manager->Session->GetSubProfiles(Lang);
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
GProfilesSimsCosinus::GProfilesSimsCosinus(GFactoryMeasure* fac)
	: GMeasure(fac), GSignalHandler(), Sims(10,5), Memory(true),
	  NullSimLevel(0.000001), MinSameDocs(7), Debug(false),
	  NeedUpdate(false)
{
	GSession::AddHandler(this);
	if(!Memory) return;
	R::RCursor<GLang> Langs(GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIns());
	for(Langs.Start();!Langs.End();Langs.Next())
		Sims.InsertPtr(new GProfilesSim(this,Langs()));
}


//-----------------------------------------------------------------------------
void GProfilesSimsCosinus::ApplyConfig(void)
{
	MinSameDocs=Factory->GetUInt("MinSameDocs");
		NullSimLevel=Factory->GetDouble("NullSimLevel");
	Memory=Factory->GetBool("Memory");
	Debug=Factory->GetBool("Debug");
	MinAgreement=Factory->GetDouble("MinAgreement");
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
double GProfilesSimsCosinus::GetMeasure(unsigned int id1,unsigned int id2,unsigned int)
{
	GSubProfile* sub1=Session->GetSubProfile(id1,0);
	GSubProfile* sub2=Session->GetSubProfile(id2,0);

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
double GProfilesSimsCosinus::GetMinMeasure(const GLang*,unsigned int)
{
	return(MinAgreement);
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
	params->InsertPtr(new GParamDouble("NullSimLevel",0.00001));
	params->InsertPtr(new GParamBool("Memory",true));
	params->InsertPtr(new GParamBool("Debug",false));
	params->InsertPtr(new GParamDouble("MinAgreement",0.6));
}


//------------------------------------------------------------------------------
GProfilesSimsCosinus::~GProfilesSimsCosinus(void)
{
	GSession::DeleteHandler(this);
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles Agreements","Count Method",GProfilesSimsCosinus)
