/*

	GALILEI Research Project

	GFilterXML.cpp

	A XML filter - Implementation.

	Copyright 2001-2007    by the Université Libre de Bruxelles.

	Authors:
		Vandaele Valery (vavdaele@ulb.ac.be)
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



//---------------------------------------------------------------------------
// include files for GALILEI
#include <gfilterxml.h>
#include <gdocxml.h>
#include <rxmlfile.h>
#include <rxmlattr.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//---------------------------------------------------------------------------
//
// class GFilterXML::Attribut
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class GALILEI::GFilterXML::Attribut
{
public:

	RString Name;
	tTag Type;
	RString Value;
	//All allowed attributs for a Tag
	R::RContainer<Attribut,true,true> SubAttrs;

	Attribut(const RString n,tTag t,const RString v)
		: Name(n), Type(t),Value(v),SubAttrs(5,2) {}
	int Compare(const Attribut* a) const {return(Name.Compare(a->Name));}
	int Compare(const Attribut& a) const {return(Name.Compare(a.Name));}
	int Compare(const RString& a) const {return(Name.Compare(a));}
	void InsertSubAttribute(Attribut* a){SubAttrs.InsertPtr(a);}
	R::RCursor<Attribut> GetSubAttributsCursor(void)
	{
		R::RCursor<Attribut> cur(SubAttrs);
		return(cur);
	}
};


//---------------------------------------------------------------------------
//
// class GFilterXML::Tag
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class GALILEI::GFilterXML::Tag
{
public:

	//struct TagStruct Name;
	struct TagStruct Name;
	tTag Type;

	//All allowed attributs for a Tag
	R::RContainer<Attribut,true,true> Attrs;

	Tag(const RString n,const RString p,tTag t)
		: Name(n,p),Type(t),Attrs(5,2) {}

	int Compare(const Tag& t) const {return(Name.Compare(t.Name));}
	int Compare(const Tag* t) const {return(Name.Compare(t->Name));}
	int Compare(const TagStruct& t) const {return(Name.Compare(t));}
	int Compare(const TagStruct* t) const {return(Name.Compare(t));}
	void InsertAttribute(Attribut* t){Attrs.InsertPtr(t);}
	R::RCursor<Attribut> GetAttributsCursor(void)
	{
		R::RCursor<Attribut> cur(Attrs);
		return(cur);
	}
};


//---------------------------------------------------------------------------
class GFilterXML::Def
{
public:
	RString Name;

	//Definition of the Tags associated to the current definition
	R::RContainer<Tag,true,true> Tags;

	Def(const RString n)
		: Name(n)/*,Mimes(5,2)*/,Tags(10,5) {}
	int Compare(const Def* d) const {return(Name.Compare(d->Name));}
	int Compare(const Def& d) const {return(Name.Compare(d.Name));}
	int Compare(const RString& d) const {return(Name.Compare(d));}
	void InsertTag(Tag* t){Tags.InsertPtr(t);}
	R::RCursor<Tag> GetTagsCursor(void)
	{
		R::RCursor<Tag> cur(Tags);
		return(cur);
	}
};


//---------------------------------------------------------------------------
class GFilterXML::MimeDef
{
public:
	RString Name;
	Def* Definition;

	MimeDef(const RString n,Def* def)
		: Name(n), Definition(def) {}
	int Compare(const MimeDef* d) const {return(Name.Compare(d->Name));}
	int Compare(const MimeDef& d) const {return(Name.Compare(d.Name));}
	int Compare(const RString& d) const {return(Name.Compare(d));}
};



//---------------------------------------------------------------------------
//
// class GFilterXML
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GFilterXML::GFilterXML(GFactoryFilter* fac)
	: GFilter(fac),xmlDefPath(""),Definitions(0),
	MimeDefinitions(0),CurrentDefinition(0),DefinitionIsInited(false)
{
	//Add generic mime type for the filter
	AddMIME("text/sgml");
	AddMIME("text/xml");
}

