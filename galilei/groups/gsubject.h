/*

	GALILEI Research Project

	Gsubjects.h

	Subject - Header.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
		Julien Lamoral (jlamoral@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
#include <ggroup.h>


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
class GSubject: public R::RNode<GSubject,false>
{
	class Intern;

	/*
	* Internal data.
	*/
	Intern* Data;

public:

	/**
	* Constructor of a subject.
	* @param id              Identificator of the subject.
	* @param name            Name of the subject.
	* @param u               Used?
	*/
	GSubject(unsigned int id,const char* name,bool u);

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
	int Compare(const R::RString& name) const;

	/**
	* Get the ideal group of the subprofile.
	* @param sub            SubProfile.
	*/
	GSubject* GetIdealGroup(GSubProfile* sub) const;

	/**
	*/
	bool IsIn(GSubProfile* sub) const;

	/**
	*/
	unsigned int GetNbIdealGroups(const GLang* lang) const;

	/**
	*/
	unsigned int GetNbSubProfiles(const GLang* lang) const;

	/**
	* Insert a subprofiles in the container.
	* @param s              Pointer to the subprofile to add.
	*/
	void InsertSubProfile(GSubProfile* s);

	/**
	* Get a cursor over the subprofiles of the system for a given language.
	* @param lang           Language.
	*/
	R::RCursor<GSubProfile> GetSubProfiles(const GLang* lang) const;

	/**
	* Compute the number of subprofiles of a given group that are also in the
	* current one.
	* @param grp             Group.
	*/
	unsigned int GetNbSubProfiles(const GGroup* grp) const;

	/**
	* Insert a document to the list of those contained in the subject.
	* @param doc             Pointer to the document.
	*/
	void Insert(GDoc* doc);

	/**
	* Get a cursor over the documents contained in the subject.
	* @return GDocCursor.
	*/
	R::RCursor<GDoc> GetDocs(void);

	/**
	* Get the number of documents associated to this subject.
	* @returns unsigned int
	*/
	unsigned int GetNbDocs(void) const;

	/**
	* Insert a profile to the list of those contained in the subject.
	* @param profile          Pointer to the profile.
	*/
	void Insert(GProfile* profile);

	/**
	* Get a cursor over the profiles contained in the subject.
	* @return GDocCursor.
	*/
	R::RCursor<GProfile> GetProfiles(void);

	/**
	* Get the number of profiles associated to this subject.
	* @returns unsigned int
	*/
	unsigned int GetNbProfiles(void) const;

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
	* Get the identificator of the Subject.
	* @returns The id of the subject.
	*/
	unsigned int GetId(void) const;

	/**
	* Verify if the subject is used.
	* @return bool.
	*/
	bool IsUsed(void) const;

	/**
	* Set the status of the subject. When the subject is not used anymore, no
	* more profiles are associated to it. It is possible to add a given number
	* of profiles to the subject. Eventually, new users are created.
	* @param session         Session.
	* @param nbprofiles      Number of profiles to create for this subject.
	* @param nbsocial        Number of social profiles still to create.
	*/
	void SetUsed(GSession* session,size_t nbprofiles,unsigned int& nbsocial);

	/**
	* Create a group for a given language with the subprofiles having a
	* description. The identificator of the group is the one of the subject.
	* The group must be destructed by the caller.
	* @param lang            Pointer to the language.
	* @return Pointer to the group.
	*/
//	GGroup* CreateGroup(GLang* lang) const;

	/**
	*/
	void ReInit(void);

	/**
	* Destructor of a subject.
	*/
	virtual ~GSubject(void);

	friend class GSubjects;
};


} //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
