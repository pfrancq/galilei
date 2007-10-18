/*

	GALILEI Research Project

	GenericSims.cpp

	Similarities between documents and groups - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

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
#include <math.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include "ggenericsims.h"
#include <gweightinfos.h>
#include <gweightinfo.h>
#include <glang.h>
#include <gconcepttype.h>
#include <gconcept.h>



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
	unsigned int Id;         // Identifier
	double Sim;              // Similarity
	tObjState State;         // State of the similarity.

	GSim(unsigned int id)
		: Id(id), Sim(0.0),State(osModified) {}
	int Compare(const GSim& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
};



//------------------------------------------------------------------------------
//
// class GSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities for a given document.
class GSims : public RContainer<GSim,true,true>
{
public:
	unsigned int Id;      // Identifier of the document

	GSims(unsigned int id)
		: RContainer<GSim,true,true>(5000,5000), Id(id) {}
	int Compare(const GSims& s) const {return(Id-s.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}
};



//------------------------------------------------------------------------------
//
//  GenericSims::GSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the similarities between documents and subprofiles of
// a given language.
class GGenericSims::GElementSims
{
public:
	R::RContainer<GSims,true,true>* Sims;             // Similarities.
	GLang* Lang;                                      // Language.
	GGenericSims* Manager;                             // Owner.
	bool NeedUpdate;
	
	// Constructor and Compare functions.
	GElementSims(GGenericSims* manager,GLang* l);
	int Compare(const GLang* l) const {return(Lang->Compare(l));}
	int Compare(const GLang& l) const {return(Lang->Compare(l));}
	int Compare(const GElementSims& sim) const {return(Lang->Compare(*sim.Lang));}

	// Analyse the similarity of the two subprofiles and insert when necessary.
	void AnalyseSim(GSims* sim,const GDoc* doc,const GGroup* grp);

	// Get the similarities between two profiles, i.e. the subprofiles of a same
	// language.
	double GetSim(size_t id1,size_t id2);

	// Update the state of the sims between documents and subprofiles. If a
	// document or a If the subprofile has changed the corresponding sim will be
	// set to state="osModified".
	// If the similarity for a given subprofile doesn't exist, the element is
	// created but not computed ( -> state to osModified ).
	void Update(void);

	// Destructor.
	~GElementSims(void) { delete Sims;}
};


//------------------------------------------------------------------------------
GGenericSims::GElementSims::GElementSims(GGenericSims* manager,GLang* l)
	: Sims(0), Lang(l), Manager(manager), NeedUpdate(true)
{
	// if memory is false, we don't stock a container of similarities.
	// sims will be re-calculated eacht time.
	if((!Manager->Memory)||(!Manager->Session))
		return;
}


//------------------------------------------------------------------------------
double GGenericSims::GElementSims::GetSim(size_t id1,size_t id2)
{
	GSims* s;
	GSim* s2;

	s=Sims->GetPtr<unsigned int>(id1);
	if(!s)
		Sims->InsertPtr(s=new GSims(id1));
	s2=s->GetPtr<unsigned int>(id2);
	if(!s2)
		s->InsertPtr(s2=new GSim(id2));
	if((s2->State==osUpdated)||(s2->State==osUpToDate)) return(s2->Sim);
	if(s2->State == osModified)
	{
		s2->State = osUpToDate;
		s2->Sim=Manager->Compute(Lang,id1,id2);
		if(fabs(s2->Sim)<Manager->NullSimLevel) s2->Sim=0.0;
		return (s2->Sim);
	}
	cerr<<"Problem in GGenericSims"<<endl;
	return(0.0);
}


//------------------------------------------------------------------------------
void GGenericSims::GElementSims::Update(void)
{
	// If memory is false, no update is needed
	// since sims are claulctaed each time
	if((!Manager->Memory)||(!Manager->Session)||(!NeedUpdate))
		return;

	//initialize the container of GSims (calculate size)
	if(!Sims)
		Sims=new RContainer<GSims,true,true>(Manager->GetMaxId1(Lang),500);

	RCursor<GSims> Cur1(*Sims);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		RCursor<GSim> Cur2(*Cur1());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			Cur2()->State=osModified;
	}
	
	
	NeedUpdate=false;
}



//------------------------------------------------------------------------------
//
//  GenericSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGenericSims::GGenericSims(GFactoryMeasure* fac,bool d,bool g,bool s)
	: GMeasure(fac), GSignalHandler(), Sims(10,5), Memory(false), Doc(d), Group(g), SubProfile(s)
{
	GSession::AddHandler(this);
	if(!Memory) return;
	Sims.Clear();
	R::RCursor<GLang> Langs(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());
	for(Langs.Start();!Langs.End();Langs.Next())
		Sims.InsertPtr(new GElementSims(this,Langs()));
}


//-----------------------------------------------------------------------------
void GGenericSims::ApplyConfig(void)
{
	NullSimLevel=Factory->GetDouble("NullSimLevel");
	Memory=Factory->GetBool("Memory");
}


//------------------------------------------------------------------------------
void GGenericSims::Measure(unsigned int measure,...)
{
	va_list ap;
	va_start(ap,measure);
	GLang* lang=va_arg(ap,GLang*);
	unsigned int id1=va_arg(ap,unsigned int);
	unsigned int id2=va_arg(ap,unsigned int);
	double* res=va_arg(ap,double*);

	if(!Memory)
	{
		(*res)=Compute(lang,id1,id2);
		va_end(ap);
		return;
	}

	// Get the language
	GElementSims* Sim=Sims.GetPtr<GLang*>(lang);
	if(!Sim)
		throw GException("Language not defined");
	if(Sim->NeedUpdate)
		Sim->Update();
		
	(*res)=Sim->GetSim(id1,id2);
	va_end(ap);
}


//------------------------------------------------------------------------------
void GGenericSims::Event(GLang* lang, tEvent event)
{
	if(!Memory) return;
	switch(event)
	{
		case eObjNew:
			Sims.InsertPtr(new GElementSims(this,lang));
			break;
		case eObjDelete:
			Sims.DeletePtr(*lang);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GGenericSims::Event(GDoc* doc, tEvent)
{
	if((!Memory)||(!Doc)) return;
	if(!doc->GetLang())
		return;
	GElementSims* Sim=Sims.GetPtr<const GLang*>(doc->GetLang());
	if(!Sim)
		throw GException("Language not defined");		
	Sim->NeedUpdate=true;	
}


//------------------------------------------------------------------------------
void GGenericSims::Event(GGroup* grp, tEvent)
{
	if((!Memory)||(!Group)) return;
	if(!grp->GetLang())
		return;
	GElementSims* Sim=Sims.GetPtr<const GLang*>(grp->GetLang());
	if(!Sim)
		throw GException("Language not defined");		
	Sim->NeedUpdate=true;	
}


//------------------------------------------------------------------------------
void GGenericSims::Event(GSubProfile* sub, tEvent)
{
	if((!Memory)||(!SubProfile)) return;
	if(!sub->GetLang())
		return;
	GElementSims* Sim=Sims.GetPtr<const GLang*>(sub->GetLang());
	if(!Sim)
		throw GException("Language not defined");		
	Sim->NeedUpdate=true;	
}


//------------------------------------------------------------------------------
void GGenericSims::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NullSimLevel",0.00001));
	params->InsertParam(new RParamValue("Memory",false));
}


//------------------------------------------------------------------------------
GGenericSims::~GGenericSims(void)
{
	GSession::DeleteHandler(this);
}


//------------------------------------------------------------------------------
//
// class GDiffSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDiffSims::GDiffSims(GFactoryMeasure* fac,bool min,tObjType objs)
	: GMeasure2Elements(fac,min,true,1.0,objs)
{
}


//------------------------------------------------------------------------------
void GDiffSims::ApplyConfig(void)
{
	GMeasure2Elements::ApplyConfig();
	SimType=0;
	RString type=Factory->Get("SimType");
	Factor=Factory->GetDouble("Factor");
	unsigned int sim; 
	if(type=="Multi-vector")
		sim=1;
	if(type=="Language")
		sim=2;
	if(SimType!=sim)
		Dirty();
	SimType=sim;
}


//------------------------------------------------------------------------------
double GDiffSims::SimilarityIFFMV(void) const
{
	// if one SubProfile is not defined -> the similarity must be null
	if((!vec1->GetNb())||(!vec2->GetNb()))
		return(0.0);

	double max1;
	double max2;	
	double GlobalSim(1.0);	
	double Sim;
	double norm1;
	double norm2;
	double w1,w2,iff;
	double TotalRef;
	GConceptType* type(0);
		
	RCursor<GWeightInfo> ptr(*vec1);
	RCursor<GWeightInfo> ptr2(*vec2);

	ptr.Start();
	ptr2.Start();
	while(!ptr.End())
	{
		// Look if the type of the concept have changed since that the last concept treated
		if(ptr()->GetConcept()->GetType()!=type)
		{
			// If a type exist -> modify global sim
			if(type)
				GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
			
			// Yes -> A new total number of references.
			type=ptr()->GetConcept()->GetType();
			TotalRef=type->GetRef(ObjsType);
			norm1=norm2=Sim=0.0;
			max1=vec1->GetMaxAbsWeight(type);
			max2=vec2->GetMaxAbsWeight(type);
		}

		iff=TotalRef/static_cast<double>(type->GetRef(ptr()->GetId(),ObjsType));
		w1=(ptr()->GetWeight()/max1)*log10(iff);
		while((!ptr2.End())&&((*ptr2())<(*ptr())))
		{
			if(ptr2()->GetConcept()->GetType()!=type)
			{
				// If a type exist -> modify global sim
				if(type)
					GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
				
				// Yes -> A new total number of references.
				type=ptr2()->GetConcept()->GetType();
				TotalRef=type->GetRef(ObjsType);
				norm1=norm2=Sim=0.0;
				max1=vec1->GetMaxAbsWeight(type);
				max2=vec2->GetMaxAbsWeight(type);				
			}			
			iff=TotalRef/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjsType));
			w2=(ptr2()->GetWeight()/max2)*log10(iff);
			norm2+=w2*w2;
			ptr2.Next();
		}
		if((!ptr2.End())&&((*ptr2())==(*ptr())))
		{
			if(ptr2()->GetConcept()->GetType()!=type)
			{
				// If a type exist -> modify global sim
				if(type)
					GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
				
				// Yes -> A new total number of references.
				type=ptr2()->GetConcept()->GetType();
				TotalRef=type->GetRef(ObjsType);
				norm1=norm2=Sim=0.0;
				max1=vec1->GetMaxAbsWeight(type);
				max2=vec2->GetMaxAbsWeight(type);				
			}			
			if((ptr()->GetWeight()>0)||(ptr2()->GetWeight()>0))
			{
				iff=TotalRef/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjsType));
				w2=(ptr2()->GetWeight()/max2)*log10(iff);
				norm2+=w2*w2;
				norm1+=w1*w1;
				Sim+=w1*w2;
			}
			ptr2.Next();
		}
		else
			norm1+=w1*w1;
		ptr.Next();
	}
	while(!ptr2.End())
	{
		if(ptr2()->GetConcept()->GetType()!=type)
		{
			// If a type exist -> modify global sim
			if(type)
				GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
			
			// Yes -> A new total number of references.
			type=ptr2()->GetConcept()->GetType();
			TotalRef=type->GetRef(ObjsType);
			norm1=norm2=Sim=0.0;
			max1=vec1->GetMaxAbsWeight(type);
			max2=vec2->GetMaxAbsWeight(type);			
		}			
		iff=TotalRef/static_cast<double>(type->GetRef(ptr2()->GetId(),ObjsType));
		w2=(ptr2()->GetWeight()/max2)*log10(iff);
		norm2+=w2*w2;
		ptr2.Next();
	}

	// If a type exist -> modify global sim
	if(type)
		GlobalSim*=(Sim+Factor)/((sqrt(norm1)*sqrt(norm2))+Factor);
	return(GlobalSim);
}


//------------------------------------------------------------------------------
double GDiffSims::SimilarityIFFL(void) const
{
	// if one SubProfile is not defined -> the similarity must be null
	if((!vec1->GetNb())||(!vec2->GetNb()))
		return(0.0);

	double max1;
	double max2;	
	double GlobalSim(1.0);	
	double Sim;
	double norm1;
	double norm2;
	double w1,w2,iff;
	double TotalRef;
	
	RCursor<GWeightInfo> ptr(*vec1);
	RCursor<GWeightInfo> ptr2(*vec2);

	ptr.Start();
	ptr2.Start();
	
	// Skip everything the first non language
	while(ptr()->GetConcept()->GetType()!=Lang)
		ptr.Next();
	while(ptr2()->GetConcept()->GetType()!=Lang)
		ptr2.Next();

	// For lang
	TotalRef=Lang->GetRef(ObjsType);
	norm1=norm2=Sim=0.0;
	max1=vec1->GetMaxAbsWeight(Lang);
	max2=vec2->GetMaxAbsWeight(Lang);
	
	while((!ptr.End())&&(ptr()->GetConcept()->GetType()==Lang))
	{
		iff=TotalRef/static_cast<double>(Lang->GetRef(ptr()->GetId(),ObjsType));
		w1=(ptr()->GetWeight()/max1)*log10(iff);
		while((!ptr2.End())&&(ptr2()->GetConcept()->GetType()==Lang)&&((*ptr2())<(*ptr())))
		{
			iff=TotalRef/static_cast<double>(Lang->GetRef(ptr2()->GetId(),ObjsType));
			w2=(ptr2()->GetWeight()/max2)*log10(iff);
			norm2+=w2*w2;
			ptr2.Next();
		}
		if((!ptr2.End())&&(ptr2()->GetConcept()->GetType()==Lang)&&((*ptr2())==(*ptr())))
		{
			if((ptr()->GetWeight()>0)||(ptr2()->GetWeight()>0))
			{
				iff=TotalRef/static_cast<double>(Lang->GetRef(ptr2()->GetId(),ObjsType));
				w2=(ptr2()->GetWeight()/max2)*log10(iff);
				norm2+=w2*w2;
				norm1+=w1*w1;
				Sim+=w1*w2;
			}
			ptr2.Next();
		}
		else
			norm1+=w1*w1;
		ptr.Next();
	}
	while((!ptr2.End())&&(ptr2()->GetConcept()->GetType()==Lang))
	{
		iff=TotalRef/static_cast<double>(Lang->GetRef(ptr2()->GetId(),ObjsType));
		w2=(ptr2()->GetWeight()/max2)*log10(iff);
		norm2+=w2*w2;
		ptr2.Next();
	}

	// If a type exist -> modify global sim
	GlobalSim*=Sim/(sqrt(norm1)*sqrt(norm2));
	return(GlobalSim);	
}


//------------------------------------------------------------------------------
double GDiffSims::Compute(GLang* lang,void* obj1,void* obj2)
{
	vec1=static_cast<GWeightInfos*>(obj1);
	vec2=static_cast<GWeightInfos*>(obj2);
	Lang=lang->GetDict();
	if(SimType==1)
		return(SimilarityIFFMV());
	if(SimType==2)
		return(SimilarityIFFL());
	return(0.0);
}


//------------------------------------------------------------------------------
void GDiffSims::CreateParams(RConfig* params)
{
	GMeasure2Elements::CreateParams(params);
	params->InsertParam(new RParamValue("SimType","Multi-vector"));
	params->InsertParam(new RParamValue("Factor",0.5));
}
