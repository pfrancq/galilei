/*

	GALILEI Research Project

	SGGAInst.h

	Instance - Header

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).
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



//-----------------------------------------------------------------------------
#ifndef SGGAInstH
#define SGGAInstH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstsg.h>
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <gdoc.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for SGGA
#include <gca_sgga.h>



//-----------------------------------------------------------------------------
/**
 * This is the normal heuristic, this class is only there for debug purposes.
 */
class SGGAHeuristicDoc : public RHeuristicSG<SGGAGroupDoc,GDoc,SGGAChromoDoc>
{
public:

	/**
	* Construct the heuristic.
	* @param r               The random generator to use.
	* @param objs            Pointer to the objects.
	* @param params          Parameters.
	* @param debug           Debugger.
	*/
	SGGAHeuristicDoc(RRandom& r,RCursor<GDoc> objs,RParamsSG* params,RDebug* debug=0)
		: RHeuristicSG<SGGAGroupDoc,GDoc,SGGAChromoDoc>(r,objs,params,debug) {}

};


//-----------------------------------------------------------------------------
/**
* The SGGAThreadDataDoc class provides a representation for "thread-dependent"
* data for the documents clustering.
* @author Pascal Francq
* @short Documents "thread-dependent" Data.
*/
class SGGAThreadDataDoc : public R::RThreadDataSG<SGGAInstDoc,SGGAChromoDoc,SGGAThreadDataDoc,SGGAGroupDoc,GDoc>
{
public:
	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	SGGAThreadDataDoc(SGGAInstDoc* owner)
		: R::RThreadDataSG<SGGAInstDoc,SGGAChromoDoc,SGGAThreadDataDoc,SGGAGroupDoc,GDoc>(owner) {}
};


//-----------------------------------------------------------------------------
/**
* The instance of the document clustering problem.
* @author Pascal Francq
* @short Documents Instance.
*/
class SGGAInstDoc : public R::RInstSG<SGGAInstDoc,SGGAChromoDoc,SGGAThreadDataDoc,SGGAGroupDoc,GDoc>
{
	/**
	* Session.
	*/
	GSession* Session;

	/**
	 * Similarity measure to used.
	 */
	GMeasure* Sims;

	/**
	 * Agreement measure to used.
	 */
	GMeasure* Agree;

	/**
	 * Disagreement measure to used.
	 */
	GMeasure* Disagree;

	/**
	 * Type of the elements to group.
	 */
	tObjType Type;

public:

	/**
	* Construct the instance.
	* @param ses            Session.
	* @param objs           The objects to group.
	* @param p              Parameters.
	* @param debug          Debugger.
	* @param type           Type of the element to group.
	*/
	SGGAInstDoc(GSession* ses,R::RCursor<GDoc> objs,RParamsSG* p,R::RDebug* debug,tObjType type);

	/**
	* Create a specific heuristic for the algorithm.
	*/
	virtual R::RGroupingHeuristic<SGGAGroupDoc,GDoc,SGGAChromoDoc>* CreateHeuristic(void);

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetDisagreementRatio(const GDoc* obj1,const GDoc* obj2) const;

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetAgreementRatio(const GDoc* obj1,const GDoc* obj2) const;

	/**
	* Compute the similarity between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetSim(const GDoc* obj1,const GDoc* obj2) const;


	// friend classes
	friend class SGGAChromoDoc;
	friend class SGGAGroupDoc;
	friend class SGGAThreadDataDoc;
	friend class SGGAHeuristicDoc;
};


//-----------------------------------------------------------------------------
/**
 * This is the normal heuristic, this class is only there for debug purposes.
 */
class SGGAHeuristicProfile : public R::RHeuristicSG<SGGAGroupProfile,GProfile,SGGAChromoProfile>
{
public:

	/**
	* Construct the heuristic.
	* @param r               The random generator to use.
	* @param objs            Pointer to the objects.
	* @param params          Parameters.
	* @param debug           Debugger.
	*/
	SGGAHeuristicProfile(R::RRandom& r,R::RCursor<GProfile> objs,R::RParamsSG* params,R::RDebug* debug=0)
		: RHeuristicSG<SGGAGroupProfile,GProfile,SGGAChromoProfile>(r,objs,params,debug) {}

};


//-----------------------------------------------------------------------------
/**
* The SGGAThreadDataProfile class provides a representation for "thread-dependent"
* data for the profiles clustering.
* @author Pascal Francq
* @short Profiles "thread-dependent" Data.
*/
class SGGAThreadDataProfile : public R::RThreadDataSG<SGGAInstProfile,SGGAChromoProfile,SGGAThreadDataProfile,SGGAGroupProfile,GProfile>
{
public:
	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	SGGAThreadDataProfile(SGGAInstProfile* owner)
		: R::RThreadDataSG<SGGAInstProfile,SGGAChromoProfile,SGGAThreadDataProfile,SGGAGroupProfile,GProfile>(owner) {}
};


//-----------------------------------------------------------------------------
/**
* The instance of the profile clustering problem.
* @author Pascal Francq
* @short Profiles Instance.
*/
class SGGAInstProfile : public R::RInstSG<SGGAInstProfile,SGGAChromoProfile,SGGAThreadDataProfile,SGGAGroupProfile,GProfile>
{
	/**
	* Session.
	*/
	GSession* Session;

	/**
	 * Similarity measure to used.
	 */
	GMeasure* Sims;

	/**
	 * Agreement measure to used.
	 */
	GMeasure* Agree;

	/**
	 * Disagreement measure to used.
	 */
	GMeasure* Disagree;

	/**
	 * Type of the elements to group.
	 */
	tObjType Type;

public:

	/**
	* Construct the instance.
	* @param ses            Session.
	* @param objs           The objects to group.
	* @param p              Parameters.
	* @param debug          Debugger.
	* @param type           Type of the element to group.
	*/
	SGGAInstProfile(GSession* ses,R::RCursor<GProfile> objs,RParamsSG* p,R::RDebug* debug,tObjType type);

	/**
	* Create a specific heuristic for the algorithm.
	*/
	virtual R::RGroupingHeuristic<SGGAGroupProfile,GProfile,SGGAChromoProfile>* CreateHeuristic(void);

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetDisagreementRatio(const GProfile* obj1,const GProfile* obj2) const;

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetAgreementRatio(const GProfile* obj1,const GProfile* obj2) const;

	/**
	* Compute the similarity between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetSim(const GProfile* obj1,const GProfile* obj2) const;

	// friend classes
	friend class SGGAChromoProfile;
	friend class SGGAGroupProfile;
	friend class SGGAThreadDataProfile;
	friend class SGGAHeuristicDoc;
};


//-----------------------------------------------------------------------------
#endif

