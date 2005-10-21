                                                                                 /*

	GALILEI Research Project

	GSubProfilesLevel.h

	Compute Profiles and Documents Levels - Header.

	Copyright 2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@e-parkos.com)


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
#ifndef GSubProfilesLevelH
#define GSubProfilesLevelH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gpostgroup.h>
#include <gstorage.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


class GSubProfilesLevelCmd : public GStorageCmd
{
public:

	/**
	* Constructor of a command.
	* @param cmd             Name of the command.
	*/
	GSubProfilesLevelCmd();

	/**
	* Run the command.
	* @param storage         Pointer to the command.
	* @param inst            XML tag representing the instruction.
	* @param caller          Caller of the function.
	*/
	virtual void Run(GStorage* storage,const GStorageTag& inst,void* caller);

	/**
	* Constructor of a command.
	* @param cmd             Name of the command.
	*/
	~GSubProfilesLevelCmd();
};


class GDocsLevelCmd : public GStorageCmd
{
	public:

	/**
	 * Constructor of a command.
	 * @param cmd             Name of the command.
	 */
		GDocsLevelCmd();

	/**
		 * Run the command.
		 * @param storage         Pointer to the command.
		 * @param inst            XML tag representing the instruction.
		 * @param caller          Caller of the function.
	 */
		virtual void Run(GStorage* storage,const GStorageTag& inst,void* caller);

	/**
		 * Constructor of a command.
		 * @param cmd             Name of the command.
	 */
		~GDocsLevelCmd();
};


//------------------------------------------------------------------------------
/**
* The GSubProfilesLevel provides a method to level of each subprofiles
* @author David wartel
* @short SubProfiles level computation.
*/
class GSubProfilesLevel  : public GPostGroup
{
	/**
	* Array of documents.
	*/
	R::RContainer<GFdbk,false,false> Docs;

	/*
	* number of levels for subprofiles
	*/
	unsigned int NbLevels;

public:

	/**
	* Constructor.
	* @param fac             Factory.
	*/
	GSubProfilesLevel(GFactoryPostGroup* fac) throw(std::bad_alloc, GException);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session) throw(GException);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session) throw(GException);

    /**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) throw(GException);

	/**
	* Destructor.
	*/
	virtual ~GSubProfilesLevel(void);
};


}  //-------- End of namespace GALILEI----------------------------------------


//-----------------------------------------------------------------------------
#endif

