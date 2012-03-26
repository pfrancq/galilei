/*

	GALILEI Research Project

	GXML.cpp

	Analyze a document - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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



//-----------------------------------------------------------------------------
// DEBUG Mode
const bool Debug=false;


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rrecfile.h>
#include <rxmlfile.h>
#include <rdate.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gxml.h>
#include <gsession.h>
#include <gdocanalyze.h>



//-----------------------------------------------------------------------------
//
// class XMLTag
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
XMLTag::XMLTag(const RString& tag)
	: Concept(0), Tag(tag), NbOccurs(0), Schema(0), Type(XMLTag::Undefined), Content(200)
{

}

//-----------------------------------------------------------------------------
int XMLTag::Compare(const XMLTag& tag) const
{
	return(Tag.Compare(tag.Tag));
}


//-----------------------------------------------------------------------------
int XMLTag::Compare(const RString& tag) const
{
	return(Tag.Compare(tag));
}


//-----------------------------------------------------------------------------
GConcept* XMLTag::GetConcept(void)
{
	if(!Concept)
		Concept=Schema->Type->GetInsertConcept(Tag);
	return(Concept);
}



//-----------------------------------------------------------------------------
//
// class XMLSchema
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Skip ending '/' if necessary
RString AdaptNamespace(const RString& name)
{
	if((name.GetLen())&&(name[name.GetLen()-1]=='/'))
	{
		return(name.Mid(0,name.GetLen()-1));
	}
	else
		return(name);
}


//-----------------------------------------------------------------------------
XMLSchema::XMLSchema(const RString& uri)
	: URI(AdaptNamespace(uri)), Type(0), MetaConcept(0), Tags(200)
{

}


//-----------------------------------------------------------------------------
int XMLSchema::Compare(const XMLSchema& schema)
{
	return(URI.Compare(schema.URI));
}


//-----------------------------------------------------------------------------
int XMLSchema::Compare(const RString& uri)
{
	return(URI.Compare(uri));
}



//-----------------------------------------------------------------------------
//
// class GXML
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GXML::GXML(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac), GXMLParser(), MetadataTags(50), ExcludeTags(50), Schemas(50),
	  Tags(30,20), Semantic(0)
{
	AddMIME("text/xml");
	AddMIME("xml");
	AddMIME("application/xml");
}


//-----------------------------------------------------------------------------
void GXML::ApplyConfig(void)
{
	// Assign the parameters
	ExtractSemantic=FindParam<RParamValue>("ExtractSemantic")->GetBool();
	ExtractMetadata=FindParam<RParamValue>("ExtractMetadata")->GetBool();
	MetadataMaxTerms=FindParam<RParamValue>("MetadataMaxTerms")->GetUInt();
	MetadataMaxOccurs=FindParam<RParamValue>("MetadataMaxOccurs")->GetUInt();
	MetadataMaxDepth=FindParam<RParamValue>("MetadataMaxDepth")->GetUInt();
	MetadataChildTags=FindParam<RParamValue>("MetadataChildTags")->GetBool();
	UseDefaultNamespace=FindParam<RParamValue>("UseDefaultNamespace")->GetBool();
	DefaultNamespace=AdaptNamespace(FindParam<RParamValue>("DefaultNamespace")->Get());
	bool PreviousDetect(DetectMetadataTag);
	DetectMetadataTag=FindParam<RParamValue>("DetectMetadataTag")->GetBool();
	MetadataTags.Clear();
	RCursor<RString> Tags(FindParam<RParamList>("MetadataTags")->GetList());
	for(Tags.Start();!Tags.End();Tags.Next())
		MetadataTags.InsertPtr(new RString(AdaptNamespace(*Tags())));
	ExcludeTags.Clear();
	Tags=FindParam<RParamList>("ExcludeTags")->GetList();
	for(Tags.Start();!Tags.End();Tags.Next())
		ExcludeTags.InsertPtr(new RString(AdaptNamespace(*Tags())));

	// If the detection status has changed -> all the existing tags must be undefined
	if(PreviousDetect!=DetectMetadataTag)
	{
		RCursor<XMLSchema> Schema(Schemas);
		for(Schema.Start();!Schema.End();Schema.Next())
		{
			RCursor<XMLTag> Tag(Schema()->Tags);
			for(Tag.Start();!Tag.End();Tag.Next())
				Tag()->Type=XMLTag::Undefined;
		}
	}
}


//-----------------------------------------------------------------------------
void GXML::Init(void)
{
}


//-----------------------------------------------------------------------------
void GXML::Clear(void)
{
	Tags.Clear();

	// Remove the content of the tags
	RCursor<XMLSchema> Schema(Schemas);
	for(Schema.Start();!Schema.End();Schema.Next())
	{
		RCursor<XMLTag> Tag(Schema()->Tags);
		for(Tag.Start();!Tag.End();Tag.Next())
			Tag()->Content.Clear();
	}
}


//------------------------------------------------------------------------------
XMLSchema* GXML::GetSchema(const RString& uri)
{
	XMLSchema* Schema(Schemas.GetInsertPtr(uri));
	if(!Schema->Type)
	{
		Schema->Type=Session->GetInsertConceptType(Semantic,uri,RString::Null);
		Schema->MetaConcept=Schema->Type->GetInsertConcept(uri);
	}
	return(Schema);
}


//------------------------------------------------------------------------------
XMLTag* GXML::GetXMLTag(const RString& namespaceURI,const RString& lName)
{
	// Look for the schema
	if(Debug)
		cout<<"Get "<<namespaceURI<<":"<<lName<<endl;
	XMLSchema* Schema(0);
	if(namespaceURI.IsEmpty())
	{
		if(UseDefaultNamespace)
			Schema=GetSchema(DefaultNamespace);
		else
			Schema=GetSchema(URI());
	}
	else
		Schema=GetSchema(namespaceURI);

	// Look for the concept
	XMLTag* Tag(Schema->Tags.GetInsertPtr(lName));
	if(!Tag->Schema)
		Tag->Schema=Schema;
	if(Tag->Type==XMLTag::Undefined)
	{
		RString Name(Schema->Type->GetName()+":"+Tag->Tag);
		if(Debug)
			cout<<"  Determine status of "+Name<<endl;
		if(ExcludeTags.IsIn(Name))
		{
			if(Debug)
				cout<<"    Exclude it !"<<endl;
			Tag->Type=XMLTag::Exclude;
		}
		else
		{
			if(DetectMetadataTag)
			{
				if(Debug)
					cout<<"    Detect if Metadata !"<<endl;
				Tag->Type=XMLTag::Unknown;
			}
			else
			{
				if(MetadataTags.IsIn(Name))
				{
				if(Debug)
					cout<<"    Metadata for sure !"<<endl;
					Tag->Type=XMLTag::Metadata;
				}
				else
				{
					if(Debug)
						cout<<"    Textual for sure !"<<endl;
					Tag->Type=XMLTag::Text;
				}
			}
		}

		// Create a concept for that tag
		if(ExtractSemantic)
			Tag->Concept=Schema->Type->GetInsertConcept(Tag->Tag);
	}
	return(Tag);
}


//-----------------------------------------------------------------------------
void GXML::BeginTag(const RString& namespaceURI,const RString& lName,const RString&)
{
	if(GetSection()!=RXMLParser::Body)
	{
		Analyzer->SkipToken();
		return;
	}

	// Get the tag and add a occurrences of it
	XMLTag* Tag(GetXMLTag(namespaceURI,lName));
	Tag->NbOccurs++;
	XMLContent* Content(new XMLContent());
	Tag->Content.InsertPtr(Content);
	XMLTagOccur* Occur(new XMLTagOccur(Tag,Content));
	Tags.Push(Occur);

	if(ExtractSemantic)
	{
		if(Debug)
			cout<<"  Add Tag "<<Tag->Schema->URI+":"+Tag->Tag<<endl;
		Occur->Occur=Analyzer->AddToken(Tag->Schema->URI+":"+Tag->Tag,ttSemantic,Tag->GetConcept(),1.0,Tag->Schema->MetaConcept,GetLastTokenPos(),GetCurrentDepth());
	}
}


//------------------------------------------------------------------------------
void GXML::ResolveNamespace(const RString& namespaceURI)
{
	if(GetSection()!=RXMLParser::Body)
		return;

	// Get the current tag occurrence, and assign it a new XML Tag
	if(Debug)
		cout<<"Resolve namespace:" <<namespaceURI<<endl;
	XMLTagOccur* Occur(Tags());
	if(Occur)
	{
		Occur->Tag->NbOccurs--;
		XMLTag* Tag(GetXMLTag(namespaceURI,Occur->Tag->Tag));
		if(Occur->Tag!=Tag)
		{
			// Name of the tag (including its namespace) has changed
			Occur->Tag=Tag;
			XMLContent* Content(new XMLContent());
			Tag->Content.InsertPtr(Content);
			Tag->NbOccurs++;

			// Change also its token if necessary
			if(ExtractSemantic)
				Analyzer->MoveToken(Occur->Occur,Tag->Concept);
		}
	}
}


//-----------------------------------------------------------------------------
void GXML::EndTag(const RString&, const RString& /*lName*/, const RString&)
{
	// Skip the tag as token
	if(GetSection()!=RXMLParser::Body)
		return;

	Tags.Pop();
}


