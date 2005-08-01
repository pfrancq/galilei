/*

	GALILEI Research Project

	GALILEI.h

	GALILEI Header - Header.

	Copyright 2001-2004 by the Universit�Libre de Bruxelles.

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
#include <rstd.h>
#include <rdate.h>
#include <rlang.h>
#include <rstring.h>
#include <rcontainer.h>
#include <rcursor.h>
#include <rxmltag.h>
#include <rtextfile.h>
#include <random.h>


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
#define proxy public


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration - Infos Part
class GInfo;
class GInfoList;
class GWeightInfo;
class GWeightInfos;
class GInfosWeightsHistory;
class GLang;
class GFactoryLang;
class GLangManager;
class GData;
class GWord;
class GWordList;
class GDict;
class GDicts;


//------------------------------------------------------------------------------
// forward class declaration - Docs Part
class GFilter;
class GFactoryFilter;
class GFilterManager;
class GDoc;
class GDocAnalyse;
class GDocAnalyseManager;
class GPostDoc;
class GPostDocManager;
class GFactoryPostDoc;
class GDocs;
class GDocSim;
class GDocsSim;
class GDocXML;
class GFactoryDocAnalyse;
class GBalancedLinks;
class GFactoryLinkCalc;
class GLink;
class GLinkCalc;
class GLinkCalcManager;
class GLinks;
class GSugs;


//------------------------------------------------------------------------------
// forward class declaration - Engines Part
class GEngine;
class GFactoryEngine;
class GEngineManager;
class GEngineDoc;
class GRanking;
class GMetaEngine;
class GFactoryMetaEngine;
class GMetaEngineManager;
class GWordOccurs;
class GIndexer;


//------------------------------------------------------------------------------
// forward class declaration - Profiles Part
class GUsers;
class GUser;
class GProfile;
class GSubProfile;
class GFdbk;
class GProfileCalc;
class GFactoryProfileCalc;
class GProfileCalcManager;
class GPreProfile;
class GPostProfile;
class GFactoryPreProfile;
class GFactoryPostProfile;
class GPreProfileManager;
class GPostProfileManager;


//------------------------------------------------------------------------------
// forward class declaration - Groups Part
class GGroups;
class GGroupsHistory;
class GGroupsHistoryManager;
class GGroup;
class GGroupHistory;
class GGrouping;
class GFactoryGrouping;
class GGroupingManager;
class GGroupCalc;
class GFactoryGroupCalc;
class GGroupCalcManager;
class GSubject;
class GSubjects;
class GPostGroup;
class GPostGroupManager;
class GFactoryPostGroup;


//------------------------------------------------------------------------------
// forward class declaration - Historic Part
class GWeightInfosHistory;
class GGroupHistory;
class GGroupsHistory;


//------------------------------------------------------------------------------
// forward class declaration - Sessions Part
class GTest;
class GTests;
class GSession;
class GSessionPrg;
class GSlot;
class GSignalHandler;
class GStatsCalc;
class GFactoryStatsCalc;
class GStatsCalcManager;
class GMixedIdealGroups;
class GParams;
class GParam;
class GConfig;
class GStorage;


//------------------------------------------------------------------------------
// forward class declaration - Similarities Part
class GProfilesSims;
class GProfilesSimsManager;
class GFactoryProfilesSims;
class GProfilesDocsSims;
class GProfilesDocsSimsManager;
class GFactoryProfilesDocsSims;
class GGroupsDocsSims;
class GGroupsDocsSimsManager;
class GFactoryGroupsDocsSims;


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
* GALILEI event.
*/
enum tEvent
{
	eUnknow                   /** Unknow sevent.*/,
	eObjCreated               /** An object was created.*/,
	eObjModified              /** An object was modified.*/,
	eObjDeleted               /** An Object will be deleted.*/
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
* Models used to describe informations.
*/
enum GInfoType
{
	infoNothing=0                  /** No type defined.*/,
	infoWord=1                     /** A word (or stem). */,
	infoWordList=2                 /** List of words.*/,
	infoWordOccurs=3               /** List of occurences of a word in the
	                                   documents.*/,
	infoDoc=3                      /** Document.*/
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
