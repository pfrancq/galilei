/*

	GALILEI Research Project
	
	GSubjectTree.h

	GSubjectTree - Implementation.

	(C) 1999-2001 by J.Lamoral & D.Wartel

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
#ifndef GSUBJECTTREEH
#define GSUBJECTTREEH


//-----------------------------------------------------------------------------
// include files for R-Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>
#include <rstd/rtree.h>
#include <rio/rtextfile.h>
using namespace RIO;
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <docs/gdoc.h>
#include <groups/gsubject.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GSubjectTree class provides a representation for a tree of subject
* handle by the system.
* @author Lamoral Julien
* @short Tree of the Galilei subject.
*/
class GSubjectTree : public RTree<GSubject,true,false>
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
	RContainer<GProfile,unsigned,false,true>* profiles;

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
	* @param nbok        %of ok document.
	* @param nbko        %of ok document.
	* @param nbusers     nb of users in the database.
	*/
	GSubjectTree(int nbOk, int nbKo, int nbusers);

	/**
	* Executes the queries to create new profiles.
	* @param ses      	 The Gailieli session
	*/
	void Judgments(GSession* ses);

	/**
	* Judge documents from the susubject sub.
	* @param profid      id of the profile wich judges the documents
	* @param sub         subsubject containing the docs to judge.
	* @param i           i=1 for OK judgement, i=0 for KO judgment.
	*/
	void JudgeDocuments(int profid,GSubject* sub,int i, GSession* ses);

	/**
	* Write the ideal groupment into a file
	* @param url         The url where the file is saved.
	*/
	void IdealGroupmentFile(char* url);

	/**
	* Creates insert the profiles into subjects
	*/
	void InsertProfiles(void);

	/**
	* Filing in the conatiner of group with a ideal groupment.
	* @param groups      The Ggroups container to fill in.
	* @param ses         The Galieli session.
	*/
	void IdealGroupment(RStd::RContainer<GGroups,unsigned int,true,true>* groups,GSession* ses);

	/**
	* Destructor
	*/
	~GSubjectTree(void){};


};//----------------------------------------------------------------------------
}
#endif


