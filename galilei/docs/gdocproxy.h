/*

	GALILEI Research Project

	GDocProxy.h

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
#ifndef GDOCPROXY_H
#define GDOCPROXY_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ClassGDocProxy
class GDocProxy
{
public:
	GDocProxy(void) {}
	virtual int Compare(const GDocProxy* ptr) const=0;
	virtual int Compare(const unsigned int id) const=0;
	virtual bool IsDefined(void) const=0;
	virtual R::RString GetURL(void) const=0;
	virtual R::RString GetName(void) const=0;
	virtual void SetName(const R::RString& name) throw(std::bad_alloc)=0;
	virtual R::RDate GetUpdated(void) const=0;
	virtual R::RDate GetComputed(void) const=0;
	virtual R::RString GetMIMEType(void) const=0;
	virtual void SetMIMEType(const R::RString& mime)=0;
	virtual tObjState GetState(void) const=0;
	virtual void SetState(tObjState state)=0;
	virtual GLang* GetLang(void) const=0;
	virtual unsigned int GetId(void) const=0;
	virtual void SetId(unsigned int id) throw(GException)=0;
	virtual unsigned int GetFailed(void) const=0;
	virtual void InitFailed(void)=0;
	virtual void IncFailed(void)=0;
	virtual void DecFailed(void)=0;
	virtual R::RCursor<GProfileProxy> GetFdbks(void)=0;
	virtual R::RCursor<GWeightInfo> GetWeightInfoCursor(void)=0;
	virtual const GWeightInfos* GetInfos(void) const=0;
	virtual double GetMaxWeight(void) const throw(GException)=0;
	virtual double Similarity(const GDoc* doc) const=0;
	virtual double SimilarityIFF(const GDoc* doc) const throw(GException)=0;
	virtual double Similarity(const GSubProfile* sub) const=0;
	virtual double SimilarityIFF(const GSubProfile* sub) const throw(GException)=0;
	virtual double Similarity(const GGroup* grp) const=0;
	virtual double SimilarityIFF(const GGroup* grp) const throw(GException)=0;
	virtual void InsertFdbk(unsigned int id) throw(std::bad_alloc)=0;
	virtual void DeleteFdbk(unsigned int id) throw(std::bad_alloc)=0;
	virtual void ClearFdbks(void)=0;
	virtual unsigned int GetNbLinks(void)=0;
	virtual void InsertLink(const GDoc* doc) throw(std::bad_alloc)=0;
	virtual void InsertLink(const GDoc* doc, unsigned int nbOccurs) throw(std::bad_alloc)=0;
	virtual GLinkCursor GetLinkCursor(void)=0;
	virtual void InsertSubject(GSubject* s)=0;
	virtual bool IsFromSubject(const GSubject* s)=0;
	virtual bool IsFromParentSubject(const GSubject* s)=0;
	virtual GSubjectCursor GetSubjectCursor(void)=0;
	virtual unsigned int GetNbSubjects(void)=0;
	virtual void Update(GLang* lang,R::RContainer<GWeightInfo,false,true>* infos)=0;
	virtual ~GDocProxy(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
