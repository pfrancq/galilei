/*

	GALILEI Research Project
	
	GSubjectTree.h

	GSubjectTree - Implementation.

	(C) 2002 by the GALILEI Team

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
#include <rmath/random.h>


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
	int NbProfiles;

	/**
	* Number of documents Ok per profile.
	*/
	int NbDocsOk;

	/**
	* Number of documents Ko per profile.
	*/
	int NbDocsKo;

	/**
	* Number of users in database;.
	*/
	int NbUsers;

	/**
	* Container of all the profiles.
	*/
	RStd::RContainer<GProfile,unsigned,false,true>* profiles;

	/**
	* set all hasjudged boolean to false;
	*/
	void InitProfiles(void);

	/**
	* set all isjudged boolean to false;
	*/
	void InitSubSubjects(void);

	/**
	* Random number generator
	*/
	RMath::RRandom* Random;

public:
	
	/**
	* Constructor.
	* @param nbOK        %of ok document.
	* @param nbKO        %of ok document.
	* @param nbusers     nb of users in the database.
	*/
	GSubjectTree(unsigned int nbOk,unsigned int nbKo,unsigned int nbusers);

	/**
	* Executes the queries to create new profiles.
	* @param ses         The Gailieli session
	* @param rand        0 total random >0 restart the sequence of randum number
	* @param nbmin       the minimal number of profiles by groups
	* @param nbmax       the maximal number of profiles by groups
	*/
	void Judgments(GSession* ses,int rand,int percok,int precko,int nbmin,int nbmax);

	/**
	* Judge documents from the susubject sub.
	* @param profid      id of the profile wich judges the documents
	* @param sub         subsubject containing the docs to judge.
	* @param i           i=1 for OK judgement, i=0 for KO judgment.
	*/
	void JudgeDocuments(int profid,GSubject* sub,bool i, GSession* ses);

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
	* Filing in the conatiner of group with a ideal groupment.
	* @param groups      The Ggroups container to fill in.
	* @param ses         The Galieli session.
	*/
	void IdealGroupment(RStd::RContainer<GGroups,unsigned int,true,true>* groups,GSession* ses,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent);

	/**
	* Destructor
	*/
	~GSubjectTree(void);
};


} //-------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------

#endif

