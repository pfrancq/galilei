/*

	GALILEI Research Project

	GSubProfileDescVector.h

	Sub-Profile Description using a vector - Header.

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
#ifndef GSubProfileDescVectorH
#define GSubProfileDescVectorH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gsubprofiledesc.h>
#include <infos/giwordweight.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GSubProfile;
class GGroup;
class GIWordsWeights;


//-----------------------------------------------------------------------------
/**
* The GSubProfileDescVector provides a representation for a sub-profile
* described as a vector corresponding to a list of pairs {keyword,weight}.
* @author Pascal Francq.
* @short Sub-Profile Vector Description.
*/
class GSubProfileDescVector : public GSubProfileDesc
{
	/**
	* List representing the sub-profile.
	*/
	GIWordsWeights* Vector;

public:

	/**
	* Constructor of the subprofile.
	* @param sub            Sub-Profile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfileDescVector(GSubProfile* sub,GGroup* grp,const char* a) throw(bad_alloc);

	/**
	* Get the type of the description.
	* @returns tSubProfileDesc enum type.
	*/
	virtual tSubProfileDesc GetType(void) const {return(sdVector);}

	/**
	* See if the subprpfile is defined, i.e. if it is computed. A subprofile
	* that isn't computed, isn't attached.
	*/
	virtual bool IsDefined(void) const;

	/**
	* Add a word with the corresponding weight to the description.
	* @param id             Identificator of the word.
	* @param w              Weight of the word.
	*/
	void AddWord(unsigned int id,double w) throw(bad_alloc);

	/**
	* Compute similarity between SubProfiles.
	*/
	virtual double Similarity(const GSubProfileDesc* desc) const;
	
	/**
	* Get a cursor over the vector.
	* @return GIWordWeightCursor element.
	*/
	GIWordWeightCursor& GetVectorCursor(void);

	/**
	* Get the number of elements of the vector that are not null.
	*/
	unsigned int GetNbNoNull(void) const;

	/**
	*Destructor.
	*/
	~GSubProfileDescVector(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

