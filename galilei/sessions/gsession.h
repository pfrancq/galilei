
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
#include <glangs/gdicts.h>
#include <gprofiles/gusers.h>
#include <gprofiles/gprofile.h>
#include <ggroups/ggroupslangs.h>
#include <glangs/gdict.h>
#include <gexceptions.h>

#include <gdocs/gdocs.h>

//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------

class GDocs;
class GGroupsLangs;
class GDicts;
class GUsers;
class GProfile;

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
	virtual unsigned DicNextId(const RString& word)=0;
	

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


#include "gsession.hh" // Template implementation


}  //-------- End of namespace Galilei-----------------------------------


//---------------------------------------------------------------------------
#endif
