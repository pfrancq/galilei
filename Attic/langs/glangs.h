/*

	GALILEI Research Project

	gwordref.h

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
#ifndef GLangsH
#define GLangsH


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>
#include <glangs/glang.h>
#include <glangs/gdicts.h>



//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------
class GDicts;
//---------------------------------------------------------------------------



// class GLangs
class GLangs : public RContainer<GLang,unsigned,true,true>
{
public:
  GLangs(unsigned nb) throw(bad_alloc);
  GLang* GetLang(const char* code);   // Return a pointer to the language
  virtual void Load(void) throw(bad_alloc,GException)=0;  //  Load the Languages
 // void CreateDicsStops(GDicts *dicts,GDicts *stops) throw(bad_alloc,GException);
	virtual ~GLangs(void) {}
};

}
#endif
