/*

	GALILEI Research Project

	GLangProxy.h

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
#ifndef GLANGPROXY_H
#define GLANGPROXY_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/glang.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ClassGLangProxy
class GLangProxy
{
public:
	GLangProxy(void) {}
	virtual int Compare(const GLangProxy* ptr) const=0;
	virtual int Compare(const unsigned int id) const=0;
	virtual void SkipSequence(const R::RString& word) throw(std::bad_alloc)=0;
	virtual R::RString GetStemming(const R::RString& kwd) throw(GException)=0;
	virtual GSession* GetSession(void) const=0;
	virtual GDict* GetDict(void) const=0;
	virtual GDict* GetStop(void) const=0;
	virtual bool InStop(const R::RString& name) const throw(GException)=0;
	virtual void IncRef(unsigned int id,tObjType ObjType) throw(GException)=0;
	virtual void DecRef(unsigned int id,tObjType ObjType) throw(GException)=0;
	virtual unsigned int GetRef(unsigned int id,tObjType ObjType) throw(GException)=0;
	virtual void IncRef(tObjType ObjType) throw(GException)=0;
	virtual void DecRef(tObjType ObjType) throw(GException)=0;
	virtual unsigned int GetRef(tObjType ObjType) throw(GException)=0;
	virtual unsigned int GetTotal(void) const throw(GException)=0;
	virtual const GData** GetDatas(void) const throw(GException)=0;
	virtual bool MustSkipSequence(const R::RChar* seq) throw(GException)=0;
	virtual ~GLangProxy(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
