/*

	GALILEI Research Project

	GSession.h

	Generic GALILEI Session - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GSessionH
#define GSessionH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glangs/glangs.h>
#include <glangs/gdicts.h>
#include <gprofiles/gusers.h>
#include <gdocs/gdocs.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GLang;
class GDict;
class GDoc;
class GDocXML;
class GUser;
class GProfile;
class GProfileCursor;
class GProfDoc;
class GSubProfile;
class GGroup;
class GGroups;
class GGroupsCursor;
class GProfileCalc;
class GFilter;
class GMIMEFilter;
class GURLManager;
class GDocAnalyse;


//-----------------------------------------------------------------------------
/**
* The GSessionSignalsReceiver provides a representation for a reciever of
* signals of a session.
*/
class GSessionSignalsReceiver
{
public:
	/**
	* Constructor.
	*/
	GSessionSignalsReceiver(void);

	/**
	* The traitment for a specific document will begin.
	* @param doc            Document.
	*/
	virtual void receiveNextDoc(const GDoc* doc);

	/**
	* The traitment for a specific document will begin.
	* @param prof           Profile.
	*/
	virtual void receiveNextProfile(const GProfile* prof);
};


//-----------------------------------------------------------------------------
/**
* The GSession provides a representation for a GALILEI session. The way the
* data are stored and loaded is dedicated to a child class of GSession through
* a set of pure virtual methods defined.
* @author Pascal Francq
* @short Generic Session.
*/
class GSession
{
protected:

	/**
	* Languages handled by the system.
	*/
	GLangs Langs;

	/**
	* Stoplists of the different languages.
	*/
	GDicts Stops;

	/**
	* Dictionnaries of the different languages.
	*/
	GDicts Dics;

	/**
	* Users handled by the system.
	*/
	GUsers Users;

	/**
	* Documents handled by the system.
	*/
	GDocs Docs;

	/**
	* Groups handled by the system.
	*/
	RStd::RContainer<GGroups,unsigned int,true,true> Groups;

	/**
	* All the judgements.
	*/
	RStd::RContainer<GProfDoc,unsigned,true,true> Fdbks;

	/**
	* SubProfiles handled by the system.
	*/
	RStd::RContainer<GProfile,unsigned int,true,true>* Profiles;

	/**
	* SubProfiles handled by the system.
	*/
	RStd::RContainer<GSubProfile,unsigned int,true,false>* SubProfiles;

	/**
	* URL Manager used by this session.
	*/
	GURLManager* Mng;

	/**
	* Analyser used for the document.
	*/
	GDocAnalyse* DocAnalyse;

	/**
	* State of the dictionnaries and stoplists.
	*/
	bool bDics;

	/**
	* State of the documents.
	*/
	bool bDocs;

	/**
	* State of the users.
	*/
	bool bUsers;

	/**
	* State of the groups.
	*/
	bool bGroups;

	/**
	* State of the User's feedbacks.
	*/
	bool bFdbks;

	/**
	* Define if the language of the language must be static.
	*/
	bool StaticLang;

	/**
	* Minimum percentage of words of a document which must be in the stop list
	* of a language to make the language the current one.
	*/
	double MinStopWords;

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
	* @name Method for Languages.
	*/
	//@{

protected:

		/**
		* Loading a dictionnary/stoplist.
		* @param code           Code of the languague.
		* @param stop           Is it a stop list.
		*/
		virtual void LoadDic(const char* code,bool s) throw(bad_alloc,GException)=0;

		/**
		* Load a specific word from a dictionnary.
		* @param id             Idenfificator of the word.
		* @param code           Code of the languague.
		*/
		virtual const char* LoadWord(const unsigned int id,const char* code)=0;

public:
		/**
		* Get the number of languages treated by the system.
		* @returns Number of languages.
		*/
		unsigned int GetNbLangs(void) const
			{return(Langs.NbPtr);}

		/**
		* Get a pointer to the languages.
		* @returns Pointer to the languages.
		*/
		const GLangs* GetLangs(void) const
			{return(&Langs);}

		/**
		* Get a pointer to a language while having its code.
		* @param code             Code of the language.
		* @returns Pointer to the language.
		*/
		GLang* GetLang(const char* code) const;

		/**
		* Verify if the dictionnaries/stoplists are loaded.
		* @returns true, if loaded.
		*/
		bool IsDicsLoad(void) const {return(bDics);}

