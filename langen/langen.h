/*

	GALILEI Research Project

	LangEN.h

	English Language - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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
using namespace GALILEI;


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
	R::RContainer<PorterRule,true,false> Rules1a;

	/**
	* List of Rules '1b'.
	*/
	R::RContainer<PorterRule,true,false> Rules1b;

	/**
	* List of Rules '1bb'.
	*/
	R::RContainer<PorterRule,true,false> Rules1bb;

	/**
	* List of Rules '1c'.
	*/
	R::RContainer<PorterRule,true,false> Rules1c;

	/**
	* List of Rules '2'.
	*/
	R::RContainer<PorterRule,true,false> Rules2;

	/**
	* List of Rules '3'.
	*/
	R::RContainer<PorterRule,true,false> Rules3;

	/**
	* List of Rules '4'.
	*/
	R::RContainer<PorterRule,true,false> Rules4;

	/**
	* List of Rules '5a'.
	*/
	R::RContainer<PorterRule,true,false> Rules5a;

	/**
	* List of Rules '5b'.
	*/
	R::RContainer<PorterRule,true,false> Rules5b;

public:

	/**
	* Constructor of the English language.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GLangEN(GSession* session,GPlugInFactory* fac);

	/**
	 * Fill a container with all the stop words associated with the language.
	 * @param stop           Container.
	 */
	virtual void GetStopWords(R::RContainer<R::RString,true,false>& stop);

	/**
	* Function that return stemming of a word. The Porter's algorithm is
	* implemented.
	* @param _kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	virtual R::RString GetStemming(const R::RString& kwd);

private:

	/**
	* Test if the character is a "normal" vowel, i.e. if it is one of:
	* 'a', 'e', 'i', 'o' or 'u'.
	* @return True if the character is a "normal" vowel.
	*/
	inline bool IsVowel(R::RChar c)
	{
		return((c=='a')||(c=='e')||(c=='i')||(c=='o')||(c=='u'));
	}

	/**
	* Test if a word contains some vowels.
	* @param kwd            Word analyze.
	* @returns True if the word contains vowels.
	*/
	bool ContainsVowel(const R::RString& kwd);

	/**
	* Test if the word is ending with a combination of CVC.
	* @param kwd            Word to analyze.
	* @returns True if the word is ending with CVC.
	*/
	bool EndsWithCVC(const R::RString& kwd);

	/**
	* Count the syllables in a way describes by Porter. To method uses a finite
	* state machine to compute it.
	* @param kwd            Word to analyze.
	*/
	int GetWordSize(const R::RString& kwd);

	/**
	* Apply the different rules of the porter's algorithm to a given word.
	* @param kwd            Pointer to the begin of the word.
	* @param end            Pointer that will hold the end of the word.
	* @param rules          Rules to apply.
	* @return True if a rule applied needs a next step to be execute.
	*/
	bool ApplyRules(R::RString& kwd,R::RContainer<PorterRule,true,false>& rules);

public:

	/**
	* Create the parameters.
	*/
	static void CreateParams(GPlugInFactory* fac);

	/**
	* Destructor.
	*/
	virtual ~GLangEN(void);
};


//-----------------------------------------------------------------------------
#endif
