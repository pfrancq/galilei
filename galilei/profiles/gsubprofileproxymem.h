/*

	GALILEI Research Project

	GSubProfileProxyMem.h

	Proxy for class GSubProfile - Header.

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
#ifndef GSUBPROFILEPROXYMEM_H
#define GSUBPROFILEPROXYMEM_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gsubprofileproxy.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ClassGSubProfileProxyMem
class GSubProfileProxyMem : public GSubProfileProxy
{
protected:
	GSubProfile* Obj;
public:
	GSubProfileProxyMem(GSubProfile* obj) : GSubProfileProxy(), Obj(obj) {}
	virtual int Compare(const GSubProfileProxy& ptr) const { return(Obj->Compare(*dynamic_cast<const GSubProfileProxyMem&>(ptr).Obj)); }
	virtual int Compare(const GSubProfileProxy* ptr) const { return(Obj->Compare(*dynamic_cast<const GSubProfileProxyMem*>(ptr)->Obj)); }
	virtual int Compare(const unsigned int id) const { return(Obj->Compare(id)); }
	virtual void InsertFdbk(GFdbk* fdbk) throw(std::bad_alloc) {Obj->InsertFdbk(fdbk);}
	virtual unsigned int GetId(void) const {return(Obj->GetId());}
	virtual void SetId(unsigned int id) throw(GException) {Obj->SetId(id);}
	virtual tObjState GetState(void) const {return(Obj->GetState());}
	virtual void SetState(tObjState state) {Obj->SetState(state);}
	virtual GLang* GetLang(void) const {return(Obj->GetLang());}
	virtual GProfile* GetProfile(void) const {return(Obj->GetProfile());}
	virtual bool IsDefined(void) const {return(Obj->IsDefined());}
	virtual bool CanCompute(void) {return(Obj->CanCompute());}
	virtual GGroup* GetGroup(void) const {return(Obj->GetGroup());}
	virtual void SetGroup(GGroup* grp) {Obj->SetGroup(grp);}
	virtual R::RDate GetAttached(void) const {return(Obj->GetAttached());}
	virtual R::RDate GetUpdated(void) const {return(Obj->GetUpdated());}
	virtual R::RDate GetComputed(void) const {return(Obj->GetComputed());}
	virtual bool IsUpdated(void) const {return(Obj->IsUpdated());}
	virtual bool SameDescription(const GFdbk* fdbk) const {return(Obj->SameDescription(fdbk));}
	virtual unsigned int GetCommonOKDocs(const GSubProfile* prof) {return(Obj->GetCommonOKDocs(prof));}
	virtual unsigned int GetCommonDocs(const GSubProfile* prof)  {return(Obj->GetCommonDocs(prof));}
	virtual unsigned int GetCommonDiffDocs(const GSubProfile* prof) {return(Obj->GetCommonDiffDocs(prof));}
	virtual unsigned int GetNbAssessedDocs(void) const {return(Obj->GetNbAssessedDocs());}
	virtual R::RCursor<GFdbk> GetFdbks(void) {return(Obj->GetFdbks());}
	virtual R::RCursor<GWeightInfo> GetWeightInfoCursor(void) {return(Obj->GetWeightInfoCursor());}
	virtual unsigned int GetNbNoNull(void) const {return(Obj->GetNbNoNull());}
	virtual double Similarity(const GDoc* doc) const {return(Obj->Similarity(doc));}
	virtual double SimilarityIFF(const GDoc* doc) const throw(GException) {return(Obj->SimilarityIFF(doc));}
	virtual double Similarity(const GDocProxy* doc) const {return(Obj->Similarity(doc));}
	virtual double SimilarityIFF(const GDocProxy* doc) const throw(GException) {return(Obj->SimilarityIFF(doc));}
	virtual double Similarity(const GSubProfile* sub) const {return(Obj->Similarity(sub));}
	virtual double SimilarityIFF(const GSubProfile* sub) const throw(GException) {return(Obj->SimilarityIFF(sub));}
	virtual double Similarity(const GGroup* grp) const {return(Obj->Similarity(grp));}
	virtual double SimilarityIFF(const GGroup* grp) const throw(GException) {return(Obj->SimilarityIFF(grp));}
	virtual void Update(R::RContainer<GWeightInfo,false,true>* infos,bool computed) {Obj->Update(infos,computed);}
	virtual void ClearFdbks(void) {Obj->ClearFdbks();}
	virtual ~GSubProfileProxyMem(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
