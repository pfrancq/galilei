/*

	GALILEI Research Project

	GDocProxyMem.h

	Proxy for class GDoc - Header.

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
	virtual bool IsDefined(void) const {return(Obj->IsDefined());}
	virtual R::RString GetURL(void) const {return(Obj->GetURL());}
	virtual R::RString GetName(void) const {return(Obj->GetName());}
	virtual void SetName(const R::RString& name) throw(std::bad_alloc) {Obj->SetName(name);}
	virtual R::RDate GetUpdated(void) const {return(Obj->GetUpdated());}
	virtual R::RDate GetComputed(void) const {return(Obj->GetComputed());}
	virtual R::RString GetMIMEType(void) const {return(Obj->GetMIMEType());}
	virtual void SetMIMEType(const R::RString& mime) {Obj->SetMIMEType(mime);}
	virtual tObjState GetState(void) const {return(Obj->GetState());}
	virtual void SetState(tObjState state) {Obj->SetState(state);}
	virtual GLang* GetLang(void) const {return(Obj->GetLang());}
	virtual unsigned int GetId(void) const {return(Obj->GetId());}
	virtual void SetId(unsigned int id) throw(GException) {Obj->SetId(id);}
	virtual unsigned int GetOwnerId(void) const {return(Obj->GetOwnerId());}
	virtual unsigned int GetFailed(void) const {return(Obj->GetFailed());}
	virtual void InitFailed(void) {Obj->InitFailed();}
	virtual void IncFailed(void) {Obj->IncFailed();}
	virtual void DecFailed(void) {Obj->DecFailed();}
	virtual R::RCursor<GProfileProxy> GetFdbks(void) {return(Obj->GetFdbks());}
	virtual R::RCursor<GWeightInfo> GetWeightInfoCursor(void) {return(Obj->GetWeightInfoCursor());}
	virtual const GWeightInfos* GetInfos(void) const {return(Obj->GetInfos());}
	virtual double GetMaxWeight(void) const throw(GException) {return(Obj->GetMaxWeight());}
	virtual double Similarity(const GDoc* doc) const {return(Obj->Similarity(doc));}
	virtual double SimilarityIFF(const GDoc* doc) const throw(GException) {return(Obj->SimilarityIFF(doc));}
	virtual double Similarity(const GSubProfile* sub) const {return(Obj->Similarity(sub));}
	virtual double SimilarityIFF(const GSubProfile* sub) const throw(GException) {return(Obj->SimilarityIFF(sub));}
	virtual double Similarity(const GGroup* grp) const {return(Obj->Similarity(grp));}
	virtual double SimilarityIFF(const GGroup* grp) const throw(GException) {return(Obj->SimilarityIFF(grp));}
	virtual void InsertFdbk(unsigned int id) throw(std::bad_alloc) {Obj->InsertFdbk(id);}
	virtual void DeleteFdbk(unsigned int id) throw(std::bad_alloc) {Obj->DeleteFdbk(id);}
	virtual void ClearFdbks(void) {Obj->ClearFdbks();}
	virtual unsigned int GetNbFdbks(void) const {return(Obj->GetNbFdbks());}
	virtual unsigned int GetNbLinks(void) {return(Obj->GetNbLinks());}
	virtual void InsertLink(const GDoc* doc) throw(std::bad_alloc) {Obj->InsertLink(doc);}
	virtual void InsertLink(const GDoc* doc, unsigned int nbOccurs) throw(std::bad_alloc) {Obj->InsertLink(doc,nbOccurs);}
	virtual R::RCursor<GLink> GetLinkCursor(void) {return(Obj->GetLinkCursor());}
	virtual void Update(GLang* lang,R::RContainer<GWeightInfo,false,true>* infos,bool computed) {Obj->Update(lang,infos,computed);}
	virtual ~GDocProxyMem(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
