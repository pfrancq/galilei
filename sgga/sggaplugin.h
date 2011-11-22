/*

	GALILEI Research Project

	SGGAPlugIn.h

	Generic Plug-in for SGGA - Header.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
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
#ifndef SGGAPluginH
#define SGGAPluginH


//------------------------------------------------------------------------------
// include files for R project
#include <robject.h>
#include <rgroupingkmeans.h>
#include <rparam.h>


//------------------------------------------------------------------------------
// include files for GALILEI project
#include <gcommunity.h>
#include <gtopic.h>


//------------------------------------------------------------------------------
// include files
#include <gca_sgga.h>
#include <sggachromo.h>


//-----------------------------------------------------------------------------
/**
 * The SGGAPlugIn class provides a representation for a generic plug-in
 * implementing a SGGA for a specific kind of objects (profiles, documents,
 * etc.).
 * @author Pascal Francq
 * @short Generic SGGA PlugIn
 */
template<class cPlugin,class cObj,class cGroup,class cInst,class cGAGroup>
	class SGGAPlugIn : public cPlugin, R::RObject, public RParamsSG
{
protected:

	/**
	 * Type of objects.
	 */
	tObjType ObjType;

	/**
	 * Type of groups.
	 */
	tObjType GroupType;

	/**
	 * Use the internal number generator.
	 */
	bool InternalRandom;

	/**
	 * Seed value to use.
	 */
	int Seed;

	/**
	 * Cout information.
	 */
	bool Cout;

public:

	/**
	* Constructor.
	*/
	SGGAPlugIn(GSession* session,GPlugInFactory* fac,const R::RString& name,tObjType objtype,tObjType grouptype);

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("SGGAPlugIn");}

protected:

	/**
	* Construct the results of the session based on a chromosome.
	*/
	template<class cAlgoGroup> void ConstructResults(GSession* session,R::RCursor<cAlgoGroup> Sol);

	/**
	 * Get a pointer to the objects to cluster.
	 */
	virtual R::RCursor<cObj> GetObjs(void) const=0;

	/**
	* Test if a group is coherent. Actually, this means verify that all the
	* objects of the group are similar together.
	* @param grp            Group to verify.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const cGroup* grp) const;

	/**
	* Test if a group is still coherent if an object is added to it.
	* Actually, this means verify that all the objects of the group are
	* similar to the one tested.
	* @param grp            Group to verify.
	* @param obj            Object eventually to add.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const cGroup* grp,const cObj* sub) const;

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all objects are to be inserted again.
	*/
	virtual bool IsValid(cGroup* grp);

	/**
	* Make the grouping.
	*/
	void RunGrouping(GSession* session,const R::RString& mes);

	/**
	 * Catch a best chromosome notification.
	 */
	//void BestChromo(const R::RNotification& notification);

	/**
	 * Handle a generation
	 */
	void Gen(const R::RNotification& notification);

public:

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);
};


//------------------------------------------------------------------------------
#endif
