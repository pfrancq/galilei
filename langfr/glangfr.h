/*

	GALILEI Research Project

	GLangFR.h

	French Language - Header.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#ifndef GLangFRH
#define GLangFRH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <infos/glang.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLangFR class provides a representation for the french language.
* @author Pascal Francq
* @short French language.
*/
class GLangFR : public GLang
{
	class FrenchPorterRule;

	/**
	* List of Rules '1'.
	*/
	R::RContainer<FrenchPorterRule,unsigned int,true,false>* Rules1;

    /**
	* List of Rules '2'.
	*/
	R::RContainer<FrenchPorterRule,unsigned int,true,false>* Rules2;

	/**
	* List of Rules '3'.
	*/
	R::RContainer<FrenchPorterRule,unsigned int,true,false>* Rules3;

public:

	/**
	* Constructor of the french language.
	* @param fac             Factory.
	*/
	GLangFR(GFactoryLang* fac) throw(bad_alloc);

	/**
	* Function that return stemming of a word. The "French" Porter's algorithm is
	* basicly implemented.
	* @param _kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	virtual R::RString& GetStemming(const R::RString& _kwd);

private:

	/**
	* Test if the character is a "normal" vowel, i.e. if it is one of:
	* 'a', 'e', 'i', 'o' or 'u'.
	* @return True if the character is a "normal" vowel.
	*/
	inline bool IsVowel(char c)
	{
		return((c=='a')||(c=='e')||(c=='i')||(c=='o')||(c=='u')||(c=='�')||(c=='�')||(c=='�')||(c=='�')||(c=='�'));
	}

	/**
	* Test if a word contains some vowels.
	* @param kwd            Word analyse.
	* @returns True if the word contains vowels.
	*/
	bool ContainsVowel(const char* kwd);

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
	bool ApplyRules(char* kwd,char* &end,R::RContainer<FrenchPorterRule,unsigned int,true,false>* rules,int ruleslevel);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	* Destructor.
	*/
	virtual ~GLangFR(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
