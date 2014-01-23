/*

	GALILEI Research Project

	GTopic.h

	Topic - Header.

	Copyright 2008-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef GTopicH
#define GTopicH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggroup.h>
#include <gdoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a virtual Topic, i.e. a group of Docs.
* @author Pascal Francq
* @short Topic.
*/
class GTopic : public GGroup<GDoc,GTopic,otTopic>
{
public:

	/**
	* Construct a new topic.
	* @param session         Session.
	* @param name            Name of the topic.
	*/
	GTopic(GSession* session,const R::RString& name);

	/**
	* Construct a topic with a specific identifier.
	* @param session         Session.
	* @param id              Identifier.
	* @param blockid         Identifier of the block.
	* @param name            Name of the topic.
	* @param u               Date of the last updated.
	* @param c               Date of the last computation.
	*/
	GTopic(GSession* session,size_t id,size_t blockid,const R::RString& name,const R::RDate& u,const R::RDate& c);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GTopic");}

	/**
	 * Give the type of the object.
    * @return otTopic
    */
	static inline tObjType GetType(void) {return(otTopic);}

	/**
	 * Get the similarity measure that must be used when computing the
	 * similarity between the documents grouped.
	 */
	 virtual GMeasure* GetSimMeasure(void) const
	 	{return(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document Similarities"));}

	/**
	* Update the representation of the topic once a computation was done. The
	* computed date and the status are updated.
	*
	* If the topic is an internal one, the following steps are done:
	* -# An 'hTopics[oeAboutToBeUpdated]' notification is send.
	* -# The references are updated.
	* -# Existing in memory description is replaced.
	* -# If necessary, the topic and its description are saved.
	* -# An 'hTopics[oeUpdated]' notification is send.
	*
	* If the topic is an external one, its description is replaced.
	* @param desc            Description to assign.
	* \warning The description is cleared by this method.
	*/
	void Update(GDescription& desc);

	/**
	* Destructor of a group.
	*/
	virtual ~GTopic(void);

   friend class GObjects<GTopic,hTopics>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
