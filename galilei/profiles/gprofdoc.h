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
//include file for GALILEI
#include <gdocs/gdoc.h>



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
	* Feedback of the profile.
	*/
	char Fdbk;

public:

	/**
	* Constructor.
	* @param doc            Pointer to the document judged.
	* @param fdbk           Feedback.
	*/
    GProfDoc(GDoc *doc,char fdbk);

	/**
	* Compare mathod used by RStd::RContainer.
	*/
	int Compare(const GProfDoc &profdoc);

	/**
	* Compare mathod used by RStd::RContainer.
	*/
	int Compare(const GProfDoc *profdoc);

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
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