//-----------------------------------------------------------------------------
void GFilterXML::ApplyConfig(void)
{
	RString oldPath(xmlDefPath);

	xmlDefPath=Factory->Get("xmlDefPath");

	//If path has changed ->Set Inited to false
	if(oldPath.Compare(xmlDefPath))
		DefinitionIsInited=false;
}

//---------------------------------------------------------------------------
GFilterXML::tTag GFilterXML::ConvertRStringtoTagEnum(RString str)
{
	tTag t;
	if(str=="tMAIN")
	{
		t=tMAIN;
		return t;
	}
	if(str=="tATTRIBUT")
	{
		t=tATTRIBUT;
		return t;
	}
	if(str=="tSKIP")
	{
		t=tSKIP;
		return t;
	}
	if(str=="tTITLE")
	{
		t=tTITLE;
		return t;
	}
	if(str=="tDATE")
	{
		t=tDATE;
		return t;
	}
	if(str=="tPUBLI")
	{
		t=tPUBLI;
		return t;
	}
	if(str=="tSOURCE")
	{
		t=tSOURCE;
		return t;
	}
	if(str=="tCONTRIB")
	{
		t=tCONTRIB;
		return t;
	}
	if(str=="tDESCRIPT")
	{
		t=tDESCRIPT;
		return t;
	}
	if(str=="tRIGHTS")
	{
		t=tRIGHTS;
		return t;
	}
	if(str=="tPAR")
	{
		t=tPAR;
		return t;
	}
	if(str=="tLANG")
	{
		t=tLANG;
		return t;
	}
	if(str=="tMETA")
	{
		t=tMETA;
		return t;
	}
	if(str=="tIDENTIFIER")
	{
		t=tIDENTIFIER;
		return t;
	}
	if(str=="tRELATION")
	{
		t=tRELATION;
		return t;
	}
	if(str=="tCOVERAGE")
	{
		t=tCOVERAGE;
		return t;
	}
	if(str=="tTEXT")
	{
		t=tTEXT;
		return t;
	}
	if(str=="tSUBJECT")
	{
		t=tSUBJECT;
		return t;
	}
	if(str=="tSUMMARY")
	{
		t=tSUMMARY;
		return t;
	}
	if(str=="tH1")
	{
		t=tH1;
		return t;
	}
	if(str=="tH2")
	{
		t=tH2;
		return t;
	}
	if(str=="tH3")
	{
		t=tH3;
		return t;
	}
	if(str=="tH4")
	{
		t=tH4;
		return t;
	}
	if(str=="tH5")
	{
		t=tH5;
		return t;
	}
	if(str=="tH6")
	{
		t=tH6;
		return t;
	}

	//ELSE return tNULL
	t=tNULL;
	return t;
}

//---------------------------------------------------------------------------
void GFilterXML::LoadDefinitions(void)
{
	DIR* dp;
	struct dirent* ep;
	RString Name;
	//bool end=false;
	int len;
	RXMLStruct fileStruct;
	RXMLTag* tag;


	//Set Definition to Inited
	DefinitionIsInited=true;
	//Delete the container of definitions
	delete Definitions;
	delete MimeDefinitions;
	Definitions=0;
	MimeDefinitions=0;
	CurrentDefinition=0;

	//Load the new definitions
	//Init containers
	Definitions=new R::RContainer<Def,true,true>(10,5);
	MimeDefinitions=new R::RContainer<MimeDef,true,true>(10,5);

	if(xmlDefPath.IsEmpty()) throw GException("Path to xml definition is Empty");
	dp=opendir(xmlDefPath);
	if(!dp) throw GException("Path to xml definition is not correct");

	while((ep=readdir(dp)))
	{
		len=strlen(ep->d_name);
		if(len<3) continue;
		if(strcmp(&ep->d_name[len-7],".xmlDef")) continue;
		try
		{
			Name=xmlDefPath+ep->d_name;
			// Init Part

			RXMLFile Src(Name,&fileStruct);
			Src.Open(RIO::Read);
			tag=fileStruct.GetTop();
			FillFilterDefinitions(tag);
		}
		catch(RString& str)
		{
			throw GException(str);
		}
		catch(RIOException& e)
		{
			throw GException(e.GetMsg());
		}
		catch(bad_alloc)
		{
			throw;
		}
		catch(GException)
		{
			throw;
		}
		catch(RException& e)
		{
			throw GException(e.GetMsg());
		}
		catch(...)
		{
			throw GException("Unexcepted exception");
		}
	}
}


