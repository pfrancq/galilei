/*

	GALILEI Research Project

	GSessionPrg.h

	Program to run on a Session - Header.

	(C) 2002 by P. Francq.

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
#include <rio/rtextfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GSession provides a representation for a GALILEI session. The way the
* data are stored and loaded is dedicated to a child class of GSession through
* a set of pure virtual methods defined.
* @author Pascal Francq
* @short Generic Session.
*/
class GSessionPrg
{
	class Inst;
	class InstType;

protected:

	/**
	* Represent the different instruction allowed.
	*/
	enum tInst{Log,Output,Sql,Comp,Group,Ideal,Test,CmpIdeal,Fdbks};

	/**
	* Name of the file to execute.
	*/
	RStd::RString FileName;

	/**
	* Session on which execute the file.
	*/
	GSession* Session;

	/**
	* Receiver of the signal.
	*/
	GSlot* Rec;

	/**
	* List of all possible "instructions" to execute.
	*/
	RStd::RContainer<InstType,unsigned int,true,true> InstTypes;

	/**
	* List of all "Instructions" to execute.
	*/
	RStd::RContainer<Inst,unsigned int,true,false> Insts;

	/**
	* Name of the current test.
	*/
	RStd::RString TestName;

	/**
	* Output file.
	*/
	RIO::RTextFile* OFile;

	/**
	* Ideal Groups.
	*/
	RContainer<GGroups,unsigned int,true,true>* Groups;

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
	* Method to create an ideal clustering.
	*/
	GIdealGroup* IdealMethod;

	/**
	* Method to create a feedback cycle.
	*/
	GGetFeedback* FdbksMethod;

	/**
	* Container used to create subjects catagories.
	*/
	RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* Parents;

	/**
	* First profile computing has be done.
	*/
	bool FirstProfile;

	/**
	* First groupment has be done.
	*/
	bool FirstGroup;

public:

	/**
	* Constructor.
	* @param f              Name of the file.
	* @param s              Session.
	* @param r              Receiver.
	*/
	GSessionPrg(RString f,GSession* s,GSlot* r) throw(bad_alloc,GException);

protected:

	/**
	* Load the groups from the file.
	* @param filename       Name of the file containing the ideal groupement.
	*/
	void LoadGroups(const char* filename) throw(GException);

	/**
	* Run an instruction.
	*/
	void Run(const Inst* i) throw(GException);

public:

	/**
	* Execute a "program" for the current session.
	*/
	void Exec(void) throw(GException);

	/**
	* Destructor.
	*/
	virtual ~GSessionPrg(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
