/*

	GALILEI Research Project

	GConfig.h

	XML Structure representing a configuration - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

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
#ifndef GConfigH
#define GConfigH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rxmlstruct.h>
#include <rstd/rxmltag.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GConfig class provides a representation for a XML structure representing
* all the parameters of the plugins.
* @author Pascal Francq
* @short XML Config Structure.
*/
class GConfig : R::RXMLStruct
{
	/**
	* File containg the document (must be local).
	*/
	R::RString FileName;

	/**
	* Tag holding the filters.
	*/
	R::RXMLTag* Filters;

	/**
	* Tag holding the profile computation methods.
	*/
	R::RXMLTag* ProfileCalcs;

	/**
	* Tag holding the grouping methods.
	*/
	R::RXMLTag* Groupings;

	/**
	* Tag holding the group computation methods.
	*/
	R::RXMLTag* GroupCalcs;

	/**
	* Tag holding the statistics.
	*/
	R::RXMLTag* StatsCalcs;

	/**
	* Tag holding the linking computation methods.
	*/
	R::RXMLTag* LinkCalcs;

	/**
	* Tag holding the languages.
	*/
	R::RXMLTag* Langs;

	/**
	* Document Analysis Methods.
	*/
	R::RXMLTag* DocAnalyses;

public:

	/**
	* Constructor.
	* @param f               Name of the file.
	*/
	GConfig(const char* f) throw(bad_alloc);

	/**
	* Load a file in the structure.
	*/
	void Load(void) throw(GException);

	/**
	* Save a file in the structure.
	*/
	void Save(void) throw(GException);

	/**
	* Read the filters from the structure.
	* @param mng             Manager.
	*/
	void Read(GURLManager& mng);

	/**
	* Store the filters in the structure.
	* @param mng             Manager.
	*/
	void Store(GURLManager& mng);

	/**
	* Read the profiles computing methods from the structure.
	* @param mng             Manager.
	*/
	void Read(GProfileCalcManager& mng);

	/**
	* Store the profiles computing methods in the structure.
	* @param mng             Manager.
	*/
	void Store(GProfileCalcManager& mng);

	/**
	* Read the grouping methods from the structure.
	* @param mng             Manager.
	*/
	void Read(GGroupingManager& mng);

	/**
	* Store the grouping methods in the structure.
	* @param mng             Manager.
	*/
	void Store(GGroupingManager& mng);

	/**
	* Read the groups computing methods from the structure.
	* @param mng             Manager.
	*/
	void Read(GGroupCalcManager& mng);

	/**
	* Store the groups computing methods in the structure.
	* @param mng             Manager.
	*/
	void Store(GGroupCalcManager& mng);

	/**
	* Read the statistics from the structure.
	* @param mng             Manager.
	*/
	void Read(GStatsCalcManager& mng);

	/**
	* Store the statistics in the structure.
	* @param mng             Manager.
	*/
	void Store(GStatsCalcManager& mng);

	/**
	* Read the links computing methods from the structure.
	* @param mng             Manager.
	*/
	void Read(GLinkCalcManager& mng);

	/**
	* Store the links computing methods in the structure.
	* @param mng             Manager.
	*/
	void Store(GLinkCalcManager& mng);

	/**
	* Read the langauges.
	* @param mng             Manager.
	*/
	void Read(GLangs& mng);

	/**
	* Store the languages.
	* @param mng             Manager.
	*/
	void Store(GLangs& mng);

	/**
	* Read the links computing methods from the structure.
	* @param mng             Manager.
	*/
	void Read(GDocAnalyseManager& mng);

	/**
	* Store the links computing methods in the structure.
	* @param mng             Manager.
	*/
	void Store(GDocAnalyseManager& mng);

	/**
	* Destructor.
	*/
	~GConfig(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
