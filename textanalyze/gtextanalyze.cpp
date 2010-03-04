/*

	GALILEI Research Project

	GTextAnalyze.cpp

	Analyze a document - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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
#include <gtextanalyze.h>
#include <gdoc.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gstorage.h>



//-----------------------------------------------------------------------------
//
// class cWords
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
cWords::cWords(void)
	: Words(5000,2500), Hash(27,27,500,250), NbWords(0), NbLangs(0)
{
}


//-----------------------------------------------------------------------------
void cWords::Init(size_t nblangs)
{
	if(!NbLangs)
	{
		NbLangs=nblangs;
		for(size_t i=5001;--i;)
			Words.InsertPtr(new cWord(NbLangs));
	}
	else
	{
		if(NbLangs>=nblangs)
			return;
		Words.Clear();
		NbLangs=nblangs;
		for(size_t i=5001;--i;)
			Words.InsertPtr(new cWord(NbLangs));
	}
}


//-----------------------------------------------------------------------------
void cWords::Clear(void)
{
	RCursor<cWord> Cur(GetWords());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Hash.Clear();
	NbWords=0;
}


//-----------------------------------------------------------------------------
inline RCursor<cWord> cWords::GetWords(void) const
{
	return(RCursor<cWord>(Words,0,NbWords-1));
}


//-----------------------------------------------------------------------------
inline cWord* cWords::GetWord(const RString& word,bool& find)
{
	cWord* ptr;

	// Find the section of double hash table concerned by the current word.
	RContainer<cWord,false,true>* Section((*(Hash)[word.HashIndex(1)])[word.HashIndex(2)]);
	// Find the index where the word is or must be.
	size_t Index=Section->GetIndex<const RString>(word,find);
	if(find)
		ptr=(*Section)[Index];
	else
	{
		if(NbWords==Words.GetNb())
			Words.InsertPtr(ptr=new cWord(NbLangs));
		else
			ptr=Words[NbWords];
		NbWords++;
		Section->InsertPtrAt(ptr,Index);
	}
	return(ptr);
}



//-----------------------------------------------------------------------------
//
// class cStructTokens
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
cStructTokens::cStructTokens(void)
	: RContainer<cStructToken,true,true>(1000,500), NbTags(0)
{
}


//-----------------------------------------------------------------------------
void cStructTokens::Clear(void)
{
	RCursor<cStructToken> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	NbTags=0;
}


//-----------------------------------------------------------------------------
inline cStructToken* cStructTokens::AddToken(GConceptType* space,const RString& token)
{
	cStructToken* ptr(GetInsertPtr(token));
	ptr->Occurs++;
	NbTags++;
	if(!ptr->Tag)
	{
		GConcept t(token,space);
		ptr->Tag=GALILEIApp->GetSession()->InsertConcept(&t);
	}
	return(ptr);
}


//-----------------------------------------------------------------------------
inline void cStructTokens::ChangeTokenNS(cStructToken* token,const RString& uri)
{
	int i=token->GetName().Find(':',-1);
	RString Name(uri+token->GetName().Mid(i));

	if(Debug)
		cout<<"Change "<<token->GetName()<<" with "<<Name<<endl;
	if(token->Occurs>1)
	{
		cerr<<"cStructTokens::ChangeTokenNS : Create a new token not implemented"<<endl;
		//token->Occurs--;
	}
	else
	{
		// Rename token
		if(Debug)
			cout<<"Rename token"<<endl;
		DeletePtr(*token,true,false);
		token->Name=Name;
		InsertPtr(token);
		GConcept* tag=token->Tag;
		if(tag->GetRef(otAnyClass))
		{
			// New concept must be created.
			GConcept t(Name,tag->GetType());
			token->Tag=GALILEIApp->GetSession()->InsertConcept(&t);
		}
		else
		{
			// Concept must be renamed
			if(Debug)
				cout<<"Rename concept"<<endl;
			token->Tag=GALILEIApp->GetSession()->RenameConcept(tag,Name);
		}
	}
}


//-----------------------------------------------------------------------------
inline RCursor<cStructToken> cStructTokens::GetStructTokens(void) const
{
	return(RCursor<cStructToken>(*this));
}


//-----------------------------------------------------------------------------
void cStructTokens::Test(void)
{
/*	RCursor<cStructToken> Cur(GetStructTokens());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		cStructToken* ptr(Cur());
		if(!ptr->GetTag())
			continue;
		cout<<"("<<(int)ptr->GetTag()->GetType()->GetId()<<","<<ptr->GetTag()->GetId()<<")\t";
		if(ptr->GetTag()->GetId()>30000)
			cout<<endl<<"Problem";
	}
	cout<<endl;*/
}



