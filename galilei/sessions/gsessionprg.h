/*

	GALILEI Research Project

	GSessionPrg.h

	Program to run on a Session - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GSessionPrgH
#define GSessionPrgH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rprg/rprgfunc.h>
#include <rprg/rprgclass.h>
#include <rprg/rprg.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GPrgSessionClass provides a class for a GALILEI session.
* @author Pascal Francq
* @short GALILEI Session.
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
	* Number of histories stored.
	*/
	unsigned int NbHistory;

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
	* First profile computing has be done.
	*/
	bool FirstProfile;

	/**
	* First groupment has be done.
	*/
	bool FirstGroup;

	/**
	* When Autosave is true, after each computing, the results is saved.
	*/
	bool AutoSave;

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
	GPrgClassSession(GSession* s) throw(bad_alloc);

	/**
	* Destructor.
	*/
	virtual ~GPrgClassSession(void);
};


//-----------------------------------------------------------------------------
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
	* @param r              Receiver.
	*/
	GSessionPrg(R::RString f,GSession* s,R::RPrgOutput* o) throw(bad_alloc,GException);

	/**
	* Destructor.
	*/
	virtual ~GSessionPrg(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
