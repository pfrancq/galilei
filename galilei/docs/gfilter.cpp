/*

	GALILEI Research Project

	GFilter.cpp

	Generic Filter for documents - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
	cout<<"Extract at "<<fragment->GetPos()<<" in ["<<fragment->GetFirst()<<","<<fragment->GetLast()<<"]"<<endl;
	RString Fragment;
	size_t Extract(0);
	if(fragment->GetLast()==cNoRef)
		Extract=80;
	RTextFile File(fragment->GetURI(),"utf-8");
	File.Open(RIO::Read);

	// Read the head of the fragment
	File.Seek(fragment->GetPos());
	while(!File.End())
	{
		RChar c(File.GetChar());
		Fragment+=c;
		if(Extract)
			Extract--;
		else if(c.IsPunct()||c.IsSpace())
			break;
	}

	// If not necessary -> skip the rest
	if(fragment->GetLast()==cNoRef)
		return(Fragment);
	Fragment+="\n";

	// Look the size to print
	size_t First(fragment->GetFirst());
	size_t Last(fragment->GetLast());
	size_t Size(Last-First+1);

	// Go the first position
	File.Seek(First);
	if(Size>200)
	{
		// Read the first and the last 100 characters
		size_t Nb(100);
		while(!File.End())
		{
			RChar c(File.GetChar());
			Fragment+=c;
			if(!(--Nb))
				break;
		}
		Fragment+="\n";
		File.Seek(Last-100);
		Nb=100;
		while(!File.End())
		{
			RChar c(File.GetChar());
			Fragment+=c;
			if(!(--Nb))
				break;
		}

	}
	else
	{
		// Print All
		while(!File.End())
		{
			RChar c(File.GetChar());
			if((File.GetPos()>Last)&&(c.IsPunct()||c.IsSpace()))
				break;
			Fragment+=c;
		}
	}
	return(Fragment);
}


//------------------------------------------------------------------------------
GFilter::~GFilter(void)
{
}
