/*

	GALILEI Research Project

	GStatement.h

	Statement - Header.

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
#ifndef GStatement_H
#define GStatement_H


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GStatement class provides a representation of a statement in the form of
* subject-predicate-object where subject and object are concepts. A weight may
* be associated with each statement.
*
* A statement can be seen as a directed graph with a weighted edge representing a
* predicate between a node representing the subject and a node representing the
* object.
* @author Pascal Francq
* @short Statement
*/
class GStatement
{
	/**
	* Identifier of the statement.
	*/
	size_t Id;

	/**
	* Subject of the statement.
	*/
	GObject* Subject;

	/**
	* Predicate of the statement.
	*/
	GPredicate* Predicate;

	/**
	* Object of the statement.
	*/
	GObject* Object;

	/**
	* Weight associated to the statement.
	*/
	double Weight;

public:

	/**
	* Constructor of a statement.
	* @param id              Identifier.
	* @param subject         Subject.
	* @param predicate       Predicate.
	* @param object          Object.
	* @param weight          Weight.
	*/
	GStatement(size_t id,GObject* subject,GPredicate* predicate,GObject* object,double weight);

	/**
	* Compare two statements to classify them.
	* @param statement       Statement to compare with.
	* @see R::RContainer.
	*/
	int Compare(const GStatement& statement) const;

	/**
	* Set the Identifier of the statement.
	* @param id              Identifier of the statement.
	*/
	void SetId(size_t id);

	/**
	* @return Identifier of the statement.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Get the subject of the statement.
	* @return Pointer to a GConcept.
	*/
	GObject* GetSubject(void) const {return(Subject);}

	/**
	* Get the predicate of the statement.
	* @return Pointer to a GPredicate.
	*/
	GPredicate* GetPredicate(void) const {return(Predicate);}

	/**
	* Get the object of the statement.
	* @return Pointer to a GConcept.
	*/
	GObject* GetObject(void) const {return(Object);}

	/**
	* Get the weight of the statement.
	*/
	double GetWeight(void) const {return(Weight);}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
