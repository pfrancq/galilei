/*

	GALILEI Research Project

	GDocsAgreement.cpp

	Agreement between docs - Implementation.

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
#include <gdoc.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GDocsAgreement
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocsAgreement : public GMeasure2Elements
{
	size_t MinProfiles;
public:
	GDocsAgreement(GFactoryMeasure* fac);
	virtual void ApplyConfig(void);
	double Compute(void* obj1,void* obj2);
	static void CreateParams(RConfig* params);
};


//------------------------------------------------------------------------------
GDocsAgreement::GDocsAgreement(GFactoryMeasure* fac)
	: GMeasure2Elements(fac,otDoc,otDoc,true)
{
}


//------------------------------------------------------------------------------
void GDocsAgreement::ApplyConfig(void)
{
	GMeasure2Elements::ApplyConfig();
	MinProfiles=Factory->GetUInt("MinProfiles");
}


//------------------------------------------------------------------------------
double GDocsAgreement::Compute(void* obj1,void* obj2)
{
	double nbcommon=double(static_cast<GDoc*>(obj1)->GetCommonProfiles(static_cast<GDoc*>(obj2)));
	if(nbcommon<MinProfiles)
		return(0.0);
	size_t nbsame=static_cast<GDoc*>(obj1)->GetCommonOKProfiles(static_cast<GDoc*>(obj2));
	return(nbsame/nbcommon);
}


//------------------------------------------------------------------------------
void GDocsAgreement::CreateParams(RConfig* params)
{
	GMeasure2Elements::CreateParams(params);
	params->InsertParam(new RParamValue("MinProfiles",7));
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Documents Agreements","Count Method",GDocsAgreement)
