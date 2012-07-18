/*

	GALILEI Research Project

	GSlotLogLog.h

	Log File Slot - Header.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rtextfile.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gslot.h>


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
	* Construct the log file.
	* @param uri             URI of the file.
	*/
	GSlotLog(const R::RURI& uri);

	/**
	* The treatment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void NextDoc(const GDoc* doc);

	/**
	* The treatment for a specific document will begin.
	* @param prof           Profile.
	*/
	virtual void NextProfile(const GProfile* prof);

	/**
	* Method called when executing a sequence of instruction to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const R::RString& str);

	/**
	* Start a job.
	* @param job             Description of the job.
	*/
	virtual void StartJob(const R::RString& job);

	/**
	* End of the last started job.
	* @param msg             Eventually, a message to print.
	*/
	virtual void EndJob(const R::RString& msg=R::RString::Null);

	/**
	* Forward a warning.
	* @param msg             Message.
	*/
	virtual void Warning(const R::RString& msg);

	/**
	* Forward an error.
	* @param msg             Message.
	*/
	virtual void Error(const R::RString& msg);

	/**
	* Forward an alert.
	* @param msg             Message.
	*/
	virtual void Alert(const R::RString& msg);

	/**
	* Destruct the log file.
	*/
	virtual ~GSlotLog(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
