/*

	GALILEI Research Project

	GIdealGroup.h

	Get the jugments for the profiles

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
*  @author Julien Lamoral
*  @short  Ideal Groupment
*/
class GIdealGroup
{
protected:

	 /**
	* The differents Subjects of the ideal groupment.
	*/
	GSubjectTree* Subjects;

	/**
	* The Users of the galilei system.
	*/
	GSession* Session;
	
	/**
	* The percentage of ok documents.
	*/
	unsigned int PercOK;

	/**
	* The percentage of ko documents.
	*/
	unsigned int PercKO;

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
	unsigned int PercSocial;

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
	void CreateJudgement(RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* &parent,RStd::RContainer<GGroups,unsigned int,true,true>* &groups,bool Save);

	/**
	* Write the ideal groupment into a file
	* @param url            The url where the file is saved.
	*/
	void CreateIdealGroupmentFile(const char* url);
	
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
