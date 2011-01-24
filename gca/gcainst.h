/*

	GALILEI Research Project

	GCAInst.h

	Instance - Header

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



//-----------------------------------------------------------------------------
#ifndef GCAInstH
#define GCAInstH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstnn.h>
#include <gdoc.h>
#include <rheuristicnn.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>


//-----------------------------------------------------------------------------
/**
 * This is the normal heuristic, this class is only there for debug purposes.
 */
class GCAHeuristicDoc : public R::RHeuristicNN<GCAGroupDoc,GDoc,GCAChromoDoc>
{
public:

	/**
	* Construct the heuristic.
	* @param r               The random generator to use.
	* @param objs            Pointer to the objects.
	* @param params          Parameters.
	* @param debug           Debugger.
	*/
	GCAHeuristicDoc(R::RRandom& r,R::RCursor<GDoc> objs,R::RParamsNN* params,R::RDebug* debug=0)
		: RHeuristicNN<GCAGroupDoc,GDoc,GCAChromoDoc>(r,objs,params,debug) {}

};


//-----------------------------------------------------------------------------
/**
* The GCAThreadDataDoc class provides a representation for "thread-dependent"
* data for the documents clustering.
* @author Pascal Francq
* @short Documents "thread-dependent" Data.
*/
class GCAThreadDataDoc : public R::RThreadDataNN<GCAInstDoc,GCAChromoDoc,GCAThreadDataDoc,GCAGroupDoc,GDoc>
{
public:
	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	GCAThreadDataDoc(GCAInstDoc* owner)
		: R::RThreadDataNN<GCAInstDoc,GCAChromoDoc,GCAThreadDataDoc,GCAGroupDoc,GDoc>(owner) {}
};


//-----------------------------------------------------------------------------
/**
* The instance of the document clustering problem.
* @author Pascal Francq
* @short Documents Instance.
*/
class GCAInstDoc : public R::RInstNN<GCAInstDoc,GCAChromoDoc,GCAThreadDataDoc,GCAGroupDoc,GDoc>
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
	GCAInstDoc(GSession* ses,R::RCursor<GDoc> objs,RParamsNN* p,R::RDebug* debug,tObjType type);

	/**
	* Create a specific heuristic for the algorithm.
	*/
	virtual R::RGroupingHeuristic<GCAGroupDoc,GDoc,GCAChromoDoc>* CreateHeuristic(void);

	/**
	* Get the maximum disagreement ratios of an element.
	* @param obj             Element.
	*/
	virtual const R::RMaxVector* GetDisagreementRatios(const GDoc* obj) const;

	/**
	* Get the disagreement ratio between two elements.
	* @param obj             Element.
	*/
	virtual const R::RMaxVector* GetAgreementRatios(const GDoc* obj) const;

	/**
	* Compute the similarity between two elements.
	* @param obj             Element.
	*/
	virtual const R::RMaxVector* GetSims(const GDoc* obj) const;

	// friend classes
	friend class GCAChromoDoc;
	friend class GCAGroupDoc;
	friend class GCAThreadDataDoc;
	friend class GCAHeuristicDoc;
};


//-----------------------------------------------------------------------------
/**
 * This is the normal heuristic, this class is only there for debug purposes.
 */
class GCAHeuristicProfile : public R::RHeuristicNN<GCAGroupProfile,GProfile,GCAChromoProfile>
{
public:

	/**
	* Construct the heuristic.
	* @param r               The random generator to use.
	* @param objs            Pointer to the objects.
	* @param params          Parameters.
	* @param debug           Debugger.
	*/
	GCAHeuristicProfile(R::RRandom& r,R::RCursor<GProfile> objs,R::RParamsNN* params,R::RDebug* debug=0)
		: RHeuristicNN<GCAGroupProfile,GProfile,GCAChromoProfile>(r,objs,params,debug) {}

};


//-----------------------------------------------------------------------------
/**
* The GCAThreadDataProfile class provides a representation for "thread-dependent"
* data for the profiles clustering.
* @author Pascal Francq
* @short Profiles "thread-dependent" Data.
*/
class GCAThreadDataProfile : public R::RThreadDataNN<GCAInstProfile,GCAChromoProfile,GCAThreadDataProfile,GCAGroupProfile,GProfile>
{
public:
	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	GCAThreadDataProfile(GCAInstProfile* owner)
		: R::RThreadDataNN<GCAInstProfile,GCAChromoProfile,GCAThreadDataProfile,GCAGroupProfile,GProfile>(owner) {}
};


//-----------------------------------------------------------------------------
/**
* The instance of the profile clustering problem.
* @author Pascal Francq
* @short Profiles Instance.
*/
class GCAInstProfile : public R::RInstNN<GCAInstProfile,GCAChromoProfile,GCAThreadDataProfile,GCAGroupProfile,GProfile>
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
	GCAInstProfile(GSession* ses,R::RCursor<GProfile> objs,RParamsNN* p,R::RDebug* debug,tObjType type);

	/**
	* Create a specific heuristic for the algorithm.
	*/
	virtual R::RGroupingHeuristic<GCAGroupProfile,GProfile,GCAChromoProfile>* CreateHeuristic(void);

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	//virtual double GetDisagreementRatio(const GCAObj* obj1,const GCAObj* obj2) const;

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	//virtual double GetAgreementRatio(const GCAObj* obj1,const GCAObj* obj2) const;

	/**
	* Compute the similarity between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	//virtual double GetSim(const GCAObj* obj1,const GCAObj* obj2) const;

	/**
	* Get the maximum disagreement ratios of an element.
	* @param obj             Element.
	*/
	virtual const R::RMaxVector* GetDisagreementRatios(const GProfile* obj) const;

	/**
	* Get the disagreement ratio between two elements.
	* @param obj             Element.
	*/
	virtual const R::RMaxVector* GetAgreementRatios(const GProfile* obj) const;

	/**
	* Compute the similarity between two elements.
	* @param obj             Element.
	*/
	virtual const R::RMaxVector* GetSims(const GProfile* obj) const;

	// friend classes
	friend class GCAChromoProfile;
	friend class GCAGroupProfile;
	friend class GCAThreadDataProfile;
	friend class GCAHeuristicDoc;
};


//-----------------------------------------------------------------------------
#endif

