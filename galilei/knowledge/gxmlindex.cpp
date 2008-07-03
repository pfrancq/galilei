/*

	GALILEI Research Project

	GXMLIndex.cpp

	XML Index - Implementation.

	Copyright 2007 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gxmlindex.h>
#include <gconcepttype.h>
#include <gsession.h>
#include <glang.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GXMLIndex
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(void)
	: GConcept(), XMLTag(0), Universal(10), Stems(10)
{
	BuildDef();
}


//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(const GXMLIndex* index)
	: GConcept(index), XMLTag(index->XMLTag), Universal(10), Stems(index->Stems)
{
}


//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(const RString& name,GConceptType* type)
	: GConcept(name,type), XMLTag(0), Universal(10), Stems(10)
{
	BuildDef();
}


//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(GConceptType* type,GConcept* tag,GLang* lang,RContainer<GConcept,false,true>& uni,RContainer<GConcept,false,true>& stems)
	: GConcept(), XMLTag(tag), Universal(uni), Stems(stems)
{
	Type=type;

	// Build the Name
	Name=RString::Number(tag->GetId())+'#';
	RCursor<GConcept> Cur(Universal);
	for(Cur.Start();!Cur.End();Cur.Next())
		Name+=':'+RString::Number(Cur()->GetId());
	Name+=lang->GetCode();
	Cur.Set(Stems);
	for(Cur.Start();!Cur.End();Cur.Next())
		Name+=':'+RString::Number(Cur()->GetId());
}



//-----------------------------------------------------------------------------
GXMLIndex::GXMLIndex(unsigned int id,const RString& name,GConceptType* type,unsigned int refdocs,unsigned int refsubprofiles,unsigned int refgroups)
	: GConcept(id,name,type,refdocs,refsubprofiles,refgroups), XMLTag(0), Universal(10), Stems(10)
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
	XMLTag=Space->GetConcept(atoi(Name.Mid(0,Pos)));

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
		Stems.InsertPtr(Space->GetConcept(id));
	}

	// Find the language of the stem
	Space=Session->GetInsertConceptType(Tag+"Stems","");

	// Add now each concept to list
	for(Pos=Name.Find(':',Old);Pos!=-1;Pos=Name.Find(':',Old))
	{
		size_t id=atoi(Name.Mid(static_cast<size_t>(Old),Pos-Old));
		Stems.InsertPtr(Space->GetConcept(id));
		Old=Pos+1;
	}
	size_t id=atoi(Name.Mid(static_cast<size_t>(Old)));
	Stems.InsertPtr(Space->GetConcept(id));
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
