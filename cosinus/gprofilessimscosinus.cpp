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
// include files for R
#include <rvectorint.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gprofilessimscosinus.h>
#include <gsubprofile.h>
#include <gprofile.h>
#include <glang.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;



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
	double** Values;
	GLang* Lang;                                          // Language
	double Deviation;                                     // Standart deviation of similarities
	GProfilesSimsCosinus* Manager;                               //manger of the gprofsim
	size_t NbLines;
	bool NeedUpdate;
	RVectorInt<true> Created;
	RVectorInt<true> Modified;
	RVectorInt<true> Deleted;
	double MeanSim;

	// Constructor and Compare methods.
	GProfilesSim(GProfilesSimsCosinus* manager,GLang* lang);
	int Compare(const GLang& l) const {return(Lang->Compare(l));}
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GProfilesSim& profilesSim) const {return(Lang->Compare(*profilesSim.Lang));}

	double Compute(GSubProfile* sub1,GSubProfile* sub2);
	void Update(void);
	void UpdateDeviation(size_t oldnbcomp,double oldsim,double newsim,int what);

	// Destructor.
	virtual ~GProfilesSim(void);
};


//------------------------------------------------------------------------------
GProfilesSimsCosinus::GProfilesSim::GProfilesSim(GProfilesSimsCosinus* manager,GLang* lang)
	:  Values(0), Lang(lang), Deviation(0.0), Manager(manager), NbLines(0), NeedUpdate(false), Created(300), Modified(300), Deleted(300), MeanSim(0.0)
{
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::GProfilesSim::UpdateDeviation(size_t oldnbcomp,double oldsim,double newsim,int what)
{
	size_t newnbcomp;
	double olddev, oldmean, newdev, newmean;
	olddev=Deviation;
	oldmean=MeanSim;

	//comptute new mean
	newnbcomp=oldnbcomp+what;
	newmean=(oldnbcomp*oldmean-oldsim+newsim)/(newnbcomp);
	//update deviation
	newdev=(olddev+(oldmean*oldmean))*oldnbcomp;
	newdev+=-(oldsim*oldsim)+(newsim*newsim);
	newdev/=newnbcomp;
	newdev-=(newmean*newmean);

	//set global value
	MeanSim=newmean;
	Deviation=newdev;
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GProfilesSim::Compute(GSubProfile* sub1,GSubProfile* sub2)
{
	return(sub1->SimilarityIFF2(*sub2,otDoc,otSubProfile,Lang));
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::GProfilesSim::Update(void)
{
	// if memory is false, no update is needed
	// since sims are claulctaed each time
	if(!Manager->Session)
		return;

	// Add the lines (if necessary)
	size_t MaxLines=Manager->Session->GetMaxProfileId()-1;
	size_t NbComp=(NbLines*(NbLines-1))/2;
	if(NbLines<MaxLines)
	{
		double** tmp=new double*[MaxLines];
		if(Values)
		{
			memcpy(tmp,Values,NbLines*sizeof(double*));
			delete[] Values;
		}
		Values=tmp;
		memset(&tmp[NbLines],0,(MaxLines-NbLines)*sizeof(double*));
		NbLines=MaxLines;
	}

	// Go thourgh all lines/cols (line=profileid+2);
	size_t i;
	double** cur=Values;
	for(i=0;i<NbLines;i++,cur++)
	{
		// Verify if this line must be deleted?
		if((Deleted.IsIn(i+2))&&(*cur))
		{
			// Go trough the cols
			size_t j;
			double* vals;
			for(j=0,vals=(*cur);j<i+1;j++,vals++)
			{
				if(Manager->AutomaticMinSim)
					UpdateDeviation(NbComp,(*vals),0,-1);
				NbComp--;
			}
			delete[] (*cur);
			(*cur)=0;
			continue;
		}

		GProfile* prof1=Manager->Session->GetProfile(i+2,false);
		if(!prof1)
			continue;
		GSubProfile* sub1=prof1->GetSubProfile(Lang);
		if(!sub1)
			continue;

		// Verify if all this line is modified
		if((Modified.IsIn(i+2))||(Created.IsIn(i+2)))
		{
			bool New;

			// If line not created -> do it and put everything at 0
			if(!(*cur))
			{
				(*cur)=new double[i+1];
				memset(*cur,0,sizeof(double)*(i+1));
				New=true;
			}
			else
				New=false;

			// Go trough the cols
			size_t j;
			double* vals;
			for(j=0,vals=(*cur);j<i+1;j++,vals++)
			{
				if(Deleted.IsIn(j+1))
				{
					if(Manager->AutomaticMinSim)
						UpdateDeviation(NbComp,(*vals),0.0,-1);
					(*vals)=0.0;
					NbComp--;
				}
				else
				{
					GProfile* prof2=Manager->Session->GetProfile(j+1,false);
					if(!prof2)
						continue;
					GSubProfile* sub2=prof2->GetSubProfile(Lang);
					if(!sub2)
						continue;
					double val=Compute(sub1,sub2);
					if(val<Manager->NullSimLevel)
						val=0.0;
					if(New)
					{
						if(Manager->AutomaticMinSim)
							UpdateDeviation(NbComp,0.0,val,1);
					}
					else
					{
						if(Manager->AutomaticMinSim)
							UpdateDeviation(NbComp,(*vals),val,0);
					}
					(*vals)=val;
					if(New)
						NbComp++;
				}
			}
			continue;
		}

		// Go trough the cols
		size_t j;
		double* vals;
		for(j=0,vals=(*cur);j<i+1;j++,vals++)
		{
			if(Deleted.IsIn(j+1))
			{
				if(Manager->AutomaticMinSim)
					UpdateDeviation(NbComp,(*vals),0.0,-1);
				(*vals)=0.0;
				NbComp--;
			}

			if(Modified.IsIn(j+1))
			{
				GProfile* prof2=Manager->Session->GetProfile(j+1,false);
				if(!prof2)
					continue;
				GSubProfile* sub2=prof2->GetSubProfile(Lang);
				if(!sub2)
					continue;
				double val=Compute(sub1,sub2);
				if(val<Manager->NullSimLevel)
					val=0.0;
				if(Manager->AutomaticMinSim)
					UpdateDeviation(NbComp,(*vals),val,0);
				(*vals)=val;
			}

			if(Created.IsIn(j+1))
			{
				GProfile* prof2=Manager->Session->GetProfile(j+1,false);
				if(!prof2)
					continue;
				GSubProfile* sub2=prof2->GetSubProfile(Lang);
				if(!sub2)
					continue;
				double val=Compute(sub1,sub2);
				if(val<Manager->NullSimLevel)
					val=0.0;
				if(Manager->AutomaticMinSim)
					UpdateDeviation(NbComp,0.0,val,1);
				(*vals)=val;
				NbComp++;
			}
		}
	}
	Deleted.Clear();
	Created.Clear();
	Modified.Clear();
	NeedUpdate=false;
}


//------------------------------------------------------------------------------
GProfilesSimsCosinus::GProfilesSim::~GProfilesSim(void)
{
	if(Values)
	{
		double** tmp;
		size_t i;
		for(tmp=Values,i=NbLines+1;--i;tmp++)
			delete[] (*tmp);
		delete[] Values;
	}
}



//------------------------------------------------------------------------------
//
//  GProfilesSimsCosinus
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesSimsCosinus::GProfilesSimsCosinus(GFactoryMeasure* fac)
	: GMeasure(fac), GSignalHandler(), Values(10,5), NullSimLevel(0.000001), MinSim(0.5), AutomaticMinSim(true)
{
	GSession::AddHandler(this);
	R::RCursor<GLang> Langs(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());
	for(Langs.Start();!Langs.End();Langs.Next())
		Values.InsertPtr(new GProfilesSim(this,Langs()));
}


//-----------------------------------------------------------------------------
void GProfilesSimsCosinus::ApplyConfig(void)
{
	NullSimLevel=Factory->GetDouble("NullSimLevel");
	MinSim=Factory->GetDouble("MinSim");
	AutomaticMinSim=Factory->GetBool("AutomaticMinSim");
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetMeasure(unsigned int id1,unsigned int id2,unsigned int)
{
	// If same subprofile -> return 1
	if(id1==id2)
		return(1.0);

	// Get the subprofiles and verify that they defined in the same language
	GSubProfile* sub1=Session->GetSubProfile(id1,0);
	GSubProfile* sub2=Session->GetSubProfile(id2,0);
	if(sub1->GetLang()!=sub2->GetLang())
		throw GException("Cannot compare two subprofiles of a different language");

	// Get the language
	GProfilesSim* profSim=Values.GetPtr<GLang*>(sub1->GetLang());
	if(!profSim)
		throw GException("Language not defined");
	if(profSim->NeedUpdate)
		profSim->Update();

	// Get the sims (use the identificators of the profile)
	id1=sub1->GetProfile()->GetId();
	id2=sub2->GetProfile()->GetId();
	if(id1<id2)
	{
		size_t tmp=id1;
		id1=id2;
		id2=tmp;
	}
	return(profSim->Values[id1-2][id2-1]);
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetMinMeasure(const GLang* lang,unsigned int)
{
	double deviationrate=1.5;

	if(!AutomaticMinSim)
		return(MinSim);

	GProfilesSim* profSim=Values.GetPtr(lang);
	if(!profSim)
		throw GException("Language not defined");
	if(profSim->NeedUpdate)
		profSim->Update();
	return(profSim->MeanSim+deviationrate*sqrt(profSim->Deviation));
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetMinMeasure(unsigned int)
{
	throw GException("No minimal measure independant of the languages");
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::Event(GLang* lang, tEvent event)
{
	switch(event)
	{
		case eObjNewMem:
			Values.InsertPtr(new GProfilesSim(this,lang));
			break;
		case eObjDeleteMem:
			Values.DeletePtr(*lang);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::Event(GSubProfile* sub, tEvent event)
{
	GProfilesSim* profSim;

	switch(event)
	{
		case eObjNewMem:
			profSim=Values.GetPtr<GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			profSim->Created.Insert(sub->GetProfile()->GetId());
			if(profSim->Deleted.IsIn(sub->GetProfile()->GetId()))
				profSim->Deleted.Delete(sub->GetProfile()->GetId());
			if(profSim->Modified.IsIn(sub->GetProfile()->GetId()))
				profSim->Modified.Delete(sub->GetProfile()->GetId());
			profSim->NeedUpdate=true;
			break;
		case eObjModified:
			profSim=Values.GetPtr<GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			if(!profSim->Created.IsIn(sub->GetProfile()->GetId()))
				profSim->Modified.Insert(sub->GetProfile()->GetId());
			if(profSim->Deleted.IsIn(sub->GetProfile()->GetId()))
				profSim->Deleted.Delete(sub->GetProfile()->GetId());
			profSim->NeedUpdate=true;
			break;
		case eObjDeleteMem:
			profSim = Values.GetPtr<const GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			if(profSim->Created.IsIn(sub->GetProfile()->GetId()))
				profSim->Created.Delete(sub->GetProfile()->GetId());
			if(profSim->Modified.IsIn(sub->GetProfile()->GetId()))
				profSim->Modified.Delete(sub->GetProfile()->GetId());
			profSim->Deleted.Insert(sub->GetProfile()->GetId());
			profSim->NeedUpdate=true;
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesSimsCosinus::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NullSimLevel",0.00001));
	params->InsertParam(new RParamValue("MinSim",0.05));
	params->InsertParam(new RParamValue("AutomaticMinSim",true));
}


//------------------------------------------------------------------------------
GProfilesSimsCosinus::~GProfilesSimsCosinus(void)
{
	GSession::DeleteHandler(this);
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles Similarities","Cosinus Method",GProfilesSimsCosinus)
