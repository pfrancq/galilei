/*

	GALILEI Research Project

	GProfilesGeneric.cpp

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
#include <gprofilesagree.h>
#include <gsubprofile.h>
#include <gprofile.h>
#include <glang.h>
#include <gsession.h>
#include <ggalileiapp.h>



//------------------------------------------------------------------------------
//
//  GProfilesGeneric
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesGeneric::GProfilesGeneric(GFactoryMeasure* fac)
	: GMeasure(fac), GSignalHandler(), NullSimLevel(0.000001), MinAgreement(0.5),
	  MinDocs(10), Memory(true), Values(0), NbLines(0), NeedUpdate(false), Created(5000), Modified(5000), Deleted(5000)
{
	GSession::AddHandler(this);
}


//-----------------------------------------------------------------------------
void GProfilesGeneric::ApplyConfig(void)
{
	MinDocs=Factory->GetUInt("MinDocs");
	NullSimLevel=Factory->GetDouble("NullSimLevel");
	MinAgreement=Factory->GetDouble("MinAgreement");
	Memory=Factory->GetBool("Memory");
}


//------------------------------------------------------------------------------
void GProfilesGeneric::Update(void)
{
	// if memory is false, no update is needed
	// since sims are claulctaed each time
	if(!Session)
		return;

	// Add the lines (if necessary)
	size_t MaxLines=Session->GetMaxProfileId()-1;
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

		GProfile* prof1=Session->GetProfile(i+2,false);
		if(!prof1)
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
					GProfile* prof2=Session->GetProfile(j+1,false);
					if(!prof2)
						continue;
					double val=Compute(prof1,prof2);
					if(val<NullSimLevel)
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
				GProfile* prof2=Session->GetProfile(j+1,false);
				if(!prof2)
					continue;
				double val=Compute(prof1,prof2);
				if(val<NullSimLevel)
					val=0.0;
				(*vals)=val;
			}

			if(Created.IsIn(j+1))
			{
				GProfile* prof2=Session->GetProfile(j+1,false);
				if(!prof2)
					continue;
				double val=Compute(prof1,prof2);
				if(val<NullSimLevel)
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
void GProfilesGeneric::Measure(unsigned int measure,...)
{
	va_list ap;
	va_start(ap,measure);
	size_t id1=va_arg(ap,size_t);
	size_t id2=va_arg(ap,size_t);
	double* res=va_arg(ap,double*);
	
	// If same profile -> return 1
	if(id1==id2)
	{
		(*res)=1.0;
		va_end(ap);
		return;
	}
	
	if(!Memory)
	{
		GProfile* prof1=Session->GetProfile(id1,false);
		if(prof1)
		{
			GProfile* prof2=Session->GetProfile(id2,false);
			if(prof2)
				(*res)=Compute(prof1,prof2);
			else
				(*res)=0.0;
		}
		else
			(*res)=0.0;
		va_end(ap);
		return;		
	}
	
	
	if(NeedUpdate)
		Update();

	if(id1<id2)
	{
		size_t tmp=id1;
		id1=id2;
		id2=tmp;
	}
	(*res)=Values[id1-2][id2-1];
	va_end(ap);
}


//------------------------------------------------------------------------------
void GProfilesGeneric::Event(GProfile* prof, tEvent event)
{
	switch(event)
	{
		case eObjNew:
			Created.Insert(prof->GetId());
			if(Deleted.IsIn(prof->GetId()))
				Deleted.Delete(prof->GetId());
			if(Modified.IsIn(prof->GetId()))
				Modified.Delete(prof->GetId());
			NeedUpdate=true;
			break;
		case eObjModified:
			if(!Created.IsIn(prof->GetId()))
				Modified.Insert(prof->GetId());
			if(Deleted.IsIn(prof->GetId()))
				Deleted.Delete(prof->GetId());
			NeedUpdate=true;
			break;
		case eObjDelete:
			if(Created.IsIn(prof->GetId()))
				Created.Delete(prof->GetId());
			if(Modified.IsIn(prof->GetId()))
				Modified.Delete(prof->GetId());
			Deleted.Insert(prof->GetId());
			NeedUpdate=true;
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GProfilesGeneric::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("MinDocs",7));
	params->InsertParam(new RParamValue("NullSimLevel",0.00001));
	params->InsertParam(new RParamValue("MinAgreement",0.6));
	params->InsertParam(new RParamValue("Memory",true));
}


//------------------------------------------------------------------------------
GProfilesGeneric::~GProfilesGeneric(void)
{
	if(Values)
	{
		double** tmp;
		size_t i;
		for(tmp=Values,i=NbLines+1;--i;tmp++)
			delete[] (*tmp);
		delete[] Values;
	}
	
	GSession::DeleteHandler(this);
}
