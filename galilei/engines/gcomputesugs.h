/*

	GALILEI Research Project

	GComputeSugs.h

	Generic Suggestions Computing Method - Header.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef GComputeSugsH
#define GComputeSugsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GComputeSugs class provides a representation for a generic method to
* compute suggestions.
* @author Pascal Francq
* @short Generic Suggestions Computing Method.
*/
class GComputeSugs : public GPlugIn
{
public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory of the plug-in.
	*/
	GComputeSugs(GSession* session,GPlugInFactory* fac);

	/**
	* Run the suggestions computing method.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor.
	*/
	virtual ~GComputeSugs(void);
};


//------------------------------------------------------------------------------
#define CREATE_COMPUTESUGS_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GComputeSugs,plugin,"ComputeSugs",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
