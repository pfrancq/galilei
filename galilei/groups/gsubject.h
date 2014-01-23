/*

	GALILEI Research Project

	GSubject.h

	Subject - Header.

	Copyright 2002-2014 by Pascal Francq (pascal@francq.info).
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
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdescription.h>


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
public:

	/**
	 * Centroid of a group.
	 */
	cObj* Centroid;

	/**
	 * Average similarity of the objects of the group to the centroid.
	 */
	double AvgSim;

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
};


//------------------------------------------------------------------------------
/**
* The GSubject class provides a representation for a subject, i.e. an ideal
* group of documents and profiles.
*
* This class is used for validation purposes.
* @author Pascal Francq, Julien Lamoral and David Wartel.
* @short Subject
*/
class GSubject : protected R::RNode<GSubjects,GSubject,true>, public GDescription
{
    using R::RNode<GSubjects,GSubject,true>::Clear;

	/**
	 * Session.
	 */
	GSession* Session;

	/**
	 * Identifier of the subject.
	 */
	size_t Id;

	/**
	 * Name of the subject.
	 */
	R::RString Name;

	/**
	 * Is the subject used.
	 */
	bool Used;

	/**
	 * Documents categorized to this subject.
	 */
	R::RContainer<GDoc,false,true> Docs;

	/**
	 * Selected documents attached to this subject.
	 */
	R::RContainer<GDoc,false,true> UsedDocs;

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
	* @param session         Session.
	* @param id              Identifier of the subject.
	* @param name            Name of the subject.
	*/
	GSubject(GSession* session,size_t id,const R::RString& name);

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
	size_t GetId(void) const {return(Id);}

	/**
	* Verify if the subject is used.
	* @return bool.
	*/
	bool IsUsed(void) const {return(Used);}

	/**
	* @return number of child subjects.
	*/
	inline size_t GetNbSubjects(void) const {return(GetNbNodes());}

	/**
	 * Get the depth of the subject.
	 */
	size_t GetDepth(void) const {return(R::RNode<GSubjects,GSubject,true>::GetDepth());}

	/**
	 * Get the parent of the subject.
	 * @return pointer to the parent or null if it is a top subject.
	 */
	GSubject* GetParent(void) const {return(Parent);}

	/**
	 * Create a description. In practice, it is computed as name of
	 * the subject and its parents, or as the gravitation center of the
	 * documents (for a leaf node) or of the children (for a non-leaf node).
	 */
	void CreateDescription(void);

	/**
	* Verify if a document is used by the subject.
	* @param doc             Pointer to the document.
	*/
	bool IsUsed(GDoc* doc) const;

	/**
	* Verify if a document is assign to the subject. This doesn't means that
	* the document is used by that subject (use GSubject::IsUsed for that
	* purpose).
	* @param doc             Pointer to the document.
	*/
	bool IsIn(GDoc* doc) const;

	/**
	* Fill a given array with all the documents. If the subject has the maximal
	* depth, all the documents of its sub-subjects are also inserted.
	*
	* The array must be created and must be large enough to hold all the
	* documents.
	* @see This method is used in GSimulator to create assessments for
	*      profiles during a simulation of a real system.
	* @param docs            Pointer to the array.
	* @param maxdepth        Maximal depth.
	* @param nb              Number of elements in the array.
	*/
	void GetObjs(GDoc** docs,size_t& nb,size_t maxdepth);

	/**
	 * Compute the maximal number of documents available for the subject (and
	 * its sub-subject).
	 * @param obj            Pseudo-parameter.
	 * @param maxdepth       Maximal depth.
	 * @return the number of documents related to this subject that can be
	 * inserted.
	 */
	size_t GetNbObjs(const GDoc* obj,size_t maxdepth);

	/**
	* Verify if a profile is assign to the subject.
	* @param prof            Pointer to the profile.
	*/
	bool IsIn(GProfile* prof) const;

	/**
	* Verify if a profile is used by the subject. In practice, all profiles
	* assigned to a subject are used.
	* @param prof            Pointer to the profile.
	*/
	bool IsUsed(GProfile* prof) const;

	/**
	 * Clear the ideal group of a given type.
	 * @param type           Type of the group.
	 */
	void ClearIdealGroup(tObjType type);

	/**
	* Assign an ideal community to the subject. This method should only be used
	* when the current clustering becomes the ideal
	* one.
	* @param com             Community.
	*/
	void AssignIdeal(GCommunity* com);

