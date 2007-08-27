/*

	GALILEI Research Project

	XMLParser.cpp

	A XML Parser to extract information entities - Implementation.

	Copyright 2007 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <gdoc.h>


//-----------------------------------------------------------------------------
// include files for this plug-in
#include <xmlparser.h>



//-----------------------------------------------------------------------------
//
// class XMLParser
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
XMLParser::XMLParser(GTextAnalyse* filter,const RURI& uri)
	: RXMLFile(uri,0,"UTF-8"), Filter(filter), IsTitle(false)
{
}


//-----------------------------------------------------------------------------
void XMLParser::BeginTag(const RString& namespaceURI,const RString& lName,const RString&,RContainer<RXMLAttr,true,true>& attrs)
{
	RString index;
	
	if(namespaceURI.IsEmpty())
		index=Filter->Doc->GetURL()+":"+lName;
	else
		index=namespaceURI+":"+lName;
	
	// Look if the current tag is title meta data
	if(index=="http://purl.org/dc/elements/1.1/:title")
	{
		Content.Clear();
		IsTitle=true;
	}
	
	if(Filter->ExtractStruct)
	{		
		if((Filter->StructIsContent)||(Filter->AttrValues))
		{
			// Parse through the attribute to index tokens and/or values as stems
			const RChar* ptr;
			RString Add(lName);
			RString Val;
			RCursor<RXMLAttr> Cur(attrs);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				if(Filter->StructIsContent)
					Add+=Cur()->GetName();
				if(Filter->AttrValues)
					Val+=Cur()->GetValue();
			}
			if(Filter->StructIsContent)
			{
				// Tokens must be indexed as stems
				ptr=Add();
				while(!ptr->IsNull())
					Filter->ExtractWord(ptr,Filter->WeightStruct);
			}
			if(Filter->AttrValues)
			{
				// Values must be indexed as stems
				ptr=Val();
				Filter->ExtractWord(ptr,Filter->WeightValues);
			}
		}
		if(!Filter->StructIsContent)
		{
			// Index tag and attributes in the token spaces
		}
	}
}


//-----------------------------------------------------------------------------
void XMLParser::EndTag(const RString&, const RString&, const RString&)
{
	if(IsTitle&&(!Content.IsEmpty()))
	{
		Filter->Doc->SetName(Content.Trim());
		IsTitle=false;
	}
}


//-----------------------------------------------------------------------------
void XMLParser::Text(const RString& text)
{
	if(!text.IsEmpty())
	{
		double w;
		if(IsTitle)
		{
			Content+=text;
			w=2.0;
		}
		else
			w=1.0;
		const RChar* ptr=text();
		while(!ptr->IsNull())
			Filter->ExtractWord(ptr,w);
	}
}
