/*

	GALILEI Research Project

	GSubProfileProxyMem.h

	Proxy for class GSubProfile - Header.

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
	virtual { return(Obj->()); }
	virtual void AddAssessment(GProfDoc* j){ Obj->AddAssessment(j); }
	virtual void RemoveAssessment(GProfDoc* j){ Obj->RemoveAssessment(j); }
	virtual void ClearFdbks(void){ Obj->ClearFdbks(); }
	virtual unsigned int GetId(void){ return(Obj->GetId()); }
	virtual void SetId(unsigned int id){ Obj->SetId(id); }
	virtual tObjState GetState(void){ return(Obj->GetState()); }
	virtual void SetState(tObjState state){ Obj->SetState(state); }
	virtual GLang* GetLang(void){ return(Obj->GetLang()); }
	virtual GProfile* GetProfile(void){ return(Obj->GetProfile()); }
	virtual { return(Obj->()); }
	virtual bool IsDefined(void){ return(Obj->IsDefined()); }
	virtual GGroup* GetGroup(void){ return(Obj->GetGroup()); }
	virtual void SetGroup(GGroup* grp){ Obj->SetGroup(grp); }
	virtual R::RDate GetAttached(void){ return(Obj->GetAttached()); }
	virtual R::RDate GetUpdated(void){ return(Obj->GetUpdated()); }
	virtual R::RDate GetComputed(void){ return(Obj->GetComputed()); }
	virtual bool IsUpdated(void){ return(Obj->IsUpdated()); }
	virtual unsigned int GetCommonOKDocs(const GSubProfile* prof){ return(Obj->GetCommonOKDocs(prof)); }
	virtual unsigned int GetCommonDocs(const GSubProfile* prof){ return(Obj->GetCommonDocs(prof)); }
	virtual unsigned int GetCommonDiffDocs(const GSubProfile* prof){ return(Obj->GetCommonDiffDocs(prof)); }
	virtual unsigned int GetNbAssessedDocs(void){ return(Obj->GetNbAssessedDocs()); }
	virtual GProfDoc* GetFeedback(const GDoc* doc){ return(Obj->GetFeedback(doc)); }
	virtual GProfDocCursor GetProfDocCursor(void){ return(Obj->GetProfDocCursor()); }
	virtual GWeightInfoCursor GetWeightInfoCursor(void){ return(Obj->GetWeightInfoCursor()); }
	virtual unsigned int GetNbNoNull(void){ return(Obj->GetNbNoNull()); }
	virtual double Similarity(const GDoc* doc){ return(Obj->Similarity(doc)); }
	virtual double SimilarityIFF(const GDoc* doc){ return(Obj->SimilarityIFF(doc)); }
	virtual double Similarity(const GSubProfile* sub){ return(Obj->Similarity(sub)); }
	virtual double SimilarityIFF(const GSubProfile* sub){ return(Obj->SimilarityIFF(sub)); }
	virtual double Similarity(const GGroup* grp){ return(Obj->Similarity(grp)); }
	virtual double SimilarityIFF(const GGroup* grp){ return(Obj->SimilarityIFF(grp)); }
	virtual void UpdateFinished(void){ Obj->UpdateFinished(); }
	virtual void SetSubject(GSubject* s){ Obj->SetSubject(s); }
	virtual GSubject* GetSubject(void){ return(Obj->GetSubject()); }
	virtual void UpdateRefs(void){ Obj->UpdateRefs(); }
	virtual void RemoveRefs(void){ Obj->RemoveRefs(); }
	virtual ~GSubProfileProxyMem(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
