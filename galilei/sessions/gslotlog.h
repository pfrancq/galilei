/*

	GALILEI Research Project

	GSlotLogLog.h

	Log File Slot - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GSlotLogH
#define GSlotLogH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rtextfile.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gslot.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The GSlotLog class provides a receiver of signals of a GALILEI session using a
* log file to store the outputs.
* @author Pascal Francq
* @short Log File Slot.
*/
class GSlotLog : public GSlot, public R::RTextFile
{
	/**
	* Internal Buffer.
	*/
	char Buf[500];

public:

	/**
	* Constructor.
	* @param name            Name of the file.
	*/
	GSlotLog(const char* name) throw(bad_alloc,R::RException);

	/**
	* The traitment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void receiveNextDoc(const GDoc* doc) throw(bad_alloc,R::RException);

	/**
	* The traitment for a specific document will begin.
	* @param prof           Profile.
	*/
	virtual void receiveNextProfile(const GProfile* prof) throw(bad_alloc,R::RException);

	/**
	* Method called by GGrouping each time a new language is analysed.
	* @param lang           Pointer to the current lang.
	*/
	virtual void NextGroupLang(const GLang* lang) throw(bad_alloc,R::RException);

	/**
	* Method called when executing a sequence of instruction to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const char* str) throw(bad_alloc,R::RException);

	/**
	* The traitment for a specific chromosome will begin.
	* @param id             Identificator of the chromosome.
	*/
	virtual void receiveNextChromosome(unsigned int id) throw(bad_alloc,R::RException);

	/**
	* The traitment for a specific method will begin.
	* @param id             The id describing the step in the method.
	*/
	virtual void receiveNextMethod(unsigned int num) throw(bad_alloc,R::RException);

	/**
	* Destructor.
	*/
	virtual ~GSlotLog(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
