/*

	GALILEI Research Project

	GDocs.h

	List of documents - Implementation.

	(C) 2001 by P. Francq.

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
