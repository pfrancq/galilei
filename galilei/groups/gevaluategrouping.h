/*

	GALILEI Research Project

	GEvaluateGrouping.h

	Compare a ideal groupement with a computed one - Header.

	(C) 2002 by David Wartel

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
#ifndef GEvaluateGroupingH
#define GEvaluateGroupingH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GEvaluateGrouping provides a way to evaluate a clustering by
* providing a list of criterion.
* @author David Wartel
* @short Clustering Evaluation .
*/
class GEvaluateGrouping
{


protected:

	/**
	* Name of the evaluation method.
	*/
	RString Name;

	/**
	* Id of the evaluation method.
	*/
	unsigned int Id;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Ideal Groups.
	*/
	RContainer<GGroups,unsigned int,true,true>* Groups;


	/**
	* all subprofiles grouped..
	*/
	RStd::RContainer<GSubProfile,unsigned int,false,true> SubProfiles;




public:

	/**
	* Constructor.
	* @param s              Session.
	* @param groups         Ideal groupement.
	*/
	GEvaluateGrouping(RString name, GSession* s,RContainer<GGroups,unsigned int,true,true>* groups, unsigned int i) throw(bad_alloc);


public:

	/**
	* return the name of the method;
	*/
	RString GetName(void){return(Name);};

    /**
	* runs the evaluation of the clustering..
	*/
	virtual double Run(void)=0;

	/**
	* Comparaison function
	*/
	int Compare(const unsigned int& id) const {return(Id-id);};;
	
	/**
	* Comparaison function
	*/
	int Compare(const GEvaluateGrouping& eval) const {return(Id-eval.Id);};;
	
	/**
	* Comparaison function
	*/
	int Compare(const GEvaluateGrouping* eval) const {return(Id-eval->Id);};


	/**
	* Destructor.
	*/
	virtual ~GEvaluateGrouping(void);
};

//-----------------------------------------------------------------------------
/**
* The GGroupingCursor class provides a way to go trough a set of grouping 
* evaluation methods.
* @short Grouping Evaluation Methods Cursor
*/
CLASSCURSOR(GEvaluateGroupingCursor,GEvaluateGrouping,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
