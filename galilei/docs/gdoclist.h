/*

	GALILEI Research Project

	GDocList.h

	List of Documents - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GDocListH
#define GDocListH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GDocList class provides a representation for a list of documents. This
* list is not responsible for the desallocation of the document, it can only be
* used to hold a sub set of documents.
* @author Pascal Francq
* @short Documents' List.
*/
class GDocList : public RStd::RContainer<GDoc,unsigned int,false,true>
{
public:

	/**
	* Construct the list of documents.
	* @param nb             Maximal number of documents.
	*/
	GDocList(const unsigned int nb) throw(bad_alloc);

	/**
	* Destruct the documents' list.
	*/
	virtual ~GDocList(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
