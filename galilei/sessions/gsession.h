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
#include <profiles/gusers.h>
#include <profiles/gsubprofile.h>
#include <docs/gdocs.h>
#include <groups/ggroupsmng.h>
#include <docs/glinkcalc.h>
#include <postgroups/gpostgroup.h>
#include <docs/gpostdoc.h>
#include <sessions/gparams.h>



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
class GSession : public GDocs, public GUsers, public GGroupsMng
{
protected:
	/**
	* Pointer to a tree of subject
	*/
	GSubjectTree* Subjects;

	/**
	* Name of the current database.
	*/
	R::RString DbName;

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
	* Pointer to the languages.
	*/
	GLangs* Langs;

	/**
	* URL Manager used by this session.
	*/
	GURLManager* URLMng;

	/**
	* Profile Calc Manager used by this session.
	*/
	GProfileCalcManager* ProfilingMng;

	/**
	* Grouping Manager used by this session.
	*/
	GGroupingManager* GroupingMng;

	/**
	* Group Computing Manager used by this session.
	*/
	GGroupCalcManager* GroupCalcMng;

	/**
	* Statistics Manager used by this session.
	*/
	GStatsCalcManager* StatsCalcMng;

	/**
	* Linking Manager used by this session.
	*/
	GLinkCalcManager* LinkCalcMng;

	/**
	* PostGroup Manager used by this session.
	*/
	GPostGroupManager* PostGroupMng;


	/**
	* Words clustering Manager used by this session.
	*/
	GPostDocManager* PostDocMng;

	/**
	* Analyser used for the document.
	*/
	GDocAnalyseManager* DocAnalyseMng;

	/**
	* Similarity between the  profiles.
	*/
	GProfilesSims* ProfilesSims;

	/**
	* behaviour inter profiles.
	*/
	GProfilesBehaviours* ProfilesBehaviours;

	/**
	*  Similarity between documents and profiles for each langage.
	*/
	GDocProfSims* DocProfSims;

	/**
	* State of the groups.
	*/
	bool bGroups;

	/**
	* State of the User's feedbacks.
	*/
	bool bFdbks;

	/**
	* The current seek for this session.
	*/
	int CurrentRandom;

	/**
	* Random number generator
	*/
	R::RRandom* Random;

	/**
	* Session Parameters
	*/
	GSessionParams* SessParams;

public:

	/**
	* Constructor.
	* @param d              Number of documents.
	* @param u              Number of users.
	* @param p              Number of profiles.
	* @param f              Number of feedbacks.
	* @param g              Number of groups.
	* @param test           Test mode.
	*/
	GSession(unsigned int d,unsigned int u,unsigned int p,unsigned int f,unsigned int g,
		GSessionParams* sessparams,bool tests) throw(bad_alloc,GException);

	/**
	* Connect the session to the different managers.
	* @param langs          Languages.
	* @param umng           URL Manager.
	* @param dmng           Document Analysis Manager.
	* @param pmng           Profiling Manager.
	* @param gmng           Grouping Manager.
	* @param gcmng          Group Computing Manager.
	* @param smng           Statistical Manager.
	* @param lmng           Linking Manager.
	*/
	void Connect(GLangs* langs,GURLManager* umng, GDocAnalyseManager* dmng, GProfileCalcManager* pmng, GGroupingManager* gmng, GGroupCalcManager* gcmng,
		GStatsCalcManager* smng, GLinkCalcManager* lmng, GPostDocManager* pdmng, GPostGroupManager* pgmng) throw(bad_alloc,GException);

	/**
	* Get the documents' analyser.
	* @returns Pointer to a GDocAnalyse class.
	*/
	GDocAnalyseManager* GetDocAnalyseMng(void) const {return(DocAnalyseMng);}

	/**
	* Get the languages.
	* @returns Pointer to a GLangs class.
	*/
	GLangs* GetLangs(void) const {return(Langs);}

	/**
	* Get the profiling manager used by this session.
	*/
	GProfileCalcManager* GetProfilingMng(void) {return(ProfilingMng);}

	/**
	* Get the grouping manager used by this session.
	*/
	GGroupingManager* GetGroupingMng(void) {return(GroupingMng);}

	/**
	* Get the link computing manager used by this session.
	*/
	GLinkCalcManager* GetLinkCalcMng(void) {return(LinkCalcMng);}

