/*

	GALILEI Research Project

	PointOfView.cpp

	Compare a ideal groupement with a computed one - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Stphane Rideau (srideau@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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



// ----------------------------------------------------------------------------
//include file for GALILEI
#include <postgroups/gexpertkwds.h>
//#include <sessions/gsessionmysql.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
#include <profiles/gprofdoc.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofilevector.h>
#include <langs/glang.h>
#include <langs/gdict.h>
#include <docs/gdocvector.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GExpertKwds
//
//-----------------------------------------------------------------------------

                       
// ----------------------------------------------------------------------------
GALILEI::GExpertKwds::GExpertKwds(void) throw(bad_alloc)
{
 //  GCpovs = new RContainer<GCpov,unsigned int,false,true>(10,5);
 // Id = 0;
}


// ----------------------------------------------------------------------------
GALILEI::GExpertKwds::GExpertKwds(unsigned ident) throw(bad_alloc)
{
	Id = ident;
}  



// ----------------------------------------------------------------------------
int GALILEI::GExpertKwds::Compare(const GExpertKwds* n) const
{
	return(Id-(n->GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::GExpertKwds::Compare(const GExpertKwds& n) const
{
	return(Id-(n.GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::GExpertKwds::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
unsigned GALILEI::GExpertKwds::GetId() const
{
	  return(Id);
}


//-----------------------------------------------------------------------------
void GALILEI::GExpertKwds::SetId(unsigned int ident)
{
	Id = ident;
}


//-----------------------------------------------------------------------------
//void GALILEI::GExpertKwds::AddPov(GCpov* pov)
//{
//	  GCpovs->InsertPtr(pov);
//}




