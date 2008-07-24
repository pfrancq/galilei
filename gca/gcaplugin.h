/*

	? Project

	gcaplugin.h

	Main - Header.

	Copyright 2008 by the Université libre de Bruxelles.

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
#ifndef GCAPluginH
#define GCAPluginH

//------------------------------------------------------------------------------
#include <robject.h>


#include <gcommunity.h>
#include <gtopic.h>

//------------------------------------------------------------------------------
// include files
#include <gca.h>
#include <gcachromo.h>
#include <gcaobj.h>


//-----------------------------------------------------------------------------
template<class cObj,class cGroup,class cFactory>
	class GCA : public R::RObject
{
protected:

	/**
	* GA Objects.
	*/
	R::RContainer<GCAObj,true,false> Objs;

	/**
	* Parameters of the GA used.
	*/
	GCAParams Params;

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
	tObjType Type;

public:

	/**
	* Constructor.
	*/
	GCA(const R::RString& name,tObjType type);

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("GCA");}

	/**
	* Configurations were applied from the factory.
	*/
	void ApplyConfig(cFactory* factory);

protected:

	/**
	* Construct the results of the session based on a chromosome.
	*/
	void ConstructResults(GSession* session,R::RCursor<GCAGroup> Sol);

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
	void DoGCA(GSession* session);

	/**
	 * Do the k-Means.
	 */
	void DokMeans(GSession* session);

	/**
	* Make the grouping.
	*/
	virtual void Run(GSession* session);

	/**
	 * Catch a best chromosome notification.
	 */
	void BestChromo(const R::RNotification& notification);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GCA(void);
};

//-----------------------------------------------------------------------------
/*class kMeansDoc;
class kMeansGroups;
class kMeansGroup : public RGroup<kMeansGroup,GCAObj,kMeansGroups>
{
public:
	kMeansGroup(kMeansGroups* owner,size_t id);
	friend class KMeansDoc;
};


//-----------------------------------------------------------------------------
class kMeansGroups : public RGroups<kMeansGroup,GCAObj,kMeansGroups>
{
public:
	kMeansGroups(RCursor<GCAObj> objs,size_t max)
		: RGroups<kMeansGroup,GCAObj,kMeansGroups>(objs,max)
		{}
	friend class KMeansDoc;
};

kMeansGroup::kMeansGroup(kMeansGroups* owner,size_t id)
	: RGroup<kMeansGroup,GCAObj,kMeansGroups>(owner,id)
{
}


//-----------------------------------------------------------------------------
class kMeansDoc : public RGroupingKMeans<kMeansGroup,GCAObj,kMeansGroups>
{
	GMeasure* Measure;

public:
	kMeansDoc(const RString& n,RRandom* r,RCursor<GCAObj> objs,RDebug* debug=0)
		: RGroupingKMeans<kMeansGroup,GCAObj,kMeansGroups>(n,r,objs,debug)
	{
		Measure=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents Similarities");
	}

	virtual double Similarity(GCAObj* obj1,GCAObj* obj2)
	{
		double d;
		Measure->Measure(0,obj1->GetElementId(),obj2->GetElementId(),&d);
		return(d);
	}
};
*/

//-----------------------------------------------------------------------------
#include <gcaplugin.hh>

//------------------------------------------------------------------------------
#endif
