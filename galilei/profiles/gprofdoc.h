/*

	GALILEI Research Project

	GProfDoc.h

	Profile's Judgement of a document - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GProfDocH
#define GProfDocH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rtimedate/rdate.h>



//-----------------------------------------------------------------------------
namespace GALILEI {
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GDoc;
class GProfile;


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
	char Fdbk;

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
    GProfDoc(GDoc* doc,GProfile* prof,char fdbk,const char* date);

	/**
	* Compare mathod used by RStd::RContainer.
	*/
	int Compare(const GProfDoc &profdoc) const;

	/**
	* Compare mathod used by RStd::RContainer.
	*/
	int Compare(const GProfDoc *profdoc) const;

	/**
	* Get the feedback for the document.
	* @returns Profile's feedback.
	*/
	char GetFdbk(void) const {return(Fdbk);}

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
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
