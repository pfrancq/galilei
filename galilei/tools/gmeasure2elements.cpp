/*

	GALILEI Research Project

	GMeasure2Elements.cpp

	Measures between two elements - Implementation.

	Copyright 2007 by the Université Libre de Bruxelles.

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
// include standard api files
#include <math.h>


//------------------------------------------------------------------------------
// include files for R
#include <rvectorint.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure2elements.h>
#include <glang.h>
#include <gsession.h>
#include <gprofile.h>
#include <gsubprofile.h>
#include <gdoc.h>
#include <ggroup.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
class LangMeasure;




//------------------------------------------------------------------------------
//
// class Matrix
//
//------------------------------------------------------------------------------

class Matrix
{
public:
	double** Values;
	size_t NbLines;
	bool NeedUpdate;
	RVectorInt<true> Created;
	RVectorInt<true> Modified;
	RVectorInt<true> Deleted;
	double Deviation;
	double MeanSim;
	
	Matrix(size_t nb);
	void Clear(size_t max);
	void Update(size_t max,bool Automatic,GMeasure2Elements* main,GLang* lang,double Null);
	void UpdateDeviation(size_t oldnbcomp,double oldsim,double newsim,int what);
	double ComputeMin(size_t max,GMeasure2Elements* main,GLang* lang,double Null);
	~Matrix(void);
};


//------------------------------------------------------------------------------
Matrix::Matrix(size_t nb)
	: Values(0), NbLines(0), NeedUpdate(false), Created(nb), Modified(nb),
	  Deleted(nb),Deviation(0.0), MeanSim(0.0)
{
}


//------------------------------------------------------------------------------
void Matrix::Clear(size_t max)
{
	delete Values;
	Values=0;
	NbLines=0;
	NeedUpdate=true;
	Created.Clear();
	Modified.Clear();
	Deleted.Clear();
	for(size_t i=0;i<max+1;i++)
		Created.Insert(i);
	Deviation=0.0;
	MeanSim=0.0;
}


//------------------------------------------------------------------------------
void Matrix::Update(size_t max,bool Automatic,GMeasure2Elements* main,GLang* lang,double Null)
{
	if(!max)
		return;
		
	// Add the lines (if necessary)
	size_t NbComp=(NbLines*(NbLines-1))/2;
		
	if(NbLines<max)
	{
		double** tmp=new double*[max];
		if(Values)
		{
			memcpy(tmp,Values,NbLines*sizeof(double*));
			delete[] Values;
		}
		Values=tmp;
		memset(&tmp[NbLines],0,(max-NbLines)*sizeof(double*));
		NbLines=max;
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
				if(Automatic)
					UpdateDeviation(NbComp,(*vals),0,-1);
				NbComp--;
			}
			delete[] (*cur);
			(*cur)=0;
			continue;
		}

		void* obj1=main->GetElement(lang,i+2);
		if(!obj1)
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
					if(Automatic)
						UpdateDeviation(NbComp,(*vals),0.0,-1);
					(*vals)=0.0;
					NbComp--;
				}
				else
				{
					void* obj2=main->GetElement(lang,j+1);
					if(!obj2)
						continue;
				
					double val=main->Compute(lang,obj1,obj2);
					if(val<Null)
						val=0.0;
					if(New)
					{
						if(Automatic)
							UpdateDeviation(NbComp,0.0,val,1);
					}
					else
					{
						if(Automatic)
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
				if(Automatic)
					UpdateDeviation(NbComp,(*vals),0.0,-1);
				(*vals)=0.0;
				NbComp--;
			}

			if(Modified.IsIn(j+1))
			{
				void* obj2=main->GetElement(lang,j+1);
				if(!obj2)
					continue;
				
				double val=main->Compute(lang,obj1,obj2);
				if(val<Null)
					val=0.0;
				if(Automatic)
					UpdateDeviation(NbComp,(*vals),val,0);
				(*vals)=val;
			}

			if(Created.IsIn(j+1))
			{
				void* obj2=main->GetElement(lang,j+1);
				if(!obj2)
					continue;
				
				double val=main->Compute(lang,obj1,obj2);
				if(val<Null)
					val=0.0;
				if(Automatic)
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
void Matrix::UpdateDeviation(size_t oldnbcomp,double oldsim,double newsim,int what)
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
double Matrix::ComputeMin(size_t max,GMeasure2Elements* main,GLang* lang,double Null)
{
	if(!max)
		return(0.0);
		
	double meansim=0.0;
	double deviation=0.0;
	double deviationrate=1.5;
	double NbComp=0.0;
	double sim;
		
	// Go thourgh all elements
	for(size_t i=0;i<max-1;i++)
	{
		void* obj1=main->GetElement(lang,i+2);
		if(!obj1)
			continue;			
		
		for(size_t j=0;j<i+1;j++)
		{
			void* obj2=main->GetElement(lang,j+1);
			if(!obj2)
				continue;			
			NbComp+=1.0;
			sim=main->Compute(lang,obj1,obj2);
			if(sim<Null)
				sim=0.0;
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
Matrix::~Matrix(void)
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
// class LangMeasure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class representing all the measure between elements of a given language.
class LangMeasure
{
public:
	GLang* Lang;                                          // Language
	Matrix* Values;
	
	// Constructor and Compare methods.
	LangMeasure(GLang* lang,size_t nb);
	int Compare(const GLang& l) const {return(Lang->Compare(l));}
	int Compare(const GLang* l) const
	{
		if(!l)
			return(1);
		return(Lang->Compare(l));
	}
	int Compare(const LangMeasure& l) const {return(Lang->Compare(*l.Lang));}

	// Destructor.
	virtual ~LangMeasure(void);
};


//------------------------------------------------------------------------------
LangMeasure::LangMeasure(GLang* lang,size_t nb)
	:  Lang(lang), Values(0)
{
	if(!nb)
		nb=200;
	Values=new Matrix(nb);
}



//------------------------------------------------------------------------------
LangMeasure::~LangMeasure(void)
{
	delete Values;
}



//------------------------------------------------------------------------------
//
//  GMeasure2Elements::Intern
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GMeasure2Elements::Intern
{
public:

	/**
	* Measures per lang.
	*/
	R::RContainer<LangMeasure,true,true>* Langs;

	/**
	 * Measures;
	 */
	Matrix* Values;
	
	Intern(void) : Langs(0), Values(0) {}
	  
	~Intern(void)
	{
		delete Langs;
		delete Values;
	}
};



