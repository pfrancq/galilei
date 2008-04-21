/*

	GALILEI Research Project

	GMeasure2Elements.cpp

	Measures between two elements - Implementation.

	Copyright 2007-2008 by the Université Libre de Bruxelles.

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
#include <gdoc.h>
#include <ggroup.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
class LangMeasure;




//------------------------------------------------------------------------------
//
// class GMeasure2Elements::Intern
//
//------------------------------------------------------------------------------

class GMeasure2Elements::Intern
{
public:
	double** Values;
	size_t NbLines;
	bool NeedUpdate;
	RVectorInt<unsigned int,true> Created;
	RVectorInt<unsigned int,true> Modified;
	RVectorInt<unsigned int,true> Deleted;
	double Deviation;
	double MeanSim;
	
	Intern(size_t nb);
	void Clear(size_t max);
	void Update(size_t max,bool Automatic,GMeasure2Elements* main,double Null);
	void UpdateDeviation(size_t oldnbcomp,double oldsim,double newsim,int what);
	double ComputeMin(size_t max,GMeasure2Elements* main,double Null);
	~Intern(void);
};


//------------------------------------------------------------------------------
GMeasure2Elements::Intern::Intern(size_t nb)
	: Values(0), NbLines(0), NeedUpdate(false), Created(nb), Modified(nb),
	  Deleted(nb),Deviation(0.0), MeanSim(0.0)
{
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Intern::Clear(size_t max)
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
void GMeasure2Elements::Intern::Update(size_t max,bool Automatic,GMeasure2Elements* main,double Null)
{
	if(!max)
		return;
		
	// Add the lines (if necessary)	
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
	
	// Number of comparisons
	size_t NbComp=(NbLines*(NbLines-1))/2;
	
	// Go thourgh all lines/cols (line=profileid+2);
	size_t i;
	double** cur=Values;
	for(i=0;i<NbLines;i++,cur++)
	{
		// Verify if this line must be deleted?
		if((*cur)&&(Deleted.IsIn(i+2)))
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

		void* obj1=main->GetElement(i+2);
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
				if(vals&&Deleted.IsIn(j+1))
				{
					if(Automatic)
						UpdateDeviation(NbComp,(*vals),0.0,-1);
					(*vals)=0.0;
					NbComp--;
				}
				else
				{
					void* obj2=main->GetElement(j+1);
					if(!obj2)
						continue;
				
					double val=main->Compute(obj1,obj2);
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
			if(vals&&Deleted.IsIn(j+1))
			{
				if(Automatic)
					UpdateDeviation(NbComp,(*vals),0.0,-1);
				(*vals)=0.0;
				NbComp--;
			}

			if(Modified.IsIn(j+1))
			{
				void* obj2=main->GetElement(j+1);
				if(!obj2)
					continue;
				
				double val=main->Compute(obj1,obj2);
				if(val<Null)
					val=0.0;
				if(Automatic)
					UpdateDeviation(NbComp,(*vals),val,0);
				(*vals)=val;
			}

			if(Created.IsIn(j+1))
			{
				void* obj2=main->GetElement(j+1);
				if(!obj2)
					continue;
				
				double val=main->Compute(obj1,obj2);
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
void GMeasure2Elements::Intern::UpdateDeviation(size_t oldnbcomp,double oldsim,double newsim,int what)
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
double GMeasure2Elements::Intern::ComputeMin(size_t max,GMeasure2Elements* main,double Null)
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
		void* obj1=main->GetElement(i+2);
		if(!obj1)
			continue;			
		
		for(size_t j=0;j<i+1;j++)
		{
			void* obj2=main->GetElement(j+1);
			if(!obj2)
				continue;			
			NbComp+=1.0;
			sim=main->Compute(obj1,obj2);
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
GMeasure2Elements::Intern::~Intern(void)
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
//  GMeasure2Elements
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMeasure2Elements::GMeasure2Elements(GFactoryMeasure* fac,bool min,double equals,tObjType objs)
	: GMeasure(fac), GSignalHandler(), Data(0), NullLevel(0.000001), MinMeasure(0.5), AutomaticMinMeasure(true), 
	  MinMeasureSense(min), Memory(true), Equals(equals), ObjsType(objs)
{
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
	size_t nb=GetMaxElementsId();
	if(!nb)
		nb=500;
	Data=new Intern(nb);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Disconnect(GSession* session)
{
	delete Data;
	Data=0;
	GMeasure::Disconnect(session);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Dirty(void)
{
	if(!Data)
		return;		
	Data->Clear(GetMaxElementsId());
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Measure(unsigned int measure,...)
{
	va_list ap;
	va_start(ap,measure);
	unsigned int id1=va_arg(ap,unsigned int);
	unsigned int id2=va_arg(ap,unsigned int);
	double* res=va_arg(ap,double*);
	
	// If same profile -> return default
	if(id1==id2)
	{
		(*res)=Equals;
		va_end(ap);
		return;
	}

	if(!Memory)
	{
		void* obj1=GetElement(id1);
		if(obj1)
		{
			void* obj2=GetElement(id2);
			if(obj2)
				(*res)=Compute(obj1,obj2);
			else
				(*res)=0.0;
		}
		else
			(*res)=0.0;
		va_end(ap);
		return;
	}

	// If Update needed, do it.
	if(Data->NeedUpdate)
		Data->Update(GetMaxElementsId(),AutomaticMinMeasure,this,NullLevel);

	// Check order of identifier and get the value
	if(id1<id2)
	{
		size_t tmp=id1;
		id1=id2;
		id2=tmp;
	}
	(*res)=Data->Values[id1-2][id2-1];
	va_end(ap);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Info(unsigned int info,...)
{
	if(!MinMeasureSense)
		return;
	
	va_list ap;
	va_start(ap,info);
	double* res=va_arg(ap,double*);
	double deviationrate=1.5;

	if(!AutomaticMinMeasure)
	{
		(*res)=MinMeasure;
		va_end(ap);
		return;
	}

	if(!Memory)
	{
		(*res)=Data->ComputeMin(GetMaxElementsId(),this,NullLevel);
		va_end(ap);
		return;
	}

	
	if(Data->NeedUpdate)
		Data->Update(GetMaxElementsId(),AutomaticMinMeasure,this,NullLevel);
	(*res)=Data->MeanSim+deviationrate*sqrt(Data->Deviation);
	va_end(ap);
}


//------------------------------------------------------------------------------
template<class C>
	void GMeasure2Elements::UpdateElement(C* element,tEvent event)
{
	size_t id=element->GetId();
	
	switch(event)
	{
		case eObjNew:
			Data->Created.Insert(id);
			if(Data->Deleted.IsIn(id))
				Data->Deleted.Delete(id);
			if(Data->Modified.IsIn(id))
				Data->Modified.Delete(id);
			Data->NeedUpdate=true;
			break;
		case eObjModified:
			if(!Data->Created.IsIn(id))
				Data->Modified.Insert(id);
			if(Data->Deleted.IsIn(id))
				Data->Deleted.Delete(id);
			Data->NeedUpdate=true;
			break;
		case eObjDelete:
			if(!Data)
				return;
			if(Data->Created.IsIn(id))
				Data->Created.Delete(id);
			if(Data->Modified.IsIn(id))
				Data->Modified.Delete(id);
			Data->Deleted.Insert(id);
			Data->NeedUpdate=true;
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
	UpdateElement(doc,event);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GProfile* prof, tEvent event)
{
	if((!Memory)||(ObjsType!=otProfile))
		return;
//	cout<<"Profile "<<prof->GetId()<<" : "<<GetEvent(event)<<endl;
	UpdateElement(prof,event);	
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GGroup* grp, tEvent event)
{
	if((!Memory)||(ObjsType!=otGroup))
		return;
	UpdateElement(grp,event);
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
	delete Data;
	GSession::DeleteHandler(this);
}
