/*

	GALILEI Research Project

	PluginkMeans.h

	Generic Plug-in for Kernel kMeans - Header.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef PluginkMeansH
#define PluginkMeansH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <memory>


//------------------------------------------------------------------------------
// include files for R project
#include <robject.h>
#include <rcursor.h>
#include <rdebug.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gplugin.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
 * The PluginkMeans class a generic plug-in of a spherical k-means to cluster a
 * specific kind of objects (profiles, documents, etc.).
 * @author Pascal Francq
 * @short Generic kMeans PlugIn
 */
template<class cPlugin,class cObj,class cGroup,class cGrp,class cGrps,class kMeans,class KernelkMeans>
	class PluginkMeans : public cPlugin, public R::RObject
{
	using cPlugin::InsertParam;
	using cPlugin::Session;

protected:

	/**
	 * Number of clusters handled.
	 */
	size_t NbClusters;

	/**
	 * Maximum number of k-means iterations.
	 */
	size_t MaxkMeans;

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
	 * Convergence.
	 */
	double Convergence;

	/**
	 * Start an incremental clustering.
	 */
	bool Incremental;

	/**
	 * Alpha parameter.
	 */
	double Alpha;

	/**
	 * Run the spherical kernel k-Means or just the kernel k-Means.
	 */
	bool Spherical;

	/**
	 * Cout information.
	 */
	bool Cout;

	/**
	 * Type of the similarity measure ('Nearest Neighbors' or 'Complete').
	 */
	RString MeasureType;

	/**
	 * Type of the kMeans ('Normal' or 'Kernel').
	 */
	RString kMeansType;

public:

	/**
	* Constructor.
	*/
	PluginkMeans(GSession* session,GPlugInFactory* fac,const R::RString& name,tObjType objtype,tObjType grouptype);

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("SkMeans");}

protected:

	/**
	* Construct the results of the session based on a chromosome.
	*/
	void ConstructResults(GSession* session,R::RCursor<cGrp> Sol);

	/**
	 * Get a pointer to the objects to cluster.
	 */
	virtual R::RCursor<cObj> GetObjs(void) const=0;

	/**
	* Make the grouping.
	*/
	void RunGrouping(R::RCursor<cGroup> groups,R::RCursor<cObj> Objs,size_t maxid);

public:

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);
};


//-----------------------------------------------------------------------------
#include <pluginkmeans.hh>


//------------------------------------------------------------------------------
#endif
