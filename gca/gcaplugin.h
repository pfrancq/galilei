/*

	GALILEI Research Project

	GCAPlugIn.h

	Generic Plug-in for GCA - Header.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef GCAPluginH
#define GCAPluginH


//------------------------------------------------------------------------------
// include files for R project
#include <robject.h>
#include <rgroupingkmeans.h>


//------------------------------------------------------------------------------
// include files for GALILEI project
#include <gcommunity.h>
#include <gtopic.h>


//------------------------------------------------------------------------------
// include files
#include <gca.h>
#include <gcachromo.h>


//-----------------------------------------------------------------------------
class kMeans : public R::RGroupingKMeans<CGroup,GCAObj,CGroups>
{
	GMeasure* Measure;
public:
	kMeans(const R::RString& n,R::RRandom* r,R::RCursor<GCAObj> objs,const R::RString& mes,R::RDebug* debug=0);

	double Similarity(const GCAObj* obj1,const GCAObj* obj2);
};


//-----------------------------------------------------------------------------
/**
 * The GCAPlugIn class provides a representation for a generic plug-in
 * implementing a GCA for a specific kind of objects (profiles, documents,
 * etc.).
 * @author Pascal Francq
 * @short Generic GCA PlugIn
 */
template<class cObj,class cGroup,class cFactory>
	class GCAPlugIn : public R::RObject, public RParamsSC
{
protected:

	/**
	* GA Objects.
	*/
	R::RContainer<GCAObj,true,false> Objs;

	/**
	 * Current clustering method
	 */
	char ClusteringMethod;

	/**
	 * Number of clusters handled by the k-Means.
	 */
	size_t NbClusters;

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

public:

	/**
	* Constructor.
	*/
	GCAPlugIn(const R::RString& name,tObjType objtype,tObjType grouptype);

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("GCAPlugIn");}

	/**
	* Configurations were applied from the factory.
	*/
	void ApplyConfig(cFactory* factory);

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
	 * Do the GCA.
	 */
	void DoGCA(GSession* session,const R::RString& mes);

	/**
	 * Do the k-Means.
	 */
	void DokMeans(GSession* session,const R::RString& mes,R::RCursor<cGroup> groups);

	/**
	* Make the grouping.
	*/
	void RunGrouping(GSession* session,const R::RString& mes,R::RCursor<cGroup> groups);

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
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destruct the GCA.
	*/
	virtual ~GCAPlugIn(void);
};


//-----------------------------------------------------------------------------
#include <gcaplugin.hh>

//------------------------------------------------------------------------------
#endif
