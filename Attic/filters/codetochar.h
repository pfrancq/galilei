/*

	R Project Library

	codetochar.h

	Main Window - Implementation.

	(C) 2001 by Lamoral Julien

	Version $Revision$

	Last Modify: $Date$


*/

//-----------------------------------------------------------------------------
#ifndef CODETOCHAR_H
#define CODETOCHAR_H






//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
/**
* The CodeToChar class provides  the representaion of a html code in char
*
* @author Lamoral julien
*/

class CodeToChar {


public:
	
	/**
	* Return the char who will be returned
	*/
	char Return;
	
	/**
	* Code the corresponding html code
	*/
	char* Code;
	
	
	/**
	* The Constructor
	* @param Returnn the char who will be returned
	* @param Coden the corresponding html code
	*/
	CodeToChar(char* Coden,char Returnn);
	
	/**
	* Utilised by RContainer
	*/
	int Compare(const char* Coder);
	
	/**
	* Utilised by RContainer
	*/
	int Compare(const CodeToChar& word);
	
	/**
	* Utilised by RContainer
	*/
	int Compare(const CodeToChar* word);

	
	
	/**
	* The Destructor
	*/
	~CodeToChar();

};

}
#endif
