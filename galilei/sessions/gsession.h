/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).
		Valery Vandaele (vavdaele@ulb.ac.be).

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
#ifndef GSessionH
#define GSessionH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <langs/glangs.h>
#include <profiles/gusers.h>
#include <profiles/gsubprofile.h>
#include <docs/gdocs.h>
#include <groups/ggroupir.h>
#include <groups/ggroupsmng.h>
#include <groups/gsubjecttree.h>
#include <docs/glinkcalc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GSession provides a representation for a GALILEI session. The way the
* data are stored and loaded is dedicated to a child class of GSession through
* a set of pure virtual methods defined.
* @author GALILEI Team
* @short Generic Session.
*/
class GSession : public GLangs, public GDocs, public GUsers, public GGroupsMng
{
protected:

	/**
	* Ideal Groups handled by the system.
	*/
	R::RContainer<GGroups,unsigned int,true,true>* IdealGroups;

	/**
	* Pointer to the ideal Docs
	*/
	R::RContainer<GGroupsEvaluate,unsigned int,false,false>* IdealDocs;

	/**
	* Pointer to a tree of subject
	*/
	GSubjectTree Subjects;

	/**
	* All the judgements.
	*/
	R::RContainer<GProfDoc,unsigned,true,true> Fdbks;

	/**
	* Container of computing method for the profiles.
	*/
	R::RContainer<GSubProfileDesc,unsigned int,true,true>* SubProfileDescs;

	/**
	* Current method used to computed the profiles.
	*/
	GSubProfileDesc* SubProfileDesc;

	/**
	* Container of computing method for the profiles.
	*/
	R::RContainer<GProfileCalc,unsigned int,true,true>* ProfileCalcs;

	/**
	* Current method used to computed the profiles.
	*/
	GProfileCalc* ProfileCalc;

	/**
	* Container of grouping method for the profiles.
	*/
	R::RContainer<GGrouping,R::tId,true,true>* Groupings;

	/**
	* Current grouping method used.
	*/
	GGrouping* Grouping;

	/**
	* Container of group description method.
	*/
	R::RContainer<GGroupCalc,R::tId,true,true>* GroupCalcs;

	/**
	* Current group description method used.
	*/
	GGroupCalc* GroupCalc;

	/**
	* Container of link description method.
	*/
	R::RContainer<GLinkCalc,unsigned int,true,true>* LinkCalcs;

	/**
	* Current link description method used.
	*/
	GLinkCalc* LinkCalc;

	/**
	* URL Manager used by this session.
	*/
	GURLManager* Mng;

	/**
	* Analyser used for the document.
	*/
	GDocAnalyse* DocAnalyse;

	/**
	* Similarity between the  profiles.    
	*/
	GProfilesSims* ProfilesSims;

	/**
	* behaviour inter profiles.
	*/
	GProfilesBehaviours* ProfilesBehaviours;

	/**
	*  Similarity between documents and  profiles for each langage.
	*/
	GDocProfSims* DocProfSims ;

	/**
	* State of the groups.
	*/
	bool bGroups;

	/**
	* State of the User's feedbacks.
	*/
	bool bFdbks;

	/**
	* Options about the documents.
	*/
	GDocOptions* DocOptions;

	/**
	* The current seek for this session.
	*/
	int CurrentRandom;

	/**
	* Random number generator
	*/
	R::RRandom* Random;

public:

	/**
	* Constructor.
	* @param d              Number of documents.
	* @param u              Number of users.
	* @param p              Number of profiles.
	* @param f              Number of feedbacks.
	* @param g              Number of groups.
	* @param mng            URL Manager.
	*/
	GSession(unsigned int d,unsigned int u,unsigned int p,unsigned int f,unsigned int g,GURLManager* mng) throw(bad_alloc,GException);

	/**
	* Get the documents' analyser.
	* @returns Pointer to a GDocAnalyse class.
	*/
	GDocAnalyse* GetDocAnalyse(void) const {return(DocAnalyse);}

	/**
	* Get a pointer to the document options.
	* @returns Pointer to GDocOptions.
	*/
	GDocOptions* GetDocOptions(void) {return(DocOptions);}

	/**
	* Get the ideal groups of the session
	* @return Pointer to R::RContainer<GGroups,unsigned int,true,true>.
	*/
	R::RContainer<GGroups,unsigned int,true,true>* GetIdealGroups(void);

