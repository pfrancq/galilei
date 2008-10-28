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
// class cNode
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
cNode::cNode(void)
{
}


//-----------------------------------------------------------------------------
void cNode::SetTag(IndexTag* tag,size_t pos,char depth)
{
	Obj.Tag=tag;
	Type=VTR::Tag;
	Pos=pos;
	Depth=depth;
	Child=0;
	Nb=0;
}


//-----------------------------------------------------------------------------
void cNode::SetAttr(size_t pos,char depth)
{
	Obj.Tag=0;
	Type=VTR::Attribute;
	Pos=pos;
	Depth=depth;
	Child=0;
	Nb=0;
}


//-----------------------------------------------------------------------------
void cNode::SetAttrValue(cWord* word,size_t pos,char depth)
{
	Obj.Word=word;
	Type=VTR::Value;
	Pos=pos;
	Depth=depth;
	Child=0;
	Nb=0;
}


//-----------------------------------------------------------------------------
void cNode::SetContent(cWord* word,size_t pos,char depth)
{
	Obj.Word=word;
	Type=VTR::Content;
	Pos=pos;
	Depth=depth;
	Child=0;
	Nb=0;
}


//-----------------------------------------------------------------------------
void cNode::Clear(void)
{
	Obj.Word=0;
	Pos=0;
	Depth=0;
	Child=0;
	Nb=0;
}



//-----------------------------------------------------------------------------
//
// class cDepth
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
cDepth::cDepth(size_t level) : RContainer<cNode,true,false>(200,100), NbNodes(0), CurTag(0), Level(level)
{
	for(size_t i=201;--i;)
		InsertPtr(new cNode());
}


