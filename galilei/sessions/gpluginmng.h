/*

	GALILEI Research Project

	GPluginMng.h

	Plugin Manager for GALILEI - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
#ifndef GPluginMngH
#define GPluginMngH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GPluginMng provides a representation for a manager of plugins. The path
* given is used to search for all the plugins.
* @author Pascal Francq.
* @short Plugins Manager.
*/
class GPluginMng
{
protected:
	/**
	* Container of computing method for the profiles.
	*/
	R::RContainer<GProfileCalc,unsigned int,true,true>* ProfileCalcs;

	/**
	* Current method used to computed the profiles.
	*/
	GProfileCalc* ProfileCalc;

	/**
	* Container of grouping method for the profiles.
	*/
	R::RContainer<GGrouping,R::tId,true,true>* Groupings;

	/**
	* Current grouping method used.
	*/
	GGrouping* Grouping;

	/**
	* Container of group description method.
	*/
	R::RContainer<GGroupCalc,R::tId,true,true>* GroupCalcs;

	/**
	* Current group description method used.
	*/
	GGroupCalc* GroupCalc;

	/**
	* Container of link description method.
	*/
	R::RContainer<GLinkCalc,unsigned int,true,true>* LinkCalcs;

	/**
	* Current link description method used.
	*/
	GLinkCalc* LinkCalc;

	/**
	* URL Manager used by this session.
	*/
	GURLManager* Mng;

	/**
	* Analyser used for the document.
	*/
	GDocAnalyse* DocAnalyse;

public:

	/**
	* Constructor.
	* @param path            Path for the plugin.
	*/
	GPluginMng(const char* path) throw(bad_alloc,GException);

	/**
	* Get the documents' analyser.
	* @returns Pointer to a GDocAnalyse class.
	*/
	GDocAnalyse* GetDocAnalyse(void) const {return(DocAnalyse);}

	/**
	* Set the current computing method.
	* @param name           Name of the computing method.
	*/
	void SetCurrentComputingMethod(const char* name) throw(GException);

	/**
	* Set the settings to the current computing method.
	* @param s              Settings of the current computing method.
	*/
	void SetCurrentComputingMethodSettings(const char* s) throw(GException);

	/**
	* Get the current computing method.
	* @returns Pointer to a GProfileCalc class.
	*/
	GProfileCalc* GetCurrentComputingMethod(void) {return(ProfileCalc);}

	/**
	* Get a cursor to the computing methods registered.
	* @return Return a GComputingCursor.
	*/
	GProfileCalcCursor& GetComputingsCursor(void);

	/**
	* Set the current grouping method.
	* @param name           Name of the grouping method.
	*/
	void SetCurrentGroupingMethod(const char* name) throw(GException);

	/**
	* Get the current grouping method.
	* @returns Pointer to a GGrouping class.
	*/
	GGrouping* GetCurrentGroupingMethod(void) {return(Grouping);}

	/**
	* Get a cursor to the grouping methods registered.
	* @return Return a GGoupingCursor.
	*/
	GGroupingCursor& GetGroupingsCursor(void);

	/**
	* Set the current group description method.
	* @param name           Name of the group description method.
	*/
	void SetCurrentGroupCalcMethod(const char* name) throw(GException);

	/**
	* Get the current group description method.
	* @returns Pointer to a GGroupCalc class.
	*/
	GGroupCalc* GetCurrentGroupCalcMethod(void) {return(GroupCalc);}

	/**
	* Get a cursor to the group description methods registered.
	* @return Return a GGroupCalcCursor.
	*/
	GGroupCalcCursor& GetGroupCalcsCursor(void);

	/**
	* Set the current link description method.
	* @param name           Name of the link description method.
	*/
	void SetCurrentLinkCalcMethod(const char* name) throw(GException);

	/**
	* Get the current link description method.
	* @returns Pointer to a GLinkCalc class.
	*/
	GLinkCalc* GetCurrentLinkCalcMethod(void) {return(LinkCalc);}

	/**
	* Get a cursor to the link description methods registered.
	* @return Return a GLinkCalcCursor.
	*/
	GLinkCalcCursor& GetLinkCalcsCursor(void);

	/**
	* Destructor.
	*/
	virtual ~GPluginMng(void) throw(GException);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
