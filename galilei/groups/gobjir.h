/*

	GALILEI Research Project

	RObjIR.h

	IR Object to place in a specific group - Header.

	(c) 2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef GObjIRH
#define GObjIRH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GObjIR class provides a representation of subprofile to to place in a
* group.
* @author Pascal Francq
* @short IR Object.
*/
class GObjIR
{
protected:

	/**
	* The identificator of the object.
	*/
	unsigned int Id;

	/**
	* Pointer to a subprofile.
	*/
	GSubProfile* SubProfile;

	/**
	* Sum of all the "probabilities" of the documents.
	*/
	double SumPjk;

public:

	/**
	* Construct the object.
	* @param id             Identificator.
	* @param s              Corresponding subprofile.
	*/
	GObjIR(const unsigned int id,GSubProfile* s);

	/**
	* Copy Construct.
	* @param obj            Source used.
	*/
	GObjIR(const GObjIR* obj);

	/**
	* Comparaison function
	*/
	int Compare(const unsigned int &id) const;
	
	/**
	* Comparaison function
	*/
	int Compare(const GObjIR& obj) const;
	
	/**
	* Comparaison function
	*/
	int Compare(const GObjIR* obj) const;

	/**
	* Return the identificator of the object.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Return the subprofile of the object.
	*/
	GSubProfile* GetSubProfile(void) const {return(SubProfile);}

	/**
	* Compute the sum of all the "probabilities" of the documents.
	* @param lang           Language.
	* @param s              Session.
	*/
	void ComputeSumEntropy(GLang* lang,GSession* s);

	/**
	* Destruct the object.
	*/
	virtual ~GObjIR(void);

	// friend classes
	friend class GGroupIR;
};


//-----------------------------------------------------------------------------
/**
* The GObjIRCursor class provides a way to go trough a set of GA objects.
* @short GA Objects Cursor.
*/
CLASSCURSOR(GObjIRCursor,GObjIR,unsigned int)


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