//---------------------------------------------------------------------------
void GFilterXML::FillFilterDefinitions(RXMLTag* currentTag)
{
	Def* definition;
	R::RCursor<RXMLAttr> xmlAttrCur;
	R::RCursor<RXMLTag> tagCur;

	if(!currentTag) return;

	// Get Filter Name (attribute of the main Tag)
	if(!(currentTag->GetName() == "xmlConverter")) return;

	//Read filter name
	xmlAttrCur=currentTag->GetAttrs();
	for(xmlAttrCur.Start();!xmlAttrCur.End();xmlAttrCur.Next())
	{
		if(xmlAttrCur()->GetName() == "name")
		{
			definition = new Def(xmlAttrCur()->GetValue());
			Definitions->InsertPtr(definition);
		}
	}

	if(!definition)
	{
		throw GException("No filter name found in tag \"xmlConverter\"");
	}

	//Treat current tag CHILDREN
	//--------------------------
	tagCur=currentTag->GetNodes();
	for(tagCur.Start();!tagCur.End();tagCur.Next())
	{
		if(tagCur()->GetName() == "mimeTypes")
		{
			FillMimeDefinition(tagCur(),definition);
		}
		if(tagCur()->GetName()== "tagsDefinition")
		{
			FillTagsDefinition(tagCur(),definition);
		}
	}
}


//---------------------------------------------------------------------------
void GFilterXML::FillTagsDefinition(RXMLTag* currentTag,Def* def)
{
	R::RCursor<RXMLAttr> xmlAttrCur;
	R::RCursor<RXMLTag> tagCur;
	RString tagName;
	RString docxmlName;
	RString parentName;
	Tag* t;


	tagCur=currentTag->GetNodes();
	for(tagCur.Start();!tagCur.End();tagCur.Next())
	{
		if(tagCur()->GetName()=="tag")
		{
			tagName=docxmlName=parentName="";
			xmlAttrCur=tagCur()->GetAttrs();
			for(xmlAttrCur.Start();!xmlAttrCur.End();xmlAttrCur.Next())
			{
				if( xmlAttrCur()->GetName()=="name")
				{
					tagName=xmlAttrCur()->GetValue();
				}
				if( xmlAttrCur()->GetName()=="docxml")
				{
					docxmlName=xmlAttrCur()->GetValue();
				}
				if( xmlAttrCur()->GetName()=="parent")
				{
					parentName=xmlAttrCur()->GetValue();
				}
			}
			//Insert found Tag
			if(!tagName.IsEmpty())
			{
				def->InsertTag(t=new Tag(tagName,parentName,ConvertRStringtoTagEnum(docxmlName)));

				//treat sub tags which are attributes of the current Tag
				FillAttributesDefinition(tagCur(),t);
			}
		}
	}
}


