/*

	GALILEI Research Project

	GProfDoc.h

	Profile's Judgement of a document - Header.

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
#ifndef GProfDocH
#define GProfDocH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rtimedate/rdate.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI {
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfDoc class provides a representation for a profile's judgement of a
* given document.
* @author Pascal Francq
* @short Profile's Judgement.
*/
class GProfDoc
{
	/**
	* Document Juged.
	*/
	GDoc* Doc;

	/**
	* Profile.
	*/
	GProfile* Profile;

	/**
	* Feedback of the profile.
	*/
	tDocJudgement Fdbk;

	/**
	* Last Updated of this feedback.
	*/
	RTimeDate::RDate Updated;

public:

	/**
	* Constructor.
	* @param doc            Pointer to the document judged.
	* @param prof           Pointer to the profile.
	* @param fdbk           Feedback.
	* @param date           Date.
	*/
    GProfDoc(GDoc* doc,GProfile* prof,tDocJudgement fdbk,const char* date);

	/**
	* Compare mathod used by RStd::RContainer.
	*/
	int Compare(const GProfDoc &profdoc) const;

	/**
	* Compare mathod used by RStd::RContainer.
	*/
	int Compare(const GProfDoc *profdoc) const;

	/**
	* Compare mathod used by RStd::RContainer for the document part.
	* @param doc            Pointer to the document.
	*/
	int Compare(const GDoc* doc) const;

	/**
	* Get the feedback for the document.
	* @returns Profile's feedback.
	*/
	tDocJudgement GetFdbk(void) const {return(Fdbk);}

	/**
	* Get the document judged.
	* @returns Pointer to the document.
	*/
	GDoc* GetDoc(void) const {return(Doc);}

	/**
	* Get the document judged.
	* @returns Pointer to the document.
	*/
	GProfile* GetProfile(void) const {return(Profile);}

	/**
	* Get the date of the feedback.
	* @returns Pointer to a date.
	*/
	const RTimeDate::RDate* GetUpdated(void) const {return(&Updated);}

	/**
	* Get the similarity between the document and the profile.
	* @return dobule.
	*/
	double Similarity(void);
};


//-----------------------------------------------------------------------------
/**
* The GProfDocCursor class provides a way to go trough a set of feedbacks.
* @short Feedbacks Cursor
*/
CLASSCURSOR(GProfDocCursor,GProfDoc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