//-----------------------------------------------------------------------------
//
// class cNode
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
cNode::cNode(void)
{
}


//-----------------------------------------------------------------------------
void cNode::SetTag(cStructToken* tag,size_t pos,cDepth* depth)
{
	if(!depth)
		throw GException("GTextAnalyze::cNode::SetTag(cStructToken*,size_t,cDepth*) : depth is null");
	Obj.Tag=tag;
	Type=GVTDRec::Tag;
	Pos=pos;
	Depth=depth;
	Child=SIZE_MAX;
	Nb=0;
}


//-----------------------------------------------------------------------------
void cNode::SetAttr(cStructToken* tag,size_t pos,cDepth* depth)
{
	if(!depth)
		throw GException("GTextAnalyze::cNode::SetAttr(cStructToken*,size_t,cDepth*) : depth is null");
	Obj.Tag=tag;
	Type=GVTDRec::Attribute;
	Pos=pos;
	Depth=depth;
	Child=SIZE_MAX;
	Nb=0;
}


//-----------------------------------------------------------------------------
void cNode::SetAttrValue(cWord* word,size_t pos,cDepth* depth)
{
	if(!depth)
		throw GException("GTextAnalyze::cNode::SetAttrValue(cStructToken*,size_t,cDepth*) : depth is null");
	Obj.Word=word;
	Type=GVTDRec::Value;
	Pos=pos;
	Depth=depth;
	Child=SIZE_MAX;
	Nb=0;
}


//-----------------------------------------------------------------------------
void cNode::SetContent(cWord* word,size_t pos,cDepth* depth)
{
	if(!depth)
		throw GException("GTextAnalyze::cNode::SetContent(cStructToken*,size_t,cDepth*) : depth is null");
	Obj.Word=word;
	Type=GVTDRec::Content;
	Pos=pos;
	Depth=depth;
	Child=SIZE_MAX;
	Nb=0;
}


//-----------------------------------------------------------------------------
void cNode::Clear(void)
{
	Obj.Word=0;
	Pos=0;
	Depth=0;
	Child=SIZE_MAX;
	Nb=0;
}



//-----------------------------------------------------------------------------
//
// class cDepth
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
cDepth::cDepth(char level)
	: RContainer<cNode,false,false>(200,100), NbNodes(0), CurTag(0),
	  Level(level), NbNodesSkipped(0), CurIndex(0), NbCurIndex(0)
{
}


//-----------------------------------------------------------------------------
void cDepth::Clear(void)
{
	RContainer<cNode,false,false>::Clear();
	NbNodes=0;
	CurTag=0;
	NbNodesSkipped=0;
	delete CurIndex;
	CurIndex=0;
	NbCurIndex=0;
}


//-----------------------------------------------------------------------------
inline RCursor<cNode> cDepth::GetNodes(void) const
{
	return(RCursor<cNode>(*this,0,NbNodes-1));
}



//-----------------------------------------------------------------------------
//
// class cDepths
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
cDepths::cDepths(void) : RContainer<cDepth,true,false>(20,5), NbDepths(0), Nodes(2000), NbNodes(0)
{
	for(char i=0;i<20;i++)
		InsertPtr(new cDepth(i));
	for(size_t i=2001;--i;)
		Nodes.InsertPtr(new cNode());
}


