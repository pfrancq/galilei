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


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


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
	* Tag holding the postgroup computation methods.
	*/
	R::RXMLTag* PostGroups;


	/**
	* Tag holding words clustering methods.
	*/
	R::RXMLTag* PostDocs;

	/**
	* Tag holding the languages.
	*/
	R::RXMLTag* Langs;

	/**
	* Document Analysis Methods.
	*/
	R::RXMLTag* DocAnalyses;

	/**
	* Document Analysis Methods.
	*/
	R::RXMLTag* SessionParams;

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
	void Read(GFilterManager& mng);

	/**
	* Store the filters in the structure.
	* @param mng             Manager.
	*/
	void Store(GFilterManager& mng);

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
	* Read the Post Group computing methods from the structure.
	* @param mng             Manager.
	*/
	void Read(GPostGroupManager& mng);

	/**
	* Store the Post Group computing methods in the structure.
	* @param mng             Manager.
	*/
	void Store(GPostGroupManager& mng);


	/**
	* Read the words clustering methods from the structure.
	* @param mng             Manager.
	*/
	void Read(GPostDocManager& mng);

	/**
	* Store the words clustering methods in the structure.
	* @param mng             Manager.
	*/
	void Store(GPostDocManager& mng);

	/**
	* Read the langauges.
	* @param mng             Manager.
	*/
	void Read(GLangManager& mng);

	/**
	* Store the languages.
	* @param mng             Manager.
	*/
	void Store(GLangManager& mng);

	/**
	* Read the DocAnalyse computing methods from the structure.
	* @param mng             Manager.
	*/
	void Read(GDocAnalyseManager& mng);

	/**
	* Store the DocAnalyse computing methods in the structure.
	* @param mng             Manager.
	*/
	void Store(GDocAnalyseManager& mng);

	/**
	* Read the Session params in the structure.
	* @param p             Session parameters.
	*/
	void Read(GSessionParams& p);

	/**
	* Store the Session params in the structure.
	* @param p             Session parameters.
	*/
	void Store(GSessionParams& p);

	/**
	* Destructor.
	*/
	~GConfig(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
