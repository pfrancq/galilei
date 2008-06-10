/*

	GALILEI Research Project

	GProfilesAgreement.cpp

	Agreement between profiles - Implementation.

	Copyright 2003-2008 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <gmeasure2elements.h>
#include <gprofile.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GProfilesDisagreement
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GProfilesDisagreement : public GMeasure2Elements
{
	size_t MinDocs;
public:
	GProfilesDisagreement(GFactoryMeasure* fac);
	virtual void ApplyConfig(void);
	double Compute(void* obj1,void* obj2);
	static void CreateParams(RConfig* params);	
};


//------------------------------------------------------------------------------
GProfilesDisagreement::GProfilesDisagreement(GFactoryMeasure* fac)
	: GMeasure2Elements(fac,true,otProfile)
{
}


//------------------------------------------------------------------------------
void GProfilesDisagreement::ApplyConfig(void)
{
	GMeasure2Elements::ApplyConfig();
	MinDocs=Factory->GetUInt("MinDocs");
}


//------------------------------------------------------------------------------
double GProfilesDisagreement::Compute(void* obj1,void* obj2)
{
	double nbcommon=double(static_cast<GProfile*>(obj1)->GetCommonDocs(static_cast<GProfile*>(obj2)));
	if(nbcommon<MinDocs)
		return(0.0);
	size_t nbdiff=static_cast<GProfile*>(obj1)->GetCommonDiffDocs(static_cast<GProfile*>(obj2));
	return(nbdiff/nbcommon);
}


//------------------------------------------------------------------------------
void GProfilesDisagreement::CreateParams(RConfig* params)
{
	GMeasure2Elements::CreateParams(params);
	params->InsertParam(new RParamValue("MinDocs",7));
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles Disagreements","Count Method",GProfilesDisagreement)
