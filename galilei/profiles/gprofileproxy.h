/*

	GALILEI Research Project

	GProfileProxy.h

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
#ifndef GPROFILEPROXY_H
#define GPROFILEPROXY_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gprofile.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// ClassGProfileProxy
class GProfileProxy
{
public:
	GProfileProxy(void) {}
	virtual int Compare(const GProfileProxy* ptr) const=0;
	virtual int Compare(const unsigned int id) const=0;
	virtual void SetSubject(GSubject* s)=0;
	virtual GSubject* GetSubject(void)=0;
	virtual unsigned int GetId(void)=0;
	virtual void SetId(unsigned int id)=0;
	virtual R::RString GetName(void)=0;
	virtual const GUser* GetUser(void)=0;
	virtual bool IsSocial(void)=0;
	virtual void SetSocial(bool social)=0;
	virtual GSubProfile* GetSubProfile(const GLang* lang)=0;
	virtual GSubProfile* GetInsertSubProfile(GLang* lang,GSession* s)=0;
	virtual GProfDoc* GetFeedback(const GDoc* doc)=0;
	virtual unsigned int GetNbAssessedDocs(const GLang* lang)=0;
	virtual GProfDocCursor GetProfDocCursor(void)=0;
	virtual GSubProfileCursor GetSubProfilesCursor(void)=0;
	virtual void InsertFdbk(GProfDoc* j,GSession* s)=0;
	virtual void Modify(GDoc* doc,GLang* newlang,GLang* oldlang)=0;
	virtual ~GProfileProxy(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
