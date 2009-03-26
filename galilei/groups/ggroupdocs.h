/*

	GALILEI Research Project

	GGroupDocs.h

	Generic Documents Grouping Method - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
// API VERSION
#define API_GROUPDOCS_VERSION "2"


//------------------------------------------------------------------------------
/**
* The GGroupDocs provides a representation for a generic method to group some
* documents.
* @author Pascal Francq
* @short Generic Documents Grouping Method.
*/
class GGroupDocs : public GPlugin<GFactoryGroupDocs>
{
protected:

	/**
	* Documents that must be grouped.
	*/
	R::RContainer<GDoc,false,false> Docs;

public:

	/**
	* Constructor of the documents grouping method.
	* @param fac             Factory of the plug-in.
	*/
	GGroupDocs(GFactoryGroupDocs* fac);

protected:

	/**
	* Make the grouping for the defined documents.
	*/
	virtual void Run(void)=0;

public:

	/**
	* Group the Documents.
	* @param rec            Receiver of the signals.
	* @param save           Save modified elements.
	* @param debug          Dubug mode (cluster only the selected documents).
	*/
	void Grouping(GSlot* rec,bool save,bool debug);

	/**
	* Destruct the documents grouping method.
	*/
	virtual ~GGroupDocs(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryGroupDocs represent a factory for a given documents grouping
* method.
* @author Pascal Francq
* @short Generic Documents Grouping Factory.
*/
class GFactoryGroupDocs : public GFactoryPlugin<GFactoryGroupDocs,GGroupDocs,GGroupDocsManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plug-in.
	* @param n               Name of the Factory/Plug-in.
	* @param f               Library of the Factory/Plug-in.
	*/
	GFactoryGroupDocs(GGroupDocsManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryGroupDocs,GGroupDocs,GGroupDocsManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GGroupDocsManager class provides a representation for a manager
* responsible to manage all the documents grouping methods.
* @author Pascal Francq
* @short Documents Grouping Methods Manager.
*/
class GGroupDocsManager : public GPluginManager<GGroupDocsManager,GFactoryGroupDocs,GGroupDocs>
{
public:

	/**
	* Construct the documents grouping methods manager.
	*/
	GGroupDocsManager(void);

	/**
	* Destruct the documents grouping methods manager.
	*/
	virtual ~GGroupDocsManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_GROUPDOCS_FACTORY(name,plugin)\
	CREATE_FACTORY(GGroupDocsManager,GFactoryGroupDocs,GGroupDocs,plugin,"GroupDocs",API_GROUPDOCS_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
