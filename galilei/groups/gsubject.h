/*

	GALILEI Research Project

	Gsubjects.h

	GSubject - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

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



//---------------------------------------------------------------------------
#ifndef GSubjectH
#define GSubjectH


//-----------------------------------------------------------------------------
//include files for R Project
#include <rstd/rnode.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <docs/gdoc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* This Class implement a representation for a subject used to create judgment
* and feedback.
* @author Pascal Francq & Julien Lamoral
* @short  Subject
*/
class GSubject: public R::RNode<GSubject,false>
{
protected:

	/**
	* The id of the subject.
	*/
	unsigned int Id;

	/**
	* The name of the subject.
	*/
	R::RString Name;

	/**
	* true if the subsubject is used.
	*/
	bool Used;

	/**
	* The language of a subsubject
	*/
	GLang* Lang;

	/**
	* Documents in the subject.
	*/
	R::RContainer<GDoc,unsigned,false,true> Docs;

	/**
	* Sub-profiles in the subject.
	*/
	R::RContainer<GSubProfile,unsigned,false,true> SubProfiles;

public:

	/**
	* Constructor
	* @param id              Identificator.
	* @param name            Name of the subject.
	* @param lang            Language.
	* @param u               Used?
	*/
	GSubject(unsigned int id,const char* name,GLang* l,bool u);

	/**
	* Insert a document.
	*/
	void InsertDoc(GDoc* d) throw(bad_alloc);

	/**
	* Get a cursor over the documents used in the system.
	*/
	GDocCursor& GetDocsCursor();

	/**
	* Get the number of documents associated to this subject.
	* @returns unsigned int
	*/
	unsigned int GetNbDocs(void) const;

	/**
	* Insert a subprofile.
	*/
	void InsertSubProfile(GSubProfile* s) throw(bad_alloc);

	/**
	* Get a cursor over the subprofiles of the system.
	*/
	GSubProfileCursor& GetSubProfilesCursor(void);

	/**
	* Get the number of subprofiles associated to this subject.
	* @returns unsigned int
	*/
	unsigned int GetNbSubProfiles(void) const;

	/**
	* Compare a subject with a given name.
	* @param name           Name used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const char* name) const {return(Name.Compare(name));}

	/**
	* Compare two subject.
	* @param sub            Subject used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const GSubject *sub) const {return(Id-sub->Id);}

	/**
	* Compare two subject.
	* @param sub            Subject used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const GSubject &sub) const {return(Id-sub.Id);}

	/**
	* Comparaison two id.
	* @param id            id used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const unsigned int id) const {return(Id-id);};

	/**
	* Return the name of the Subject.
	* @returns a string containing the name.
	*/
	const char* GetName(void) {return(Name());};

	/**
	* Return the id of the Subject.
	* @returns The id of the subject.
	*/
	unsigned int GetId(void) {return(Id);};

	/**
	* Return true if the subject is Used
	* @returns bool.
	*/
	bool IsUsed(void) {return(Used);}

	/**
	* Set the status of the subject.
	*/
	void SetUsed(bool b) {Used=b;}

	/**
	* Return the Lang of the Subject.
	* @returns a string containing the lang.
	*/
	GLang* GetLang(void) const;

	/**
	* Return the Lang of the Subject.
	* @param lang              The lang of the subject.
	*/
	void SetLang(GLang* lang);

	/**
	* Destructor.
	*/
	~GSubject(void);
};


//-----------------------------------------------------------------------------
/**
* The GSubjectCursor class provides a way to go trough a set of subjects.
* @short Subject Cursor
*/
CLASSCURSOR(GSubjectCursor,GSubject,unsigned int)



}; //-------- End of namespace GALIELI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
