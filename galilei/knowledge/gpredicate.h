/*

	GALILEI Research Project

	GPredicate.h

	Predicate - Header.

	Copyright 2006-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef GPredicate_H
#define GPredicate_H


//------------------------------------------------------------------------------
// include files for R
#include <robject.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GPredicate provides a representation for a predicate.
* @param author Pascal Francq
* @param short Predicate.
*/
class GPredicate : public R::RObject
{
	/**
	* Identifier of the predicate.
	*/
	size_t Id;

	/**
	* Short description of the predicate.
	*/
	R::RString Description;

	/**
	* Statements of the predicate.
	*/
	R::RContainer<GStatement,false,true> Statements;

public:

	/**
	* Construct a predicate.
	* @param id              Identifier of the predicate.
	* @param name            Name of the predicate.
	* @param desc            Short description.
	*/
	GPredicate(size_t id,const R::RString& name,const R::RString& desc);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GPredicate");}

	/**
	* Compare two predicates.
	* @param predicate            Predicate used.
	* @see R::RContainer.
	*/
	int Compare(const GPredicate& predicate) const;

	/**
	* Compare a relation with a name.
	* @param name            Name used.
	* @see R::RContainer.
	*/
	int Compare(const R::RString& name) const;

	/**
	* Get the identifier.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	 * Set the identifier.
	 * @param id             Identifier.
	 */
	void SetId(size_t id);

	/**
	* Get the description.
	*/
	R::RString GetDescription(void) const {return(Description);}

	/**
	* Get a cursor over the statements.
	* @return R::RCursor over GStatement
	*/
	R::RCursor<GStatement> GetStatements(void) const;

protected:

	/**
	* Insert a statement.
	* @param statement       Statement to insert.
	*/
	void InsertStatement(GStatement* statement);

public:

	/**
	* Destructor.
	*/
	virtual ~GPredicate(void);

	friend class GSession;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
