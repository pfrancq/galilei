/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).
		Valery Vandaele (vavdaele@ulb.ac.be).

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
#ifndef GSessionH
#define GSessionH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <profiles/gusers.h>
#include <docs/gdocs.h>
#include <groups/ggroups.h>
#include <sessions/gparams.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GSession provides a representation for a generic GALILEI session.
* @author GALILEI Team
* @short Generic Session.
*/
class GSession : public GDocs, public GUsers, public GGroups
{
protected:

	/**
	* Pointer to a tree of subjects used for testing purposes.
	*/
	GSubjects* Subjects;

	/**
	* All the assessments.
	*/
	R::RContainer<GProfDoc,unsigned,true,true> Fdbks;

	/**
	* Languages Manager.
	*/
	GLangManager* Langs;

	/**
	* URL Manager.
	*/
	GFilterManager* URLMng;

	/**
	* SubProfile Computing Methods Manager.
	*/
	GProfileCalcManager* ProfilingMng;

	/**
	* Grouping Methods Manager.
	*/
	GGroupingManager* GroupingMng;

	/**
	* Group Computing Methods Manager.
	*/
	GGroupCalcManager* GroupCalcMng;

	/**
	* Statistics Methods Manager.
	*/
	GStatsCalcManager* StatsCalcMng;

	/**
	* Link Methods Manager.
	*/
	GLinkCalcManager* LinkCalcMng;

	/**
	* Post-Group Computing Methods Manager.
	*/
	GPostGroupManager* PostGroupMng;

	/**
	* Documents Post-Analysis Methods Manager.
	*/
	GPostDocManager* PostDocMng;

	/**
	* Document Analysis Methods Manager.
	*/
	GDocAnalyseManager* DocAnalyseMng;

	/**
	* Hitoric of grouping Manager
	*/
	GGroupsHistoryManager* GroupsHistoryMng;

	/**
	* Similarities between the subprofiles.
	*/
	GProfilesSims* ProfilesSims;

	/**
	* Agreement and disagreement ratios between the subprofiles.
	*/
	GProfilesBehaviours* ProfilesBehaviours;

	/**
	*  Similarities between documents and subprofiles.
	*/
	GDocProfSims* DocProfSims;

	/**
	* Current seek for this session.
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

	/**
	* Storage manager.
	*/
	GStorage* Storage;

public:

	/**
	* Constructor.
	* @param str             Storage manager.
	* @param sessparams      Parameters of the session.
	* @param tests           Test mode.
	*/
	GSession(GStorage* str,GSessionParams* sessparams,bool tests) throw(std::bad_alloc,GException);

	/**
	* Connect the session to managers.
	* @param langs           Languages Manager.
	* @param umng            URL Manager.
	* @param dmng            Document Analysis Methods Manager.
	* @param pmng            Subprofile Computing Methods Manager.
	* @param gmng            Grouping Methods Manager.
	* @param gcmng           Group Computing Methods Manager.
	* @param smng            Statistics Methods Manager.
	* @param pdmng           Documents Post-Analysis Methods Manager.
	* @param pgmng           Post-Group Computing Methods Manager
	*/
	void Connect(GLangManager* langs,GFilterManager* umng, GDocAnalyseManager* dmng, GProfileCalcManager* pmng, GGroupingManager* gmng, GGroupCalcManager* gcmng,
		GStatsCalcManager* smng, GPostDocManager* pdmng, GPostGroupManager* pgmng) throw(std::bad_alloc,GException);

	/**
	* Post-connect the session to other manager.
	* @param lmng            Link Methods Manager.
	*/
	void PostConnect(GLinkCalcManager* lmng) throw(std::bad_alloc,GException);

	/**
	* Get the languages manager.
	* @return Pointer to GLangManager.
	*/
	GLangManager* GetLangs(void) const {return(Langs);}

	/**
	* Get the document analysis methods manager.
	* @return Pointer to GDocAnalyse.
	*/
	GDocAnalyseManager* GetDocAnalyseMng(void) const {return(DocAnalyseMng);}

	/**
	* Get the subprofile computing methods manager.
	* @return Pointer to GProfileCalcManager.
	*/
	GProfileCalcManager* GetProfilingMng(void) {return(ProfilingMng);}

	/**
	* Get the grouping methods manager.
	* @return Pointer to GGroupingManager.
	*/
	GGroupingManager* GetGroupingMng(void) {return(GroupingMng);}

	/**
	* Get the link methods manager.
	* @return Pointer to GLinkCalcManager.
	*/
	GLinkCalcManager* GetLinkCalcMng(void) {return(LinkCalcMng);}

	/**
	* Get the post-group computing methods manager.
	* @return Pointer to GPostGroupManager.
	*/
	GPostGroupManager* GetPostGroupMng(void) {return(PostGroupMng);}

	/**
	* Get the group computing methods manager.
	* @return Pointer to GGroupCalcManager.
	*/
	GGroupCalcManager* GetGroupCalcMng(void) {return(GroupCalcMng);}

	/**
	* Get the statistics methods manager.
	* @return Pointer to GStatsCalcManager.
	*/
	GStatsCalcManager* GetStatsCalcMng(void) {return(StatsCalcMng);}

	/**
	* Get the historic groups manager.
	* @return Pointer to GGroupsHistoryMng.
	*/
	GGroupsHistoryManager* GetGroupsHistoryManager(void) {return(GroupsHistoryMng);}

