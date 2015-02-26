/*

	GALILEI Research Project

	GXML.h

	XML Parser that retrieves a fragment - Header.

	Copyright 2011-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef GXMLParserH
#define GXMLParserH


//------------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rxmlparser.h>
#include <gdocfragment.h>
#include <gdoc.h>
#include <gdocref.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
class GXMLParser : public RXMLParser
{
public:
	GXMLParser(void) : RXMLParser() {}

	/**
	 * Extract a text fragment.
	 * @param fragment       Fragment to extract.
    * @return a string containing the text fragment.
    */
	inline R::RString GetFragment(GDocFragment* fragment)
	{
		cout<<"Extract at "<<fragment->GetPos()<<" in ["<<fragment->GetBegin()<<","<<fragment->GetEnd()<<"]"<<endl;
		RString Fragment;
		RTextFile File(fragment->GetDoc()->GetURI(),"utf-8");
		File.Open(RIO::Read);

		// Read the head of the fragment
		bool Tag(false);
		if(fragment->GetPos())
		{
			File.Seek(fragment->GetPos()-1);
			RChar c(File.GetChar());
			if(c=='<')
			{
				Tag=true;
				Fragment+='<';
			}
		}
		else
			File.Seek(fragment->GetPos());
		while(!File.End())
		{
			RChar c(File.GetChar());
			Fragment+=c;
			if((Tag&&(c=='>'))||((!Tag)&&((c.IsPunct()||c.IsSpace()))))
				break;
		}
		Fragment+="\n";

		// Look the size to print
		size_t Begin(fragment->GetBegin());
		size_t End(fragment->GetEnd());
		size_t Size(End-Begin+1);

		// Go the first position
		File.Seek(Begin);
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
			Fragment+="\n...........\n";
			File.Seek(End-100);
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
				if((File.GetPos()>End)&&(c.IsPunct()||c.IsSpace()))
					break;
				Fragment+=c;
			}
		}

		return(Fragment);
	}
};

//-----------------------------------------------------------------------------
#endif
