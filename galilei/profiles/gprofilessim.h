/*

	GALILEI Research Project

	GProfilesSim.h

	Similarities between Profiles - Header

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
#ifndef GProfilesSimH
#define GProfilesSimH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GProfilesSim class provides a representation for all the similarities
* between the profiles, i.e. the subprofiles of a given language.
* @author Pascal Francq
* @short Profiles Similarities.
*/
class GProfilesSim
{
	class GSims;

	/**
	* The similarities.
	*/
	RStd::RContainer<GSims,unsigned int,true,true> Sims;

public:

	/**
	* Constructor.
	* @param s              Subprofiles of the system.
	* @param global         Global approach.
	*/
	GProfilesSim(RStd::RContainer<GSubProfile,unsigned int,false,true>* s,bool global) throw(bad_alloc);

	/**
	* Constructor.
	* @param s              Subprofiles of the system.
	* @param global         Global approach.
	*/
	GProfilesSim(RStd::RContainer<GSubProfile,unsigned int,false,true>& s,bool global) throw(bad_alloc);

	/**
	* Get the similarities between two profiles, i.e. the subprofiles of a same
	* language.
	* @param i              Identificator of the first subprofile.
	* @param j              Identificator of the second subprofile.
	* @return double.
	*/
	double GetSim(unsigned int i,unsigned int j);

	/**
	* Get the similarities between two profiles, i.e. the subprofiles of a same
	* language.
	* @param s1             Pointer to the first subprofile.
	* @param s2             Pointer to second subprofile.
	* @return double.
	*/
	double GetSim(const GSubProfile* s1,const GSubProfile* s2);

	/**
	* Destructor.
	*/
	~GProfilesSim(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
