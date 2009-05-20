/*

	GALILEI Research Project

	GALILEI.h

	GALILEI Header - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rnumcontainer.h>
#include <rnumcursor.h>
#include <rxmltag.h>
#include <rtextfile.h>
#include <random.h>
#include <rdebug.h>
#include <rnotificationcenter.h>


//------------------------------------------------------------------------------
/**
 * \mainpage GALILEI Research Project
 *
 * \section intro_sec Introduction
 *
 * GALILEI is a research and development platform in the field of information
 * science.
 *
 * \section main_classes_sec Main classes
 * The main classes are:
 * - GALILEI::GOntology.
 * - GALILEI::GIndexer
 *
 */


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
/**
* Types of GALILEI objects.
*/
enum tObjType
{
	otNoClass                /** No specific class. */,
	otSession                /** GALILEI Session. */,
	otDoc                    /** Document. */,
	otUser                   /** User.*/,
	otProfile                /** Profile.*/,
	otCommunity              /** Community.*/,
	otFdbk                   /** Assessment of a document by a profile.*/,
	otLang                   /** Language. */,
	otHistory                /** History. */,
	otTopic                  /** Topic. */,
	otSubject                /** Subject (Theoretical topic of documents). */,
	otEngine                 /** Search engine.*/,
	otEngineDoc              /** Document proposed by a search engine.*/,
	otConcept                /** Concept.*/,
	otConceptType            /** Concept Type.*/,
	otPredicate              /** Predicate.*/,
	otStatement 	         /** Statement.*/,
	otClass                  /** Class.*/
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
	osUnknow                 /** Unknow state.*/,
	osCreated                /** Object created in the system.*/,
	osNew                    /** Object was allocated in memory.*/,
	osNeedLoad               /** Object must load information.*/,
	osOnDemand               /** Object demands to load information. */,
	osLoaded                 /** Object loaded information.*/,
	osUpToDate               /** Object is up to date.*/,
	osModified               /** Object was modified and computation must be update.*/,
	osUpdated                /** Object is updated and needs to be save.*/,
	osSaved                  /** Object was saved.*/,
	osDelete                 /** Object is deleted from memory.*/,
	osDestroyed              /** Object is destroyed from the system.*/
};


//------------------------------------------------------------------------------
/**
* Get a string representing a state of objects.
* @param state               State.
*/
R::RString GetState(tObjState state);


//------------------------------------------------------------------------------
/**
* Assessment over a document.
*
* The 4 first bits are used to express the assessment of the Doc (OK,KO,N), the
* 5th and 6th bits are used to express the status of the Doc (Hub, Authority ).
*  The masks are used to separate the assessment and the status of a doc.
* @short Document Assessment
*/
enum tDocAssessment
{
	djUnknow                 /** Unknown assessment.*/=0,
	djOK                     /** Document is relevant.*/=1,
	djKO                     /** Document is fuzzy relevant.*/=2,
	djOutScope               /** Document is irrelevant.*/=8,
	djHub                    /** Document is a Hub.*/=16,
	djAuthority              /** Document is an Authority.*/=32,
	djMaskJudg               /** Mask for the assessment of the doc.*/=15,
	djMaskHubAuto            /** Mask for the Hub or Authority.*/=48
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
 * Type of a given measure related to objects in GALILEI. It is supposed that
 * only measure between two objects are computed.
 * @short Type of measures.
 */
enum tTypeMeasure
{
	tmUnknow                 /** Unknow measure.*/,
	tmDoc                    /** Measure(document,document).*/,
	tmProfile                /** Measure(profile,profile).*/,
	tmDocProfile             /** Measure(document,profile).*/,
	tmCommunity              /** Measure(community,community).*/,
	tmDocCommunity           /** Measure(document,community).*/,
	tmProfileCommunity       /** Measure(profile,community).*/,
	tmTopic                  /** Measure(topic,topic).*/,
	tmDocTopic               /** Measure(document,topic).*/
};


//------------------------------------------------------------------------------
/**
 * Get a string representing the type of measure.
 * @param type               Type of the measure.
 */
R::RString GetMeasureType(tTypeMeasure type);


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
	*///------------------------------------------------------------------------------
	GException(void) throw()
		: R::RException() {}
};