//-----------------------------------------------------------------------------
void cDepths::Clear(void)
{
	RCursor<cDepth> Cur(GetDepths());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	NbDepths=0;
	RCursor<cNode> Node(GetNodes());
	for(Node.Start();!Node.End();Node.Next())
		Node()->Clear();
	NbNodes=0;
}


//-----------------------------------------------------------------------------
inline cDepth* cDepths::GetDepth(char depth)
{
	cDepth* ptr;
	if(static_cast<size_t>(depth)==GetNb())
		InsertPtr(ptr=new cDepth(depth));
	else
		ptr=(*this)[depth];
	if(static_cast<size_t>(depth+1)>NbDepths)
		NbDepths=depth+1;
	return(ptr);
}


//-----------------------------------------------------------------------------
inline RCursor<cDepth> cDepths::GetDepths(void) const
{
	return(RCursor<cDepth>(*this,0,NbDepths-1));
}


//-----------------------------------------------------------------------------
inline cNode* cDepths::GetNewNode(cDepth* depth,bool tag)
{
	cNode* ptr;
	if(NbNodes==Nodes.GetNb())
		Nodes.InsertPtr(ptr=new cNode());
	else
		ptr=Nodes[NbNodes];
	NbNodes++;
	depth->InsertPtr(ptr);
	depth->NbNodes++;
	if(tag)
		depth->CurTag=ptr;
	return(ptr);
}


//-----------------------------------------------------------------------------
inline RCursor<cNode> cDepths::GetNodes(void) const
{
	return(RCursor<cNode>(Nodes,0,NbNodes-1));
}



//-----------------------------------------------------------------------------
//
// class GTextAnalyze
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GTextAnalyze::GTextAnalyze(GPlugInFactory* fac)
	: GDocAnalyze(fac), Words(), StructTokens(/*100*/), Depths(),
	  Sl(0), Sldiff(0), MetaTags(20), StructSpace(0), IndexSpace(0), IsTitle(false),
	  IsIdentifier(false)
{
}


