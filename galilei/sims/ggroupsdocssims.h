/*

	GALILEI Research Project

	GGroupsDocsSims.h

	Generic Computing of Similarities between groups and documents - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GGroupsDocsSimsH
#define GGroupsDocsSimsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>



//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_GROUPSDOCSSIMS_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GGroupsDocsSims class provides a representation for Similarity between
* groups and documents.
* @author Pascal Francq
* @short Groups/Documents Similarities.
*/
class GGroupsDocsSims : public GPlugin<GFactoryGroupsDocsSims>
{
public:

	/**
	* Constructor.
	* @param fac             Plug-in factory.
	*/
	GGroupsDocsSims(GFactoryGroupsDocsSims* fac);

	/**
	* Get the similarity between a group and a document.
	* @param doc             Pointer to the document.
	* @param grp             Pointer to the group.
	*/
	virtual double GetSimilarity(const GDoc* doc,const GGroup* grp)=0;

	/**
	* Get the minimum of similarity between groups and documents.
	* @param lang            Language.
	*/
	virtual double GetMinSimilarity(const GLang* lang)=0;

	/**
	* Destructor.
	*/
	virtual ~GGroupsDocsSims(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryGroupsDocsSims represent a factory for a given
* groups/documents similarities method.
* @author Pascal Francq
* @short Generic Groups/Documents Similarities Factory.
*/
class GFactoryGroupsDocsSims : public GFactoryPlugin<GFactoryGroupsDocsSims,GGroupsDocsSims,GGroupsDocsSimsManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryGroupsDocsSims(GGroupsDocsSimsManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryGroupsDocsSims,GGroupsDocsSims,GGroupsDocsSimsManager>(mng,n,f) {}
};


//-----------------------------------------------------------------------------
/**
* The GGroupsDocsSimsManager class provides a representation for a manager
* responsible to manage all the groups/documents similarities methods.
* @author Pascal Francq
* @short Groups/Documents Similarities Methods Manager.
*/
class GGroupsDocsSimsManager : public GPluginManager<GGroupsDocsSimsManager,GFactoryGroupsDocsSims,GGroupsDocsSims>
{
public:

	/**
	* Constructor.
	*/
	GGroupsDocsSimsManager(void);

	/**
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Get the similarity between a group and a document.
	* @param doc             Pointer to the document.
	* @param grp             Pointer to the group.
	*/
	virtual double GetSimilarity(const GDoc* doc,const GGroup* grp);

	/**
	* Get the minimum of similarityof the subprofiles, needed by clusteirng
	* algorithms using the current method.
	* @param lang            Language.
	*/
	double GetMinSimilarity(const GLang* lang);

	/**
	* Destructor of manager.
	*/
	virtual ~GGroupsDocsSimsManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_GROUPSDOCSSIMS_FACTORY(name,plugin)\
	CREATE_FACTORY(GGroupsDocsSimsManager,GFactoryGroupsDocsSims,GGroupsDocsSims,plugin,"GroupsDocsSims",API_GROUPSDOCSSIMS_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
