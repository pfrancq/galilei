/*

	GALILEI Research Project

	GDocXML.cpp

	DocXML representing the document content - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
#include <gdocxml.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GDocXML
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocXML::GDocXML(const RURI& url,const RURI& filename)
	: RXMLStruct(), URL(url), FileName(filename)
{
	InitDocXML();
}


//------------------------------------------------------------------------------
void GDocXML::InitDocXML(void)
{
	if(Top)
		return;

	RXMLTag* t;
	AddTag(0,t=new RXMLTag("rdf:RDF"));
	InsertEntity("xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
	InsertEntity("xmlns:dc","http://purl.org/dc/elements/1.1/");
	InsertEntity("xmlns:docxml","http://www.imrdp.org/docxml");
	AddTag(t,MetaData=new RXMLTag("docxml:metaData"));
	AddTag(t,Content=new RXMLTag("docxml:content"));
	AddTag(t,Links=new RXMLTag("docxml:links"));
}


//------------------------------------------------------------------------------
RURI GDocXML::GetURL(void) const
{
	return(URL);
}


//------------------------------------------------------------------------------
RURI GDocXML::GetFile(void) const
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

	AddTag(Links,t=new RXMLTag("docxml:metaData"));
	return(t);

}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::Add(R::RString tag,R::RString val,R::RXMLTag* metaData)
{
	RXMLTag* t;
	if(!metaData)
		AddTag(MetaData,t=new RXMLTag("dc:"+tag));
	else
		AddTag(metaData,t=new RXMLTag("dc:"+tag));
	if(!val.IsEmpty())
		t->AddContent(val);
	return(t);
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddTitle(RString val,RXMLTag* metaData)
{
	return(Add("title",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddCreator(RString val,RXMLTag* metaData)
{
	return(Add("creator",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddSubject(RString val,RXMLTag* metaData)
{
	return(Add("subject",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddDescription(RString val,RXMLTag* metaData)
{
	return(Add("description",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddPublisher(RString val,RXMLTag* metaData)
{
	return(Add("publisher",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddContributor(RString val,RXMLTag* metaData)
{
	return(Add("contributor",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddDate(RString val,RXMLTag* metaData)
{
	return(Add("date",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddType(RString val,RXMLTag* metaData)
{
	return(Add("type",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddFormat(RString val,RXMLTag* metaData)
{
	return(Add("format",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddIdentifier(RString val,RXMLTag* metaData)
{
	return(Add("identifier",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddSource(RString val,RXMLTag* metaData)
{
	return(Add("source",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddLanguage(RString val,RXMLTag* metaData)
{
	return(Add("language",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddRelation(RString val,RXMLTag* metaData)
{
	return(Add("relation",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddCoverage(RString val,RXMLTag* metaData)
{
	return(Add("coverage",val,metaData));
}


//------------------------------------------------------------------------------
R::RXMLTag* GDocXML::AddRights(RString val,RXMLTag* metaData)
{
	return(Add("rights",val,metaData));
}


//----------------------------------------------------------------------------
GDocXML::~GDocXML(void)
{
}
