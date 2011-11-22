/*

	GALILEI Research Project

	NNGGAInst.h

	Instance - Header

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
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
#ifndef NNGGAInstH
#define NNGGAInstH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstnn.h>
#include <gdoc.h>
#include <rheuristicnn.h>


//-----------------------------------------------------------------------------
// include files for NNGGA
#include <gca_nngga.h>


//-----------------------------------------------------------------------------
/**
 * This is the normal heuristic, this class is only there for debug purposes.
 */
class NNGGAHeuristicDoc : public R::RHeuristicNN<NNGGAGroupDoc,GDoc,NNGGAChromoDoc>
{
public:

	/**
	* Construct the heuristic.
	* @param r               The random generator to use.
	* @param objs            Pointer to the objects.
	* @param params          Parameters.
	* @param debug           Debugger.
	*/
	NNGGAHeuristicDoc(R::RRandom& r,R::RCursor<GDoc> objs,R::RParamsNN* params,R::RDebug* debug=0)
		: RHeuristicNN<NNGGAGroupDoc,GDoc,NNGGAChromoDoc>(r,objs,params,debug) {}

};


//-----------------------------------------------------------------------------
/**
* The NNGGAThreadDataDoc class provides a representation for "thread-dependent"
* data for the documents clustering.
* @author Pascal Francq
* @short Documents "thread-dependent" Data.
*/
class NNGGAThreadDataDoc : public R::RThreadDataNN<NNGGAInstDoc,NNGGAChromoDoc,NNGGAThreadDataDoc,NNGGAGroupDoc,GDoc>
{
public:
	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	NNGGAThreadDataDoc(NNGGAInstDoc* owner)
		: R::RThreadDataNN<NNGGAInstDoc,NNGGAChromoDoc,NNGGAThreadDataDoc,NNGGAGroupDoc,GDoc>(owner) {}
};


//-----------------------------------------------------------------------------
/**
* The instance of the document clustering problem.
* @author Pascal Francq
* @short Documents Instance.
*/
class NNGGAInstDoc : public R::RInstNN<NNGGAInstDoc,NNGGAChromoDoc,NNGGAThreadDataDoc,NNGGAGroupDoc,GDoc>
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
	NNGGAInstDoc(GSession* ses,R::RCursor<GDoc> objs,RParamsNN* p,R::RDebug* debug,tObjType type);

	/**
	* Create a specific heuristic for the algorithm.
	*/
	virtual R::RGroupingHeuristic<NNGGAGroupDoc,GDoc,NNGGAChromoDoc>* CreateHeuristic(void);

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
	friend class NNGGAChromoDoc;
	friend class NNGGAGroupDoc;
	friend class NNGGAThreadDataDoc;
	friend class NNGGAHeuristicDoc;
};


//-----------------------------------------------------------------------------
/**
 * This is the normal heuristic, this class is only there for debug purposes.
 */
class NNGGAHeuristicProfile : public R::RHeuristicNN<NNGGAGroupProfile,GProfile,NNGGAChromoProfile>
{
public:

	/**
	* Construct the heuristic.
	* @param r               The random generator to use.
	* @param objs            Pointer to the objects.
	* @param params          Parameters.
	* @param debug           Debugger.
	*/
	NNGGAHeuristicProfile(R::RRandom& r,R::RCursor<GProfile> objs,R::RParamsNN* params,R::RDebug* debug=0)
		: RHeuristicNN<NNGGAGroupProfile,GProfile,NNGGAChromoProfile>(r,objs,params,debug) {}

};


//-----------------------------------------------------------------------------
/**
* The NNGGAThreadDataProfile class provides a representation for "thread-dependent"
* data for the profiles clustering.
* @author Pascal Francq
* @short Profiles "thread-dependent" Data.
*/
class NNGGAThreadDataProfile : public R::RThreadDataNN<NNGGAInstProfile,NNGGAChromoProfile,NNGGAThreadDataProfile,NNGGAGroupProfile,GProfile>
{
public:
	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	NNGGAThreadDataProfile(NNGGAInstProfile* owner)
		: R::RThreadDataNN<NNGGAInstProfile,NNGGAChromoProfile,NNGGAThreadDataProfile,NNGGAGroupProfile,GProfile>(owner) {}
};


//-----------------------------------------------------------------------------
/**
* The instance of the profile clustering problem.
* @author Pascal Francq
* @short Profiles Instance.
*/
class NNGGAInstProfile : public R::RInstNN<NNGGAInstProfile,NNGGAChromoProfile,NNGGAThreadDataProfile,NNGGAGroupProfile,GProfile>
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
	NNGGAInstProfile(GSession* ses,R::RCursor<GProfile> objs,RParamsNN* p,R::RDebug* debug,tObjType type);

	/**
	* Create a specific heuristic for the algorithm.
	*/
	virtual R::RGroupingHeuristic<NNGGAGroupProfile,GProfile,NNGGAChromoProfile>* CreateHeuristic(void);

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	//virtual double GetDisagreementRatio(const NNGGAObj* obj1,const NNGGAObj* obj2) const;

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	//virtual double GetAgreementRatio(const NNGGAObj* obj1,const NNGGAObj* obj2) const;

	/**
	* Compute the similarity between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	//virtual double GetSim(const NNGGAObj* obj1,const NNGGAObj* obj2) const;

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
	friend class NNGGAChromoProfile;
	friend class NNGGAGroupProfile;
	friend class NNGGAThreadDataProfile;
	friend class NNGGAHeuristicDoc;
};


//-----------------------------------------------------------------------------
#endif