	/**
	* Get a cursor over the ideal groups of the system.
	*/
	GGroupsCursor& GetIdealGroupsCursor(void);

	/**
	* Get a pointer to the ideal docs
	*/
	R::RContainer<GGroupsEvaluate,unsigned int,false,false>* GetIdealDocs(void);

	/**
	* Get a cursor over the ideal documents of the system.
	*/
	GGroupsEvaluateCursor& GetIdealDocsCursor(void);

	/**
	* Return a pointer to a tree of subjects
	*/
	GSubjectTree* GetSubjects() {return(&Subjects);}

	/**
	* Register a description method for the profiles.
	* @param com            Description method to register.
	*/
	void RegisterProfileDesc(GSubProfileDesc* grp) throw(bad_alloc);

	/**
	* Set the current description method.
	* @param name           Name of the description method.
	*/
	void SetCurrentProfileDesc(const char* name) throw(GException);

	/**
	* Get the current description method.
	* @returns Pointer to a GSubProfileDesc class.
	*/
	GSubProfileDesc* GetCurrentProfileDesc(void) {return(SubProfileDesc);}

	/**
	* Get a cursor to the description methods registered.
	* @return Return a GProfileDescCursor.
	*/
	GSubProfileDescCursor& GetProfileDescsCursor(void);

	/**
	* Register a computing method for the profiles.
	* @param com            Grouping method to register.
	*/
	void RegisterComputingMethod(GProfileCalc* grp) throw(bad_alloc);

	/**
	* Set the current computing method.
	* @param name           Name of the computing method.
	*/
	void SetCurrentComputingMethod(const char* name) throw(GException);

	/**
	* Set the settings to the current computing method.
	* @param s              Settings of the current computing method.
	*/
	void SetCurrentComputingMethodSettings(const char* s) throw(GException);

	/**
	* Get the current computing method.
	* @returns Pointer to a GProfileCalc class.
	*/
	GProfileCalc* GetCurrentComputingMethod(void) {return(ProfileCalc);}

	/**
	* Get a cursor to the computing methods registered.
	* @return Return a GComputingCursor.
	*/
	GProfileCalcCursor& GetComputingsCursor(void);

	/**
	* Register a grouping method for the profiles.
	* @param grp            Grouping method to register.
	*/
	void RegisterGroupingMethod(GGrouping* grp) throw(bad_alloc);

	/**
	* Set the current grouping method.
	* @param name           Name of the grouping method.
	*/
	void SetCurrentGroupingMethod(const char* name) throw(GException);

	/**
	* Set the settings for the current grouping method.
	* @param s              Settings of the current grouping method.
	*/
	void SetCurrentGroupingMethodSettings(const char* s) throw(GException);

	/**
	* Get the settings of a given grouping method.
	* @param n              Name of the grouping method.
	* @returns C String representing the settings of the given grouping method.
	*/
	const char* GetGroupingMethodSettings(const char* n) throw(GException);

	/**
	* Get the current grouping method.
	* @returns Pointer to a GGrouping class.
	*/
	GGrouping* GetCurrentGroupingMethod(void) {return(Grouping);}

	/**
	* Get a cursor to the grouping methods registered.
	* @return Return a GGoupingCursor.
	*/
	GGroupingCursor& GetGroupingsCursor(void);

	/**
	* Register a group description method.
	* @param grp            Description method to register.
	*/
	void RegisterGroupCalcMethod(GGroupCalc* grp) throw(bad_alloc);

	/**
	* Set the current group description method.
	* @param name           Name of the group description method.
	*/
	void SetCurrentGroupCalcMethod(const char* name) throw(GException);

	/**
	* Set the settings for the current group description method.
	* @param s              Settings of the current group description method.
	*/
	void SetCurrentGroupCalcMethodSettings(const char* s) throw(GException);

	/**
	* Get the settings of a given group description method.
	* @param n              Name of the group description method.
	* @returns C String representing the settings of the given group
	* description method.
	*/
	const char* GetGroupCalcMethodSettings(const char* n) throw(GException);

	/**
	* Get the current group description method.
	* @returns Pointer to a GGroupCalc class.
	*/
	GGroupCalc* GetCurrentGroupCalcMethod(void) {return(GroupCalc);}