//-----------------------------------------------------------------------------
void GXML::Text(const RString& text)
{
	// Nothing to do if empty text
	if(text.ContainOnlySpaces())
		return;

	if(GetSection()!=RXMLParser::Body)
	{
		Analyzer->SkipToken();
		return;
	}

	XMLTagOccur* Occur(Tags());
	switch(Occur->Tag->Type)
	{
		case XMLTag::Metadata:
			if(ExtractMetadata)
				if(Debug)
					cout<<"    Add metadata "<<text<<endl;
				Analyzer->ExtractTextual(text,Occur->Tag->GetConcept(),GetLastTokenPos(),GetCurrentDepth());
			break;
		case XMLTag::Text:
			if(Debug)
				cout<<"    Add textual "<<text<<endl;
			Analyzer->ExtractBody(text,GetLastTokenPos(),GetCurrentDepth());
			break;
		case XMLTag::Unknown:
			// We must guess if it is a metadata tag
			if((!DetectMetadataTag) || (DetectMetadataTag&&((GetCurrentDepth()>MetadataMaxDepth)||(Occur->Tag->NbOccurs>MetadataMaxOccurs))))
			{
				if(Debug)
					cout<<"    Add textual "<<text<<endl;

				// We are sure that isn't a metadata -> Add normal text
				Analyzer->ExtractBody(text,GetLastTokenPos(),GetCurrentDepth());
			}
			else
			{
				if(Debug)
					cout<<"    delay "<<text<<endl;

				// We must delay the decision
				Occur->Content->InsertPtr(new XMLText(text,GetLastTokenPos(),GetCurrentDepth()));
			}
			break;
		default:
			break;
	}
}