	/**
	* Get the PostGroup manager used by this session.
	*/
	GPostGroupManager* GetPostGroupMng(void) {return(PostGroupMng);}

	/**
	* Get the group computing manager used by this session.
	*/
	GGroupCalcManager* GetGroupCalcMng(void) {return(GroupCalcMng);}

	/**
	* Get the statistics manager used by this session.
	*/
	GStatsCalcManager* GetStatsCalcMng(void) {return(StatsCalcMng);}

	/**
	* Return a pointer to a tree of subjects
	*/
	GSubjectTree* GetSubjects(void) {return(Subjects);}

	/**
	* Get the current description method.
	* @returns Pointer to a GSubProfileDesc class.
	*/
	GSubProfileDesc* GetCurrentProfileDesc(void) {return(SubProfileDesc);}

	/**
	* Get a cursor to the link description methods registered.
	* @return Return a GLinkCalcCursor.
	*/
	GFactoryLinkCalcCursor& GetLinkCalcsCursor(void);

	/**
	* Get a cursor to the words clustering description methods registered.
	* @return Return a GPostDocCursor.
	*/
	GFactoryPostDocCursor& GetPostDocsCursor(void);

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
	* Remove concepts from the whole set of documents.
	*/
	void RemoveAssociation();

	/**
	* Analyse all the necessary documents.
	* @param rec        Receiver for the signals.
	* @param modified   Recompute only modified elements or all.
	*/
	void AnalyseDocs(GSlot* rec=0,bool modified=true) throw(GException);

	/**
	* Return the identifier of a new word of a dictionnary.
	* @param word           Word to find.
	* @param dict           Dictionnary.
	*/
	virtual unsigned int GetDicNextId(const char* word,const GDict* dict)=0;

	/**
	* Loading a dictionnary/stoplist.
	* @param dic            Pointer to the dictionary.
	* @param lang           Languague.
	* @param stop           Is it a stop list.
	*/
	virtual void LoadDic(GDict* &dic,GLang* lang,bool s) throw(bad_alloc,GException)=0;

	/**
	* Load a specific word from a dictionnary.
	* @param id             Idenfificator of the word.
	* @param code           Code of the languague.
	*/
	virtual const char* LoadWord(unsigned int id,const char* code)=0;

	/**
	* Load the Users.
	*/
	void InitUsers(bool wg,bool w) throw(bad_alloc,GException);

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
	* Return the disagreement ratio between two subProfiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetDisagreementRatio(GSubProfile* sub1,GSubProfile* sub2);

	/**
	* Return the agreement ratio between two subProfiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2);

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
	void InsertFdbk(GProfile* p,GDoc* d,tDocJudgement j,R::RDate date) throw(bad_alloc);

public:

	/**
	* Verify if the groups are loaded.
	* @returns true, if loaded.
	*/
	bool IsGroupsLoad(void) const {return(bGroups);}

	/**
	* Load the Groups.
	*/
	void InitGroups(bool wg,bool w) throw(bad_alloc,GException);

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
	* Return the name of the current database.
	*/
	virtual R::RString GetDbName()=0;
	
	/**
	* Get a cursor over the filters of the system.
	*/
	GFactoryFilterCursor& GetFiltersCursor(void);

	/**
	* Get the filter for a specific mime type.
	* @param mime           Name of the mimetype.
	* @return Pointer to a GMIMEFilter.
	*/
	const char* GetMIMEType(const char* mime) const;

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
	* Delete clusters of words.
	* @param dict           Dictionnary.
	*/
	virtual void DeleteWordsGroups(GDict* dict)=0;

	/**
	* Save clusters of words.
	* @param dict           Dictionnary.
	*/
	virtual void SaveWordsGroups(GDict* dict)=0;

	/**
	* Save clusters of words in a document.
	* @param doc            Document to save.
	* @param n              Id of the first cluster of words.
	*/
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
	* @returns Pointer to RMath::RRandom;
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


//-----------------------------------------------------------------------------
/**
* The GSessionParams represents  the parameter used to create the session.
* @short GSession Parameters.
*/
class GSessionParams : public GParams
{

public:

	/**
	* constuctor
	*/
	GSessionParams(void);

	/**
	* Destructor
	*/
	~GSessionParams(void) {};

};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
