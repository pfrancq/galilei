/*

	GALILEI Research Project

	GFilter.cpp

	Generic Filter for documents - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggalileiapp.h>
#include <gdocfragment.h>
#include <gfilter.h>
#include <gdoc.h>
#include <gdocref.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GFilter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilter::GFilter(GSession* session,GPlugInFactory* fac)
	: GPlugIn(session,fac)
{
}


//---------------------------------------------------------------------------
void GFilter::AddMIME(const RString& name)
{
	GALILEIApp->AddMIME(name,this);
}


//---------------------------------------------------------------------------
void GFilter::AddScheme(const RString& scheme)
{
	GALILEIApp->AddMIME(scheme,this);
}


//------------------------------------------------------------------------------
R::RURI GFilter::GetValidURI(const R::RURI& uri,bool& mustdelete)
{
	mustdelete=false;
	return(uri);
}


//------------------------------------------------------------------------------
RString GFilter::GetTextFragment(GDoc* doc,size_t begin,size_t end)
{
	if(begin>end)
		mThrowGException("Text fragment problem begin ("+RString::Number(begin)+") is greater than end ("+RString::Number(end));

	// Look the size to print
	size_t Size(end-begin+1);

	// Extract maximum 205 characters
	RString Fragment;
	Fragment.SetLen(Size);
	Fragment.SetLen(0);

	// Open the file and put it at the beginning of the window position
	GFilter* Filter(GALILEIApp->FindScheme(doc));
	bool Delete,ToManage;
	RURI URI;
	if(Filter)
	{
		ToManage=true;
		URI=Filter->GetValidURI(doc->GetName(),Delete);
	}
	else
	{
		ToManage=false;
		URI=doc->GetName();
	}
	RTextFile File(URI,"utf-8");
	File.Open(RIO::Read);
	File.Seek(begin);

	// Extract the fragment
	while(!File.End())
	{
		RChar c(File.GetChar());
		if(File.GetPos()>end)
			break;
		Fragment+=c;
	}

	File.Close();
	if(ToManage&&Delete)
		RFile::RemoveFile(URI);
	return(Fragment);
}


//------------------------------------------------------------------------------
GFilter::~GFilter(void)
{
}
