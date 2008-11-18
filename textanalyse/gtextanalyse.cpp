/*

	GALILEI Research Project

	GTextAnalyse.cpp

	Analyze a document - Implementation.

	Copyright 2001-2008 by the Université libre de Bruxelles.

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
#include <gtextanalyse.h>
#include <gdoc.h>
#include <gdocxml.h>
#include <gconcept.h>
#include <gxmlindex.h>
#include <gconcepttype.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gstorage.h>
using namespace R;
using namespace GALILEI;
using namespace std;



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
	return(RCursor<cWord>(Words,0,NbWords));
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
		GConcept t(cNoRef,token,space,0,0,0,0);
		ptr->Tag=space->InsertConcept(&t);
	}
	return(ptr);
}


//-----------------------------------------------------------------------------
inline RCursor<cStructToken> cStructTokens::GetStructTokens(void) const
{
	return(RCursor<cStructToken>(*this));
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
cDepth::cDepth(char level) : RContainer<cNode,false,false>(200,100), NbNodes(0), CurTag(0), Level(level), NbNodesSkipped(0)
{
}


//-----------------------------------------------------------------------------
void cDepth::Clear(void)
{
	RContainer<cNode,false,false>::Clear();
	NbNodes=0;
	CurTag=0;
	NbNodesSkipped=0;
}


//-----------------------------------------------------------------------------
inline RCursor<cNode> cDepth::GetNodes(void) const
{
	return(RCursor<cNode>(*this,0,NbNodes));
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
	return(RCursor<cDepth>(*this,0,NbDepths));
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
	return(RCursor<cNode>(Nodes,0,NbNodes));
}



//-----------------------------------------------------------------------------
//
// class GTextAnalyse
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GTextAnalyse::GTextAnalyse(GFactoryDocAnalyse* fac)
	: GDocAnalyse(fac), RXMLParser(), Words(), StructTokens(/*100*/), Depths(),
	  Sl(0), Sldiff(0), StructSpace(0), IndexSpace(0), IsTitle(false)
{
	SetInvalidXMLCodes(true);
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ApplyConfig(void)
{
	StaticLang=Factory->GetBool("StaticLang");
	MinStopWords=Factory->GetDouble("MinStopWords");
	MinWordSize=Factory->GetUInt("MinWordSize");
	MinStemSize=Factory->GetUInt("MinStemSize");
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
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Connect(GSession* session)
{
	// First initialization
	if(Session) return;
	GDocAnalyse::Connect(session);

	// Create local structures
	CurLangs=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns();
	NbLangs=GALILEIApp->GetManager<GLangManager>("Lang")->GetNbPlugIns();
	Words.Init(NbLangs);
	Sl=new size_t[NbLangs];
	Sldiff=new size_t[NbLangs];

	// Get the pointers to the concept types
	StructSpace=Session->GetInsertConceptType("XMLStruct","XML Structure");
	IndexSpace=Session->GetInsertConceptType("XMLIndex","XML Index");
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Disconnect(GSession* session)
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
		GDocAnalyse::Disconnect(session);
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Clear(void)
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

}


//-----------------------------------------------------------------------------
void GTextAnalyse::BeginTag(const RString& namespaceURI,const RString& lName,const RString&)
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
				tmpStr=URI+":"+lName;
		}
		else
			tmpStr=namespaceURI+":"+lName;
	}
	else
		tmpStr=lName;
	if(tmpStr=="http://purl.org/dc/elements/1.1/:title")
		IsTitle=true;

	// Stopped the analyze if not necessary anymore
	if((!MustFullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;

	// Create the tag
	//cout<<"Index: *"<<tmpStr<<"*"<<endl;
	// Get the node of the previous depth
	cDepth* Depth=Depths.GetDepth(CurDepth);
	cNode* CurNode=Depths.GetNewNode(Depth,true);
	cStructToken* tag=StructTokens.AddToken(StructSpace,tmpStr);
	CurNode->SetTag(tag,LastTokenPos,Depth);

	// Make the sibling if depth>0
	if(CurDepth)
	{
		cNode* parent=Depths.GetDepth(CurDepth-1)->GetCurTag();
		if(parent&&parent->Obj.Tag)
		{
//			cout<<"Sibling between '"<<CurNode->Obj.Tag->Name<<"' and '"<<parent->Obj.Tag->Name<<"'"<<endl;
			if(parent->Child==SIZE_MAX)
				parent->Child=Depth->GetNbNodes()-1;
			parent->Nb++;
		}
	}
}


//------------------------------------------------------------------------------
void GTextAnalyse::AddAttribute(const RString& namespaceURI,const RString& lName, const RString&)
{
	if(!ExtractStruct) return;

	// If attributes names are content -> add short parameter name
	if(StructIsContent)
		ExtractValidWords(lName,WeightStruct,GVTDRec::Attribute);

	// Stopped the analyze if not necessary anymore
	if((!MustFullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;

	// Compute the name of the tag
	if(GetSection()==RXMLParser::Body)
	{
		if(namespaceURI.IsEmpty())
		{
			if(UseDefaultNamespace)
				tmpStr=DefaultNamespace+":"+lName;
			else
				tmpStr=URI+":"+lName;
		}
		else
			tmpStr=namespaceURI+":"+lName;
	}
	else
		tmpStr=lName;

	cDepth* Depth=Depths.GetDepth(CurDepth);
	cNode* CurNode=Depths.GetNewNode(Depth,false);
	cStructToken* tag=StructTokens.AddToken(StructSpace,tmpStr);
	CurNode->SetAttr(tag,LastTokenPos,Depth);
}


//------------------------------------------------------------------------------
void GTextAnalyse::Value(const RString& text)
{
	if(ExtractValues)
		ExtractValidWords(text,WeightValues,GVTDRec::Value);
}


//-----------------------------------------------------------------------------
void GTextAnalyse::EndTag(const RString&, const RString& /*lName*/, const RString&)
{
	// Stopped the analyze if not necessary anymore
/*	if((!MustFullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;*/
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Text(const RString& text)
{
	double w;
	if(IsTitle)
		w=2.0;
	else
		w=1.0;
	ExtractValidWords(text,w,GVTDRec::Content);
}


//------------------------------------------------------------------------------
RChar GTextAnalyse::CodeToChar(RString& str)
{
	RChar res=RXMLParser::CodeToChar(str);
	if(res!=0)
		return(res);
	return(' ');
}


//-----------------------------------------------------------------------------
void GTextAnalyse::AddWord(const RString& word,double weight,bool letters,GVTDRec::RecType type)
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
	if((!MustFullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;

	// Create a new node
	cDepth* Depth=Depths.GetDepth(CurDepth);
	cNode* node=Depths.GetNewNode(Depth,false);
	if(type==GVTDRec::Value)
		node->SetAttrValue(w,LastTokenPos,Depth);
	else if(type==GVTDRec::Content)
		node->SetContent(w,LastTokenPos,Depth);
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ExtractValidWords(const R::RString& str,double weight,GVTDRec::RecType type)
{
//	cout<<"Extract ("<<LastTokenPos<<"): *"<<str<<"*"<<endl;
	for(const RChar* ptr=str();!ptr->IsNull();)
	{
		// Skip Spaces
		while((!ptr->IsNull())&&(ptr->IsSpace()))
			ptr++;
		if(ptr->IsNull())
			return;

		// If only letters word -> skip non letters
		if(!NonLetterWords)
		{
			while((!ptr->IsNull())&&(!ptr->IsAlpha()))
				ptr++;

			if(ptr->IsNull())
				return;
		}

		// Always skip ' or "
		while((!ptr->IsNull())&&(((*ptr)=='\'')||((*ptr)=='"')))
			ptr++;
		if(ptr->IsNull())
			return;

		// Start to read characters and suppose there no non-alpha characters
		size_t NoAlpha(0);
		const RChar* begin(ptr);
		size_t len(0);
		size_t MaxSame(0);  // Maximal number of identical characters in a row
		size_t ActSame(0);  // How many identical characters are actually in a row
		RChar Last(0);      // Reference character

		// Parse until :
		// a) A ' or a " was found
		// b) A non letter character was found and only letters are aloud
		while((!ptr->IsNull())&&((*ptr)!='\'')&&((*ptr)!='"')&&((!NonLetterWords&&ptr->IsAlpha())||(NonLetterWords)))
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
		if(ActSame>MaxSame)
			MaxSame=ActSame;

		// Verify that the string is not empty
		if(!len)
			continue;

		// If the last characters added are punctuation then remove them
		bool RemovePunct=false;
		const RChar* LastPtr=ptr-1;
		size_t NbSkip=0;
		while(len&&(((*LastPtr)=='?')||((*LastPtr)=='!')||((*LastPtr)==',')||((*LastPtr)==';')||((*LastPtr)=='.')||((*LastPtr)==':')))
		{
			len--;
			LastPtr--;
			NoAlpha--;
			RemovePunct=true;
			NbSkip++;
		}

		// Verify that the string is not empty
		if(!len)
			continue;

		// If Filtering and remove multiple ending punctuation -> recompute MaxSame
		if(RemovePunct&&Filtering&&(NbSkip==MaxSame))
		{
			// Must verify again the whole string
			size_t i;
			for(LastPtr=begin,i=len+1,MaxSame=0,ActSame=0,Last=0;--i;LastPtr++)
			{
				if(Last==(*LastPtr))
					ActSame++;
				else
				{
					Last=(*LastPtr);
					if(ActSame>MaxSame)
						MaxSame=ActSame;
					ActSame=1;
				}
			}
			if(ActSame>MaxSame)
				MaxSame=ActSame;
		}

		// Simply verify if there are not too much same characters in a row
		if(Filtering&&(MaxSame>NbSameOccur))
			continue;

		// Verify that the string is not empty and (if necessary) if there are enough letters in the word
		if((!len)||(Filtering&&((double)(len-NoAlpha)/(double)(len)<NormalRatio)))
			continue;

		// Add the word.
		RString word(begin,len);
		//cout<<"  Index: *"<<word.ToLower()<<"*  ("<<(NoAlpha==0)<<") ="<<(double)(len-NoAlpha)/(double)(len)<<endl;
		AddWord(word.ToLower(),weight,(NoAlpha==0),type);
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyse::DetermineLang(void)
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
GConcept* GTextAnalyse::GetStemConcept(cWord* word)
{
	RString stem(MaxWordLen);

	// If Stop list -> do not treat it.
	if(word->InStop[LangIndex])
		return(0);

	// if len<MinWordSize -> do not treat it.
	if(word->Word.GetLen()<MinWordSize)
		return(0);

	// If only letters -> apply stemming algorithm.
	if(word->OnlyLetters)
		stem=Lang->GetStemming(word->Word);
	else
		stem=word->Word;
	if(stem.GetLen()<MinStemSize)
		return(0);
	GConcept w(cNoRef,stem,Lang->GetDict(),0,0,0,0);
	return(Lang->GetDict()->InsertConcept(&w));
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ConstructInfos(void)
{
	GWeightInfo* Occur;
	RString stem(MaxWordLen);

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

		Occur=Infos.GetInsertPtr(Word()->Concept);
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
void GTextAnalyse::IndexXMLPart(void)
{
	if(!ExtractStruct) return;

	// Index first the tags
	RCursor<cStructToken> Tags(StructTokens.GetStructTokens());
	for(Tags.Start();!Tags.End();Tags.Next())
	{
		if(!Tags()->GetOccurs())
			continue;
		GWeightInfo* info=Infos.GetInsertPtr(Tags()->GetTag());
		(*info)+=1.0;
	}
	if((!ExtractIndex)&&(!MustFullIndex)) return;

	// Index the metadata and the structure
	Struct.SetSizes(Depths.GetNbNodes(),Depths.GetNbDepths());
	GConceptType* LangSpace=Session->GetInsertConceptType(Lang->GetCode()+RString("Stems"),"");
	GXMLIndex w(IndexSpace,LangSpace);
	size_t max(SIZE_MAX),nb;
	bool IsTag;
	if(!ExtractStruct)
		max=MaxDepth;

	// Parse the nodes
	RCursor<cNode> Node(Depths.GetNodes());
	for(Node.Start(),IsTag=false;!Node.End();Node.Next())
	{
		cNode* ptr=Node();
		switch(ptr->Type)
		{
			case GVTDRec::Tag:
			{
				//cout<<"  <"<<ptr->Obj.Tag->GetName()<<">: "<<ptr->Child<<"("<<ptr->Nb<<")"<<endl;
				if(MustFullIndex)
				{
					if(ptr->Child!=SIZE_MAX)
						ptr->Child-=Depths.GetDepth(ptr->Depth->Level+1)->NbNodesSkipped;
					Struct.AddRecord(ptr->Obj.Tag->GetTag(),GVTDRec::Tag,ptr->Pos,ptr->Depth->Level,ptr->Child,ptr->Depth->GetNbNodes());
				}

				// Already a tag indexed
				if(IsTag&&nb&&nb<=MaxTerms)
				{
					GWeightInfo* info=Infos.GetInsertPtr(IndexSpace->InsertConcept(&w));
					(*info)+=1.0;
					IsTag=false;
				}

				// Index this tag
				//      Maximal depth                          // Child Tags or not                                       // Max occurrences
				if( (static_cast<size_t>(ptr->Depth->Level)<=MaxDepth)&&  (((!ChildTags)&&(!ptr->Nb))||(ChildTags)) && (ptr->Obj.Tag->GetOccurs()) && (ptr->Obj.Tag->GetOccurs()<=MaxOccurs)      )
				{
					//cout<<Node()->Pos<<": "<<Node()->Obj.Tag->Name<<": "<<Node()->Child<<endl;
					nb=0;
					w.SetTag(ptr->Obj.Tag->GetTag());
					IsTag=true;
				}
				break;
			}
			case GVTDRec::Attribute:
			{
				if(MustFullIndex)
					Struct.AddRecord(ptr->Obj.Tag->GetTag(),GVTDRec::Attribute,ptr->Pos,ptr->Depth->Level);
//					cout<<"  "<<ptr->Obj.Tag->GetName()<<endl;
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
//					cout<<"    +="<<ptr->Obj.Word->Word<<endl;
				break;
			}
			case GVTDRec::Content:
			{
				if(MustFullIndex&&ptr->Obj.Word->Concept)
					Struct.AddRecord(ptr->Obj.Word->Concept,GVTDRec::Content,ptr->Pos,ptr->Depth->Level);
//					cout<<"    "<<ptr->Obj.Word->Word<<endl;
				if(IsTag)
				{
					if(ptr->Obj.Word->Concept)
					{
						nb++;
						if(nb>MaxTerms)
							IsTag=false;
						else
							w.AddStem(ptr->Obj.Word->Concept);
					}
					else
						ptr->Depth->NbNodesSkipped++;
				}
				break;
			}
		}
	}

	// A tag is still indexed
	if(IsTag&&nb&&nb<=MaxTerms)
	{
		GWeightInfo* info=Infos.GetInsertPtr(IndexSpace->InsertConcept(&w));
		(*info)+=1.0;
		IsTag=false;
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Analyze(const GDoc* doc,const R::RURI& file,bool native)
{
	RString Name;
	RCursor<RXMLTag> Tags;

	// Init part
	if(FullIndex&&native)
		MustFullIndex=true;
	else
		MustFullIndex=false;
	Lang=doc->GetLang();
	Clear();

	// If the language must not be found -> Compute LangIndex
	if(!FindLang)
		for(CurLangs.Start(),LangIndex=0;CurLangs()!=Lang;CurLangs.Next(),LangIndex++) ;

	// Load the xml from the file
//	cout<<"Analyse "<<uri<<endl;
	Open(file,RIO::Read,"UTF-8");
	Close();

	// // Determine the Language if necessary. If the language cannot be found -> document is not indexed.
	if(FindLang)
		DetermineLang();
	if(!Lang)
		return;

	// Construct Information from the stop words extracted and the XML
	ConstructInfos();
	IndexXMLPart();
/*	if(Struct)
	{
		Struct->Print();
		cout<<"-----"<<endl;
	}*/
}


//------------------------------------------------------------------------------
void GTextAnalyse::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("StaticLang",false));
	params->InsertParam(new RParamValue("MinStopWords",0.09));
	params->InsertParam(new RParamValue("MinWordSize",3));
	params->InsertParam(new RParamValue("MinStemSize",3));
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
}


//-----------------------------------------------------------------------------
GTextAnalyse::~GTextAnalyse(void)
{
	Disconnect(0);
}


//------------------------------------------------------------------------------
CREATE_DOCANALYSE_FACTORY("Text Analyse",GTextAnalyse)