	/**
	* Get a cursor to the group description methods registered.
	* @return Return a GGroupCalcCursor.
	*/
	GGroupCalcCursor& GetGroupCalcsCursor(void);

	/**
	* Register a link description method.
	* @param lnk            Description method to register.
	*/
	void RegisterLinkCalcMethod(GLinkCalc* lnk) throw(bad_alloc);

	/**
	* Set the current link description method.
	* @param name           Name of the link description method.
	*/
	void SetCurrentLinkCalcMethod(const char* name) throw(GException);

	/**
	* Set the settings for the current link description method.
	* @param s              Settings of the current link description method.
	*/
	void SetCurrentLinkCalcMethodSettings(const char* s) throw(GException);

	/**
	* Get the settings of a given link description method.
	* @param n              Name of the link description method.
	* @returns C String representing the settings of the given link
	* description method.
	*/
	const char* GetLinkCalcMethodSettings(const char* n) throw(GException);

	/**
	* Get the current link description method.
	* @returns Pointer to a GLinkCalc class.
	*/
	GLinkCalc* GetCurrentLinkCalcMethod(void) {return(LinkCalc);}

	/**
	* Get a cursor to the link description methods registered.
	* @return Return a GLinkCalcCursor.
	*/
	GLinkCalcCursor& GetLinkCalcsCursor(void);


	/**
	* Create a XML structure of the content of a document. The structure
	* created has to be desallocate by the caller.
	* @param doc        Document to analyse.
	*/
	GDocXML* CreateDocXML(GDoc* doc) throw(GException);

	/**
	* Create concept in the whole set of documents.
	* @param save        Enable de concept saving.
	*/
	void AnalyseAssociation(bool save);

	/**
	* Analyse all the necessary documents.
	* @param rec        Receiver for the signals.
	* @param modified   Recompute only modified elements or all.
	*/
	void AnalyseDocs(GSlot* rec=0,bool modified=true) throw(GException);

	/**
	* Load the Users.
	*/
	void InitUsers(void) throw(bad_alloc,GException);

	/**
	* Create a new user in the system.
	* @param usr            User.
	* @param pwd            Password.
	* @param name           Name.
	* @param email          EMail.
	* @param title          Title.
	* @param org            Organisation.
	* @param addr1          Address (Part 1).
	* @param addr2          Address (Part 2).
	* @param city           City.
	* @param country        Country.
	*/
	GUser* NewUser(const char* usr,const char* pwd,const char* name,const char* email,
	                  const char* title,const char* org,const char* addr1,
	                  const char* addr2,const char* city,const char* country) throw(bad_alloc);
                  
	/**
	* Initialise the table of similarity between the profiles
	*/
	void InitProfilesSims(void);
	                  
	/**
	* Update the state of similarity between the subProfiles for a given language.
	* @param global         use the Global/Locale similarity
	* @param lang           The lang of the subprofiles
	*/
	void ChangeProfilesSimState(bool global,GLang* lang)throw(bad_alloc);

	/**
	* Update the state of similarity between the subProfiles for all language in the system.
	* @param global         use the Global/Locale similarity
	* @param lang           The lang of the subprofiles
	*/
	void ChangeAllProfilesSimState(bool global)throw(bad_alloc);

	/**
	* Return the similarity between two subProfiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetSimProf(const GSubProfile* sub1,const GSubProfile* sub2);

	/**
	* Initialise the table of similarity between the profiles
	*/
	void InitProfilesBehaviours(void);

	/**
	* Update the state of similarity between the subProfiles for a given language.
	* @param global         use the Global/Locale similarity
	* @param lang           The lang of the subprofiles
	*/
	void ChangeProfilesBehaviourState(GLang* lang)throw(bad_alloc);

	/**
	* Update the state of similarity between the subProfiles for all language in the system.
	*/
	void ChangeAllProfilesBehaviourState(void) throw(bad_alloc);

	/**
	* Return the similarity between two subProfiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	* @param threshold       minimum of common documents
	*/
	double GetDisAgreementRatio(GSubProfile* sub1,GSubProfile* sub2,unsigned int threshold=0);

