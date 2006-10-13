/*

	GALILEI Research Project

	GDict.h

	Concepts Dictionary - Header.

	Copyright 2001-2006 by the Université Libre de Bruxelles.

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
#ifndef GDictH
#define GDictH


//------------------------------------------------------------------------------
// include files for R Project
#include <rdblhashcontainer.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDict class provides a representation for a dictionary of concepts of a
* given type in a given language. Each concept has its own identificator and
* name.
* @author Pascal Francq
* @short Concepts Dictionary.
*/
class GDict : protected R::RDblHashContainer<GConcept,27,27,true>
{
	/**
	* Language corresponding to the dictionary.
	*/
	GLang *Lang;

	/**
	* Type of the concepts handle by the dictionnary.
	*/
	unsigned int Type;

	/**
	* Array of concepts ordered by identificator.
	*/
	GConcept** Direct;

	/**
	* Highest identificator that the dictionary can handle.
	*/
	unsigned int MaxId;

	/**
	* Highest identificator of the concept stored.
	*/
	unsigned int UsedId;

	/**
	* State of the dictionary.
	*/
	bool Loaded;

public:

	/**
	* Constructor of the dictionary.
	* @param lang            Corresponding language.
	* @param type            Type of the dictionary.
	* @param m               Total maximal number of data to create at
	*                        initialisation.
	* @param ml              Maximal number of data to create at initialisation
	*                        for the different letters.
	*/
	GDict(GLang* lang,unsigned int type,unsigned m,unsigned ml);

	/**
	* Clear the dictionary.
	*/
	void Clear(void);

	/**
	* Compare two dictionnaries by their type.
	* @param dict            Dictionnary used to compare.
	* @see R::RContainer.
	*/
	int Compare(const GDict& dict) const;

	/**
	* Compare a dictionnary with a given type.
	* @param type            Type used to compare.
	* @see R::RContainer.
	*/
	int Compare(unsigned int type) const;

private:

	/**
	* Put a concept in direct.
	* @param concept         Pointer to the concept to insert.
	*/
	void PutDirect(GConcept* concept);

public:

	/**
	* Insert a concept in the dictionary. In practice, it is a copy of the
	* concept which is inserted.
	* @param concept         Concept to insert.
	* @return Pointer to the concept inserted in the dictionary.
	*/
	GConcept* InsertConcept(const GConcept* concept);

	/**
	* Delete a given concept from the dictionary.
	* @param concept         Concept to delete.
	*/
	void DeleteConcept(GConcept* concept);

	/**
	* Get the concept with a specific identificator.
	* @param id             Identificator.
	* @return Pointer to a GConcept.
	*/
	GConcept* GetConcept(const unsigned int id) const;

	/**
	* Get the highest identificator of the concept stored by the dictionary.
	* @return unsigned int
	*/
	unsigned int GetConceptMaxId(void) const {return(UsedId);}

	/**
	* Get the list of the concepts currently defined in the dictionary.
	* @returns const Pointer to array of GConcept*.
	*/
	const GConcept** GetConcepts(void) const {return(const_cast<const GConcept**>(Direct));}

	/**
	* Get the type of the dictionary.
	* @returns unsigned int.
	*/
	unsigned int GetType(void) const {return(Type);}

	/**
	* Get the language of the dictionary.
	* @return Pointer to GLang.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Method giving the number of concepts contained in the dictionary.
	* @return unsigned int
	*/
	unsigned int GetNbConcepts(void) const {return(GetNb());}

	/**
	* Look if a given concept is in the dictionary.
	* @param name            Name fo the concept to look for.
	* @return true if the concept is in the dictionary.
	*/
	bool IsIn(const R::RString& name) const;

	/**
	* Get a given concept from the dictionary.
	* @param name            Name fo the concept to look for.
	* @return Pointer to the concept.
	*/
	GConcept* GetConcept(const R::RString& name) const;

private:

	/**
	* Increase the number of objects of a given type that make a reference to a
	* concept.
	* @param id             Identificator of the concept.
	* @param ObjType        Type of the object.
	* @return New number of references.
	*/
	unsigned int IncRef(unsigned int id,tObjType ObjType);

	/**
	* Decrease the number of objects of a given type that make a reference to a
	* concept.
	* @param id             Identificator of the concept.
	* @param ObjType        Type of the object.
	* @return New number of references.
	*/
	unsigned int DecRef(unsigned int id,tObjType ObjType);

	/**
	* Get the number of objects of a given type that make a reference to a
	* concept.
	* @param id             Identificator of the concept.
	* @param ObjType        Type of the object.
	* @return unsigned int.
	*/
	unsigned int GetRef(unsigned int id,tObjType ObjType);

public:

	/**
	* Destructor of the dictionary.
	*/
	virtual ~GDict(void);

	friend class GLang;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
