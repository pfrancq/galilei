
#ifndef GLangH
#define GLangH




#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei

#include <galilei.h>



//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// class GLang
class GLang
{
public:
  RString Lang;               // Language
  char Code[3];               // The code used in the databases
  bool Activ;                 // Defines if the language is activ

  // Constructor
  GLang(const RString& lang,const char* code) throw(bad_alloc);

  // Function needed by GLangs (-> RContainer)
  int Compare(const GLang& lang);
  int Compare(const GLang* lang);

  // Dictionnay and Stop List names
  virtual RString& GetDic(void)=0;
  virtual RString& GetStop(void)=0;

	// Destructor
	virtual ~GLang(void) {}
};

}
 #endif
