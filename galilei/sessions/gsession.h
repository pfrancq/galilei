
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
	RContainer<GMIMEType,unsigned int,true,true>* MIMETypes;

  // Constructor & Init part
  GSession(void) throw(bad_alloc,GException);

  // Dictionnaries
  void LoadDics(void) throw(bad_alloc,GException);
  void LoadStops(void) throw(bad_alloc,GException);
  GDict* GetDic(GLang *lang) throw(GException);
  GDict* GetStop(GLang *lang) throw(GException);
  void ClearDics(void) throw(GException);
  void ClearStops(void) throw(GException);
	virtual unsigned DicNextId(const RString& word)=0;
	

  // Documents
  void PutAllDocs(bool alldocs);
  bool GetAllDocs(void) { return(AllDocs); }
  unsigned GetNbDocs(void);
  void AnalyseDocs(URLFunc *urlfunc,InfoFunc *infofunc) throw(bad_alloc,GException);
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
