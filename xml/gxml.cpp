/*

	GALILEI Research Project

	GXML.cpp

	Analyze a document - Implementation.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
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
#include <gslot.h>



//-----------------------------------------------------------------------------
//
// class GXML
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GXML::GXML(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac), RXMLParser(), MetaTags(20), IsTitle(false),
	  IsIdentifier(false)
{
	AddMIME("text/xml");
	AddMIME("xml");
}


//-----------------------------------------------------------------------------
void GXML::ApplyConfig(void)
{
	ExtractStruct=FindParam<RParamValue>("ExtractStruct")->GetBool();
	StructIsContent=FindParam<RParamValue>("StructIsContent")->GetBool();
	ExtractIndex=FindParam<RParamValue>("ExtractIndex")->GetBool();
	MaxTerms=FindParam<RParamValue>("MaxTerms")->GetUInt();
	MaxOccurs=FindParam<RParamValue>("MaxOccurs")->GetUInt();
	MaxDepth=static_cast<char>(FindParam<RParamValue>("MaxDepth")->GetUInt());
	ChildTags=FindParam<RParamValue>("ChildTags")->GetBool();
	WeightStruct=FindParam<RParamValue>("WeightStruct")->GetDouble();
	ExtractValues=FindParam<RParamValue>("ExtractValues")->GetBool();
	WeightValues=FindParam<RParamValue>("WeightValues")->GetDouble();
	UseDefaultNamespace=FindParam<RParamValue>("UseDefaultNamespace")->GetBool();
	DefaultNamespace=FindParam<RParamValue>("DefaultNamespace")->Get();
	DetectMetaTag=FindParam<RParamValue>("DetectMetaTag")->GetBool();
	MetaTags.Clear();
	RCursor<RString> Tags(FindParam<RParamList>("IndexTags")->GetList());
	for(Tags.Start();!Tags.End();Tags.Next())
		MetaTags.InsertPtr(new RString(*Tags()));
}


//-----------------------------------------------------------------------------
void GXML::Init(void)
{
}


//-----------------------------------------------------------------------------
void GXML::Clear(void)
{
	IsTitle=false;
	IsIdentifier=false;
}


//-----------------------------------------------------------------------------
void GXML::BeginTag(const RString& namespaceURI,const RString& lName,const RString&)
{
	if(!ExtractStruct) return;

	// If tags are content -> add short name of the tag
	if(StructIsContent)
		ExtractValidWords(lName,WeightStruct,GVTDRec::Tag);

	// Index the tag
	if(GetSection()==RXMLParser::Body)
	{
		if(namespaceURI.IsEmpty())
		{
			if(UseDefaultNamespace)
				tmpStr=DefaultNamespace+":"+lName;
			else
				tmpStr=URI()+":"+lName;
		}
		else
			tmpStr=namespaceURI+":"+lName;
	}
	else
		tmpStr=lName;
	if(tmpStr=="http://purl.org/dc/elements/1.1:title")
		IsTitle=true;
	else if(tmpStr=="http://purl.org/dc/elements/1.1:identifier")
		IsIdentifier=true;

	// Stopped the analyze if not necessary anymore
	if(Debug)
		cout<<"Test: *"<<tmpStr<<"* ("<<(int)GetCurrentDepth()<<")"<<endl;
	if(StopAnalyseTag())
		return;

	// Create the tag
	if(Debug)
		cout<<"\tIndex: *"<<tmpStr<<"*"<<endl;

	// Get the node of the previous depth
	cDepth* Depth=Depths.GetDepth(GetCurrentDepth());
	cNode* CurNode=Depths.GetNewNode(Depth,true);
	LastInsertTag=StructTokens.AddToken(StructSpace,tmpStr);
	CurNode->SetTag(LastInsertTag,GetLastTokenPos(),Depth);

	// Make the sibling if depth>0
	if(GetCurrentDepth())
	{
		cNode* parent=Depths.GetDepth(GetCurrentDepth()-1)->GetCurTag();
		if(parent&&parent->Obj.Tag)
		{
			if(Debug)
				cout<<"Sibling between '"<<CurNode->Obj.Tag->GetName()<<"' and '"<<parent->Obj.Tag->GetName()<<"'"<<endl;
			if(parent->Child==SIZE_MAX)
				parent->Child=Depth->GetNbNodes()-1;
			parent->Nb++;
		}
	}
}


//------------------------------------------------------------------------------
void GXML::ResolveNamespace(const RString& namespaceURI)
{
	// Stopped the analyze if not necessary anymore
	if(StopAnalyseTag())
		return;
	if(LastInsertTag)
		StructTokens.ChangeTokenNS(LastInsertTag,namespaceURI);
}


//------------------------------------------------------------------------------
void GXML::AddAttribute(const RString& namespaceURI,const RString& lName, const RString&)
{
	if(!ExtractStruct) return;

	// If attributes names are content -> add short parameter name
	if(StructIsContent)
		ExtractValidWords(lName,WeightStruct,GVTDRec::Attribute);

	// Stopped the analyze if not necessary anymore
	if(StopAnalyseTag())
		return;

	// Compute the name of the tag
	if(GetSection()==RXMLParser::Body)
	{
		if(namespaceURI.IsEmpty())
		{
			if(UseDefaultNamespace)
				tmpStr=DefaultNamespace+":"+lName;
			else
				tmpStr=URI()+":"+lName;
		}
		else
			tmpStr=namespaceURI+":"+lName;
	}
	else
		tmpStr=lName;

	cDepth* Depth=Depths.GetDepth(GetCurrentDepth());
	cNode* CurNode=Depths.GetNewNode(Depth,false);
	cStructToken* tag=StructTokens.AddToken(StructSpace,tmpStr);
	CurNode->SetAttr(tag,GetLastTokenPos(),Depth);
}


//------------------------------------------------------------------------------
void GXML::Value(const RString& text)
{
	if(ExtractValues)
	{
		ExtractValidWords(text,WeightValues,GVTDRec::Value);
	}
}


//-----------------------------------------------------------------------------
void GXML::EndTag(const RString&, const RString& /*lName*/, const RString&)
{
	// Stopped the analyze if not necessary anymore
/*	if((!MustFullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;*/
/*	if(CurDepth)
		CurNode=Depths.GetDepth(CurDepth-1)->GetCurTag();*/
}