		/**
		* Load the dictionnaries and stoplists.
		*/
		void InitDics(void) throw(bad_alloc,GException);

		/**
		* Get a pointer to a dictionnary corresponding to a given language.
		* @param lang       Pointer to the language.
		* @returns Pointer to a dictionnary.
		*/
		GDict* GetDic(const GLang* lang) const throw(GException);

		/**
		* Get a pointer to a stoplist corresponding to a given language.
		* @param lang       Pointer to the language.
		* @returns Pointer to a stoplist.
		*/
		GDict* GetStop(const GLang* lang) const throw(GException);

		/**
		* Get the word corresponding to a given identificator for a language.
		* @param id         Identificator.
		* @param lang       Pointer to the corresponding language.
		* @returns Pointer to a C string.
		*/
		const char* GetWord(const unsigned int id,const GLang* lang);

		/**
		* Get the word corresponding to a given identificator of a dictionnary.
		* @param dic        Pointer to the corresponding dictionnary.
		* @returns Pointer to a C string.
		*/
		const char* GetWord(const unsigned int id,const GDict* dict) const;

		/**
		* Return the identifier of a new word of a dictionnary.
		* @param word           Word to find.
		* @param dict           Dictionnary.
		*/
		virtual unsigned int GetDicNextId(const char* word,const GDict* dict)=0;

	//@}

	/**
	* @name Method for Information.
	*/
	//@{

	//@}

	/**
	* @name Method for Documents.
	*/
	//@{

protected:

		/**
		* Load the documents.
		*/
		virtual void LoadDocs(void) throw(bad_alloc,GException)=0;

public:

		/**
		* Create a new document.
		* @param url        URL of the document.
		* @param name       Name of the document.
		* @param mime       MIME Type of the document
		* @returns Pointer to a new created document.
		*/
		virtual GDoc* NewDoc(const char* url,const char* name,const char* mime) throw(GException)=0;

		/**
		* Save a document.
		* @param doc        Document to save.
		*/
		virtual void Save(GDoc* doc) throw(GException)=0;

		/**
		* Get the documents' analyser.
		* @returns Pointer to a GDocAnalyse class.
		*/
		GDocAnalyse* GetDocAnalyse(void) const {return(DocAnalyse);}

		/**
		* Get the number of documents treated by the system.
		* @returns Number of documents.
		*/
		unsigned int GetNbDocs(void) const
			{return(Docs.NbPtr);}

		/**
		* Get a pointer to the documents.
		* @returns Pointer to the documents.
		*/
		const GDocs* GetDocs(void) const
			{return(&Docs);}

		/**
		* Verify if the documents are loaded.
		* @returns true, if loaded.
		*/
		bool IsDocsLoad(void) const {return(bDocs);}

		/**
		* See if the languages are static for the documents.
		* @returns Boolean.
		*/
		bool IsStaticLang(void) const {return(StaticLang);}

		/**
		* Set if the languages are static for the document.
		* @param s          Static?
		*/
		void SetStaticLang(bool s) {StaticLang=s;}

		/**
		* Get the minimum percentage of stop words.
		* @returns Double.
		*/
		double GetMinStopWords(void) const {return(MinStopWords);}

		/**
		* Set the minimum percentage of stop words.
		* @param min        Minimal Percentage.
		*/
		void SetMinStopWords(double min) {MinStopWords=min;}

		/**
		* Load the documents.
		*/
		void InitDocs(void) throw(bad_alloc,GException);

		/**
		* Create a XML structure of the content of a document. The structure
		* created has to be desallocate by the caller.
		* @param doc        Document to analyse.
		*/
		GDocXML* CreateDocXML(GDoc* doc) throw(GException);

		/**
		* Analyse all the necessary documents.
		* @param rec        Receiver for the signals.
		* @param modified   Recompute only modified elements or all.
		*/
		void AnalyseDocs(GSessionSignalsReceiver* rec,bool modified=true) throw(GException);

	//@}

	/**
	* @name Method for Users/Profiles.
	*/
	//@{

protected:

		/**
		* Load the Users.
		*/
		virtual void LoadUsers(void) throw(bad_alloc,GException)=0;

public:

