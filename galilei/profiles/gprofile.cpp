/*

	GALILEI Research Project

	gprofile.cpp

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

#include <stdlib.h>

//	include files for GALILEI
#include<gprofile.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
//  GProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GProfile::GProfile(GUser *owner) throw(bad_alloc)
  : RContainer<GSubProfile,unsigned,true,true>(10,5),Owner(owner),Id(cNoRef),Name(),
    Modified(false),FdbkDocs(100,50)
{
}


//-----------------------------------------------------------------------------
GProfile::GProfile(GUser *owner,unsigned int id,RString &name) throw(bad_alloc)
  : RContainer<GSubProfile,unsigned,true,true>(10,5),Owner(owner),Id(id),Name(name),
    Modified(false),FdbkDocs(100,50)
{
}


//-----------------------------------------------------------------------------
int GProfile::Compare(const unsigned int &id)
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GProfile::Compare(const GProfile &profile)
{
  return(Id-profile.Id);
}


//-----------------------------------------------------------------------------
int GProfile::Compare(const GProfile *profile)
{
  return(Id-profile->Id);
}


//-----------------------------------------------------------------------------
void GProfile::Calc(void)
{
	GSubProfile *sub,**subs;
	unsigned int i;

	for(FdbkDocs.Start();!FdbkDocs.End();FdbkDocs.Next())
		if(!FdbkDocs()->Doc->MustAnalyse())
		{
			sub=GetPtr<GLang*>(FdbkDocs()->Doc->GetLang(),false);
			if(sub)
				sub->Analyse(FdbkDocs());
		}
	for(i=NbPtr+1,subs=Tab;--i;subs++)
		(*subs)->Analyse();
	Modified=true;
}


