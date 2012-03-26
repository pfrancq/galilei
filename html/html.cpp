/*

	GALILEI Research Project

	HTML.cpp

	A HTML filter - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2003 by Valery Vandaele.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <html.h>
#include <gdocanalyze.h>
#include <gdoc.h>


//------------------------------------------------------------------------------
//
// class GFilterHTML
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterHTML::GFilterHTML(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac), GXMLParser(), DivisionTags(10), Divisions(30,20)
{
	AddMIME("text/html");
	SetHTMLMode(true);
	DivisionTags.InsertPtr(new DivisionTag("html",0));
	DivisionTags.InsertPtr(new DivisionTag("body",1));
	DivisionTags.InsertPtr(new DivisionTag("h1>",2));
	DivisionTags.InsertPtr(new DivisionTag("h2",3));
	DivisionTags.InsertPtr(new DivisionTag("h3",4));
	DivisionTags.InsertPtr(new DivisionTag("h4",5));
	DivisionTags.InsertPtr(new DivisionTag("h5",6));
	DivisionTags.InsertPtr(new DivisionTag("h6",7));
	DivisionTags.InsertPtr(new DivisionTag("p",8));
}


//-----------------------------------------------------------------------------
void GFilterHTML::ApplyConfig(void)
{
	// Assign the parameters
	DetectDivisions=FindParam<RParamValue>("DetectDivisions")->GetBool();
}


//------------------------------------------------------------------------------
void GFilterHTML::Analyze(GDocAnalyze* analyzer,const GDoc*,const R::RURI& file)
{
	Divisions.Clear();
	Analyzer=analyzer;
	BodyTag=TitleTag=MetaTag=false;
	Open(file);
	Close();
}


//------------------------------------------------------------------------------
RString GFilterHTML::GetTextFragment(GDocFragment* fragment)
{
	return(GetFragment(fragment));
}


//------------------------------------------------------------------------------
void GFilterHTML::BeginTag(const RString&,const RString& lName,const RString&)
{
	// Find the depth (if any)
	if(DetectDivisions)
	{
		DivisionTag* Find(DivisionTags.GetPtr(lName));
		if(Find)
		{
			// Eliminate all the lower tags from the stack
			while((Divisions.GetNb())&&(Divisions()->Tag->Depth>=Find->Depth))
				Divisions.Pop();

			// Create a new division
			if(Divisions.GetNb())
				Divisions.Push(new DocDivision(Find,Divisions()->Depth+1));
			else
				Divisions.Push(new DocDivision(Find,0));
		}
	}

	// If the 'BODY' tag was parsed -> No interesting tags anymore
	if(BodyTag)
		return;

	RString Tag(lName.ToUpper());
	if(Tag=="META")
		MetaTag=true;
	else if(Tag=="TITLE")
		TitleTag=true;
	else if(Tag=="BODY")
		BodyTag=true;
	Meta.SetLen(0);
	Content=false;
}


//------------------------------------------------------------------------------
void GFilterHTML::AddAttribute(const RString&,const RString& lName,const RString&)
{
	// If the 'BODY' tag was parsed -> No interesting attributes
	if(BodyTag)
		return;

	// Parse some header elements
	RString Attr(lName.ToLower());
	if(Attr=="content")
	{
		Content=true;
	}
	else if(Attr=="name")
	{
		Meta="creator";
	}
	else if(Attr=="keywords")
	{
		Meta="description";
	}
	else if(Attr=="date")
	{
		Meta="date";
	}
	else if(Attr=="description")
	{
		Meta="description";
	}
}


//------------------------------------------------------------------------------
void GFilterHTML::Value(const RString& value)
{
	if((!Content)||(Meta.IsEmpty()))
		return;
	if(DetectDivisions)
		Analyzer->ExtractDCMI(Meta,value,GetLastTokenPos(),1);
	else
		Analyzer->ExtractDCMI(Meta,value,GetLastTokenPos(),GetCurrentDepth());
}


//------------------------------------------------------------------------------
void GFilterHTML::EndTag(const RString&,const RString&,const RString&)
{
	// If the 'BODY' tag was parsed -> No interesting tags anymore
	if(BodyTag)
		return;

	TitleTag=MetaTag=false;
}


//------------------------------------------------------------------------------
void GFilterHTML::Text(const RString& text)
{
	if(TitleTag)
	{
		if(DetectDivisions)
			Analyzer->ExtractDCMI("title",text,GetLastTokenPos(),1);
		else
			Analyzer->ExtractDCMI("title",text,GetLastTokenPos(),GetCurrentDepth());
	}
	else if(BodyTag)
	{
		if(DetectDivisions)
			Analyzer->ExtractBody(text,GetLastTokenPos(),Divisions()->Depth);
		else
			Analyzer->ExtractBody(text,GetLastTokenPos(),GetCurrentDepth());
	}
}


//------------------------------------------------------------------------------
void GFilterHTML::CreateConfig(void)
{
	InsertParam(new RParamValue("DetectDivisions",true,"Detect the document divisions?"));
}


//------------------------------------------------------------------------------
GFilterHTML::~GFilterHTML()
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("HTML Filter","HTML Filter",GFilterHTML)
