/*

	GALILEI Research Project

	GSlot.h

	Generic Slot for a GALILEI Session - Header.

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
	* Constructor.
	*/
	GSlot(void);

	/**
	* The traitment for a specific document will begin.
	* @param doc             Document.
	*/
	virtual void NextDoc(const GDoc* doc);

	/**
	* The traitment for a specific document will begin.
	* @param prof            Profile.
	*/
	virtual void NextProfile(const GProfile* prof);

	/**
	* The traitment for a specific profile will begin.
	* @param prof            Profile.
	*/
	virtual void NextProfileExport(const GProfile* prof);

	/**
	* The traitment for a specific document will begin.
	* @param doc             Document
	*/
	virtual void NextDocumentExport(const GDoc* doc);

	/**
	* The traitment for a specific group will begin.
	* @param grp             Group.
	*/
	virtual void NextGroupExport(const GCommunity* grp);

	/**
	* Method called by GCommunitying each time a new language is analysed.
	* @param lang            Pointer to the current lang.
	*/
	virtual void NextGroupLang(const GLang* lang);

	/**
	* Method called when executing a sequence of instruction to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const R::RString& str);
	
	/**
	* The traitment for a specific chromosome will begin.
	* @param id              Identificator of the chromosome.
	*/
	virtual void NextChromosome(size_t id);

	/**
	* The traitment for a specific method will begin.
	* @param num             Number of the current step.
	*/
	virtual void NextMethod(size_t num);

	/**
	* Start a job.
	* @param job             Description of ther job.
	*/
	virtual void StartJob(const R::RString job);

	/**
	* End of the last started job.
	*/
	virtual void EndJob(void);

	/**
	* This is used by the caller to interact. Typically, it is used to give the
	* control of the process to the GUI.
	*/
	virtual void Interact(void);

	/**
	* Forward a warning.
	* @param msg             Message.
	*/
	virtual void Warning(R::RString msg);

	/**
	* Forward an error.
	* @param msg             Message.
	*/
	virtual void Error(R::RString msg);

	/**
	* Forward an alert.
	* @param msg             Message.
	*/
	virtual void Alert(R::RString msg);

	/**
	* Destructor.
	*/
	virtual ~GSlot(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
