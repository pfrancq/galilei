/*

	GALILEI Research Project

	GALILEI.h

	GALILEI Header - Header.

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
#ifndef GALILEIH
#define GALILEIH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>
#include <rstd/rdblhashcontainer.h>
#include <rtimedate/rdate.h>
#include <rxml/rxmltag.h>
#include <rio/rtextfile.h>
#include <rmath/random.h>


//-----------------------------------------------------------------------------
/**
* \namespace GALILEI
* \brief GALILEI Classes.
*
* This namespace declares all the classes implemented during the GALILEI
* project.
*/


//-----------------------------------------------------------------------------
// Defines
#ifndef GALILEITEST
	#define GALILEITEST 1
#endif


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration - Information Part
class GInfo;
class GIWord;
class GIWordList;
class GIWordWeight;
class GIWordWeightCursor;
class GIWordsWeights;


//-----------------------------------------------------------------------------
// forward class declaration - Language Part
class GLang;
class GLangCursor;
class GLangs;
class GWord;
class GDict;
class GDicts;


//-----------------------------------------------------------------------------
// forward class declaration - Document Part
class GDoc;
class GDocVector;
class GDocCursor;
class GDocXML;
class GDocs;
class GDocAnalyse;
class GDocOptions;
class GDocSim;
class GDocsSim;
class GDocsLang;
class GDocProfSim;
class GDocProfSims;

//-----------------------------------------------------------------------------
// forward class declaration - User Part
class GUsers;
class GUser;
class GUserCursor;
class GSubProfileDesc;
class GSubProfileDescCursor;
class GProfile;
class GProfileCursor;
class GSubProfiles;
class GSubProfile;
class GSubProfileCursor;
class GSubProfileVector;
class GProfDoc;
class GProfDocCursor;
class GProfileCalc;
class GProfileCalcCursor;
class GProfilesSim;
class GProfilesSims;
class GSubProfilesSameDocs;


//-----------------------------------------------------------------------------
// forward class declaration - Grouping Part
class GGroups;
class GGroupsCursor;
class GGroup;
class GGroupCursor;
class GGroupVector;
class GGrouping;
class GGroupingKCos;
class GGroupingSupKMeans;
class GGroupingCure;
class GGroupingGGA;
class GGroupingSim;
class GGroupingRandom;
class GGroupingCursor;
class GGroupCalc;
class GGroupCalcCursor;
#if GALILEITEST
	class GSubject;
	class GSubjectCursor;
	class GSubjectTree;
#endif


//-----------------------------------------------------------------------------
// forward class declaration - GA Part
class GIR;
class GInstIR;
class GObjIR;
class GChromoIR;
class GGroupIR;
class GHeuristicIR;


//-----------------------------------------------------------------------------
// forward class declaration - Session Part
class GTest;
class GTests;
class GSession;
class GSessionPrg;
class GPrgInst;
class GPrgVar;
class GSlot;


//-----------------------------------------------------------------------------
// forward class declaration - Interface Part
class GFilter;
class GFilterCursor;
class GMIMEFilter;
class GURLManager;


//-----------------------------------------------------------------------------
// forward class declaration - Test Part
class GSubjectTree;
class GIdealGroup;
class GGetFeedback;
class GGroupEvaluate;
class GGroupEvaluateCursor;
class GGroupsEvaluate;
class GGroupsEvaluateCursor;
class GSubject;
class GStatSimSubProf;
class GStatSimDoc;
class GMixedIdealGroups;


//-----------------------------------------------------------------------------
// forward class declaration - Parameters Part
// Grouping parameters
class GGroupingParams;
class GIRParams;
class GKMeansParams;
class GKProtosParams;
class GCureParams;
class GSupKMeansParams;
class GSimParams;
// Computing parameters
class GCalcParams;
class GFeedbackParams;             
class GReWeightingParams;
class GStatisticalParams;
// Group Description parameters
class GGroupCalcParams;
class GCalcRelevantParams;
class GCalcGravitationParams;


//-----------------------------------------------------------------------------
/**
* The cNoRef constant represented an undefined identificator reference.
*/
const unsigned cNoRef=0xFFFFFFFF;


//-----------------------------------------------------------------------------
/**
* Types of GALILEI objects.
*/
enum tObjType
{
	otNoClass               /** No specific class. */,
	otSession               /** GALILEI Session. */,
	otDoc                   /** Document. */,
	otDocs                  /** Documents. */,
	otUsers                 /** Users.*/,
	otUser                  /** User.*/,
	otProfile               /** Profile.*/,
	otSubProfile            /** SubProfile.*/,
	otGroups                /** Groups.*/,
	otGroup                 /** Group.*/,
	otDocSubProfile         /** Document or SubProfile.*/,
	otDocGroup              /** Document or Group.*/,
	otSubProfileGroup       /** SubProfile or Group.*/
};


//-----------------------------------------------------------------------------
/**
* States of a given GALILEI object.
*/
enum tObjState
{
	osUnknow                /** Unknow state.*/,
	osCreated               /** Object was created.*/,
	osUpToDate              /** Object is up to date.*/,
	osModified              /** Object was modified and computation must be update.*/,
	osUpdated               /** Object is updated and needs to be save.*/,
	osDelete                /** Object must be deleted.*/
};


//-----------------------------------------------------------------------------
/**
* Judgments over a document.
*/
enum tDocJudgement
{
	djUnknow                /** Unknow judgement.*/,
	djOK                    /** Document is OK.*/,
	djKO                    /** Document is KO.*/,
	djNav                   /** Document is an interesting navigation document.*/,
	djOutScope              /** Document is out of scope.*/
};


//-----------------------------------------------------------------------------
/**
* Models used to describe subprofiles.
*/
enum tSubProfileDesc
{
	sdNothing               /** Unknow Model.*/,
	sdVector                /** Vector Model.*/
};


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

public:

	/**
	* Construct an exception.
	* @param str                      Message of the error.
	*/
	GException(const RStd::RString& str) throw(bad_alloc)
		: Msg(str) {}

	/**
	* Get the content of the exception.
	* @returns Pointer to a C String.
	*/
	const char* GetMsg(void) const {return(Msg());}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
