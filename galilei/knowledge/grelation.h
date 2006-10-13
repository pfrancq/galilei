/*

	GALILEI Research Project

	GRelation.h

	Relation - Header.

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
* -# The subject implies in the relation (id and language).
* -# The type of the relation.
* -# The object of the relation (id and language).
* A relation can be seen as a directed graph with a edge representing the type
* of the relation between a node representing the subject and a node
* representing the object.
* @author Pascal Francq
* @short Concept Relation
*/
class GRelation
{
	/**
	* Identifier of the subject of the relation.
	*/
	unsigned int SubjectId;

	/**
	* Language of the subject of the relation.
	*/
	GLang* SubjectLang;

	/**
	* Type of the concept.
	*/
	unsigned int Type;

	/**
	* Identifier of the object of the relation.
	*/
	unsigned int ObjectId;

	/**
	* Language of the object of the relation.
	*/
	GLang* ObjectLang;

public:

	/**
	* Constructor of a relation.
	* @param subjectid       Identifier of the subject.
	* @param subjectlang     Language of the subject.
	* @param type            Type of the concept.
	* @param objectid        Identifier of the object.
	* @param objectlang      Language of the object.
	*/
	GRelation(unsigned int subjectid,GLang* subjectlang,unsigned int type,unsigned int objectid,GLang* objectlang);

	/**
	* Compare two relations to classify them.
	* @param relation        Relation to compare with.
	* @see R::RContainer.
	*/
	int Compare(const GRelation& relation) const;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
