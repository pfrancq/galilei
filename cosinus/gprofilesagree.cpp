/*

	GALILEI Research Project

	GProfilesAgreement.cpp

	Agreement between profiles - Implementation.

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
// include files for GALILEI
#include <gmeasure2elements.h>
#include <gprofile.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GProfilesAgreement
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GProfilesAgreement : public GMeasure2Elements
{
	size_t MinDocs;
public:
	GProfilesAgreement(GFactoryMeasure* fac);
	virtual void ApplyConfig(void);
	double Compute(void* obj1,void* obj2);
	void* GetElement(size_t id);
	size_t GetMaxElementsId(void);
	size_t GetNbElements(void);
	static void CreateParams(RConfig* params);	
};


//------------------------------------------------------------------------------
GProfilesAgreement::GProfilesAgreement(GFactoryMeasure* fac)
	: GMeasure2Elements(fac,false,1.0,otProfile)
{
}


//------------------------------------------------------------------------------
void GProfilesAgreement::ApplyConfig(void)
{
	GMeasure2Elements::ApplyConfig();
	MinDocs=Factory->GetUInt("MinDocs");
}


//------------------------------------------------------------------------------
double GProfilesAgreement::Compute(void* obj1,void* obj2)
{
	double nbcommon=double(static_cast<GProfile*>(obj1)->GetCommonDocs(static_cast<GProfile*>(obj2)));
	if(nbcommon<MinDocs)
		return(0.0);
	size_t nbsame=static_cast<GProfile*>(obj1)->GetCommonOKDocs(static_cast<GProfile*>(obj2));
	return(nbsame/nbcommon);
}


//------------------------------------------------------------------------------
void* GProfilesAgreement::GetElement(size_t id)
{
	return(Session->GetProfile(id,false));
} 


//------------------------------------------------------------------------------
size_t GProfilesAgreement::GetMaxElementsId(void)
{
	return(Session->GetMaxProfileId());
}


//------------------------------------------------------------------------------
size_t  GProfilesAgreement::GetNbElements(void)
{
	return(Session->GetNbProfiles());
}


//------------------------------------------------------------------------------
void GProfilesAgreement::CreateParams(RConfig* params)
{
	GMeasure2Elements::CreateParams(params);
	params->InsertParam(new RParamValue("MinDocs",7));
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles Agreements","Count Method",GProfilesAgreement)
