/*

	GALILEI Research Project

	GDocs.h

	List of documents - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GDocsH
#define GDocsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GSession;
class GDoc;


//-----------------------------------------------------------------------------
/**
* The GDocs class provides a representation for a list of documents.
* @author Pascal Francq
* @short List of documents.
*/
class GDocs : public RStd::RContainer<GDoc,unsigned,true,true>
{
	/**
	* Sesssion.
	*/
	GSession* Session;

public:

	/**
	* Constructor.
	* @param nb             Maximal number of documents.
	* @param session        Session.
	*/
	GDocs(unsigned int nb,GSession *session) throw(bad_alloc);

	/**
	* Destructor.
	*/
	virtual ~GDocs(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
