/*

	GALILEI Research Project

	GProfileProxyMem.h

	Proxy for class GProfile - Header.

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
#ifndef GPROFILEPROXYMEM_H
#define GPROFILEPROXYMEM_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gprofileproxy.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ClassGProfileProxyMem
class GProfileProxyMem : public GProfileProxy
{
protected:
	GProfile* Obj;
public:
	GProfileProxyMem(GProfile* obj) : GProfileProxy(), Obj(obj) {}
	virtual int Compare(const GProfileProxy* ptr) const { return(Obj->Compare(dynamic_cast<const GProfileProxyMem*>(ptr)->Obj)); }
	virtual int Compare(const unsigned int id) const { return(Obj->Compare(id)); }
	virtual void SetSubject(GSubject* s){ Obj->SetSubject(s); }
	virtual GSubject* GetSubject(void){ return(Obj->GetSubject()); }
	virtual unsigned int GetId(void){ return(Obj->GetId()); }
	virtual void SetId(unsigned int id){ Obj->SetId(id); }
	virtual R::RString GetName(void){ return(Obj->GetName()); }
	virtual const GUser* GetUser(void){ return(Obj->GetUser()); }
	virtual bool IsSocial(void){ return(Obj->IsSocial()); }
	virtual void SetSocial(bool social){ Obj->SetSocial(social); }
	virtual GSubProfile* GetSubProfile(const GLang* lang){ return(Obj->GetSubProfile(lang)); }
	virtual GSubProfile* GetInsertSubProfile(GLang* lang,GSession* s){ return(Obj->GetInsertSubProfile(lang,s)); }
	virtual GProfDoc* GetFeedback(const GDoc* doc){ return(Obj->GetFeedback(doc)); }
	virtual unsigned int GetNbAssessedDocs(const GLang* lang){ return(Obj->GetNbAssessedDocs(lang)); }
	virtual GProfDocCursor GetProfDocCursor(void){ return(Obj->GetProfDocCursor()); }
	virtual GSubProfileCursor GetSubProfilesCursor(void){ return(Obj->GetSubProfilesCursor()); }
	virtual void InsertFdbk(GProfDoc* j,GSession* s){ Obj->InsertFdbk(j,s); }
	virtual void Modify(GDoc* doc,GLang* newlang,GLang* oldlang){ Obj->Modify(doc,newlang,oldlang); }
	virtual ~GProfileProxyMem(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
