
#ifndef GDictH
#define GDictH


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>
#include <gdocs/gword.h>
#include <glangs/glang.h>


//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// class GDict
class GDict : public RHashContainer<GWord,unsigned,27,true>
{
public:
  GWord** Direct;

  unsigned MaxId,UsedId;
  GLang *Lang;
  RString Name,Desc;
  bool Loaded;

  GDict(const RString& name,GLang *lang,unsigned max,unsigned maxletter) throw(bad_alloc);
  GDict(const RString& name,const RString& desc,GLang* lang=NULL) throw(bad_alloc);
  void Clear(void);
  void PutDirect(GWord *word) throw(bad_alloc); // Put a word in Direct
  inline void Put(unsigned id,const RString &word) throw(bad_alloc)
  {
    GWord Word(id,word),*ptr;
    ptr=GetInsertPtr<GWord>(Word,false);
    PutDirect(ptr);
  }
  virtual unsigned NextId(const RString& word) throw(GException)=0;          // Return id for a new word
  unsigned GetId(const RString& word) throw(bad_alloc);    // Return the id for a given word
  virtual void Load(void) throw(bad_alloc,GException)=0;
  int Compare(const GDict* dict) { return(Name.Compare(dict->Name)); }
  int Compare(const GDict& dict) { return(Name.Compare(dict.Name)); }
  int Compare(const GLang* lang) { return(Lang->Lang.Compare(lang->Lang)); }
  virtual ~GDict(void);
};



}  //-------- End of namespace HyperPRISME-----------------------------------


//---------------------------------------------------------------------------
#endif
