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
#include <ggroups/ggroupslangs.h>
#include <gdocs/gdocs.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


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

	GDicts* Stops;
	GDicts* Dics;
	GUsers* Users;
	GDocs* Docs;
	RStd::RContainer<GGroups,unsigned int,true,true> Groups;
	RStd::RContainer<GMIMEType,unsigned int,true,true>* MIMETypes;

public:

  // Constructor & Init part
  GSession(void) throw(bad_alloc,GException);

  // Dictionnaries
  virtual void LoadDics(void) throw(bad_alloc,GException)=0;
  GDict* GetDic(const GLang* lang) throw(GException);
  GDict* GetStop(const GLang* lang) throw(GException);
  void ClearDics(void) throw(GException);
  void ClearStops(void) throw(GException);
	virtual unsigned DicNextId(const RString& word)=0;
	

protected:

public:

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
	*
	*/
	GUser* CreateUser(const char* usr,const char* pwd,const char* name,const char* email,
	                  const char* title,const char* org,const char* addr1,
	                  const char* addr2,const char* city,const char* country) throw(bad_alloc);

  // Documents
  unsigned GetNbDocs(void);
  void ClearDocs(void) throw(GException);

  	unsigned int GetNbUsers(void) const {return(Users->NbPtr);}


	/**
	* @return Pointer to the mime type.
	*/
	GMIMEType* GetMIMEType(const char* name)
		{return(MIMETypes->GetPtr<const char*>(name));}

	/**
	* @return Pointer to the mime type.
	*/
	GMIMEType* GetMIMEType(const RString& name)
		{return(MIMETypes->GetPtr<const RString>(name));}

	/**
	* @name Method for Languages.
	*/
	//@{

protected:


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
		GLang* GetLang(const char* code);

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
		/**
		* Get the number of documents treated by the system.
		* @returns Number of documents.
		*/
		unsigned int GetNbDocs(void) const
			{return(Docs->NbPtr);}

		/**
		* Get a pointer to the documents.
		* @returns Pointer to the documents.
		*/
		const GDocs* GetDocs(void) const
			{return(Docs);}

	//@}

	/**
	* @name Method for Users/Profiles.
	*/
	//@{

		/**
		* Get a pointer to the Users.
		* @returns Pointer to the users.
		*/
		const GUsers* GetUsers(void) const
			{return(Users);}

	//@}

	/**
	* @name Method for Groups.
	*/
	//@{

	/**
	* Find the groups for a specific language.
	* @param lang           Pointer to the language.
	* @returns Pointer to the group.
	*/
	GGroups* GetGroups(const GLang* lang) const;

	/**
	* Create a new group.
	*/
	GGroup* NewGroup(void);

	//@}

  // Destructor
  virtual ~GSession(void) throw(GException);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
