/*

	GALILEI Research Project

	GDocXML.cpp

	DocXML representing the document content - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocxml.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDocXML
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocXML::GDocXML(const char* url,const char* filename) throw(bad_alloc)
	: RXMLStruct(), URL(url), FileName(filename)
{
	RXMLTag* t;

	AddNode(0,t=new RXMLTag("rdf:RDF"));
	InsertEntity("xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
	InsertEntity("xmlns:dc","http://purl.org/dc/elements/1.1/");
	InsertEntity("xmlns:docxml","http://cfao30.ulb.ac.be/cfao/projetcs/GALILEI/public/document/docxml#");
	AddNode(t,MetaData=new RXMLTag("docxml:metaData"));
	AddNode(t,Content=new RXMLTag("docxml:content"));
	AddNode(t,Links=new RXMLTag("docxml:links"));
}


//------------------------------------------------------------------------------
const char* GDocXML::GetURL(void) const
{
	return(URL.Latin1());
}


//------------------------------------------------------------------------------
const char* GDocXML::GetFile(void) const
{
	return(FileName.Latin1());
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
RXMLTag* GDocXML::AddLink(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(Links,t=new RXMLTag("docxml:metaData"));
	return(t);

}

//------------------------------------------------------------------------------
void GDocXML::AddTitle(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;
	if (! metaData)
	{
		AddNode(MetaData,t=new RXMLTag("dc:title"));
	}
	else
	{
		AddNode(metaData,t=new RXMLTag("dc:title"));
	}
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddTitle(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:title"));
	else
		AddNode(metaData,t=new RXMLTag("dc:title"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddCreator(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:creator"));
	else
		AddNode(metaData,t=new RXMLTag("dc:creator"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddCreator(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:creator"));
	else
		AddNode(metaData,t=new RXMLTag("dc:creator"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddSubject(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:subject"));
	else
		AddNode(metaData,t=new RXMLTag("dc:subject"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddSubject(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:subject"));
	else
		AddNode(metaData,t=new RXMLTag("dc:subject"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddDescription(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:description"));
	else
		AddNode(metaData,t=new RXMLTag("dc:description"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddDescription(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:description"));
	else
		AddNode(metaData,t=new RXMLTag("dc:description"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddPublisher(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:publisher"));
	else
		AddNode(metaData,t=new RXMLTag("dc:publisher"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddPublisher(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:publisher"));
	else
		AddNode(metaData,t=new RXMLTag("dc:publisher"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddContributor(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:contributor"));
	else
		AddNode(metaData,t=new RXMLTag("dc:contributor"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddContributor(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:contributor"));
	else
		AddNode(metaData,t=new RXMLTag("dc:contributor"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddDate(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:date"));
	else
		AddNode(metaData,t=new RXMLTag("dc:date"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddDate(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:date"));
	else
		AddNode(metaData,t=new RXMLTag("dc:date"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddType(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:type"));
	else
		AddNode(metaData,t=new RXMLTag("dc:type"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddType(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:type"));
	else
		AddNode(MetaData,t=new RXMLTag("dc:type"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddFormat(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:format"));
	else
		AddNode(metaData,t=new RXMLTag("dc:format"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddFormat(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:format"));
	else
		AddNode(metaData,t=new RXMLTag("dc:format"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddIdentifier(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:identifier"));
	else
		AddNode(metaData,t=new RXMLTag("dc:identifier"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddIdentifier(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:identifier"));
	else
		AddNode(metaData,t=new RXMLTag("dc:identifier"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddSource(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:source"));
	else
		AddNode(metaData,t=new RXMLTag("dc:source"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddSource(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:source"));
	else
		AddNode(metaData,t=new RXMLTag("dc:source"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddLanguage(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:language"));
	else
		AddNode(metaData,t=new RXMLTag("dc:language"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddLanguage(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:language"));
	else
		AddNode(metaData,t=new RXMLTag("dc:language"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddRelation(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:relation"));
	else
		AddNode(metaData,t=new RXMLTag("dc:relation"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddRelation(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:relation"));
	else
		AddNode(metaData,t=new RXMLTag("dc:relation"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddCoverage(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:coverage"));
	else
		AddNode(metaData,t=new RXMLTag("dc:coverage"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddCoverage(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:coverage"));
	else
		AddNode(metaData,t=new RXMLTag("dc:coverage"));
	return(t);
}


//------------------------------------------------------------------------------
void GDocXML::AddRights(const char* val,RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:rights"));
	else
		AddNode(metaData,t=new RXMLTag("dc:rights"));
	t->AddContent(val);
}


//------------------------------------------------------------------------------
RXMLTag* GDocXML::AddRights(RXMLTag* metaData) throw(bad_alloc)
{
	RXMLTag* t;

	if(!metaData)
		AddNode(MetaData,t=new RXMLTag("dc:rights"));
	else
		AddNode(metaData,t=new RXMLTag("dc:rights"));
	return(t);
}


//----------------------------------------------------------------------------
GDocXML::~GDocXML(void)
{
}
