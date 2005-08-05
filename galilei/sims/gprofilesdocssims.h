/*

	GALILEI Research Project

	GProfilesDocsSims.h

	Generic Computing of Similarities between profiles and documents - Implementation.

	Copyright 2003-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

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
#ifndef GProfilesDocsSimsH
#define GProfilesDocsSimsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>



//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_PROFILESDOCSSIMS_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GProfilesDocsSims class provides a representation for Similarity between
* profiles and documents.
* @author Pascal Francq and Valery Vandaele
* @short Profiles/Documents Similarities.
*/
class GProfilesDocsSims : public GPlugin<GFactoryProfilesDocsSims>
{
public:

	/**
	* Constructor.
	* @param fac             Plug-in factory.
	*/
	GProfilesDocsSims(GFactoryProfilesDocsSims* fac);

	/**
	* Get the similarity between a subprofile and a document.
	* @param doc             Pointer to the document.
	* @param sub             Pointer to the subprofile.
	*/
	virtual double GetSimilarity(const GDoc* doc,const GSubProfile* sub)=0;

	/**
	* Get the minimum of similarity between subprofiles and documents.
	* @param lang            Language.
	*/
	virtual double GetMinSimilarity(const GLang* lang)=0;

	/**
	* Destructor.
	*/
	virtual ~GProfilesDocsSims(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryProfilesDocsSims represent a factory for a given
* profiles/documents similarities method.
* @author Pascal Francq
* @short Generic Profiles/Documents Similarities Factory.
*/
class GFactoryProfilesDocsSims : public GFactoryPlugin<GFactoryProfilesDocsSims,GProfilesDocsSims,GProfilesDocsSimsManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryProfilesDocsSims(GProfilesDocsSimsManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryProfilesDocsSims,GProfilesDocsSims,GProfilesDocsSimsManager>(mng,n,f) {}
};


//-----------------------------------------------------------------------------
/**
* The GProfilesDocsSimsManager class provides a representation for a manager
* responsible to manage all the profiles/documents similarities methods.
* @author Pascal Francq
* @short Profiles/Documents Similarities Methods Manager.
*/
class GProfilesDocsSimsManager : public GPluginManager<GProfilesDocsSimsManager,GFactoryProfilesDocsSims,GProfilesDocsSims>
{
public:

	/**
	* Constructor.
	*/
	GProfilesDocsSimsManager(void);

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
	* Get the similarity between a subprofiles and a document.
	* @param doc             Pointer to the document.
	* @param sub             Pointer to the subprofile.
	*/
	double GetSimilarity(const GDoc* doc,const GSubProfile* sub);

	/**
	* Get the minimum of similarity between documents and subprofiles for the
	* current method.
	* @param lang            Language.
	*/
	double GetMinSimilarity(const GLang* lang);

	/**
	* Destructor of manager.
	*/
	virtual ~GProfilesDocsSimsManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_PROFILESDOCSSIMS_FACTORY(name,plugin)\
	CREATE_FACTORY(GProfilesDocsSimsManager,GFactoryProfilesDocsSims,GProfilesDocsSims,plugin,"ProfilesDocsSims",API_PROFILESDOCSSIMS_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
