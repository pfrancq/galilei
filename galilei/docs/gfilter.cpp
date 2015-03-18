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
void GFilter::AddMIME(RString name)
{
	GALILEIApp->AddMIME(name,this);
}


//------------------------------------------------------------------------------
RString GFilter::GetTextFragment(GDocFragment* fragment)
{
	//cout<<"Extract at "<<fragment->GetPos()<<" in ["<<fragment->GetBegin()<<","<<fragment->GetEnd()<<"]"<<endl;

	// Look the size to print
	size_t Begin(fragment->GetBegin());
	size_t End(fragment->GetEnd());
	size_t Size(End-Begin+1);

	// Extract maximum 205 characters
	RString Fragment;
	Fragment.SetLen(205);
	Fragment.SetLen(0);

	// Open the file and put it at the beginning of the window position
	RTextFile File(fragment->GetDoc()->GetURI(),"utf-8");
	File.Open(RIO::Read);
	File.Seek(Begin);

	// Extract the fragment
	while(!File.End())
	{
		RChar c(File.GetChar());
		if(File.GetPos()>End)
			break;
		Fragment+=c;
	}

	return(Fragment);
}


//------------------------------------------------------------------------------
GFilter::~GFilter(void)
{
}
