/*

	GALILEI Research Project

	GLangData.h

	Language Specific Data - Header.

	Copyright 2007 by the Université libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GLangData_H
#define GLangData_H


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GLangData class provides a representation for a set of elements
 * (documents, subprofiles and groups) related to a specific language. Each
 * language has its own numbering for the subprofiles and the groups.
 * @author Pascal Francq
 * @short Language Specific Data
 */
class GLangData
{
	struct Intern;
	
	/*
	 * Internal data.
	 */
	Intern* Data;
	
	/**
	 * Language.
	 */
	GLang* Lang;
	
public:

	/**
	 * Constructor.
	 * @param lang           Language.
	 * @param nbsub          Number of subprofiles.
	 */
	GLangData(GLang* lang);
	
	/**
	 * Compare method.
	 * @param specifc        Language-specific structure.
	 */
	int Compare(const GLangData& specific) const;
	
	/**
	 * Compare method.
	 * @param lang           Language.
	 */
	int Compare(const GLang* lang) const;
	
	/**
	 * Compare method.
	 * @param code           Code of the language.
	 */
	int Compare(const char* code) const;

	/**
	 * Clear language-dependent data.
	 * @param type           Type of the data the clear.
	 */
	void Clear(tObjType type);
	
	/**
	 * Get a pointer to the language corresponding to this group of data.
	 */
	GLang* GetLang(void) const {return(Lang);}
	
	/**
	 * Get a cursor ovet the documents of a particular language.
	 * @return RCursor<GDoc>
	 */
	R::RCursor<GDoc> GetDocs(void) const;
	
	/**
	 * Insert a document.
	 * @param doc            Document to insert.
	 */
	void InsertDoc(GDoc* doc);
	
	/**
	 * Delete a document.
	 * @param doc            Document to insert.
	 */
	void DeleteDoc(GDoc* doc);
	
	/**
	 * Get the number of documents of a particular language.
	 */
	size_t GetNbDocs(void) const;
	
	/**
	 * Get a cursor over the subprofiles of a particular language.
	 * @return RCursor<GSubProfile>
	 */
	R::RCursor<GSubProfile> GetSubProfiles(void) const;
	
	/**
	 * Get the number of subprofiles of a particular language.
	 */
	size_t GetNbSubProfiles(void) const;

	/**
	* Get the highest identificator assign to a profile.
	*/
	size_t GetMaxSubProfileId(void) const;
	
	/**
	 * Get a pointer to a subprofile having a given identificator.
	 * @param id             Identificator.
	 * @return A valid pointer or null if the identificator is not assigned.
	 */
	GSubProfile* GetSubProfile(unsigned int id) const;
	
	/**
	 * Assign a identificator to the subprofile based on the subprofiles
	 * already in memory.
	 * @param sub            Subprofile.
	 */
	void AssignId(GSubProfile* sub) const;
	
	/**
	 * Insert a given subprofile. The subprofile must have a valid
	 * identificator.
	 * @param sub            Subprofile.
	 */
	void InsertSubProfile(GSubProfile* sub);

	/**
	* Get a cursor on all the groups.
	* @return GGroupCursor.
	*/
	R::RCursor<GGroup> GetGroups(void) const;

	/**
	* Get the number of groups handled.
	* @returns Number of groups.
	*/
	size_t GetNbGroups(void) const;

	/**
	 * Get a pointer to a group having a given identificator.
	 * @param id             Identificator.
	 * @return A valid pointer or null if the identificator is not assigned.
	 */
	GGroup* GetGroup(unsigned int id) const;
	
	/**
	 * Assign a identificator to the group based on the groups already in
	 * memory.
	 * @param grp            Group.
	 */ 
	void AssignId(GGroup* grp);
	
	/**
	 * Insert a group.
	 * @param group          Group.
	 */	 
	void InsertGroup(GGroup* grp);
	
	/**
	 * Delete a group.
	 * @param group          Group.
	 */	 
	void DeleteGroup(GGroup* grp);
		
	/**
	 * Destructor.
	 */
	virtual ~GLangData(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