//---------------------------------------------------------------------------
void GFilterXML::FillAttributesDefinition(RXMLTag* currentTag,Tag* tag)
{
	R::RCursor<RXMLAttr> xmlAttrCur;
	R::RCursor<RXMLTag> tagCur,subTagCur;
	RString tagName;
	RString docxmlName;
	RString value;
	Attribut* attr;

	//Parse sub tags of the current one
	tagCur=currentTag->GetNodes();
	for(tagCur.Start();!tagCur.End();tagCur.Next())
	{
		if(tagCur()->GetName()=="attribut")
		{
			tagName=docxmlName=value="";
			xmlAttrCur=tagCur()->GetAttrs();
			for(xmlAttrCur.Start();!xmlAttrCur.End();xmlAttrCur.Next())
			{
				if( xmlAttrCur()->GetName()=="name")
				{
					tagName=xmlAttrCur()->GetValue();
				}
				if( xmlAttrCur()->GetName()=="docxml")
				{
					docxmlName=xmlAttrCur()->GetValue();
				}
				if( xmlAttrCur()->GetName()=="value")
				{
					value=xmlAttrCur()->GetValue();
				}
			}
			//Insert found Tag
			if(!tagName.IsEmpty())
			{
				tag->InsertAttribute(attr=new Attribut(tagName,ConvertRStringtoTagEnum(docxmlName),value));

				//treat sub tags which are attributes of the current Tag
				subTagCur=tagCur()->GetNodes();
				for(subTagCur.Start();!subTagCur.End();subTagCur.Next())
				{
					if(subTagCur()->GetName()=="attribut")
					{
						tagName=docxmlName=value="";
						xmlAttrCur=subTagCur()->GetAttrs();
						for(xmlAttrCur.Start();!xmlAttrCur.End();xmlAttrCur.Next())
						{
							if( xmlAttrCur()->GetName()=="name")
							{
								tagName=xmlAttrCur()->GetValue();
							}
							if( xmlAttrCur()->GetName()=="docxml")
							{
								docxmlName=xmlAttrCur()->GetValue();
							}
							if( xmlAttrCur()->GetName()=="value")
							{
								value=xmlAttrCur()->GetValue();
							}
						}
						//Insert found Tag
						if(!tagName.IsEmpty())
						{
							attr->InsertSubAttribute(new Attribut(tagName,ConvertRStringtoTagEnum(docxmlName),value));
						}
					}
				}
			}
		}
	}
}


//---------------------------------------------------------------------------
void GFilterXML::FillMimeDefinition(RXMLTag* currentTag,Def* def)
{
	R::RCursor<RXMLAttr> xmlAttrCur;
	R::RCursor<RXMLTag> tagCur;

	tagCur=currentTag->GetNodes();
	for(tagCur.Start();!tagCur.End();tagCur.Next())
	{
		//Tag Read must have the name "mimeType"
		if(tagCur()->GetName()=="mimeType")
		{
			xmlAttrCur=tagCur()->GetAttrs();
			for(xmlAttrCur.Start();!xmlAttrCur.End();xmlAttrCur.Next())
			{
				//Attribut name contents the name of the current mimeType
				if(xmlAttrCur()->GetName() == "name")
				{
					MimeDefinitions->InsertPtr(new MimeDef(xmlAttrCur()->GetValue(),def));
					//Add Mime to the filter
					AddMIME(xmlAttrCur()->GetValue());
				}
			}
		}
	}
}


//---------------------------------------------------------------------------
RXMLTag* GFilterXML::InsertTag(tTag t,RXMLTag* parentTag)
{

	RXMLTag* resTag;
	resTag= NULL;

	switch(t)
	{
		//ALL metadata types tags
		case tTITLE:
			resTag=Doc->AddTitle(RString::Null,0);
			break;

		case tAUTHOR:
			resTag=Doc->AddCreator(RString::Null,0);
			break;

		case tSUBJECT:
			resTag=Doc->AddSubject(RString::Null,0);
			break;

		case tDESCRIPT:
			resTag=Doc->AddDescription(RString::Null,0);
			break;

		case tPUBLI:
			resTag=Doc->AddPublisher(RString::Null,0);
			break;

		case tDATE:
			resTag=Doc->AddDate(RString::Null,0);
			break;

		case tSOURCE:
			resTag=Doc->AddSource(RString::Null,0);
			break;

		case tLANG:
			resTag=Doc->AddLanguage(RString::Null,0);
			break;

		case tRIGHTS:
			resTag=Doc->AddRights(RString::Null,0);
			break;

		case tCONTRIB:
			resTag=Doc->AddRights(RString::Null,0);
			break;

		case tRELATION:
			resTag=Doc->AddRelation(RString::Null,0);
			break;

		case tCOVERAGE:
			resTag=Doc->AddCoverage(RString::Null,0);
			break;

		case tMETA:
			break;

		//ALL content types tags
		case tMAIN:
			break;

		case tTEXT:
			resTag=Doc->GetContent();
			break;

		case tPAR:
			Doc->AddTag(parentTag,resTag=new RXMLTag("docxml:p"));
			break;

		case tH1:
			Doc->AddTag(parentTag,resTag=new RXMLTag("docxml:h1"));
			break;

		case tH2:
			Doc->AddTag(parentTag,resTag=new RXMLTag("docxml:h2"));
			break;

		case tH3:
			Doc->AddTag(parentTag,resTag=new RXMLTag("docxml:h3"));
			break;

		case tH4:
			Doc->AddTag(parentTag,resTag=new RXMLTag("docxml:h4"));
			break;

		case tH5:
			Doc->AddTag(parentTag,resTag=new RXMLTag("docxml:h5"));
			break;

		case tH6:
			Doc->AddTag(parentTag,resTag=new RXMLTag("docxml:h6"));
			break;

		case tSKIP:
			break;

		default:
			resTag=parentTag;
			break;
	};

	return resTag;
}


