/*

	GALILEI Research Project

	GALILEI.h

	GALILEI Header - Header.

	(C) 2001-2002 by P. Francq.

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
#ifndef GALILEIH
#define GALILEIH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>


//-----------------------------------------------------------------------------
/**
* \namespace GALILEI
* \brief GALILEI Classes.
*
* This namespace declares all the classes implemented during the GALILEI
* project.
*/


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GIWordWeight;
class GIWordWeightCursor;
class GIWordsWeights;
class GLang;
class GLangCursor;
class GLangs;
class GDict;
class GDicts;
class GDoc;
class GDocCursor;
class GDocXML;
class GDocs;
class GDocAnalyse;
class GDocOptions;
class GUser;
class GUserCursor;
class GUsers;
class GProfile;
class GProfileCursor;
class GProfilesSim;
class GProfDoc;
class GProfDocCursor;
class GProfileCalc;
class GProfileCalcCursor;
class GSubProfile;
class GSubProfileCursor;
class GSubProfileDesc;
class GSubProfileDescCursor;
class GGroup;
class GGroupCursor;
class GGroupsLang;
class GGroupsLangCursor;
class GGroups;
class GGroupsCursor;
class GGrouping;
class GGroupingCursor;
class GTest;
class GTests;
class GSession;
class GSlot;
class GFilter;
class GMIMEFilter;
class GURLManager;


//-----------------------------------------------------------------------------
// General constant
const unsigned cNoRef=0xFFFFFFFF;        // A undefined identificator reference


//-----------------------------------------------------------------------------
/**
* Represent different state of a GALILEI object.
*/
enum tObjState{osUnknow   /** Unknow state.*/,
               osCreated  /** Object was created.*/,
               osUpToDate /** Object is up to date.*/,
               osModified /** Object was modified and computation must be update.*/,
               osUpdated  /** Object is updated and needs to be save.*/};


//-----------------------------------------------------------------------------
/**
* Represent different methods to describe subprofiles.
*/
enum tSubProfileDesc{sdNothing=0,sdVector=1};


//-----------------------------------------------------------------------------
/**
* Represent different methods to compute the subpeofiles for the vector space.
*/
enum tVectorMethod{vmNothing=0,vmQueryExpansion=1,vmDirectReweighting=2};


//-----------------------------------------------------------------------------
/**
* Represent different methods to group the profiles.
*/
enum tProfilesGroup{pgNothing=0,pgHeuristicSim=1,pgGA=2};


//-----------------------------------------------------------------------------
/**
* The GException class provides a basic representation for an exception.
* @author Pascal Francq
* @short Basic Exception.
*/
class GException
{
	/**
	* Message holding some information.
	*/
	RStd::RString Msg;

	/**
	* A code representing the exception.
	*/
	unsigned int Code;

public:

	/**
	* Predefined Constants for Errors.
	*/
	enum eErrorType{NoDefinedError=0,OpenSessionError=1,Languages=2,Dic=3,Docs=4};

	/**
	* Construct an exception.
	* @param str                      Message of the error.
	*/
	GException(const RStd::RString& str) throw(bad_alloc)
		: Msg(str), Code(NoDefinedError) {}

	/**
	* Construct an exception.
	* @param code                     Code of the error.
	* @param str                      Message of the error.
	*/
	GException(const unsigned code,const RStd::RString& str) throw(bad_alloc)
		: Msg(str), Code(code) {}

	/**
	* Get the content of the exception.
	* @returns Pointer to a C String.
	*/
	const char* GetMsg(void) const {return(Msg());}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
