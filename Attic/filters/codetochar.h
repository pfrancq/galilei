/*

	GALILEI Research Project

	CodeToChar.h

	Correspondance between Code and characters in HTML - Header.

	(C) 2001 by Pascal Francq and Lamoral Julien

	Version $Revision$

	Last Modify: $Date$


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
