/*

	GALILEI Research Project

	GGroupingSim.h

	Heuristic using Similarity - Header.

	(C) 2001 by P. Francq.

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
#ifndef GGroupingSimH
#define GGroupingSimH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggrouping.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GGroup;


//-----------------------------------------------------------------------------
/**
* The GGroupingSim provides a representation for a method to group some
* subprofiles using the notion of similarity.
* @author Pascal Francq
* @short Similarity Grouping.
*/
class GGroupingSim : public GGrouping
{
	/**
	* Level of similarity to have two subprofiles considered as similar.
	*/
	double Level;

	/**
	* Determine if full similarity is needed, i.e. if a subprofile has to be
	* similar with all the other subprofiles to have the group coherent, or
	* just one.
	*/
	bool Full;

public:

	/**
	* Constructor.
	* @param s              Session.
	* @param f              Full (true/false)
	* @param l              Level.
	*/
	GGroupingSim(GSession* s,const bool f=true,const double l=0.5) throw(bad_alloc);

	/**
	* Initialisation of the method.
	*/
	virtual void Init(void) throw(bad_alloc);

	/**
	* Get the level of similarity used.
	* @return Double.
	*/
	double GetLevel(void) const {return(Level);}

	/**
	* See if the similarity is full.
	* @return Boolean.
	*/
	bool GetFull(void) const {return(Full);}

	/**
	* Set if the similarity has to be full.
	* @param full           Boolean value.
	*/
	void SetFull(bool full) {Full=full;}

	/**
	* Set the level of similarity used.
	* @param level          Level to use.
	*/
	void SetLevel(double level) {Level=level;}

protected:

	/**
	* Test if a group is coherent. Actually, this means verify that all the
	* subprofiles of the group are similar together.
	* @param grp            Group to verify.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GGroup* grp) const;

	/**
	* Test if a group is still coherent if a subprofile is added to it.
	* Actually, this means verify that all the subprofiles of the group are
	* similar to the one tested.
	* @param grp            Group to verify.
	* @param sub            SubProfile eventually to add.
	* @returns True if it is coherent.
	*/
	bool IsCoherent(const GGroup* grp,const GSubProfile* sub) const;

	/**
	* Test if a group is valid. If a group isn't not valid, the group is
	* deleted and all profiles are to be inserted again.
	*/
	virtual bool IsValid(GGroup* grp);

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void);

public:

	/**
	* Destructor.
	*/
	virtual ~GGroupingSim(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
