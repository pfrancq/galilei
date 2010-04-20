/*

	GALILEI Research Project

	GPostDoc.h

	Generic Documents Post-Analysis Method - Header.

	Copyright 2003-2004 by Nicolas Kumps.
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
#ifndef GPostDocH
#define GPostDocH


//------------------------------------------------------------------------------
// include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GPostDoc provides a representation for a generic documents post-analysis
* method.
* @author Nicolas Kumps
* @short Generic Document Post Analysis Method.
*/
class GPostDoc : public GPlugIn
{
public :

	/**
	* Constructor of the documents post-analysis method.
	* @param session         Session.
	* @param fac             Factory of the plugin.
	*/
	GPostDoc(GSession* session,GPlugInFactory* fac);

	/**
	* Run the method.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor of the documents post-analysis method.
	*/
	virtual ~GPostDoc(void);
};


//-------------------------------------------------------------------------------
#define CREATE_POSTDOC_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GPostDoc,plugin,"PostDoc",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
