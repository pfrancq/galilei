/*

	GALILEI Research Project

	GSubProfileProxy.h

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
#ifndef GSUBPROFILEPROXY_H
#define GSUBPROFILEPROXY_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gsubprofile.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ClassGSubProfileProxy
class GSubProfileProxy
{
public:
	GSubProfileProxy(void) {}
	virtual =0;
	virtual void AddAssessment(GProfDoc* j)=0;
	virtual void RemoveAssessment(GProfDoc* j)=0;
	virtual void ClearFdbks(void)=0;
	virtual unsigned int GetId(void)=0;
	virtual void SetId(unsigned int id)=0;
	virtual tObjState GetState(void)=0;
	virtual void SetState(tObjState state)=0;
	virtual GLang* GetLang(void)=0;
	virtual GProfile* GetProfile(void)=0;
	virtual =0;
	virtual bool IsDefined(void)=0;
	virtual GGroup* GetGroup(void)=0;
	virtual void SetGroup(GGroup* grp)=0;
	virtual R::RDate GetAttached(void)=0;
	virtual R::RDate GetUpdated(void)=0;
	virtual R::RDate GetComputed(void)=0;
	virtual bool IsUpdated(void)=0;
	virtual unsigned int GetCommonOKDocs(const GSubProfile* prof)=0;
	virtual unsigned int GetCommonDocs(const GSubProfile* prof)=0;
	virtual unsigned int GetCommonDiffDocs(const GSubProfile* prof)=0;
	virtual unsigned int GetNbAssessedDocs(void)=0;
	virtual GProfDoc* GetFeedback(const GDoc* doc)=0;
	virtual GProfDocCursor GetProfDocCursor(void)=0;
	virtual GWeightInfoCursor GetWeightInfoCursor(void)=0;
	virtual unsigned int GetNbNoNull(void)=0;
	virtual double Similarity(const GDoc* doc)=0;
	virtual double SimilarityIFF(const GDoc* doc)=0;
	virtual double Similarity(const GSubProfile* sub)=0;
	virtual double SimilarityIFF(const GSubProfile* sub)=0;
	virtual double Similarity(const GGroup* grp)=0;
	virtual double SimilarityIFF(const GGroup* grp)=0;
	virtual void UpdateFinished(void)=0;
	virtual void SetSubject(GSubject* s)=0;
	virtual GSubject* GetSubject(void)=0;
	virtual void UpdateRefs(void)=0;
	virtual void RemoveRefs(void)=0;
	virtual ~GSubProfileProxy(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
