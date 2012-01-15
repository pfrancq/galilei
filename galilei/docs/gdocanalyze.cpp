/*

	GALILEI Research Project

	GDocAnalyze.cpp

	AGeneric Document Analysis - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <ctype.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdocanalyze.h>
#include <gdoc.h>
#include <gsession.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <ggalileiapp.h>
#include <gslot.h>
#include <gfilter.h>
#include <gtokenizer.h>
#include <ganalyzer.h>
#include <gtexttoken.h>
#include <rtextfile.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
// Constant
const size_t MaxWordLen=50;
const bool Debug=false;



//------------------------------------------------------------------------------
//
// class GDocAnalyze
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocAnalyze::GDocAnalyze(GSession* session)
	: RDownload(), Session(session), Description(), Struct(), Tokenizer(0),
	 DCMI(0), Body(0), MemoryTokens(500), MemoryOccurs(20000),
	  OrderTokens(27,27,50,20), Tokens(500), Occurs(20000)
{
	// Create some tokens and some occurrences
	for(size_t i=500;--i;)
		MemoryTokens.InsertPtr(new GTextToken());
	for(size_t i=20000;--i;)
		MemoryOccurs.InsertPtr(new GTextToken::Occurrence());
}


//------------------------------------------------------------------------------
void GDocAnalyze::AddToken(const RString& token)
{
	// Verify that is not a null token
	if(token==RString::Null)
		return;

	// Search for the token
	GTextToken* Token;
	size_t Idx1(token.HashIndex(1)), Idx2(token.HashIndex(2));
	bool Find;
	RDblHashContainer<GTextToken,false>::Hash2* Hash2((*OrderTokens[Idx1])[Idx2]);
	size_t Idx(Hash2->GetIndex(token,Find));

	// Create the token (if necessary)
	if(Find)
		Token=(*Hash2)[Idx];
	else
	{
		if(MemoryTokens.GetNb()<=NbMemoryTokensUsed)
		{
			MemoryTokens.InsertPtr(Token=new GTextToken(token));
			NbMemoryTokensUsed++;
		}
		else
		{
			Token=MemoryTokens[NbMemoryTokensUsed++];
			Token->Occurs.Clear();
			Token->Token=token;
			Token->Concept=0;
		}
		Hash2->InsertPtrAt(Token,Idx,false);
		Token->Index=Tokens.GetNb();
		Tokens.InsertPtr(Token);
	}

	// Add a new occurrence
	GTextToken::Occurrence* Occur;
	if(MemoryOccurs.GetNb()<=NbMemoryOccursUsed)
	{
		MemoryOccurs.InsertPtr(Occur=new GTextToken::Occurrence(Token,CurVector,CurPos,CurDepth));
		NbMemoryOccursUsed++;
	}
	else
	{
		Occur=MemoryOccurs[NbMemoryOccursUsed++];
		Occur->Token=Token;
		Occur->Vector=CurVector;
		Occur->Pos=CurPos;
		Occur->Depth=CurDepth;
	}
	Occur->Index=Occurs.GetNb();
	Occurs.InsertPtr(Occur);
	Token->Occurs.InsertPtr(Occur);
}


//------------------------------------------------------------------------------
void GDocAnalyze::AddConcept(GConcept* concept,double weight,GConcept* metaconcept,size_t pos,size_t depth)
{
	// Verify that is not a null concept
	if(!concept)
		return;

	// Get the vector associated with the concept
	CurVector=Description.GetInsertVector(metaconcept);
	CurDepth=depth;
	CurPos=pos;

	// Get the concept
	GConceptRef* Ref(CurVector->GetRef(concept));
	(*Ref)+=weight;
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractTextual(const R::RString& text,GConcept* metaconcept,size_t pos,size_t depth)
{
	// Get the vector associated with the concept
	CurVector=Description.GetInsertVector(metaconcept);
	CurDepth=depth;
	CurPos=pos;

	// Parse the text to detect the tokens
	if(Debug)
		cout<<"Treat *"+text+"*"<<endl;
	const RChar* Car(text());
	Tokenizer->Start();  // Make sure previous stuff is cleaned
	while(!Car->IsNull())
	{
		Tokenizer->TreatChar(this,*Car);
		Car++;
		CurPos++;
	}
	Tokenizer->TreatChar(this,0); // Be sure to terminate with the zero character.
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractDCMI(const R::RString& element,const R::RString& value,size_t pos,size_t depth)
{
	// Search for the concept types corresponding to the DMCI
	if(!DCMI)
	{
		// Verify that all concepts are OK.
		DCMI=Session->GetInsertConceptType(Session->GetInsertConceptCat("Metadata"),"http://purl.org/dc/elements/1.1","Dublin Core Metadata Initiative (DMCI)");
		Session->InsertConcept(DCMI,"contributor");
		Session->InsertConcept(DCMI,"coverage");
		Session->InsertConcept(DCMI,"creator");
		Session->InsertConcept(DCMI,"date");
		Session->InsertConcept(DCMI,"description");
		Session->InsertConcept(DCMI,"format");
		Session->InsertConcept(DCMI,"identifier");
		Session->InsertConcept(DCMI,"language");
		Session->InsertConcept(DCMI,"publisher");
		Session->InsertConcept(DCMI,"relation");
		Session->InsertConcept(DCMI,"rights");
		Session->InsertConcept(DCMI,"source");
		Session->InsertConcept(DCMI,"subject");
		Session->InsertConcept(DCMI,"title");
		Session->InsertConcept(DCMI,"type");
	}

	// Find the vector corresponding to the concept
	GConcept* Metadata(DCMI->GetConcept(element));
	if(!Metadata)
		ThrowGException("'"+element+"' is not a valid DCMI element");
	ExtractTextual(value,Metadata,pos,depth);
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractBody(const R::RString& content,size_t pos,size_t depth)
{
	// Search for the meta-concept corresponding to the default textual content
	if(!Body)
		Body=Session->InsertConcept(Session->GetInsertConceptType(Session->GetInsertConceptCat("Text"),"content","Raw content"),"body");

	// Find the vector corresponding to the concept
	ExtractTextual(content,Body,pos,depth);
}


//------------------------------------------------------------------------------
void GDocAnalyze::AssignPlugIns(void)
{
	Tokenizer=GALILEIApp->GetCurrentPlugIn<GTokenizer>("Tokenizer");
	if(!Tokenizer)
		ThrowGException("No tokenizer selected");
	Analyzers=GALILEIApp->GetPlugIns<GAnalyzer>("Analyzer");
}


//------------------------------------------------------------------------------
void GDocAnalyze::DeleteToken(GTextToken* token)
{
	// Delete the occurrences and the token from the containers.
	RCursor<GTextToken::Occurrence> TokenOccur(token->Occurs);
	for(TokenOccur.StartFromEnd();!TokenOccur.Begin();TokenOccur.Prev())
		Occurs.DeletePtrAt(TokenOccur()->Index);
	Tokens.DeletePtrAt(token->Index);
	OrderTokens.DeletePtr(*token);

	// Update the index for the tokens (if necessary)
	if(token->Index<Tokens.GetNb())
	{
		RCursor<GTextToken> Token(Tokens);
		for(Token.GoTo(token->Index);!Token.End();Token.Next())
			Token()->Index--;
	}

	// Update the index for the occurrences
	RCursor<GTextToken::Occurrence> Occur(Occurs);
	TokenOccur.Start();
	if((TokenOccur.End())||(TokenOccur()->Index>=Occurs.GetNb()))
		return;
	Occur.GoTo(TokenOccur()->Index);
	size_t Dec(1);
	TokenOccur.Next();
	for(;!Occur.End();Occur.Next())
	{
		while((!TokenOccur.End())&&(Occur()->Index>TokenOccur()->Index))
		{
			Dec++;
			TokenOccur.Next();
		}
		Occur()->Index-=Dec;
	}
}


//------------------------------------------------------------------------------
bool GDocAnalyze::ReplaceToken(GTextToken* token,RString value)
{
	// If current token has the value -> nothing to do
	if(token->GetToken()==value)
		return(true);

	// Look first if another token already exist with that value
	// Search for the token
	size_t Idx1(value.HashIndex(1)), Idx2(value.HashIndex(2));
	bool Find;
	RDblHashContainer<GTextToken,false>::Hash2* Hash2((*OrderTokens[Idx1])[Idx2]);
	size_t Idx(Hash2->GetIndex(value,Find));

	if(Find)
	{
		// Merge the two tokens occurrences by ascending order of index in Occurs
		GTextToken* NewToken((*Hash2)[Idx]);
		RCursor<GTextToken::Occurrence> Occur(token->Occurs);
		for(Occur.Start();!Occur.End();Occur.Next())
		{
			NewToken->Occurs.InsertPtr(Occur());
			Occur()->Token=NewToken;
		}
		NewToken->Occurs.ReOrder();

		// Remove token
		Tokens.DeletePtrAt(token->Index);
		Hash2->DeletePtrAt(Idx);

		// Update the index for the tokens (if necessary)
		if(token->Index<Tokens.GetNb())
		{
			RCursor<GTextToken> Token(Tokens);
			for(Token.GoTo(token->Index);!Token.End();Token.Next())
				Token()->Index--;
		}
		token=NewToken;
	}
	else
	{
		// Replace the token : Simply replace it in OrderTokens.
		OrderTokens.DeletePtr(*token);
		token->Token=value;
		OrderTokens.InsertPtr(token);
	}

	return(!Find);
}


//------------------------------------------------------------------------------
void GDocAnalyze::SetLang(GLang* lang)
{
	Lang=lang;
}


//------------------------------------------------------------------------------
void GDocAnalyze::PostAnalyze(void)
{
	// Parse the occurrences :
	// 1. For each token, add the occurrences for a given vector
	RCursor<GTextToken::Occurrence> Occur(Occurs);
	for(Occur.Start();!Occur.End();Occur.Next())
	{
		// Is the token a valid concept
		GConcept* Concept(Occur()->Token->Concept);
		if(!Concept)
			continue;

		// Create the entry in the corresponding vector and increment its weights
		GConceptRef* Ref(Occur()->Vector->GetRef(Concept));
		(*Ref)+=1.0;
	}

	// Verify that there is no empty vector
	RContainer<GVector,false,false> ToDel(10);
	RCursor<GVector> Vector(Description.GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
		if(!Vector()->GetNb())
			ToDel.InsertPtr(Vector());
	Vector.Set(ToDel);
	for(Vector.Start();!Vector.End();Vector.Next())
		Description.DeleteVector(Vector()->GetMetaConcept());
}


//------------------------------------------------------------------------------
void GDocAnalyze::Analyze(GDoc* doc,bool ram)
{
	// Verify the the document must be computed
	if(!doc->MustCompute()) return;

	// Prepare the analyze
	NbMemoryTokensUsed=0;
	NbMemoryOccursUsed=0;
	Doc=doc;
	Lang=doc->GetLang();
	bool Save=(Session->SaveResults&&(doc->GetId()!=cNoRef));
	bool DelRefs(doc->IsDefined());
	if((!Tokenizer)||(!Analyzers.GetNb()))
		AssignPlugIns();
	Description.Clear();
	Struct.Clear();
	OrderTokens.Clear();
	Tokens.Clear();
	Occurs.Clear();

	// Find the filter for this document
	R::RSmartTempFile TmpFile;
	RURI File;
	GFilter* Filter(GALILEIApp->FindMIMEType(doc));

	// If it is not a local	file -> Download it
	if(doc->GetURI().GetScheme()!="file")
	{
		File=TmpFile.GetName();
		DownloadFile(doc->GetURI(),File);
	}
	else
		File=doc->GetURI();

	// Analyze it with the good filter
	Filter->Analyze(this,doc,File);

	// Treat the tokens by the different analyzers.
	for(Analyzers.Start();!Analyzers.End();Analyzers.Next())
		Analyzers()->TreatTokens(this);

	PostAnalyze();

	// Set the information to the document

	doc->Update(Lang,Description,Struct,ram||(!Save),DelRefs);

	// Save the information related to the object
	if(Save&&(!ram))
	{
		if(doc->IsDefined())
			doc->SaveDesc();
		if(Struct.GetNbNodes())
			Session->SaveStruct(pDoc,&Struct,doc->StructId,doc->Id);
		Session->Storage->SaveObj(doc);
		if(ram)
			doc->SetState(osSaved);
	}
}


//------------------------------------------------------------------------------
GDocAnalyze::~GDocAnalyze(void)
{
}