//------------------------------------------------------------------------------
// forward class declaration - Knowledge Part
class GConcept;
class GConceptType;
class GPredicate;
class GStatement;
class GLang;
class GFactoryLang;
class GLangManager;
class GWeightInfo;
class GWeightInfos;
class GWeightInfosObj;
class GOntology;
class GClass;
class GClasses;


//------------------------------------------------------------------------------
// forward class declaration - Documents Part
class GFilter;
class GFactoryFilter;
class GFilterManager;
class GTopic;
class GDoc;
class GDocAnalyse;
class GDocAnalyseManager;
class GPostDoc;
class GPostDocManager;
class GFactoryPostDoc;
class GDocXML;
class GFactoryDocAnalyse;
class GBalancedLinks;
class GFactoryLinkCalc;
class GLink;
class GLinkCalc;
class GLinkCalcManager;
class GLinks;
class GSugs;
class GTVDRec;
class GDocStruct;


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
class GIndexer;


//------------------------------------------------------------------------------
// forward class declaration - Profiles Part
class GUser;
class GProfile;
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
// forward class declaration - Communities Part
class GCommunity;
class GGroupProfiles;
class GFactoryGroupProfiles;
class GFactoryGroupDocs;
class GGroupProfilesManager;
class GGroupDocsManager;
class GCommunityCalc;
class GCommunityCalcManager;
class GTopicCalcManager;
class GSubject;
class GSubjects;
class GPostCommunity;


//------------------------------------------------------------------------------
// forward class declaration - Topics Part
class GTopic;
class GGroupDocs;
class GTopicCalc;
class GFactoryCommunityCalc;
class GFactoryTopicCalc;
class GPostTopic;
class GPostCommunityManager;
class GPostTopicManager;
class GFactoryPostCommunity;
class GFactoryPostTopic;


//------------------------------------------------------------------------------
// forward class declaration - Session Part
class GObject;
class GDebugObject;
class GSession;
class GSlot;
class GStatsCalc;
class GFactoryStatsCalc;
class GStatsCalcManager;
class GStorage;
class GStorageManager;
class GFactoryStorage;
class GMeasure;
class GMeasureManager;
class GFactoryMeasure;
class GGALILEIApp;
class GGALILEIPrg;


//------------------------------------------------------------------------------
// forward class declaration - Tools Part
class GTool;
class GFactoryTool;
class GToolManager;
class GMatrixMeasure;


//------------------------------------------------------------------------------
/**
 * The GEvent provides information when the notification message
 * "ObjectChanged" is emitted.
 * @author Pascal Francq
 * @short Event information.
 */
struct GEvent
{
	/**
	* GALILEI event.
	* @short Event
	*/
	enum tEvent
	{
		eUnknow                  /** Unknown event.*/,
		eObjCreated              /** An object was created in the system.*/,
		eObjNew                  /** An object was created in memory.*/,
		eObjModified             /** An object was modified.*/,
		eObjDelete               /** An Object will be deleted from memory.*/,
		eObjDestroyed            /** An object was destroyed from the system.*/
	};

	/**
	 * Event.
	 */
	tEvent Event;

	/**
	 * Object.
	 */
	const GObject* Object;

	/**
	 * Construct the event.
	 * @param event          Type of the event.
	 * @param object         Object about which the signal is emitted.
	 */
	GEvent(tEvent event,const GObject* object) : Event(event), Object(object) {}

	/**
	* Get a string representing a event.
	* @param event               Event.
	*/
	static R::RString GetEvent(tEvent event);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
