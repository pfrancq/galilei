/*

	GALILEI Research Project

	gwordoccurs.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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

//---------------------------------------------------------------------------
#ifndef GWordOccursH
#define GWordOccursH


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>
#include <gdocs/gdoc.h>
#include <gdocs/gwordoccur.h>


//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------

  class GWordOccur;
 class GDoc;
//---------------------------------------------------------------------------
// class GWordOccurs
class GWordOccurs : public RContainer<GWordOccur,unsigned,true,true>
{
	GDoc *Owner;

public:
	GWordOccurs(GDoc *owner,unsigned nb) throw(bad_alloc);
	void Insert(unsigned id,GDoc *doc) throw(bad_alloc);
	GWordOccur* GetPtr(unsigned id,GDoc *doc) throw(bad_alloc);
};





}  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
