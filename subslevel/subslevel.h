/*

	GALILEI Research Project

	SubsLevel.h

	Compute Profiles and Documents Confidence - Header.

	Copyright 2005-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2005 by David Wartel.
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GSubsLevelH
#define GSubsLevelH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gpostcommunity.h>
#include <gcommunity.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
struct ProfileScoring
{
	size_t ProfileId;
	double Score;
	size_t NbDocs;

	ProfileScoring(size_t id) : ProfileId(id), Score(0.0), NbDocs(0) {}
	int Compare(const ProfileScoring& scoring) const {return(CompareIds(ProfileId,scoring.ProfileId));}
	int Compare(const size_t id) const {return(CompareIds(ProfileId,id));}
};


//------------------------------------------------------------------------------
struct ProfileDoc
{
	size_t ProfileId;
	RDate When;

	ProfileDoc(size_t profileid,const RDate& when)
		: ProfileId(profileid), When(when) {}
	int Compare(const ProfileDoc&) const {return(-1);}
};


//------------------------------------------------------------------------------
/**
* The SubsLevel provides a method to compute each grouped profile and the
* corresponding assessed documents.
* @author Pascal Francq and David wartel
* @short Compute Profiles and Documents Confidence
*/
class SubsLevel  : public GPostCommunity
{
	/**
	* Array of documents.
	*/
	R::RContainer<ProfileDoc,false,false> Fdbks;

	/**
	 * A list of relevant documents.
	 */
	GCommunityDocs Docs;

	/**
	 * Scores of the profiles.
	 */
	RContainer<ProfileScoring,true,true> Scores;

	/**
	* Number of levels for the profiles.
	*/
	size_t NbLevels;

public:

	/**
	* Constructor.
	* @param fac             Factory.
	*/
	SubsLevel(GFactoryPostCommunity* fac);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Static function used to order the feedbacks by date (the latest
	* first). The function can be used with the RContainer::ReOrder method.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int SortOrderAssessed(const void* a,const void* b);

	/**
	 * Compute a date-ordered list of assessments of a given document for a
	 * given community.
	 * @param docid          Document identifier.
	 * @param group          Community.
	 */
	void ComputeOrderedFdbks(size_t docid,GCommunity* group);

	/**
	 * Compute a list of assessments of a given document for a given community.
	 * @param docid          Document identifier.
	 * @param group          Community.
	 */
	void ComputeFdbks(size_t docid,GCommunity* group);

	/**
	 * Compute the confidence of the profiles.
	 */
	void ComputeProfilesConfidence(void);

	/**
	 * Compute the confidence of the documents.
	 */
	void ComputeDocsConfidence(void);

    /**
	* Compute the confidence scores of the profiles and the documents.
	*/
	virtual void Run(void);
};


//-----------------------------------------------------------------------------
#endif

