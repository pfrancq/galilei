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
#include <gprofiles/gprofilecalc.h>
#include <ggroups/ggroups.h>
#include <gdocs/gdocs.h>
#include <urlmanagers/gurlmanager.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GFilter;


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
	RContainer<GProfDoc,unsigned,true,true> Fdbks;

	/**
	* URL Manager used by this session.
	*/
	GURLManager* Mng;

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
	* State of the groups' Member.
	*/
	bool bGroupsMember;

	/**
	* State of the User's feedbacks.
	*/
	bool bFdbks;

	/**
	* Define if the language of the language must be static.
	*/
	bool StaticLang;

public:

	/**
	* Constructor.
	* @param d              Number of documents.
	* @param u              Number of users.
	* @param f              Number of Feedbacks.
	* @param mng            URL Manager.
	*/
	GSession(unsigned int d,unsigned int u,unsigned int f,GURLManager* mng) throw(bad_alloc,GException);

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

		/**
		* Save a document.
		* @param doc            Document to save.
		*/
		virtual void Save(GDoc* doc) throw(GException)=0;

public:

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
		*/
		void AnalyseDocs(GSessionSignalsReceiver* rec) throw(GException);

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
		GUser* CreateUser(const char* usr,const char* pwd,const char* name,const char* email,
		                  const char* title,const char* org,const char* addr1,
		                  const char* addr2,const char* city,const char* country) throw(bad_alloc);

		/**
		* Compute all the necessary profiles.
		* @param rec        Receiver for the signals.
		* @param method         Method used to compute.
		*/
		void CalcProfiles(GSessionSignalsReceiver* rec,GProfileCalc* method) throw(GException);

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

		/**
		* Load the group's member.
		* @param grp        Group to load the members.
		*/
		virtual void LoadGroupsMember(GGroup* grp) throw(bad_alloc,GException)=0;

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
		* Verify if the groups' members are loaded.
		* @returns true, if loaded.
		*/
		bool IsGroupsMember(void) const {return(bGroupsMember);}

		/**
		* Load the Groups' Member.
		*/
		void InitGroupsMember(void) throw(bad_alloc,GException);

		/**
		* Find the groups for a specific language.
		* @param lang           Pointer to the language.
		* @returns Pointer to the group.
		*/
		GGroups* GetGroups(const GLang* lang) const;

		/**
		* Create a new group.
		* @param lang       Language of the group to create.
		*/
		GGroup* NewGroup(GLang* lang);

	//@}

	/**
	* @name Method for URL Manager and Filters.
	*/
	//@{

		/**
		* Start the iterator to go trough the filters.
		*/
		inline void FiltersStart(void)
			{Mng->GetFilters()->Start();}

		/**
		* Test if the end of the container of filters is reached.
		*/
		inline bool FiltersEnd(void) const
			{return(Mng->GetFilters()->End());}

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	inline void FiltersNext(void)
		{Mng->GetFilters()->Next();}

	/**
	* Get the current word.
	* @returns Pointer to the current word.
	*/
	GFilter* GetCurFilters(void) {return((*Mng->GetFilters())());}

	//@}

	/**
	* Destructor.
	*/
	virtual ~GSession(void) throw(GException);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
