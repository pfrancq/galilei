	/*

	GALILEI Research Project

	ClusteringEval.h

	Clustering Evaluation - Header.

	Copyright 2007-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef ClusteringEvalH
#define ClusteringEvalH


//-----------------------------------------------------------------------------
// include files for R Project
#include <robject.h>
#include <rcontainer.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gsubjects.h>
#include <gsubject.h>
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
/**
 * The ClusterScore represent the precision and the recall of a given computed
 * cluster.
 */
template<class cGroup>
	class ClusterScore
{
public:
	cGroup* Group;              // Pointer to the group.
	double Precision;           // Precision inside the group.
	double Recall;              // Recall inside the group.

	// Constructor and Compare methods.
	ClusterScore(cGroup* grp) : Group(grp), Precision(0.0), Recall(0.0) {}
	int Compare(const ClusterScore& grp) const {return(Group->Compare(*grp.Group));}
};


//------------------------------------------------------------------------------
/**
 * The ClusterId class is used to compute the adjusted Rand Index. It makes the
 * link between a given cluster and a position of the vectors used for the
 * computation.
 */
class ClusterId
{
public:
	size_t GrpId;       // Group Identifier.
	size_t position;    // Position of the group.

	// Constructor and Compare methods.
	ClusterId(size_t RealId,size_t Position) : GrpId(RealId), position(Position) {}
	int Compare(const size_t ID) const {return(CompareIds(GrpId,ID));}
	int Compare(const ClusterId& grp) const {return(CompareIds(GrpId,grp.GrpId));}
};


//------------------------------------------------------------------------------
/**
* The ClusteringEval class provides a method to compute the precision, the
* recall and the adjusted Rand Index of a computed (profiles or documents)
* clustering in comparison with an ideal one.
* @author Pascal Francq.
* @short Clustering Evaluation.
*/
template<class cGroup,class cObj>
	class ClusteringEval : public R::RObject, public GMeasure
{
protected:

	/**
	 * Type of the group.
	 */
	tObjType GroupType;

	/**
	 * Type of the object.
	 */
	tObjType ObjType;

	/**
	 * Determine if the recall and precision must be recomputed.
	 */
	bool DirtyRecallPrecision;

	/**
	 * Determine if the adjusted Rand index must be recomputed.
	 */
	bool DirtyAdjustedRandIndex;

	/**
	 * Determine if the J measure must be recomputed.
	 */
	bool DirtyJ;

	/**
	 * The current recall.
	 */
	double Recall;

	/**
	 * The current precision.
	 */
	double Precision;

	/**
	 * The current adjusted Rand index.
	 */
	double AdjustedRandIndex;

	/**
	 * The current J.
	 */
	double J;

	/**
	 * Scores for all the computed clusters.
	 */
	RContainer<ClusterScore<cGroup>,true,true> ClustersScore;

public:

	/**
	 * Constructor of the measure.
	 * @param fac            Factory.
	 * @param grouptype      Type of the clusters.
	 * @param objtype        Type of the objects grouped.
	 */
	ClusteringEval(GPluginFactory* fac,tObjType grouptype,tObjType objtype);

	/**
	 * Virtual method inherits from R::RObject and that must be re-implemented
	 * in all child classes.
	 * @return Name of the class.
	 */
	virtual R::RCString GetClassName(void) const {return("ClusteringEval");}

	/**
	 * Makes the measures dirty.
	 */
	void Dirty(void);

	/**
	* Connect to the session.
	*
	* The storage of the matrix is opened (if necessary).
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Access to several information related to the evaluation of a given
	* clustering. If necessary, the measures are updated.
	* @param info        Type of the information to take. Three values are
	*                    accepted:
	*                    - case '0': The recall.
	*                    - case '1': The precision.
	*
	* The class supposes that two more parameter is passed:
	* - Identifier of the cluster (size_t).
	* - A pointer to a variable of type double that will contain the result.
	*/
	virtual void Measure(size_t measure,...);

	/**
	* Access to several information related to the clustering evaluation. If
	* necessary, the measures are updated.
	* @param info        Type of the information to take. Three values are
	*                    accepted:
	*                    - case '0': The recall.
	*                    - case '1': The precision.
	*                    - case '2': The adjusted Rand Index.
	*                    - case '3': The J measure.
	*
	* The class supposes that one more parameter is passed:
	* - A pointer to a variable of type double that will contain the result.
	*/
	virtual void Info(size_t measure,...);

private:

	/**
	 * Look if the object is alone in the ideal group.
	 * @param obj            Object.
	 * @return true if the object is alone.
	 */
	virtual bool IsObjAloneInIdealGroup(cObj* obj);

	/**
	 * Compute the "best" recall and precision for a given set of objects and a
	 * computed group.
	 * @param objs           Objects.
	 * @param grp            Score of the computed group.
	 * @param ingroup        Number of objects in the computed group.
	 */
	virtual void ComputeBestLocalRecallPrecision(RCursor<cObj>& objs,ClusterScore<cGroup>* grp,size_t ingroup);

	/**
	 * Compute the recall and the precision.
	 */
	void ComputeRecallPrecision(void);

	/**
	 * Compute the adjusted Rand index.
	 */
	void ComputeAdjustedRandIndex(void);

	/**
	 * Compute the J measure.
	 */
	void ComputeJ(void);

	/**
	 * Pure function that gives the clusters computed.
	 * @return RCursor over the computed clusters.
	 */
	virtual RCursor<cGroup> GetClusters(void)=0;
};


//-----------------------------------------------------------------------------
#include <clusteringeval.hh>


//------------------------------------------------------------------------------
#endif
