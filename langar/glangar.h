/*

	GALILEI Research Project

	GLangAR.h

	Arabic Language - Header.

	Copyright 2004 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).
		Sarah Rolfo (srolfo@ulb.ac.be).

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
#include <sessions/galilei.h>
#include <infos/glang.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLangAR class provides a representation for the arabic language.
* @author David Wartel
* @short Arabic Language.
*/
class GLangAR : public GLang
{
	class ArabicRule;

	/**
	* List of Rules '1a'.
	*/
	R::RContainer<ArabicRule,unsigned int,true,false>* Rules1;

	/**
	* List of Rules '2'.
	*/
	R::RContainer<ArabicRule,unsigned int,true,false>* Rules2;

	/**
	* List of Rules '3'.
	*/
	R::RContainer<ArabicRule,unsigned int,true,false>* Rules3;

	/**
	* List of Rules '4'.
	*/
	R::RContainer<ArabicRule,unsigned int,true,false>* Rules4;

	/**
	* List of Rules '5'.
	*/
	R::RContainer<ArabicRule,unsigned int,true,false>* Rules5;

	/**
	* List of Rules '6'.
	*/
	R::RContainer<ArabicRule,unsigned int,true,false>* Rules6;

	/**
	* List of Rules '7'.
	*/
	R::RContainer<ArabicRule,unsigned int,true,false>* Rules7;


public:

	/**
	* Constructor of the english language.
	* @param fac             Factory.
	*/
	GLangAR(GFactoryLang* fac) throw(std::bad_alloc,GException);

	/**
	* Function that return stemming of a word. The Porter's algorithm is
	* implemented.
	* @param _kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	virtual R::RString GetStemming(const R::RString& _kwd) throw(GException);

private:

	/**
	* Load the rules from a unicode textfile ('utf-8' enconded).
	* Data in textfile are separated by semi-colon.
	*/
	void LoadRules(void) throw(GException);

	/**
	* Apply the different rules of the porter's algorithm to a gieven word.
	* @param kwd            Pointer to the begin of the word.
	* @param rules          Rules to apply.
	* @return True if a rule applied needs a next step to be execute.
	*/
	void ApplyRules(R::RString kwd,R::RContainer<ArabicRule,unsigned int,true,false>* rules);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	* Destructor.
	*/
	virtual ~GLangAR(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
