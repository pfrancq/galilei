/*

	GALILEI Research Project

	GIdealGroup.h

	Get the jugments for the profiles

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
#ifndef GIdealGroupH
#define GIdealGroupH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**  This Class implement a representation of a ideal groupment for GALILEI
*  @author Pascal Francq
*  @short Ideal Groupment
*/
class GIdealGroup
{
protected:

	/**
	* The Users of the galilei system.
	*/
	GSession* Session;
	
	/**
	* The percentage of ok documents.
	*/
	double PercOK;

	/**
	* The percentage of ko documents.
	*/
	double PercKO;

	/**
	* The percentage of hs documents compare to Ok documents.
	*/
	double PercHS;

	/**
	* The Percentage of documents judged whith error.
	*/
	double PercErr;

	/**
	* The Percentage of Group to use.
	*/
	double PercGrp;

	/**
	* The minimal number of docs in the group to keep it.
	*/
	unsigned int NbDocPerGrp;

	/**
	* Parameter to initialyse the random function for autocreate jugment.
	* 0 to use randomly, >0 to use whit the same su sequence
	*/
	int Rand;

	/**
	* The maximal number of profiles in a group.
	*/
	unsigned int NbProfMax;

	/**
	* The minimal number of profiles in a group.
	*/
	unsigned int NbProfMin;

	/**
	* The % of profiles who are social	
	*/
	double PercSocial;

	/**
	* Temporary Array of documents.
	*/
	GDoc** Docs;

	/**
	* Number of documents actually in Docs.
	*/
	unsigned int NbDocs;

	/**
	* The date of today.
	*/
	char today[12];

	/**
	* Container of last added subprofiles.
	*/
	RStd::RContainer<GSubProfile,unsigned int,false,true> LastAdded;

public:

	/**
	* Constructor Get GIdealGroup Function.
	* Create the profiles,subprofiles and the judgment randomly from a file
	* where is stoked the grouping of document.
	* @param session        The Galilei Session.
	*/
	GIdealGroup(GSession* session);

	/**
	* Create randomly the judgment for all the users.
	* @param parent         The container to assign groupid and parent id.
	* @param groups         The ideal groupment into a GGroups container.
	* @param Save           Save the results.
	*/
	void Run(bool Save);

	/**
	* Choose the subjects that will be used.
	*/
	void ChooseSubjects(void);

	/**
	* Create the set based on the subjects used.
	*/
	void CreateSet(void);

	/**
	* Add assessments to the profiles creates.
	* @param Save           Save the results.
	* @param nbDocs         Number of Documents to judge.
	*/
	void AddAssessments(bool Save,unsigned int nbDocs);

	/**
	* Simulatation of judgments for a profile for a given subject.
	* @param prof           Profile that judges the documents.
	* @param sub            Subject.
	* @param maxDocOK       Maximal OK Documents to judge.
	* @param maxDocKO       Maximal KO Documents to judge.
	* @param maxDocH        Maximal H Documents to judge.
	*/
	void ProfileJudges(GProfile* prof,GSubject* sub,unsigned int maxDocsOK,unsigned int maxDocsKO,unsigned int maxDocsH);
	
	/**
	* Add profiles of a new not used topic.
	* @param Save � � � � � Save the results.
	* @return true if a not used topic was found.
	*/
	bool AddTopic(bool Save);

	/**
	* Add judgements for some new not used profiles.
	* @param minprofiles � � � � � minimum number of new profiles to create.
	* @param maxprofiles � � � � � maximum number of new profiles to create.
	*/
	unsigned int AddProfiles(unsigned int minprofiles, unsigned int maxprofiles, bool Save);

	/**
	* Computed the percentage of correct assignments for the subprofiles last
	* added.
	* @return double;
	*/
	double ComputePercAss(void);

	/**
	* Clear the container of last added subprofiles.
	*/
	void ClearLastAdded(void);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param s*             C string coding the settings.
	*/
	void SetSettings(const char* s);

	/**
	* Destructor.
	*/
	virtual ~GIdealGroup(void);
};

}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