//---------------------------------------------------------------------------
void GFilterXML::AnalyseTag(RXMLTag* currentTag,RXMLTag* currentTagParent ,RXMLTag* docXMLparentTag)
{
	Tag *t;
	R::RCursor<RXMLTag> tagCur;
	R::RCursor<RXMLAttr> attrCur;
	RXMLTag* newTag;

	bool description=false;

	//Get Correspondence between read tag and docxml tag using the parent info
	t=CurrentDefinition->Tags.GetPtr<const TagStruct>(TagStruct(currentTag->GetName(),currentTagParent->GetName()));
	if(!t)
		t=CurrentDefinition->Tags.GetPtr<const TagStruct>(TagStruct(currentTag->GetName(),""));

	if(!t)
	{
		//If No Tag associated -> Insert content but not tag to the parent
		newTag=docXMLparentTag;
	}
	else
	{
		newTag=InsertTag(t->Type,docXMLparentTag);

		//return if tag must be skipped.
		if(t->Type == tSKIP) return;
		if(t->Type == tDESCRIPT) description=true;
	}


	//Treat current tag CONTENT
	//-------------------------
	if(newTag)
	{
		//Insert content in Node
		if(currentTag->HasContent())
		{
			if(description)
			{
				AnalyzeKeywords(currentTag->GetContent(),',',newTag);
				description=false;
			}
			else
			{
				AnalyzeBlock(currentTag->GetContent(),newTag);
			}
		}
	}

	//Treat current node Attributes
	//-------------------------
	AnalyzeAttributes(currentTag,currentTagParent);

	//Treat current tag CHILDREN if the current one is not skipped
	//--------------------------
	tagCur=currentTag->GetNodes();
	for(tagCur.Start();!tagCur.End();tagCur.Next())
	{
		if(newTag)
		{
			AnalyseTag(tagCur(),currentTag,newTag);
		}
		else
		{
			AnalyseTag(tagCur(),currentTag,docXMLparentTag);
		}
	}
}


