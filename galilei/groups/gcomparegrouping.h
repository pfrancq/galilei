/*

	GALILEI Research Project

	GCompareGrouping.h

	Compare a ideal groupement with a computed one - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#ifndef GCompareGroupingH
#define GCompareGroupingH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GCompareGrouping provides a way to compare the groupement obtained with a
* ideal one by computed the precision, the recall and the adjusted rand index.
* @author Pascal Francq and Julien Lamoral
* @short Clustering Comparaison.
*/
class GCompareGrouping
{
	class GroupScore;
	class GGroupId;
	CLASSCURSOR(GroupScoreCursor,GroupScore,unsigned int)

protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Ideal Groups.
	*/
	RContainer<GGroups,unsigned int,true,true>* Groups;

	/**
	* Precision of the groupement.
	*/
	double Precision;

	/**
	* Recall of the groupement.
	*/
	double Recall;

	/**
	* Total comparaison between for groupment.
	*/
	double Total;

	/**
	* SubProfiles that where changed and must be grouped again.
	*/
	RContainer<GroupScore,unsigned int,true,true>* GroupsScore;

public:

	/**
	* Constructor.
	* @param s              Session.
	* @param groups         Ideal groupement.
	*/
	GCompareGrouping(GSession* s,RContainer<GGroups,unsigned int,true,true>* groups) throw(bad_alloc);

protected:

	/**
	* Get the ideal group of the subprofile.
	* @param sub            SubProfile.
	*/
	GGroup* GetIdealGroup(GSubProfile* sub) const;

	/**
	* Compute the Recall and the Precision.
	* @param rec            Receiver of the signals.
	*/
	void ComputeRecallPrecision(GSlot* rec=0);

	/**
	* Compute the Total based on the Adjusted Rand Index (Hubert and Arabie)
	* The total is the mean of the total for each languages pondered by the
	* number of subprofiles by languages.
	* @param rec            Receiver of the signals.
	*/
	void ComputeTotal(GSlot* rec=0);

public:

	/**
	* Make the groups.
	* @param rec            Receiver of the signals.
	* @param modified   Recompute only modified elements or all.
	*/
	void Compare(GSlot* rec=0);

	/**
	* Get the total precision of the groupement.
	*/
	double GetPrecision(void) const {return(Precision);}

	/**
	* Get the total recall of the groupement.
	*/
	double GetRecall(void) const {return(Recall);}

	/**
	* Get the total recall of the groupement.
	*/
	double GetTotal(void) const {return(Total);}

	/**
	* Get the precision of a group.
	* @param grp            Group.
	*/
	double GetPrecision(GGroup* grp);

	/**
	* Get the total recall of a group.
	* @param grp            Group.
	*/
	double GetRecall(GGroup* grp);

	/**
	* Destructor.
	*/
	virtual ~GCompareGrouping(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