//------------------------------------------------------------------------------
//
//  GMeasure2Elements
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMeasure2Elements::GMeasure2Elements(GFactoryMeasure* fac,bool min,bool perlang,double equals,tObjType objs)
	: GMeasure(fac), GSignalHandler(), Data(0),NullLevel(0.000001), MinMeasure(0.5), AutomaticMinMeasure(true), 
	  MinMeasureSense(min), Memory(true), PerLang(perlang),
	  Equals(equals), ObjsType(objs)
{
	Data=new Intern();
	GSession::AddHandler(this);
}


//-----------------------------------------------------------------------------
void GMeasure2Elements::ApplyConfig(void)
{
	NullLevel=Factory->GetDouble("NullLevel");
	MinMeasure=Factory->GetDouble("MinMeasure");
	AutomaticMinMeasure=Factory->GetBool("AutomaticMinMeasure");
	Memory=Factory->GetBool("Memory");
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Connect(GSession* session)
{
	GMeasure::Connect(session);
	if(PerLang)
	{
		Data->Langs=new R::RContainer<LangMeasure,true,true>(10,5);
		R::RCursor<GLang> Cur(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());
		for(Cur.Start();!Cur.End();Cur.Next())
			Data->Langs->InsertPtr(new LangMeasure(Cur(),GetMaxElementsId(Cur())));
	}
	else
	{
		size_t nb= GetMaxElementsId(0);
		if(!nb)
			nb=500;
		Data->Values=new Matrix(nb);
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Disconnect(GSession* session)
{
	if(PerLang)
	{
		delete Data->Langs;
		Data->Langs=0;
	}
	else
	{
		delete Data->Values;
		Data->Values=0;
	}
	GMeasure::Disconnect(session);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Dirty(void)
{
	if(PerLang)
	{
		if(!Data->Langs)
			return;
/*		delete Data->Langs;
		Data->Langs=0;*/
		RCursor<LangMeasure> Cur(*Data->Langs);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			Cur()->Values->Clear(GetMaxElementsId(Cur()->Lang));
			/*Cur()->Values->Created.Clear();
			Cur()->Values->Deleted.Clear();
			Cur()->Values->Modified.Clear();
			Cur()->Values->NeedUpdate=true;
			for(size_t i=0;i<GetMaxElementsId(Cur()->Lang);i++)
				Cur()->Values->Modified.Insert(i);*/				
		}
	}
	else
	{
		if(!Data->Values)
			return;		
		Data->Values->Clear(GetMaxElementsId(0));
/*		delete Data->Values;
		Data->Values=0;*/
		/*Data->Values->Created.Clear();
		Data->Values->Deleted.Clear();
		Data->Values->Modified.Clear();
		Data->Values->NeedUpdate=true;
		for(size_t i=0;i<GetMaxElementsId(0);i++)
			Data->Values->Modified.Insert(i);*/	
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Measure(unsigned int measure,...)
{
	va_list ap;
	va_start(ap,measure);
	GLang* lang;
	if(PerLang)
		lang=va_arg(ap,GLang*);
	else
		lang=0;		
	unsigned int id1=va_arg(ap,unsigned int);
	unsigned int id2=va_arg(ap,unsigned int);
	double* res=va_arg(ap,double*);
	Matrix* Values;
	
	// If same profile -> return default
	if(id1==id2)
	{
		(*res)=Equals;
		va_end(ap);
		return;
	}

	if(!Memory)
	{
		void* obj1=GetElement(lang,id1);
		if(obj1)
		{
			void* obj2=GetElement(lang,id2);
			if(obj2)
				(*res)=Compute(lang,obj1,obj2);
			else
				(*res)=0.0;
		}
		else
			(*res)=0.0;
		va_end(ap);
		return;
	}


	//  Choose the correct matrix to pointed
	if(PerLang)
	{
		// Get the language
		LangMeasure* l=Data->Langs->GetPtr(lang);
		if(!l)
			throw GException("Language not defined");
		Values=l->Values;
	}
	else
		Values=Data->Values;
		
	
	// If Update needed, do it.
	if(Values->NeedUpdate)
		Values->Update(GetMaxElementsId(lang),AutomaticMinMeasure,this,lang,NullLevel);

	// Check order of identifier and get the value
	if(id1<id2)
	{
		size_t tmp=id1;
		id1=id2;
		id2=tmp;
	}
	(*res)=Values->Values[id1-2][id2-1];
	va_end(ap);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Info(unsigned int info,...)
{
	if(!MinMeasureSense)
		return;
	
	va_list ap;
	GLang* lang;
	va_start(ap,info);
	if(PerLang)
		lang=va_arg(ap,GLang*);
	else
		lang=0;
	double* res=va_arg(ap,double*);
	Matrix* Values;
	double deviationrate=1.5;

	if(!AutomaticMinMeasure)
	{
		(*res)=MinMeasure;
		va_end(ap);
		return;
	}

	if(!Memory)
	{
		(*res)=Values->ComputeMin(GetMaxElementsId(lang),this,lang,NullLevel);
		va_end(ap);
		return;
	}

	//  Choose the correct matrix to pointed
	if(PerLang)
	{
		// Get the language
		LangMeasure* l=Data->Langs->GetPtr(lang);
		if(!l)
			throw GException("Language not defined");
		Values=l->Values;		
	}
	else
		Values=Data->Values;

		
	if(Values->NeedUpdate)
		Values->Update(GetMaxElementsId(lang),AutomaticMinMeasure,this,lang,NullLevel);
	(*res)=Values->MeanSim+deviationrate*sqrt(Values->Deviation);
	va_end(ap);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GLang* lang, tEvent event)
{
	if((!PerLang)||(!Memory)||(!Session))
		return;

	switch(event)
	{
		case eObjNew:
			if(Data->Langs)
				Data->Langs->InsertPtr(new LangMeasure(lang,GetMaxElementsId(lang)));
			break;
		case eObjDelete:
			if(Data->Langs)
				Data->Langs->DeletePtr(*lang);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
template<class C>
	void GMeasure2Elements::UpdateElement(C* element,tEvent event,GLang* lang)
{
	Matrix* Values;
	size_t id=element->GetId();
	
	switch(event)
	{
		case eObjNew:
			if(PerLang)
			{
				if((!Data->Langs)||(!lang))
					break;				
				LangMeasure* l=Data->Langs->GetPtr(lang);
				if(!l)
					throw GException("Language not defined");
				Values=l->Values;
			}
			else
				Values=Data->Values;
			Values->Created.Insert(id);
			if(Values->Deleted.IsIn(id))
				Values->Deleted.Delete(id);
			if(Values->Modified.IsIn(id))
				Values->Modified.Delete(id);
			Values->NeedUpdate=true;
			break;
		case eObjModified:
			if(PerLang)
			{
				if((!Data->Langs)||(!lang))
					break;				
				LangMeasure* l=Data->Langs->GetPtr(lang);
				if(!l)
					throw GException("Language not defined");
				Values=l->Values;
			}
			else
				Values=Data->Values;
			if(!Values->Created.IsIn(id))
				Values->Modified.Insert(id);
			if(Values->Deleted.IsIn(id))
				Values->Deleted.Delete(id);
			Values->NeedUpdate=true;
			break;
		case eObjDelete:
			if(PerLang)
			{
				if((!Data->Langs)||(!lang))
					break;
				LangMeasure* l=Data->Langs->GetPtr(lang);
				if(!l)
					throw GException("Language not defined");
				Values=l->Values;
			}
			else
				Values=Data->Values;
			if(Values)
			{		
				if(Values->Created.IsIn(id))
					Values->Created.Delete(id);
				if(Values->Modified.IsIn(id))
					Values->Modified.Delete(id);
				Values->Deleted.Insert(id);
				Values->NeedUpdate=true;
			}
			break;
		default:
			break;
	}	
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GDoc* doc, tEvent event)
{
	if((!Memory)||(ObjsType!=otDoc))
		return;
	UpdateElement(doc,event,doc->GetLang());
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GProfile* prof, tEvent event)
{
	if((!Memory)||(ObjsType!=otProfile))
		return;
	UpdateElement(prof,event,0);	
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GSubProfile* sub, tEvent event)
{
	if((!Memory)||(ObjsType!=otSubProfile))
		return;
	UpdateElement(sub,event,sub->GetLang());
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GGroup* grp, tEvent event)
{
	if((!Memory)||(ObjsType!=otGroup))
		return;
	UpdateElement(grp,event,grp->GetLang());
}


//------------------------------------------------------------------------------
void GMeasure2Elements::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NullLevel",0.00001));
	params->InsertParam(new RParamValue("MinMeasure",0.05));
	params->InsertParam(new RParamValue("AutomaticMinMeasure",true));
	params->InsertParam(new RParamValue("Memory",true));
}


//------------------------------------------------------------------------------
GMeasure2Elements::~GMeasure2Elements(void)
{
	GSession::DeleteHandler(this);
}
