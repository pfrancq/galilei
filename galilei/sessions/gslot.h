/*

	GALILEI Research Project

	GSlot.h

	Generic Slot for a GALILEI Session - Header.

	Copyright 2002-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GSlotH
#define GSlotH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprgoutput.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GSlot class provides a representation for a receiver of signals emit by a
* GALILEI session.
* @author Pascal Francq
* @short Generic Slot.
*/
class GSlot : public R::RPrgOutput
{
public:

	/**
	* Construct the slot.
	*/
	GSlot(void);

	/**
	* The treatment for a specific concept type will begin.
	* @param type            Concept type.
	*/
	virtual void NextConceptType(const GConceptType* type);

	/**
	* The treatment for a specific document will begin.
	* @param doc             Document.
	*/
	virtual void NextDoc(const GDoc* doc);

	/**
	* The treatment for a specific document will begin.
	* @param prof            Profile.
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
	* This is used by the caller to interact. Typically, it is used to give the
	* control of the process to the GUI.
	*/
	virtual void Interact(void);

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
	* Destruct the slot.
	*/
	virtual ~GSlot(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
