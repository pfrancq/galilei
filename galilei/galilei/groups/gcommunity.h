/*

	GALILEI Research Project

	GCommunity.h

	Community - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
#include <gsugs.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a virtual community, i.e. a group of profiles.
* @short Community.
*/
class GCommunity : public GGroup<GProfile,GCommunity,otCommunity>
{
public:

	/**
	* Construct a new community group.
	* @param session         Session.
	* @param name            Name of the community.
	*/
	GCommunity(GSession* session,const R::RString& name);

	/**
	* Construct a community with a specific identifier.
	* @param session         Session.
	* @param id              Identifier.
	* @param blockid         Identifier of the block.
	* @param name            Name of the community.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	*/
	GCommunity(GSession* session,size_t id,size_t blockid,const R::RString& name,const R::RDate& u,const R::RDate& c);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GCommunity");}

	/**
	 * Give the type of the object.
    * @return otCommunity
    */
	static inline tObjType GetType(void) {return(otCommunity);}

	/**
	 * Get the similarity measure that must be used when computing the
	 * similarity between the profiles grouped.
	 */
	 virtual GMeasure* GetSimMeasure(void) const
	 	{return(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profile Similarities"));}

	/**
	 * Get the list of relevant documents, i.e. documents assessed as relevant
	 * by at least one profile of the community. All the documents have a null
	 * ranking.
	 *
	 * If a document is assessed one time as relevant and multiple times as
	 * irrelevant, it is also added to the list.
	 * @param docs           List that will be filled (and cleared before).
	 */
	void GetRelevantDocs(GCommunityDocs& docs);

	/**
	* Update the representation of the community once a computation was done. The
	* computed date and the status are updated.
	*
	* If the community is an internal one, the following steps are done:
	* -# An 'hCommunities[oeAboutToBeUpdated]' notification is send.
	* -# The references are updated.
	* -# Existing in memory description is replaced.
	* -# If necessary, the community and its description are saved.
	* -# An 'hCommunities[oeUpdated]' notification is send.
	*
	* If the community is an external one, its description is replaced.
	* @param desc            Description to assign.
	* \warning The description is cleared by this method.
	*/
	void Update(GDescription& desc);

	/**
	* Destructor of a group.
	*/
	virtual ~GCommunity(void);
};


//------------------------------------------------------------------------------
/**
 * The GCommunityDocs class provides a representation for a set of documents
 * assessed as relevant by the members of a community.
 */
class GCommunityDocs : public R::RContainer<GDocFragmentRank,true,false>
{
	/**
	 * Identifier of the community.
	 */
	size_t CommunityId;

public:

	/**
	 * Create a list of relevant documents.
	 * @param size           Initial size of the list.
	 * @param communityid    Identifier of the community.
	 */
	GCommunityDocs(size_t size=0,size_t communityid=0);

	/**
	 * Assign a new community identifier and clear the list.
	 * @param communityid    Identifier of the community.
	 */
	void SetCommunityId(size_t communityid);

	/**
	 * @return the identifier of the community.
	 */
	size_t GetCommunityId(void) const {return(CommunityId);}

	friend class GObjects<GCommunity,hCommunities>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
