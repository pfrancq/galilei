/*

	GALILEI Research Project

	GSubProfileProxy.h

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
	virtual int Compare(const GSubProfileProxy* ptr) const=0;
	virtual int Compare(const unsigned int id) const=0;
	virtual void InsertFdbk(GFdbk* fdbk) throw(std::bad_alloc)=0;
	virtual unsigned int GetId(void) const=0;
	virtual void SetId(unsigned int id) throw(GException)=0;
	virtual tObjState GetState(void) const=0;
	virtual void SetState(tObjState state)=0;
	virtual GLang* GetLang(void) const=0;
	virtual GProfile* GetProfile(void) const=0;
	virtual bool IsDefined(void) const=0;
	virtual bool CanCompute(void)=0;
	virtual GGroup* GetGroup(void) const=0;
	virtual void SetGroup(GGroup* grp)=0;
	virtual R::RDate GetAttached(void) const=0;
	virtual R::RDate GetUpdated(void) const=0;
	virtual R::RDate GetComputed(void) const=0;
	virtual bool IsUpdated(void) const=0;
	virtual bool SameDescription(const GFdbk* fdbk) const=0;
	virtual unsigned int GetCommonOKDocs(const GSubProfile* prof)=0;
	virtual unsigned int GetCommonDocs(const GSubProfile* prof) =0;
	virtual unsigned int GetCommonDiffDocs(const GSubProfile* prof)=0;
	virtual unsigned int GetNbAssessedDocs(void) const=0;
	virtual R::RCursor<GFdbk> GetFdbks(void)=0;
	virtual R::RCursor<GWeightInfo> GetWeightInfoCursor(void)=0;
	virtual unsigned int GetNbNoNull(void) const=0;
	virtual double Similarity(const GDoc* doc) const=0;
	virtual double SimilarityIFF(const GDoc* doc) const throw(GException)=0;
	virtual double Similarity(const GDocProxy* doc) const=0;
	virtual double SimilarityIFF(const GDocProxy* doc) const throw(GException)=0;
	virtual double Similarity(const GSubProfile* sub) const=0;
	virtual double SimilarityIFF(const GSubProfile* sub) const throw(GException)=0;
	virtual double Similarity(const GGroup* grp) const=0;
	virtual double SimilarityIFF(const GGroup* grp) const throw(GException)=0;
	virtual void Update(R::RContainer<GWeightInfo,false,true>* infos,bool computed)=0;
	virtual void ClearFdbks(void)=0;
	virtual ~GSubProfileProxy(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
