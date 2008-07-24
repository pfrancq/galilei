/*

	GALILEI Research Project

	GPostTopic.h

	Generic Post-Topic Computing Method - Header.

	Copyright 2008 by the Université Libre de Bruxelles.

	Authors:
		Francq Pascal (pfrancq@ulb.ac.be)

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
#ifndef GPostTopicH
#define GPostTopicH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_POSTTOPIC_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GPostTopic class provides a representation for a generic method to
* compute after the clustering of documents.
* @author Pascal Francq
* @short Generic Post-Topic Computing Method.
*/
class GPostTopic : public GPlugin<GFactoryPostTopic>
{
public:

	/**
	* Constructor.
	* @param fac             Factory of the plug-in.
	*/
	GPostTopic(GFactoryPostTopic* fac);

	/**
	* Run the post-topic method.
	*/
	virtual void Run(void)=0;

	/**
	* Destruct.
	*/
	virtual ~GPostTopic(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryPostTopic represent a factory for a given post-topic method.
* @author Pascal Francq
* @short Generic Post-Topic Computing Method Factory.
*/
class GFactoryPostTopic : public GFactoryPlugin<GFactoryPostTopic,GPostTopic,GPostTopicManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plug-in.
	* @param n               Name of the Factory/Plug-in.
	* @param f               Library of the Factory/Plug-in.
	*/
	GFactoryPostTopic(GPostTopicManager* mng,const char* n,const char* f)
	 : GFactoryPlugin<GFactoryPostTopic,GPostTopic,GPostTopicManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GPostTopicManager class provides a representation for a manager
* responsible to manage all the post-topic computing methods.
* @author Pascal Francq
* @short Post-Topic Computing Methods Manager.
*/
class GPostTopicManager : public GPluginManager<GPostTopicManager,GFactoryPostTopic,GPostTopic>
{
public:

	/**
	* Construct the post-topic computing methods manager.
	*/
	GPostTopicManager(void);

	/**
	* Destruct the post-topic computing methods manager.
	*/
	virtual ~GPostTopicManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_POSTTOPIC_FACTORY(name,plugin)\
	CREATE_FACTORY(GALILEI::GPostTopicManager,GALILEI::GFactoryPostTopic,GALILEI::GPostTopic,plugin,"PostTopic",API_POSTTOPIC_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
