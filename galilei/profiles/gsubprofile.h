/*

	GALILEI Research Project

	GSubProfile.h

	Sub-Profile - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#ifndef GSubProfileH
#define GSubProfileH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* This class represents a sub-profile for a specific language. The sub-profiles
* are ordered by identificator. A sub-profile is in reality a container of
* sub-profile descriptions.
* @author Pascal Francq.
* @short Sub-Profile.
*/
class GSubProfile
{
protected:

	/**
	* Identifier of the subprofile.
	*/
	unsigned Id;

	/**
	* Owner profile of the subprofile.
	*/
	GProfile* Profile;

	/**
	* Language of the subprofile.
	*/
	GLang* Lang;

	/**
	* Corresponding Group.
	*/
	GGroup* Group;

	/**
	* Date of attachment to the group.
	*/
	RTimeDate::RDate Attached;

public:

	/**
	* Constructor of the subprofile.
	* @param prof           Profile.
	* @param id             Identifier.
	* @param lang           Language of the subprofile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfile(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a) throw(bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const=0;

	/**
	* Create a new profile.
	* @param GProfile*      Pointer to the profile.
	* @param unsigned int   Identifier.
	* @param GLang*         Language of the subprofile.
	* @param GGroup*        Pointer to the group.
	* @param const char*    C String representing the date where it was attached.
	*/
	static GSubProfile* NewSubProfile(GProfile*,unsigned int,GLang*,GGroup*,const char*) {return(0);}

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const unsigned int id) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfile& subprofile) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfile* subprofile) const;
	
	/**
	* Get the identifier of the subprofile.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

	/**
	* Get the language of the subprofile.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Get the corresponding profile.
	* @return Pointer to the profile.
	*/
	GProfile* GetProfile(void) const {return(Profile);}

	/**
	* Compute similarity between SubProfiles.
	*/
	virtual double Similarity(const GSubProfile*) const {return(0.0);};

	/**
	* Compute similarity between SubProfiles using a global approach.
	*/
	virtual double GlobalSimilarity(const GSubProfile*) const {return(0.0);};

	/**
	* Compute similarity between a subProfile and a document.
	*/
	virtual double Similarity(const GDoc*) const {return(0.0);};

	/**
	* Compute similarity between a subProfile and a document using a global
	*  approach.
	*/
	virtual double GlobalSimilarity(const GDoc*) const {return(0.0);};

	/**
	* See if the subprpfile is defined, i.e. if it is computed. A subprofile
	* that isn't computed, isn't attached.
	*/
	virtual bool IsDefined(void) const {return(false);};

	/**
	* Get the corresponding group.
	* @returns Pointer to GGroup.
	*/
	GGroup* GetGroup(void) const {return(Group);}

	/**
	* Set the group for the subprofile.
	* @params grp           Group where to attached.
	*/
	void SetGroup(GGroup* grp);

	/**
	* Get the date of the last attachment.
	* @returns Pointer to date.
	*/
	const RTimeDate::RDate* GetAttached(void) const {return(&Attached);}

	/**
	* See if the subprofiles was updated until the last attachment to a group.
	* @returns Boolean.
	*/
	bool IsUpdated(void) const;

	/**
	* Get the number of common OK document between two subprofiles. In
	* practice, it computes the number of the common documents of the
	* corresponding profiles which are of the language of the profile.
	*/
	unsigned int GetCommonOKDocs(const GSubProfile* prof) const;

	/**
	* Get the number of common document between two subprofiles. In
	* practice, it computes the number of the common documents of the
	* corresponding profiles which are of the language of the profile.
	*/
	unsigned int GetCommonDocs(const GSubProfile* prof) const;

	/**
	* Get the number of common document with different judgement between two
	* subprofiles. In practice, it computes the number of the common documents
	* of the corresponding profiles which are of the language of the profile.
	*/
	unsigned int GetCommonDiffDocs(const GSubProfile* prof) const;

	/**
	* Get the number of documents judged by the subprofile. In practice, it
	* computes the number of the common documents of the corresponding profiles
	* which are of the language of the profile.
	*/
	unsigned int GetNbJudgedDocs(void) const;

	/**
	*Destructor
	*/
	virtual ~GSubProfile(void);
};


//-----------------------------------------------------------------------------
/**
* The GSubProfileCursor class provides a way to go trough a set of subprofiles.
* @short SubProfiles Cursor
*/
CLASSCURSOR(GSubProfileCursor,GSubProfile,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

