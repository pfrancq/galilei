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
// class GMeasure2Elements::Measure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GMeasure2Elements::Measures
{
public:
	double* Values;      // Values
	
	Measures(size_t max) : Values(0) {Values=new double[max];}	
	int Compare(const Measures&) const {return(-1);}	
	~Measures(void) {delete[] Values;}
};



//------------------------------------------------------------------------------
//
//  GMeasure2Elements
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMeasure2Elements::GMeasure2Elements(GFactoryMeasure* fac,bool sym,tObjType type)
	: GMeasure(fac), GSignalHandler(), Values(30), Symetric(sym),
	  NullLevel(0.000001), DirtyValue(-2.0), MinMeasure(0.5), AutomaticMinMeasure(true), 
	  MinMeasureSense(true), InMemory(true), InFile(false), Lines(type), Cols(type),
	  Mean(0.0), Deviation(0.0), NbValues(0)
{
	GSession::AddHandler(this);
}


//------------------------------------------------------------------------------
GMeasure2Elements::GMeasure2Elements(GFactoryMeasure* fac,tObjType lines,tObjType cols)
	: GMeasure(fac), GSignalHandler(), Values(30), Symetric(false),
	  NullLevel(0.000001), DirtyValue(-2.0), MinMeasure(0.5), AutomaticMinMeasure(true), 
	  MinMeasureSense(true), InMemory(true), InFile(false), Lines(lines), Cols(cols),
	  Mean(0.0), Deviation(0.0), NbValues(0)
{
	GSession::AddHandler(this);
}


//-----------------------------------------------------------------------------
void GMeasure2Elements::SetElementsType(bool sym,tObjType lines,tObjType cols)
{
	if(sym&&(lines!=cols))
		throw GException("Symetric measures are only allowed if the elements are of the same type");
	Symetric=sym;
	Lines=lines;
	Cols=cols;
}


