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
	virtual int Compare(const GDocProxy* ptr) const { return(Obj->Compare(dynamic_cast<const GDocProxyMem*>(ptr)->Obj)); }
	virtual int Compare(const unsigned int id) const { return(Obj->Compare(id)); }
	virtual bool IsDefined(void){ return(Obj->IsDefined()); }
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
	virtual unsigned int GetId(void){ return(Obj->GetId()); }
	virtual void SetId(unsigned int id){ Obj->SetId(id); }
	virtual unsigned int GetFailed(void){ return(Obj->GetFailed()); }
	virtual void InitFailed(void){ Obj->InitFailed(); }
	virtual void IncFailed(void){ Obj->IncFailed(); }
	virtual void DecFailed(void){ Obj->DecFailed(); }
	virtual R::RCursor<GProfileProxy> GetFdbks(void){ return(Obj->GetFdbks()); }
	virtual GWeightInfoCursor GetWeightInfoCursor(void){ return(Obj->GetWeightInfoCursor()); }
	virtual double Similarity(const GDoc* doc){ return(Obj->Similarity(doc)); }
	virtual double SimilarityIFF(const GDoc* doc){ return(Obj->SimilarityIFF(doc)); }
	virtual double Similarity(const GSubProfile* sub){ return(Obj->Similarity(sub)); }
	virtual double SimilarityIFF(const GSubProfile* sub){ return(Obj->SimilarityIFF(sub)); }
	virtual double Similarity(const GGroup* grp){ return(Obj->Similarity(grp)); }
	virtual double SimilarityIFF(const GGroup* grp){ return(Obj->SimilarityIFF(grp)); }
	virtual void InsertFdbk(unsigned int id){ Obj->InsertFdbk(id); }
	virtual void DeleteFdbk(unsigned int id){ Obj->DeleteFdbk(id); }
	virtual unsigned int GetNbLinks(void){ return(Obj->GetNbLinks()); }
	virtual void InsertLink(const GDoc* doc){ Obj->InsertLink(doc); }
	virtual void InsertLink(const GDoc* doc, unsigned int nbOccurs){ Obj->InsertLink(doc,nbOccurs); }
	virtual GLinkCursor GetLinkCursor(void){ return(Obj->GetLinkCursor()); }
	virtual void InsertSubject(GSubject* s){ Obj->InsertSubject(s); }
	virtual bool IsFromSubject(const GSubject* s){ return(Obj->IsFromSubject(s)); }
	virtual bool IsFromParentSubject(const GSubject* s){ return(Obj->IsFromParentSubject(s)); }
	virtual GSubjectCursor GetSubjectCursor(void){ return(Obj->GetSubjectCursor()); }
	virtual unsigned int GetNbSubjects(void){ return(Obj->GetNbSubjects()); }
	virtual void Update(GLang* lang,R::RContainer<GWeightInfo,false,true>* infos){ Obj->Update(lang,infos); }
	virtual ~GDocProxyMem(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