//---------------------------------------------------------------------------
void GFilterXML::AnalyzeAttributes(RXMLTag* currentTag,RXMLTag* currentTagParent)
{
	Tag* t;
	RXMLTag* attrTag;
	Attribut* subattr;
	R::RCursor<Attribut> attrCur,subAttrCur;

	RString attrName;
	RString valueName;

	t=CurrentDefinition->Tags.GetPtr<const TagStruct>(TagStruct(currentTag->GetName(),currentTagParent->GetName()));
	if(!t)
		t=CurrentDefinition->Tags.GetPtr<const TagStruct>(TagStruct(currentTag->GetName(),""));

	if(!t) return;

	attrCur=t->GetAttributsCursor();
	for(attrCur.Start();!attrCur.End();attrCur.Next())
	{
		//Test if this attributes exists in the current Tag
		if(currentTag->IsAttrDefined(attrCur()->Name))
		{
			//If the Tag Attribute has no child attribute --> The attribue can be add with its value
			if(!attrCur()->SubAttrs.GetNb())
			{
				attrTag=InsertTag(attrCur()->Type,currentTag);
				if(attrTag)
				{
					//Insert content in Node
					AnalyzeBlock(currentTag->GetAttrValue(attrCur()->Name)(),attrTag);
				}
			}
			// If SubAttributes ->The get Value and parse sub Tags
			else
			{
				attrName=currentTag->GetAttrValue(attrCur()->Name);

				subattr=attrCur()->SubAttrs.GetPtr<const RString>(attrName);
				if(subattr)
				{
					valueName=subattr->Value;
					if(currentTag->IsAttrDefined(valueName))
					{
						//Insert/Get appropriate Node in xml structure
						attrTag=InsertTag(subattr->Type,currentTag);
						//Insert content in Node
						AnalyzeBlock(currentTag->GetAttrValue(valueName),attrTag);
					}
				}
			}
		}
	}
}


//---------------------------------------------------------------------------
bool GFilterXML::Analyze(GDocXML* doc)
{
	RString mime;
	MimeDef* mimeDef;
	R::RCursor<Def> defCur;
	//Def* def;
	RXMLTag* part;
	RXMLTag* tag;
	R::RCursor<RXMLTag> Cur;
	RXMLStruct fileStruct;

	try
	{

		//Test if filters are loaded
		if(!DefinitionIsInited)
		{
			//Load definitions from file
			LoadDefinitions();
		}
		if((!Definitions->GetNb())||(!MimeDefinitions->GetNb()))
		{
			throw GException("No definitions found for filter XML! Verify that the path to the definition is correct.");
		}
		// Init Part
		Doc=doc;
		RXMLFile Src(Doc->GetFile(),&fileStruct);
		Src.Open(RIO::Read);

		//Choose the correct tags definition (according to the mime type)
		if(!Doc->GetMetaData()->GetTag("dc:format")) return false;
		mime=Doc->GetTag("dc:format")->GetContent();
		if(mime.IsEmpty())
		{
			throw GException("The MimeType of the document is not defined !");
		}
		mimeDef= MimeDefinitions->GetPtr<RString>(mime);
		if(mimeDef)
		{
			CurrentDefinition= mimeDef->Definition;
		}
		else
		{
			tag=fileStruct.GetTop();

			//Go through all the definitions of the tags and look if the main tag is the same
			defCur.Set(*Definitions);
			for(defCur.Start();!defCur.End();defCur.Next())
			{
				if(defCur()->Tags.GetPtr<TagStruct>(TagStruct(tag->GetName(),"")))
				{
					CurrentDefinition=defCur();
					continue;
				}
			}
		}

		// Create the metaData tag and the first information
		part=Doc->GetContent();
		Doc->AddIdentifier(Doc->GetURL());
		tag=fileStruct.GetTop();
		AnalyseTag(tag,tag,0);
	}
	catch(RIOException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(RException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(...)
	{
		throw GException("GFilterXML: Undefined Error");
	}
	// Return OK
	return(true);
}


//------------------------------------------------------------------------------
R::RCursor<GFilterXML::Def> GFilterXML::GetDefinitionsCursor(void)
{
	R::RCursor<Def> cur(*Definitions);
	return(cur);
}


//------------------------------------------------------------------------------
R::RCursor<GFilterXML::MimeDef> GFilterXML::GetMimeDefinitionsCursor(void)
{
	R::RCursor<MimeDef> cur(*MimeDefinitions);
	return(cur);
}

//------------------------------------------------------------------------------
void GFilterXML::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("xmlDefPath","/etc/galilei/XMLdefinitions/"));
}


//---------------------------------------------------------------------------
GALILEI::GFilterXML::~GFilterXML()
{
	if(Definitions) delete Definitions;
	if(MimeDefinitions) delete MimeDefinitions;
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("XML Filter",GFilterXML)
