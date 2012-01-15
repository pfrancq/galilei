/*

	GALILEI Research Project

	FeaturesEval.cpp

	Features Evaluation - Header.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef FeaturesEvalH
#define FeaturesEvalH


//-----------------------------------------------------------------------------
// include files for R Project
#include <robject.h>
#include <rcontainer.h>
#include <rvector.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gdoc.h>
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
class FeaturesEval : public RObject, public GMeasure
{
	/**
	 * Measures dirty ?
	 */
	bool Dirty;

	/**
	 * Mutual information.
	 */
	RVector MutualInfos;

	/**
	 * Current weights.
	 */
	size_t CurWeights;

public:

	/**
	 * Constructor of the measure.
	 * @param session        Session.
	 * @param fac            Factory.
	 */
	FeaturesEval(GSession* session,GPlugInFactory* fac);

	/**
	 * Virtual method inherits from R::RObject and that must be re-implemented
	 * in all child classes.
	 * @return Name of the class.
	 */
	virtual R::RCString GetClassName(void) const {return("FeaturesEval");}

	/**
	* Access to several information related to the evaluation of a given
	* feature. If necessary, the measures are updated.
	* @param measure         Type of the information to take :
	*                        - case '0': Idf.
	*                        - case '1': Mutual information.
	*
	* The class supposes that two more parameter is passed:
	* - Identifier of the feature (size_t).
	* - A pointer to a variable of type double that will contain the result.
	*/
	virtual void Measure(size_t measure,...);

	/**
	 * Get a given information.
	 * @param info           Information asked :
	 * 						 - cNoRef : Number of features weights.
	 *                       - Other : Name of the features weights.
	 */
	virtual void Info(size_t info,...);

	/**
	 * Build the mutual information.
	 */
	void BuildMutualInformation(void);

	/**
	 * This method handles the modification of the status of a given
	 * object.
	 * @param notification   Notification received.
	 */
	void Handle(const R::RNotification& notification);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);
};


//------------------------------------------------------------------------------
#endif
