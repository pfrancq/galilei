/*

	GALILEI Research Project

	GMIMEFilter.h

	Represents a couple file extention / filter - Implementation.

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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gmimefilter.h>
#include <filters/gfilter.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GMIMEFilter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GMIMEFilter::GMIMEFilter(const char* t,GFilter* f)
	: Type(t), Filter(f)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const GMIMEFilter* f) const
{
	return(Type.Compare(f->Type));
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const GMIMEFilter& f) const
{
	return(Type.Compare(f.Type));
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const RString& t) const
{
	return(Type.Compare(t));
}


//-----------------------------------------------------------------------------
int GALILEI::GMIMEFilter::Compare(const char* t) const
{
	return(Type.Compare(t));
}


//-----------------------------------------------------------------------------
GALILEI::GMIMEFilter::~GMIMEFilter(void)
{
}
