                                                                  /*

	GALILEI Research Project

	GGroupingKCos.h

	Heuristic using Similarity - Header.

	(C) 2001 by David Wartel.

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GGroupingKCosH
#define GGroupingKCosH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcursor.h>
//tmp
#include <rga/robjs.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include<galilei.h>
#include<groups/ggrouping.h>
#include<groups/ggroupingparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GGroup;


//-----------------------------------------------------------------------------
/**
* The GKMeansParam represents all the parameter used in the KMeans module.
* @short GKMeans Parameters.
*/
class GKMeansParams : public GGroupingParams
{
public:

	/**
	* enum of initial conditions
	*/
	enum Initial {Refined,Random};

	/**
	* initial method.
	*/
	Initial Init;

	/**
	* Number of Groups.
	*/
	unsigned int NbGroups;

	/**
	* Maximal number of iterations.
	*/
	unsigned int NbIters;

	/**
	* Number of generation for each step.
	*/
	unsigned int NbSubSamples;

	/**
	* Minimum similarity level between the profiles of a group.
	*/
	unsigned int SubSamplesRate;

	/**
	* Minimum number of common OK documents needed to force two profiles to be
	* in the same group.
	*/
	double Epsilon;

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*);

	/**
	* Assignment operator.
	* @param p              Parameters used as source.
	*/
	GKMeansParams& operator=(const GKMeansParams& src);

	/**
	* Constructor.
	*/
	GKMeansParams(void);
};

//-----------------------------------------------------------------------------
/**
* The GGroupingKCos provides a representation for a method to group
some
* subprofiles using the KMeans algorithm adapted to cosinus distance.
* @author David Wartel
* @short KMeansCos Grouping.
*/

class GGroupingKCos  : public GGrouping
{

	/**
	* KMeans Parameters
	*/
	GKMeansParams* Params;

public:

	/**
	* Constructor.
	* @param s              Session.
	* @param grps          Ideal groupment.
	*/
	GGroupingKCos(GSession* s, GKMeansParams* p) throw(bad_alloc);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param s*             C string coding the settings.
	*/
	void SetSettings(const char* s);

	/**
	* is the group valid ?
	*/
	bool IsValid(GGroup* ) {return(true);} ;


protected:

	/**
	* Make the grouping for a specific Language.
	*/
	void Run(void) throw(GException);


public:

	/**
	* Destructor.
	*/
	virtual ~GGroupingKCos(void);
};


}  //-------- End of namespace GALILEI----------------------------------------


//-----------------------------------------------------------------------------
#endif

