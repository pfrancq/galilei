/*

	GALILEI Research Project

	gwordoccur.h

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
#ifndef GWordOccurH
#define GWordOccurH


//---------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>
#include <gdocs/gdoc.h>
#include <gdocs/gwordref.h>

using namespace GALILEI;

//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------

class GDoc;

//---------------------------------------------------------------------------
// class GWordOccur

class GWordOccur : public GWordRef

{
public:

  GDoc *Doc;
  unsigned Occur;

  GWordOccur(void);
  GWordOccur(unsigned id);
  int Compare(const GWordOccur& ref);
  int Compare(GWordOccur *ref);
  int Compare(unsigned id);
};



}  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
