/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/
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
#include <glangs/gword.h>
#include <glangs/glang.h>



//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------
class GSession;

//---------------------------------------------------------------------------
// class GDict
class GDict : public RHashContainer<GWord,unsigned,27,true>
{
public:
  GWord** Direct;
	GSession* owner;

  unsigned MaxId;
  unsigned UsedId;
  GLang *Lang;
  RString Name;
  RString Desc;
  bool Loaded;

	GDict(GSession* owner,const RString& name,GLang *lang,unsigned max,unsigned maxletter) throw(bad_alloc);
	GDict(GSession* owner,const RString& name,const RString& desc,GLang* lang=NULL) throw(bad_alloc);
  void Clear(void);
  void PutDirect(GWord *word) throw(bad_alloc); // Put a word in Direct
  inline void Put(unsigned id,const RString &word) throw(bad_alloc)
  {
    GWord Word(id,word),*ptr;
    ptr=GetInsertPtr<GWord>(Word,false);
    PutDirect(ptr);
  }
//  virtual unsigned NextId(const RString& word) throw(GException)=0;          // Return id for a new word
  unsigned GetId(const RString& word) throw(bad_alloc);    // Return the id for a given word
//  virtual void Load(void) throw(bad_alloc,GException)=0;
  int Compare(const GDict* dict) { return(Name.Compare(dict->Name)); }
  int Compare(const GDict& dict) { return(Name.Compare(dict.Name)); }
  int Compare(const GLang* lang) { return(Lang->Compare(lang)); }
  virtual ~GDict(void);
};



}  //-------- End of namespace HyperPRISME-----------------------------------


//---------------------------------------------------------------------------
#endif
