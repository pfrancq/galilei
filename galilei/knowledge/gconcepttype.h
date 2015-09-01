/*

	GALILEI Research Project

	GConceptType.h

	Concept Type - Header.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GConceptType_H
#define GConceptType_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rhashcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glang.h>
#include <gobject.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GConceptType provides a representation for a given type of concepts. It
* also manage a dictionary of concepts, each concept having its own identifier.
* The name of a concept is unique for a given type, while the identifiers are
* unique for all concepts.
* @param author Pascal Francq
* @param short Concept Type.
*/
class GConceptType : public GObject
{
   /**
    * Category.
    */
   tConceptCat Category;

	/**
	 * The neutral concept;
	 */
	GConcept* Neutral;

	/**
	 * Concepts.
	 */
	R::RContainer<GConcept,false,false> Concepts;

	/**
	* Short description of the type.
	*/
	R::RString Description;

public:

	/**
	* Construct a concept type.
	* @param session         Session.
   * @param cat             Category.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	* @param s               Size of the second hash table.
	*/
	GConceptType(GSession* session,tConceptCat cat,size_t id,const R::RString& name,const R::RString& desc,size_t s);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GConceptType");}

	/**
	* Compare two concepts types.
	* @param type            Concept type used.
	* @see R::RContainer.
	*/
	int Compare(const GConceptType& type) const;

	/**
	* Compare two concepts types.
	* @param type            Concept type used.
	* @see R::RContainer.
	*/
	int Compare(const GConceptType* type) const;

	/**
	* Compare a concept type with a name.
	* @param name            Name used.
	* @see R::RContainer.
	*/
	int Compare(const R::RString& name) const;

   /**
    * @return the category.
    */
   tConceptCat GetCategory(void) const {return(Category);}

	/**
	* @return the description.
	*/
	R::RString GetDescription(void) const {return(Description);}

	/**
    * @return the neutral concept.
    */
	GConcept* GetNeutral(void) const;

	/**
	* @return a cursor over the main hash table.
	* @param obj             Pseudo-parameter.
	*/
	R::RCursor<GConcept> GetObjs(const GConcept* obj) const {return(R::RCursor<GConcept>(Concepts));}

	/**
	* Look if a given concept is in the dictionary.
	* @param obj             Pseudo-parameter.
	* @param name            Name of the concept to look for.
	* @return true if the concept is in the dictionary.
	*/
	bool IsIn(const GConcept* obj,const R::RString& name) const;

   /**
	* Get a given concept from the dictionary.
	* @param obj             Pseudo-parameter.
	* @param name            Name of the concept to look for.
	* @param null            If set to true, if the concept does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to the concept.
	*/
	GConcept* GetObj(const GConcept* obj,const R::RString& name,bool null) const;

private:

	/**
	* Insert a concept in the dictionary.
	* @param concept         Concept.
	*/
	void InsertObj(GConcept* concept);

	/**
	* Delete a concept from the dictionary.
	* @param concept         Concept.
	*/
	void DeleteObj(GConcept* concept);

public:

	/**
	* Get the total number of concepts.
	* @param obj             Pseudo-parameter.
	*/
	inline size_t GetNbObjs(const GConcept* obj) const {return(Concepts.GetNb());
}

	/**
	 * Destruct.
	 */
	virtual ~GConceptType(void);

	friend class GConcept;
	friend class GKB;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
