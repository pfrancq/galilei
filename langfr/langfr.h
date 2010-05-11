/*

	GALILEI Research Project

	LangFR.h

	French Language - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001 by Marjorie Paternostre and Julien Lamoral.
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
#ifndef GLangFRH
#define GLangFRH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glang.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
* The GLangFR class provides a representation for the French language.
* @author Pascal Francq
* @short French language.
*/
class GLangFR : public GALILEI::GLang
{
	class FrenchPorterRule;

	/**
	* List of Rules '1'.
	*/
	R::RContainer<FrenchPorterRule,true,false> Rules1;

    /**
	* List of Rules '2'.
	*/
	R::RContainer<FrenchPorterRule,true,false> Rules2;

	/**
	* List of Rules '3'.
	*/
	R::RContainer<FrenchPorterRule,true,false> Rules3;

public:

	/**
	* Constructor of the French language.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GLangFR(GSession* session,GPlugInFactory* fac);

	/**
	 * Fill a container with all the stop words associated with the language.
	 * @param stop           Container.
	 */
	static void GetStopWords(R::RContainer<R::RString,true,false>& stop);

	/**
	* Function that return stemming of a word. The "French" Porter's algorithm is
	* basically implemented.
	* @param kwd             Word to find the stemming.
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
		return((c=='a')||(c=='e')||(c=='i')||(c=='o')||(c=='u')||
				(c==R::RChar(165))||
				((c>=R::RChar(129))&&(c<=R::RChar(151)))||
				(c==R::RChar(153))||
				(c==R::RChar(154))||
				((c>=R::RChar(160))&&(c<=R::RChar(165))));
	}

	/**
	* Count the syllables in a way describes by Porter. To method uses a finite
	* state machine to compute it.
	* @param kwd            Word to analyze.
	*/
	int GetWordSize(const R::RString& kwd);

	/**
	* Apply the different rules of the porter's algorithm to a given word.
	* @param kwd            Word.
	* @param rules          Rules to apply.
	* @return True if a rule applied needs a next step to be execute.
	*/
	bool ApplyRules(R::RString& kwd,R::RContainer<FrenchPorterRule,true,false>& rules);

public:

	/**
	* Create the parameters.
	*/
	static void CreateParams(GPlugInFactory* fac);

	/**
	* Destruct.
	*/
	virtual ~GLangFR(void);
};


//-----------------------------------------------------------------------------
#endif
