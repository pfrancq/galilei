/*

	GALILEI Research Project

	GSubProfileDesc.cpp

	Generic Subprofile Description Method - Implementation.

	(C) 2002 by P. Francq.

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
//include files for GALILEI
#include<profiles/gsubprofiledesc.h>
#include<profiles/gsubprofile.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GSubProfileDesc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubProfileDesc::GSubProfileDesc(const char* name,GSubProfile* (*func)(GProfile* prof,unsigned int id,GLang*,GGroup*,const char*)) throw(bad_alloc)
	: ProfDescName(name), Create(func)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfileDesc::Compare(const GSubProfileDesc& desc) const
{
	return(ProfDescName.Compare(desc.ProfDescName));
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfileDesc::Compare(const GSubProfileDesc* desc) const
{
	return(ProfDescName.Compare(desc->ProfDescName));
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfileDesc::Compare(const char* name) const
{
	return(ProfDescName.Compare(name));
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfileDesc::~GSubProfileDesc(void)
{
}
