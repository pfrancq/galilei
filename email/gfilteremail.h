/*

	GALILEI Research Project

	GDocXML.h

	A XML representation of a document - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

*/



//-----------------------------------------------------------------------------
#ifndef GFilterEMailH
#define GFilterEMailH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gfilter.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilterEMail class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
*/
class GFilterEMail : public GFilter
{
public:

	/**
	* Construct the email filter for a specific email.
	* @param url            URL of the document.
	*/
	GFilterEMail(const RString& url);

	/**
	* Analyze an email and fill the XML structure with the information content.
	* @param doc            XML Structure that will represent the email.
	*/
	virtual bool Analyze(GDocXML* doc);

	/**
	* Destructor of the email filter.
	*/
	virtual ~GFilterEMail(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
