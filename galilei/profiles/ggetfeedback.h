/*

	GALILEI Research Project

	ggetfeedback.h

	Auto create feedback for a user.

	(C) 2002 by Julien Lamoral

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
	* The number of documents judget by user feeddback.
	*/
	unsigned int NbDoc;

	/**
	* The number of documents judget by user feeddback.
	*/
	bool Global;

	/**
	* IdealGroup              the idealgroupment.
	*/
	RStd::RContainer<GGroups,unsigned int,true,true>* IdealGroup;

	/**
	* Parent                  the id of the parent of the different subsubject.
	*/
	RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* Parent;

	/**
	* Return 1 if the profiles are in the same group.
	* Return 2 if the profiles are in different group but whith the same parent group.
	* Return 3 if the profiles are in different group.
	* @param prof1            the id of the first profile.
	* @param prof2            the id of the second profile.
	*/
	int AreInSameGroup(int prof1,int prof2);

	/**
	* Create a feedback for the user whith profil id prof1 whith the documents doc.
	* @param fdbk             1 if the feedback is ok,2 for ko and 3 for hs.
	* @param sub              the subprofile.
	* @param ses              the galilei session.
	* @param doc              the doc who will be judged
	*/
	void CreateNewFeedback(int fdbk,GSubProfile* sub,GSession* ses,GDoc* doc);

public:

	/**
	* the constructor
	* @param ses              the galilei session.
	* @param idealgroup       the idealgroupment.
	* @param parent           the id of the parent of a subsubject.
	*/
	GGetFeedback(RStd::RContainer<GGroups,unsigned int,true,true>* idealgroup,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent);

	/**
	* Create new feedback for the different users of the system.
	*/
	void Run(GSession* ses);
	
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

};


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif


