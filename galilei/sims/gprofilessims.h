/*

	GALILEI Research Project

	GProfilesSims.h

	Generic Computing of Similarities between profiles - Implementation.

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
#ifndef GProfilesSimsH
#define GProfilesSimsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_PROFILESSIMS_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GProfilesSims class provides a representation for Similarity between
* Profiles.
* @author Pascal Francq and Valery Vandaele
* @short Profiles Similarities.
*/
class GProfilesSims : public GPlugin<GFactoryProfilesSims>
{
public:

	/**
	* Constructor of the similarities between subprofiles.
	* @param fac             Plug-in factory.
	*/
	GProfilesSims(GFactoryProfilesSims* fac);

	/**
	* Get the similarity between two subprofiles.
	* @param sub1            Pointer to the first subprofile.
	* @param sub2            Pointer to the second subprofile.
	*/
	virtual double GetSimilarity(const GSubProfile* sub1,const GSubProfile* sub2)=0;

	/**
	* Get the minimum of similarityof the subprofiles, needed by clusteirng
	* algorithms.
	* @param lang            Language.
	* @param deviationrate   factor of the standart deviation.
	*/
	virtual double GetMinSimilarity(const GLang* lang)=0;

	/**
	* Return the disagreement ratio between two subprofiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	virtual double GetDisagreementRatio(const GSubProfile* sub1,const GSubProfile* sub2)=0;

	/**
	* Return the minimum disagreement ratio to force two subprofiles not to be
	* grouped together.
	* @param lang            Language.
	*/
	virtual double GetMinDisagreementRatio(const GLang* lang)=0;

	/**
	* Return the agreement ratio between two subprofiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	virtual double GetAgreementRatio(const GSubProfile* sub1,const GSubProfile* sub2)=0;

	/**
	* Return the minimum agreement ratio to force two subprofiles to be grouped
	* together.
	* @param lang            Language.
	*/
	virtual double GetMinAgreementRatio(const GLang* lang)=0;

	/**
	* Destructor.
	*/
	virtual ~GProfilesSims(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryProfilesSims represent a factory for a given profiles similarities
* method.
* @author Pascal Francq
* @short Generic Profiles Similarities Factory.
*/
class GFactoryProfilesSims : public GFactoryPlugin<GFactoryProfilesSims,GProfilesSims,GProfilesSimsManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryProfilesSims(GProfilesSimsManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryProfilesSims,GProfilesSims,GProfilesSimsManager>(mng,n,f) {}
};


//-----------------------------------------------------------------------------
/**
* The GProfilesSimsManager class provides a representation for a manager
* responsible to manage all the profiles similarities methods.
* @author Pascal Francq
* @short Profiles Similarities Methods Manager.
*/
class GProfilesSimsManager : public GPluginManager<GProfilesSimsManager,GFactoryProfilesSims,GProfilesSims>
{
public:

	/**
	* Constructor.
	*/
	GProfilesSimsManager(void);

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
	* Get the similarity between two subprofiles using the current method.
	* @param sub1            Pointer to the first subprofile.
	* @param sub2            Pointer to the second subprofile.
	*/
	double GetSimilarity(const GSubProfile* sub1,const GSubProfile* sub2);

	/**
	* Get the minimum of similarity between subprofiles for the current method.
	* @param lang            Language.
	*/
	double GetMinSimilarity(const GLang* lang);

	/**
	* Return the disagreement ratio between two subprofiles  using the current
	* method.
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetDisagreementRatio(const GSubProfile* sub1,const GSubProfile* sub2);

	/**
	* Return the minimum disagreement ratio to force two subprofiles not to be
	* grouped together using the current method.
	* @param lang            Language.
	*/
	double GetMinDisagreementRatio(const GLang* lang);

	/**
	* Return the agreement ratio between two subprofiles using the current
	* method.
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetAgreementRatio(const GSubProfile* sub1,const GSubProfile* sub2);

	/**
	* Return the minimum agreement ratio to force two subprofiles to be grouped
	* together using the current method.
	* @param lang            Language.
	*/
	double GetMinAgreementRatio(const GLang* lang);

	/**
	* Destructor of manager.
	*/
	virtual ~GProfilesSimsManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_PROFILESSIMS_FACTORY(name,plugin)\
	CREATE_FACTORY(GProfilesSimsManager,GFactoryProfilesSims,GProfilesSims,plugin,"ProfilesSims",API_PROFILESSIMS_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