	/**
	* Return the similarity between two subProfiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	* @param threshold       minimum of common documents
	*/
	double GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2,unsigned int threshold=0);

	/**
	* return the minim of similarityof the subprofiles, needed by clusteirng algorithms.
	* @param subprofiles    set of subprofiles on which mean minsim must be
	*  caculated
	* @param deviationrate  factor of the standart deviation.
	*/
	double GetMinimumOfSimilarity(R::RContainer<GSubProfile,unsigned int,false,true>* subprofiles, double deviationrate=1.5);

	/**
	* Initialise the table of similarity between documents and profiles
	*/
	void InitDocProfSims(void);

	/**
	* Update the state of similarity between a profile and a subProfiles for a given language.
	* @param global         use the Global/Locale similarity
	* @param lang           The lang of the document/subprofiles
	*/
	void ChangeDocProfState(bool global,GLang* lang)throw(bad_alloc);

	/**
	* Update the state of similarity between a profile and a subProfiles for all the langages contained in the system.
	* @param global         use the Global/Locale similarity1
	*/
	void ChangeAllDocProfState(bool global)throw(bad_alloc);

	/**
	* Return the similarity between a document and a subProfiles for a given language.
	* @param l              The language used
	* @param id_doc            Identificator of the document
	* @param id_sub            Identificator of the subprofile
	*/
	double GetSimDocProf(GLang* l,unsigned int id_doc, unsigned int id_sub);

	/**
	* Return the similarity between a document and a subProfiles.
	* @param doc           The Pointer to the document.
	* @param sub2          The Pointer to the subprofile.
	*/
	double GetSimDocProf(const GDoc* doc,const GSubProfile* sub);

	/**
	* Initialise the graph of links needed by the linkCalc Algo.
	*/
	void InitLinks();
	        
	/**
	* Compute all the necessary profiles.
	* @param rec            Receiver for the signals.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	*/
	void CalcProfiles(GSlot* rec,bool modified,bool save) throw(GException);

	/**
	* Compute a profile.
	* @param prof       Pointer to the profile to compute.
	*/
	void CalcProfile(GProfile* prof) throw(GException);

	/**
	* Make the groups.
	* @param rec            Receiver of the signals.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	*/
	void GroupingProfiles(GSlot* rec,bool modified,bool save)  throw(GException);

protected:

	/**
	* Load the Feedbacks.
	*/
	virtual void LoadFdbks(void) throw(bad_alloc,GException)=0;

public:

	/**
	* Load the SubjectTree.
	*/
	virtual void LoadSubjectTree()=0;

	/**
	* Verify if the feedback are loaded.
	* @returns true, if loaded.
	*/
	bool IsFdbksLoad(void) const {return(bFdbks);}

	/**
	* Load the Users feedback.
	*/
	void InitFdbks(void) throw(bad_alloc,GException);

	/**
	* Get a Cursor on the feedbacks.
	* @return GProfDocCursor.
	*/
	GProfDocCursor& GetProfDocCursor(void);

	/**
	* Clear all the feedbacks.
	*/
	void ClearFdbks(void);

	/**
	* Insert a new Feedback.
	* @param p          Pointer to the profile.
	* @param d          Pointer to the document.
	* @param j          Feedback.
	* @param date       Date on the last feedback.
	*/
	void InsertFdbk(GProfile* p,GDoc* d,tDocJudgement j,const char* date) throw(bad_alloc);

