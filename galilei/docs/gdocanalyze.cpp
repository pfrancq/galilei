/*

	GALILEI Research Project

	GDocAnalyze.cpp

	AGeneric Document Analysis - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
#include <gconceptnode.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
// Constant
bool Debug=false;



//------------------------------------------------------------------------------
//
// class GDocAnalyze
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocAnalyze::GDocAnalyze(GSession* session)
	: RDownloadFile(), Session(session), Description(), Tree(0,20000,2000), Tokenizer(0),
	  DefaultText(0), DCMI(0), DefaultURI(0), DefaultNamedEntity(0),
	  MemoryTokens(500), MemoryOccurs(20000),
	  OrderTokens(27,27,50,20), Tokens(500), Occurs(20000), Top(200), Depths(100,50),
	  SyntacticPos(20)
{
}


//------------------------------------------------------------------------------
size_t GDocAnalyze::SkipToken(void)
{
	return(CurSyntacticPos++);
}


//------------------------------------------------------------------------------
GToken* GDocAnalyze::CreateToken(const R::RString& token,tTokenType type)
{
	GToken* Token;

	if(MemoryTokens.GetNb()<=NbMemoryTokensUsed)
	{
		// A new token must be allocated
		MemoryTokens.InsertPtr(Token=new GToken(token));
		NbMemoryTokensUsed++;
	}
	else
	{
		// Get a free existing token
		Token=MemoryTokens[NbMemoryTokensUsed++];
		Token->Occurs.Clear();
		Token->Token=token;
		Token->Concept=0;
	}
	Token->Type=type;
	return(Token);
}


//------------------------------------------------------------------------------
GConcept* GDocAnalyze::GetDefaultText(void)
{
	if(!DefaultText)
		DefaultText=Session->InsertObj(pConcept,Session->GetObj(pConceptType,ccText,"text block","A block of text"),"*");
	return(DefaultText);
}


//------------------------------------------------------------------------------
GConceptType* GDocAnalyze::GetDCMI(void)
{
	// Search for the concept types corresponding to the DMCI
	if(!DCMI)
	{
		// Verify that all concepts are OK.
		DCMI=Session->GetObj(pConceptType,ccMetadata,"http://purl.org/dc/elements/1.1","Dublin Core Metadata Initiative (DMCI)");
		Session->InsertObj(pConcept,DCMI,"contributor");
		Session->InsertObj(pConcept,DCMI,"coverage");
		Session->InsertObj(pConcept,DCMI,"creator");
		Session->InsertObj(pConcept,DCMI,"date");
		Session->InsertObj(pConcept,DCMI,"description");
		Session->InsertObj(pConcept,DCMI,"format");
		Session->InsertObj(pConcept,DCMI,"identifier");
		Session->InsertObj(pConcept,DCMI,"language");
		Session->InsertObj(pConcept,DCMI,"publisher");
		Session->InsertObj(pConcept,DCMI,"relation");
		Session->InsertObj(pConcept,DCMI,"rights");
		Session->InsertObj(pConcept,DCMI,"source");
		Session->InsertObj(pConcept,DCMI,"subject");
		Session->InsertObj(pConcept,DCMI,"title");
		Session->InsertObj(pConcept,DCMI,"type");
	}
	return(DCMI);
}


//------------------------------------------------------------------------------
GConcept* GDocAnalyze::GetDefaultURI(void)
{
	if(!DefaultURI)
		DefaultURI=Session->InsertObj(pConcept,Session->GetObj(pConceptType,ccLink,"URI","Uniform Resource Identifier"),"*");
	return(DefaultURI);
}


//------------------------------------------------------------------------------
GConceptType* GDocAnalyze::GetDefaultNamedEntityType(void)
{
	if(!DefaultNamedEntityType)
		DefaultNamedEntityType=Session->GetObj(pConceptType,ccText,"Named Entities","Named Entities");
	return(DefaultNamedEntityType);
}


//------------------------------------------------------------------------------
GConcept* GDocAnalyze::GetDefaultNamedEntity(void)
{
	if(!DefaultNamedEntity)
		DefaultNamedEntity=Session->InsertObj(pConcept,GetDefaultNamedEntityType(),"*");
	return(DefaultNamedEntity);
}


//------------------------------------------------------------------------------
void GDocAnalyze::SetCurrentVector(GVector* vector)
{
	// Verify this vector is a valid one
	if(vector!=Description.GetInsertVector(vector->GetMetaConcept()))
		mThrowGException("Invalid vector");
	CurVector=vector;
}


//------------------------------------------------------------------------------
GTokenOccur* GDocAnalyze::AddToken(const RString& token,tTokenType type,double weight)
{
	// Verify that is not a null token
	if(token==RString::Null)
		return(0);

	// Verify that the token is not empty
	if(token.IsEmpty())
		return(0);

	// Search for the token
	GToken* Token;
	size_t Idx1(token.HashIndex(1)), Idx2(token.HashIndex(2));
	bool Find;
	RDblHashContainer<GToken,false>::Hash2* Hash2((*OrderTokens[Idx1])[Idx2]);
	size_t Idx(Hash2->GetIndex(GToken::Search(token,type),Find));
	if(type==ttUnknown)
		type=CurTokenType;
	if(weight==0.0)
		weight=CurTokenWeight;

	// Create the token (if necessary)
	if(Find)
	{
		Token=(*Hash2)[Idx];
		if(Token->Type!=type)
			mThrowGException("The token '"+token+"' exists with different token type");
	}
	else
	{
		Token=CreateToken(token,type);
		Hash2->InsertPtrAt(Token,Idx,false);
		Token->Index=Tokens.GetNb();
		Tokens.InsertPtr(Token);
	}

	// Add a new occurrence
	GTokenOccur* Occur;
	if(MemoryOccurs.GetNb()<=NbMemoryOccursUsed)
	{
		size_t nb(0);
		if(Session->DoCreateTree(pDoc))
			nb=2000;
		MemoryOccurs.InsertPtr(Occur=new GTokenOccur(Token,CurVector,CurPos,CurDepth,CurSyntacticPos,nb));
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
	if(Debug)
		cout<<"  "<<Occur->Pos<<endl;
	Occur->Index=Occurs.GetNb();
	Occur->Weight=weight;
	Occurs.InsertPtr(Occur);
	Token->Occurs.InsertPtr(Occur);

	// Eliminate all the lower tokens from the stack
	while((Depths.GetNb())&&(Depths()->Depth>=Occur->Depth))
		Depths.Pop();

	// Look for a parent and make this token becomes the leading token of its depth
	if(Depths.GetNb())
	{
		GTokenOccur* Parent(Depths());
		if(Parent->Depth<Occur->Depth-1)
		{
			if(!DepthError)
			{
				Session->GetLog().WriteLog(Doc->GetName()+": The depths of the tokens '"+token+"' ("+RString::Number(Occur->Depth)+") and '"+Parent->Token->Token+"' ("+RString::Number(Parent->Depth)+") are not synchronized",true);
				DepthError=true;
			}
		}
		else
			DepthError=false;

		Parent->Children.InsertPtr(Occur);
	}
	else
		Top.InsertPtr(Occur);
	Depths.Push(Occur);

	// Increment the syntactic position
	CurSyntacticPos++;

	return(Occur);
}


//------------------------------------------------------------------------------
GTokenOccur* GDocAnalyze::AddToken(const R::RString& token,GConcept* metaconcept,tTokenType type,double weight)
{
	CurVector=Description.GetInsertVector(metaconcept);
	return(AddToken(token,type,weight));
}


//------------------------------------------------------------------------------
GTokenOccur* GDocAnalyze::AddToken(const R::RString& token,tTokenType type,GConcept* concept,double weight,GConcept* metaconcept,size_t pos,size_t depth,size_t spos)
{
	// Verify that the token is not empty
	if(token.IsEmpty())
		return(0);

	// Verify that is not a null concept
	if(!concept)
		return(0);

	// Get the vector associated with the concept
	CurVector=Description.GetInsertVector(metaconcept);
	CurDepth=depth;
	CurPos=pos;
	CurTokenType=type;
	CurTokenWeight=weight;

	if(spos!=SIZE_MAX)
		CurSyntacticPos=spos;
	if(Debug)
		cout<<"In AddToken *"+token+"*"<<endl;
	GTokenOccur* Occur(AddToken(token,type,weight));
	if(Occur->Token->Concept)
	{
		if(Occur->Token->Concept!=concept)
			mThrowGException("Token  '"+token+"' associated to multiple concepts");
	}
	else
		Occur->Token->Concept=concept;

	return(Occur);
}


//------------------------------------------------------------------------------
GTokenOccur* GDocAnalyze::AddDefaultNamedEntityToken(const R::RString& token,double weight,size_t pos,size_t depth,size_t spos)
{
	// Verify that the token is not empty
	if(token.IsEmpty())
		return(0);

	// Be sure that all parts starts with a uppercase
	RString Name;
	Name.SetLen(token.GetLen());
	Name.SetLen(0);
	bool Upper(true);
	RChar* Car(Name());
	while(!Car->IsNull())
	{
		if(Upper)
		{
			while((!Car->IsNull())&&Car->IsSpace())
				Car++;
			Name+=RChar::ToUpper(*Car);
			Upper=false;
			Car++;
		}
		else if(Car->IsSpace())
		{
			Upper=true;
			Name+=" ";
		}
		else
			Name+=(*Car);
	}
	GConcept* Concept(Session->InsertObj(pConcept,GetDefaultNamedEntityType(),token));
	AddToken(token,ttText,Concept,weight,GetDefaultNamedEntity(),pos,depth,spos);
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractText(const R::RString& text,GConcept* metaconcept,size_t pos,size_t depth,size_t spos)
{
	ExtractText(text,ttText,1.0,metaconcept,pos,depth,spos);
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractText(const R::RString& text,tTokenType type,double weight,GConcept* metaconcept,size_t pos,size_t depth,size_t spos)
{
	// Verify that the text is not empty
	if(text.IsEmpty())
		return;

	// Get the vector associated with the concept
	CurVector=Description.GetInsertVector(metaconcept);
	CurDepth=depth;
	CurPos=pos;
	if(spos!=SIZE_MAX)
		CurSyntacticPos=spos;
	CurTokenType=type;
	CurTokenWeight=weight;

	// Parse the text to detect the tokens
	if(Debug)
		cout<<"Extract Text *"+text+"*"<<endl;
	const RChar* Car(text());
	Tokenizer->Start();  // Make sure previous stuff is cleaned
	size_t Read(CurPos);
	while(!Car->IsNull())
	{
		if(Tokenizer->TreatChar(this,*Car))
			CurPos=Read;
		Car++;
		Read++;
	}
	Tokenizer->TreatChar(this,0); // Be sure to terminate with the zero character.
	CurPos=Read;
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractDCMI(const R::RString& element,const R::RString& value,size_t pos,size_t depth,size_t spos)
{
	// Find the vector corresponding to the concept
	GConcept* Metadata(GetDCMI()->GetObj(pConcept,element,true));
	if(!Metadata)
		mThrowGException("'"+element+"' is not a valid DCMI element");
	ExtractText(value,Metadata,pos,depth,spos);
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractDefaultText(const R::RString& content,size_t pos,size_t depth,size_t spos)
{
	ExtractDefaultText(content.ToLower(),ttText,1.0,pos,depth,spos);
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractDefaultText(const R::RString& content,tTokenType type,double weight,size_t pos,size_t depth,size_t spos)
{
	// Find the vector corresponding to the concept
	ExtractText(content.ToLower(),type,weight,GetDefaultText(),pos,depth,spos);
}


//------------------------------------------------------------------------------
void GDocAnalyze::ExtractDefaultURI(const R::RString& uri,size_t pos,size_t depth,size_t spos)
{
	ExtractText(uri,GetDefaultURI(),pos,depth,spos);
}


//------------------------------------------------------------------------------
void GDocAnalyze::AssignPlugIns(void)
{
	Tokenizer=GALILEIApp->GetCurrentPlugIn<GTokenizer>("Tokenizer");
	if(!Tokenizer)
		mThrowGException("No tokenizer selected");
	Analyzers=GALILEIApp->GetPlugIns<GAnalyzer>("Analyzer");
}


//------------------------------------------------------------------------------
void GDocAnalyze::DeleteToken(GToken* token)
{
	// Delete the occurrences and the token from the containers.
	RCursor<GTokenOccur> TokenOccur(token->Occurs);
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
	size_t Idx(Hash2->GetIndex(GToken::Search(value,token->Type),Find));

	if(Find)
	{
		// Merge the two token occurrences.
		GToken* NewToken((*Hash2)[Idx]);
		if(NewToken->Type==ttDeleted)
			NewToken->Type=token->Type;
		RCursor<GTokenOccur> Occur(token->Occurs);
		for(Occur.Start();!Occur.End();Occur.Next())
		{
			NewToken->Occurs.InsertPtr(Occur());
			Occur()->Token=NewToken;
		}

		// Ensure that the ascending order of index in Occurs is respected
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
void GDocAnalyze::MoveToken(GTokenOccur* occur,R::RString value)
{
	if(!occur)
		mThrowGException("Null token occurrence");

	// If current token has the value -> nothing to do
	GToken* Token(occur->Token);
	if(Token->GetToken()==value)
		return;

	// Look first if another token already exist with that value
	// Search for that token
	size_t Idx1(value.HashIndex(1)), Idx2(value.HashIndex(2));
	bool Find;
	RDblHashContainer<GToken,false>::Hash2* Hash2((*OrderTokens[Idx1])[Idx2]);
	size_t Idx(Hash2->GetIndex(GToken::Search(value,Token->Type),Find));

	if(Find)
	{
		// Copy the occurrence from one token to another.
		GToken* NewToken((*Hash2)[Idx]);
		if(NewToken->Type==ttDeleted)
			NewToken->Type=occur->Token->Type;
		Token->Occurs.DeletePtr(*occur);
		NewToken->Occurs.InsertPtr(occur);
		occur->Token=NewToken;

		// Ensure that the ascending order of index in Occurs is respected
		NewToken->Occurs.ReOrder();

		// Considered it as deleted if no more occurrences
		if(!Token->Occurs.GetNb())
		{
			Token->Occurs.Clear();
			Token->Type=ttDeleted;
		}
	}
	else
	{
		if(Token->Occurs.GetNb()==1)
		{
			// Replace the token : Simply replace it in OrderTokens.
			OrderTokens.DeletePtr(*Token);
			Token->Token=value;
			OrderTokens.InsertPtr(Token);
		}
		else
		{
			// A new token must be created
			GToken* NewToken(CreateToken(value,Token->Type));
			OrderTokens.InsertPtr(NewToken);
			NewToken->Index=Tokens.GetNb();
			Tokens.InsertPtr(NewToken);

			// Move the occurrence to it
			Token->Occurs.DeletePtr(*occur);
			NewToken->Occurs.InsertPtr(occur);
			occur->Token=NewToken;
		}
	}
}


//------------------------------------------------------------------------------
void GDocAnalyze::MoveToken(GTokenOccur* occur,GConcept* concept)
{
	if(!occur)
		mThrowGException("Null token occurrence");
	if(!concept)
		mThrowGException("Null concept");

	// If current token has the concept -> nothing to do
	GToken* Token(occur->Token);
	if(Token->Concept==concept)
		return;

	// Look first if another token already exist with that concept
	// Search for that token
	RString Value(concept->GetName());
	size_t Idx1(Value.HashIndex(1)), Idx2(Value.HashIndex(2));
	bool Find;
	RDblHashContainer<GToken,false>::Hash2* Hash2((*OrderTokens[Idx1])[Idx2]);
	size_t Idx(Hash2->GetIndex(GToken::Search(Value,Token->Type),Find));

	if(Find)
	{
		// Copy the occurrence from one token to another.
		GToken* NewToken((*Hash2)[Idx]);
		if(NewToken->Concept!=concept)
			mThrowGException("Token '"+Value+"' associated to multiple concepts");
		if(NewToken->Type==ttDeleted)
			NewToken->Type=occur->Token->Type;
		Token->Occurs.DeletePtr(*occur);
		NewToken->Occurs.InsertPtr(occur);
		occur->Token=NewToken;

		// Ensure that the ascending order of index in Occurs is respected
		NewToken->Occurs.ReOrder();

		// Considered it as deleted if no more occurrences
		if(!Token->Occurs.GetNb())
		{
			Token->Occurs.Clear();
			Token->Type=ttDeleted;
		}
	}
	else
	{
		if(Token->Occurs.GetNb()==1)
		{
			// Replace the token : Simply replace the concept
			OrderTokens.DeletePtr(*Token);
			Token->Token=Value;
			Token->Concept=concept;
			OrderTokens.InsertPtr(Token);
		}
		else
		{
			// A new token must be created
			GToken* NewToken(CreateToken(Value,Token->Type));
			OrderTokens.InsertPtr(NewToken);
			NewToken->Index=Tokens.GetNb();
			Tokens.InsertPtr(NewToken);
			NewToken->Concept=concept;

			// Move the occurrence to it
			Token->Occurs.DeletePtr(*occur);
			NewToken->Occurs.InsertPtr(occur);
			occur->Token=NewToken;
		}
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
	RCursor<GTokenOccur> Occur(Occurs);
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
		(*Ref)+=Occur()->Weight;
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
void GDocAnalyze::BuildTree(GConceptNode* parent,GTokenOccur* occur)
{
	// Never treat a deleted token
	if(occur->Token->Type==ttDeleted)
		return;

	// Is the token a valid concept
	GConcept* Concept(occur->Token->Concept);
	if(!Concept)
		return;

	// Create a node
	GConceptNode* Node(Tree.InsertNode(parent,occur->Token->Type,occur->Token->Concept->GetId(),occur->SyntacticPos,occur->Pos,occur->Depth));

	// Treat its children
	RCursor<GTokenOccur> Occur(occur->Children);
	for(Occur.Start();!Occur.End();Occur.Next())
		BuildTree(Node,Occur());
}


//------------------------------------------------------------------------------
void GDocAnalyze::Print(GTokenOccur* occur)
{
	if(Debug)
		cout<<"Create node "<<occur->Token->Token<<","<<occur->Pos<<endl;

	// Treat its children
	RCursor<GTokenOccur> Occur(occur->Children);
	for(Occur.Start();!Occur.End();Occur.Next())
		Print(Occur());
}


//------------------------------------------------------------------------------
void GDocAnalyze::Analyze(GDoc* doc,bool force)
{
	// Verify the the document must be computed
	if((!doc->MustCompute())&&(!force)) return;

	if(doc->GetId()!=cNoRef)
		Session->GetLog().WriteLog("Analyze "+doc->GetName()+" ("+RString::Number(doc->GetId())+")",true);

	// Prepare the analyze
	NbMemoryTokensUsed=0;
	NbMemoryOccursUsed=0;
	CurSyntacticPos=0;
	Doc=doc;
	Lang=doc->GetLang();
	if((!Tokenizer)||(!Analyzers.GetNb()))
		AssignPlugIns();
	Description.Clear();
	Tree.Clear();
	OrderTokens.Clear();
	Tokens.Clear();
	Occurs.Clear();
	Top.Clear();
	Depths.Clear();
	DepthError = false;

	// Find the filter for this document
	R::RSmartTempFile TmpFile;
	RURI File;
	GFilter* Filter(GALILEIApp->FindMIMEType(doc));

	// If it is not a local	file -> Download it
	RURI URI(doc->GetName());
	if(URI.GetScheme()!="file")
	{
		File=TmpFile.GetName();
		Download(URI,File);
	}
	else
		File=URI;

	// Analyze it with the good filter
	Filter->Analyze(this,doc,File);

	// Treat the tokens by the different analyzers.
	for(Analyzers.Start();!Analyzers.End();Analyzers.Next())
		Analyzers()->TreatTokens(this);

	// Build the representations and set them for the document
	BuildTensor();
	if(Session->DoCreateTree(pDoc))
	{
		// Build only the tree if necessary
		RCursor<GTokenOccur> Occur(Top);
		for(Occur.Start();!Occur.End();Occur.Next())
			BuildTree(0,Occur());

		if(Debug)
			Tree.Print();
	}
	doc->Update(Lang,Description,Tree);
}


//------------------------------------------------------------------------------
GDocAnalyze::~GDocAnalyze(void)
{
}