//-----------------------------------------------------------------------------
void GXML::AddAttribute(const RString& /*namespaceURI*/,const RString& /*lName*/,const RString& /*name*/)
{
	// Simply skip the token
	Analyzer->SkipToken();
}


//-----------------------------------------------------------------------------
void GXML::Value(const RString& /*value*/)
{
	// Simply skip the token
	Analyzer->SkipToken();
}


//------------------------------------------------------------------------------
RChar GXML::CodeToChar(RString& str)
{
	RChar res=RXMLParser::CodeToChar(str);
	if(res!=0)
		return(res);
	return(' ');
}


//------------------------------------------------------------------------------
void GXML::Analyze(GDocAnalyze* analyzer,const GDoc*,const R::RURI& file)
{
	// Initialize the analyze
	Clear();
	if(!Semantic)
		Semantic=Session->GetInsertConceptCat("Metadata");
	Analyzer=analyzer;

	// Read the XML file
	Open(file,RIO::Read);
	Close();

	// Parse the tags to treat the unknown content
	RCursor<XMLSchema> Schema(Schemas);
	for(Schema.Start();!Schema.End();Schema.Next())
	{
		RCursor<XMLTag> Tag(Schema()->Tags);
		for(Tag.Start();!Tag.End();Tag.Next())
		{
			if(Debug)
				cout<<"Treat "<<Schema()->URI<<":"<<Tag()->Tag<<endl;
			if(DetectMetadataTag&&Tag()->Type==XMLTag::Unknown)
			{
				if(Debug)
					cout<<"   Tag status is unknown"<<endl;

				// If the tag appears more than MaxOccurs -> Is is certainly text
				bool SureText(Tag()->NbOccurs>MetadataMaxOccurs);
				RCursor<XMLContent> Content(Tag()->Content);
				for(Content.Start();!Content.End();Content.Next())
				{
					// It is text if it appears too often or has too many terms
					bool IsText(SureText||Content()->GetNb()>MetadataMaxTerms);
					RCursor<XMLText> Text(*Content());
					for(Text.Start();!Text.End();Text.Next())
					{
						if(IsText)
							Analyzer->ExtractBody(Text()->Text,Text()->Pos,Text()->Depth);
						else if(ExtractMetadata)
							Analyzer->ExtractTextual(Text()->Text,Tag()->GetConcept(),Text()->Pos,Text()->Depth);
					}
				}
			}
			Tag()->Content.Clear();
			Tag()->NbOccurs=0;
		}
	}
}


//------------------------------------------------------------------------------
RString GXML::GetTextFragment(GDocFragment* fragment)
{
	return(GetFragment(fragment));
}


//------------------------------------------------------------------------------
void GXML::CreateConfig(void)
{
	InsertParam(new RParamValue("ExtractSemantic",true,"Extract the semantic information?"));
	InsertParam(new RParamValue("ExtractMetadata",true,"Extract metadata tags?"));
	InsertParam(new RParamValue("MetadataMaxTerms",10,"Maximum number of terms of an automatically detected metadata tag."));
	InsertParam(new RParamValue("MetadataMaxDepth",3,"Maximum depth of an automatically detected metadata tag."));
	InsertParam(new RParamValue("MetadataMaxOccurs",5,"Maximum number of occurrences of an automatically detected metadata tag."));
	InsertParam(new RParamValue("MetadataChildTags",false,"Can an automatically detected metadata tag have child?"));
	InsertParam(new RParamValue("UseDefaultNamespace",false,"When tags have no namespace, must the default namespace be associated."));
	InsertParam(new RParamValue("DefaultNamespace","","Default namespace for the tags."));
	InsertParam(new RParamValue("DetectMetadataTag",false,"Should the metadata tag be detected automatically?"));
	InsertParam(new RParamList("MetadataTags","List of the tags (including their namespace) to be considered as metadata."));
	InsertParam(new RParamList("ExcludeTags","List of the tags (including their namespace) which contents must be excluded."));
}


//-----------------------------------------------------------------------------
GXML::~GXML(void)
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("XML Filter","XML Filter",GXML)
