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
	bool IFF;                                            // Inverse Frequency Factor
	GLang* Lang;                                         // Language
	double MeanSim;                                      // Mean of similarities
	double Deviation;                                    // Standart deviation of similarities
	unsigned int OldNbComp;                              // Old number of comparisons
	unsigned int* ModifiedProfs;                         // Identificators of modified profiles
	unsigned int NbModified;                             // Number of modified subprofiles

	// Constructor and Compare methods.
	GProfilesSim(GSubProfileCursor& s,bool iff,GLang* lang) throw(bad_alloc);
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GProfilesSim* profilesSim) const {return(Lang->Compare(profilesSim->Lang));}

	// Analyse the similarity of the two subprofiles and insert when necessary.
	void AnalyseSim(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2);

	// Get the similarities between two profiles.
	double GetSim(const GSubProfile* s1,const GSubProfile* s2);

	// Update the state of the profiles sims : If the subprofile has changed
	// the corresponding sim will be set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element
	// is created but not computed ( -> state to osModified )
	void Update(bool iff)throw(bad_alloc);

	// Update the deviation od similarities.
	void UpdateDeviationAndMeanSim(GSubProfileCursor& subprofiles);

	// Add a subprofile to the listof the modified one.
	void AddModifiedProfile(unsigned int id);

	// Destructor.
	virtual ~GProfilesSim(void) {}
};


//------------------------------------------------------------------------------
GProfilesSims::GProfilesSim::GProfilesSim(GSubProfileCursor& s,bool iff,GLang* l) throw(bad_alloc)
	: IFF(iff), Lang(l)
{
	GSubProfileCursor Cur1, Cur2;
	unsigned int i,j, pos;
	GSims* sim;

	//initialize the container of GSims (calculate size)
	for (s.Start(), i=0; !s.End(); s.Next())
		if (s()->GetProfile()->GetId()>i)
			i=s()->GetProfile()->GetId();
	Sims=new RContainer<GSims,unsigned int,true,false>(i+1,1);

	//initialize table of modified subprofiles;
	ModifiedProfs=new unsigned int [s.GetNb()];
	NbModified=0;

	//builds the left inferior triangular matrix.
	Cur1=s;
	Cur2=s;
	if(!Cur1.GetNb()) return;
	for(Cur1.Start(), i=0; !Cur1.End();Cur1.Next(),i++)
	{
		pos=Cur1()->GetProfile()->GetId();
		sim=new GSims(Cur1()->GetProfile()->GetId(),pos-1);
		Sims->InsertPtrAt(sim,pos);
		for(Cur2.Start(), j=0;j<i;Cur2.Next(),j++)
		{
			// insert a null similarity with state osModified
			// it will be thus recomputed the next time it is called.
			sim->InsertPtrAt(new GSim(Cur2()->GetProfile()->GetId(),0.0,osModified), Cur2()->GetProfile()->GetId());
		}
	}

	//init the MeansSim
	MeanSim=Deviation=0.0;
}


//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::AnalyseSim(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2)
{
	double tmp;
	unsigned int pos;

	if(sub1->GetLang()!=sub2->GetLang()) return;
	if((!sub1->IsDefined())||(!sub2->IsDefined())) return;
	if(IFF)
		tmp=sub1->SimilarityIFF(sub2);
	else
		tmp=sub1->Similarity(sub2);
	if(fabs(tmp)<1e-10) return;
	pos=sub2->GetProfile()->GetId();
	sim->InsertPtrAt(new GSim(pos,tmp,osUpdated), pos);

}


//------------------------------------------------------------------------------
double GProfilesSims::GProfilesSim::GetSim(const GSubProfile* sub1,const GSubProfile* sub2)
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
void  GProfilesSims::GProfilesSim::Update(bool iff) throw(bad_alloc)
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

	if(iff!=IFF) // the type of similarity has changed => All the sims values must be updated.
	{
		IFF=iff;
		 for (Sims->Start(); !Sims->End(); Sims->Next())
		 	for ((*Sims)()->Start(); !(*Sims)()->End(); (*Sims)()->Next())
      				(*(*Sims)())()->State=osModified;
	}

	//reset the number of modified subprofiles.
	NbModified=0;
	#warning osDelete to add
	#warning when all the sim between the subprofiles are computed -> set Profile State to osUpdated
}


//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::UpdateDeviationAndMeanSim(GSubProfileCursor& subprofiles)
{
	GSim* sim;
	GSims* sims;
	unsigned int nbcomp,i,j;
	double oldsim,newsim;
	double newmean, oldmean, newdev, olddev;
	GSubProfileCursor subprofiles2;

	oldmean=MeanSim; olddev=Deviation;

	//manage number of comparisons.
	nbcomp=(subprofiles.GetNb())*(subprofiles.GetNb()-1)/2;

	newmean=OldNbComp*oldmean;
	newdev=OldNbComp*(olddev+(oldmean*oldmean));

	subprofiles2=subprofiles;
	for(subprofiles.Start(),i=0,j=subprofiles.GetNb();--j;subprofiles.Next(),i++)
	{
		sims=Sims->GetPtrAt(subprofiles()->GetProfile()->GetId());
		for(subprofiles2.GoTo(i+1);!subprofiles2.End();subprofiles2.Next())
		{
			sim=sims->GetPtrAt(subprofiles2()->GetProfile()->GetId());
			// if the similarity isn't in profsim, continue
			if(!sim) continue;
			//if ths similarity is in "modified" state :
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


//------------------------------------------------------------------------------
void GProfilesSims::GProfilesSim::AddModifiedProfile(unsigned int id)
{
	ModifiedProfs[NbModified]=id;
	NbModified++;
}



//------------------------------------------------------------------------------
//
//  GProfilesSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesSims::GProfilesSims(GSession* session,bool iff) throw(bad_alloc)
	: Sims(10,5), Session(session), IFF(iff)
{
	GFactoryLangCursor Langs;
	GLang* Lang;

	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		Sims.InsertPtr(new GProfilesSim(Session->GetSubProfilesCursor(Lang),IFF,Lang));
	}
}

//------------------------------------------------------------------------------
void GProfilesSims::ReInit(void) throw(bad_alloc)
{
	GFactoryLangCursor Langs;
	GLang* Lang;

	Sims.Clear();
	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Lang=Langs()->GetPlugin();
		if(!Lang) continue;
		Sims.InsertPtr(new GProfilesSim(Session->GetSubProfilesCursor(Lang),IFF,Lang));
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
	profSim->AddModifiedProfile(sub->GetProfile()->GetId());
}


//------------------------------------------------------------------------------
GProfilesSims::~GProfilesSims(void)
{
}
