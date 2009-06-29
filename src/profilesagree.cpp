/*

	GALILEI Research Project

	ProfilesAgree.cpp

	Agreement between profiles - Implementation.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gmatrixmeasure.h>
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
class GProfilesAgreement : public GMatrixMeasure
{
	size_t MinDocs;
public:
	GProfilesAgreement(GFactoryMeasure* fac);
	virtual R::RCString GetClassName(void) const {return("GProfilesAgreement");}
	virtual void ApplyConfig(void);
	double Compute(void* obj1,void* obj2);
	static void CreateParams(RConfig* params);
};


//------------------------------------------------------------------------------
GProfilesAgreement::GProfilesAgreement(GFactoryMeasure* fac)
	: GMatrixMeasure(fac,otProfile,otProfile,true)
{
}


//------------------------------------------------------------------------------
void GProfilesAgreement::ApplyConfig(void)
{
	GMatrixMeasure::ApplyConfig();
	MinDocs=Factory->GetUInt("MinDocs");
}


//------------------------------------------------------------------------------
double GProfilesAgreement::Compute(void* obj1,void* obj2)
{
	double nbcommon=static_cast<double>(static_cast<GProfile*>(obj1)->GetCommonDocs(static_cast<GProfile*>(obj2)));
	if(nbcommon<MinDocs)
		return(0.0);
	size_t nbsame=static_cast<GProfile*>(obj1)->GetCommonOKDocs(static_cast<GProfile*>(obj2));
	return(static_cast<double>(nbsame)/nbcommon);
}


//------------------------------------------------------------------------------
void GProfilesAgreement::CreateParams(RConfig* params)
{
	GMatrixMeasure::CreateParams(params);
	params->InsertParam(new RParamValue("MinDocs",7));
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles Agreements","Count Method",GProfilesAgreement)