//-----------------------------------------------------------------------------
void GXML::Text(const RString& text)
{
	if(IsIdentifier)
	{
		IsIdentifier=false;
		return;
	}
	double w;
	if(IsTitle)
	{
		IsTitle=false;
		w=2.0;
	}
	else
		w=1.0;
	ExtractValidWords(text,w,GVTDRec::Content);
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
void GXML::Analyze(GDoc*,const RURI& uri,RXMLParser*,GSlot* rec)
{
	try
	{
		Open(uri,RIO::Read);
		Close();
	}
	catch(RIOException& e)
	{
		if(rec)
			rec->Warning(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GXML::CreateConfig(void)
{
	InsertParam(new RParamValue("ExtractStruct",false,"Extract the structure elements?"));
	InsertParam(new RParamValue("StructIsContent",false,"Must the structure elements be considered as content?"));
	InsertParam(new RParamValue("ExtractIndex",false,"Extract metadata tags?"));
	InsertParam(new RParamValue("MaxTerms",10,"Maximum number of terms of an automatically detected metadata tag."));
	InsertParam(new RParamValue("MaxDepth",2,"Maximum depth of an automatically detected metadata tag."));
	InsertParam(new RParamValue("MaxOccurs",5,"Maximum number of occurrences of an automatically detected metadata tag."));
	InsertParam(new RParamValue("ChildTags",false,"Can an automatically detected metadata tag have child?"));
	InsertParam(new RParamValue("WeightStruct",2.0,"Weight associated to the structure elements when extracted."));
	InsertParam(new RParamValue("ExtractValues",false,"Should the content of attribute values be extracted?"));
	InsertParam(new RParamValue("WeightValues",2.0,"Weight associated to the content of attribute values when extracted."));
	InsertParam(new RParamValue("UseDefaultNamespace",false,"When tags have no namespace, must the default namespace be associated."));
	InsertParam(new RParamValue("DefaultNamespace","","Default namespace for the tags."));
	InsertParam(new RParamValue("DetectMetaTag",true,"Should the metadata be detected automatically?"));
	InsertParam(new RParamList("IndexTags","List of the tags (including their namespace) to be considered as metadata."));
}


//-----------------------------------------------------------------------------
GXML::~GXML(void)
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("XML Filter","XML Filter",GXML)
