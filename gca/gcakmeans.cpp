/*

	GALILEI Research Project

	GCA.cpp

	Main - Implementation.

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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>
#include <gprofile.h>
#include <guser.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gmeasure.h>


//-----------------------------------------------------------------------------
// includes files for GCA
#include <gcakmeans.h>
using namespace R;




//-----------------------------------------------------------------------------
//
// class kMeansDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
kMeansDoc::kMeansDoc(const R::RString& n,R::RRandom* r,R::RCursor<GDoc> objs,R::RDebug* debug)
	: R::RGroupingKMeans<CGroupDoc,GDoc,CGroupsDoc>(n,r,objs,debug)
{
	Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Documents Similarities");
}


//-----------------------------------------------------------------------------
double kMeansDoc::Similarity(const GDoc* obj1,const GDoc* obj2)
{
	double d;
	Measure->Measure(0,obj1->GetId(),obj2->GetId(),&d);
	return(d);
}



//-----------------------------------------------------------------------------
//
// class kMeansProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
kMeansProfile::kMeansProfile(const R::RString& n,R::RRandom* r,R::RCursor<GProfile> objs,R::RDebug* debug)
	: R::RGroupingKMeans<CGroupProfile,GProfile,CGroupsProfile>(n,r,objs,debug)
{
	Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profiles Similarities");
}


//-----------------------------------------------------------------------------
double kMeansProfile::Similarity(const GProfile* obj1,const GProfile* obj2)
{
	double d;
	Measure->Measure(0,obj1->GetId(),obj2->GetId(),&d);
	return(d);
}
