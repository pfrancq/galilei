/*

	GALILEI Research Project

	GSubProfileSamesDocs.h

	SubProfiles with common documents judged - Header

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GSubProfileSamesDocsH
#define GSubProfileSamesDocsH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GSubProfileSamesDocs class provides a representation of a couple of
* subprofiles having a number of documents in common.
* @author Pascal Francq
* @short SubProfiles judging common documents.
*/
class GSubProfilesSameDocs
{
private:

	/**
	* Identificator of the first subprofile.
	*/
	RStd::tId Id1;

	/**
	* Identificator of the Second subprofile.
	*/
	RStd::tId Id2;

	/**
	* Ratio of "common documents" judged either with a similar feedback or with
	* different feedbacks by the subprofiles.
	*/
	double Ratio;

public:

	/**
	* Constructor.
	* @param i1             Identificator of the first subprofile.
	* @param i2             Identificator of the second subprofile.
	* @param nb             Number of common documents.
	* @param tot            Total number documents.
	*/
	GSubProfilesSameDocs(RStd::tId i1,RStd::tId i2,double r);

	/**
	* Compare method by RStd::RContainer.
	*/
	int Compare(const GSubProfilesSameDocs* g) const;

	/**
	* Compare method by RStd::RContainer.
	*/
	int Compare(const GSubProfilesSameDocs& g) const;

	/**
	* Look if a given subprofile is one of the couple.
	* @param obj            Identificator to test.
	* @return bool.
	*/
	bool IsIn(const RStd::tId obj) const;

	/**
	* Get the ratio.
	* @return double.
	*/
	double GetRatio(void) const {return(Ratio);}

	/**
	* Get the little identificator.
	* @return tId.
	*/
	RStd::tId GetId1(void) const {return(Id1);}

	/**
	* Get the highest identificator.
	* @return tId.
	*/
	RStd::tId GetId2(void) const {return(Id2);}
};


//-----------------------------------------------------------------------------
/**
* The GSubProfilesSameDocsCursor class provides a way to go trough a set of
* GSubProfilesSameDocs.
* @short SubProfiles judging common documents Cursor.
*/
CLASSCURSOR(GSubProfilesSameDocsCursor,GSubProfilesSameDocs,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

