/*

	GALILEI Research Project

	GSubjects.h

	Subject - Header.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2004 by Julien Lamoral.
	Copyright 2002-2004 by David Wartel.
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GSubjectH
#define GSubjectH


//------------------------------------------------------------------------------
// include files for R Project
#include <rnode.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * The GSubjectData represents a set of information related to subjects and
 * that can be associated to a group (topic or community). In particular,
 * quality measures associated with a given (computed) group in comparison with
 * the corresponding (ideal) subject.
 */
template<class cObj>
	class GSubjectData
{
	/**
	 * Centroid of a group.
	 */
	cObj* Centroid;

	/**
	 * Average similarity of the objects of the group to the centroid.
	 */
	double AvgSim;

public:

	/**
	 * Constructor.
	 */
	GSubjectData(void) : Centroid(0), AvgSim(-2.0) {}

	/**
	 * Method called each time a group is modified.
	 */
	void Dirty(void)
	{
		Centroid=0;
	}

	friend class GSubjects;
};


//------------------------------------------------------------------------------
/**
* This Class implement a representation for a subject, i.e. an ideal group.
*
* This class is used for validation purposes.
* @author Pascal Francq, Julien Lamoral and David Wartel.
* @short Subject
*/
class GSubject: public R::RNode<GSubject,true,false>
{
	/**
	 * Identifier of the subject.
	 */
	size_t Id;

	/**
	 * Name of the subject.
	 */
	R::RString Name;

	/**
	 * Determine if the subject is used.
	 */
	bool Used;

	/**
	 * Documents attached to this subject.
	 */
	R::RContainer<GDoc,false,true> AllDocs;

	/**
	 * Documents attached to this subject and selected.
	 */
	R::RContainer<GDoc,false,true> Docs;

	/**
	 * Profiles attached to this subject.
	 */
	R::RContainer<GProfile,false,true> Profiles;

	/**
	 * Ideal community.
	 */
	GCommunity* Community;

	/**
	 * Ideal topic.
	 */
	GTopic* Topic;

public:

	/**
	* Constructor of a subject.
	* @param id              Identifier of the subject.
	* @param name            Name of the subject.
	* @param u               Used?
	*/
	GSubject(size_t id,const char* name,bool u);

	/**
	* Compare two subjects by comparing their identifier.
	* @see R::RContainer
	* @param sub             Subject.
	* @return int
	*/
	int Compare(const GSubject& sub) const;

	/**
	* Compare the identifier of a subject with another one.
	* @see R::RContainer
	* @param id              Identifier.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* Compare the name of a subject with a given string.
	* @see R::RContainer
	* @param name            String.
	* @return int
	*/
	int Compare(const R::RString& name) const;

	/**
	* Get the ideal group of the profile.
	* @param prof            Profile.
	*/
	GSubject* GetIdealGroup(GProfile* prof) const;

	/**
	* Get the ideal group of the document.
	* @param doc            Document.
	*/
	GSubject* GetIdealGroup(GDoc* doc) const;

	/**
	* Verify if a profile is part of the subject.
	* @param prof            Pointer to the profile.
	*/
	bool IsIn(GProfile* prof) const;

	/**
	* Verify if a document is part of the subject.
	* @param doc             Pointer to the document.
	*/
	bool IsIn(GDoc* doc) const;

	/**
	 *
	 * @param com
	 */
	void ClearIdealGroup(tObjType type);

	/**
	* Assign an ideal community to the subject. This method can only be used
	* when the current clustering becomes the ideal
	* one.
	* @param com             Community.
	*/
	void AssignIdealGroup(GCommunity* com);

	/**
	* Assign an ideal topic to the subject. This method can only be used
	* when the current clustering becomes the ideal
	* one.
	* @param top             Community.
	*/
	void AssignIdealGroup(GTopic* top);

	/**
	 * Get the ideal community associated with the subject.
	 */
	GCommunity* GetIdealCommunity(void) const;

