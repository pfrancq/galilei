/*

	GALILEI Research Project

	CodeToChar.h

	Correspondance between Code and characters in HTML - Header.

	(C) 2001 by Pascal Francq and Lamoral Julien

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



//-----------------------------------------------------------------------------
#ifndef CodeToCharH
#define CodeToCharH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The CodeToChar class provides a representation for a correspondance between
* a HTML code (&...;) and a specific character.
* @author Pascal Francq and Julien Lamoral.
* @hsort HTML Code/Character Correspondance.
*/
class CodeToChar
{
	/**
	* Character corresponding to the code.
	*/
	char Char;
	
	/**
	* Code.
	*/
	RStd::RString Code;

public:

	/**
	* The Constructor.
	* @param code           Code.
	* @param c              Character.
	*/
	CodeToChar(const char* code,char c) throw(bad_alloc);

	/**
	* Utilised by RStd::RContainer
	*/
	int Compare(const char* code) const;

	/**
	* Utilised by RStd::RContainer
	*/
	int Compare(const CodeToChar& code) const;

	/**
	* Utilised by RStd::RContainer
	*/
	int Compare(const CodeToChar* code) const;

	/**
	* Get the characters corresponding to this code.
	* @returns char.
	*/
	char GetChar(void) const {return(Char);}

	/**
	* Get The len of the code.
	* @returns int.
	*/
	int GetLen(void) const {return(Code.GetLen());}

	/**
	* The Destructor.
	*/
	~CodeToChar(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
