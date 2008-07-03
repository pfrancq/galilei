/*

	Genetic Documents Algorithm

	GGDAGrouping.h

	GDA Plug-in - Header.

	Copyright 2006-2008 by the Université Libre de Bruxelles.

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
#ifndef GGDAGroupingH
#define GGDAGroupingH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gpostdoc.h>

//-----------------------------------------------------------------------------
// include files for GDA
#include <ggca.h>


//-----------------------------------------------------------------------------
/**
* The GGDAGrouping provides a representation for a method to group some
* subprofiles using a grouping genetic algorithm.
* @author Pascal Francq
* @short GGA Method.
*/
class GGDAGrouping : public GPostDoc
{
protected:

	/**
	* GA Objects.
	*/
	R::RContainer<GGCAObj,true,false> Objs;

	/**
	* Parameters of the GA used.
	*/
	GGCAParams Params;

	/**
	 * Current clustering method
	 */
	char ClusteringMethod;

	/**
	 * Number of clusters handled by the k-Means.
	 */
	size_t NbClusters;

public:

	/**
	* Constructor.
	* @param f              Factory.
	*/
	GGDAGrouping(GFactoryPostDoc* fac);

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
	* Construct the results of the session based on a chromosome.
	*/
	void ConstructResults(R::RCursor<GGCAGroup> Sol);

protected:

	/**
	* Test if a group is coherent. Actually, this means verify that all the
	* subprofiles of the group are similar together.
	* @param grp            Group to verify.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GCommunity* grp) const;

	/**
	* Test if a group is still coherent if a subprofile is added to it.
	* Actually, this means verify that all the subprofiles of the group are
	* similar to the one tested.
	* @param grp            Group to verify.
	* @param sub            SubProfile eventually to add.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GCommunity* grp,const GProfile* sub) const;

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all profiles are to be inserted again.
	*/
	virtual bool IsValid(GCommunity* grp);

	/**
	 * Do the GDA.
	 */
	void DoGDA(void);

	/**
	 * Do the GDA.
	 */
	void DokMeans(void);

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GGDAGrouping(void);
};


//-----------------------------------------------------------------------------
#endif