public:

	/**
	* Verify if the groups are loaded.
	* @returns true, if loaded.
	*/
	bool IsGroupsLoad(void) const {return(bGroups);}

	/**
	* Load the Groups.
	*/
	void InitGroups(void) throw(bad_alloc,GException);

	/**
	* Copy the ideal groupment in the current one.
	*/
	void CopyIdealGroups(void) throw(bad_alloc,GException);

	/**
	* Save a group, i.e. save all the information of the subprofiles
	* concerning the groupement.
	* @param grp            Group to save.
	*/
	virtual void Save(GGroup* grp) throw(GException);

	/**
	* Save the groups of the session
	* @param mixedgroups    groups to save
	* @param id             Identificator.
	* @param historic       if false,  groups will be saved in 'tempchromo', if true in 'historic'
	*/
	virtual void SaveMixedGroups(R::RContainer<GGroups,unsigned int,true,true> * mixedgroups,unsigned int id, bool historic=false)=0;

	/**
	* Save profiles in history
	* @param historicid     id of the historic.
	*/
	virtual void SaveHistoricProfiles(unsigned int historicid)=0;
	
	/**
	* Get a cursor over the filters of the system.
	*/
	GFilterCursor& GetFiltersCursor(void);

	/**
	* Get the filter for a specific mime type.
	* @param mime           Name of the mimetype.
	* @return Pointer to a GMIMEFilter.
	*/
	GMIMEFilter* GetMIMEType(const char* mime) const;

	/**
	* Execute a sequence of steps needed to construct data. Typically, this
	* can be a SQL file.
	* @param filename       Name of the file.
	*/
	virtual void ExecuteData(const char* filename) throw(GException)=0;

	/**
	* Load the ideal groupment.
	* @param idealgroup   The ideal container of group	
	*/
	virtual void LoadIdealGroupment()=0;

	/**
	* Save the ideal groupment
	* @param idealgroup   The ideal container of group	
	*/
	virtual void SaveIdealGroupment(R::RContainer<GGroups,unsigned int,true,true>* idealgroup)=0;

	/**
	* Save the feedbaks
	*/
	virtual void SaveFdbks(void)=0;

	/**
	* Clear all the chromosomes stored.
	*/
	virtual void ClearStoredChromos(void)=0;

	/**
	* Save a chromosome.
	* @param chromo         The chromosome to save.
	* @param id             An id to find the chromosome after saving.
	* @param objs           Objects.
	* @param lang           The lang of the subprofiles in the chromosome. 
	*/
	virtual void SaveChromo(GChromoIR* chromo,unsigned int id,R::RObjs<GObjIR>* objs)=0;

	/**
	* Load a instance of chromosome for statisical use only.
	* @param lang           The lang of the subprofiles in the chromosome.
	* @param objs           Objects.
	* @param p              Parameters.
	* @return pointer to a Ginstir
	*/
	virtual GInstIR* LoadInstIR(GLang* lang,R::RObjs<GObjIR>* objs,GIRParams* p)=0;

	/**
	* Save The Documents Simylarities into the database.
	*/
	virtual void SaveDocSim(void)=0;

	virtual void SaveWordsGroups(GDict* dict)=0;

	virtual void SaveUpDatedDoc(GDoc* doc,unsigned n) throw(GException)=0;

	/**
	* Run a "program" for this session.
	* @param filename       Name of the file.
	*/
	void RunPrg(GSlot* rec,const char* filename) throw(GException);

	/**
	* Run the filter on the docments
	* @param nbdocs         The number of docs where the word must be present
	* @param nboccurs       The minimal occurs for the word in a document
	*/
	void DocsFilter(int nbdocs,int nboccurs) throw(GException);

	/**
	* Get The Current RandomSeed.
	* @return The current RandomSeed.
	*/
	int GetCurrentRandom(void) {return(CurrentRandom);};

	/**
	* Set The Current RandomSeed.
	* @param rand             The current RandomSeed.
	*/
	void SetCurrentRandom(int rand);

	/**
	* Get The Current Random Value.
	* @param max              The max Random Value.
	* @return The current Random value * max.
	*/
	int GetCurrentRandomValue(unsigned int max);

	/**
	* Get the random number generator.
	* @returns Pointer to R::RRandom;
	*/
	R::RRandom* GetRandom(void) const {return(Random);}

	/**
	* load the historic groups.
	*/
	R::RContainer<GGroupsHistory, unsigned int, false,true>* LoadHistoricGroups (R::RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang,unsigned int mingen, unsigned int maxgen);

	/*
	* load an historic groups.
	*/
	virtual GGroupsHistory* LoadAnHistoricGroups(R::RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang, unsigned int historicaID)=0;

	/**
	*returns the number of historic groups stored in database.
	*/
	virtual unsigned int GetHistorySize(void) =0;

	/**
	* re-init the session (clear all containers).
	* @param Save           must the reinit be saved?
	*/
	void ReInit(bool Save);

	/**
	* Count the number of rows of a table.
	* @param tbl            Table to analyse.
	* @returns Number of rows.
	*/
	virtual unsigned int GetCount(const char* tbl)=0;
	
	/**
	* Destructor.
	*/
	virtual ~GSession(void) throw(GException);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
