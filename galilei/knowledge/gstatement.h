/*

	GALILEI Research Project

	GStatement.h

	Statement - Header.

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
* a triple of objects (concept, concepts types, documents, profiles, etc.).The
* first element represents the subject, the second element is the predicate and
* the third element represents the value.
*
* It can be used, for example, to represent a herarchical relation between two
* concepts. Suppose that the two concepts "car" and "electric car" are defined.
* It is then possible to define a third concept "IsA" and a new statement
* ("electric car","IsA","car").
*
* In the case that two concepts are linked by a symetric relation (such as a
* synonym), it is the responsible of the developer to take this into account.
* In practice, two solutions exist. First, two statements can be created where
* each concept is first a subject and then a value. Second, a coherent
* convention is adopted for naming concepts that are predicates in order to help
* the developer to determine if a given predicate is birectionnal or not. The
* first solution is simplier but consumes more memory.
*
* The statements can be seen as a directed graph with a weighted edge
* representing an object between nodes also representing objects.
*
* The GKB class maintains two lists of statements: "normal" ones that are
* stored, and "internal" ones that are just created in memory (for example
* between a GConceptList and its GConcept).
* @author Pascal Francq
* @short Statement
*/
class GStatement
{
private:

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
	GObject* Predicate;

	/**
	* Value of the statement.
	*/
	GObject* Value;

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
	* @param value           Value.
	* @param weight          Weight.
	*/
	GStatement(size_t id,GObject* subject,GObject* predicate,GObject* value,double weight);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GStatement");}

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
	* @return Pointer to a GObject.
	*/
	GObject* GetSubject(void) const {return(Subject);}

	/**
	* Get the predicate of the statement.
	* @return Pointer to a GObject.
	*/
	GObject* GetPredicate(void) const {return(Predicate);}

	/**
	* Get the value of the statement.
	* @return Pointer to a GObject.
	*/
	GObject* GetValue(void) const {return(Value);}

	/**
	* Get the weight of the statement.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	 * Set a new weight.
    * @param weight         Weight to assign.
    */
	void SetWeight(double weight);

	/**
	 * Destructor.
	 */
	virtual ~GStatement(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
