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
#include <xmlparser.h>
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
// Constants
const size_t MaxWordLen=50;



//-----------------------------------------------------------------------------
//
// class cWord
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
cWord::cWord(size_t nblangs)
	:  Word(MaxWordLen+1), InStop(0), Occurs(10)
{
	InStop=new bool[nblangs];
	Clear();
}


//-----------------------------------------------------------------------------
cWord::~cWord(void)
{
	if(InStop) delete[] InStop;
}



//-----------------------------------------------------------------------------
//
// class IndexTag
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//
// class GTextAnalyse
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GTextAnalyse::GTextAnalyse(GFactoryDocAnalyse* fac)
	: GDocAnalyse(fac), Struct(0), Words(5000,2500), Hash(27,27,500,250), Infos(5000,2500), IndexTags(100),
	  Sl(0), Sldiff(0), Lang(0), StructSpace(0), IndexSpace(0)
{
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ApplyConfig(void)
{
	StaticLang=Factory->GetBool("StaticLang");
	MinStopWords=Factory->GetDouble("MinStopWords");
	MinWordSize=Factory->GetUInt("MinWordSize");
	MinStemSize=Factory->GetUInt("MinStemSize");
	StoreFullWords=Factory->GetBool("StoreFullWords");
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
	MaxPercOccurs=Factory->GetDouble("MaxPercOccurs")/100;
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
	Sl=new size_t[CurLangs.GetNb()];
	Sldiff=new size_t[CurLangs.GetNb()];
	for(size_t i=Words.GetMaxNb()+1;--i;)
		Words.InsertPtr(new cWord(CurLangs.GetNb()));

	// Init database
	if(StoreFullWords)
		Session->GetStorage()->CreateDummy("wordsstems");

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
	Lang=Doc->GetLang();
	FindLang=((!Lang)||(!StaticLang));

	// Clear all structures needed to handle a document
	memset(Sl,0,sizeof(size_t)*CurLangs.GetNb());
	memset(Sldiff,0,sizeof(size_t)*CurLangs.GetNb());
	N=Ndiff=Nwords=V=Vdiff=S=Sdiff=0;
	RCursor<cWord> Word(Words);
	for(Word.Start();!Word.End();Word.Next())
		Word()->Clear();
	Hash.Clear();

	// Clear Infos
	// Rem: Since Infos is not responsible for allocation/desallocation
	//      -> parse it to prevent memory leaks
	RCursor<GWeightInfo> Cur(Infos);
	for(Cur.Start();!Cur.End();Cur.Next())
		delete Cur();
	Infos.Clear();

	// Clear Indexes
	IndexTags.Clear();
	NbTags=0;
}


//-----------------------------------------------------------------------------
void GTextAnalyse::AddWord(const RString& word,double weight,cContent* content,GDocStructNode* parent,size_t pos,bool letters)
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
			for(i=GALILEIApp->GetManager<GLangManager>("Lang")->GetNbPlugIns()+1,is=w->InStop,tmp2=Sl;--i;is++,tmp2++)
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
	if(content)
		content->InsertPtr(w);
	else
	{
		N++;
		w->Nb++;
		w->Weight+=weight;
		w->NormalStem=true;
		if(parent)
			w->Occurs.InsertPtr(new cWordOccur(parent,pos));
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ExtractWord(const R::RString& str,double weight,cContent* content,GDocStructNode* parent,size_t pos)
{
//	cout<<"Extract ("<<pos<<"): *"<<str<<"*"<<endl;
	for(const RChar* ptr=str();!ptr->IsNull();)
	{
		// If only letters word -> skip non letters
		if(!NonLetterWords)
		{
			while((!ptr->IsNull())&&(!ptr->IsAlpha()))
				ptr++;

			if(ptr->IsNull())
				return;
		}

		// Always skip leading ' or "
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
		RString word;
		word.Copy(begin,len);
//		cout<<"  Index: *"<<word.ToLower()<<"*  ("<<(NoAlpha==0)<<") ="<<(double)(len-NoAlpha)/(double)(len)<<endl;
		AddWord(word.ToLower(),weight,content,parent,pos,(NoAlpha==0));
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
void GTextAnalyse::ConstructInfos(size_t docid)
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

		Occur=Infos.GetInsertPtr(concept);
		if(!Occur->GetWeight())
			Vdiff++;
		V+=Word()->Nb;
		(*Occur)+=Word()->Weight;

		// Add the occurrences in the XML structure
		if(Struct)
		{
			RCursor<cWordOccur> Occurs(Word()->Occurs);
			for(Occurs.Start();!Occurs.End();Occurs.Next())
				Struct->InsertContent(Occurs()->Parent,Occur,Occurs()->Pos);
		}

		// Store full word if necessary
		if(StoreFullWords)
			StoreWordStemInDatabase(Occur->GetId(),Word()->Word,docid);
	}


	// Verify that each occurrences is not under the minimal.
	if(MinOccur<2)
			return;

	// Delete pointer from container with a cursor.
	RCursor<GWeightInfo> Cur(Infos);
	RContainer<GWeightInfo,false,false> Del(30);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetWeight()<MinOccur)
			Del.InsertPtr(Cur());
	}
	Cur=Del;
	for(Cur.Start();!Cur.End();Cur.Next())
		Infos.DeletePtr(*Cur());
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Analyze(GDoc *doc,const R::RURI& uri,bool native)
{
	RString Name;
	RCursor<RXMLTag> Tags;

	// Init part
	Doc=doc;
	if(FullIndex&&native)
	{
		Struct=Doc->GetStruct();
		Struct->Clear();
	}
	else
		Struct=0;
	Clear();

	// If the language must not be found -> Compute LangIndex
	if(!FindLang)
		for(CurLangs.Start(),LangIndex=0;CurLangs()!=Lang;CurLangs.Next(),LangIndex++);

	// Load the xml from the file
	XMLParser In(this,uri);
	In.Open(RIO::Read);
	In.Close();

	// // Determine the Language if necessary. If the language cannot be found -> document is not indexed.
	if(FindLang)
		DetermineLang();
	if(!Lang)
		return;

	// Construct Information from the stop words extracted and the XML
	ConstructInfos(doc->GetId());
	IndexXMLPart();

/*	if(Struct)
	{
		Struct->Print();
		cout<<"-----"<<endl;
	}*/

	// Set the Variable of the document
	Doc->Update(Lang,&Infos,true);
}


//-----------------------------------------------------------------------------
void GTextAnalyse::IndexXMLPart(void)
{
	// Look which index must be used
	RCursor<IndexTag> Cur(IndexTags);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// If too much tags -> skip it
		if((Cur()->Occurs>MaxOccurs)&&((static_cast<double>(Cur()->Occurs)/static_cast<double>(NbTags))>MaxPercOccurs))
			continue;

		// Find the concept corresponding to the tag
		GConcept t(cNoRef,Cur()->Name,StructSpace,0,0,0,0);
		GConcept* Tag=StructSpace->InsertConcept(&t);

		// Verify if the structural element is already an information entity
		// If not -> add it
		GWeightInfo* info=Infos.GetPtr(Tag);
		if(!info)
		{
			info=Infos.GetInsertPtr(Tag);
			(*info)+=1.0;
		}

		// Add the concepts related to the stems
		RCursor<cContent> Idxs(*Cur());
		for(Idxs.Start();!Idxs.End();Idxs.Next())
		{
			RContainer<GConcept,false,true> Stems(10);
			RContainer<GConcept,false,true> Universal(10);
			RCursor<cWord> Idx(*Idxs());
			for(Idx.Start();!Idx.End();Idx.Next())
			{
				// Find the concept in stem stpace
				GConcept* concept=GetStemConcept(Idx());
				if(!concept)
					continue;

				Stems.InsertPtr(concept);
			}

			// If no stems -> Not a valid index
			if(!Stems.GetNb())
				continue;

			// Add this index
			GXMLIndex w(IndexSpace,Tag,Lang,Universal,Stems);
			info=Infos.GetInsertPtr(IndexSpace->InsertConcept(&w));
			(*info)+=1.0;
		}
	}
}


