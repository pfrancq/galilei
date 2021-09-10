/*

	GALILEI Research Project

	GGroupDocs.h

	Generic Documents Grouping Method - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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



//------------------------------------------------------------------------------
#ifndef GGroupDocsH
#define GGroupDocsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GGroupDocs provides a representation for a generic method to group some
* documents.
*
* See the documentation related to GPlugIn for more general information.
* @short Generic Documents Grouping Method.
*/
class GGroupDocs : public GPlugIn
{
protected:

	/**
	* Documents that must be grouped.
	*/
	R::RContainer<GDoc,false,false> Docs;

public:

	/**
	* Constructor of the documents grouping method.
	* @param session         Session.
	* @param fac             Factory of the plug-in.
	*/
	GGroupDocs(GSession* session,GPlugInFactory* fac);

protected:

	/**
	* Make the grouping for the defined documents.
	*/
	virtual void Run(void)=0;

public:

	/**
	* Group the Documents.
	* @param rec            Receiver of the signals.
	* @param debug          Debug mode (cluster only the selected documents).
	*/
	void Grouping(GSlot* rec,bool debug);

	/**
	* Destruct the documents grouping method.
	*/
	virtual ~GGroupDocs(void);
};


//-------------------------------------------------------------------------------
#define CREATE_GROUPDOCS_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GGroupDocs,plugin,"GroupDocs",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