	/**
	* Assign an ideal topic to the subject. This method should only be used
	* when the current clustering becomes the ideal
	* one.
	* @param top             Community.
	*/
	void AssignIdeal(GTopic* top);

	/**
	 * Get the ideal community associated with the subject.
	 */
	GCommunity* GetIdealCommunity(void) const {return(Community);}

	/**
	 * Get the ideal topic associated with the subject.
	 */
	GTopic* GetIdealTopic(void) const {return(Topic);}

	/**
	* Get the number of objects of a given type (otProfile or otDoc) associated
	* to the subject (and eventually its sub-subjects).
	* @param obj             Pseudo-parameter.
	* @param type            Type.
	*/
	size_t GetNbObjs(const GSubject* obj,tObjType type) const;

	/**
	* Get the number of used objects of a given type (otProfile or otDoc)
	* associated to the subject (and eventually its sub-subjects).
	* @param obj             Pseudo-parameter.
	* @param type            Type.
	*/
	size_t GetNbUsedObjs(const GSubject* obj,tObjType type) const;

	/**
	* Compute the number of profiles of a given community that are also in the
	* current one.
	* @param com             Community.
	*/
	size_t GetNbUsedObjs(const GCommunity* com) const;

	/**
	* Compute the number of used documents of a given topic that are also in the
	* current one.
	* @param top             Topic.
	*/
	size_t GetNbUsedObjs(const GTopic* top) const;

	/**
	* Compute the number of used documents of a given container that are also in
	* the current one.
	* @param docs            Container of documents.
	*/
	size_t GetNbUsedObjs(const R::RContainer<GDoc,false,false>* docs) const;

	/**
	* Get the number of objects of a given type (otProfile or otDoc) associated
	* to a the subject.
	* @param type            Type.
	*/
	size_t GetNbUsedObjs(tObjType type) const;

	/**
	* Get a cursor over the profiles contained in the subject.
	* @param obj             Pseudo-parameter.
	*/
	R::RCursor<GProfile> GetObjs(const GProfile* obj) const;

	/**
	* Get a cursor over the used profiles contained in the subject. In practice,
	* all the profiles assigned to a subject are used.
	* @param obj             Pseudo-parameter.
	*/
	R::RCursor<GProfile> GetUsedObjs(const GProfile* obj) const;

	/**
	* Get a cursor over the documents contained in the subject.
	* @param obj             Pseudo-parameter.
	*/
	R::RCursor<GDoc> GetUsedObjs(const GDoc* obj) const;

	/**
	* Get a cursor over the all the documents contained in the subject.
	* @param obj             Pseudo-parameter.
	*/
	R::RCursor<GDoc> GetObjs(const GDoc* obj) const;

	/**
	 * The the number of documents assigned to this subject.
	 * @param obj             Pseudo-parameter.
	 */
	size_t GetNbObjs(const GDoc* obj) const;

	/**
	 * Guess the language of a subject. It is the language of the most
	 * documents attached to this subject. If not found, it is supposed that
	 * the language is the one of the parent.
	 * @param lookparent     Look also in the parents.
	 * @return a pointer to the corresponding GLang.
	 */
	GLang* GuessLang(bool lookparent=true) const;

	/**
	 * Get the cost of an Up operation of the current node. The method adds a
	 * cost computed for each vector. The cost of one vector is :
	 * -# The number of concept references if the class has no parent.
	 * -# The difference of the number of concept references between the parent
	 *    and the current class.
	 *
	 * In their paper <em>TreeRank: A Similarity Measure for Nearest Neighbor
	 * Searching in Phylogenetic Databases</em>, Wang, Shan, Shasha and Piel
	 * define the up operation as the operation that moves a token from one
	 * node to its parent.
	 * @return Cost of the up operation.
	 */
	virtual double GetUpOperationCost(void) const;

private:

   /**
    * Clear the subject.
    */
   virtual void Clear(void);

	/**
	* Initialize the subject (reset all profiles assigned).
	*
	* An notification eUnselectDoc is generated for each document.
	*/
	void ReInit(void);

public:

	/**
	* Destruct the subject.
	*/
	virtual ~GSubject(void);

	friend class GSubjects;
	friend class R::RTree<GSubjects,GSubject,true>;
	friend class R::RNode<GSubjects,GSubject,true>;
	friend class R::RNodeCursor<GSubjects,GSubject>;
};


} //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
