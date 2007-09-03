/*

	Genetic Community Algorithm

	GCAGrouping.h

	GCA Plug-in - Header.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GGCAGroupingH
#define GGCAGroupingH

//-----------------------------------------------------------------------------
// include files for R project
#include <robject.h>
#include <ggca.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggrouping.h>


//-----------------------------------------------------------------------------
/**
* The GGCAGrouping provides a representation for a method to group some
* subprofiles using a grouping genetic algorithm.
* @author Pascal Francq
* @short GGA Method.
*/
class GGCAGrouping : public GGrouping, public RObject
{
protected:

	/**
	* GA Objects.
	*/
	RContainer<GGCAObj,true,false> Objs;

	/**
	* Instance of the GA.
	*/
	GGCAInst* Instance;

	/**
	* Parameters of the GA used.
	*/
	GGCAParams Params;

public:

	/**
	* Constructor.
	* @param f              Factory.
	*/
	GGCAGrouping(GFactoryGrouping* fac);

	virtual RCString GetClassName(void) const {return("GGCAGrouping");}
	
	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Initialisation of the method.
	*/
	virtual void Init(void);

	/**
	* Construct the groups of the session based on a chromosome.
	*/
	void ConstructGroupsFromChromo(GGCAChromo* chromo);

protected:

	/**
	* Test if a group is coherent. Actually, this means verify that all the
	* subprofiles of the group are similar together.
	* @param grp            Group to verify.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GGroup* grp) const;

	/**
	* Test if a group is still coherent if a subprofile is added to it.
	* Actually, this means verify that all the subprofiles of the group are
	* similar to the one tested.
	* @param grp            Group to verify.
	* @param sub            SubProfile eventually to add.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GGroup* grp,const GSubProfile* sub) const;

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all profiles are to be inserted again.
	*/
	virtual bool IsValid(GGroup* grp);

	/**
	 * Catch a best chromosome notification.
	 */
	void BestChromo(const RNotification& notification);
	   
	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GGCAGrouping(void);
};


//-----------------------------------------------------------------------------
#endif
