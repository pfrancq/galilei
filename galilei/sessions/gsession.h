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
#include <sessions/galilei.h>
#include <profiles/gusers.h>
#include <docs/gdocs.h>
#include <groups/ggroups.h>
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
class GSession : public GDocs, public GUsers, public GGroups
{
protected:
	/**
	* Pointer to a tree of subject
	*/
	GSubjects* Subjects;

	/**
	* Name of the current database.
	*/
	R::RString DbName;

	/**
	* All the judgements.
	*/
	R::RContainer<GProfDoc,unsigned,true,true> Fdbks;

	/**
	* Pointer to the languages.
	*/
	GLangManager* Langs;

	/**
	* URL Manager used by this session.
	*/
	GFilterManager* URLMng;

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
	*/
	void Connect(GLangManager* langs,GFilterManager* umng, GDocAnalyseManager* dmng, GProfileCalcManager* pmng, GGroupingManager* gmng, GGroupCalcManager* gcmng,
		GStatsCalcManager* smng, GPostDocManager* pdmng, GPostGroupManager* pgmng) throw(bad_alloc,GException);

	/**
	* Connect the session to the linking manager.
	* @param lmng           Linking Manager.
	*/
	void PostConnect(GLinkCalcManager* lmng) throw(bad_alloc,GException);

	/**
	* Get the documents' analyser.
	* @returns Pointer to a GDocAnalyse class.
	*/
	GDocAnalyseManager* GetDocAnalyseMng(void) const {return(DocAnalyseMng);}

	/**
	* Get the languages.
	* @returns Pointer to a GLangManager class.
	*/
	GLangManager* GetLangs(void) const {return(Langs);}

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
	GSubjects* GetSubjects(void) {return(Subjects);}

	/**
	* Get the parameters of the session.
	*/
	GSessionParams* GetSessionParams(void) const {return(SessParams);}

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
	*/
	void AnalyseAssociation(void) throw(GException);

	/**
	* Remove concepts from the whole set of documents.
	*/
	void RemoveAssociation(void) throw(GException);

	/**
	* Analyse all the necessary documents.
	* @param rec        Receiver for the signals.
	* @param modified   Recompute only modified elements or all.
	*/
	void AnalyseDocs(GSlot* rec=0,bool modified=true) throw(GException);

	/**
	* Assign an identifier to a new data of a given dictionary.
	* @param data           Data.
	* @param dict           Dictionary.
	*/
	virtual void AssignId(GData* data,const GDict* dict)=0;

	/**
	* Assign an identifier to a new grou.
	* @param grp            Group.
	*/
	virtual void AssignId(GGroup* data);

	/**
	* Loading a dictionary/stoplist.
	* @param dic            Pointer to the dictionary.
	* @param lang           Languague.
	* @param stop           Is it a stop list.
	*/
	virtual void LoadDic(GDict* &dic,GLang* lang,bool s) throw(bad_alloc,GException)=0;

	/**
	* Load a specific word from a dictionary.
	* @param id             Idenfificator of the word.
	* @param code           Code of the languague.
	*/
	virtual const char* LoadWord(unsigned int id,const char* code)=0;

	/**
	* Load a specific wordlist from a dictionary.
	* @param w              WordList to load.
	* @param code           Code of the languague.
	*/
	virtual void LoadWordList(GWordList* w,GLang* lang) throw(bad_alloc,GException)=0;

	/**
	* Save a specific wordlist in the database.
	* @param dic            Pointer to the dictionary.
	* @param w              WordList to save.
	*/
	virtual void SaveWordList(GDict* dic,GWordList* w) throw(bad_alloc,GException)=0;

	/**
	* Delete all the wordlists from the database.
	* @param dic            Pointer to the dictionary.
	*/
	virtual void DeleteWordList(GDict* dic) throw(bad_alloc,GException)=0;

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
	* Set if the Inverse Frequency Factor should be used for the similarities
	* between (sub)profiles.
	* @param iff             Use Inverse Frequency Factor.
	*/
	void UseIFFProfs(bool iff);

	/**
	* Return the similarity between two subProfiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetSimProf(const GSubProfile* sub1,const GSubProfile* sub2) throw(GException);

	/**
	* Update the state of agreement and disagreement ratios between the
	* subprofiles.
	*/
	void UpdateBehaviours(void) throw(bad_alloc);

	/**
	* Return the disagreement ratio between two subprofiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetDisagreementRatio(GSubProfile* sub1,GSubProfile* sub2) throw(GException);

	/**
	* Return the agreement ratio between two subprofiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2) throw(GException);

	/**
	* Get the minimum of similarityof the subprofiles, needed by clusteirng
	* algorithms.
	* @param lang            Language.
	* @param deviationrate   factor of the standart deviation.
	*/
	double GetMinimumOfSimilarity(GLang* lang, double deviationrate=1.5) throw(GException);

	/**
	* Set if the Inverse Frequency Factor should be used for the similarities
	* between documents and (sub)profiles.
	* @param iff             Use Inverse Frequency Factor.
	*/
	void UseIFFDocProf(bool iff);

	/**
	* Return the similarity between a document and a subProfiles.
	* @param doc           The Pointer to the document.
	* @param sub2          The Pointer to the subprofile.
	*/
	double GetSimDocProf(const GDoc* doc,const GSubProfile* sub) throw(GException);
       
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
	* Apply the postgroup treatment.
	*/
	void CalcPostGroup(void) throw(GException);

	/**
	* Make the groups.
	* @param rec            Receiver of the signals.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	*/
	void GroupingProfiles(GSlot* rec,bool modified,bool save)  throw(GException);

	/**
	* Load the Feedbacks.
	*/
	virtual void LoadFdbks(void) throw(bad_alloc,GException)=0;

	/**
	* Load the SubjectTree.
	*/
	virtual void LoadSubjectTree()=0;

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
	void InsertFdbk(GProfile* p,GDoc* d,tDocAssessment j,R::RDate& date) throw(bad_alloc);

	/**
	* Insert a new Feedback.
	* @param p          Pointer to the profile.
	* @param d          Pointer to the document.
	* @param j          Feedback.
	* @param date       String representing the date on the last feedback.
	*/
	void InsertFdbk(GProfile* p,GDoc* d,tDocAssessment j,const char* date) throw(bad_alloc);

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
	virtual void SaveMixedGroups(GGroups* mixedgroups,unsigned int id, bool historic=false)=0;

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
	virtual void SaveIdealGroupment(GGroups* idealgroup)=0;

	/**
	* Save the feedbaks
	*/
	virtual void SaveFdbks(void)=0;

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
	* Create a dummy table to store different kid of
	* data.
	* @ param name              name of the dummy object.
	*/
	virtual void CreateDummy(const char* name)=0;

	/**
	* Add a dummy entry into a dummy table;
	* @param name               name of the dummy table.
	 * @param id                id of the dymmy entry.
	 * @param desc              description of the dymmy entry.
	*/
	virtual void AddDummyEntry(const char* name, unsigned int id, const char* desc, unsigned int parentid)=0;

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
