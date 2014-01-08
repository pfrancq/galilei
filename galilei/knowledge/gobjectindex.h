/*

	GALILEI Research Project

	GObjectIndex.h

	Object Index - Header.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef GObjectIndexH
#define GObjectIndexH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
class GObjectSyntacticPos
{
	 GConcept* Concept;
	 R::RNumContainer<size_t,true> Pos;
public:
	 GObjectSyntacticPos(void);
	 int Compare(const GObjectSyntacticPos& obj) const;
};


//------------------------------------------------------------------------------
/**
 * The GObjectIndex provides a representation for a object list and, eventually,
 * the syntactic positions of the occurrences of concepts in these objects.
 * @author Pascal Francq
 * @short Object Index
 */
class GObjectIndex
{
	/**
	 * The identifiers of the objects.
	 */
	R::RNumContainer<size_t,true> Objs;

	/**
	 * Syntactic positions of the concepts.
	 */
	R::RContainer<GObjectSyntacticPos,true,true> SyntacticPos;

public:

	 GObjectIndex(size_t nb);
	 ~GObjectIndex(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
