/*

	GALILEI Research Project

	Gsubjects.h

	Subject - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
		Julien Lamoral (jlamoral@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef GSubjectH
#define GSubjectH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rnode.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggroupvector.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* This Class implement a representation for a subject, i.e. an ideal group. A
* subject is supposed to be associated with one language.
*
* This class is used for validation purposes.
* @author Pascal Francq, Julien Lamoral and David Wartel.
* @short Subject
*/
class GSubject: public R::RNode<GSubject,false>, public GGroupVector
{
protected:

	/**
	* Name of the subject.
	*/
	R::RString Name;

	/**
	* Determine if the subject is used.
	*/
	bool Used;

	/**
	* Documents in the subject.
	*/
	R::RContainer<GDoc,unsigned,false,true> Docs;

public:

	/**
	* Constructor of a subject.
	* @param id              Identificator of the subject.
	* @param name            Name of the subject.
	* @param lang            Language.
	* @param u               Used?
	*/
	GSubject(unsigned int id,const char* name,GLang* l,bool u) throw(bad_alloc);

	/**
	* Compare two subjects by comparing their identificator.
	* @see R::RContainer
	* @param sub             Pointer to a subject.
	* @return int
	*/
	int Compare(const GSubject* sub) const;

	/**
	* Compare two subjects by comparing their identificator.
	* @see R::RContainer
	* @param sub             Subject.
	* @return int
	*/
	int Compare(const GSubject& sub) const;

	/**
	* Compare the identificator of a subject with another one.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare the name of a subject with a given string.
	* @see R::RContainer
	* @param name            String.
	* @return int
	*/
	int Compare(const char* name) const;

	/**
	* Insert a document to the list of those contained in the subject.
	*/
	void InsertDoc(GDoc* d) throw(bad_alloc);

	/**
	* Get a cursor over the documents contained in the subject.
	* @return GDocCursor.
	*/
	GDocCursor& GetDocsCursor(void);

	/**
	* Get the number of documents associated to this subject.
	* @returns unsigned int
	*/
	unsigned int GetNbDocs(void) const;

	/**
	* Return the name of the Subject.
	* @return RString.
	*/
	R::RString& GetName(void) const;

	/**
	* Get the identificator of the Subject.
	* @returns The id of the subject.
	*/
	unsigned int GetId(void) const {return(GGroup::GetId());}

	/**
	* Verify if the subject is used.
	* @return bool.
	*/
	bool IsUsed(void) const {return(Used);}

	/**
	* Set the status of the subject.
	*/
	void SetUsed(bool b);

	/**
	* Destructor of a subject.
	*/
	~GSubject(void) throw(GException);
};


//------------------------------------------------------------------------------
/**
* The GSubjectCursor class provides a way to go trough a set of subjects.
* @short Subject Cursor
*/
CLASSCURSOR(GSubjectCursor,GSubject,unsigned int)



}; //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
