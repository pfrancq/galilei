/*

	GALILEI Research Project

	GConceptRecord.cpp

	Concept Record - Implementation.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
#include <gconceptrecord.h>
#include <gconcept.h>
#include <gdoc.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GConceptRecord
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptRecord::GConceptRecord(void)
	: Type(ttUnknown), ConceptId(cNoRef), SyntacticPos(cNoRef), Pos(cNoRef),
	  SyntacticDepth(cNoRef), Index(cNoRef)
{
}


//------------------------------------------------------------------------------
GConceptRecord::GConceptRecord(const GConceptRecord& rec)
	: Type(rec.Type), ConceptId(rec.ConceptId), SyntacticPos(rec.SyntacticPos), Pos(rec.Pos),
	  SyntacticDepth(rec.SyntacticDepth), Index(rec.Index)
{
}


//------------------------------------------------------------------------------
GConceptRecord::GConceptRecord(size_t conceptid)
	: Type(ttUnknown), ConceptId(conceptid), SyntacticPos(cNoRef), Pos(cNoRef),
	  SyntacticDepth(cNoRef), Index(cNoRef)
{
}


//------------------------------------------------------------------------------
GConceptRecord::GConceptRecord(tTokenType type,size_t conceptid,size_t synpos,size_t pos,size_t syndepth,size_t idx)
	: Type(type), ConceptId(conceptid), SyntacticPos(synpos), Pos(pos),
	  SyntacticDepth(syndepth), Index(idx)
{
}


//------------------------------------------------------------------------------
int GConceptRecord::Compare(const GConceptRecord& node) const
{
	if(ConceptId!=node.ConceptId)
		return(CompareIds(ConceptId,node.ConceptId));

	if(SyntacticDepth==node.SyntacticDepth)
	{
		if(Pos==node.Pos)
		{
			if(SyntacticPos==node.SyntacticPos)
				return(0);
			else if(SyntacticPos>node.SyntacticPos)
				return(1);
			else
				return(-1);
		}
		else if (Pos>node.Pos)
			return(1);
		else
			return(-1);

	}

	return(CompareIds(SyntacticDepth,node.SyntacticDepth));
}


//------------------------------------------------------------------------------
GConceptRecord& GConceptRecord::operator=(const GConceptRecord& rec)
{
	Type=rec.Type;
	ConceptId=rec.ConceptId;
	SyntacticPos=rec.SyntacticPos;
	Pos=rec.Pos;
	SyntacticDepth=rec.SyntacticDepth;
	Index=rec.Index;
	return(*this);
}

//------------------------------------------------------------------------------
bool GConceptRecord::operator==(const GConceptRecord& rec) const
{
	if(Type!=rec.Type)
		return(false);
	if(ConceptId!=rec.ConceptId)
		return(false);
	if(SyntacticPos!=rec.SyntacticPos)
		return(false);
	if(Pos!=rec.Pos)
		return(false);
	if(SyntacticDepth!=rec.SyntacticDepth)
		return(false);
	return(true);
}


//------------------------------------------------------------------------------
bool GConceptRecord::operator!=(const GConceptRecord& rec) const
{
	if(Type!=rec.Type)
		return(true);
	if(ConceptId!=rec.ConceptId)
		return(true);
	if(SyntacticPos!=rec.SyntacticPos)
		return(true);
	if(Pos!=rec.Pos)
		return(true);
	if(SyntacticDepth!=rec.SyntacticDepth)
		return(true);
	return(false);
}


//------------------------------------------------------------------------------
void GConceptRecord::Print(void) const
{
	cout<<"Node "<<ConceptId<<","<<Pos<<","<<SyntacticPos<<SyntacticDepth<<endl;
}


//------------------------------------------------------------------------------
GConceptRecord::~GConceptRecord(void)
{
}



//------------------------------------------------------------------------------
//
//  GConceptRecordIterator
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptRecordIterator::GConceptRecordIterator(GDoc* doc,const GConceptRecord& rec)
	: Doc(doc), Record(rec)
{
}


//------------------------------------------------------------------------------
void GConceptRecordIterator::Parse(void)
{
	GConceptRecord Child;
	size_t Idx(Record.GetIndex()+1);
	while(Doc->GetSession()->FindFirstChildRecord(Doc,Record,Child,Idx))
	{
		Idx=Child.GetIndex()+1;
		Treat(Child);
	}
}