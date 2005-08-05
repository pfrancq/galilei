/*

	GALILEI Research Project

	GProfilesGroupsSims.h

	Generic Computing of Similarities between groups and profiles - Implementation.

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
#ifndef GProfilesGroupsSimsH
#define GProfilesGroupsSimsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>



//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// API VERSION
#define API_PROFILESGROUPSSIMS_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GProfilesGroupsSims class provides a representation for Similarity between
* groups and profiles.
* @author Pascal Francq
* @short Groups/Profiles Similarities.
*/
class GProfilesGroupsSims : public GPlugin<GFactoryProfilesGroupsSims>
{
public:

	/**
	* Constructor.
	* @param fac             Plug-in factory.
	*/
	GProfilesGroupsSims(GFactoryProfilesGroupsSims* fac);

	/**
	* Get the similarity between a group and a subprofile.
	* @param sub             Pointer to the subprofile.
	* @param grp             Pointer to the group.
	*/
	virtual double GetSimilarity(const GSubProfile* sub,const GGroup* grp)=0;

	/**
	* Get the minimum of similarity between groups and profiles.
	* @param lang            Language.
	*/
	virtual double GetMinSimilarity(const GLang* lang)=0;

	/**
	* Destructor.
	*/
	virtual ~GProfilesGroupsSims(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryProfilesGroupsSims represent a factory for a given
* groups/profiles similarities method.
* @author Pascal Francq
* @short Generic Groups/Profiles Similarities Factory.
*/
class GFactoryProfilesGroupsSims : public GFactoryPlugin<GFactoryProfilesGroupsSims,GProfilesGroupsSims,GProfilesGroupsSimsManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryProfilesGroupsSims(GProfilesGroupsSimsManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryProfilesGroupsSims,GProfilesGroupsSims,GProfilesGroupsSimsManager>(mng,n,f) {}
};


//-----------------------------------------------------------------------------
/**
* The GProfilesGroupsSimsManager class provides a representation for a manager
* responsible to manage all the groups/profiles similarities methods.
* @author Pascal Francq
* @short Groups/Profiles Similarities Methods Manager.
*/
class GProfilesGroupsSimsManager : public GPluginManager<GProfilesGroupsSimsManager,GFactoryProfilesGroupsSims,GProfilesGroupsSims>
{
public:

	/**
	* Constructor.
	*/
	GProfilesGroupsSimsManager(void);

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
	* Get the similarity between a group and a subprofile.
	* @param sub             Pointer to the subprofile.
	* @param grp             Pointer to the group.
	*/
	virtual double GetSimilarity(const GSubProfile* sub,const GGroup* grp);

	/**
	* Get the minimum of similarity between groups and subprofiles for the
	* current method.
	* @param lang            Language.
	*/
	double GetMinSimilarity(const GLang* lang);

	/**
	* Destructor of manager.
	*/
	virtual ~GProfilesGroupsSimsManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_PROFILESGROUPSSIMS_FACTORY(name,plugin)\
	CREATE_FACTORY(GProfilesGroupsSimsManager,GFactoryProfilesGroupsSims,GProfilesGroupsSims,plugin,"ProfilesGroupsSims",API_PROFILESGROUPSSIMS_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
