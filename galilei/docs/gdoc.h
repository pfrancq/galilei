

//---------------------------------------------------------------------------
#ifndef GDocH
#define GDocH


//---------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;

//---------------------------------------------------------------------------
// include files for Galilei

#include <galilei.h>
#include <gdocs/gwordoccurs.h>
#include <glangs/glang.h>
#include <gdocs/gdocs.h>




//---------------------------------------------------------------------------

namespace GALILEI{

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// class GDoc
class GDoc
{
  char* Buffer;       // Pointer
  char Word[51];      // Word
  GDocs *Owner;       // Owner (Container)
public:
	GWordOccurs* Words;             // Occurencies of Words
	unsigned NbWords;              // Total number of words
	unsigned NbDiffWords;          // Number of different words
	GLang* Lang;                        // Pointer to the language
	RString URL;                        // URL
	unsigned Id;                   // Unique identifier
	char* Content;                      // Pointer to the content
	bool Calc;                          // Must the doc be analysed?
	bool bSave;											// Must the doc be saved?

  GDoc(GDocs *owner,const RString& URL) throw(bad_alloc);
  int Compare(const GDoc& doc);
  int Compare(const GDoc* doc);
  int Compare(unsigned id);
  unsigned char GetChar(void);
  bool IsSpace(void);
  void SkipSpaces(void);
  void InitWords(void);
  bool NextWord(void);
  void Analyse(char* body) throw(GException);
  virtual void Load(void) throw(GException,bad_alloc)=0;
  virtual void Save(void) throw(GException)=0;
	virtual void Download(void)=0;
  virtual ~GDoc(void);
};




}  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
