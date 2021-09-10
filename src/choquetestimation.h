/*

	GALILEI Research Project

	ChoquetEstimation.h

	Estimation of the parameters of the Choquet Intregral - Header.

	Copyright 2013-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef ChoquetEstimationH
#define ChoquetEstimationH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <gtool.h>
#include <robject.h>
#include <rvector.h>
#include <rmatrix.h>
#include <rlowertriangularmatrix.h>
#include <rtestchoquet.h>
#include <choquetsim.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
/**
* This plug-in evaluates the values of the different Choquet's parameters used
* to compute the similarity. In practice, it uses the "ideal clustering" of the
* selected documents (based on the subjects) to find the best parameter values.
*
* These values are then used for all the similarity measure (documents, profiles,
* topics, etc.).
*
* @remark The plug-in supposes that the current similarity measure used for
*         the different objects is the one from the tensor space model based on
*         four criteria (text, metadata, semantic and links).
* @author Pascal Francq
* @short Similarity Choquet's Parameters Determination.
*/
class ChoquetEstimation : public RObject, public GTool, public RTestChoquet
{
public:

	/**
	 * Values of each criteria for the last similarity computed:
	 */
	RVector Sims;

	/**
	 * Matrix containing the average intra- (first line) and inter-cluster
	 * (second line) similarities.
	 */
	RMatrix AvgSim;

	/**
	 * Matrix containing the minimum intra- (first line) and inter-cluster
	 * (second line) similarity for each pair of criteria.
	 */
	RContainer< RLowerTriangularMatrix,true,false> MinSim;

	/**
	 * Matrix containing the maximum intra- (first line) and inter-cluster
	 * (second line) similarity for each pair of criteria.
	 */
	RContainer< RLowerTriangularMatrix,true,false> MaxSim;

public:

	/**
	* Construct tool.
	* @param session        Session.
	* @param fac            Factory.
	*/
	ChoquetEstimation(GSession* session,GPlugInFactory* fac);

	/**
    * @return the name of the class.
    */
	virtual RCString GetClassName(void) const {return("ChoquetEstimation");}

	/**
	 * Handle the similarity signals. It is send by the similarity (is asked)
	 * measure after a computation and gives the "local similarities" associated
	 * to the different criteria.
    * @param notification   Notification.
    */
	void HandleSim(const RNotification& notification);

	/**
	 * Compute the average intra- and inter-cluster similarities between
	 * documents. In practice, it catches all the document similarity signal
	 * emitted while asking to recompute the similarities for a set of documents.
	 */
	void ComputeSims(void);

	/**
	 * Verify that a given similarity is in [0,+1].
    * @param sim             Similarity to test.
    * @return the similarity tested.
    */
	double Sim(double sim) const;

	/**
	 * Compute a ratio to maximize between the intra- and inter-cluster
	 * similarities for a given solution.
    * @param prop            Solution to evaluate.
    * @return a ratio in [-1,+1].
    */
	virtual double Compute(RChoquetPropagator& prop);

	/**
	 * Use a solution to set the parameters for a given measure.
    * @param mes             Name of the measure.
	 * @param max             Maximum value of the integer parameters.
    */
	void SetParams(const RString& mes);

	/**
	 * Run the GeCode engine to find the parameters.
	 * @param slot            Slot that will receive the messages.
    */
	void RunGecode(void);

	/**
	* Run the tool.
	* @param slot            Slot that will receive the messages.
	*/
	virtual void Run(GSlot* slot);

	/**
	* Destructor.
	*/
	virtual ~ChoquetEstimation(void);
};


//-----------------------------------------------------------------------------
#endif
