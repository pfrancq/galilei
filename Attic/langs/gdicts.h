/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/
#ifndef   GDictsH
#define   GDictsH


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rhashcontainer.h>
#include <rstd/rstring.h>

using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>
#include <gsessions/gsession.h>
#include <glangs/glang.h>
#include <glangs/gdict.h>


//---------------------------------------------------------------------------
namespace GALILEI{
//---------------------------------------------------------------------------
class GDict;
class GSession;

//---------------------------------------------------------------------------
// class GDicts
class GDicts : public RContainer<GDict,unsigned,true,true>
{

  GSession *Session;
public:
  GDicts(unsigned nb,GSession *session) throw(bad_alloc);
  void Load(bool dep,bool del) throw(bad_alloc,GException);
  //virtual void CreateDic(const RString &name,GLang *lang) throw(bad_alloc,GException)=0;
  void Clear(void);
	virtual ~GDicts(void) {}
};




}  //-------- End of namespace Galilei-----------------------------------


//---------------------------------------------------------------------------
#endif
