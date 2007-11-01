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
#include <gconcepttype.h>
#include <gconcept.h>
#include <gweightinfo.h>


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
	: RXMLFile(uri,0,"UTF-8"), Filter(filter), IsTitle(false), Contents(20)
{
}


//-----------------------------------------------------------------------------
void XMLParser::BeginTag(const RString& namespaceURI,const RString& lName,const RString&,RContainer<RXMLAttr,true,true>& attrs)
{
	RString index;
	
	// Add tags
	Filter->NbTags++;
	
	// Push current content
	Contents.Push(new RString(Content));
	Content.Clear();
	
	if(namespaceURI.IsEmpty())
		index=Filter->Doc->GetURL();
	else
		index=namespaceURI;
	
	// Look if the current tag is title meta data
	if(index+":"+lName=="http://purl.org/dc/elements/1.1/:title")
	{
		IsTitle=true;
		return;
	}
	
	// If the structure must not be extracted -> return from the method	
	if(!Filter->ExtractStruct)
		return;

	RString StructStems;  // String containing structure elements supposed to be considered as stems 
	RString ValuesStems;  // String containing parameters values supposed to be considered as stems
	
	// Always index the structure
	AddStructElement(index+":"+lName,GetCurrentDepth());
	
	// If tags are content -> add short name of the tag
	if(Filter->StructIsContent)
		StructStems+=lName+" ";	
	
	RCursor<RXMLAttr> Cur(attrs);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Always index the structure
		AddStructElement(Cur()->GetFullName(),GetCurrentDepth());
		
		// If attributes names are content -> add short parameter name 
		if(Filter->StructIsContent)
			StructStems+=Cur()->GetName()+" ";

					
		// If attributes values are content -> add short parameter name
		if(Filter->AttrValues)
			ValuesStems+=Cur()->GetValue()+" ";
	}
	
	// If StructStems is not empty -> Add them
	if(!StructStems.IsEmpty())
	{
		const RChar* ptr=StructStems();
		Filter->ExtractWord(ptr,Filter->WeightStruct,0);
	}
	
	// If ValuesStems is not empty -> Add them
	if(!ValuesStems.IsEmpty())
	{
		const RChar* ptr=ValuesStems();
		Filter->ExtractWord(ptr,Filter->WeightValues,0);
	}
}


//-----------------------------------------------------------------------------
void XMLParser::EndTag(const RString& namespaceURI, const RString& lName, const RString&)
{
	if(Content.IsEmpty())
		return;
	if(IsTitle)
	{
		Filter->Doc->SetName(Content.Trim());
		IsTitle=false;
	}
	if(Filter->ExtractIndex)
	{
		RString index;
		if(namespaceURI.IsEmpty())
			index=Filter->Doc->GetURL();
		else
			index=namespaceURI;		
		AddIndex(index+":"+lName,Content,GetCurrentDepth());
	}
	
	// Pop current content
	Content=(*Contents());
	Contents.Pop();
}


//-----------------------------------------------------------------------------
void XMLParser::Text(const RString& text)
{
	if(!text.IsEmpty())
	{
		double w;
		if(IsTitle||Filter->ExtractIndex)
			Content+=text;
		if(IsTitle)
			w=2.0;
		else
			w=1.0;
		const RChar* ptr=text();
		while(!ptr->IsNull())
			Filter->ExtractWord(ptr,w,0);
	}
}


//------------------------------------------------------------------------------
RChar XMLParser::CodeToChar(RString& str)
{
	RChar res=RXMLFile::CodeToChar(str);
	if(res!=0)
		return(res);
	return(' ');
}


//-----------------------------------------------------------------------------
void XMLParser::AddStructElement(const RString& element,size_t)
{
	GConcept w(cNoRef,element,Filter->StructSpace,0,0,0);
	GWeightInfo* info=Filter->Infos.GetInsertPtr(Filter->StructSpace->InsertConcept(&w));
	(*info)+=1.0;
}


//-----------------------------------------------------------------------------
void XMLParser::AddIndex(const RString& element,const RString& content,size_t depth)
{
	bool Idx=false;
	
	// Find corresponding tag
	IndexTag* ptr=Filter->IndexTags.GetInsertPtr(element);
	ptr->Occurs++;
	
	// If maximum depth, cannot be used
	if(depth>Filter->MaxDepth)
		return;
		
	// Count number of terms -> if too much, cannot be used
	unsigned int NbTerms(0);
	const RChar* car=content();
	RString Index;
	while(!car->IsNull())
	{
		// Skip spaces
		while((!car->IsNull())&&(car->IsSpace()))
				car++;
	
		// Is this a term?
		if(!car->IsNull())
		{
			if(Idx)
				Index+=' ';
			
			// Yes -> skip it and increase
			NbTerms++;
			while((!car->IsNull())&&(!car->IsSpace()))
					Index+=(*(car++));
			Idx=true;			
		}
	}	
	if(NbTerms>Filter->MaxTerms)
		return;
	
	// OK Index terms
	car=Index();
	while(!car->IsNull())
		Filter->ExtractWord(car,1.0,ptr);	
}
