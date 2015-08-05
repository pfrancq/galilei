/*

	GALILEI Research Project

	GSubjects.h

	Subjects - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef GSubjectsH
#define GSubjectsH


//------------------------------------------------------------------------------
// include files for R Project
#include <rnode.h>
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gkb.h>
#include <gsubject.h>
#include <gdoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GSubjects provides a representation for a hierarchy of subjects
 * (GSubject).
 *
 * This class is used for validation purposes. It allows to associate documents
 * and profiles to ideal groups and to compare them to computed groups (topics
 * for documents and communities for profiles).
 *
 * It is possible to use some documents and subjects only.
 * @author Pascal Francq.
 * @short Subjects
 */
class GSubjects : virtual GKB, public R::RTree<GSubjects,GSubject,true>
{
    using R::RTree<GSubjects,GSubject,true>::Clear;

protected:

	/**
	 * Container of subjects ordered by identifier.
	 */
	R::RContainer<GSubject,false,true> Subjects;

	/**
	 * Container of selected subjects ordered by identifier.
	 */
	R::RContainer<GSubject,false,true> UsedSubjects;

	/**
	 * Container of used documents.
	 */
	R::RContainer<GDoc,false,true> UsedDocs;

	/**
	 * Document status (selected or not).
	 */
	R::RBoolVector DocsStatus;

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
	 * Specify if the subjects must be loaded.
	 */
	bool MustLoad;

	/**
	 * Type of the description.
	 */
	tSubjectDesc DescType;

protected:

	/**
	* Construct a tree of subjects.
	* @param session        Session.
	*/
	GSubjects(GSession* session);

   /**
    * Clear the tree.
    */
   virtual void Clear(void);

	/**
	 * Re-initialize the subjects.
    */
	void ReInit(void);

	/**
	 * Load the subjects.
    * @param obj            Pseudo-parameter.
    */
	void LoadObjs(const GSubject* obj);

public:

	/**
	 * Get the depth of the tree formed by all the subjects.
	 */
	size_t GetMaxDepth(void) const;

	/**
	 * Get the total number of subjects.
	  @param obj             Pseudo-parameter.
    */
	size_t GetNbObjs(const GSubject* obj) const;

	/**
	* Get a cursor over all the nodes of the tree.
	* @param obj             Pseudo-parameter.
	*/
	R::RCursor<GSubject> GetObjs(const GSubject* obj) const;

	/**
	* Get a cursor over the child subjects of a given subject.
	* @param obj             Pseudo-parameter.
	* @param parent          Parent subject. If null, the top subjects are
	*                        returned.
	*/
	R::RNodeCursor<GSubjects,GSubject> GetObjs(const GSubject* obj,const GSubject* parent) const;

	/**
	* Fill a given array with all the subjects. The array must be created and
	* must be large enough to hold all the subjects.
	* @see This method is used in GSimulator to create assessments for
	*      profiles during a simulation of a real system.
	* @param subjects        Pointer to the array.
	* @returns Size of the data copied in the array.
	*/
	size_t GetObjs(GSubject** subjects);

	/**
	* Get a subject.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the subject.
	* @return Pointer to GSubject.
	*/
	GSubject* GetObj(const GSubject* obj,size_t id) const;

	/**
	* Get a pointer to a given subject.
	* @param obj             Pseudo-parameter.
	* @param name            Name of the subject.
	* @return Return the pointer or 0 if the element is not in the tree.
	*/
	GSubject* GetObj(const GSubject* obj,const R::RString& name) const;

	/**
	* Insert a subject and attached it to a parent. If the parent is null,
	* the subject is considered as one of the top subject of the tree.
	* @param to              Parent where the subject is to attached.
	* @param subject         Subject to insert.
	*/
	void InsertObj(GSubject* to,GSubject* subject);

	/**
	* Compute the number of used object of a given type (otProfile or otDoc).
	* @param obj             Pseudo-parameter.
	* @param type            Type.
	*/
	size_t GetNbUsedObjs(tObjType type) const;

	/**
	* Get the number of subjects associated with a given document.
	* @param obj             Pseudo-parameter.
	* @param doc             Document.
	*/
	size_t GetNbObjs(const GSubject* obj,GDoc* doc) const;

