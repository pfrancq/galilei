/*

	GALILEI Research Project

	GLangProxyMem.h

	Proxy for class GLang - Header.

	Copyright 2004 by the Université Libre de Bruxelles.

	Authors:
		CProxy program

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/



//------------------------------------------------------------------------------
#ifndef GLANGPROXYMEM_H
#define GLANGPROXYMEM_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/glangproxy.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ClassGLangProxyMem
class GLangProxyMem : public GLangProxy
{
protected:
	GLang* Obj;
public:
	GLangProxyMem(GLang* obj) : GLangProxy(), Obj(obj) {}
	virtual void SkipSequence(const R::RString& word){ Obj->SkipSequence(word); }
	virtual R::RString GetStemming(const R::RString& kwd){ return(Obj->GetStemming(kwd)); }
	virtual GSession* GetSession(void){ return(Obj->GetSession()); }
	virtual GDict* GetDict(void){ return(Obj->GetDict()); }
	virtual GDict* GetStop(void){ return(Obj->GetStop()); }
	virtual bool InStop(const R::RString& name){ return(Obj->InStop(name)); }
	virtual void IncRef(unsigned int id,tObjType ObjType){ Obj->IncRef(id,ObjType); }
	virtual void DecRef(unsigned int id,tObjType ObjType){ Obj->DecRef(id,ObjType); }
	virtual unsigned int GetRef(unsigned int id,tObjType ObjType){ return(Obj->GetRef(id,ObjType)); }
	virtual void IncRef(tObjType ObjType){ Obj->IncRef(ObjType); }
	virtual void DecRef(tObjType ObjType){ Obj->DecRef(ObjType); }
	virtual unsigned int GetRef(tObjType ObjType){ return(Obj->GetRef(ObjType)); }
	virtual unsigned int GetTotal(void){ return(Obj->GetTotal()); }
	virtual const GData** GetDatas(void){ return(Obj->GetDatas()); }
	virtual bool MustSkipSequence(const R::RChar* seq){ return(Obj->MustSkipSequence(seq)); }
	virtual ~GLangProxyMem(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
