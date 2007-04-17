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
#include <gprofilesdisagree.h>
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
	GProfilesSimsCosinus* Manager;                               //manger of the gprofsim
	size_t NbLines;
	bool NeedUpdate;
	RVectorInt<true> Created;
	RVectorInt<true> Modified;
	RVectorInt<true> Deleted;

	// Constructor and Compare methods.
	GProfilesSim(GProfilesSimsCosinus* manager,GLang* lang);
	int Compare(const GLang& l) const {return(Lang->Compare(l));}
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GProfilesSim& profilesSim) const {return(Lang->Compare(*profilesSim.Lang));}

	double Compute(GSubProfile* sub1,GSubProfile* sub2);
	void Update(void);

	// Destructor.
	virtual ~GProfilesSim(void);
};


//------------------------------------------------------------------------------
GProfilesSimsCosinus::GProfilesSim::GProfilesSim(GProfilesSimsCosinus* manager,GLang* lang)
	:  Values(0), Lang(lang), Manager(manager), NbLines(0), NeedUpdate(false), Created(5000), Modified(5000), Deleted(5000)
{
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GProfilesSim::Compute(GSubProfile* sub1,GSubProfile* sub2)
{
	unsigned int nbdiff;
	double nbcommon;
	double disagree=0.0;

	nbdiff=sub1->GetCommonDiffDocs(sub2);
	nbcommon=double(sub1->GetCommonDocs(sub2));
	if(nbcommon>=Manager->MinDiffDocs)
		disagree=nbdiff/nbcommon;
	return(disagree);
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
			NbComp-=(i+1);
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
	: GMeasure(fac), GSignalHandler(), Values(10,5), NullSimLevel(0.000001), MinDisagreement(0.5), MinDiffDocs(6)
{
	GSession::AddHandler(this);
	R::RCursor<GLang> Langs(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());
	for(Langs.Start();!Langs.End();Langs.Next())
		Values.InsertPtr(new GProfilesSim(this,Langs()));
}


//-----------------------------------------------------------------------------
void GProfilesSimsCosinus::ApplyConfig(void)
{
	MinDiffDocs=Factory->GetUInt("MinDiffDocs");
	NullSimLevel=Factory->GetDouble("NullSimLevel");
	MinDisagreement=Factory->GetDouble("MinDisagreement");
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetMeasure(unsigned int id1,unsigned int id2,unsigned int)
{
	// If same subprofile -> return 1
	if(id1==id2)
		return(1.0);

	// Get the subprofiles and verify that they defined in the same language
	GSubProfile* sub1=Session->GetSubProfile(id1);
	GSubProfile* sub2=Session->GetSubProfile(id2);
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
double GProfilesSimsCosinus::GetMinMeasure(const GLang*,unsigned int)
{
	return(MinDisagreement);
}


//------------------------------------------------------------------------------
double GProfilesSimsCosinus::GetMinMeasure(unsigned int)
{
	return(MinDisagreement);
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
	params->InsertParam(new RParamValue("MinDiffDocs",4));
	params->InsertParam(new RParamValue("NullSimLevel",0.00001));
	params->InsertParam(new RParamValue("MinDisagreement",0.6));
}


//------------------------------------------------------------------------------
GProfilesSimsCosinus::~GProfilesSimsCosinus(void)
{
	GSession::DeleteHandler(this);
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles Disagreements","Count Method",GProfilesSimsCosinus)
