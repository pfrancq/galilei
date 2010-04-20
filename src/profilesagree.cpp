/*

	GALILEI Research Project

	ProfilesAgree.cpp

	Agreement between profiles - Implementation.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
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
	GProfilesAgreement(GSession* session,GPlugInFactory* fac);
	virtual R::RCString GetClassName(void) const {return("GProfilesAgreement");}
	virtual void ApplyConfig(void);
	double Compute(void* obj1,void* obj2);
	virtual size_t GetId(void* obj,bool)
	{
		return(static_cast<GWeightInfosObj*>(obj)->GetId());
	}
	static void CreateParams(GPlugInFactory* fac);
};


//------------------------------------------------------------------------------
GProfilesAgreement::GProfilesAgreement(GSession* session,GPlugInFactory* fac)
	: GMatrixMeasure(session,fac,otProfile,otProfile,true)
{
}


//------------------------------------------------------------------------------
void GProfilesAgreement::ApplyConfig(void)
{
	GMatrixMeasure::ApplyConfig();
	MinDocs=Factory->FindParam<RParamValue>("MinDocs")->GetUInt();
}


//------------------------------------------------------------------------------
double GProfilesAgreement::Compute(void* obj1,void* obj2)
{
	return(static_cast<GProfile*>(obj1)->GetAgreementRatio(static_cast<GProfile*>(obj2),MinDocs));
}


//------------------------------------------------------------------------------
void GProfilesAgreement::CreateParams(GPlugInFactory* fac)
{
	GMatrixMeasure::CreateParams(fac);
	fac->InsertParam(new RParamValue("MinDocs",7));
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles Agreements","Count Method","Count Method",GProfilesAgreement)
