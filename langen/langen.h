/*

	GALILEI Research Project

	LangEN.h

	English Language - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GLangENH
#define GLangENH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glang.h>


//-----------------------------------------------------------------------------
/**
* The GLangEN class provides a representation for the english language.
* @author Pascal Francq
* @short English Language.
*/
class GLangEN : public GALILEI::GLang
{
	class PorterRule;

	/**
	* List of Rules '1a'.
	*/
	R::RContainer<PorterRule,true,false>* Rules1a;

	/**
	* List of Rules '1b'.
	*/
	R::RContainer<PorterRule,true,false>* Rules1b;

	/**
	* List of Rules '1bb'.
	*/
	R::RContainer<PorterRule,true,false>* Rules1bb;

	/**
	* List of Rules '1c'.
	*/
	R::RContainer<PorterRule,true,false>* Rules1c;

	/**
	* List of Rules '2'.
	*/
	R::RContainer<PorterRule,true,false>* Rules2;

	/**
	* List of Rules '3'.
	*/
	R::RContainer<PorterRule,true,false>* Rules3;

	/**
	* List of Rules '4'.
	*/
	R::RContainer<PorterRule,true,false>* Rules4;

	/**
	* List of Rules '5a'.
	*/
	R::RContainer<PorterRule,true,false>* Rules5a;

	/**
	* List of Rules '5b'.
	*/
	R::RContainer<PorterRule,true,false>* Rules5b;

public:

	/**
	* Constructor of the english language.
	* @param fac             Factory.
	*/
	GLangEN(GALILEI::GFactoryLang* fac);

	/**
	* Function that return stemming of a word. The Porter's algorithm is
	* implemented.
	* @param _kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	virtual R::RString GetStemming(const R::RString& _kwd);

private:

	/**
	* Test if the character is a "normal" vowel, i.e. if it is one of:
	* 'a', 'e', 'i', 'o' or 'u'.
	* @return True if the character is a "normal" vowel.
	*/
	inline bool IsVowel(char c)
	{
		return((c=='a')||(c=='e')||(c=='i')||(c=='o')||(c=='u'));
	}

	/**
	* Test if a word contains some vowels.
	* @param kwd            Word analyse.
	* @returns True if the word contains vowels.
	*/
	bool ContainsVowel(const char* kwd);

	/**
	* Test if the word is ending with a combinaison of CVC.
	* @param kwd            Word to analyse.
	* @param end            Pointer that will hold the end of the word.
	* @returns True if the word is ending with CVC.
	*/
	bool EndsWithCVC(char* kwd,char* &end);

	/**
	* Count the syllables in a way describes in Porter. To compute this, a
	* finite state machine is used.
	* @param kwd            Word to analyse.
	* @return Number of syllables.
	*/
	int GetWordSize(char* kwd);

	/**
	* Apply the different rules of the porter's algorithm to a gieven word.
	* @param kwd            Pointer to the begin of the word.
	* @param end            Pointer that will hold the end of the word.
	* @param rules          Rules to apply.
	* @return True if a rule applied needs a next step to be execute.
	*/
	bool ApplyRules(char* kwd,char* &end,R::RContainer<PorterRule,true,false>* rules);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GLangEN(void);
};


//-----------------------------------------------------------------------------
#endif