//-----------------------------------------------------------------------------
void GMeasure2Elements::ApplyConfig(void)
{
	NullLevel=Factory->GetDouble("NullLevel");
	MinMeasure=Factory->GetDouble("MinMeasure");
	AutomaticMinMeasure=Factory->GetBool("AutomaticMinMeasure");
	InMemory=Factory->GetBool("Memory");
	InFile=Factory->GetBool("File");
	Dir=Factory->Get("Dir");
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Connect(GSession* session)
{
	GMeasure::Connect(session);
	size_t nb=Session->GetMaxElementId(Lines);
	if(!nb)
		nb=500;
	Values.VerifyTab(nb);
	NbValues=0;
	Mean=Deviation=0.0;
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Disconnect(GSession* session)
{
	Values.Clear();
	NbValues=0;
	GMeasure::Disconnect(session);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Dirty(void)
{
	Values.Clear();
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Measure(unsigned int measure,...)
{
	va_list ap;
	va_start(ap,measure);
	unsigned int id1=va_arg(ap,unsigned int);
	unsigned int id2=va_arg(ap,unsigned int);
	size_t idx1,idx2;
	double* res=va_arg(ap,double*);
	va_end(ap);
	
	if(InMemory) // Search in memory
	{
		// Check order of identifier and get the value (If Symetric)
		if((Symetric)&&(id1<id2))
		{
			size_t tmp=id1;
			id1=id2;
			id2=tmp;
		}
		idx1=id1-1; // First line is empty (id1==1 -> Line=0)
		idx2=id2-1; // First column is id2=1 -> Col=0
		(*res)=Values[idx1]->Values[idx2];		
		
		// Recomputing it necessary
		if((*res)==DirtyValue)
		{
			void* obj1=Session->GetElement(Lines,id1);
			if(obj1)
			{
				void* obj2=Session->GetElement(Cols,id2);
				if(obj2)
					(*res)=Compute(obj1,obj2);
			}
			//cout<<"Compute sim("<<id1<<","<<id2<<")="<<(*res)<<endl;
			Values[idx1]->Values[idx2]=(*res);
			AddValue(*res);
		}
		//cout<<"Return sim("<<id1<<","<<id2<<")="<<(*res)<<endl;
	}
	else if(InFile) // Search in binary file
	{		
		RToImplement()
	}
	else  // Recompute it
	{
		void* obj1=Session->GetElement(Lines,id1);
		if(obj1)
		{
			void* obj2=Session->GetElement(Cols,id2);
			if(obj2)
				(*res)=Compute(obj1,obj2);
			else
				(*res)=0.0;
		}
		else
			(*res)=0.0;		
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Info(unsigned int info,...)
{
	if(!MinMeasureSense)
		return;
	
	va_list ap;
	va_start(ap,info);
	double* res=va_arg(ap,double*);
	va_end(ap);	
	double deviationrate=1.5;

	if(!AutomaticMinMeasure)
	{
		(*res)=MinMeasure;
		return;
	}

	if(InMemory)
	{
		Update();
		(*res)=Mean+deviationrate*sqrt(Deviation);		
	}
	else if(InFile)
	{
	}
	else
	{
		size_t max=Session->GetMaxElementId(Lines);
		if(!max)
		{
			(*res)=0.0;
			return;
		}
		
		double NbComp=0.0;
		double sim;
			
		// Go thourgh all elements
		for(size_t i=0;i<max-1;i++)
		{
			void* obj1=Session->GetElement(Lines,i+2);
			if(!obj1)
				continue;			
			
			for(size_t j=0;j<i+1;j++)
			{
				void* obj2=Session->GetElement(Cols,j+1);
				if(!obj2)
					continue;			
				NbComp+=1.0;
				sim=Compute(obj1,obj2);
				if(sim<NullLevel)
					sim=0.0;
				Mean+=sim;
				Deviation+=sim*sim;
			}
		}
		if(NbComp>0.0)
		{
			Mean/=NbComp;
			Deviation=(Deviation/NbComp)-(Mean*Mean);
		}
		(*res)=Mean+deviationrate*sqrt(Deviation);
	}
}


//------------------------------------------------------------------------------
size_t GMeasure2Elements::GetNbDiffElements(void)
{
	if(Lines==Cols)
		return(Session->GetNbElements(Lines));	
	return(Session->GetNbElements(Lines)+Session->GetNbElements(Cols));
}


//------------------------------------------------------------------------------
void GMeasure2Elements::AddIdentificator(size_t id,bool line)
{
	if(line)
	{
		// Verify that it does not exits
		if((Values.GetNb()>id-1)&&(Values[id-1]))
		{
			char tmp[80];
			sprintf(tmp,"GMeasure2Elements::AddIdentificator : index %zu exists",id);
			throw std::range_error(tmp);
		}		
			
		// Create the line
		size_t max;
		if(Symetric)
			max=id;
		else			
			max=Session->GetMaxElementId(Cols);
		Measures* ptr=new Measures(max);
		Values.InsertPtrAt(ptr,id-1);
	
		// Dirty the cols
		double* col;
		size_t j;
		for(j=max+1,col=ptr->Values;--j;col++)
			(*col)=DirtyValue;
	}
	else
	{
		if(Symetric)
		{
			// Is there something to update?
			if(Values.GetMaxPos()<id)
				return;						
			RCursor<Measures> Lines(Values);
			for(Lines.GoTo(id);!Lines.End();Lines.Next())
				Lines()->Values[id-1]=DirtyValue;
		}
		else
		{		
			RCursor<Measures> Lines(Values);
			for(Lines.Start();!Lines.End();Lines.Next())
				Lines()->Values[id-1]=DirtyValue;					
		}
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::DirtyIdentificator(size_t id,bool line)
{	
	// Something to dirty
	if(!Values.GetNb())
		return;
	
	if(line)
	{
		Measures* ptr=(Values)[id-1];
		if(!ptr)
		{
			char tmp[80];
			sprintf(tmp,"GMeasure2Elements::DirtyIdentificator : index %zu doesn't exist",id);
			throw std::range_error(tmp);
		}
				
		// Dirty the cols
		size_t max;
		if(Symetric)
			max=id;
		else			
			max=Session->GetMaxElementId(Cols);
		double* col;
		size_t j;
		for(j=max+1,col=ptr->Values;--j;col++)
			DeleteValue(*col);
	}
	else
	{
		// Goes for the rest of lines
		if(Symetric)
		{
			// Is there something to update?
			if(Values.GetMaxPos()<id)
				return;				
			RCursor<Measures> Lines(Values);
			for(Lines.GoTo(id);!Lines.End();Lines.Next())
				DeleteValue(Lines()->Values[id-1]);
		}
		else
		{
			RCursor<Measures> Lines(Values);
			for(Lines.Start();!Lines.End();Lines.Next())
				DeleteValue(Lines()->Values[id-1]);
		}
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::DeleteIdentificator(size_t id,bool line)
{
	// Something to delete
	if(!Values.GetNb())
		return;
	
	if(line)
	{
		Measures* ptr=(Values)[id-1];
		if(!ptr)
		{
			char tmp[80];
			sprintf(tmp,"GMeasure2Elements::DeleteIdentificator : index %zu doesn't exist",id);
			throw std::range_error(tmp);
		}
		
		// Dirty the cols
		size_t max;
		if(Symetric)
			max=id;
		else			
			max=Session->GetMaxElementId(Cols);
		double* col;
		size_t j;
		for(j=max+1,col=ptr->Values;--j;col++)
			DeleteValue(*col);
		
		// Delete the line
		Values.DeletePtrAt(id-1,false);
	}
	else
	{
		// Goes for the rest of lines
		if(Symetric)
		{
			// Is there something to update?
			if(Values.GetMaxPos()<id)
				return;						
			RCursor<Measures> Lines(Values);
			for(Lines.GoTo(id);!Lines.End();Lines.Next())
				DeleteValue(Lines()->Values[id-1]);
		}
		else
		{
			RCursor<Measures> Lines(Values);
			for(Lines.Start();!Lines.End();Lines.Next())
				DeleteValue(Lines()->Values[id-1]);					
		}
	}
}


//------------------------------------------------------------------------------
template<class C>
	void GMeasure2Elements::UpdateElement(C* element,tEvent event,bool line)
{
	switch(event)
	{
		case eObjNew:
			AddIdentificator(element->GetId(),line);
			break;
		case eObjModified:
			DirtyIdentificator(element->GetId(),line);
			break;
		case eObjDelete:
			DeleteIdentificator(element->GetId(),line);
			break;
		default:
			break;
	}	
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GDoc* doc, tEvent event)
{
	if(InMemory)
	{
		if(Lines==otDoc)
			UpdateElement(doc,event,true);
		if(Cols==otDoc)
			UpdateElement(doc,event,false);		
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GProfile* prof, tEvent event)
{
	if(InMemory)
	{
		if(Lines==otProfile)
			UpdateElement(prof,event,true);
		if(Cols==otProfile)
			UpdateElement(prof,event,false);
		//	cout<<"Profile "<<prof->GetId()<<" : "<<GetEvent(event)<<" ("<<GetPlugInName()<<")"<<endl;
	}	
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GCommunity* community, tEvent event)
{
	if(InMemory)
	{
		if(Lines==otCommunity)
			UpdateElement(community,event,true);
		if(Cols==otCommunity)
			UpdateElement(community,event,false);		
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Update(void)
{
	if(Symetric)
	{
		size_t max,i,j;
		double* vals;
		RCursor<Measures> Rows(Values);
		for(Rows.Start(),i=1;!Rows.End();Rows.Next(),max++,i++)
		{
			void* obj1=Session->GetElement(Lines,i);
			for(j=1,vals=Rows()->Values;j<i;j++,vals++)
			{
				if((*vals)!=DirtyValue)
					continue;
				void* obj2=Session->GetElement(Cols,j);
				(*vals)=Compute(obj1,obj2);
				AddValue(*vals);
			}
		}
	}
	else
	{
		size_t max=Session->GetMaxElementId(Cols),i,j;
		double* vals;
		RCursor<Measures> Rows(Values);
		for(Rows.Start(),i=1;!Rows.End();Rows.Next(),max++,i++)
		{
			void* obj1=Session->GetElement(Lines,i);
			for(j=1,vals=Rows()->Values;j<max;j++,vals++)
			{
				if((*vals)!=DirtyValue)
					continue;			
				void* obj2=Session->GetElement(Cols,j);
				(*vals)=Compute(obj1,obj2);
				AddValue(*vals);
			}
		}		
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::AddValue(double val)
{
	if(val==DirtyValue)
		return;
	double oldmean(Mean);
	Mean=(NbValues*Mean+val)/(NbValues+1);
	Deviation=(((Deviation+(oldmean*oldmean))*NbValues)+(val*val))/(NbValues+1)-(Mean*Mean);
	NbValues++;	
}

	
//------------------------------------------------------------------------------
void GMeasure2Elements::DeleteValue(double& val)
{
	if(val==DirtyValue)
		return;
	if(NbValues==1)
	{
		NbValues=0;
		Mean=Deviation=0.0;
	}
	else
	{
		double oldmean(Mean);
		Mean=(NbValues*Mean-val)/(NbValues-1);
		Deviation=(((Deviation+(oldmean*oldmean))*NbValues)-(val*val))/(NbValues-1)-(Mean*Mean);
		NbValues--;
	}
	val=DirtyValue;
}


//------------------------------------------------------------------------------
void GMeasure2Elements::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NullLevel",0.00001));
	params->InsertParam(new RParamValue("MinMeasure",0.05));
	params->InsertParam(new RParamValue("AutomaticMinMeasure",true));
	params->InsertParam(new RParamValue("Memory",true));
	params->InsertParam(new RParamValue("File",false));
	params->InsertParam(new RParamValue("Dir","/var/galilei"));
}


//------------------------------------------------------------------------------
GMeasure2Elements::~GMeasure2Elements(void)
{
	GSession::DeleteHandler(this);
}
