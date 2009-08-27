/*

	GALILEI Research Project

	GComputeTrust.h

	Generic Trust Computing Method - Header.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GComputeTrustH
#define GComputeTrustH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_COMPUTETRUST_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GComputeTrust class provides a representation for a generic method for
* trust computing.
* @author Pascal Francq
* @short Generic Trust Computing Method.
*/
class GComputeTrust : public GPlugin<GFactoryComputeTrust>
{
public:

	/**
	* Constructor.
	* @param fac             Factory of the plug-in.
	*/
	GComputeTrust(GFactoryComputeTrust* fac);

	/**
	* Run the trust computing method.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor.
	*/
	virtual ~GComputeTrust(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryComputeTrust represent a factory for a given trust computing
* method.
* @author Pascal Francq
* @short Generic Trust Computing Factory.
*/
class GFactoryComputeTrust : public GFactoryPlugin<GFactoryComputeTrust,GComputeTrust,GComputeTrustManager>
{
public:

	/**
	* Constructor.
	* @param mng             Manager of the plug-in.
	* @param n               Name of the Factory/Plug-in.
	* @param f               Library of the Factory/Plug-in.
	*/
	GFactoryComputeTrust(GComputeTrustManager* mng,const char* n,const char* f)
	 : GFactoryPlugin<GFactoryComputeTrust,GComputeTrust,GComputeTrustManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GComputeTrustManager class provides a representation for a manager
* responsible to manage all the trust computing methods.
* @author Pascal Francq
* @short Trust Computing Methods Manager.
*/
class GComputeTrustManager : public GPluginManager<GComputeTrustManager,GFactoryComputeTrust,GComputeTrust>
{
public:

	/**
	* Construct the trust computing methods manager.
	*/
	GComputeTrustManager(void);

	/**
	* Destruct the trust computing methods manager.
	*/
	virtual ~GComputeTrustManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_COMPUTETRUST_FACTORY(name,plugin)\
	CREATE_FACTORY(GALILEI::GComputeTrustManager,GALILEI::GFactoryComputeTrust,GALILEI::GComputeTrust,plugin,"ComputeTrust",API_COMPUTETRUST_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
