/*

	GALILEI Research Project

	GEngine.h

	Generic Engine for extraction of results from the a search engine - Header.

	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GEngineH
#define GEngineH


//------------------------------------------------------------------------------
// include files for CUL library
#include <curl/curl.h>


//------------------------------------------------------------------------------
//include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <genginedoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GEngine class provides a representation for a results extractor
* for Search Engine
* @author Valery Vandaele
* @short Results extractor for search engine
*/
class GEngine : public GPlugIn
{
protected:
	/**
	* Pointer to an instance of the CURL library.
	*/
	CURL* Lib;

	/**
	* The weight associated to the engine
	*/
	float Weight;

public:

	/**
	* Construct an engine plug-in.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GEngine(GSession* session,GPlugInFactory* fac);

	/**
	* Process the Engine extraction. All results will be extracted, the URL of
	* the next page.
	* @param keyWords        The set of keywords to be searched
	*/
	virtual void Process(R::RContainer<R::RString,false,false>& keyWords)=0;

	/**
	* Get the weight associated to this engine
	*@return double         The weight.
	*/
	virtual double GetEngineWeight(void)=0;

	/**
	* Get the weight associated to the engine
	* @return float          The weight associated to the engine
	*/
	float GetEngineWeight(void) const {return Weight;};

	/**
	* Associate a new weight to the engine
	* @param w               The weight to associate to the engine
	*/
	void SetEngineWeight(float w) {Weight=w;};

	/**
	* Destructor of the search engine.
	*/
	virtual ~GEngine(void);
};


//-------------------------------------------------------------------------------
#define CREATE_ENGINE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GEngine,plugin,"Engine",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
