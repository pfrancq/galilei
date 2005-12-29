/*

	GALILEI Research Project

	GDocXML.cpp

	DocXML representing the document content - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdocxml.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDocXML
//
//------------------------------------------------------------------------------
GDocXML::GDocXML(RString url,RString filename)
	: RXMLStruct(), URL(url), FileName(filename)
{
	RXMLTag* t;

	AddTag(0,t=new RXMLTag(this,"rdf:RDF"));
	InsertEntity("xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
	InsertEntity("xmlns:dc","http://purl.org/dc/elements/1.1/");
	InsertEntity("xmlns:docxml","http://cfao30.ulb.ac.be/cfao/projetcs/GALILEI/public/document/docxml#");
	AddTag(t,MetaData=new RXMLTag(this,"docxml:metaData"));
	AddTag(t,Content=new RXMLTag(this,"docxml:content"));
	AddTag(t,Links=new RXMLTag(this,"docxml:links"));
}


//------------------------------------------------------------------------------
RString GDocXML::GetURL(void) const
{
	return(URL);
}


//------------------------------------------------------------------------------
RString GDocXML::GetFile(void) const
{
	return(FileName);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::GetMetaData(void)
{
	return(MetaData);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::GetContent(void)
{
	return(Content);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::GetLinks(void)
{
	return(Links);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddLink(void)
{
	RXMLTag* t;

	AddTag(Links,t=new RXMLTag(this,"docxml:metaData"));
	return(t);

}


//------------------------------------------------------------------------------
void GDocXML::AddTitle(RString val,RXMLTag* metaData)
{
	RXMLTag* t;
	if (! metaData)
	{
		AddTag(MetaData,t=new RXMLTag(this,"dc:title"));
	}
	else
	{
		AddTag(metaData,t=new RXMLTag(this,"dc:title"));
	}
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddTitle(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:title"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:title"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddCreator(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:creator"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:creator"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddCreator(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:creator"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:creator"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddSubject(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:subject"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:subject"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddSubject(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:subject"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:subject"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddDescription(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:description"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:description"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddDescription(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:description"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:description"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddPublisher(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:publisher"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:publisher"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddPublisher(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:publisher"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:publisher"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddContributor(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:contributor"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:contributor"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddContributor(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:contributor"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:contributor"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddDate(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:date"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:date"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddDate(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:date"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:date"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddType(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:type"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:type"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddType(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:type"));
	else
		AddTag(MetaData,t=new RXMLTag(this,"dc:type"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddFormat(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:format"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:format"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddFormat(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:format"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:format"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddIdentifier(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:identifier"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:identifier"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddIdentifier(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:identifier"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:identifier"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddSource(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:source"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:source"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddSource(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:source"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:source"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddLanguage(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:language"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:language"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddLanguage(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:language"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:language"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddRelation(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:relation"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:relation"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddRelation(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:relation"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:relation"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddCoverage(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:coverage"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:coverage"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddCoverage(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:coverage"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:coverage"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddRights(RString val,RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:rights"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:rights"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddRights(RXMLTag* metaData)
{
	RXMLTag* t;

	if(!metaData)
		AddTag(MetaData,t=new RXMLTag(this,"dc:rights"));
	else
		AddTag(metaData,t=new RXMLTag(this,"dc:rights"));
	return(t);
}


//----------------------------------------------------------------------------
GDocXML::~GDocXML(void)
{
}
