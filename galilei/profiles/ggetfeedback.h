/*

	GALILEI Research Project

	ggetfeedback.h

	Auto create feedback for a user.

	Copyright 2002 by the Université Libre de Bruxelles.

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
/**
* This Class implement a representation to the user feedback for GALILEI
* @author Pascal Francq
* @short  auto create a user feedback.
*/
class GGetFeedback
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* The number of documents judged by user feeddback.
	*/
	unsigned int NbDocs;

	/**
	* Global Similarity must be used to compute the relevance of documents.
	*/
	bool Global;

	/**
	* The Percentage of documents judged whith error.
	*/
	unsigned int PercErr;

	/**
	* New documents to judge.
	*/
	R::RContainer<GProfDoc,unsigned,false,false> Docs;

	/**
	* The date of today.
	*/
	char today[12];

public:

	/**
	* the constructor
	* @param session        Galilei session.
	*/
	GGetFeedback(GSession* session);

	/**
	* Create new feedback for the different users of the system.
	* @param Save           Save the results.
	*/
	void Run(bool Save);
	
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
