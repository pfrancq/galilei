/*

	GALILEI Research Project

	GDocsSims.cpp

	Similarities between subprofiles - Implementation.

	Copyright 2003-2007 by the Université Libre de Bruxelles.

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
#include <ggenericsims.h>
#include <gdoc.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GDocsSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocsSims : public GDiffSims
{
public:
	GDocsSims(GFactoryMeasure* fac);
	void* GetElement(size_t id);
	size_t GetMaxElementsId(void);
	size_t GetNbElements(void);
};


//------------------------------------------------------------------------------
GDocsSims::GDocsSims(GFactoryMeasure* fac)
	: GDiffSims(fac,true,otDoc)
{
}


//------------------------------------------------------------------------------
void* GDocsSims::GetElement(size_t id)
{
	return(Session->GetDoc(id,false));
} 


//------------------------------------------------------------------------------
size_t GDocsSims::GetMaxElementsId(void)
{
	return(Session->GetMaxDocId());
}


//------------------------------------------------------------------------------
size_t GDocsSims::GetNbElements(void)
{
	return(Session->GetMaxDocId());
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Documents Similarities","Cosinus Method",GDocsSims)
