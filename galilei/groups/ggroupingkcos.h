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
#include <rstd/rcontainercursor.h>

//-----------------------------------------------------------------------------
// include files for GALILEI
#include<galilei.h>
#include <groups/ggroupingkmeans.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GGroup;


//-----------------------------------------------------------------------------
/**
* The GGroupingKCos provides a representation for a method to group
some
* subprofiles using the KMeans algorithm adapted to cosinus distance.
* @author David Wartel
* @short KMeansCos Grouping.
*/

class GGroupingKCos :   public GGroupingKMeans
{


protected:

	/**
	* Container of subprofiles considered as prototypes,
	* needed to calculate the error between two iterations
	*/
	RStd::RContainer<GSubProfile,unsigned int,false,false>* protoserror;

	/**
	* Temporary container of groupment, needed to run tests
	*/
	RStd::RContainer<GGroup,unsigned int,false,false>* grpstemp;

	/**
	* Container of the final groupment
	*/
	RStd::RContainer<GGroup,unsigned int,false,false>* grpsfinal;


public:

	/**
	* Constructor.
	* @param s              Session.
	* @param grps          Ideal groupment.
	*/
	GGroupingKCos(GSession* s) throw(bad_alloc);

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
	* Initialisation of the method.
	*/
	void Init(void) throw(bad_alloc);

	/**
	* Calculates the cost function for a kmeanscos clustering
	*/
	double CostFunction(RStd::RContainer<GGroup,unsigned int,false,false>* grps);

	/**
	*  reallocate the subprofiles to prototypes
	*/
	void ReAllocate(RStd::RContainer<GSubProfile, unsigned int, false, true>* dataset);

	/**
	*  recenters the prototypes
	*/
	void ReCenter(void);

	/**
	*  excute the kmeans algorithm
	* @param init           initialisation step?
	*/
	void Execute(RStd::RContainer<GSubProfile, unsigned int, false, true>* Dataset, unsigned int nbtests);

	/**
	* Calculate the distance between two subprofiles
	* @param init           initialization step?
	*/
	double Distance(GSubProfile *s1, GSubProfile *s2);

	/**
	*  calculates the error between two iterations
	*/
	int CalcError(void);

	/**
	* returns the Calinsky index for the clustering
	*/
	double CalcCalinsky(void);

	/**
	* returns the knearestneighboors measure.
	*/
	double TestMeasure(void);

	/**
	* displays infos about kmeanscos parameters
	*/
	void DisplayInfos(void);


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

