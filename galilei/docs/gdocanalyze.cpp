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
#include <rtextfile.h>
#include <gconcepttree.h>
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
	  OrderTokens(27,27,50,20), Tokens(500), Occurs(20000), Top(200), Depths(100,50),
		  Docs(20), SyntacticPos(20)
{
}


//------------------------------------------------------------------------------
void GDocAnalyze::SkipToken(void)
{
	CurSyntacticPos++;
}


//------------------------------------------------------------------------------
void GDocAnalyze::AddToken(const RString& token,tTokenType type)
{
	// Verify that is not a null token
	if(token==RString::Null)
		return;

	// Search for the token
	GToken* Token;
	size_t Idx1(token.HashIndex(1)), Idx2(token.HashIndex(2));
	bool Find;
	RDblHashContainer<GToken,false>::Hash2* Hash2((*OrderTokens[Idx1])[Idx2]);
	size_t Idx(Hash2->GetIndex(token,Find));

	// Create the token (if necessary)
	if(Find)
	{
		Token=(*Hash2)[Idx];
		if(Token->Type!=type)
			ThrowGException("An existing token with a same type exists");
	}
	else
	{
		if(MemoryTokens.GetNb()<=NbMemoryTokensUsed)
		{
			MemoryTokens.InsertPtr(Token=new GToken(token));
			NbMemoryTokensUsed++;
		}
		else
		{
			Token=MemoryTokens[NbMemoryTokensUsed++];
			Token->Occurs.Clear();
			Token->Token=token;
			Token->Concept=0;
		}
		Token->Type=type;
		Hash2->InsertPtrAt(Token,Idx,false);
		Token->Index=Tokens.GetNb();
		Tokens.InsertPtr(Token);
	}

	// Add a new occurrence
	GToken::Occurrence* Occur;
	if(MemoryOccurs.GetNb()<=NbMemoryOccursUsed)
	{
		size_t nb(0);
		if(Session->DoCreateStruct(pDoc))
			nb=2000;
		MemoryOccurs.InsertPtr(Occur=new GToken::Occurrence(Token,CurVector,CurPos,CurDepth,CurSyntacticPos,nb));
		NbMemoryOccursUsed++;
	}
	else
	{
		Occur=MemoryOccurs[NbMemoryOccursUsed++];
		Occur->Token=Token;
		Occur->Vector=CurVector;
		Occur->Pos=CurPos;
		Occur->Depth=CurDepth;
		Occur->SyntacticPos=CurSyntacticPos;
		Occur->Children.Clear();
	}
	Occur->Index=Occurs.GetNb();
	Occurs.InsertPtr(Occur);
	Token->Occurs.InsertPtr(Occur);

	// Eliminate all the lower tokens from the stack
	while((Depths.GetNb())&&(Depths()->Depth>=Occur->Depth))
		Depths.Pop();

	// Look for a parent and make this token becomes the leading token of its depth
	if(Depths.GetNb())
	{
		GToken::Occurrence* Parent(Depths());
		if(Parent->Depth<Occur->Depth-1)
			cerr<<"Problem"<<endl;
		 Parent->Children.InsertPtr(Occur);
	}
	else
		Top.InsertPtr(Occur);
	Depths.Push(Occur);

	// Increment the syntactic position
	CurSyntacticPos++;
}


