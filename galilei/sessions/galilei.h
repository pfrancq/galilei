/*

	GALILEI Research Project

	GALILEI.h

	GALILEI Header - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
#include <rrandom.h>
#include <rdebug.h>
#include <rnotificationcenter.h>



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
	otAnyClass               /** Any class. */,
	otSession                /** GALILEI Session. */,
	otDoc                    /** Document. */,
	otUser                   /** User.*/,
	otProfile                /** Profile.*/,
	otCommunity              /** Community.*/,
	otFdbk                   /** Assessment of a document by a profile.*/,
	otLang                   /** Language. */,
	otTopic                  /** Topic. */,
	otSubject                /** Subject (Theoretical topic of documents). */,
	otEngine                 /** Search engine.*/,
	otEngineDoc              /** Document proposed by a search engine.*/,
	otConcept                /** Concept.*/,
	otConceptType            /** Concept Type.*/,
   otConceptCat            /** Concept Category.*/,
	otPredicate              /** Predicate.*/,
	otStatement 	         /** Statement.*/,
	otClass                  /** Class.*/,
	otDocStruct              /** Document structure. */,
	otSuggestion             /** Suggestion.*/,
	otDocRanking             /** Document ranking.*/,
	otDescFile               /** Description file.*/,
	otStructFile             /** Structure file.*/,
	otIndexFile              /** Index file.*/,
	otReference              /** Reference.*/
};


//------------------------------------------------------------------------------
/**
 * Get a string representing a type of objects.
 * @param type               Type.
 * @param upper              First letter in uppercase ?
 * @param plural             Plural ?
 */
R::RString GetObjType(tObjType type,bool upper,bool plural);


//------------------------------------------------------------------------------
/**
* States of a given GALILEI object.
* @short Object States
*/
enum tObjState
{
	osUnknow                 /** Unknown state.*/,
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
 * @param upper               First letter in uppercase ?
 */
R::RString GetState(tObjState state,bool upper);


//------------------------------------------------------------------------------
/**
 * During the document analysis process, the filters extract tokens from the
 * documents. Different sort of tokens are currently managed.
 * @short Token Type
 */
enum tTokenType
{
	 ttUnknown               /** Unknown type.*/,
	 ttDeleted               /** A deleted token.*/,
	 ttText                  /** Text.*/,
	 ttDivision              /** Document part, chapter, section, etc.*/,
	 ttXMLTag                /** An XML tag.*/,
	 ttXMLAttr               /** An XML attribute.*/
};


//------------------------------------------------------------------------------
/**
* Different types of feedbacks over a document.
* @short Feedback Type
*/
enum tFdbkType
{
	ftUnknown                /** Unknown feedback type.*/,
	ftRelevant               /** Document is assessed as relevant.*/,
	ftFuzzyRelevant          /** Document is assessed as fuzzy relevant.*/,
	ftIrrelevant             /** Document is assessed as irrelevant.*/
};


//------------------------------------------------------------------------------
/**
 * @return a string representing a feedback type.
 * @param type                TYpe of the feedback.
 * @param upper               First letter in uppercase ?
 */
R::RString GetFdbkType(tFdbkType type,bool upper);


//------------------------------------------------------------------------------
/**
* return a feedback type.
* @param type                Feedback type.
*/
tFdbkType GetFdbkType(unsigned int type);


//------------------------------------------------------------------------------
/**
* Different types of profiles.
* @short Profile Type
*/
enum tProfileType
{
	ptUnknown                /** Unknown profile type.*/,
	ptInterest               /** Profile representing an interest of a user
	                             expressed through a set of feedbacks on
	                             documents.*/,
	ptExpertise              /** Profile representing a particular expertise of
	                             a user.*/
};


//------------------------------------------------------------------------------
/**
* @return a string representing a profile type.
* @param type                Profile type.
* @param upper               First letter in uppercase ?
* @param plural             Plural ?
*/
R::RString GetProfileType(tProfileType type,bool upper,bool plural);


//------------------------------------------------------------------------------
/**
* return a profile type.
* @param type                Profile type.
*/
tProfileType GetProfileType(unsigned int type);


//------------------------------------------------------------------------------
/**
 * Descriptions for the subjects.
 * @short Subject Description.
 */
enum tSubjectDesc
{
	sdNames                 /** Names of the subjects are used.*/,
	sdDocs		            /** Selected documents are used.*/
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
	* Construct an exception with the message "func [where]: str". A typical use is:
	* @code
	* if(!ptr)
	* 	throw GException(__PRETTY_FUNCTION__,__LINE__,"ptr cannot be a null pointer");
	* @endcode
	* @see The ThrowGException marco.
	* @param func                     Function producing the error.
	* @param where                    Line position of the error.
	* @param str                      Message of the error.
	*/
	GException(const char* func,long where,const char* str) throw()
		: R::RException(func,where,str) {}

