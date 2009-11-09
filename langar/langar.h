/*

	GALILEI Research Project

	LangAR.h

	Arabic Language - Header.

	Copyright 2004-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2004 by David Wartel.
	Copyright 2004 by Sarah Rolfo.
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GLangARH
#define GLangARH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glang.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLangAR class provides a representation for the Arabic language.
* @author David Wartel
* @short Arabic Language.
*/
class GLangAR : public GLang
{
	class ArabicRule;

	/**
	* List of Rules '0' (vowels rules)
	*/
	R::RContainer<ArabicRule,true,false> Rules0;

	/**
	* List of Rules '1a'.
	*/
	R::RContainer<ArabicRule,true,false> Rules1;

	/**
	* List of Rules '2'.
	*/
	R::RContainer<ArabicRule,true,false> Rules2;

	/**
	* List of Rules '3'.
	*/
	R::RContainer<ArabicRule,true,false> Rules3;

	/**
	* List of Rules '4'.
	*/
	R::RContainer<ArabicRule,true,false> Rules4;

	/**
	* List of Rules '5'.
	*/
	R::RContainer<ArabicRule,true,false> Rules5;

	/**
	* List of Rules '6'.
	*/
	R::RContainer<ArabicRule,true,false> Rules6;

	/**
	* List of Rules '7'.
	*/
	R::RContainer<ArabicRule,true,false> Rules7;

	/*
	* rules file.
	*/
	R::RString RulesFile;

public:

	/**
	* Constructor of the Arabic language.
	* @param fac             Factory.
	*/
	GLangAR(GPlugInFactory* fac);

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
	* Apply the different rules of the porter's algorithm to a gieven word.
	* @param kwd            Pointer to the begin of the word.
	* @param rules          Rules to apply.
	* @return True if a rule applied needs a next step to be execute.
	*/
	void ApplyRules(R::RString& kwd,R::RContainer<ArabicRule,true,false>& rules);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GLangAR(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
