/*

	GALILEI Research Project

	GSessionPrg.h

	Program running a Session object - Header.

	Copyright 2002-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef GSessionPrgH
#define GSessionPrgH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprgfunc.h>
#include <rprgclass.h>
#include <rprg.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GPrgSessionClass provides a representation of a class of a GALILEI
* session for the internal scripting language.
* @author Pascal Francq
* @short GALILEI Session Script Class.
*/
class GPrgClassSession : public R::RPrgClass
{
public:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Name of the current test.
	*/
	R::RString TestName;

	/**
	* Output file.
	*/
	R::RTextFile* OFile;

	/**
	* Graph Output file.
	*/
	R::RTextFile* GOFile;

	/**
	* Statistics Output file.
	*/
	R::RTextFile* SOFile;

	/**
	* Documents Statistics Output file.
	*/
	R::RTextFile* DSOFile;

	/**
	* Precision of the current groupement.
	*/
	double Precision;

	/**
	* Recall of the current groupement.
	*/
	double Recall;

	/**
	* Total comparaison between for the current groupment.
	*/
	double Total;

	/**
	* Percentage of correct assignments for the subprofiles last added.
	*/
	double PercAss;

	/**
	* When TrackNewProfile is true, the system
	*/
	bool TrackNewProfiles;

	/**
	* Clock Reference used to measure laps of execution time.
	*/
	time_t ClockRef;

	/**
	* Create a class.
	* @param s              Session.
	*/
	GPrgClassSession(GSession* s);

	/**
	* Destructor.
	*/
	virtual ~GPrgClassSession(void);
};


//------------------------------------------------------------------------------
/**
* The GSessionPrg provides a class for executing a GALILEI Program.
* @author Pascal Francq
* @short GALILEI Program.
*/
class GSessionPrg : public R::RPrg
{
public:

	/**
	* Constructor.
	* @param f              Name of the file.
	* @param s              Session.
	* @param o              Generic output class.
	*/
	GSessionPrg(R::RString f,GSession* s,R::RPrgOutput* o);

	/**
	* Destructor.
	*/
	virtual ~GSessionPrg(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
