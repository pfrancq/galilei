/*

	GALILEI Research Project

	GEngineGoogle.h

	Engine for extraction of results from the Yahoo search engine - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
#ifndef __GENGINEXMLH__			// Avoids multiple declarations
#define __GENGINEXMLH__


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gengine.h>
#include <genginedoc.h>
//#include <engines/genginemanager.h>
//#include <gmetaenginemanager.h>
//#include <gmetaengine.h>
//-----------------------------------------------------------------------------
// include files for XML
#include <xquery.h>
#include <gxmlir.h>

//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The GEngineGoogle class provides a representation for a results extractor
* for Google Search Engine
* @author Valery Vandaele
* @short results extractor for Google engine
*/
class GEngineXML : public GEngine
{
	
	/**
	* Name of the file.
	*/
	R::RString Name;
	
	/**
	* Param of Promethee
	*/
	GXmlParams Params;
	/**
	* Param for number of results
	*/
	unsigned int NbResults;

	/**
	* Params for the weight of this engine
	*/
	double Weight;

	/**
	* Tells if database needs to be updated
	*/
	bool ask_update;

	/**
	* Tells if database needs to be updated
	*/
	bool ask_reset;

public:
	/**
	* Construct the extractor for the Google engine.
	* @param fac             Factory.
	* @param owner            The parent of this class.
	*/
	GEngineXML(GFactoryEngine *fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession *session) throw(GException);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession *session) throw(GException);


	/**
	* Process the Engine extraction. All results will be extracted, the url of the next page...
	* @param keyWords        The set of keywords to be searched
	*/
//NORMAL
// 	virtual void Process(R::RContainer<R::RString, false, false> &keyWords) throw(GException);
virtual void Process(R::RContainer<R::RString, false, false>& keyWords) throw(GException);
	/**
	* Get the weight associated to this engine.
	*@return double          The weight of the engine.
	*/
	virtual double GetEngineWeight(void){return Weight;}

private:
	/**
	* Construct the query based on the given keywords
	* @param keyWords        The set of keywords to use to create the query
	* @return RString        The query
	*/
	virtual R::RString ConstructQuery(R::RContainer<R::RString, false, false> &keyWords);

	/**
	* Update database
	*/
	void UpdateDb();

public:
	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams (RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GEngineXML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
