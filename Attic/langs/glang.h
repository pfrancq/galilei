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
#ifndef GLangH
#define GLangH




#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei

#include <galilei.h>



//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// class GLang
class GLang
{
public:
  RString Lang;               // Language
  char Code[3];               // The code used in the databases
  bool Activ;                 // Defines if the language is activ

  // Constructor
  GLang(const RString& lang,const char* code) throw(bad_alloc);

  // Function needed by GLangs (-> RContainer)
  int Compare(const GLang& lang);
  int Compare(const GLang* lang);

  // Dictionnay and Stop List names
  virtual RString& GetDic(void)=0;
  virtual RString& GetStop(void)=0;

	// Destructor
	virtual ~GLang(void) {}
};

}
 #endif
