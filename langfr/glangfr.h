/*

	GALILEI Research Project

	GLangFR.h

	French Language - Header.

	(C) 2001 by P. Francq and J. Lamoral.

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
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>


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
	RContainer<FrenchPorterRule,unsigned int,true,false>* Rules1;
    /**
	* List of Rules '2'.
	*/
	RContainer<FrenchPorterRule,unsigned int,true,false>* Rules2;
	/**
	* List of Rules '3'.
	*/
	RContainer<FrenchPorterRule,unsigned int,true,false>* Rules3;

public:

	/**
	* Constructor of the french language.
	*/
	GLangFR(void) throw(bad_alloc);

	/**
	* Function that return stemming of a word. The "French" Porter's algorithm is
	* basicly implemented.
	* @param _kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	virtual RStd::RString& GetStemming(const RStd::RString& _kwd);

private:

	/**
	* Test if the character is a "normal" vowel, i.e. if it is one of:
	* 'a', 'e', 'i', 'o' or 'u'.
	* @return True if the character is a "normal" vowel.
	*/
	inline bool IsVowel(char c)
	{
		return((c=='a')||(c=='e')||(c=='i')||(c=='o')||(c=='u')||(c=='é')||(c=='è')||(c=='ê')||(c=='à')||(c=='â'));
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
	bool ApplyRules(char* kwd,char* &end,RContainer<FrenchPorterRule,unsigned int,true,false>* rules,int ruleslevel);

public:

	/**
	* Destructor.
	*/
	virtual ~GLangFR(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