//------------------------------------------------------------------------------
void GDocAnalyze::AddToken(const R::RString& token,tTokenType type,GConcept* concept,double weight,GConcept* metaconcept,size_t pos,size_t depth,size_t spos)
{
	// Verify that is not a null concept
	if(!concept)
		return;

	// Get the vector associated with the concept
	CurVector=Description.GetInsertVector(metaconcept);
	CurDepth=depth;
	CurPos=pos;
	if(spos!=SIZE_MAX)
		CurSyntacticPos=spos;
	AddToken(token,type);

	// Get the concept
	GConceptRef* Ref(CurVector->GetRef(concept));
	(*Ref)+=weight;
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractTextual(const R::RString& text,GConcept* metaconcept,size_t pos,size_t depth,size_t spos)
{
	// Get the vector associated with the concept
	CurVector=Description.GetInsertVector(metaconcept);
	CurDepth=depth;
	CurPos=pos;
	if(spos!=SIZE_MAX)
		CurSyntacticPos=spos;

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
void GDocAnalyze::ExtractDCMI(const R::RString& element,const R::RString& value,size_t pos,size_t depth,size_t spos)
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
	ExtractTextual(value,Metadata,pos,depth,spos);
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
void GDocAnalyze::DeleteToken(GToken* token)
{
	// Delete the occurrences and the token from the containers.
	RCursor<GToken::Occurrence> TokenOccur(token->Occurs);
	for(TokenOccur.StartFromEnd();!TokenOccur.Begin();TokenOccur.Prev())
		Occurs.DeletePtrAt(TokenOccur()->Index,false);  // Don't shif them
	token->Type=ttDeleted;
	token->Occurs.Clear();
}


//------------------------------------------------------------------------------
void GDocAnalyze::ReplaceToken(GToken* token,RString value)
{
	// If current token has the value -> nothing to do
	if(token->GetToken()==value)
		return;

	// Look first if another token already exist with that value
	// Search for that token
	size_t Idx1(value.HashIndex(1)), Idx2(value.HashIndex(2));
	bool Find;
	RDblHashContainer<GToken,false>::Hash2* Hash2((*OrderTokens[Idx1])[Idx2]);
	size_t Idx(Hash2->GetIndex(value,Find));

	if(Find)
	{
		// Merge the two tokens occurrences by ascending order of index in Occurs
		GToken* NewToken((*Hash2)[Idx]);
		if(NewToken->Type==ttDeleted)
			NewToken->Type=token->Type;
		RCursor<GToken::Occurrence> Occur(token->Occurs);
		for(Occur.Start();!Occur.End();Occur.Next())
		{
			NewToken->Occurs.InsertPtr(Occur());
			Occur()->Token=NewToken;
		}
		NewToken->Occurs.ReOrder();

		// Considered it as deleted
		token->Occurs.Clear();
		token->Type=ttDeleted;
	}
	else
	{
		// Replace the token : Simply replace it in OrderTokens.
		OrderTokens.DeletePtr(*token);
		token->Token=value;
		OrderTokens.InsertPtr(token);
	}
}


//------------------------------------------------------------------------------
void GDocAnalyze::SetLang(GLang* lang)
{
	Lang=lang;
}


//------------------------------------------------------------------------------
void GDocAnalyze::BuildTensor(void)
{
	// For each token, add the occurrences for a given vector
	RCursor<GToken::Occurrence> Occur(Occurs);
	for(Occur.Start();!Occur.End();Occur.Next())
	{
		// Never treat a deleted token
		if(Occur()->Token->Type==ttDeleted)
			continue;

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
bool GDocAnalyze::ReadIndex(GConcept* concept,size_t& read)
{
	RIndexFile* Index(Session->GObjects<GDoc>::Index);
	RNumContainer<size_t,false>* CurPos;
	size_t nb,id,nbpos,pos;

	// Load in Docs the identifier of documents, and in Pos their positions.
	bool Found(false);
	read=0;
	Docs.Clear();
	size_t BlockId(concept->GetIndex(otDoc));
	if(BlockId)
	{
		// Position the block file to the correct position
		Index->Seek(BlockId,concept->GetId());

		// Read the number of documents
		Index->Read((char*)&nb,sizeof(size_t));
		read+=sizeof(size_t);

		// Read the details
		for(size_t i=0;i<nb;i++)
		{
			// Read the identifier of the documents
			Index->Read((char*)&id,sizeof(size_t));
			Index->Read((char*)&nbpos,sizeof(size_t));

			if(id==Doc->GetId())
			{
				Found=true;
				for(size_t j=0;j<nbpos;j++)
					Index->Read((char*)&pos,sizeof(size_t));
			}
			else
			{
				read+=(nbpos+2)*sizeof(size_t);
				Docs.Insert(id);
				SyntacticPos.InsertPtrAt(CurPos=new RNumContainer<size_t,false>(nbpos),i);

				for(size_t j=0;j<nbpos;j++)
				{
					Index->Read((char*)&pos,sizeof(size_t));
					CurPos->Insert(pos);
				}
			}
		}
	}
	return(Found);
}


//------------------------------------------------------------------------------
void GDocAnalyze::WriteIndex(GConcept* concept,size_t write)
{
	RIndexFile* Index(Session->GObjects<GDoc>::Index);
	RNumContainer<size_t,false>* CurPos;
	size_t nb,id,nbpos,pos;

	// Prepare the writing
	size_t BlockId(concept->GetIndex(otDoc));
	Index->Seek(BlockId,concept->GetId(),write);
	nb=Docs.GetNb();
	Index->Write((char*)&nb,sizeof(size_t));
	RNumCursor<size_t> Doc(Docs);
	for(Doc.Start();!Doc.End();Doc.Next())
	{
		CurPos=SyntacticPos[Doc.GetPos()];
		id=Doc();
		nbpos=CurPos->GetNb();
		Index->Write((char*)&id,sizeof(size_t));
		Index->Write((char*)&nbpos,sizeof(size_t));
		RNumCursor<size_t> Occur(*CurPos);
		for(Occur.Start();!Occur.End();Occur.Next())
		{
			pos=Occur();
			Index->Write((char*)&pos,sizeof(size_t));
		}
	}
	concept->SetIndex(otDoc,BlockId);
}


//------------------------------------------------------------------------------
void GDocAnalyze::UpdateIndex(void)
{
	// Remove information for the older Document information
	R::RCursor<GVector> Vector(Doc->GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		// Update the index for all concepts
		// 1. Read all the information related to the documents indexed except the current one.
		// 2. Simply rewrite the information
		R::RCursor<GConceptRef> Cur(Vector()->GetRefs());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			size_t ToWrite;
			if(ReadIndex(Cur()->GetConcept(),ToWrite))
				WriteIndex(Cur()->GetConcept(),ToWrite);
		}
	}

	// Add information from current analysis.
	// 1. Read the existing document information.
	// 2. Add the information for the current one
	// 3. Write the whole thing back
	RCursor<GToken> Token(Tokens);
	for(Token.Start();!Token.End();Token.Next())
	{
		// Never treat a deleted token
		if(Token()->Type==ttDeleted)
			continue;

		if(!Token()->Concept)
			continue;
		size_t ToWrite;
		if(ReadIndex(Token()->GetConcept(),ToWrite))
			ThrowGException("Big problem for concept "+RString::Number(Token()->GetConcept()->GetId()));

		ToWrite+=(Token()->Occurs.GetNb()+3)*sizeof(size_t);
		Docs.Insert(Doc->GetId());
		Docs.ReOrder(); // ReOrder the documents
		bool Find;
		size_t Pos(Docs.GetId(Doc->GetId(),Find));
		RNumContainer<size_t,false>* CurPos(new RNumContainer<size_t,false>(Token()->Occurs.GetNb()));
		SyntacticPos.InsertPtrAt(CurPos,Pos,false);
		RCursor<GToken::Occurrence> Occur(Token()->Occurs);
		for(Occur.Start();!Occur.End();Occur.Next())
			CurPos->Insert(Occur()->SyntacticPos);
		CurPos->ReOrder();
		WriteIndex(Token()->GetConcept(),ToWrite);
	}
}


//------------------------------------------------------------------------------
void GDocAnalyze::BuildStruct(GConceptNode* parent,GToken::Occurrence* token)
{
	// Never treat a deleted token
	if(token->Token->Type==ttDeleted)
		return;

	// Is the token a valid concept
	GConcept* Concept(token->Token->Concept);
	if(!Concept)
		return;

	// Create a node

	// Treat its children
//	RCursor<GToken::Occurrence> Occur(token->Children);
//	for(Occur.Start();!Occur.End();Occur.Next())
//		BuildStruct(token,Occur());
}


//------------------------------------------------------------------------------
void GDocAnalyze::Analyze(GDoc* doc,bool ram)
{
	// Verify the the document must be computed
	if(!doc->MustCompute()) return;

	// Prepare the analyze
	NbMemoryTokensUsed=0;
	NbMemoryOccursUsed=0;
	CurSyntacticPos=0;
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
	Depths.Clear();

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

	// Set the information to the document
	BuildTensor();
	UpdateIndex();
	doc->Update(Lang,Description,Struct,ram||(!Save),DelRefs);
	RCursor<GToken::Occurrence> Occur(Top);
	for(Occur.Start();!Occur.End();Occur.Next())
		BuildStruct(0,Occur());

	// Save the information related to the object
	if(Save&&(!ram))
	{
		if(doc->IsDefined())
			doc->SaveDesc();
		if(Session->DoCreateStruct(pDoc))
			Session->SaveStruct(pDoc,Struct,doc->StructId,doc->Id);
		Session->Storage->SaveObj(doc);
		if(ram)
			doc->SetState(osSaved);
	}
}


//------------------------------------------------------------------------------
GDocAnalyze::~GDocAnalyze(void)
{
}
