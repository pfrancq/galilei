/*

	GALILEI Research Project

	GCommunity.h

	Community - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GCommunityH
#define GCommunityH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggroup.h>
#include <gprofile.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a virtual community, i.e. a group of profiles.
* @author Pascal Francq
* @short Community.
*/
class GCommunity : public GGroup<GProfile,GCommunity,otCommunity>
{
public:

	/**
	* Construct a new community group.
	* @param name            Name of the community.
	*/
	GCommunity(const R::RString& name);

	/**
	* Construct a community with a specific identifier.
	* @param id              Identifier.
	* @param name            Name of the community.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	* @param size           Size of the vector.
	*/
	GCommunity(size_t id,const R::RString& name,const R::RDate& u,const R::RDate& c,size_t size);

	/**
	 * Get the similarity measure that must be used when computing the
	 * similarity between the profiles grouped.
	 */
	 virtual GMeasure* GetSimMeasure(void) const
	 	{return(GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities"));}

private:

	/**
	* Static function used to ordered by similarity.
	*/
	static int sortOrder(const void *a,const void *b);

public:

	/**
	* Construct the list of all feedbacks of the profiles of a group not
	* already assessed by a given profile. If a document is assessed multiple
	* times differently, most important OK>N>KO>H.
	* @param docs            Documents not assessed.
	* @param prof            Profile.
	*/
	void NotJudgedDocsList(R::RContainer<GFdbk,false,true>& docs, GProfile* prof) const;

	/**
	* Construct the list of all relevant documents of the profiles of a
	* group not already assessed by a given profile and ordered in descending
	* order of their similarity with the chosen profile.
	* @param measure         The measure used to compute the similarities.
	* @param docs            Documents not assessed.
	* @param prof            Profile.
	* @param session         Session.
	* \warning This method uses an internal container which is not optimal.
	*/
	void NotJudgedDocsRelList(GMeasure* measure,R::RContainer<GFdbk,false,false>& docs, GProfile* prof,GSession* session) const;

	/**
	* Destructor of a group.
	*/
	virtual ~GCommunity(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
