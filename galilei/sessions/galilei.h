/*

	GALILEI Research Project

	GALILEI.h

	GALILEI Header - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GALILEIH
#define GALILEIH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rdate.h>
#include <rstd/rlang.h>
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>
#include <rstd/rxmltag.h>
#include <rstd/rtextfile.h>
#include <rstd/random.h>


//------------------------------------------------------------------------------
/**
* \namespace GALILEI
* \brief GALILEI Classes.
*
* This namespace declares all the classes implemented during the GALILEI
* project.
*/


//------------------------------------------------------------------------------
// Defines
#ifndef GALILEITEST
	#define GALILEITEST 1
#endif
#define proxy public


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration - Infos Part
class GInfo;
class GWeightInfo;
class GWeightInfoCursor;
class GWeightInfos;
class GInfosWeightsHistory;
class GLang;
class GLangProxy;
class GFactoryLang;
class GFactoryLangCursor;
class GLangManager;
class GData;
class GDataCursor;
class GWord;
class GWordCursor;
class GWordList;
class GDict;
class GDicts;


//------------------------------------------------------------------------------
// forward class declaration - Docs Part
class GFilter;
class GFactoryFilter;
class GFactoryFilterCursor;
class GFilterManager;
class GDoc;
class GDocProxy;
class GDocAnalyse;
class GDocAnalyseManager;
class GDocCursor;
class GPostDoc;
class GPostDocManager;
class GFactoryPostDoc;
class GFactoryPostDocCursor;
class GDocProfSims;
class GDocs;
class GDocSim;
class GDocsSim;
class GDocXML;
class GFactoryDocAnalyse;
class GFactoryDocAnalyseCursor;
class GBalancedLinks;
class GBalancedLinksCursor;
class GFactoryLinkCalc;
class GFactoryLinkCalcCursor;
class GLink;
class GLinkCalc;
class GLinkCalcManager;
class GLinkCursor;
class GLinks;


//------------------------------------------------------------------------------
// forward class declaration - Engines Part
class GEngine;
class GFactoryEngine;
class GFactoryEngineCursor;
class GEngineManager;
class GEngineDoc;
class GRanking;
class GMetaEngine;
class GFactoryMetaEngine;
class GFactoryMetaEngineCursor;
class GMetaEngineManager;


//------------------------------------------------------------------------------
// forward class declaration - Profiles Part
class GUsers;
class GUser;
class GUserCursor;
class GProfile;
class GSubProfile;
class GSubProfileProxy;
class GSubProfileCursor;
class GProfDoc;
class GProfDocCursor;
class GProfileCalc;
class GFactoryProfileCalc;
class GFactoryProfileCalcCursor;
class GProfileCalcManager;
class GProfilesSims;
class GProfilesBehaviours;


//------------------------------------------------------------------------------
// forward class declaration - Groups Part
class GGroups;
class GGroupsHistory;
class GGroupsHistoryCursor;
class GGroupsHistoryManager;
class GGroup;
class GGroupCursor;
class GGroupHistory;
class GGrouping;
class GFactoryGrouping;
class GFactoryGroupingCursor;
class GGroupingManager;
class GGroupCalc;
class GFactoryGroupCalc;
class GFactoryGroupCalcCursor;
class GGroupCalcManager;
class GSubject;
class GSubjectCursor;
class GSubjects;
class GPostGroup;
class GPostGroupManager;
class GFactoryPostGroup;
class GFactoryPostGroupCursor;


//------------------------------------------------------------------------------
// forward class declaration - Historic Part
class GWeightInfosHistory;
class GGroupHistory;
class GGroupHistoryCursor;
class GGroupsHistory;


//------------------------------------------------------------------------------
// forward class declaration - Sessions Part
class GTest;
class GTests;
class GSession;
class GSessionPrg;
class GSlot;
class GStatsCalc;
class GFactoryStatsCalc;
class GFactoryStatsCalcCursor;
class GStatsCalcManager;
class GMixedIdealGroups;
class GParams;
class GParam;
class GConfig;
class GSessionParams;
class GStorage;


//------------------------------------------------------------------------------
/**
* The cNoRef constant represented an undefined identificator reference.
*/
const unsigned cNoRef=0xFFFFFFFF;


//------------------------------------------------------------------------------
/**
* Types of GALILEI objects.
*/
enum tObjType
{
	otNoClass=0                  /** No specific class. */,
	otSession=1                  /** GALILEI Session. */,
	otDoc=2                      /** Document. */,
	otDocs=3                     /** Documents. */,
	otUsers=4                    /** Users.*/,
	otUser=5                     /** User.*/,
	otProfile=6                  /** Profile.*/,
	otSubProfile=7               /** SubProfile.*/,
	otGroups=8                   /** Groups.*/,
	otGroup=9                    /** Group.*/,
	otDocSubProfile=10           /** Document or SubProfile.*/,
	otDocGroup=11                /** Document or Group.*/,
	otSubProfileGroup=12         /** SubProfile or Group.*/,
	otFdbk=13                    /** Assessment of a document by a profile.*/
};


//------------------------------------------------------------------------------
/**
* States of a given GALILEI object.
*/
enum tObjState
{
	osUnknow=0                /** Unknow state.*/,
	osCreated=1               /** Object was created.*/,
	osUpToDate=2              /** Object is up to date.*/,
	osModified=3              /** Object was modified and computation must be update.*/,
	osUpdated=4               /** Object is updated and needs to be save.*/,
	osDelete=5                /** Object must be deleted.*/,
	osNotNeeded=6             /** Object is not needed.*/
};


//------------------------------------------------------------------------------
/**
* Judgments over a document.
*/
enum tDocAssessment
{
	 // the 4 first bits are used to express the judgemnet of the Doc (OK,KO,N,HS)
	 // the 5th and 6th bits are used to express the status of the Doc (Hub, Autority )
	 // the masks are used to separate the judgement and the status of a doc.
	djUnknow=0                  /** Unknow judgement.*/,
	djOK=1                      /** Document is relevant.*/,
	djKO=2                      /** Document is fuzzy relevant.*/,
	djOutScope=8                /** Document is irrelevant.*/,
	djHub=16                    /** Document is a Hub.*/,
	djAutority=32               /** Document is an Autority.*/,
	djMaskJudg=15               /** Mask for the judgement of the doc.*/,
	djMaskHubAuto=48            /** Mask for the Hub or Autority.*/

};


//------------------------------------------------------------------------------
/**
* Models used to describe subprofiles.
*/
enum tSubProfileDesc
{
	sdNothing=0               /** Unknow Model.*/,
	sdVector=1                /** Vector Model.*/
};


//------------------------------------------------------------------------------
/**
* Models used to describe informations.
*/
enum GInfoType
{
	infoNothing=0                  /** No type defined.*/,
	infoWord=1                     /** A word (or stem). */,
	infoWordList=2                 /** List of words.*/
};


//------------------------------------------------------------------------------
/**
* The GException class provides a basic representation for an exception.
* @author Pascal Francq
* @short Basic Exception.
*/
class GException : public R::RException
{
public:

	/**
	* Construct an exception.
	* @param str                      Message of the error.
	*/
	GException(const char* str) throw()
		: R::RException(str) {}

	/**
	* Construct an exception.
	*/
	GException(void) throw()
		: R::RException() {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
