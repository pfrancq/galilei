/*

	GALILEI Research Project
	
	GSubjectTree.h

	GSubjectTree - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#ifndef GSubjectTreeH
#define GSubjectTreeH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rtree.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GSubjectTree class provides a representation for a tree of subject
* handle by the system.
* @author GALILEI Team
* @short Tree of the Galilei subject.
*/
class GSubjectTree : public RStd::RTree<GSubject,true,false>
{
protected:

	/**
	* Number of profiles needed.
	*/
	unsigned int NbProfiles;

	/**
	* Number of documents Ok per profile.
	*/
	unsigned int NbDocsOk;

	/**
	* Number of documents Ko per profile.
	*/
	unsigned int NbDocsKo;

	/**
	* Number of documents Hs per profile.
	*/
	unsigned int NbDocsHs;

	/**
	* Number of users in database;.
	*/
	unsigned int NbUsers;

	/**
	* The date of today.
	*/
	char today[12];

	/**
	* the date.
	*/
	RTimeDate::RDate date;

	/**
	* Container of all the profiles.
	*/
	RStd::RContainer<GProfile,unsigned int,false,true>* profiles;

	/**
	* The idealgroupment of document.
	*/
	RContainer<GGroupsEvaluate,unsigned int,false,false>* IdealDoc;

	/**
	* set all hasjudged boolean to false;
	*/
	void InitProfiles(void);

	/**
	* set all isjudged boolean to false;
	*/
	void InitSubSubjects(void);


public:
	
	/**
	* Constructor.
	* @param nbOK           %of ok document.
	* @param nbKO           %of ok document.
	* @param nbusers        nb of users in the database.
	*/
	GSubjectTree(unsigned int nbOk,unsigned int nbKo,unsigned int nbusers);

	/**
	* Static function used to ordered by frenquecy.
	*/
	static int sortOrder(const void *a,const void *b);

	/**
	* Chose subject to juge whith
	* @param ses            The Gailieli session
	* @param precgrp        The perc of group who will be in the session.
	* @param NBDocPerGrp    The minimal number of document per group to keep the group.
	*/
	void ChooseSubject(GSession* ses,unsigned int percgrp,unsigned int NbDocPerGrp);

	/**
	* Executes the queries to create new profiles.
	* @param ses            The Gailieli session
	* @param precok         The perc of ok document who will judge the profile.
	* @param precko         The perc of ko document who will judge the profile.
	* @param prechs         The perc of hs document who will judge the profile (percentage of ok documents).
	* @param nbmin          The minimal number of profiles by groups
	* @param nbmax          The maximal number of profiles by groups
	* @param presocial      The % of profiels who are social
	* @param precerr        The perc of document who will judge the profile non specialy conforming to the ideal groupment.
	*/
	void Judgments(GSession* ses,int percok,int precko,int prechs,unsigned int nbmin,unsigned int nbmax,unsigned int percsocial,int precerr);

	/**
	* Judge documents from the susubject sub.
	* @param profid         Id of the profile wich judges the documents
	* @param sub            Subsubject containing the docs to judge.
	* @param i              I=1 for OK judgement, i=2 for KO judgment i=3 for hs documents.
	* @param precerr        The perc of document who will judge the profile non specialy conforming to the ideal groupment.
	*/
	void JudgeDocuments(int profid,GSubject* sub,int i, GSession* ses,int precerr);

	/**
	* Write the ideal groupment into a file
	* @param url         The url where the file is saved.
	*/
	void IdealGroupmentFile(const char* url);

	/**
	* Creates insert the profiles into subjects
	*/
	void InsertProfiles(void);

	/**
	* Filling in the conatiner of group with a ideal groupment.
	* @param groups      The Ggroups container to fill in.
	* @param ses         The Galieli session.
	*/
	void IdealGroupment(RStd::RContainer<GGroups,unsigned int,true,true>* groups,GSession* ses,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent);

	/**
	* Creates a container of GroupIdParentId from a subjectree.
	*/
	void CreateParent(RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent);

	/**
	* Destructor
	*/
	~GSubjectTree(void);
};


} //-------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------

#endif

