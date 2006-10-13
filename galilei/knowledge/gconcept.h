/*

	GALILEI Research Project

	GConcept.h

	Concept - Header.

	Copyright 2006 by the Université Libre de Bruxelles.

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
#ifndef GConcept_H
#define GConcept_H


//------------------------------------------------------------------------------
// include files for R
#include <rvectorint.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GConcept class provides a representation for a generic concept. A concept
* is a "representation" of a given piece of knowledge.
* @author Pascal Francq
* @short Concept
*/
class GConcept
{
	/**
	* Identifier of the concept.
	*/
	unsigned int Id;

	/**
	* Language of the concept.
	*/
	GLang* Lang;

	/**
	* Name of the concept.
	*/
	R::RString Name;

	/**
	* Type of the concept.
	*/
	unsigned int Type;

	/**
	* Relations with other concepts.
	*/
	R::RVectorInt<true>* Relations;

	/**
	* Number of references in documents.
	*/
	unsigned int NbRefDocs;

	/**
	* Number of references in subprofiles.
	*/
	unsigned int NbRefSubProfiles;

	/**
	* Number of references in groups.
	*/
	unsigned int NbRefGroups;

public:

	/**
	* Constructor of a concept.
	*/
	GConcept(void);

	/**
	* Constructor of a generic concept.
	* @param lang            Language of the concept.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	*/
	GConcept(GLang* lang,const R::RString& name,unsigned int type);

	/**
	* Constructor of a generic concept.
	* @param id              Identifier.
	* @param lang            Language of the concept.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	* @param refdocs         Number of documents referenced.
	* @param refsubprofiles  Number of subprofiles referenced.
	* @param refgroups       Number of groups referenced.
	*/
	GConcept(unsigned int id,GLang* lang,const R::RString& name,unsigned int type,unsigned int refdocs,unsigned int refsubprofiles,unsigned int refgroups);

	/**
	* Copy constructor.
	* @param c               Original concept.
	*/
	GConcept(const GConcept& c);

	/**
	* Compare two concepts.
	* @param c               Concept used.
	* @see R::RContainer
	*/
	int Compare(const GConcept& c) const;

	/**
	* Compare a concept with a given name.
	* @param name            Name.
	* @see R::RContainer
	*/
	int Compare(const R::RString& name) const;

	/**
	* Compare a concept with a given identifier.
	* @param id              Identifier used.
	* @see R::RContainer
	*/
	int Compare(unsigned int id) const;

	/**
	* Assignment operator for concept.
	* @param c               Concept.
	*/
	GConcept& operator=(const GConcept& d);

	/**
	* Compute the first hash index of the name of a concept.
	* @see R::RHashContainer
	* @param c               Concept.
	*/
	static int HashIndex(const GConcept& c)
		{return(R::RString::HashIndex(c.Name));}

	/**
	* Compute the first hash index of the name of a concept.
	* @see R::RHashContainer
	* @param d               Concept.
	*/
	static int HashIndex(const GConcept* c)
		{return(R::RString::HashIndex(c->Name));}

	/**
	* Compute the first hash index of a string.
	* @see R::RHashContainer
	* @param name           String.
	*/
	static int HashIndex(const R::RString& name)
		{return(R::RString::HashIndex(name));}

	/**
	* Compute the first hash index of an array of characters.
	* @see R::RHashContainer
	* @param name           Pointer to the array.
	*/
	static int HashIndex(const R::RChar* name)
		{return(R::RString::HashIndex(name));}

	/**
	* Compute the second hash index of the name of a concept.
	* @see R::RHashContainer
	* @param d               Concept.
	*/
	static int HashIndex2(const GConcept& c)
		{return(R::RString::HashIndex2(c.Name));}

	/**
	* Compute the second hash index of the name of a concept.
	* @see R::RHashContainer
	* @param d               Concept.
	*/
	static int HashIndex2(const GConcept* c)
		{return(R::RString::HashIndex2(c->Name));}

	/**
	* Compute the second hash index of an array of characters.
	* @see R::RHashContainer
	* @param name           Pointer to to the array.
	*/
	static int HashIndex2(const R::RChar* name)
		{return(R::RString::HashIndex2(name));}

	/**
	* Compute the second hash index of a string.
	* @see R::RHashContainer
	* @param name           String.
	*/
	static int HashIndex2(const R::RString& name)
		{return(R::RString::HashIndex2(name));}

	/**
	* Get the type of the concept.
	*/
	unsigned int GetType(void) const {return(Type);}

	/**
	* Get the language of the concept.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Set the Identificator of the word.
	* @param id              Identificator of the word.
	*/
	void SetId(unsigned int id);

	/**
	* @return Identificator of the word.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* @return A string representing the word.
	*/
	R::RString GetName(void) const {return(Name);}

	/**
	* Increase the number of references on this data for a given object type.
	* @param ObjType        Type of the reference.
	*/
	unsigned int IncRef(tObjType ObjType);

	/**
	* Decrease the number of references on this data for a given object type.
	* @param ObjType        Type of the reference.
	*/
	unsigned int DecRef(tObjType ObjType);

	/**
	* Get the number of references on this data for a given object type.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(tObjType ObjType) const;

	/**
	* Clear the information of the data link to a specific object.
	* @param ObjType        Type of the object.
	*/
	virtual void Clear(tObjType ObjType);

	/**
	* Clear all the information of the data.
	*/
	virtual void Clear(void);

	/**
	* Test if the data is considered as empty. By default, every data defined by
	* a non-empty name, is also considered as not empty.
	* @return bool
	*/
	virtual bool IsEmpty(void) const;

	/**
	* Desctuctor of a concept.
	*/
	virtual ~GConcept(void);

	friend class GInfo;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
