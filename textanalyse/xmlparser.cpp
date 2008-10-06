/*

	GALILEI Research Project

	XMLParser.cpp

	A XML Parser to extract information entities - Implementation.

	Copyright 2007-2008 by the Université libre de Bruxelles.

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
using namespace std;



//-----------------------------------------------------------------------------
//
// class XMLParser
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
XMLParser::XMLParser(GTextAnalyse* filter,const RURI& uri)
	: RXMLParser(uri,"UTF-8"), Filter(filter), IsTitle(false), Contents(20),
	  CurTag(0), CurAttr(0)
{
}


//-----------------------------------------------------------------------------
void XMLParser::BeginTag(const RString& namespaceURI,const RString& lName,const RString&)
{
	RString index;

	// Add tags
	Filter->NbTags++;

	// Push current content
	Contents.Push(new RString(Content));
	Content.Clear();

	if(GetSection()==RXMLParser::Body)
	{
		if(namespaceURI.IsEmpty())
		{
			if(Filter->UseDefaultNamespace)
				index=Filter->DefaultNamespace+":"+lName;
			else
				index=Filter->Doc->GetURL()+":"+lName;
		}
		else
			index=namespaceURI+":"+lName;
	}
	else
		index=lName;

	// Look if the current tag is title meta data
	if(index=="http://purl.org/dc/elements/1.1/:title")
		IsTitle=true;

	// If the structure must not be extracted -> return from the method
	if(!Filter->ExtractStruct)
		return;

	// Always index the structure
	AddStructElement(index,true);

	// If tags are content -> add short name of the tag
	if(Filter->StructIsContent&&(!lName.ContainOnlySpaces()))
		Filter->ExtractWord(lName,Filter->WeightStruct,0,0,0);
}


//------------------------------------------------------------------------------
void XMLParser::AddAttribute(const RString& namespaceURI,const RString& lName, const RString&)
{
	if(!Filter->ExtractStruct)
		return;

	RString index;
	if(GetSection()==RXMLParser::Body)
	{
		if(namespaceURI.IsEmpty())
		{
			if(Filter->UseDefaultNamespace)
				index=Filter->DefaultNamespace+":"+lName;
			else
				index=Filter->Doc->GetURL()+":"+lName;
		}
		else
			index=namespaceURI+":"+lName;
	}
	else
		index=lName;

	// Always index the structure
	AddStructElement(index,false);

	// If attributes names are content -> add short parameter name
	if(Filter->StructIsContent&&(!lName.ContainOnlySpaces()))
		Filter->ExtractWord(lName,Filter->WeightStruct,0,0,0);
}


//------------------------------------------------------------------------------
void XMLParser::Value(const RString& value)
{
	if(!Filter->AttrValues)
		return;
	if(!value.ContainOnlySpaces())
		Filter->ExtractWord(value,Filter->WeightValues,0,CurAttr,GetLastTokenPos());
}


//-----------------------------------------------------------------------------
void XMLParser::EndTag(const RString& namespaceURI, const RString& lName, const RString&)
{
	// Right tag
	if(CurTag)
		CurTag=CurTag->GetParent();

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
		if(GetSection()==RXMLParser::Body)
		{
			if(namespaceURI.IsEmpty())
			{
				if(Filter->UseDefaultNamespace)
					index=Filter->DefaultNamespace+":"+lName;
				else
					index=Filter->Doc->GetURL()+":"+lName;
			}
			else
				index=namespaceURI+":"+lName;
		}
		else
			index=lName;
		AddTagIndex(index,Content);
	}

	// Pop current content
	Content=(*Contents());
	Contents.Pop();
}


//-----------------------------------------------------------------------------
void XMLParser::Text(const RString& text)
{
	if(!text.ContainOnlySpaces())
	{
		double w;
		if(IsTitle||Filter->ExtractIndex)
			Content+=text;
		if(IsTitle)
			w=2.0;
		else
			w=1.0;
		Filter->ExtractWord(text,w,0,CurTag,GetLastTokenPos());
	}
}


//------------------------------------------------------------------------------
RChar XMLParser::CodeToChar(RString& str)
{
	RChar res=RXMLParser::CodeToChar(str);
	if(res!=0)
		return(res);
	return(' ');
}


//-----------------------------------------------------------------------------
void XMLParser::AddStructElement(const RString& element,bool tag)
{
	GConcept w(cNoRef,element,Filter->StructSpace,0,0,0,0);
	GWeightInfo* info=Filter->Infos.GetInsertPtr(Filter->StructSpace->InsertConcept(&w));
	(*info)+=1.0;

	// Create a node in the structure
	if(!Filter->Struct)
		return;
	if(tag)
	{
		GDocStructNode* ptr=new GDocStructNode(info,GetLastTokenPos(),GDocStructNode::Tag);
		Filter->Struct->InsertNode(CurTag,ptr);
		CurTag=ptr;
	}
	else
	{
		GDocStructNode* ptr=new GDocStructNode(info,GetLastTokenPos(),GDocStructNode::Attribute);
		Filter->Struct->InsertNode(CurTag,ptr);
		CurAttr=ptr;
	}
}


//-----------------------------------------------------------------------------
void XMLParser::AddTagIndex(const RString& element,const RString& content)
{
	if(!content.GetLen())
		return;

	bool Idx(false);

	// Find corresponding tag
	IndexTag* ptr=Filter->IndexTags.GetInsertPtr(element);
	ptr->Occurs++;

	// If maximum depth, cannot be used
	if(GetCurrentDepth()>Filter->MaxDepth)
		return;

	// Create new content
	cContent* Content;
	ptr->InsertPtr(Content=new cContent());

	// Count number of terms -> if too much, cannot be used
	size_t NbTerms(0);
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
	if(Idx)
		Filter->ExtractWord(Index,1.0,Content,0,0);
}