	/**
	 * Get the ideal topic associated with the subject.
	 */
	GTopic* GetIdealTopic(void) const;

	/**
	* Get the number of groups of a given type (otProfile or otDoc) associated
	* to a the subject (and its sub-subjects).
	* @param type            Type.
	*/
	size_t GetNbIdealGroups(tObjType type) const;

	/**
	* Compute the number of topics (and its sub-topics) containing a document.
	*/
	size_t GetNbTopicsDocs(void) const;

	/**
	* Compute the number of profiles of a given community that are also in the
	* current one.
	* @param com             Community.
	*/
	size_t GetNbObjs(const GCommunity* com) const;

	/**
	* Compute the number of documents of a given topic that are also in the
	* current one.
	* @param top             Topic.
	*/
	size_t GetNbObjs(const GTopic* top) const;

	/**
	* Compute the number of documents of a given container that are also in the
	* current one.
	* @param docs            Container of documents.
	*/
	size_t GetNbDocs(const R::RContainer<GDoc,false,false>* docs) const;

	/**
	* Insert a document to the list of those contained in the subject.
	* @param doc             Pointer to the document.
	*/
	void Insert(GDoc* doc);

	/**
	* Insert a profile to the list of those contained in the subject.
	* @param profile          Pointer to the profile.
	*/
	void Insert(GProfile* profile);

	/**
	* Get a cursor over the profiles contained in the subject.
	*
	* The pointer passed as parameter is needed by the C++ compiler to identify
	* the method, but is not used. In practice, it can be called:
	* @code
	* GSubject* sub;
	* ...
	* R::RCursor<GProfile> Profiles(sub->GetObjs(static_cast<GProfile*>(0));
	* @endcode
	*/
	R::RCursor<GProfile> GetObjs(GProfile*) const;

	/**
	* Get a cursor over the documents contained in the subject.
	*
	* The pointer passed as parameter is needed by the C++ compiler to identify
	* the method, but is not used. In practice, it can be called:
	* @code
	* GSubject* sub;
	* ...
	* R::RCursor<GDoc> Docs(sub->GetObjs(static_cast<GDoc*>(0));
	* @endcode
	*/
	R::RCursor<GDoc> GetObjs(GDoc*) const;

	/**
	* Get the number of objects of a given type associated to this subject.
	* @param type            Type.
	* @returns size_t
	*/
	size_t GetNbObjs(tObjType type) const;

	/**
	 * @return The total number of documents assigned to this subject.
	 */
	size_t GetNbTotalDocs(void) const {return(AllDocs.GetNb());}

	/**
	* Return the name of the Subject.
	* @return RString.
	*/
	R::RString GetName(void) const;

	/**
	* Return the full name of the subject including the name of its parents.
	* @return RString.
	*/
	R::RString GetFullName(void) const;

	/**
	* Get the identifier of the Subject.
	* @returns The id of the subject.
	*/
	size_t GetId(void) const;

	/**
	* Verify if the subject is used.
	* @return bool.
	*/
	bool IsUsed(void) const;

private:

	/**
	* Set the status of the subject. When the subject is not used anymore, no
	* more profiles are associated to it. It is possible to add a given number
	* of profiles to the subject. Eventually, new users are created.
	* @param session         Session.
	* @param random          Random number generator.
	* @param nbdocs          Number of documents to select.
	* @param tmp             Temporary array.
	* @param nbprofiles      Number of profiles to create for this subject.
	* @param nbsocial        Number of social profiles still to create.
	*/
	void SetUsed(GSession* session,R::RRandom* random,size_t nbdocs,GDoc** tmp,size_t nbprofiles,size_t& nbsocial);

	/**
	* Initialize the subject (reset all profiles assigned).
	* @param unselected      Specify if the subject must (eventually) be unselected.
	*/
	void ReInit(bool unselected);

public:

	/**
	* Destruct the subject.
	*/
	virtual ~GSubject(void);

	friend class GSubjects;
};


} //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
