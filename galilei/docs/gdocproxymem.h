/*

	GALILEI Research Project

	GDocProxyMem.h

	Proxy for class GDoc - Header.

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
#ifndef GDOCPROXYMEM_H
#define GDOCPROXYMEM_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocproxy.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ClassGDocProxyMem
class GDocProxyMem : public GDocProxy
{
protected:
	GDoc* Obj;
public:
	GDocProxyMem(GDoc* obj) : GDocProxy(), Obj(obj) {}
	virtual bool IsDefined(void){ return(Obj->IsDefined()); }
	virtual void ClearInfos(bool l){ Obj->ClearInfos(l); }
	virtual void ClearFdbks(void){ Obj->ClearFdbks(); }
	virtual R::RString GetURL(void){ return(Obj->GetURL()); }
	virtual R::RString GetName(void){ return(Obj->GetName()); }
	virtual void SetName(const R::RString& name){ Obj->SetName(name); }
	virtual R::RDate GetUpdated(void){ return(Obj->GetUpdated()); }
	virtual R::RDate GetComputed(void){ return(Obj->GetComputed()); }
	virtual R::RString GetMIMEType(void){ return(Obj->GetMIMEType()); }
	virtual void SetMIMEType(const R::RString& mime){ Obj->SetMIMEType(mime); }
	virtual tObjState GetState(void){ return(Obj->GetState()); }
	virtual void SetState(tObjState state){ Obj->SetState(state); }
	virtual GLang* GetLang(void){ return(Obj->GetLang()); }
	virtual void SetLang(GLang* l){ Obj->SetLang(l); }
	virtual unsigned int GetId(void){ return(Obj->GetId()); }
	virtual void SetId(unsigned int id){ Obj->SetId(id); }
	virtual unsigned int GetFailed(void){ return(Obj->GetFailed()); }
	virtual void InitFailed(void){ Obj->InitFailed(); }
	virtual void IncFailed(void){ Obj->IncFailed(); }
	virtual void DecFailed(void){ Obj->DecFailed(); }
	virtual ~GDocProxyMem(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
