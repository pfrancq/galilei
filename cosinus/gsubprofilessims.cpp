/*

	GALILEI Research Project

	GSubProfiles.cpp

	List of SubProfiles for a given Language - Implementation.

	Copyright 2003-2007 by the Université Libre de Bruxelles.

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
#include <gsubprofilessims.h>
#include <gsubprofile.h>
#include <gprofile.h>
#include <glang.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GSubProfilesSims::GProfilesSim
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities between subprofiles of a given
// language.
class GSubProfilesSims::GProfilesSim
{
public:
	double** Values;
	GLang* Lang;                                          // Language
	double Deviation;                                     // Standart deviation of similarities
	GSubProfilesSims* Manager;                               //manger of the gprofsim
	size_t NbLines;
	bool NeedUpdate;
	RVectorInt<true> Created;
	RVectorInt<true> Modified;
	RVectorInt<true> Deleted;
	double MeanSim;

	// Constructor and Compare methods.
	GProfilesSim(GSubProfilesSims* manager,GLang* lang);
	int Compare(const GLang& l) const {return(Lang->Compare(l));}
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GProfilesSim& profilesSim) const {return(Lang->Compare(*profilesSim.Lang));}

	double Compute(GSubProfile* sub1,GSubProfile* sub2);
	void Update(void);
	void UpdateDeviation(size_t oldnbcomp,double oldsim,double newsim,int what);
	double ComputeMin(void);

	// Destructor.
	virtual ~GProfilesSim(void);
};


//------------------------------------------------------------------------------
GSubProfilesSims::GProfilesSim::GProfilesSim(GSubProfilesSims* manager,GLang* lang)
	:  Values(0), Lang(lang), Deviation(0.0), Manager(manager), NbLines(0), NeedUpdate(false), Created(5000), Modified(5000), Deleted(5000), MeanSim(0.0)
{
}


//------------------------------------------------------------------------------
void GSubProfilesSims::GProfilesSim::UpdateDeviation(size_t oldnbcomp,double oldsim,double newsim,int what)
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
double GSubProfilesSims::GProfilesSim::Compute(GSubProfile* sub1,GSubProfile* sub2)
{
	return(sub1->SimilarityIFF2(*sub2,otDoc,otSubProfile,Lang));
}


//------------------------------------------------------------------------------
void GSubProfilesSims::GProfilesSim::Update(void)
{
	// if memory is false, no update is needed
	// since sims are claulctaed each time
	if(!Manager->Session)
		return;

	// Add the lines (if necessary)
	size_t MaxLines=Manager->Session->GetMaxSubProfileId(Lang)-1;
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

		GSubProfile* sub1=Manager->Session->GetSubProfile(Lang,i+2,false);
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
					GSubProfile* sub2=Manager->Session->GetSubProfile(Lang,j+1,false);
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
				GSubProfile* sub2=Manager->Session->GetSubProfile(Lang,j+1,false);
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
				GSubProfile* sub2=Manager->Session->GetSubProfile(Lang,j+1,false);			
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
double GSubProfilesSims::GProfilesSim::ComputeMin(void)
{
	double meansim=0.0;
	double deviation=0.0;
	double deviationrate=1.5;
	double NbComp=0.0;
	double sim;
	size_t i;
	
	RCursor<GSubProfile> Cur1(Manager->Session->GetSubProfiles(Lang));
	RCursor<GSubProfile> Cur2(Manager->Session->GetSubProfiles(Lang));
	for(Cur1.Start(),i=2;i<Cur1.GetNb();Cur1.Next(),i++)
	{
		if(!Cur1()->IsDefined())
			continue;
		for(Cur2.GoTo(i);!Cur2.End();Cur2.Next())
		{
			if(!Cur2()->IsDefined())
				continue;
			NbComp+=1.0;
			sim=Compute(Cur1(),Cur2());
			meansim+=sim;
			deviation+=sim*sim;
		}
	}
	if(NbComp>0.0)
	{
		meansim/=NbComp;
		deviation=(deviation/NbComp)-(meansim*meansim);
	}
	sim=meansim+deviationrate*sqrt(deviation);
	return(sim);
}


//------------------------------------------------------------------------------
GSubProfilesSims::GProfilesSim::~GProfilesSim(void)
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
//  GSubProfilesSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubProfilesSims::GSubProfilesSims(GFactoryMeasure* fac)
	: GMeasure(fac), GSignalHandler(), Values(10,5), NullSimLevel(0.000001),
	  MinSim(0.5), AutomaticMinSim(true), Memory(true)
{
	GSession::AddHandler(this);
	R::RCursor<GLang> Langs(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());
	for(Langs.Start();!Langs.End();Langs.Next())
		Values.InsertPtr(new GProfilesSim(this,Langs()));
}


//-----------------------------------------------------------------------------
void GSubProfilesSims::ApplyConfig(void)
{
	NullSimLevel=Factory->GetDouble("NullSimLevel");
	MinSim=Factory->GetDouble("MinSim");
	AutomaticMinSim=Factory->GetBool("AutomaticMinSim");
	Memory=Factory->GetBool("Memory");
}


//------------------------------------------------------------------------------
void GSubProfilesSims::Measure(unsigned int measure,...)
{
	va_list ap;
	va_start(ap,measure);
	GLang* lang=va_arg(ap,GLang*);
	unsigned int id1=va_arg(ap,unsigned int);
	unsigned int id2=va_arg(ap,unsigned int);
	double* res=va_arg(ap,double*);
	
	// If same subprofile -> return 1
	if(id1==id2)
	{
		(*res)=1.0;
		va_end(ap);
		return;
	}

	if(!Memory)
	{
		GSubProfile* sub1=Session->GetSubProfile(lang,id1,false);
		if(sub1)
		{
			GSubProfile* sub2=Session->GetSubProfile(lang,id2,false);
			if(sub2)
				(*res)=sub1->SimilarityIFF2(*sub2,otDoc,otSubProfile,lang);
			else
				(*res)=0.0;
		}
		else
			(*res)=0.0;
		va_end(ap);
		return;		
	}

	// Get the language
	GProfilesSim* profSim=Values.GetPtr<GLang*>(lang);
	if(!profSim)
		throw GException("Language not defined");
	if(profSim->NeedUpdate)
		profSim->Update();

	// Get the sims (use the identificators of the profile)
	if(id1<id2)
	{
		size_t tmp=id1;
		id1=id2;
		id2=tmp;
	}
	(*res)=profSim->Values[id1-2][id2-1];
	va_end(ap);
}


//------------------------------------------------------------------------------
void GSubProfilesSims::Info(unsigned int info,...)
{
	va_list ap;
	va_start(ap,info);
	GLang* lang=va_arg(ap,GLang*);
	double* res=va_arg(ap,double*);
	
	double deviationrate=1.5;

	if(!AutomaticMinSim)
	{
		(*res)=MinSim;
		va_end(ap);
		return;
	}
	
	GProfilesSim* profSim=Values.GetPtr(lang);
	if(!profSim)
		throw GException("Language not defined");
		
	if(!Memory)
	{		
		(*res)=profSim->ComputeMin();
		va_end(ap);
		return;
	}
	
	if(profSim->NeedUpdate)
		profSim->Update();
	(*res)=profSim->MeanSim+deviationrate*sqrt(profSim->Deviation);
	va_end(ap);
}


//------------------------------------------------------------------------------
void GSubProfilesSims::Event(GLang* lang, tEvent event)
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
void GSubProfilesSims::Event(GSubProfile* sub, tEvent event)
{
	GProfilesSim* profSim;

	switch(event)
	{
		case eObjNewMem:
			profSim=Values.GetPtr<GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			profSim->Created.Insert(sub->GetId());
			if(profSim->Deleted.IsIn(sub->GetId()))
				profSim->Deleted.Delete(sub->GetId());
			if(profSim->Modified.IsIn(sub->GetId()))
				profSim->Modified.Delete(sub->GetId());
			profSim->NeedUpdate=true;
			break;
		case eObjModified:
			profSim=Values.GetPtr<GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			if(!profSim->Created.IsIn(sub->GetId()))
				profSim->Modified.Insert(sub->GetId());
			if(profSim->Deleted.IsIn(sub->GetId()))
				profSim->Deleted.Delete(sub->GetId());
			profSim->NeedUpdate=true;
			break;
		case eObjDeleteMem:
			profSim = Values.GetPtr<const GLang*>(sub->GetLang());
			if(!profSim)
				throw GException("Language not defined");
			if(profSim->Created.IsIn(sub->GetId()))
				profSim->Created.Delete(sub->GetId());
			if(profSim->Modified.IsIn(sub->GetId()))
				profSim->Modified.Delete(sub->GetId());
			profSim->Deleted.Insert(sub->GetId());
			profSim->NeedUpdate=true;
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GSubProfilesSims::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NullSimLevel",0.00001));
	params->InsertParam(new RParamValue("MinSim",0.05));
	params->InsertParam(new RParamValue("AutomaticMinSim",true));
	params->InsertParam(new RParamValue("Memory",true));
}


//------------------------------------------------------------------------------
GSubProfilesSims::~GSubProfilesSims(void)
{
	GSession::DeleteHandler(this);
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("SubProfiles Similarities","Cosinus Method",GSubProfilesSims)