//------------------------------------------------------------------------------
bool GTextAnalyse::StoreWordStemInDatabase(size_t stemid, RString word,size_t docid)
{
	// Check if the words/stem couple does not already exist
	RQuery* q=Session->GetStorage()->SelectDummyEntry("wordsstems",stemid,word,0,3);
	if(!q->End())
		return(false);
	Session->GetStorage()->AddDummyEntry("wordsstems", stemid, word, docid );
	return(true);
}


//------------------------------------------------------------------------------
void GTextAnalyse::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("StaticLang",false));
	params->InsertParam(new RParamValue("MinStopWords",0.09));
	params->InsertParam(new RParamValue("MinWordSize",3));
	params->InsertParam(new RParamValue("MinStemSize",3));
	params->InsertParam(new RParamValue("StoreFullWords",false));
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
	params->InsertParam(new RParamValue("MaxPercOccurs",5.0));
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
	// Clear Infos
	// Rem: Since Infos is not responsible for allocation/desallocation
	//      -> parse it to prevent memory leaks
	RCursor<GWeightInfo> Cur(Infos);
	for(Cur.Start();!Cur.End();Cur.Next())
		delete Cur();
	Infos.Clear();
}


//------------------------------------------------------------------------------
CREATE_DOCANALYSE_FACTORY("Text Analyse",GTextAnalyse)
