/*

	GALILEI Research Project

	GDocXML.cpp

	A XML representation of a document - Implementation.

	(C) 2001 by P. Francq.

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
	RXMLTag* rdf;

	AddNode(0,t=new RXMLTag("DocXML"));
	AddNode(t,rdf=new RXMLTag("rdf:RDF"));
	rdf->InsertAttr("xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
	rdf->InsertAttr("xmlns:dc","http://purl.org/dc/elements/1.1/");
	AddNode(rdf,MetaData=new RXMLTag("rdf:Description"));
	MetaData->InsertAttr("rdf:about",URL);
	AddNode(t,Content=new RXMLTag("Content"));
	AddNode(t,Links=new RXMLTag("Links"));
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddTitle(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Title"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddTitle(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Title"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddCreator(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Creator"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddCreator(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Creator"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddSubject(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Subject"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddSubject(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Subject"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddDescription(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Description"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddDescription(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Description"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddPublisher(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Publisher"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddPublisher(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Publisher"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddContributor(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Contributor"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddContributor(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Contributor"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddDate(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Date"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddDate(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Date"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddType(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Type"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddType(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Type"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddFormat(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Format"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddFormat(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Format"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddIdentifier(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Identifier"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddIdentifier(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Identifier"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddSource(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Source"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddSource(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Source"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddLanguage(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Language"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddLanguage(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Language"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddRelation(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Relation"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddRelation(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Relation"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddCoverage(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Coverage"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddCoverage(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Coverage"));
	return(t);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocXML::AddRights(const char* val) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Rights"));
	t->AddContent(val);
}


//-----------------------------------------------------------------------------
RXMLTag* GALILEI::GDocXML::AddRights(void) throw(bad_alloc)
{
	RXMLTag* t;

	AddNode(MetaData,t=new RXMLTag("dc:Rights"));
	return(t);
}


//---------------------------------------------------------------------------
GDocXML::~GDocXML(void)
{
}
