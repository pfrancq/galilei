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
#include <rtimedate/rdate.h>


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
class GSubProfiles;
class GSubProfile;
class GSubProfileCursor;
class GSubProfileDesc;
class GSubProfileDescCursor;
class GGroup;
class GGroupCursor;
class GGroups;
class GGroupsCursor;
class GGrouping;
class GGroupingCursor;
class GTest;
class GTests;
class GSession;
class GSlot;
class GFilter;
class GFilterCursor;
class GMIMEFilter;
class GURLManager;


//-----------------------------------------------------------------------------
/**
* The cNoRef constant represented an undefined identificator reference.
*/
const unsigned cNoRef=0xFFFFFFFF;


//-----------------------------------------------------------------------------
/**
* Types of GALILEI objects.
*/
enum tObjType{
	otNoClass               /** No specific class. */,
	otSession               /** GALILEI Session. */,
	otDoc                   /** Document. */,
	otDocs                  /** Documents. */,
	otUsers                 /** Users.*/,
	otUser                  /** User.*/,
	otProfile               /** Profile.*/,
	otSubProfile            /** SubProfile.*/,
	otGroups                /** Groups.*/,
	otGroup                 /** Group.*/};


//-----------------------------------------------------------------------------
/**
* States of a given GALILEI object.
*/
enum tObjState{
	osUnknow                /** Unknow state.*/,
	osCreated               /** Object was created.*/,
	osUpToDate              /** Object is up to date.*/,
	osModified              /** Object was modified and computation must be update.*/,
	osUpdated               /** Object is updated and needs to be save.*/,
	osDelete                /** Object must be deleted.*/};


//-----------------------------------------------------------------------------
/**
* Judgments over a document.
*/
enum tDocJudgement{
	djUnknow                /** Unknow judgement.*/,
	djOK                    /** Document is OK.*/,
	djKO                    /** Document is KO.*/,
	djNav                   /** Document is an interesting navigation document.*/,
	djOutScope              /** Document is out of scope.*/};


//-----------------------------------------------------------------------------
/**
* Models used to describe subprofiles.
*/
enum tSubProfileDesc{
	sdNothing               /** Unknow Model.*/,
	sdVector                /** Vector Model.*/};


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
