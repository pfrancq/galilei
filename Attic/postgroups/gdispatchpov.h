/*

	GALILEI Research Project

	PointOfView.h

	Document representing a GALILEI session - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
    Stephane Rideau (srideau@ulb.ac.be).

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



//-----------------------------------------------------------------------------
#ifndef GDispatchPovH
#define GDispatchPovH

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/ggroup.h>
#include <sessions/gsession.h>
#include <sessions/gsessionmysql.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
#include <postgroups/ginoutputbase.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
//  class dispatch : main class for the Point of view
//
//-----------------------------------------------------------------------------
class GDispatchpov : public GSession, public GInOutputBase
{

public :


	/**
	*
	*/
	GDispatchpov(R::RContainer<GGroups,unsigned int,true,true>* groups  )throw(bad_alloc);

 	/**
	*
	*/
	GDispatchpov(R::RContainer<GGroup,unsigned int,true,true>* group ) throw(bad_alloc);

  	/**
	*
	*/
	GDispatchpov(GSubProfile* profile  )throw(bad_alloc);

 	/**
	*
	*/
	GDispatchpov( ) throw(bad_alloc);    

	/**
	*
	*/
	void GeneratePoV(R::RContainer<GGroups,unsigned int,true,true>* groups);

	/**
	* build the hierarchy of expert kwds for all the profiles af all the group
	*/
	void GeneratePoVfromScratch(GGroup* grp, GSession *s);

	/**
	* Update the hierarchy of expert kwds for all the profiles af all the groups
	*/
	void UpdatePovForAPrf(GSubProfile *prfle, GInOutputBase *datainput, GSession *s);

};



}//-------- End of namespace GALILEI----------------------------------------


#endif



