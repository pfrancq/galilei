/*

	GALILEI Research Project

	ggetfeedback.h

	Auto create feedback for a user.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
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
#ifndef GGetFeedbackH
#define GGetFeedbackH



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <profiles/gusers.h>
#include <groups/gsubject.h>
#include <tests/ggroupsevaluate.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward class declaration
class GSubProfile;


//-----------------------------------------------------------------------------
/**  This Class implement a representation to the user feedback for GALILEI
*  @author Julien Lamoral
*  @short  auto create a user feedback.
*/
class GGetFeedback
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* The number of documents judget by user feeddback.
	*/
	unsigned int NbDoc;

	/**
	* The number of documents judget by user feeddback.
	*/
	bool Global;

	/**
	* The Percentage of documents judged whith error.
	*/
	unsigned int PercErr;

	/**
	* IdealGroup              the idealgroupment.
	*/
	RStd::RContainer<GGroups,unsigned int,true,true>* IdealGroup;

	/**
	* Parent                  the id of the parent of the different subsubject.
	*/
	RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* Parent;

	/**
	* IdealDoc                the idealgroupment of document.
	*/
	RContainer<GGroupsEvaluate,unsigned int,false,false>* IdealDoc;

	/**
	* Return 1 if the profiles judge the doc OK.
	* Return 2 if the profiles judge the doc KO.
	* Return 3 if the profiles judge the doc HS.
	* If PercErr is not null the judgment can be different from the ideal judgment.
	* @param Subprof          the subprofiles who judg the document.
	* @param Doc              the Document who will be judged.
	*/
	int JudgType(GSubProfile* SubProf,GDoc* Doc);

	/**
	* Create a feedback for the user whith profil id prof1 whith the documents doc.
	* @param fdbk             1 if the feedback is ok,2 for ko and 3 for hs.
	* @param sub              the subprofile.
	* @param doc              the doc who will be judged
	*/
	void CreateNewFeedback(int fdbk,GSubProfile* sub,GDoc* doc);

public:

	/**
	* the constructor
	* @param session        Galilei session.
	*/
	GGetFeedback(GSession* session);

	/**
	* Create new feedback for the different users of the system.
	* @param parent         the id of the parent of a subsubject.
	* @param idealgroup     the idealgroupment.
	* @param Save           Save the results.
	*/
	void Run(RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent,RStd::RContainer<GGroups,unsigned int,true,true>* idealgroup,bool Save);
	
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
	~GGetFeedback(void);

};


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif


