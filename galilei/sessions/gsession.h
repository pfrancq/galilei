/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by Julien Lamoral.
	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by David Wartel.
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
#ifndef GSession_H
#define GSession_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rconfig.h>
#include <random.h>
#include <rindexfile.h>
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <genginedoc.h>
#include <gclass.h>
#include <gsubject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GSession provides a representation for a GALILEI session.
* @author GALILEI Team
* @short Generic Session.
*/
class GSession : public R::RConfig
{
	class IndexType;
	class GDocRefURL;

	/**
	 * Identifier of the session.
	 */
	size_t Id;

	/**
	 * Name of the session.
	 */
	R::RString Name;

	/**
	 * Storage manager.
	 */
	GStorage* Storage;

	/**
	 *  Must the results be saved after computed ?
	 */
	bool SaveResults;

    /**
     * Log file for the session.
     */
	 R::RTextFile Log;

	/**
	 * The simulator (if any).
	 */
	GSimulator* Simulator;

    /**
     * Current seek for the session.
     */
	int CurrentRandom;

    /**
     * Random number generator.
     */
	R::RRandom* Random;

	/**
	 *  Types of Concepts.
	 */
	R::RContainer<GConceptType,true,true> ConceptTypes;

	/**
	 *  Types of Concepts (accessed by identifiers).
	 */
	R::RContainer<GConceptType,false,false> ConceptTypesByIds;

	/**
	* Array of concepts ordered.
	*/
	R::RContainer<GConcept,true,false> Concepts;

	/**
	 * Predicates.
	 */
	R::RContainer<GPredicate,false,true> Predicates;

	/**
	 * Predicates (accessed by identifiers).
	 */
	R::RContainer<GPredicate,true,false> PredicatesByIds;

	/**
	 * Array of statements ordered.
	 */
	R::RContainer<GStatement,true,false> Statements;

	/**
	 * File containing the structure of the documents.
	 */
	R::RIndexFile* StructDoc;

	/**
	 * Temporary vector of references.
	 */
	R::RNumContainer<size_t,true> tmpRefs;

	/**
	 * Types managed by the indexer.
	 */
	R::RContainer<IndexType,true,true> Types;

	/**
	 * Names of the types managed by the indexer.
	 */
	R::RContainer<R::RString,true,true> TypesNames;

    /**
     * Documents handled by the system.
     */
	R::RContainer<GDoc,true,true> Docs;

    /**
     * Are the documents loaded?
     */
	bool DocsLoaded;

	/**
	 * Documents ordered by URL.
	 */
	R::RContainer<GDocRefURL,true,true> DocsRefUrl;

    /**
     * Users handled by the system.
     */
	R::RContainer<GUser,true,true> Users;

	/**
	 * Are the users and the profiles loaded?
	 */
	bool UsersLoaded;

	/**
	 * Profiles handled by the system.
	 */
	R::RContainer<GProfile,true,true> Profiles;

	/**
	 * Communities handled by the system.
	 */
	R::RContainer<GCommunity,true,true> Communities;

    /**
     * Are the communities loaded?
     */
	bool CommunitiesLoaded;

    /**
     * Topics handled by the system.
     */
	R::RContainer<GTopic,true,true> Topics;

	/**
	 * Are the topics loaded?
	 */
	bool TopicsLoaded;

	/**
	 * Classes organizing the concepts.
	 */
	GClasses Classes;

	/**
	 * Determine if all the classes were loaded.
	 */
	bool ClassesLoaded;

    /**
     * Maximum number of documents to handle in memory.
     */
	size_t MaxDocs;

	/**
	 * Maximum number of profiles to handle in memory.
	 */
	size_t MaxProfiles;

	 /**
	  * Maximum number of groups to handle in memory.
	  */
	size_t MaxGroups;

	/**
	 * Limit the clustering of the documents to the selected ones.
	 */
	bool ClusterSelectedDocs;

	/**
	 * Subjects.
	 */
	GSubjects Subjects;

	/**
	 * External break.
	 */
	bool ExternBreak;

public:

	/**
	* Constructor.
	* @param id              Identifier.
	* @param name            Name of the session.
	*/
	GSession(size_t id,const R::RString& name);

	/**
	* Analyze a document.
	* @param doc             Pointer to the document to analyze.
	* @param ram             Must the description of the document be maintain
	*                        in RAM.
	*
	* @param method          Pointer to the method used to analyze. If null,
	*                        the default method is used.
	* @param rec             Receiver for the signals.
	*/
	void AnalyzeDoc(GDoc* doc,bool ram=true,GDocAnalyze* method=0,GSlot* rec=0);

	/**
	* Analyze the documents. At the end, all the enabled post-documents methods
	* are run.
	* @param ram             Must the description of each document be maintain
	*                        in RAM.
	* @param rec             Receiver for the signals.
	*/
	void AnalyzeDocs(bool ram=false,GSlot* rec=0);

	/**
	* Analyze a given string to search for variables of the type '%%var%%'. These
	* variables are then replaced with the corresponding value. Actually, the
	* known variables are:
	* - %%world% : Name of the session.
	*
	* @param str             String to analyze.
	*/
	R::RString AnalyzeString(const R::RString& str);

	/**
	 * Apply the internal configuration. This method must be called to take
	 * changes in the configuration into account.
	 */
	void ApplyConfig(void);

	/**
	* Assign an identifier to a new community.
	* @param com             Community.
	*/
	void AssignId(GCommunity* com);

	/**
	* Assign an identifier to a new document.
	* @param doc             Document.
	*/
	void AssignId(GDoc* doc);

	/**
	* Assign an identifier to a new profile.
	* @param p               Pointer to the profile.
	*/
	void AssignId(GProfile* p);

	/**
	* Assign an identifier to a new topic.
	* @param top             Topic.
	*/
	void AssignId(GTopic* top);

	/**
	* Assign an identifier to a new user.
	* @param user            Pointer to the user.
	*/
	void AssignId(GUser* user);

	/**
	 * Assign a specific description to a given class. If necessary, the
	 * description is saved.
	 * @param theclass       The class.
	 * @param infos          Vector to assign.
	 */
	void AssignInfos(GClass* theclass,GWeightInfos& infos);

	/**
	* See if the session must break.
	*/
	bool MustBreak(void);

	/**
	 * Build the references from scratch for a given objects type.
	 * @param type           Type of the objects.
	 * @param slot           Slot receiving the information.
	 */
	void BuildRefs(tObjType type,GSlot* slot=0);

	/**
	* Compute a profile.
	* @param profile         Profile to compute.
	* @param method          Profile Computation method.
	* @param linkcalc        Link Computation method.
	* @param rec             Receiver for the signals.
	*/
	void CalcProfile(GProfile* profile,GProfileCalc* method=0,GLinkCalc* linkcalc=0,GSlot* rec=0);

	/**
	* Compute the profiles.
	* @param rec             Receiver for the signals.
	*/
	void CalcProfiles(GSlot* rec=0);

	/**
	 * Clear the objects.
	 * @param type           Type of the objects.
	 * @param meta           Meta-type (if any) of objects (may be files or
	 *                       references).
	 */
	void Clear(tObjType type,tObjType meta=otNoClass);

	/**
	 * Compare two session regarding their identifiers.
	 * @param session        Session to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const GSession& session) const;

	/**
	 * Compare the identifier of the session with a given one.
	 * @param id             Identifier to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(size_t id) const;

	/**
	 * Compare the name of the session with a given one.
	 * @param name           Name to compare with.
	 * @return a value compatible with R::RContainer.
	 */
	int Compare(const R::RString& name) const;

	/**
	* Run all the enabled suggestions computing methods.
	* @param rec             Receiver of the signals.
	*/
	void ComputeSugs(GSlot* rec=0);

	/**
	* Run all the enabled trust computing methods.
	* @param rec             Receiver of the signals.
	*/
	void ComputeTrust(GSlot* rec=0);

	/**
	* Delete a community.
	* @param com             Community.
	*/
	void Delete(GCommunity* com);

	/**
	* Delete a given concept from the dictionary.
	* @param concept         Concept to delete.
	*/
	void Delete(GConcept* concept);

	/**
	* Delete a topic.
	* @param top             Topic.
	*/
	void Delete(GTopic* top);

	/**
	* Fill a given array with all the topics. The array must be created and
	* must be large enough to hold all the topics. It can contained null
	* pointers.
	* @see This method is used in GSubjects to degrade the ideal clustering.
	* @param topics          Pointer to the array.
	* @returns Size of the data (including the null pointers) copied in the
	* array.
	*/
	size_t FillTopics(GTopic** topics);

	/**
	* Fill a given array with all the selected documents. The array must be
	* created and must be large enough to hold all the documents.
	* @see This method is used in GSimulator to create assessments for
	*      profiles during a simulation of a real system.
	* @param docs            Pointer to the array.
	* @returns Size of the data copied in the array.
	*/
	size_t FillSelectedDocs(GDoc** docs);

	/**
	* Fill a given array with all the subjects. The array must be created and
	* must be large enough to hold all the subjects.
	* @see This method is used in GSimulator to create assessments for
	*      profiles during a simulation of a real system.
	* @param subjects        Pointer to the array.
	* @returns Size of the data copied in the array.
	*/
	size_t FillSubjects(GSubject** subjects);

	/**
	 * Flush the caches of a given object type: All the blocks in memory that
	 * are dirtied are save on disk. This can be useful to do after a long
	 * computation (such as the document analysis).
	 * @param type           Type of the object.
	 */
	void FlushDesc(tObjType type);

	/**
	* Force some objects to be re-computed even if they are updated.
	* @param type            Type of the objects. Only 'otDocs', 'otUsers' and
	*                        'otGroups' are allowed.
	*/
	void ForceReCompute(tObjType type);

	/**
	 * Search for a class with a given identifier.
	 * @param id             Identifier.
	 * @param null           If true, 0 is returned if the class is not found,
	 *                       else an exception is generated.
	 * @return Pointer to the class.
	 */
	GClass* GetClass(size_t id,bool null);

	/**
	* Get a cursor on all the classes.
	*/
	R::RCursor<GClass> GetClasses(void) const {return(R::RCursor<GClass>(Classes.GetNodes()));}

	/**
	 * @return Pointer to the class.
	 */
	GClasses* GetClassTree(void) {return(&Classes);}

	/**
	* Get a cursor on the communities.
	*/
	R::RCursor<GCommunity> GetCommunities(void);

	/**
	* Get a community corresponding to a given identifier.
	* @param id              Identifier of the community.
	* @param load            If set to true, the community is eventually loaded
	*                        into memory.
	* @param null            If set to true, if the community does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to GCommunity.
	*/
	GCommunity* GetCommunity(size_t id,bool load=true,bool null=false);

	/**
	* Get the concept with a specific identifier.
	* @param id             Identifier.
	* @return Pointer to a GConcept.
	*/
	GConcept* GetConcept(size_t id);

	/**
	* Get all concepts defined.
	* @return RCursor over GConcept.
	*/
	R::RCursor<GConcept> GetConcepts(void) const;

	/**
	* Get the a pointer to a type of concept.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetConceptType(char id,bool null);

	/**
	* Get the a pointer to a type of concept. If the concept type doesn't
	* exist, it is created.
	* @param name            Name of the type.
	* @param desc            Short description
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetConceptType(const R::RString& name,const R::RString& desc);

	/**
	* Get the a pointer to a type of concept.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetConceptType(const R::RString& name,bool null);

	/**
	* Get all concept types defined.
	* @return RCursor over GConceptType.
	*/
	inline R::RCursor<GConceptType> GetConceptTypes(void) const {return(R::RCursor<GConceptType>(ConceptTypes));}

	/**
	* Get The Current RandomSeed.
	* @return The current RandomSeed.
	*/
	inline int GetCurrentRandom(void) const {return(CurrentRandom);}

	/**
	* Get a current Random Value.
	* @param max             Max Random Value.
	* @return The current Random value * max.
	*/
	inline size_t GetCurrentRandomValue(size_t max) {return(Random->GetValue(max));}

	/**
	* Get a document corresponding to a given identifier.
	* @param id              Identifier of the document.
	* @param load            If set to true, the document is eventually loaded
	*                        into memory.
	* @param null            If set to true, if the document does not exist,
	*                        return 0, else an exception is generated.
	*/
	GDoc* GetDoc(size_t id,bool load=true,bool null=false);

	/**
	* Get a document corresponding to a given URL.
	* @param url             URL of the document.
	* @param load            If set to true, the document is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the document does not exist,
	*                        return 0, else an exception is generated.
	*/
	GDoc* GetDoc(const R::RString& url,bool load=true,bool null=false) const;

	/**
	* Get a cursor on all the documents.
	*/
	R::RCursor<GDoc> GetDocs(void) const;

	/**
	 * @return the identifier of the session.
	 */
	inline size_t GetId(void) const {return(Id);}

	/**
	 * @return the name of the session.
	 */
	inline R::RString GetName(void) const {return(Name);}

	/**
	 * Get the depth of the tree formed by all the subjects.
	 */
	size_t GetMaxDepth(void) const;

	/**
	 * Get the highest identifier for the elements of a given type.
	 * @param type            Type of the elements.
	 * @returns Highest identifier.
	 */
	size_t GetMaxObjectId(tObjType type) const;

	/**
	 * Get the maximum position of the objects of a given type handled.
	 * @param type            Type of the elements.
	 * @returns Maximum position occupied by the elements.
	 */
	size_t GetMaxObjectPos(tObjType type) const;

	/**
	* Compute the number of ideal groups of a given type (otProfile or otDoc)
	* for a given topic (and its sub-topics).
	* @param type            Type.
	*/
	size_t GetNbIdealGroups(tObjType type) const;

	/**
	* Get the number of elements of a given type.
	* @param type            Type of the elements.
	* @returns Number of elements.
	*/
	size_t GetNbObjects(tObjType type) const;

	/**
	* Get the number of subjects associated with a given document.
	* @param doc            Pointer to the document.
	*/
	size_t GetNbSubjects(GDoc* doc) const;

	/**
	* Compute the number of topics (and its sub-topics) containing a document.
	*/
	size_t GetNbTopicsDocs(void) const;

	/**
	 * Find an object of a given type and with a particular identifier.
	 * @param type           Type. Only the objects managed.
	 * @param id             Identifier.
	 * @param null           If set to true, if the document does not exist,
	 *                       return 0, else an exception is generated.
	 * @return Pointer to the object.
	 */
	GObject* GetObject(tObjType type,size_t id,bool null=false);

	/**
	 * Fill an array of pointers with the elements of a given type.
	 * @param type           Type of the elements.
	 * @param tab            Array that will be filled.
	 * @param alloc          Must the array be allocated.
	 * @return number of elements in the array (including null pointers).
	 */
	size_t GetObjects(tObjType type,GObject** &tab,bool alloc);

	/**
	* Get the a pointer to a predicate.relation type
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GRelationType
	*/
	GPredicate* GetPredicate(size_t id,bool null);

	/**
	* Get the a pointer to a predicate.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GRelationType
	*/
	GPredicate* GetPredicate(const R::RString& name,bool null);

	/**
	* Get all predicates defined.
	*/
	R::RCursor<GPredicate> GetPredicates(void) const;

	/**
	* Get a profile with a specific identifier.
	* @param id              Identifier.
	* @param load            If set to true, the profile is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the profile does not exist,
	*                        return 0, else an exception is generated.
	*/
	GProfile* GetProfile(const size_t id,bool load=true,bool null=false);

	/**
	* Get a cursor over the profiles of the system.
	*/
	R::RCursor<GProfile> GetProfiles(void) const;

	/**
	* Get the random number generator.
	* @returns Pointer to RMath::RRandom;
	*/
	inline R::RRandom* GetRandom(void) const {return(Random);}

	/**
	 * @return Simulator associated with the session.
	 */
	GSimulator* GetSimulator(void) const;

	/**
	* Get a statement.
	* @param id              Identifier of the statement.
	* @return Pointer to a statement.
	*/
	GStatement* GetStatement(size_t id);

	/**
	* @return Pointer to storage manager.
	*/
	inline GStorage* GetStorage(void) const {return(Storage);}

	/**
	* Get the subject of a given community.
	* @param com             Pointer to the community.
	*/
	const GSubject* GetSubject(GCommunity* com) const;

	/**
	* Get the subject of a given document.
	* @param doc             Pointer to the document.
	*/
	const GSubject* GetSubject(GDoc* doc) const;

	/**
	* Get a subject.
	* @param id              Identifier of the subject.
	* @return Pointer to GSubject.
	*/
	GSubject* GetSubject(size_t id) const;

	/**
	* Get a pointer to a given subject.
	* @param name            Name of the subject.
	* @return Return the pointer or 0 if the element is not in the tree.
	*/
	GSubject* GetSubject(const R::RString& name) const;

	/**
	* Get the subject of a given profile.
	* @param prof            Pointer to the profile.
	*/
	const GSubject* GetSubject(GProfile* prof) const;

	/**
	* Get a cursor over all the nodes of the tree.
	*/
	R::RCursor<GSubject> GetSubjects(void) const;

	/**
	* Get a Cursor on the subjects of a given document.
	* @param doc            Pointer to the document.
	*/
	R::RCursor<GSubject> GetSubjects(GDoc* doc) const;

	/**
	* Get a cursor over the child subjects of a given subject.
	* @param subject            Parent subject. If null, the top subjects are returned.
	*/
	R::RCursor<GSubject> GetSubjects(const GSubject* subject) const;

	/**
	* @return a cursor on the top classes.
	*/
	R::RCursor<GClass> GetTopClasses(void) const {return(R::RCursor<GClass>(Classes.GetTopNodes()));}

	/**
	* @return a cursor over the top nodes.
	*/
	R::RCursor<GSubject> GetTopSubjects(void) const;

	/**
	* Get a topic corresponding to a given identifier.
	* @param id              Identifier of the topic.
	* @param load            If set to true, the topic is eventually loaded
	*                        into memory.
	* @param null            If set to true, if the topic does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to GTopic.
	*/
	GTopic* GetTopic(size_t id,bool load=true,bool null=false);

	/**
	* @return a cursor on the topics.
	*/
	R::RCursor<GTopic> GetTopics(void);

	/**
	 * @return the names of the objects types supported.
	 */
	R::RCursor<R::RString> GetTypesNames(void) const {return(R::RCursor<R::RString>(TypesNames));}

	/**
	 * Get the cost of the Up operations to move a token from a node to another
	 * one.
	 *
	 * In their paper <em>TreeRank: A Similarity Measure for Nearest Neighbor
	 * Searching in Phylogenetic Databases</em>, Wang, Shan, Shasha and Piel
	 * define a similarity measure between two trees based on the different
	 * costs of up operations of the nodes of each tree.
	 * @param u              First node.
	 * @param v              Second node.
	 * @return Cost of the up operations.
	 */
	double GetUpOperationsCost(const GSubject* u,const GSubject* v) const;

	/**
	* Get a user.
	* @param id              Identifier.
	* @param load            If set to true, the user is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the user does not exist,
	*                        return 0, else an exception is generated.
	*/
	GUser* GetUser(size_t id,bool load=true,bool null=false);

	/**
	* Get a user.
	* @param name            Name of the user.
	* @param load            If set to true, the user is eventually loaded into
	*                        memory.
	* @param null            If set to true, if the user does not exist,
	*                        return 0, else an exception is generated.
	*/
	GUser* GetUser(const R::RString name,bool load=true,bool null=false);

	/**
	* Get a cursor over the users used in the system.
	*/
	R::RCursor<GUser> GetUsers(void) const;

	/**
	 * Get the XML structure corresponding to a document. If the document is
	 * not a XML one, a filter is searched.
	 * @param doc            Document.
	 * @param xml            XML struct that will be hold the result.
	 * @param native         Native document.
	 * @param rec             Receiver for the signals.
	 */
	void GetXMLStruct(GDoc* doc,R::RXMLStruct* xml,bool& native,GSlot* rec=0);

	/**
	* Groups the documents into topics. At the end, all the enabled post-topic
	* computing methods are called.
	* @param rec             Receiver of the signals.
	*/
	void GroupDocs(GSlot* rec=0);

	/**
	* Groups the profiles into communities. At the end, all the enabled
	* post-community computing methods are called.
	* @param rec             Receiver of the signals.
	*/
	void GroupProfiles(GSlot* rec=0);

	/**
	 * Initialize the session.
	 */
	void Init(void);

	/**
	 * Insert a class.
	 * @param tclass         Class.
	 * @param parent         Parent class.
	 */
	void Insert(GClass* parent,GClass* tclass);

	/**
	* Insert a community.
	* @param com             Community.
	*/
	void Insert(GCommunity* com);

	/**
	* Insert a concept. In practice, it is a copy of the concept which is
	* inserted.
	* @param concept         Concept to insert.
	* @return Pointer to the concept inserted.
	*/
	GConcept* Insert(const GConcept* concept);

	/**
	* Insert a document. The document is stored in the different containers.
	* @param d               Pointer to the document.
	*/
	void Insert(GDoc* d);

	/**
	* Add a given document to a subject.
	* @param doc            Pointer to the document.
	* @param subjectid      Identifier of the real subject.
	* @param usedid         Identifier of the subject used.
	*/
	void Insert(GDoc* doc,size_t subjectid,size_t usedid);

	/**
	* Insert a new profile.
	* @param p               Pointer to the profile to add.
	*/
	void Insert(GProfile* p);

	/**
	* Add a given profile to a subject.
	* @param profile        Pointer to the profile.
	* @param subjectid      Identifier of the subject.
	*/
	void Insert(GProfile* profile,size_t subjectid);

	/**
	* Insert a subject and attached it to a parent. If the parent is null,
	* the subject is considered as one of the top subject of the tree.
	* @param to              Parent where the subject is to attached.
	* @param subject         Subject to insert.
	*/
	void Insert(GSubject* to,GSubject* subject);

	/**
	* Insert a topic.
	* @param topic           Topic.
	*/
	void Insert(GTopic* topic);

	/**
	* Insert an user.
	* @param user            Pointer to the user to insert.
	*/
	void Insert(GUser* user);

	/**
	* Insert a new concept type.
	* @param id              Identifier of the type.relation type
	* @param name            Name of the type.
	* @param desc            Short description.
	* @param refdocs         Number of documents referenced.
	* @param refprofiles     Number of profiles referenced.
	* @param refgroups       Number of commuinities referenced.
	* @param reftopics       Number of topics referenced.
	* @param refclasses      Number of classes referenced.
	*/
	void InsertConceptType(char id,const R::RString& name,const R::RString& desc,size_t refdocs,size_t refprofiles,size_t refgroups,size_t reftopics,size_t refclasses);

	/**
	* Add a new feedback of a given profile on a given document.
	* @param profid          Identifier of the profile.
	* @param docid           Identifier of the document.
	* @param fdbk            Feedback type.
	* @param date            Date on the last feedback.
	* @param computed        Date on the last computation of the document.
	* @param newone          New feedback in the system?
	*/
	void InsertFdbk(size_t profid,size_t docid,tFdbkType fdbk,R::RDate date,R::RDate computed,bool newone=false);

	/**
	* Insert a new predicate.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	*/
	GPredicate* InsertPredicate(size_t id,const R::RString& name,const R::RString& desc);

	/**
	* Insert a new statement.
	* @param id              Identifier of the statement.
	* @param predicate       Identifier of the predicate.
	* @param xi              Identifier of the \f$x_i\f$.
	* @param xitype          Type of the \f$x_i\f$.
	* @param xj              Identifier of the \f$x_j\f$.
	* @param xjtype          Type of the \f$x_j\f$.
	* @param weight          Weight of the statement.
	*/
	void InsertStatement(size_t id,size_t predicate,size_t xi,tObjType xitype,size_t xj,tObjType xjtype,double weight);

	/**
	* Look if a document is in the parent subject.
	* @param doc            Pointer to the document.
	* @param s              Subject.
	* @return bool
	*/
	bool IsFromParentSubject(GDoc* doc,const GSubject* s) const;

	/**
	* Look if a document is from a given subject.
	* @param doc            Pointer to the document.
	* @param s              Pointer to the subject.
	* @return bool
	*/
	bool IsFromSubject(GDoc* doc,const GSubject* s) const;

	/**
	* Method that load the classes from where they are stored.
	*/
	void LoadClasses(void);

	/**
	* Load the groups. This method is called once by the session.
	*/
	void LoadCommunities(void);

	/**
	* Method that load the documents from where they are stored.
	*/
	void LoadDocs(void);

	/**
	* Load the description of a given object.
	* @param infos           Container that will hold the description. If null,it is created.
	* @param type            Type of the object (otDoc,otProfile,otCommunity,otTopic).
	* @param blockid         Identifier of the block of the object.
	* @param id              Identifier of the object.
	*/
	void LoadInfos(GWeightInfos* &infos,tObjType type,size_t blockid,size_t id);

	/**
	* Find all the references of a given concept for a given objects type.
	* @param concept         Concept to search for.
	* @param refs            Vector that will contain the references. The
	*                        vector is emptied and the objects identifiers
	*                        are ordered.
	* @param type            Type of objects to search for.
	*
	* Here is an example of a search for all the documents containing the
	* English word "connection":
	* @code
	* GConceptType* English(Session->GetConceptType("enTerms",false));         // Get a pointer to the English dictionary.
	* GConcept Word(English,English->GetLang()->GetStemming("connection"));    // Create a concept.
	* GConcept* Concept(Session->InsertConcept(Word));                         // Get the corresponding "real" concept.
	* RNumContainer<size_t,true> Docs(20);                                     // Vector that will contain the documents identifiers.
	* Session->LoadRefs(Concept,Docs,otDoc);                                   // Make the search.
	* for(Docs.Start();!Docs.End();Docs.Next())
	*    cout<<Docs()<<"\t";                                                   // Print the identifiers.
	* cout<<endl;
	* @endcode
	*/
	void LoadRefs(GConcept* concept,R::RNumContainer<size_t,true>& refs,tObjType type);

	/**
	 * Method that load the structure of a document.
	 * @param docstruct      Structure of the document. If null,it is created.
	 * @param blockid        Identifier of the block of the object.
	 * @param id             Identifier of the object.
	 */
	void LoadStruct(GDocStruct* &docstruct,size_t blockid,size_t id);

	/**
	 * Load the subjects.
	 */
	void LoadSubjects(void) const;

	/**
	* Load the topics.
	*/
	void LoadTopics(void);

	/**
	* Load the profiles and the users.
	*/
	void LoadUsers(void);

	/**
	* Verify if the results of the computations must be saved.
	*/
	bool MustSaveResults(void) const {return(SaveResults);}

