/*

  GSession.h

  HyperPRISME Session - Header.

  (C) 1998-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  HyperPRISME base library

*/



//---------------------------------------------------------------------------
#ifndef GSessionH
#define GSessionH


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for HyperPRISME
#include <galilei.h>
#include <glangs/glangs.h>
#include <glang/gdicts.h>
#include <gprofile/gusers.h>
#include <ggroups/ggroupslangs.h>
#include <glangs/gdict.h>


//---------------------------------------------------------------------------
namespace GALILEI
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// GSession
class GSession
{
protected:
	bool AllDocs;     // All Documents
public:
	GLangs* Langs;
	GDicts *Stops;
	GDicts *Dics;
	GUsers *Users;
	GDocs* Docs;
	GGroupsLangs *GroupsLangs;

  // Constructor & Init part
  GSession(void) throw(bad_alloc,GException);

  // Languages
  template<class hlangs,class hdicts> void LoadLanguages(unsigned nb) throw(bad_alloc,GException);

  // Dictionnaries
  void LoadDics(void) throw(bad_alloc,GException);
  void LoadStops(void) throw(bad_alloc,GException);
  GDict* GetDic(GLang *lang) throw(GException);
  GDict* GetStop(GLang *lang) throw(GException);
  void ClearDics(void) throw(GException);
  void ClearStops(void) throw(GException);

  // Documents
  template<class hdocs> void LoadDocuments(unsigned nb) throw(bad_alloc,GException);
  template<class hdocs> void LoadDocuments(unsigned nb,GProfile* profile) throw(bad_alloc,GException);
  void PutAllDocs(bool alldocs);
  bool GetAllDocs(void) { return(AllDocs); }
  unsigned GetNbDocs(void);
  void AnalyseDocs(URLFunc *urlfunc,InfoFunc *infofunc) throw(bad_alloc,GException);
  void ClearDocs(void) throw(GException);

  // Users
  template<class husers> void LoadUsers(unsigned nb) throw(bad_alloc,GException);

	// Groups
	template<class hgroupslangs,class hgroups> void LoadGrps(void) throw(bad_alloc,GException);

  // Destructor
  virtual ~GSession(void) throw(GException);
};


#include "GSession.hh" // Template implementation


}  //-------- End of namespace Galilei-----------------------------------


//---------------------------------------------------------------------------
#endif
