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
#include <rdebug.h>


//------------------------------------------------------------------------------
/**
* \namespace GALILEI
* \brief GALILEI Classes.
*
* This namespace declares all the classes implemented during the GALILEI
* project.
*/


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration - Knowledge Part
class GConcept;
class GConceptType;
class GRelation;
class GRelationType;
class GLang;
class GFactoryLang;
class GLangManager;
class GDict;


//------------------------------------------------------------------------------
// forward class declaration - Infos Part
class GInfo;
class GWeightInfo;
class GWeightInfos;
class GInfosWeightsHistory;


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
class GGroup;
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
class GGroupsHistoryManager;


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
class GStorage;
class GStorageManager;
class GFactoryStorage;
class GMeasure;
class GMeasureManager;
class GFactoryMeasure;
class GTypeMeasureManager;
class GGALILEIApp;


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
	otFdbk=13                    /** Assessment of a document by a profile.*/,
	otLang=14                    /** Language. */,
	otHistory=15                 /** History */
};


//------------------------------------------------------------------------------
/**
* Get a string representing a type of objects.
* @param objtype             Type.
*/
R::RString GetObjType(tObjType objtype);


//------------------------------------------------------------------------------
/**
* States of a given GALILEI object.
* @short Object States
*/
enum tObjState
{
	osUnknow                  /** Unknow state.*/,
	osNewMem                  /** Object was created in memory.*/,
	osUpToDate                /** Object is up to date.*/,
	osModified                /** Object was modified and computation must be update.*/,
	osUpdated                 /** Object is updated and needs to be save.*/,
	osSaved                   /** Object was saved.*/,
	osDeleteMem               /** Object must be deleted from memory.*/,
	osNotNeeded               /** Object is not needed.*/,
	osOnDemand                /** Object demands to load information. */,
	osNeedLoad                /** Object must load information.*/
};


//------------------------------------------------------------------------------
/**
* Get a string representing a state of objects.
* @param state               State.
*/
R::RString GetState(tObjState state);


//------------------------------------------------------------------------------
/**
* GALILEI event.
* @short Event
*/
enum tEvent
{
	eUnknow                   /** Unknow event.*/,
	eObjCreate                /** An object was created in the database.*/,
	eObjNewMem                /** An object was created in memory.*/,
	eObjModified              /** An object was modified.*/,
	eObjDeleteMem             /** An Object will be deleted from memory.*/,
	eObjRemove                /** An object was removed from the database.*/
};


//------------------------------------------------------------------------------
/**
* Get a string representing a event.
* @param event               Event.
*/
R::RString GetEvent(tEvent event);


//------------------------------------------------------------------------------
/**
* Judgments over a document.
* @short Document Assessment
*/
enum tDocAssessment
{
	// the 4 first bits are used to express the assessment of the Doc (OK,KO,N)
	// the 5th and 6th bits are used to express the status of the Doc (Hub, Autority )
	// the masks are used to separate the assessment and the status of a doc.
	djUnknow=0                  /** Unknow judgement.*/,
	djOK=1                      /** Document is relevant.*/,
	djKO=2                      /** Document is fuzzy relevant.*/,
	djOutScope=8                /** Document is irrelevant.*/,
	djHub=16                    /** Document is a Hub.*/,
	djAutority=32               /** Document is an Autority.*/,
	djMaskJudg=15               /** Mask for the assessment of the doc.*/,
	djMaskHubAuto=48            /** Mask for the Hub or Autority.*/
};


//------------------------------------------------------------------------------
/**
* Get a string representing an assessment.
* @param assessment          Assessment.
*/
R::RString GetAssessment(tDocAssessment assessment);


//------------------------------------------------------------------------------
/**
* Get a string representing the code of an assessment.
* @param assessment          Assessment.
*/
R::RString GetAssessmentCode(tDocAssessment assessment);


//------------------------------------------------------------------------------
/**
* Get a string representing the code of an assessment.
* @param assessment          Assessment.
*/
tDocAssessment GetAssessmentType(const R::RString& assessment);


//------------------------------------------------------------------------------
/**
* Models used to describe informations.
* @short Information Type
*/
enum tInfoType
{
	infoNothing                    /** No type defined.*/,
	infoWord                       /** A word (or stem). */,
	infoWordList                   /** List of words.*/,
	infoWordOccurs                 /** List of occurences of a word in the
	                                   documents.*/,
	infoDoc                        /** Document.*/
};


//------------------------------------------------------------------------------
/**
* Get a string representing a type of information.
* @param infotype            Type of information.
*/
R::RString GetInfoType(tInfoType infotype);


//------------------------------------------------------------------------------
/**
* Status of a document.
*/
enum tDocStatus
{
	dsOK=0                         /** Document is OK.*/,
	dsToAnalyse=1                  /** Document must be analysed.*/,
	dsCannotAccess=2               /** Cannot access the document. */,
	dsCannotAnalyse=3              /** Document was downloaded but cannot be analysed.*/
};


//------------------------------------------------------------------------------
/**
* Get a string representing a status of a document.
* @param status              Status.
*/
R::RString GetDocStatus(tDocStatus status);


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