	/**
	* Send a Query to the Meta engine selected. The pages are researched and ranked.
	* @param keyWords        The set of keywords to form the query.
	*/
	void QueryMetaEngine(R::RContainer<R::RString,true,false>& keyWords);

	/**
	* Re-initialize the session (clear all containers).
	*/
	void ReInit(void);

	/**
	 * Rename a concept.
	 * @param concept        Concept to rename.
	 * @param name           New name.
	 * @return Pointer to the correct concept (may be an existing one).
	 */
	GConcept* RenameConcept(GConcept* concept,const R::RString& name);

	/**
	* Reset the break on a session. This method must be called after a SetBreak
	* to allow the session to do something again.
	*/
	void ResetBreak(void);

	/**
	 * Run a specific tool.
	 * @param name           Name of the tool.
	 * @param slot           Slot.
	 * @param need           If set to true and the tool doesn't exist, it
	 *                       generates an exception.
	 */
	void RunTool(const R::RString& name,GSlot* slot=0,bool need=true);

	/**
	* Save the description of a given object.
	* @param infos           Container that will hold the description.
	* @param type            Type of the object (otDoc,otProfile,otCommunity,otTopic).
	* @param blockid         Identifier of the block of the object (0 means the block will be found).
	* @param id              Identifier of the object.
	*/
	void SaveInfos(const GWeightInfos& infos,tObjType type,size_t& blockid,size_t id);

	/**
	 * Method that save the structure of a document.
	 * @param docstruct      Structure of the document.
	 * @param blockid        Identifier of the block of the object (0 means the
	 *                       block will be found).
	 * @param id             Identifier of the object.
	 */
	void SaveStruct(GDocStruct* docstruct,size_t& blockid,size_t id);

	/**
	* Ask to session to break as soon as possible. The method ResetBreak should
	* be called to allow the session to do something again.
	*/
	void SetBreak(void);

	/**
	* Set The Current RandomSeed.
	* @param rand            Current RandomSeed.
	*/
	void SetCurrentRandom(int rand);

	/**
	 * Set the descriptions of the subjects.
	 * @param type           TYpe of the subjects.
	 */
	void SetDescType(tSubjectDesc type);

	/**
	* Set if the results of the computation must be saved or not.
	* @param save            Save the objects after computation?
	*/
	void SetSaveResults(bool save=true) {SaveResults=save;}

	/**
	 * Test if the subjects are well-formed.
	 */
	void TestSubjects(void);

	/**
	* A profile was updated and the corresponding community must be updated.
	* @param prof            Profile.
	*/
	void UpdateCommunity(GProfile* prof);

	/**
	* A profile was updated and the corresponding community must be updated.
	* @param profid          Identifier of the profile.
	*/
	void UpdateCommunity(size_t profid);

	/**
	* A document was updated and the corresponding feedbacks must be updated.
	* @param docid           Identifier of the document.
	*/
	void UpdateProfiles(size_t docid);

	/**
	 * Update the index of a given type. In practice, the vector is parsed and,
	 * for each concept, the inverted file is update.
	 * @param infos          Vector.
	 * @param type           Type of the object.
	 * @param id             Identifier of the object.
	 * @param add            Object must be added or removed from the index.
	 */
	void UpdateRefs(const GWeightInfos& infos,tObjType type,size_t id,bool add);

	/**
	* A document was updated and the corresponding topic must be updated.
	* @param doc             Document.
	*/
	void UpdateTopic(GDoc* doc);

	/**
	* A document was updated and the corresponding topic must be updated.
	* @param docid           Identifier of the document.
	*/
	void UpdateTopic(size_t docid);

public:

	/**
	* Destruct the session.
	*/
	virtual ~GSession(void);

	friend class GDebugObject;
	friend class GSimulator;
	friend class GPlugIn;
	friend class GPlugInList;
	friend class GPlugInManager;
	friend class GConceptType;
	friend class GGALILEIApp;
	friend class GSubject;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
