/*

	GALILEI Research Project

	GSession.h

	Basic GALILEI Session - Header.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//---------------------------------------------------------------------------
#ifndef GSessionH
#define GSessionH


//---------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//---------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glangs/glangs.h>
#include <glangs/gdicts.h>
#include <gprofiles/gusers.h>
#include <gprofiles/gprofile.h>
#include <ggroups/ggroupslangs.h>
#include <glangs/gdict.h>
#include <gexceptions.h>
#include <gdocs/gmimetype.h>
#include <gdocs/gdocs.h>


//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// GSession
class GSession
{
protected:
	bool AllDocs;     // All Documents
public:
	GLangs* Langs;
	GDicts* Stops;
	GDicts* Dics;
	GUsers* Users;
	GDocs* Docs;
	GGroupsLangs* GroupsLangs;
	RStd::RContainer<GMIMEType,unsigned int,true,true>* MIMETypes;

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
//  void PutAllDocs(bool alldocs);
//  bool GetAllDocs(void) { return(AllDocs); }
  unsigned GetNbDocs(void);
//  void AnalyseDocs(URLFunc *urlfunc,InfoFunc *infofunc) throw(bad_alloc,GException);
  void ClearDocs(void) throw(GException);

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

  // Destructor
  virtual ~GSession(void) throw(GException);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