		/**
		* Create a new profile.
		* @param usr        Pointer to the user of the profile.
		* @param desc       Description of the profile.
		* @returns Pointer to GProfile.
		*/
		virtual GProfile* NewProfile(GUser* usr,const char* desc) throw(bad_alloc,GException)=0;

		/**
		* Save information about the groupement (Group and attachment date) of
		* a subprofile. For a complete save, call Save(const GProfile*).
		* @param sub        Subprofile to save.
		*/
		virtual void Save(const GSubProfile* sub) throw(GException)=0;

		/**
		* Save a profile.
		* @param prof       Profile to save.
		*/
		virtual void Save(const GProfile* prof) throw(GException)=0;

		/**
		* Get a profile with a specific identifier.
		* @param id         Identifier.
		*/
		GProfile* GetProfile(const unsigned int id) const;

		/**
		* Get a cursor over the profiles used in the system.
		*/
		GProfileCursor& GetProfilesCursor(void);

		/**
		* Get the number of users treated by the system.
		* @returns Number of Users.
		*/
		unsigned int GetNbUsers(void) const {return(Users.NbPtr);}

		/**
		* Get a pointer to the Users.
		* @returns Pointer to the users.
		*/
		const GUsers* GetUsers(void) const
			{return(&Users);}

		/**
		* Verify if the users are loaded.
		* @returns true, if loaded.
		*/
		bool IsUsersLoad(void) const {return(bUsers);}

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
		* Compute all the necessary profiles.
		* @param rec        Receiver for the signals.
		* @param method     Method used to compute.
		* @param modified   Recompute only modified elements or all.
		*/
		void CalcProfiles(GSessionSignalsReceiver* rec,GProfileCalc* method,bool modified=true) throw(GException);

	//@}

	/**
	* @name Method for Feedbacks.
	*/
	//@{

protected:

		/**
		* Load the Feedbacks.
		*/
		virtual void LoadFdbks(void) throw(bad_alloc,GException)=0;

public:

		/**
		* Verify if the feedback are loaded.
		* @returns true, if loaded.
		*/
		bool IsFdbksLoad(void) const {return(bFdbks);}

		/**
		* Load the Users' feedback.
		*/
		void InitFdbks(void) throw(bad_alloc,GException);

		/**
		* Insert a new Feedback.
		* @param p          Pointer to the profile.
		* @param d          Pointer to the document.
		* @param j          Feedback.
		* @param date       Date on the last feedback.
		*/
		void InsertFdbk(GProfile* p,GDoc* d,char j,const char* date) throw(bad_alloc);

	//@}

	/**
	* @name Method for Groups.
	*/
	//@{

protected:

		/**
		* Load the groups.
		*/
		virtual void LoadGroups(void) throw(bad_alloc,GException)=0;

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
		* Get a cursor over the groups of the system.
		*/
		GGroupsCursor& GetGroupsCursor(void);

		/**
		* Find the groups for a specific language.
		* @param lang           Pointer to the language.
		* @returns Pointer to the group.
		*/
		GGroups* GetGroups(const GLang* lang) const;

		/**
		* Save a group, i.e. save all the information of the subprofiles
		* concerning the groupement.
		* @param grp        Group to save.
		*/
		void Save(GGroup* grp) throw(GException);

		/**
		* Create a new group.
		* @param lang       Language of the group to create.
		*/
		virtual GGroup* NewGroup(GLang* lang)=0;

		/**
		* Delete a group.
		* @param grp        Group to delete.
		*/
		virtual void DeleteGroup(GGroup* grp)=0;

	//@}

	/**
	* @name Method for URL Manager and Filters.
	*/
	//@{

		/**
		* Start the iterator to go trough the filters.
		*/
		void FiltersStart(void);

		/**
		* Test if the end of the container of filters is reached.
		*/
		bool FiltersEnd(void) const;

		/**
		* Goto the next element, if the end is reached, go to the beginning.
		*/
		void FiltersNext(void);

		/**
		* Get the current filter.
		* @returns Pointer to the current filter.
		*/
		GFilter* GetCurFilters(void);

		/**
		* Get the filter for a specific mime type.
		* @param mime           Name of the mimetype.
		* @return Pointer to a GMIMEFilter.
		*/
		GMIMEFilter* GetMIMEType(const char* mime) const;

	//@}

	/**
	* Destructor.
	*/
	virtual ~GSession(void) throw(GException);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
