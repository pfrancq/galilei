/*

	GALILEI Research Project

	GDocVector.cpp

	Document in the Vector model - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <ctype.h>
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocvector.h>
#include <profiles/gsubprofilevector.h>
#include <groups/ggroupvector.h>
#include <docs/gdocxml.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
#include <langs/gword.h>
#include <langs/gdict.h>
#include <langs/glang.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GDocVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocVector::GDocVector(const char* url,const char* name,unsigned int id,GLang* lang,const char* mime,const char* u,const char* a,unsigned int f,unsigned int n,unsigned int ndiff,unsigned int v,unsigned int vdiff,unsigned int nbf) throw(bad_alloc)
	: GDoc(url,name,id,lang,mime,u,a,f,n,ndiff,v,vdiff,nbf),
	  GIWordsWeights(vdiff>600?vdiff:600)
{
}


//-----------------------------------------------------------------------------
bool GALILEI::GDocVector::HasRepresentation(void) const
{
	return(!GIWordsWeights::IsEmpty());
}


//-----------------------------------------------------------------------------
void GALILEI::GDocVector::ClearInfos(bool l)
{
	GDoc::ClearInfos(l);
	RemoveRefs();
	Clear();
}


//-----------------------------------------------------------------------------
void GALILEI::GDocVector::SetInfos(GLang *l,unsigned int n,unsigned int nd,unsigned int v,unsigned int vd)
{
	GDoc::SetInfos(l,n,nd,v,vd);
	UpdateRefs();
}


//-----------------------------------------------------------------------------
void GALILEI::GDocVector::AddWord(const unsigned int id,const double nb)
{
	InsertPtr(new GIWordWeight(id,nb));
}
//-----------------------------------------------------------------------------
void GALILEI::GDocVector::AddWordList(const unsigned int id,const double nb)
{
	InsertPtr(new GIWordWeight(id,nb,infoWordList));
}


//-----------------------------------------------------------------------------
GIWordWeightCursor& GALILEI::GDocVector::GetWordWeightCursor(void)
{
	GIWordWeightCursor *cur=GIWordWeightCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
double GALILEI::GDocVector::Similarity(const GDoc* doc) const
{
	return(GIWordsWeights::Similarity(dynamic_cast<const GDocVector*>(doc)));
}


//-----------------------------------------------------------------------------
double GALILEI::GDocVector::GlobalSimilarity(const GDoc* doc) const
{
	return(GIWordsWeights::SimilarityIdf(dynamic_cast<const GDocVector*>(doc),otDoc,Lang));
}


//-----------------------------------------------------------------------------
double GALILEI::GDocVector::Similarity(const GSubProfile* sub) const
{
	return(GIWordsWeights::Similarity(dynamic_cast<const GSubProfileVector*>(sub)));
}


//-----------------------------------------------------------------------------
double GALILEI::GDocVector::GlobalSimilarity(const GSubProfile* sub) const
{
	return(GIWordsWeights::SimilarityIdf(dynamic_cast<const GSubProfileVector*>(sub),otDocSubProfile,Lang));
}


//-----------------------------------------------------------------------------
double GALILEI::GDocVector::Similarity(const GGroup* grp) const
{
	return(GIWordsWeights::Similarity(dynamic_cast<const GGroupVector*>(grp)));
}


//-----------------------------------------------------------------------------
double GALILEI::GDocVector::GlobalSimilarity(const GGroup* grp) const
{
	return(GIWordsWeights::SimilarityIdf(dynamic_cast<const GGroupVector*>(grp),otDocGroup,Lang));
}


//-----------------------------------------------------------------------------
void GALILEI::GDocVector::UpdateRefs(void) const
{
	GDict* d;

	if(!Lang) return;
	d=Lang->GetDict();
	if(d)
		AddRefs(otDoc,d);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocVector::RemoveRefs(void) const
{
	GDict* d;

	if(!Lang) return;
	d=Lang->GetDict();
	if(d)
		DelRefs(otDoc,d);
}


//-----------------------------------------------------------------------------
GALILEI::GDocVector::~GDocVector(void)
{
}
