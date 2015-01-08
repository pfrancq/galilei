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
* \f$P(x_i,x_j,w_{i,j,p})\f$ where:
* - \f$P\f$ is a GPredicate.
* - \f$x_1\f$ and \f$x_2\f$ are any GObject (concept, concepts types, documents
*   , profiles, etc.).
* - \f$w_{i,j,p}\f$ is weight associated to that statement.
*
* The statements can be seen as a directed graph with a weighted edge
* representing a predicate between nodes representing the objects.
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
	* Predicate of the statement.
	*/
	GPredicate* Predicate;

	/**
	* Xi of the statement.
	*/
	GObject* Xi;

	/**
	* Xj of the statement.
	*/
	GObject* Xj;

	/**
	* Weight associated to the statement.
	*/
	double Weight;

public:

	/**
	* Constructor of a statement.
	* @param id              Identifier.
	* @param predicate       Predicate.
	* @param xi              \f$x_i\f$.
	* @param xj              \f$x_j\f$.
	* @param weight          Weight.
	*/
	GStatement(size_t id,GPredicate* predicate,GObject* xi,GObject* xj,double weight);

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
	* Get the predicate of the statement.
	* @return Pointer to a GPredicate.
	*/
	GPredicate* GetPredicate(void) const {return(Predicate);}

	/**
	* Get the \f$x_i\f$ of the statement.
	* @return Pointer to a GConcept.
	*/
	GObject* GetXi(void) const {return(Xi);}

	/**
	* Get the \f$x_j\f$ of the statement.
	* @return Pointer to a GConcept.
	*/
	GObject* GetXj(void) const {return(Xj);}

	/**
	* Get the weight of the statement.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	 * Destructor.
	 */
	virtual ~GStatement(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
