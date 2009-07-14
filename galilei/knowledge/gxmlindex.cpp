/*

	GALILEI Research Project

	GXMLIndex.cpp

	XML Index - Implementation.

	Copyright 2007-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gxmlindex.h>
#include <gconcepttype.h>
#include <gsession.h>
#include <glang.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GXMLIndex
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(const GXMLIndex* index)
	: GConcept(index), XMLTag(index->XMLTag), Lang(index->Lang), Universal(10), Stems(index->Stems)
{
}


//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(const RString& name,GConceptType* type)
	: GConcept(name,type), XMLTag(0), Lang(type), Universal(10), Stems(10)
{
	BuildDef();
}


//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(GConceptType* type,GConceptType* lang)
	: GConcept(RString::Null,type), XMLTag(0), Lang(lang), Universal(10), Stems(10)
{
}


//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(GConceptType* type,GConcept* tag,GConceptType* lang)
	: GConcept(RString::Null,type), XMLTag(tag), Lang(lang), Universal(10), Stems(10)
{
}


//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(GConceptType* type,GConcept* tag,GConceptType* lang,RContainer<GConcept,false,true>& uni,RContainer<GConcept,false,true>& stems)
	: GConcept(RString::Null,type), XMLTag(tag), Lang(lang), Universal(uni), Stems(stems)
{
	BuildName();
}



//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(size_t id,const RString& name,GConceptType* type,size_t refdocs,off_t indexdocs,size_t refprofiles,size_t refcommunities,size_t reftopics,size_t refclasses)
	: GConcept(id,name,type,refdocs,indexdocs,refprofiles,refcommunities,reftopics,refclasses), XMLTag(0), Universal(10), Stems(10)
{
	BuildDef();
}


//-----------------------------------------------------------------------------
void GXMLIndex::BuildDef(void)
{
	GSession* Session=GSession::Get();

	// Find the tag id of the XML tag
	GConceptType* Space=Session->GetConceptType("XMLStruct","XML Structure");
	int Pos=Name.Find('#');
	XMLTag=Session->GetConcept(atoi(Name.Mid(0,Pos)));

	// Find the universal part -> numbers until string of 2
	int Old=Pos+1;
	bool Uni;
	RString Tag;

	// Find the Universal terms
	Space=Session->GetInsertConceptType("UniversalTerms","Universal Terms");
	for(Pos=Name.Find(':',Old),Uni=true;(Pos!=-1)&&(Uni);Pos=Name.Find(':',Old))
	{
		Tag=Name.Mid(static_cast<size_t>(Old),Pos-Old);
		Old=Pos+1;
		if(Tag.GetLen()==2)
		{
			// Can be a language
			if(Tag[static_cast<size_t>(0)].IsAlpha()||(Tag=="00"))
				break;
		}
		size_t id=atoi(Tag);
		GConcept* ptr=Session->GetConcept(id);
		if(ptr)
			Stems.InsertPtr(ptr);
	}

	// Find the language of the stem
	Lang=Session->GetInsertConceptType(Tag+"Terms","");

	// Add now each concept to list
	for(Pos=Name.Find(':',Old);Pos!=-1;Pos=Name.Find(':',Old))
	{
		size_t id=atoi(Name.Mid(static_cast<size_t>(Old),Pos-Old));
		Stems.InsertPtr(Session->GetConcept(id));
		Old=Pos+1;
	}
	size_t id=atoi(Name.Mid(static_cast<size_t>(Old)));
	GConcept* ptr=Session->GetConcept(id);
	if(ptr)
		Stems.InsertPtr(ptr);
}


//-----------------------------------------------------------------------------
void GXMLIndex::BuildName(void)
{
	Name.SetLen(0);
	Name=RString::Number(XMLTag->GetId())+'#';
	RCursor<GConcept> Cur(Universal);
	for(Cur.Start();!Cur.End();Cur.Next())
		Name+=':'+RString::Number(Cur()->GetId());
	Name+=Lang->GetLang()->GetCode();
	Cur.Set(Stems);
	for(Cur.Start();!Cur.End();Cur.Next())
		Name+=':'+RString::Number(Cur()->GetId());
}


//-----------------------------------------------------------------------------
GConcept* GXMLIndex::DeepCopy(void) const
{
	return(new GXMLIndex(this));
}


//-----------------------------------------------------------------------------
void GXMLIndex::Clear(void)
{
	GConcept::Clear();
	XMLTag=0;
	Stems.Clear();
	Universal.Clear();
}


//-----------------------------------------------------------------------------
void GXMLIndex::SetConceptTypes(GConceptType* type,GConceptType* lang)
{
	Type=type;
	Lang=lang;
}


//-----------------------------------------------------------------------------
void GXMLIndex::SetTag(GConcept* tag)
{
	XMLTag=tag;
	Stems.Clear();
	Universal.Clear();
	BuildName();
}


//-----------------------------------------------------------------------------
R::RCursor<GConcept> GXMLIndex::GetUniversalTerms(void) const
{
	return(RCursor<GConcept>(Universal));
}


//-----------------------------------------------------------------------------
R::RCursor<GConcept> GXMLIndex::GetStems(void) const
{
	return(RCursor<GConcept>(Stems));
}


//-----------------------------------------------------------------------------
bool GXMLIndex::IsEmpty(void) const
{
	return((!Universal.GetNb())&&(!Stems.GetNb()));
}


//-----------------------------------------------------------------------------
size_t GXMLIndex::GetNbWords(void) const
{
	return(Universal.GetNb()+Stems.GetNb());
}


//-----------------------------------------------------------------------------
void GXMLIndex::AddUniversalTerm(GConcept* concept)
{
	if(concept->GetType()!=GSession::Get()->GetInsertConceptType("UniversalTerms","Universal Terms"))
		throw GException("GXMLIndex::AddUniversalTerms: Bad concept type");
	if(!Universal.IsIn(concept))
		Universal.InsertPtr(concept);
	BuildName();
}


//-----------------------------------------------------------------------------
void GXMLIndex::AddStem(GConcept* concept)
{
	if(concept->GetType()!=Lang)
		throw GException("GXMLIndex::AddStem: Bad concept type");
	if(!Stems.IsIn(concept))
		Stems.InsertPtr(concept);
	BuildName();
}


//-----------------------------------------------------------------------------
double GXMLIndex::GetSimilarity(const GXMLIndex* index) const
{
	size_t MinUniversal(std::min(Universal.GetNb(),index->Universal.GetNb()));
	size_t MinStems(std::min(Stems.GetNb(),index->Stems.GetNb()));

	// If not same tags or nothing to compare -> return 0
	if((XMLTag!=index->XMLTag)||((!MinUniversal)&&(!MinStems)))
		return(0.0);

	size_t nb(0);

	// Universal stems
	RCursor<GConcept> Stem1(Universal);
	RCursor<GConcept> Stem2(index->Universal);
	for(Stem1.Start(),Stem2.Start();!Stem1.End(); Stem1.Next())
	{
		while((!Stem2.End())&&(Stem2()->GetId()<Stem1()->GetId()))
			Stem2.Next();
		if((!Stem2.End())&&(Stem2()->GetId()==Stem1()->GetId()))
		{
			nb++;
			Stem2.Next();
		}
	}

	// Language bases stems
	Stem1.Set(Stems);
	Stem2.Set(index->Stems);
	for(Stem1.Start(),Stem2.Start();!Stem1.End(); Stem1.Next())
	{
		while((!Stem2.End())&&(Stem2()->GetId()<Stem1()->GetId()))
			Stem2.Next();
		if((!Stem2.End())&&(Stem2()->GetId()==Stem1()->GetId()))
		{
			nb++;
			Stem2.Next();
		}
	}
	return(static_cast<double>(nb)/static_cast<double>(MinUniversal+MinStems));
}



//-----------------------------------------------------------------------------
GXMLIndex::~GXMLIndex(void)
{
}