//-----------------------------------------------------------------------------
void GTextAnalyze::ApplyConfig(void)
{
	StaticLang=Factory->GetBool("StaticLang");
	MinStopWords=Factory->GetDouble("MinStopWords");
	MinWordSize=Factory->GetUInt("MinWordSize");
	NonLetterWords=Factory->GetBool("NonLetterWords");
	Filtering=Factory->GetBool("Filtering");
	NbSameOccur=Factory->GetUInt("NbSameOccur");
	NormalRatio=Factory->GetDouble("NormalRatio");
	ExtractStruct=Factory->GetBool("ExtractStruct");
	StructIsContent=Factory->GetBool("StructIsContent");
	ExtractIndex=Factory->GetBool("ExtractIndex");
	MaxTerms=Factory->GetUInt("MaxTerms");
	MaxOccurs=Factory->GetUInt("MaxOccurs");
	MaxDepth=static_cast<char>(Factory->GetUInt("MaxDepth"));
	ChildTags=Factory->GetBool("ChildTags");
	WeightStruct=Factory->GetDouble("WeightStruct");
	ExtractValues=Factory->GetBool("ExtractValues");
	WeightValues=Factory->GetDouble("WeightValues");
	FullIndex=Factory->GetBool("FullIndex");
	UseDefaultNamespace=Factory->GetBool("UseDefaultNamespace");
	DefaultNamespace=Factory->Get("DefaultNamespace");
	DetectMetaTag=Factory->GetBool("DetectMetaTag");
	MetaTagFile=Factory->Get("MetaTagFile");
	MetaTags.Clear();
	if(!DetectMetaTag)
	{
		try
		{
			RTextFile Tags(MetaTagFile);
			Tags.Open(RIO::Read);
			while(!Tags.End())
				MetaTags.InsertPtr(new RString(Tags.GetLine()));
		}
		catch(RException& e)
		{
			cerr<<e.GetMsg()<<endl;
		}
		catch(...)
		{
			cerr<<"Unknown error"<<endl;
		}
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyze::Connect(GSession* session)
{
	// First initialization
	if(Session) return;
	GDocAnalyze::Connect(session);

	// Create local structures
	CurLangs=GALILEIApp->GetPlugIns<GLang>("Lang");
	NbLangs=GALILEIApp->GetManager("Lang")->GetNbPlugIns();
	Words.Init(NbLangs);
	Sl=new size_t[NbLangs];
	Sldiff=new size_t[NbLangs];

	// Get the pointers to the concept types
	StructSpace=Session->GetConceptType("XMLStruct",true);
	if(!StructSpace)
		throw GException("No XML Structure types");
	IndexSpace=Session->GetConceptType("XMLIndex",true);
	if(!IndexSpace)
		throw GException("No XML Index types");

}


//-----------------------------------------------------------------------------
void GTextAnalyze::Disconnect(GSession* session)
{
	// Spaces are null
	StructSpace=0;
	IndexSpace=0;

	// Local Structures
	if(Sldiff)
	{
		delete[] Sldiff;
		Sldiff=0;
	}
	if(Sl)
	{
		delete[] Sl;
		Sl=0;
	}

	if(Session)
		GDocAnalyze::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GTextAnalyze::Clear(void)
{
	// Language information
	FindLang=((!Lang)||(!StaticLang));

	// Clear all structures needed to handle a document
	memset(Sl,0,sizeof(size_t)*CurLangs.GetNb());
	memset(Sldiff,0,sizeof(size_t)*CurLangs.GetNb());
	N=Nwords=V=Vdiff=S=Sdiff=0;
	Words.Clear();
	Depths.Clear();
	StructTokens.Clear();
	Infos.Clear();
	Struct.Clear();
	IsTitle=false;
	IsIdentifier=false;
	LastInsertTag=0;
}


//-----------------------------------------------------------------------------
void GTextAnalyze::BeginTag(const RString& namespaceURI,const RString& lName,const RString&)
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
void GTextAnalyze::ResolveNamespace(const RString& namespaceURI)
{
	// Stopped the analyze if not necessary anymore
	if(StopAnalyseTag())
		return;
	if(LastInsertTag)
		StructTokens.ChangeTokenNS(LastInsertTag,namespaceURI);
}


//------------------------------------------------------------------------------
void GTextAnalyze::AddAttribute(const RString& namespaceURI,const RString& lName, const RString&)
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
void GTextAnalyze::Value(const RString& text)
{
	if(ExtractValues)
	{
		ExtractValidWords(text,WeightValues,GVTDRec::Value);
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyze::EndTag(const RString&, const RString& /*lName*/, const RString&)
{
	// Stopped the analyze if not necessary anymore
/*	if((!MustFullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;*/
/*	if(CurDepth)
		CurNode=Depths.GetDepth(CurDepth-1)->GetCurTag();*/
}


//-----------------------------------------------------------------------------
void GTextAnalyze::Text(const RString& text)
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
RChar GTextAnalyze::CodeToChar(RString& str)
{
	RChar res=RXMLParser::CodeToChar(str);
	if(res!=0)
		return(res);
	return(' ');
}


//-----------------------------------------------------------------------------
void GTextAnalyze::AddWord(const RString& word,double weight,bool letters,GVTDRec::RecType type)
{
	bool Find;
	size_t i;
	bool *is;
	size_t* tmp1;
	size_t* tmp2;

	// Find the word
	cWord* w(Words.GetWord(word,Find));

	// If the word wasn't found, insert it and look for each language if it's
	// in the corresponding stoplist.
	if(!Find)
	{
		// Create the word and insert it in the Occurs.
		w->OnlyLetters=letters;
		w->Word=word;

		// Look for each language if the word is in the stop list.
		if(FindLang)
		{
			for(CurLangs.Start(),is=w->InStop,tmp1=Sldiff,tmp2=Sl;!CurLangs.End();CurLangs.Next(),is++,tmp1++,tmp2++)
			{
				(*is)=CurLangs()->InStop(word);
				if(*is)
				{
					// In the stoplist -> Inc different words of the stop lists.
					(*tmp1)++;
					(*tmp2)++;
				}
			}
		}
		else
		{
			if(Lang->InStop(word))
			{
				w->InStop[LangIndex]=true;
				Sl[LangIndex]++;
				Sldiff[LangIndex]++;
			}
			else
				w->InStop[LangIndex]=false;
		}
	}
	else
	{
		if(FindLang)
		{
			for(i=NbLangs+1,is=w->InStop,tmp2=Sl;--i;is++,tmp2++)
			{
				if(*is)
					(*tmp2)++;
			}
		}
		else
		{
			if(w->InStop[LangIndex])
				Sl[LangIndex]++;
		}
	}

	// Treat the correctly word
	N++;
	w->Nb++;
	w->Weight+=weight;
	w->NormalStem=true;

	// Stopped the analyze if not necessary anymore
	if(StopAnalyseTag())
		return;

	// Create a new node
	cDepth* Depth=Depths.GetDepth(GetCurrentDepth());
	cNode* node=Depths.GetNewNode(Depth,false);
	if(type==GVTDRec::Value)
		node->SetAttrValue(w,GetLastTokenPos(),Depth);
	else if(type==GVTDRec::Content)
		node->SetContent(w,GetLastTokenPos(),Depth);
}


//-----------------------------------------------------------------------------
inline bool SepChar(const RChar* ptr)
{
	return(
			(ptr->IsSpace())||
			((*ptr)=='\'')||((*ptr)=='"')||
			((*ptr)=='/')||((*ptr)=='\\')||((*ptr)=='-')||((*ptr)=='+')||((*ptr)=='_')||
			((*ptr)=='.')||((*ptr)==';')||((*ptr)==',')||((*ptr)==':')||((*ptr)=='!')||((*ptr)=='?')||((*ptr)=='~')||
			((*ptr)=='(')||((*ptr)==')')||((*ptr)=='[')||((*ptr)==']')||((*ptr)=='{')||((*ptr)=='}')
		  );
}


//-----------------------------------------------------------------------------
void GTextAnalyze::ExtractValidWords(const R::RString& str,double weight,GVTDRec::RecType type)
{
	if(Debug)
		cout<<"Extract ("<<GetLastTokenPos()<<"): *"<<str<<"*"<<endl;
	for(const RChar* ptr=str();!ptr->IsNull();)
	{

		// Skip Spaces and leading not alpha characters if Non letters words are not extracted
		for(bool Cont=true;Cont;)
		{
			Cont=false;
			while(SepChar(ptr))
				ptr++;
			if(ptr->IsNull())
				return;
			if(!NonLetterWords)
			{
				const RChar* tmp(ptr);
				while((!ptr->IsNull())&&(!ptr->IsAlpha()))
					ptr++;
				Cont=(ptr!=tmp);
			}
		}

		// Start to read characters and suppose there no non-alpha characters
		const RChar* begin(ptr);

		// Parse until :
		// a) A ' or a " was found
		// b) A non letter character was found and only letters are aloud
		size_t NoAlpha(0);   // Number of non-alphabetic characters
		size_t len(0);       // Size of the word
		size_t MaxSame(0);  // Maximal number of identical characters in a row
		size_t ActSame(0);  // How many identical characters are actually in a row
		RChar Last(0);      // Reference character
		while((!ptr->IsNull())&&(!SepChar(ptr)))
		{
			if(Last==(*ptr))
				ActSame++;
			else
			{
				Last=(*ptr);
				if(ActSame>MaxSame)
					MaxSame=ActSame;
				ActSame=1;
			}
			if(!ptr->IsAlpha())
				NoAlpha++;
			ptr++;
			len++;
		}

		// Verify that the string matches the rules:
		if( (!NonLetterWords&&(NoAlpha>0)) ||    // 1. If letters-only -> Verify that no non-letters are present
			(NonLetterWords&&Filtering&&	     // 2. If non-letters allowed -> verify if the filtering is activate and valid
				(
						(MaxSame>NbSameOccur)||                             // Not too much same characters in a row
						((double)(len-NoAlpha)/(double)(len)<NormalRatio))) // Ratio must be respected
				)
			continue;

		// Add the word.
		RString word(begin,len);
		if(Debug)
			cout<<"  Index word: *"<<word.ToLower()<<"*  ("<<(NoAlpha==0)<<") ="<<(double)(len-NoAlpha)/(double)(len)<<endl;
		AddWord(word.ToLower(),weight,(NoAlpha==0),type);
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyze::DetermineLang(void)
{
	double Frac,MinFrac;
	size_t i;
	size_t* tmp1;
	size_t* tmp2;

	LangIndex=cNoRef;
	MinFrac=MinStopWords;
	Lang=0;

	if(!Words.GetNbWords())
		return;

	for(CurLangs.Start(),i=0,tmp1=Sldiff,tmp2=Sl;!CurLangs.End();CurLangs.Next(),tmp1++,tmp2++,i++)
	{
		Frac=((double)(*tmp1))/((double)Words.GetNbWords());
//		cout<<"  "<<CurLangs()->GetCode()<<" : "<<Frac<<endl;
		if(((*tmp2)>S)&&(Frac>=MinFrac))
		{
			Lang=CurLangs();
			S=(*tmp2);
			Sdiff=(*tmp1);
			LangIndex=i;
		}
	}
}


//-----------------------------------------------------------------------------
GConcept* GTextAnalyze::GetStemConcept(cWord* word)
{
	RString stem(MaxWordLen);

	// If Stop list -> do not treat it.
	if(word->InStop[LangIndex])
		return(0);

	// If only letters -> apply stemming algorithm.
	if(word->OnlyLetters)
		stem=Lang->GetStemming(word->Word);
	else
		stem=word->Word;
	if(stem.GetLen()<MinWordSize)
		return(0);
	GConcept w(stem,Lang->GetDict());
	return(Session->InsertConcept(&w));
}


//-----------------------------------------------------------------------------
void GTextAnalyze::ConstructInfos(void)
{
	// Insert all the occurrences of the valid words
	RCursor<cWord> Word(Words.GetWords());
	for(Word.Start();!Word.End();Word.Next())
	{
		if(!Word()->NormalStem)
			continue;

		if(!Word()->Concept)
		{
			Word()->Concept=GetStemConcept(Word());
			if(!Word()->Concept)
				continue;
		}

		GWeightInfo* Occur(Infos.GetInfo(Word()->Concept));
		if(!Occur->GetWeight())
		{
			Vdiff++;
//			cout<<Word()->Word<<endl;
		}
		V+=Word()->Nb;
		(*Occur)+=Word()->Weight;
	}
}



//-----------------------------------------------------------------------------
void GTextAnalyze::IndexXMLPart(void)
{
	if(!ExtractStruct) return;

	if(Debug)
		cout<<endl<<"IndexXMLPart"<<endl;

	// Index first the tags
	RCursor<cStructToken> Tags(StructTokens.GetStructTokens());
	for(Tags.Start();!Tags.End();Tags.Next())
	{
		if(!Tags()->GetOccurs())
			continue;
		GConcept* concept(Tags()->GetTag());
		GWeightInfo* info=Infos.GetInfo(concept);
		(*info)+=1.0;
	}
	if((!ExtractIndex)&&(!MustFullIndex)) return;

	// Index the metadata and the structure
	Struct.SetSizes(Depths.GetNbNodes(),Depths.GetNbDepths());
//	GXMLIndex w(IndexSpace,LangSpace);
	size_t max(SIZE_MAX)/*, nb(0)*/;
//	bool IsTag(false);   // Is there a current tag indexed ?
	cDepth* Depth(0);
	if(!ExtractStruct)
		max=MaxDepth;

	// Parse the nodes
	RCursor<cNode> Node(Depths.GetNodes());
	for(Node.Start();!Node.End();Node.Next())
	{
		cNode* ptr=Node();
		Depth=ptr->Depth;  //<-- Pal
		if(!Depth)
			continue;
		if(!Depth)
			ThrowGException("Invalid XML structure");
		if(!Depth->CurIndex)
			Depth->CurIndex=new GXMLIndex(IndexSpace,LangSpace);
		switch(ptr->Type)
		{
			case GVTDRec::Tag:
			{
				if(Debug)
					cout<<"Tag: <"<<ptr->Obj.Tag->GetName()<<">: "<<ptr->Pos<<" - "<<(int)ptr->Depth->Level<<" ("<<ptr->Child<<":"<<ptr->Nb<<")";

				if(MustFullIndex)
				{
					if(ptr->Child!=SIZE_MAX)
						ptr->Child-=Depths.GetDepth(ptr->Depth->Level+1)->NbNodesSkipped;
					Struct.AddRecord(ptr->Obj.Tag->GetTag(),GVTDRec::Tag,ptr->Pos,ptr->Depth->Level,ptr->Child,ptr->Depth->GetNbNodes());
				}

				// Already a tag indexed
				if(ptr->Depth->NbCurIndex)
				{
					GWeightInfo* info=Infos.GetInfo(Session->InsertConcept(ptr->Depth->CurIndex));
					(*info)+=1.0;
//					IsTag=false;
					ptr->Depth->CurIndex->Clear();
					ptr->Depth->CurIndex->SetConceptTypes(IndexSpace,LangSpace);
					ptr->Depth->NbCurIndex=0;
				}

				// Index this tag
				if(
					(
						(!DetectMetaTag)&&(MetaTags.IsIn(ptr->Obj.Tag->GetName()))       // No detection -> Look in list
					) ||
					(
						DetectMetaTag&&                                                            // Detection ->
						(ptr->Depth->Level<=MaxDepth)                                              // Maximal depth
						&&  (((!ChildTags)&&(!ptr->Nb))||(ChildTags))                              // Child Tags or not
						&& (ptr->Obj.Tag->GetOccurs()) && (ptr->Obj.Tag->GetOccurs()<=MaxOccurs)   // Max occurrences
					)
				)
				{
					if(Debug)
						cout<<" - Index";
					ptr->Depth->CurIndex->SetTag(ptr->Obj.Tag->GetTag());
					ptr->Depth->NbCurIndex=0;
//					IsTag=true;
				}
				if(Debug)
					cout<<endl;
				break;
			}
			case GVTDRec::Attribute:
			{
				if(MustFullIndex)
					Struct.AddRecord(ptr->Obj.Tag->GetTag(),GVTDRec::Attribute,ptr->Pos,ptr->Depth->Level);
					if(Debug)
						cout<<"\tAttribute: "<<ptr->Obj.Tag->GetName()<<endl;
				break;
			}
			case GVTDRec::Value:
			{
				if(MustFullIndex)
				{
					if(ptr->Obj.Word->Concept)
						Struct.AddRecord(ptr->Obj.Word->Concept,GVTDRec::Value,ptr->Pos,ptr->Depth->Level);
					else
						ptr->Depth->NbNodesSkipped++;
				}
				if(Debug)
					cout<<"\t\t+="<<ptr->Obj.Word->Word<<endl;
				break;
			}
			case GVTDRec::Content:
			{
				if(MustFullIndex&&ptr->Obj.Word->Concept)
					Struct.AddRecord(ptr->Obj.Word->Concept,GVTDRec::Content,ptr->Pos,ptr->Depth->Level);
				if(Debug)
				{
					if(ptr->Obj.Word->Concept)
					{
						cout<<"\tWord: "<<ptr->Obj.Word->Word;
						if(ptr->Depth->CurIndex->GetXMLTag())
							cout<<" - Add to XML index to "<<ptr->Depth->CurIndex->GetXMLTag()->GetName();
						cout<<endl;
					}
				}
				if(ptr->Depth->CurIndex->GetXMLTag())
				{
					if(ptr->Obj.Word->Concept)
					{
						ptr->Depth->NbCurIndex++;
						if(DetectMetaTag&&(ptr->Depth->NbCurIndex>MaxTerms))
							ptr->Depth->NbCurIndex=0;
						else
							Depth->CurIndex->AddStem(ptr->Obj.Word->Concept);
					}
					else
						Depth->NbNodesSkipped++;
				}
				break;
			}
		}
	}

	// Verify that they are not tags in the different depth
	RCursor<cDepth> Cur(Depths.GetDepths());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->NbCurIndex)
		{
			GWeightInfo* info=Infos.GetInfo(Session->InsertConcept(Cur()->CurIndex));
			(*info)+=1.0;
		}
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyze::PrepareAnalyze(const GDoc* doc,bool native)
{
	// Init part
	//cout<<doc->GetName()<<endl;
	Native=native;
	if(FullIndex&&native)
		MustFullIndex=true;
	else
		MustFullIndex=false;
	Lang=doc->GetLang();
	Clear();

	// If the language must not be found -> Compute LangIndex
	if(!FindLang)
		for(CurLangs.Start(),LangIndex=0;CurLangs()!=Lang;CurLangs.Next(),LangIndex++) ;
}


//-----------------------------------------------------------------------------
void GTextAnalyze::TerminateAnalyze(void)
{
	RString Name;
	RCursor<RXMLTag> Tags;

	// Determine the Language if necessary. If the language cannot be found -> document is not indexed.
	if(FindLang)
		DetermineLang();
	if(!Lang)
		return;

	// Construct Information from the stop words extracted and the XML
	LangSpace=Session->GetConceptType(Lang->GetCode()+RString("Terms"),true);
	if(!LangSpace)
		ThrowGException("TextAnalyse : Invalid language "+Lang->GetLangName());
	ConstructInfos();
	IndexXMLPart();
}


//------------------------------------------------------------------------------
void GTextAnalyze::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("StaticLang",false));
	params->InsertParam(new RParamValue("MinStopWords",0.09));
	params->InsertParam(new RParamValue("MinWordSize",3));
	params->InsertParam(new RParamValue("NonLetterWords",true));
	params->InsertParam(new RParamValue("Filtering",true));
	params->InsertParam(new RParamValue("NbSameOccur",3));
	params->InsertParam(new RParamValue("NormalRatio",0.3));
	params->InsertParam(new RParamValue("ExtractStruct",false));
	params->InsertParam(new RParamValue("StructIsContent",false));
	params->InsertParam(new RParamValue("ExtractIndex",false));
	params->InsertParam(new RParamValue("MaxTerms",10));
	params->InsertParam(new RParamValue("MaxDepth",2));
	params->InsertParam(new RParamValue("MaxOccurs",5));
	params->InsertParam(new RParamValue("ChildTags",false));
	params->InsertParam(new RParamValue("WeightStruct",2.0));
	params->InsertParam(new RParamValue("ExtractValues",false));
	params->InsertParam(new RParamValue("WeightValues",2.0));
	params->InsertParam(new RParamValue("FullIndex",false));
	params->InsertParam(new RParamValue("UseDefaultNamespace",false));
	params->InsertParam(new RParamValue("DefaultNamespace",""));
	params->InsertParam(new RParamValue("MetaTagFile",""));
	params->InsertParam(new RParamValue("DetectMetaTag",true));
}


//-----------------------------------------------------------------------------
GTextAnalyze::~GTextAnalyze(void)
{
	Disconnect(0);
}


//------------------------------------------------------------------------------
CREATE_DOCANALYZE_FACTORY("Text Analyze","Text Analyze",GTextAnalyze)