	/**
	* Construct an exception.
	*///------------------------------------------------------------------------------
	GException(void) throw()
		: R::RException() {}
};


//------------------------------------------------------------------------------
// Macro to generate a GException
#define ThrowGException(msg) throw GALILEI::GException(__PRETTY_FUNCTION__,__LINE__,msg)


//------------------------------------------------------------------------------
// forward class declaration - Knowledge Part
class GConcept;
class GConceptCat;
class GConceptType;
class GPredicate;
class GStatement;
class GLang;
class GConceptRef;
class GVector;
class GDescription;
class GDescriptionSet;
class GConceptNode;
class GConceptTree;
template<class C> class GDescriptionObject;
class GClass;
class GClasses;


//------------------------------------------------------------------------------
// forward class declaration - Documents Part
class GDoc;
class GBalancedLinks;
class GLink;
class GLinks;
class GDocAnalyze;
class GFilter;
class GTokenizer;
class GAnalyzer;
class GToken;


//------------------------------------------------------------------------------
// forward class declaration - Engines Part
class GDocRetrieved;
class GDocRanking;
class GSuggestion;
class GSugs;
class GComputeSugs;
class GComputeTrust;
class GEngine;
class GMetaEngine;


//------------------------------------------------------------------------------
// forward class declaration - Profiles Part
class GUser;
class GProfile;
class GFdbk;
class GProfileCalc;
class GLinkCalc;


//------------------------------------------------------------------------------
// forward class declaration - Groups Part
class GCommunity;
class GCommunityDocs;
class GSubject;
class GSubjects;
class GTopic;
class GGroupDocs;
class GGroupProfiles;
class GTopicCalc;
class GCommunityCalc;


//------------------------------------------------------------------------------
// forward class declaration - Session Part
class GObject;
template<class C> class GObjects;
class GBasicSession;
class GSession;
class GSlot;
class GStorage;
class GGALILEIApp;
class GGALILEIPrg;
class GSimulator;


//------------------------------------------------------------------------------
// forward class declaration - Plug-ins Part
class GPlugIn;
class GPlugInFactory;
class GPlugInManager;
class GMeasure;
class GMatrixMeasure;
class GTool;
class GFunction;


//------------------------------------------------------------------------------
// Some null pointers
/** Null pointer for GConcept.*/
extern const GConcept* pConcept;
/** Null pointer for GConceptType.*/
extern const GConceptType* pConceptType;
/** Null pointer for GDoc.*/
extern const GDoc* pDoc;
/** Null pointer for GTopic.*/
extern const GTopic* pTopic;
/** Null pointer for GClass.*/
extern const GClass* pClass;
/** Null pointer for GUser.*/
extern const GUser* pUser;
/** Null pointer for GProfile.*/
extern const GProfile* pProfile;
/** Null pointer for GCommunity.*/
extern const GCommunity* pCommunity;


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
	* @param upper               First letter in uppercase ?
	*/
	static R::RString GetEvent(tEvent event,bool upper);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
