/*

	GALILEI Research Project

	GSubjects.h

	Subject - Header.

	Copyright 2002-2012 by Pascal Francq (pascal@francq.info).
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
* This Class implement a representation for a subject, i.e. an ideal group of
* documents and profiles.
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
	 * Determine if the subject is used.
	 */
	bool Used;

	/**
	 * Documents categorized to this subject.
	 */
	R::RContainer<GDoc,false,true> CategorizedDocs;

	/**
	 * Selected documents attached to this subject.
	 */
	R::RContainer<GDoc,false,true> Docs;

	/**
	 * Store where the documents are attached. The index of a document in
	 * CategorizedDocs corresponds to the index in this container.
	 */
	R::RContainer<GSubject,false,false> WhereDocs;

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
	* @param u               Used?
	*/
	GSubject(GSession* session,size_t id,const R::RString& name,bool u);

	/**
	* Get a cursor over all the child subjects.
	*/
	inline R::RCursor<GSubject> GetSubjects(void) const {return(GetNodes());}

	/**
	* @return number of child subjects.
	*/
	inline size_t GetNbSubjects(void) const {return(GetNbNodes());}

	/**
	* Initialize the subject (reset all profiles assigned).
	*/
	void ReInit(void);

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

private:

	/**
	 * Create a description. In practice, it is computed as name of
	 * the subject and its parents, or as the gravitation center of the
	 * documents (for a leaf node) or of the children (for a non-leaf node).
	 */
	void CreateDescription(void);

public:

	/**
	* Verify if a profile is part of the subject.
	* @param prof            Pointer to the profile.
	*/
	bool IsIn(GProfile* prof) const;

	/**
	* Verify if a document is selected in the subject.
	* @param doc             Pointer to the document.
	*/
	bool IsIn(GDoc* doc) const;

	/**
	* Verify if a document is categorized in the subject.
	* @param doc             Pointer to the document.
	*/
	bool IsCategorized(GDoc* doc) const;

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
	void FillDocs(GDoc** docs,size_t& nb,size_t maxdepth);

	/**
	 * Compute the maximal number of documents available for the subject (and
	 * its sub-subject).
	 * @param maxdepth       Maximal depth.
	 * @return the number of documents related to this subject that can be
	 * inserted.
	 */
	size_t GetMaxDocs(size_t maxdepth);

   /**
    * Clear the subject.
    */
   virtual void Clear(void);

	/**
	 * Clear the ideal group of a given type.
	 * @param type           Type of the group.
	 */
	void ClearIdealGroup(tObjType type);

private:

	/**
	* Assign an ideal community to the subject. This method can only be used
	* when the current clustering becomes the ideal
	* one.
	* @param com             Community.
	*/
	void AssignIdeal(GCommunity* com);

	/**
	* Assign an ideal topic to the subject. This method can only be used
	* when the current clustering becomes the ideal
	* one.
	* @param top             Community.
	*/
	void AssignIdeal(GTopic* top);

public:

	/**
	 * Get the ideal community associated with the subject.
	 */
	GCommunity* GetIdealCommunity(void) const {return(Community);}

	/**
	 * Get the ideal topic associated with the subject.
	 */
	GTopic* GetIdealTopic(void) const {return(Topic);}

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

public:

	/**
	* Get a cursor over the profiles contained in the subject.
	*
	* The pointer passed as parameter is needed by the C++ compiler to identify
	* the method, but is not used. In practice, it can be called:
	* @code
	* GSubject* sub;
	* ...
	* R::RCursor<GProfile> Profiles(sub->GetObjs(pProfile);
	* @endcode
	*/
	R::RCursor<GProfile> GetObjs(const GProfile*) const;

	/**
	* Get a cursor over the documents contained in the subject.
	*
	* The pointer passed as parameter is needed by the C++ compiler to identify
	* the method, but is not used. In practice, it can be called:
	* @code
	* GSubject* sub;
	* ...
	* R::RCursor<GDoc> Docs(sub->GetObjs(pDoc);
	* @endcode
	*/
	R::RCursor<GDoc> GetObjs(const GDoc*) const;

	/**
	* Get the number of objects of a given type associated to this subject.
	* @param type            Type.
	* @returns size_t
	*/
	size_t GetNbObjs(tObjType type) const;

	/**
	* Get a cursor over the all the documents contained in the subject.
	*/
	R::RCursor<GDoc> GetTotalDocs(void) const {return(R::RCursor<GDoc>(CategorizedDocs));}

	/**
	 * @return The total number of documents assigned to this subject.
	 */
	size_t GetNbTotalDocs(void) const {return(CategorizedDocs.GetNb());}

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

	/**
	* Destruct the subject.
	*/
	virtual ~GSubject(void);

	friend class GSubjects;
	friend class GSession;
	friend class R::RTree<GSubjects,GSubject,true>;
	friend class R::RNode<GSubjects,GSubject,true>;
	friend class GSimulator;
};


//------------------------------------------------------------------------------
/**
 * The GSubjects provides just a tree of GSubject.
 * @author Pascal Francq.
 * @short Subjects
 */
class GSubjects : public R::RTree<GSubjects,GSubject,true>
{
    using R::RTree<GSubjects,GSubject,true>::Clear;

	/**
	 * Ordered container of subjects.
	 */
	R::RContainer<GSubject,false,true> Subjects;

	/**
	 * Container of selected documents.
	 */
	R::RContainer<GDoc,false,true> SelectedDocs;

	/**
	 * Subjects for the documents (the identifier of the document provides the
	 * index in the container).
	 */
	R::RContainer<R::RContainer<GSubject,false,false>,true,false> DocsSubjects;

	/**
	 * Subjects for the profiles (the identifier of the profile provides the
	 * index in the container).
	 */
	R::RContainer<GSubject,false,false> ProfilesSubject;

	/**
	 * Specify if the subjects are loaded or not.
	 */
	bool SubjectsLoaded;

	/**
	 * Type of the description.
	 */
	tSubjectDesc DescType;

public:

	/**
	* Construct a tree of subjects.
	* @param max            Initial size of subjects.
	*/
	GSubjects(size_t max);

   /**
    * Clear the tree.
    */
   virtual void Clear(void);

	friend class GSubject;
	friend class GSession;
};


} //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
