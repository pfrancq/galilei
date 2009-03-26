/*

	GALILEI Research Project

	GRelation.h

	Relation - Header.

	Copyright 2006-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef GRelation_H
#define GRelation_H


//------------------------------------------------------------------------------
// include files for R


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GRelation class provides a representation of a relation between two
* concepts. A relation is always defined by three elements:
* -# The subject implies in the relation.
* -# The type of the relation.
* -# The object of the relation.
* A relation can be seen as a directed graph with a edge representing the type
* of the relation between a node representing the subject and a node
* representing the object.
* @author Pascal Francq
* @short Concept Relation
*/
class GRelation
{
	/**
	* Identifier of the concept.
	*/
	size_t Id;

	/**
	* Name of the concept.
	*/
	R::RString Name;

	/**
	* Subject of the relation.
	*/
	GConcept* Subject;

	/**
	* Type of the concept.
	*/
	size_t Type;

	/**
	* Object of the relation.
	*/
	GConcept* Object;

	/**
	* Weight associated to the relation.
	*/
	double Weight;

public:

	/**
	* Constructor of a relation.
	* @param id              Identifier.
	* @param name            Name of the concept.
	* @param subject         Subject.
	* @param type            Type of the concept.
	* @param object          Object.
	* @param weight          Weight of the relation.
	*/
	GRelation(size_t id,const R::RString& name,GConcept* subject,size_t type,GConcept* object,double weight);

	/**
	* Constructor of a relation.
	* @param name            Name of the concept.
	* @param subject         Subject.
	* @param type            Type of the concept.
	* @param object          Object.
	* @param weight          Weight of the relation.
	*/
	GRelation(const R::RString& name,GConcept* subject,size_t type,GConcept* object,double weight);

	/**
	* Compare two relations to classify them.
	* @param relation        Relation to compare with.
	* @see R::RContainer.
	*/
	int Compare(const GRelation& relation) const;

	/**
	* Compare the identifier of a relation with a given identifier.
	* @param id              Identifier used to compare.
	* @see R::RContainer.
	*/
	int Compare(size_t id) const;

	/**
	* Set the Identificator of the word.
	* @param id              Identificator of the word.
	*/
	void SetId(size_t id);

	/**
	* @return Identificator of the word.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* @return A string representing the word.
	*/
	R::RString GetName(void) const {return(Name);}

	/**
	* Get the subject.
	* @return Pointer to a GConcept.
	*/
	GConcept* GetSubject(void) const {return(Subject);}

	/**
	* Get the type of the relation.
	*/
	size_t GetType(void) const {return(Type);}

	/**
	* Get the object.
	* @return Pointer to a GConcept.
	*/
	GConcept* GetObject(void) const {return(Object);}

	/**
	* Get the weight of the relation.
	*/
	double GetWeight(void) const {return(Weight);}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
