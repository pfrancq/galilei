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
	virtual bool IsDefined(void)=0;
	virtual void ClearInfos(bool l)=0;
	virtual void ClearFdbks(void)=0;
	virtual R::RString GetURL(void)=0;
	virtual R::RString GetName(void)=0;
	virtual void SetName(const R::RString& name)=0;
	virtual R::RDate GetUpdated(void)=0;
	virtual R::RDate GetComputed(void)=0;
	virtual R::RString GetMIMEType(void)=0;
	virtual void SetMIMEType(const R::RString& mime)=0;
	virtual tObjState GetState(void)=0;
	virtual void SetState(tObjState state)=0;
	virtual GLang* GetLang(void)=0;
	virtual void SetLang(GLang* l)=0;
	virtual unsigned int GetId(void)=0;
	virtual void SetId(unsigned int id)=0;
	virtual unsigned int GetFailed(void)=0;
	virtual void InitFailed(void)=0;
	virtual void IncFailed(void)=0;
	virtual void DecFailed(void)=0;
	virtual ~GDocProxy(void) {}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
