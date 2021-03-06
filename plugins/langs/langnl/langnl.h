/*

	GALILEI Research Project

	LangNL.h

	Dutch Language - Header.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2001 by Nicolas Kumps.
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
#ifndef GLangNLH
#define GLangNLH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glang.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
* The GLangNL class provides a representation for the Dutch language.
*
* It is based on the paper "Porter's stemming algorithm for Dutch" from Wessel
* Kraaij et Renée Pohlmann.
* @author Pascal Francq
* @short Duth Language.
*/
class GLangNL : public GALILEI::GLang
{
	class DutchPorterRule;

	/**
	* List of Rules '1a'.
	*/
	R::RContainer<DutchPorterRule,true,false> Rules1a;

	/**
	* List of Rules '1b'.
	*/
	R::RContainer<DutchPorterRule,true,false> Rules1b;

	/**
	* List of Rules '1c'.
	*/
	R::RContainer<DutchPorterRule,true,false> Rules1c;

	/**
	* List of Rules '2a'.
	*/
	R::RContainer<DutchPorterRule,true,false> Rules2a;

	/**
	* List of Rules '2aa'.
	*/
	R::RContainer<DutchPorterRule,true,false> Rules2aa;

	/**
	* List of Rules '3a'.
	*/
	R::RContainer<DutchPorterRule,true,false> Rules3a;

	/**
	* List of Rules '3b'.
	*/
	R::RContainer<DutchPorterRule,true,false> Rules3b;

	/**
	* List of Rules '4'.
	*/
	R::RContainer<DutchPorterRule,true,false> Rules4;

	/**
	* List of Rules '5'.
	*/
	R::RContainer<DutchPorterRule,true,false> Rules5;

public:

	/**
	* Constructor of the Dutch language.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GLangNL(GSession* session,GPlugInFactory* fac);

	/**
	 * Fill a container with all the stop words associated with the language.
	 * @param stop           Container.
	 */
	static void GetStopWords(R::RContainer<R::RString,true,false>& stop);

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
	* Test if a word contains some vowels.
	* @param kwd            Word analyze.
	* @returns True if the word contains vowels.
	*/
	bool ContainsVowel(const R::RString& kwd);

	/**
	* Test if the word after stemming ends with a non-vowel.
	* @param newend            End of the word.
	*/
	bool EndsWithCons(const R::RString& kwd);

	/**
	* Test if the word after stemming ends with a 'c'.
	* @param newend            End of the word.
	*/
	bool EndsWithc(const R::RString& kwd);

	/**
	* Test if the word after stemming ends with a 'e'
	* @param newend            End of the word.
	*/
	bool EndsWithe(const R::RString& kwd);

	/**
	* Test if the word is ending with a combination of CVD (D = non vowel
	* other than l).
	* @param kwd            Word to analyze.
	* @returns True if the word is ending with CVD.
	*/
	bool EndsWithCVD(const R::RString& kwd);

	/**
	* Count the syllables in a way describes in Porter. To compute this, a
	* finite state machine is used.
	* @param kwd            Word to analyze.
	* @return Number of syllables.
	*/
	int GetWordSize(const R::RString& kwd);

	/**
	* Apply the different rules of the porter's algorithm to a given word.
	* @param kwd            Pointer to the begin of the word.
	* @param end            Pointer that will hold the end of the word.
	* @param rules          Rules to apply.
	* @return True if a rule applied needs a next step to be execute.
	*/
	bool ApplyRules(R::RString& kwd,R::RContainer<DutchPorterRule,true,false>& rules);

public:

	/**
	* Create the parameters.
	*/
	static void CreateParams(GPlugInFactory*);

	/**
	* Destruct.
	*/
	virtual ~GLangNL(void);
};


//-----------------------------------------------------------------------------
#endif