	/**
	* Get the storage manager.
	* @return Pointer to GStorage.
	*/
	GStorage* GetStorage(void) const {return(Storage);}

	/**
	* Get the subjects defined.
	* @return Pointer to GSubjects.
	*/
	GSubjects* GetSubjects(void) {return(Subjects);}

	/**
	* Get the parameters of the session.
	* @return Pointer to GSessionParams.
	*/
	GSessionParams* GetSessionParams(void) const {return(SessParams);}

	/**
	* Get a cursor over the factories of the link methods.
	* @return GLinkCalcCursor.
	*/
	GFactoryLinkCalcCursor GetLinkCalcsCursor(void);

	/**
	* Get a cursor over the factories of the document post-analysis methods.
	* @return GPostDocCursor.
	*/
	GFactoryPostDocCursor GetPostDocsCursor(void);

	/**
	* Get a cursor over the assessments made.
	* @return GProfDocCursor.
	*/
	GProfDocCursor GetProfDocsCursor(void);

	/**
	* Assign an identifier to a new data of a given dictionary.
	* @param data           Data.
	* @param dict           Dictionary.
	*/
	virtual void AssignId(GData* data,const GDict* dict);

	/**
	* Assign an identifier to a new group.
	* @param grp            Group.
	*/
	virtual void AssignId(GGroup* grp);

	/**
	* Assign an identifier to a new document.
	* @param doc            Document.
	*/
	virtual void AssignId(GDoc* doc);

	/**
	* Assign an identifier to a new subprofile.
	* @param sub             Subprofile.
	*/
	virtual void AssignId(GSubProfile* sub);

	/**
	* Create a XML structure with the content of a document. The structure
	* created has to be desallocate by the caller.
	* @param doc             Document to analyse.
	* @return Pointer to a GDocXML structure.
	*/
	GDocXML* CreateDocXML(GDoc* doc) throw(GException);

	/**
	* Analyse the documents. At the end, all the enabled post-docs methods are
	* run.
	* @param rec             Receiver for the signals.
	* @param modified        Recompute only modified elements or all.
	* @param save            Document must be saved.
	*/
	void AnalyseDocs(GSlot* rec=0,bool modified=true,bool save=true) throw(GException);

	/**
	* run post-grouping methods are called.
	* @param rec            Receiver of the signals.
	*/
	void ComputePostDoc(GSlot* rec)  throw(GException);

	/**
	* Compute the (sub)profiles.
	* @param rec            Receiver for the signals.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	* @param saveLinks      Save links informations.
	*/
	void CalcProfiles(GSlot* rec,bool modified,bool save,bool saveLinks) throw(GException);

	/**
	* Groups the subprofile into virtual communities. At the end, all the
	* enabled post-grouping methods are called.
	* @param rec            Receiver of the signals.
	* @param modified       Recompute only modified elements or all.
	* @param save           Save modified elements.
	* @param savehistory    Save groups in history.
	*/
	void GroupingProfiles(GSlot* rec,bool modified,bool save, bool savehistory)  throw(GException);

	/**
	* run post-grouping methods are called.
	* @param rec            Receiver of the signals.
	*/
	void ComputePostGroup(GSlot* rec)  throw(GException);

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
	void UpdateBehaviours(void) throw(std::bad_alloc);

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
	* @param sub           The Pointer to the subprofile.
	*/
	double GetSimDocProf(const GDoc* doc,const GSubProfile* sub) throw(GException);
       
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
	void InsertFdbk(GProfile* p,GDoc* d,tDocAssessment j,R::RDate date) throw(std::bad_alloc);

	/**
	* Insert a new Feedback.
	* @param p          Pointer to the profile.
	* @param d          Pointer to the document.
	* @param j          Feedback.
	* @param date       String representing the date on the last feedback.
	*/
	void InsertFdbk(GProfile* p,GDoc* d,tDocAssessment j,const char* date) throw(std::bad_alloc);

	/**
	* Copy the ideal groupment in the current one.
	* @param save           Save modified elements.
	*/
	void CopyIdealGroups(bool save) throw(std::bad_alloc,GException);

	/**
	* Get a cursor over the filters of the system.
	*/
	GFactoryFilterCursor GetFiltersCursor(void);

	/**
	* Get the filter for a specific mime type.
	* @param mime           Name of the mimetype.
	* @return Pointer to a GMIMEFilter.
	*/
	const char* GetMIMEType(const char* mime) const;

	/**
	* Run a "program" for this session.
	* @param rec            Slot that receive information.
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
	void LoadHistoricGroupsById(unsigned int mingen, unsigned int maxgen);

	/**
	* load the historic groups.
	*/
	void LoadHistoricGroupsByDate(R::RString mindate, R::RString maxdate);

	/**
	* Re-init the session (clear all containers).
	* @param Save           must the reinit be saved?
	*/
	void ReInit(bool Save);

	/**
	* Export the vectors/words matrix
	* @param rec            Slot that can receive ionformation.
	* @param type           type of export ("Profiles", "Documents" or "Groups")
	* @param filename       export file name
	* @param lang           lang of the export
	* @param label          display words id aned vectors id ?
	*/
	virtual void ExportMatrix(GSlot* rec,const char* type, const char* filename, GLang* lang, bool label);

	/**
	* Destructor.
	*/
	virtual ~GSession(void);
};


//------------------------------------------------------------------------------
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


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
