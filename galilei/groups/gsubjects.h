/*

	GALILEI Research Project

	GSubjects.h

	Subjects - Header.

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



//-----------------------------------------------------------------------------
#ifndef GSubjectsH
#define GSubjectsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rtree.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdebugobject.h>
#include <gbasicsession.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GSubjects class provides a representation for a set of subjects
* representing ideal groups. Since different subjects may be part of a main
* subject, it is in fact a tree of subjects which are stored.
* @author Pascal Francq, Julien Lamoral and David Wartel.
* @short Subjects.
*/
class GSubjects : protected R::RTree<GSubjects,GSubject,true>, virtual public GBasicSession
{
protected:

	/**
	 * Ordered container of subjects.
	 */
	R::RContainer<GSubject,false,true> Subjects;

	/**
	 * Depth of the tree representing the subjects.
	 */
	size_t MaxDepth;

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
	 * Specify if the subjects must loaded or not.
	 */
	bool ToLoad;

public:

	/**
	* Constructor of the subjects.
	*/
	GSubjects(void);

private:

	/**
	 * verify if the subject where loaded and load them if necessary.
	 */
	inline void VerifyLoad(void) const;

public:

	/**
	* Re-initialize the subjects.
	*/
	void ReInit(void);

	/**
	* Get a cursor over the top nodes.
	* @return RCursor<N>.
	*/
	R::RCursor<GSubject> GetTopSubjects(void) const;

	/**
	* Get the total number of nodes in the tree.
	* @return size_t
	*/
	size_t GetNbSubjects(void) const;

	/**
	* Get a cursor over all the nodes of the tree.
	*/
	R::RCursor<GSubject> GetSubjects(void) const;

	/**
	* Get a cursor over the child subjects of a given subject.
	* @param subject            Parent subject. If null, the top subjects are returned.
	*/
	R::RCursor<GSubject> GetSubjects(const GSubject* subject) const;

	/**
	* @return the number of child subjects of a given subject.
	* @param subject            Parent subject. If null, the top subjects are returned.
	*/
	size_t GetNbSubjects(const GSubject* subject) const;

	/**
	* Get a pointer to a certain node of the tree.
	* @param subject         Subject.
	* @return Return the pointer or 0 if the element is not in the tree.
	*/
	GSubject* GetSubject(const R::RString& subject) const;

	/**
	* Insert a subject and attached it to a parent. If the parent is null,
	* the subject is considered as one of the top subject of the tree.
	* @param to              Parent where the subject is to attached.
	* @param subject         Subject to insert.
	*/
	void InsertSubject(GSubject* to,GSubject* subject);

	/**
	 * Clear the subjects.
	 * @param nbsubjects     Number of subjects.
	 * @param nbdocs         Number of documents.
	 * @param nbprofiles     Number of profiles.
	 */
	void Clear(size_t nbsubjects,size_t nbdocs,size_t nbprofiles);

	/**
	 * Get the depth of the tree formed by all the subjects.
	 */
	size_t GetMaxDepth(void) const;

	/**
	* Get a subject.
	* @param subjectid       Identifier of the subject.
	* @return Pointer to GSubject.
	*/
	GSubject* GetSubject(size_t subjectid) const;

	/**
	* Compute the number of ideal groups of a given type (otProfile or otDoc)
	* for a given topic (and its sub-topics).
	* @param type            Type.
	*/
	size_t GetNbIdealGroups(tObjType type) const;

	/**
	* Compute the number of topics (and its sub-topics) containing a document.
	*/
	size_t GetNbTopicsDocs(void) const;

	/**
	* Add a given profile to a subject.
	* @param profile        Pointer to the profile.
	* @param subjectid      Identifier of the subject.
	*/
	void Insert(GProfile* profile,size_t subjectid);

	/**
	* Get the subject of a given profile.
	* @param prof            Pointer to the profile.
	*/
	const GSubject* GetIdealGroup(GProfile* prof) const;

	/**
	* Add a given document to a subject.
	* @param doc            Pointer to the document.
	* @param subjectid      Identifier of the real subject.
	* @param usedid         Identifier of the subject used.
	*/
	void Insert(GDoc* doc,size_t subjectid,size_t usedid);

	/**
	* Fill a given array with all the selected documents. The array must be
	* created and must be large enough to hold all the documents.
	* @see This method is used in GSimulator to create assessments for
	*      profiles during a simulation of a real system.
	* @param docs            Pointer to the array.
	* @returns Size of the data copied in the
	* array.
	*/
	size_t FillSelectedDocs(GDoc** docs);

	/**
	* Get the subject of a given document. If a selected document is part of
	* @param doc             Pointer to the document.
	*/
	const GSubject* GetIdealGroup(GDoc* doc) const;

	/**
	* Look if a document is from a given subject.
	* @param doc            Pointer to the document.
	* @param s              Pointer to the subject.
	* @return bool
	*/
	bool IsFromIdealGroup(GDoc* doc,const GSubject* s) const;

	/**
	* Look if a document is in the parent subject.
	* @param doc            Pointer to the document.
	* @param s              Subject.
	* @return bool
	*/
	bool IsFromParentIdealGroup(GDoc* doc,const GSubject* s) const;

	/**
	* Get a Cursor on the subjects of a given document.
	* @param doc            Pointer to the document.
	*/
	R::RCursor<GSubject> GetIdealGroups(GDoc* doc) const;

	/**
	* Get the number of subjects associated with a given document.
	* @param doc            Pointer to the document.
	*/
	size_t GetNbIdealGroups(GDoc* doc) const;

	/**
	* Destruct the subjects.
	*/
	~GSubjects(void);
};


} //-------- End of namespace GALILEI ------------------------------------------


//------------------------------------------------------------------------------
#endif
