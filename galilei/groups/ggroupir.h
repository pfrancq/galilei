/*

	GALILEI Research Project

	GGroupIR.h

	Group of an IR GA - Header.

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
#ifndef GGroupIRH
#define GGroupIRH



//-----------------------------------------------------------------------------
// include files for R Project
#include <rgga/rgroups.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gir.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroupDataIR provides a representation of the information needed to
* construct a group in the IR Problem.
* @author Pascal Francq
* @short Data for IR Group.
*/
class GGroupDataIR
{
public:
	/**
	* Constructor of the group data.
	* @param m              Maximum size of the groups.
	*/
	GGroupDataIR(const unsigned int m) {}
};


//-----------------------------------------------------------------------------
/**
* The RGroupBP provides a representation for a group for the IR Problem.
* @author Pascal Francq
* @short IR Group.
*/
class GGroupIR : public RGGA::RGroup<GGroupIR,GSubProfile,GGroupDataIR>
{
protected:

	/**
	* Average similarity between the profiles of the group.
	*/
	double AvgSim;

public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	GGroupIR(GGroupIR* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identificator of the group.
	* @param data           Data needed for the group.
	*/
	GGroupIR(RGGA::RGroups<GGroupIR,GSubProfile,GGroupDataIR>* owner,const unsigned int id,const GGroupDataIR* data);

	/**
	* Verify if the group is not violating the integrity of the system.
	* @return true if the group is correct, false else.
	*/
	virtual bool Verify(void);

	/**
	* Clear the information container in a group.
	*/
	virtual void Clear(void);

	/**
	* Look if an object can be insert in the group.
	* @param obj            Pointer to the object to insert.
	*/
	virtual bool CanInsert(const GSubProfile* obj);

	/**
	* Method call after an object was inserted in the group.
	* @param obj            Pointer to the object to insert.
	*/
	virtual void PostInsert(const GSubProfile* obj);

	/**
	* Method call after an object was deleted from the group.
	* @param obj            Pointer to the object to delete.
	*/
	virtual void PostDelete(const GSubProfile* obj);

	/**
	* Do a local optimisation by checking whether it is possible to replace up
	* to three objects in the group by one or two objects of objs, in such a
	* way that the total size of the group increases. If so, perform the
	* changes in the group and in objs.
	*/
	bool DoOptimisation(GSubProfile** objs,unsigned int& nbobjs);

	/**
	* Assignment operator.
	* @param grp            The group used as source.
	*/
	GGroupIR& operator=(const GGroupIR& grp);

	/**
	* Destruct the group.
	*/
	virtual ~GGroupIR(void);
};


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
