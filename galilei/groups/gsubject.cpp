/*

	GALILEI Research Project

	GSubject.cpp

	GSubject - Implementation.

	(C) 1999-2001 by J. Lamoral & D.Wartel

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
//include files for R Project
#include <rstd/rcontainercursor.h>
#include <rio/rtextfile.h>
using namespace RIO;
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gsubject.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <profiles/gprofile.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
#include <langs/glang.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//include specific files
#include <stdlib.h>
#include <iostream.h>
#include <math.h>




//-----------------------------------------------------------------------------
//
// class GSubject
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSubject::GSubject(RString name, int id)
	 : RNode<GSubject,false>(10,2)
{
	IsJudged=0;
	urls=new RContainer<GDoc,unsigned,false,false> (10,5);
	Name=name;
	Id=id;
}


//-----------------------------------------------------------------------------
int GSubject::SubSubjectMinId(void)
{
	int min;
	GSubject **s1;
	unsigned int i;

	for(s1=this->Tab,i=this->NbPtr;--i;s1++)
	{
		GSubject* subject=(*s1);
		min=subject->GetId();
	}
	
	for(s1=this->Tab,i=this->NbPtr;--i;s1++)
	{
		GSubject* subject=(*s1);
		if (subject->GetId()<min) min=subject->GetId();
	}
	return min;
}


//-----------------------------------------------------------------------------
void GSubject::SetLang(const GLang* lang)
{
	Lang=lang->GetCode();
}


//-----------------------------------------------------------------------------
GSubject::~GSubject(void)
{
}