//-----------------------------------------------------------------------------
void cDepth::Clear(void)
{
	RCursor<cNode> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	NbNodes=0;
	CurTag=0;
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
cDepths::cDepths(void) : RContainer<cDepth,true,false>(20,5), NbDepths(0), CurTag(0)
{
	for(size_t i=0;i<20;i++)
		InsertPtr(new cDepth(i));
}


//-----------------------------------------------------------------------------
void cDepths::Clear(void)
{
	RCursor<cDepth> Cur(GetDepths());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	NbDepths=0;
	CurTag=0;
}


//-----------------------------------------------------------------------------
inline cDepth* cDepths::GetDepth(size_t depth)
{
	cDepth* ptr;
	if(depth==GetNb())
		InsertPtr(ptr=new cDepth(depth));
	else
		ptr=(*this)[depth];
	if(depth+1>NbDepths)
		NbDepths=depth+1;
	return(ptr);
}


//-----------------------------------------------------------------------------
inline RCursor<cDepth> cDepths::GetDepths(void) const
{
	return(RCursor<cDepth>(*this,0,NbDepths));
}


//-----------------------------------------------------------------------------
inline RCursor<cDepth> cDepths::GetDepths(size_t max) const
{
	if(max>NbDepths)
		max=NbDepths;
	return(RCursor<cDepth>(*this,0,max));
}


//-----------------------------------------------------------------------------
inline cNode* cDepths::GetNewNode(size_t depth,bool tag)
{
	cDepth* CurDepth=GetDepth(depth);
	cNode* ptr;
	if(CurDepth->NbNodes==CurDepth->GetNb())
		CurDepth->InsertPtr(ptr=new cNode());
	else
		ptr=(*CurDepth)[CurDepth->NbNodes];
	CurDepth->NbNodes++;
	if(tag)
	{
		CurTag=ptr;
		CurDepth->CurTag=ptr;
	}
	return(ptr);
}



//-----------------------------------------------------------------------------
//
// class GTextAnalyse
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GTextAnalyse::GTextAnalyse(GFactoryDocAnalyse* fac)
	: GDocAnalyse(fac), RXMLParser(), Struct(0), Infos(0), Words(5000,2500), Hash(27,27,500,250), IndexTags(100), Depths(),
	  Sl(0), Sldiff(0), Lang(0), StructSpace(0), IndexSpace(0), IsTitle(false)
{
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ApplyConfig(void)
{
	StaticLang=Factory->GetBool("StaticLang");
	MinStopWords=Factory->GetDouble("MinStopWords");
	MinWordSize=Factory->GetUInt("MinWordSize");
	MinStemSize=Factory->GetUInt("MinStemSize");
	MinOccur=Factory->GetUInt("MinOccur");
	NonLetterWords=Factory->GetBool("NonLetterWords");
	Filtering=Factory->GetBool("Filtering");
	NbSameOccur=Factory->GetUInt("NbSameOccur");
	NormalRatio=Factory->GetDouble("NormalRatio");
	ExtractStruct=Factory->GetBool("ExtractStruct");
	StructIsContent=Factory->GetBool("StructIsContent");
	ExtractIndex=Factory->GetBool("ExtractIndex");
	MaxTerms=Factory->GetUInt("MaxTerms");
	MaxOccurs=Factory->GetUInt("MaxOccurs");
	MaxDepth=Factory->GetUInt("MaxDepth");
	ChildTags=Factory->GetBool("ChildTags");
	WeightStruct=Factory->GetDouble("WeightStruct");
	AttrValues=Factory->GetBool("AttrValues");
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
	Sl=new size_t[NbLangs];
	Sldiff=new size_t[NbLangs];
	for(size_t i=Words.GetMaxNb()+1;--i;)
		Words.InsertPtr(new cWord(NbLangs));

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
	Hash.Clear();
	Words.Clear();
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

	Struct=0;

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
	N=Ndiff=Nwords=V=Vdiff=S=Sdiff=0;
	RCursor<cWord> Word(Words);
	for(Word.Start();!Word.End();Word.Next())
		Word()->Clear();
	Hash.Clear();
	Depths.Clear();

	// Clear Indexes
	IndexTags.Clear();
	NbTags=0;

//	TopNode.Clear();
}


//-----------------------------------------------------------------------------
void GTextAnalyse::BeginTag(const RString& namespaceURI,const RString& lName,const RString&)
{
	if(!ExtractStruct) return;

	// If tags are content -> add short name of the tag
	if(StructIsContent)
		ExtractValidWords(lName,WeightStruct);

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
	IndexTag* tag=IndexTags.GetInsertPtr(tmpStr);
	tag->Occurs++;
	NbTags++;

	// Stopped the analyze if not necessary anymore
	if((!FullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;

	// Create the tag
//	cout<<"Index: *"<<tmpStr<<"*"<<endl;
	// Get the node of the previous depth
	cNode* CurNode=Depths.GetNewNode(CurDepth,true);
	CurNode->SetTag(tag,LastTokenPos,CurDepth);

	// Make the sibling if depth>0
	if(CurDepth)
	{
		cNode* parent=Depths.GetDepth(CurDepth-1)->GetCurTag();
		if(parent&&parent->Obj.Tag)
		{
//			cout<<"Sibling between '"<<CurNode->Obj.Tag->Name<<"' and '"<<parent->Obj.Tag->Name<<"'"<<endl;
			if(!parent->Child)
				parent->Child=Depths.GetDepth(CurDepth)->GetNbNodes()-1;
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
		ExtractValidWords(lName,WeightStruct);

	// Stopped the analyze if not necessary anymore
	if((!FullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
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
}


//------------------------------------------------------------------------------
void GTextAnalyse::Value(const RString&)
{
	if(!ExtractStruct) return;

	// Stopped the analyze if not necessary anymore
	if((!FullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;
}


//-----------------------------------------------------------------------------
void GTextAnalyse::EndTag(const RString&, const RString& /*lName*/, const RString&)
{
	// Stopped the analyze if not necessary anymore
	if((!FullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Text(const RString& text)
{
	double w;
	if(IsTitle)
		w=2.0;
	else
		w=1.0;
	ExtractValidWords(text,w);
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
void GTextAnalyse::AddWord(const RString& word,double weight,bool letters)
{
	bool Find;
	size_t Index;
	cWord* w;
	RContainer<cWord,false,true>* Section;
	size_t i;
	bool *is;
	size_t* tmp1;
	size_t* tmp2;

	// Find the section of double hash table concerned by the current word.
	Section=(*(Hash)[word.HashIndex(1)])[word.HashIndex(2)];

	// Find the index where the word is or must be.
	Index=Section->GetIndex<const RString>(word,Find);

	// If the word wasn't found, insert it and look for each language if it's
	// in the corresponding stoplist.
	if(!Find)
	{
		// Create the word and insert it in the Occurs.
		w=Words[Ndiff++];
		Section->InsertPtrAt(w,Index);
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
		w=(*Section)[Index];
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
	if((!FullIndex)&&((!ExtractIndex)||(ExtractIndex&&(GetCurrentDepth()>MaxDepth))))
		return;

	// Create a new node
	cNode* node=Depths.GetNewNode(CurDepth,false);
	node->SetContent(w,LastTokenPos,CurDepth);
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ExtractValidWords(const R::RString& str,double weight)
{
	//cout<<"Extract ("<<pos<<"): *"<<str<<"*"<<endl;
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
		AddWord(word.ToLower(),weight,(NoAlpha==0));
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

	if(!Ndiff)
		return;

	for(CurLangs.Start(),i=0,tmp1=Sldiff,tmp2=Sl;!CurLangs.End();CurLangs.Next(),tmp1++,tmp2++,i++)
	{
		Frac=((double)(*tmp1))/((double)Ndiff);
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
	size_t i;
	RString stem(MaxWordLen);

	// Insert all the occurrences of the valid words
	RCursor<cWord> Word(Words);
	for(i=Ndiff+1,Word.Start();--i;Word.Next())
	{
		if(!Word()->NormalStem)
			continue;
		GConcept* concept=GetStemConcept(Word());
		if(!concept)
			continue;

		Occur=Infos->GetInsertPtr(concept);
		if(!Occur->GetWeight())
		{
			Vdiff++;
//			cout<<Word()->Word<<endl;
		}
		V+=Word()->Nb;
		(*Occur)+=Word()->Weight;
	}

	// Verify that each occurrences is not under the minimal.
	if(MinOccur<2)
		return;

	// Delete pointer from container with a cursor.
	RCursor<GWeightInfo> Cur(*Infos);
	RContainer<GWeightInfo,false,false> Del(30);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetWeight()<MinOccur)
			Del.InsertPtr(Cur());
	}
	Cur=Del;
	for(Cur.Start();!Cur.End();Cur.Next())
		Infos->DeletePtr(*Cur());
}



//-----------------------------------------------------------------------------
void GTextAnalyse::IndexXMLPart(void)
{
	if(!ExtractStruct) return;

	// Index first the tags
	RCursor<IndexTag> Tags(IndexTags);
	for(Tags.Start();!Tags.End();Tags.Next())
	{
		GConcept t(cNoRef,Tags()->Name,StructSpace,0,0,0,0);
		Tags()->Tag=StructSpace->InsertConcept(&t);
		GWeightInfo* info=Infos->GetInsertPtr(Tags()->Tag);
		(*info)+=1.0;
	}
	if((!ExtractIndex)&&(!FullIndex)) return;

	// Index the metadata and the structure
	GConceptType* LangSpace=Session->GetInsertConceptType(Lang->GetCode()+RString("Stems"),"");
	GXMLIndex w(IndexSpace,LangSpace);
	size_t max(SIZE_MAX),nb;
	bool IsTag;
	if(!ExtractStruct)
		max=MaxDepth;

	RCursor<cDepth> Depth(Depths.GetDepths(max));
	//cout<<"Iterate "<<Depth.GetNb()<<endl;
	for(Depth.Start();!Depth.End();Depth.Next())
	{
		//cout<<Depth()->GetLevel()<<" - "<<Depth()->GetNbNodes()<<endl;
		// Parse the node at this level
		RCursor<cNode> Node(Depth()->GetNodes());
		for(Node.Start(),IsTag=false;!Node.End();Node.Next())
		{
			cNode* ptr=Node();
			if(ptr->Type==VTR::Tag)
			{
				//cout<<"  "<<ptr->Obj.Tag->Name<<endl;

				// Already a tag indexed
				if(IsTag&&nb&&nb<=MaxTerms)
				{
					GWeightInfo* info=Infos->GetInsertPtr(IndexSpace->InsertConcept(&w));
					(*info)+=1.0;
					IsTag=false;
				}

				// Index this tag
				if(   (((!ChildTags)&&(!ptr->Nb))||(ChildTags)) && (ptr->Obj.Tag->Occurs<=MaxOccurs))
				{
					//cout<<Node()->Pos<<": "<<Node()->Obj.Tag->Name<<": "<<Node()->Child<<endl;
					nb=0;
					w.SetTag(ptr->Obj.Tag->Tag);
					IsTag=true;
				}
			}
			else if(ptr->Type==VTR::Content)
			{
				//cout<<"    "<<ptr->Obj.Word->Word<<endl;
				if(IsTag)
				{
					// Find the concept in stem space
					GConcept* concept=GetStemConcept(ptr->Obj.Word);
					if(!concept)
						continue;
					nb++;
					w.AddStem(concept);
				}
			}
		}
		// A tag is still indexed
		if(IsTag&&nb&&nb<=MaxTerms)
		{
			GWeightInfo* info=Infos->GetInsertPtr(IndexSpace->InsertConcept(&w));
			(*info)+=1.0;
			IsTag=false;
		}
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Analyze(const R::RURI& uri,const R::RURI& file,bool native,GLang* &lang,GWeightInfos* infos)
{
	RString Name;
	RCursor<RXMLTag> Tags;

	// Init part
	URI=uri;
	Lang=lang;
	Infos=infos;
	if(FullIndex&&native)
	{
/*		Struct=Doc->GetStruct();
		Struct->Clear();*/
	}
	else
		Struct=0;
	Clear();

	// If the language must not be found -> Compute LangIndex
	if(!FindLang)
		for(CurLangs.Start(),LangIndex=0;CurLangs()!=Lang;CurLangs.Next(),LangIndex++);

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
	lang=Lang;
}


//------------------------------------------------------------------------------
void GTextAnalyse::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("StaticLang",false));
	params->InsertParam(new RParamValue("MinStopWords",0.09));
	params->InsertParam(new RParamValue("MinWordSize",3));
	params->InsertParam(new RParamValue("MinStemSize",3));
	params->InsertParam(new RParamValue("MinOccur",1));
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
	params->InsertParam(new RParamValue("AttrValues",false));
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
