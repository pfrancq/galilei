/*

	GALILEI Research Project

	GIdealGroup.h

	Get the jugments for the profiles

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
#ifndef GIdealGroupH
#define GIdealGroupH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <profiles/gusers.h>
#include <groups/gsubject.h>
#include <groups/gsubjecttree.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class GSubProfile;


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
	GSubjectTree* subjects;

	/**
	* The Users of the galilei system.
	*/
	GUsers* users;

public:

	/**
	* Constructor Get GIdealGroup Function.
	* Create the profiles,subprofiles and the judgment randomly from a file
	* where is stoked the grouping of document.
	* @param txturl           The url of the file.
	* @param user             The usercontainer.
	*/
	GIdealGroup(const char* txturl,GUsers* user);

	/**
	* Create randomly the judgment for all the users.
	* Return the ideal groupment into a GGroups container.
	* @param ses		The Galilei Session.
	*/
	RStd::RContainer<GGroups,unsigned int,true,true>* CreateJudgement(GSession* ses);

	/**
	* Write the ideal groupment into a file
	* @param url            The url where the file is saved.
	*/
	void CreateIdealGroupmentFile(char * url);

};


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
