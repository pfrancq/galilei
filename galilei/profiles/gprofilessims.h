/*

	GALILEI Research Project

	GProfilesSims.h

	List of SubProfiles for a given Language - Implementation.

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
#ifndef GProfilesSimsH
#define GProfilesSimsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The GProfilesSims class provides a representation for a set of Similarity between Profiles of
* a given language.
* @author Pascal Francq and Valery Vandaele
* @short SubProfiles.
*/
class GProfilesSims
{
	// Internal class
	class GProfilesSim;

	/**
	* Similarities.
	*/
	R::RContainer<GProfilesSim,unsigned int,true,true> Sims;

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
	* Constructor of the similarities between subprofiles.
	* @param session         Session.
	* @param iff             Use Inverse Frequency Factor.
	*/
	GProfilesSims(GSession* session,bool iff) throw(bad_alloc);

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
	* Get the similarity between two subprofiles.
	* @param sub1            Pointer to the first subprofile.
	* @param sub2            Pointer to the second subprofile.
	*/
	double GetSim(const GSubProfile* sub1,const GSubProfile* sub2) throw(GException);

	/**
	* Get the minimum of similarityof the subprofiles, needed by clusteirng
	* algorithms.
	* @param lang            Language.
	* @param deviationrate   factor of the standart deviation.
	*/
	double GetMinimumOfSimilarity(GLang* lang, double deviationrate=1.5) throw(GException);

	/**
	* Add a subprofile to the listof the modified one.
	* @param sub             Pointer to the subprofile.
	*/
	void AddModifiedProfile(GSubProfile* sub) throw(bad_alloc,GException);

	/**
	* Destructor.
	*/
	virtual ~GProfilesSims(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