	/**
	* Get a Cursor on the subjects of a given document.
	* @param obj             Pseudo-parameter.
	* @param doc             Document.
	*/
	R::RCursor<GSubject> GetObjs(const GSubject* obj,const GDoc* doc) const;

	/**
	* Get the first subject of a given document.
	* @param obj             Pseudo-parameter.
	* @param doc             Document.
	* @param only            Suppose the document is associated with only one
	*                        subject. If true, and if there are multiple subjects
	*                        associated, an exception is generated.
	*/
	const GSubject* GetObj(const GSubject* obj,const GDoc* doc,bool one=true) const;

	/**
	 * Select or deselect a subject.
	 * @param subject        Subject.
    * @param selected       Must the document be selected.
	 *
	 * If a subject is deselected, all the documents attached are automatically
	 * deselected. The inverse is not true.
    */
	void SetUsed(GSubject* subject,bool select);

	/**
	 * Look if a document is selected or not.
    * @param doc            Document.
    * @return true if the document is selected.
    */
	bool IsUsed(const GDoc* doc) const;

	/**
	* Fill a given array with all the selected documents. The array must be
	* created and must be large enough to hold all the documents.
	* @see This method is used in GSimulator to create assessments for
	*      profiles during a simulation of a real system.
	* @param docs            Pointer to the array.
	* @returns Size of the data copied in the array.
	*/
	size_t GetUsedObjs(GDoc** docs);

	/**
	 * Get a cursor over the selected documents.
    * @param obj             Pseudo-parameter.
    */
	R::RCursor<GDoc> GetUsedObjs(const GDoc* obj) const;

	/**
	* Look if a document is in the parent subject.
	* @param doc            Pointer to the document.
	* @param subject        Subject.
	* @return true if it is the case.
	*/
	bool IsFromParentSubject(const GDoc* doc,const GSubject* subject) const;

	/**
	* Look if a document is from a given subject.
	* @param doc            Pointer to the document.
	* @param subject        Subject.
	* @return true if it is the case.
	*/
	bool IsFromSubject(const GDoc* doc,const GSubject* subject) const;

	/**
	* Add a given document to a subject. The document is consider as deselected.
	* @param subject        Subject.
	* @param doc            Document.
	*
	*/
	void InsertObj(GSubject* subject,GDoc* doc);

	/**
	 * Select or deselect an document in relation with a given subject.
    * @param doc            Document.
	 * @param subject        Subject.
    * @param selected       Must the document be selected.
	 *
	 * A notification eSelectDoc is generated if the document is not already
	 * selected (a notification is therefore never emitted while the subjects are
	 * loaded). A notification oeDeselected is generated for the document if it
	 * isn't selected anymore.
    */
	void SetUsed(GDoc* doc,GSubject* subject,bool select);

	/**
	* Get the subject of a given profile.
	* @param obj             Pseudo-parameter.
	* @param profile         Profile.
	*/
	const GSubject* GetObj(const GSubject* obj,const GProfile* profile) const;

	/**
	* Add a given profile to a subject.
	* @param subject        Subject.
	* @param profile        Profile.
	*
	*/
	void InsertObj(GSubject* subject,GProfile* profile);

	/**
	* Get the subject of a given community.
	* @param obj             Pseudo-parameter.
	* @param com             Pointer to the community.
	*/
	const GSubject* GetObj(const GSubject* obj,const GCommunity* com) const;

	/**
	 * Set the descriptions of the subjects.
	 * @param type           TYpe of the subjects.
	 */
	void SetDescType(tSubjectDesc type);

	/**
	 * Repair the subjects. It practice, it performs the following modifications:
	 * - All leaf subjects without document associated are removed.
	 * - If a subject has associated documents and child subjects, a new child
	 *   subject is created and the documents are transfered to it.
	 */
	void Repair(void);

	friend class R::RTree<GSubjects,GSubject,true>;
	friend class R::RNode<GSubjects,GSubject,true>;
	friend class R::RNodeCursor<GSubjects,GSubject>;
	friend class GSubject;
};


} //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
