/*

	GALILEI Research Project

	GProfileProxyMem.h

	Proxy for class GProfile - Header.

	Copyright 2004 by the Universit�Libre de Bruxelles.

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
#include <gprofileproxy.h>


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
	virtual int Compare(const GProfileProxy& ptr) const { return(Obj->Compare(*dynamic_cast<const GProfileProxyMem&>(ptr).Obj)); }
	virtual int Compare(const GProfileProxy* ptr) const { return(Obj->Compare(*dynamic_cast<const GProfileProxyMem*>(ptr)->Obj)); }
	virtual int Compare(const unsigned int id) const { return(Obj->Compare(id)); }
	virtual unsigned int GetId(void) const {return(Obj->GetId());}
	virtual void SetId(unsigned int id)  throw(GException) {Obj->SetId(id);}
	virtual R::RString GetName(void) const {return(Obj->GetName());}
	virtual GUser* GetUser(void) const {return(Obj->GetUser());}
	virtual bool IsSocial(void) const {return(Obj->IsSocial());}
	virtual void SetSocial(bool social) {Obj->SetSocial(social);}
	virtual GSubProfile* GetSubProfile(const GLang* lang) const {return(Obj->GetSubProfile(lang));}
	virtual GSubProfile* GetInsertSubProfile(GLang* lang,GSession* s) {return(Obj->GetInsertSubProfile(lang,s));}
	virtual unsigned int GetNbAssessedDocs(const GLang* lang) const {return(Obj->GetNbAssessedDocs(lang));}
	virtual unsigned int GetNbAssessedDocs(void) const {return(Obj->GetNbAssessedDocs());}
	virtual R::RCursor<GFdbk> GetFdbks(void) {return(Obj->GetFdbks());}
	virtual R::RCursor<GSubProfile> GetSubProfilesCursor(void) {return(Obj->GetSubProfilesCursor());}
	virtual void InsertFdbk(unsigned int id,tDocAssessment assess,R::RDate date) throw(std::bad_alloc) {Obj->InsertFdbk(id,assess,date);}
	virtual void DeleteFdbk(unsigned int id) throw(std::bad_alloc) {Obj->DeleteFdbk(id);}
	virtual void ClearFdbks(void) {Obj->ClearFdbks();}
	virtual GFdbk* GetFdbk(unsigned int id) const {return(Obj->GetFdbk(id));}
	virtual void HasUpdate(unsigned int id,bool computed) {Obj->HasUpdate(id,computed);}
	virtual void Update(void) {Obj->Update();}
	virtual ~GProfileProxyMem(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
