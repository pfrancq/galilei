/*

	GALILEI Research Project

	GObjRef.h

	Class representing a reference to a given object - Header.

	Copyright 2004 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GObjRefH
#define GObjRefH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>
#include <groups/ggroup.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GObjRef class represent a generic reference to a given object. In GALILEI,
* all the objects (users, profiles, documents, ...) are identify through an
* unique identifier.
* @author Pascal Francq
* @short Generic object reference.
*/
class GObjRef
{
	/**
	* Identifier of the object referenced.
	*/
	unsigned int Id;

public:

	/**
	* Constructor.
	* @param id            Identifier.
	*/
	GObjRef(unsigned int id) : Id(id) {}

	/**
	* Get the identifier of the object referenced.
	*/
	unsigned int GetId(void) const {return(Id);}
};


//------------------------------------------------------------------------------
/**
* The GDocRef class represents a reference to a document.
* @author Pascal Francq
* @short Reference to a document.
*/
class GDocRef : public GObjRef
{
public:
	GDocRef(unsigned int id) :  GObjRef(id) {}
	GDocRef(const GDoc* doc) :  GObjRef(doc->GetId()) {}
};


//------------------------------------------------------------------------------
/**
* The GGroupRef class represents a reference to a group.
* @author Pascal Francq
* @short Reference to a group.
*/
class GGroupRef : public GObjRef
{
public:
	GGroupRef(unsigned int id) :  GObjRef(id) {}
	GGroupRef(const GGroup* group) :  GObjRef(group->GetId()) {}
};


//------------------------------------------------------------------------------
/**
* The GUserRef class represents a reference to a user.
* @author Pascal Francq
* @short Reference to a user.
*/
class GUserRef : public GObjRef
{
public:
	GUserRef(unsigned int id) :  GObjRef(id) {}
	GUserRef(const GUser* usr) :  GObjRef(usr->GetId()) {}
};


//------------------------------------------------------------------------------
/**
* The GProfileRef class represents a reference to a profile.
* @author Pascal Francq
* @short Reference to a profile.
*/
class GProfileRef : public GObjRef
{
public:
	GProfileRef(unsigned int id) :  GObjRef(id) {}
	GProfileRef(const GProfile* profile) :  GObjRef(profile->GetId()) {}
};


//------------------------------------------------------------------------------
/**
* The GSubProfileRef class represents a reference to a subprofile.
* @author Pascal Francq
* @short Reference to a subprofile.
*/
class GSubProfileRef : public GObjRef
{
public:
	GSubProfileRef(unsigned int id) :  GObjRef(id) {}
	GSubProfileRef(const GSubProfile* subprofile) :  GObjRef(subprofile->GetId()) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
