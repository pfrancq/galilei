/*

	GALILEI Research Project

	GLangs.h

	List of the languages - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>


//-----------------------------------------------------------------------------
// include file for Galilei
#include <langs/glangs.h>
#include <langs/glang.h>
#include <langs/glangen.h>
#include <langs/glangfr.h>
#include <langs/gdicts.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GLangs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLangs::GLangs(unsigned nb) throw(bad_alloc)
  : RContainer<GLang,unsigned,true,true>(nb,nb/2), bDics(false), Stops(0), Dics(0)
{
	Dics=new GDicts(nb);
	Stops=new GDicts(nb);
	InsertPtr(new GLangEN(0));
	InsertPtr(new GLangFR(0));
}


//-----------------------------------------------------------------------------
GLang* GALILEI::GLangs::GetLang(const char* code)
{
	if(!code)
		return(0);
	return(GetPtr<const char*>(code));
}


//-----------------------------------------------------------------------------
GLangCursor& GALILEI::GLangs::GetLangsCursor(void)
{
	GLangCursor *cur=GLangCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GLangs::InitDics(void) throw(bad_alloc,GException)
{
	// If dictionnary already loaded, do nothing.
	if(bDics) return;

	// For each Lang, create a dictionnary and a stop list
	GLangCursor CurLang=GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{ 
		LoadDic(CurLang()->GetCode(),true);
		LoadDic(CurLang()->GetCode(),false);
	}
	bDics=true;
}


//-----------------------------------------------------------------------------
GDict* GALILEI::GLangs::GetDic(const GLang *lang) const throw(GException)
{
	return(Dics->GetPtr<const GLang*>(lang,false));
}


//-----------------------------------------------------------------------------
GDict* GALILEI::GLangs::GetStop(const GLang *lang) const throw(GException)
{
	return(Stops->GetPtr<const GLang*>(lang,false));
}


//-----------------------------------------------------------------------------
const char* GALILEI::GLangs::GetWord(const unsigned int id,const GLang* lang)
{
	if(bDics)
		return(Dics->GetPtr<const GLang*>(lang,false)->GetWord(id));
	return(LoadWord(id,lang->GetCode()));
}


//-----------------------------------------------------------------------------
const char* GALILEI::GLangs::GetWord(const unsigned int id,const GDict* dict) const
{
	if(bDics&&dict)
		return(dict->GetWord(id));
	return(0);
}


//-----------------------------------------------------------------------------
GALILEI::GLangs::~GLangs(void)
{
	if(Dics) delete Dics;
	if(Stops) delete Stops;
}
