/*

	GALILEI Research Project

	DocsDisagree.cpp

	Document Disagreements - Implementation.

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
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
#include <gdoc.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GDocsDisagreement
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocsDisagreement : public GMatrixMeasure
{
	size_t MinProfiles;
public:
	GDocsDisagreement(GSession* session,GPlugInFactory* fac);
	virtual R::RCString GetClassName(void) const {return("GDocsDisagreement");}
	virtual void ApplyConfig(void);
	double Compute(GObject* obj1,GObject* obj2);
	virtual size_t GetId(void* obj,bool)
	{
		return(static_cast<GDoc*>(obj)->GetId());
	}
	virtual void CreateConfig(void);
};


//------------------------------------------------------------------------------
GDocsDisagreement::GDocsDisagreement(GSession* session,GPlugInFactory* fac)
	: GMatrixMeasure(session,fac,otDoc,otDoc,true)
{
}


//------------------------------------------------------------------------------
void GDocsDisagreement::ApplyConfig(void)
{
	GMatrixMeasure::ApplyConfig();
	MinProfiles=FindParam<RParamValue>("MinProfiles")->GetUInt();
}


//------------------------------------------------------------------------------
double GDocsDisagreement::Compute(GObject* obj1,GObject* obj2)
{
	return(static_cast<GDoc*>(obj1)->GetDisagreementRatio(static_cast<GDoc*>(obj2),MinProfiles));
}


//------------------------------------------------------------------------------
void GDocsDisagreement::CreateConfig(void)
{
	GMatrixMeasure::CreateConfig();
	InsertParam(new RParamValue("MinProfiles",7));
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Document Disagreements","Count Method","Count Method",GDocsDisagreement)
