/*

	GALILEI Research Project

	GDocProfSims.h

	Similarities between documents and subprofiles - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

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



//------------------------------------------------------------------------------
#ifndef GDocProfSimsH
#define GDocProfSimsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The GDocProfSims class provides a representation for the similarities between
* documents and profiles.
* @author Pascal Francq and Valery Vandaele
* @short SubProfiles-Documents Similarities.
*/
class GDocProfSims
{
	// Internal class
	class GDocProfSim;

	/**
	* Similarities.
	*/
 	R::RContainer<GDocProfSim,unsigned int,true,true> Sims;

	/**
	* Session managing the (sub)profiles and the documents.
	*/
	GSession* Session;

	/**
	* Inverse Frequency Factor must be used to compute the similarities.
	*/
	bool IFF;

public:

	/**
	* Constructor of the similarities between documents and subprofiles.
	* @param session         Session.
	* @param iff             Use Inverse Frequency Factor.
	*/
	GDocProfSims(GSession* session,bool iff) throw(bad_alloc);

	/**
	* Re-initialize the similarities. This method can be used for testing
	* purpose when it is necessary to start from different initial conditions.
	*/
	void ReInit(void) throw(bad_alloc);

	/**
	* Set if the Inverse Frequency Factor should be used.
	* @param iff             Use Inverse Frequency Factor.
	*/
	void UseIFF(bool iff) throw(bad_alloc);

	/**
	* Get the similarity between a document and a subprofile.
	* @param doc             Pointer to the document.
	* @param sub             Pointer to the subprofile.
	*/
	double GetSim(const GDoc* doc,const GSubProfile* sub) throw(GException);

	/**
	* Destructor of the similarities between documents and subprofiles.
	*/
	virtual ~GDocProfSims(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
