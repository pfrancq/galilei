/*

	GALILEI Research Project

	GGroupingOptions.h

	Options for the grouping methods - Header.

	(C) 2002 by P. Francq.

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
#ifndef GGroupingOptionsH
#define GGroupingOptionsH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroupingOptions provides a representation for the options concerning the
* grouping methods.
* @author Pascal Francq
* @short Grouping Options.
*/
class GGroupingOptions
{
public:

	/**
	* Method used for the grouping.
	*/
	tProfilesGroup Method;

	/**
	* Full Similarity for the groups.
	*/
	bool FullSim;

	/**
	* Level of similarity for the groupement.
	*/
	double LevelSim;

	/**
	* Size of the population of the GA.
	*/
	unsigned int GAPopSize;

	/**
	* Maximal number of generation to run.
	*/
	unsigned int GAMaxGen;

	/**
	* Is the GA in step mode?
	*/
	bool GAStep;

	/**
	* Number of generation for each step.
	*/
	unsigned int GAStepGen;

	/**
	* Default Constructor.
	*/
	GGroupingOptions(void);

	/**
	* Copy Constructor.
	*/
	GGroupingOptions(const GGroupingOptions& opt);

	/**
	* Copy Constructor.
	*/
	GGroupingOptions(const GGroupingOptions* opt);

	/**
	* Assignment operator using another options.
	*/
	GGroupingOptions& operator=(const GGroupingOptions& opt);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
