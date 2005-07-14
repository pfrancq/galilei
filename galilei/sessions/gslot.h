/*

	GALILEI Research Project

	GSlot.h

	Generic Slot for a GALILEI Session - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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
#ifndef GSlotH
#define GSlotH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprg/rprgoutput.h>


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
	* Constructor.
	*/
	GSlot(void) throw(std::bad_alloc,R::RException);

	/**
	* The traitment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void receiveNextDoc(const GDoc* doc) throw(std::bad_alloc,R::RException);

	/**
	* The traitment for a specific document will begin.
	* @param prof           Profile.
	*/
	virtual void receiveNextProfile(const GProfile* prof) throw(std::bad_alloc,R::RException);

	/**
	* The traitment for a specific profile will begin.
	* @param prof           Profile.
	*/
	virtual void receiveNextProfileExport(const GProfile* prof) throw(std::bad_alloc,R::RException);

	/**
	* The traitment for a specific document will begin.
	* @param doc           Document
	*/
	virtual void receiveNextDocumentExport(const GDoc* doc) throw(std::bad_alloc,R::RException);

	/**
	* The traitment for a specific group will begin.
	* @param grp           Group.
	*/
	virtual void receiveNextGroupExport(const GGroup* grp) throw(std::bad_alloc,R::RException);

	/**
	* Method called by GGrouping each time a new language is analysed.
	* @param lang           Pointer to the current lang.
	*/
	virtual void NextGroupLang(const GLang* lang) throw(std::bad_alloc,R::RException);

	/**
	* The traitment for a specific chromosome will begin.
	* @param id             Identificator of the chromosome.
	*/
	virtual void receiveNextChromosome(unsigned int id) throw(std::bad_alloc,R::RException);

	/**
	* The traitment for a specific method will begin.
	* @param num            Number of the current step.
	*/
	virtual void receiveNextMethod(unsigned int num) throw(std::bad_alloc,R::RException);

	/**
	* This is used by the caller to interact. Typically, it is used to give the
	* control of the process to the GUI.
	*/
	virtual void Interact(void);

	/**
	* Destructor.
	*/
	virtual ~GSlot(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
