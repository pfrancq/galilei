/*

	GALILEI Research Project

	GDocXML.cpp

	A XML representation of a document - Implementation.

	Copyright 2001 by the Universit� Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocxml.h>
using namespace GALILEI;
using namespace RXML;



//-----------------------------------------------------------------------------
//
// class GDocXML
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocXML::GDocXML(const char* url,const char* filename) throw(bad_alloc)
	: RXMLStruct(), URL(url), FileName(filename)
{
	RXMLTag* t;

	AddNode(0,t=new RXMLTag("rdf:RDF"));
	InsertEntity("xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
	InsertEntity("xmlns:dc","http://purl.org/dc/elements/1.1/");
	InsertEntity("xmlns:docxml","http://cfao30.ulb.ac.be/cfao/projetcs/GALILEI/public/document/docxml");
	AddNode(t,MetaData=new RXMLTag("docxml:metaData"));
//	MetaData->InsertAttr("xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
//	MetaData->InsertAttr("xmlns:dc","http://purl.org/dc/elements/1.1/");
	AddNode(t,Content=new RXMLTag("docxml:content"));
	AddNode(t,Links=new RXMLTag("docxml:links"));
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddTitle(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:title"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddTitle(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:title"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddCreator(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:creator"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddCreator(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:creator"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddSubject(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:subject"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddSubject(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:subject"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddDescription(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:description"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddDescription(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:description"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddPublisher(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:publisher"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddPublisher(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:publisher"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddContributor(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:contributor"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddContributor(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:contributor"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddDate(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:date"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddDate(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:date"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddType(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:type"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddType(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:type"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddFormat(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:format"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddFormat(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:format"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddIdentifier(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:identifier"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddIdentifier(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:identifier"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddSource(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:source"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddSource(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:source"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddLanguage(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:language"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddLanguage(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:language"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddRelation(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:relation"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddRelation(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:relation"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddCoverage(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:coverage"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddCoverage(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:coverage"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddRights(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:rights"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddRights(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:rights"));
	return(t);
}


//---------------------------------------------------------------------------
GDocXML::~GDocXML(void)
{
}
