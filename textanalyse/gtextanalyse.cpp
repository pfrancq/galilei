/*

	GALILEI Research Project

	GTextAnalyse.cpp

	Analyse a document - Implementation.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
// Constance
const unsigned int MaxWordLen=50;



//-----------------------------------------------------------------------------
//
// class WordWeight
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
WordWeight::WordWeight(unsigned int nb)
	:  Word(MaxWordLen+1), InStop(0)
{
	InStop=new bool[nb];
	Clear();
}


//-----------------------------------------------------------------------------
WordWeight::~WordWeight(void)
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
	: GDocAnalyse(fac), Weights(0), Infos(5000,2500), IndexTags(100), Direct(0), NbDirect(5000),
	  Order(0), NbOrder(5000), Sl(0), Sldiff(0), Lang(0), StructSpace(0), IndexSpace(0)
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
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Connect(GSession* session)
{
	WordWeight** ptr;
	GConcept** pt;
	unsigned int i;

	// First init
	if(Session) return;
	GDocAnalyse::Connect(session);

	// Create local structures
	CurLangs=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns();
	Sl=new unsigned int[CurLangs.GetNb()];
	Sldiff=new unsigned int[CurLangs.GetNb()];
	Weights=new RDblHashContainer<WordWeight,false>(27,27,500,250);
	Direct=new WordWeight*[NbDirect];
	for(i=NbDirect+1,ptr=Direct;--i;ptr++)
		(*ptr)=new WordWeight(CurLangs.GetNb());
	Order=new GConcept*[NbOrder];
	for(i=NbOrder+1,pt=Order;--i;pt++)
		(*pt)=new GConcept();

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
	WordWeight** ptr;
	GConcept** pt;
	unsigned int i;

	// Spaces are null
	StructSpace=0;
	IndexSpace=0;
	
	// Local Structure
	if(Weights)
	{
		delete Weights;
		Weights=0;
	}
	if(Direct)
	{
		for(i=NbDirect+1,ptr=Direct;--i;ptr++)
			delete(*ptr);
		delete[] Direct;
		Direct=0;
	}
	if(Order)
	{
		for(i=NbOrder+1,pt=Order;--i;pt++)
			delete(*pt);
		delete[] Order;
		Order=0;
	}
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
	WordWeight** ptr;
	GConcept** pt;
	unsigned int i;

	// Language information
	Lang=Doc->GetLang();
	FindLang=((!Lang)||(!StaticLang));	
	
	// Clear all structures needed to handle a document
	memset(Sl,0,sizeof(unsigned int)*CurLangs.GetNb());
	memset(Sldiff,0,sizeof(unsigned int)*CurLangs.GetNb());
	N=Ndiff=Nwords=V=Vdiff=S=Sdiff=0;
	for(i=NbDirect+1,ptr=Direct;--i;ptr++)
		(*ptr)->Clear();
	for(i=NbOrder+1,pt=Order;--i;pt++)
		if(*pt) (*pt)->Clear();
	Weights->Clear();

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
void GTextAnalyse::VerifyDirect(void)
{
	unsigned int i;
	WordWeight** ptr;

	if(NbDirect==Ndiff)
	{
		ptr=new WordWeight*[NbDirect+2500];
		memcpy(ptr,Direct,NbDirect*sizeof(WordWeight*));
		delete[] Direct;
		Direct=ptr;
		for(i=2500+1,ptr=&Direct[NbDirect];--i;ptr++)
			(*ptr)=new WordWeight(GALILEIApp->GetManager<GLangManager>("Lang")->GetNbFactories());
		NbDirect+=2500;
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyse::VerifyOrder(void)
{
	unsigned int i;
	GConcept** ptr;

	if(NbOrder==Nwords)
	{
		ptr=new GConcept*[NbOrder+2500];
		memcpy(ptr,Order,NbOrder*sizeof(GConcept*));
		delete[] Order;
		Order=ptr;
		for(i=2500+1,ptr=&Order[NbOrder];--i;ptr++)
			(*ptr)=new GConcept();
		NbOrder+=2500;
	}
}



//-----------------------------------------------------------------------------
bool GTextAnalyse::ValidWord(const RString kwd)
{
	unsigned int nb;
	const RChar* ptr;
	UChar old,act,lw;
	double fracnorm;
	int nbnorm;

	if(!kwd.GetLen()) return(false);


	ptr=kwd();
	old=ptr->Unicode();
	nb=1;
	nbnorm=0;

	lw=RChar::ToLower(*ptr).Unicode();
	if(ptr->IsAlNum())
		nbnorm++;

	for(ptr++;(!ptr->IsNull())&&(nb<NbSameOccur);ptr++)
	{
		act=ptr->Unicode();
		if(act!=old)
		{
			old=act;
			nb=1;
			lw=RChar::ToLower(*ptr).Unicode();
			if(ptr->IsAlNum())
				nbnorm++;
		}
		else
			nb++;
	}
	fracnorm=(double)nbnorm/(double)kwd.GetLen();
	return((nb<NbSameOccur)&&(fracnorm>NormalRatio));
}


//-----------------------------------------------------------------------------
void GTextAnalyse::AddWord(const RString word,double weight,IndexTag* idx)
{
	bool Find;
	unsigned int Index;
	WordWeight* w;
	RContainer<WordWeight,false,true>* Section;
	unsigned int i;
	bool *is;
	unsigned int* tmp1;
	unsigned int* tmp2;

	// If word not valid, skip it
	if((Filtering)&&(!ValidWord(word))) return;

	// Find the section of double hash table concerned by the current word.
	Section=(*(*Weights)[word.HashIndex(1)])[word.HashIndex(2)];

	// Find the index where the word is or must be.
	Index=Section->GetIndex<const RString>(word,Find);

	// If the word wasn't found, insert it and look for each language if it's
	// in the corresponding stoplist.
	if(!Find)
	{
		// If Direct to small, extend it.
		VerifyDirect();

		// Create the word and insert it in the Occurs.
		w=Direct[Ndiff++];
		Section->InsertPtrAt(w,Index);
		w->OnlyLetters=OnlyLetters;
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
	
	// Treat the correctly wordweight
	if(idx)
		idx->InsertPtr(w);
	else
	{
		N++;
		w->Nb++;
		w->Weight+=weight;	
		w->NormalStem=true;
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ExtractWord(const RChar* &ptr,double weight,IndexTag* idx)
{
	unsigned len;
	const RChar* begin;
	bool Cont;
	bool Letter=false;
	static RString Test1("-.@\\/");
	static RString Test2("\'\\");
	RString word;

BeginExtract:

	// Init Part
	len=0;
	Cont=true;
	OnlyLetters=true;

	// Skip spaces and punctation.
	while((!ptr->IsNull())&&(!ptr->IsAlNum()))
		ptr++;
	begin=ptr;

	// Read the word
	while((!ptr->IsNull())&&Cont)
	{
		// Read word --> look for a non alphanumeric character.
		while((!ptr->IsNull())&&(ptr->IsAlNum()))
		{
			if(!ptr->IsDigit())
				Letter=true;
			else
				OnlyLetters=false;
			len++;
			ptr++;
		}
		Cont=false;   // Normally, end of the word.
		if(ptr->IsNull()) continue;

		// If the next character is in {-./\@} and no blank space after,
		// then continue the word.
		if((RChar::StrChr(Test1(),*ptr))&&((ptr+1)->IsAlNum()))
		{
			Cont=true;
			OnlyLetters=false;
			ptr++;
			len++;
		}
	}

	// If len null -> Nothing else to extract
	if(!len)
		return;

	// If not only letters and non-letter words not enabled -> extract next word.
	if((!OnlyLetters)&&(!NonLetterWords))
		goto BeginExtract;

	// If just numbers or special characters or it doesn't begin with a letter, extract next word.
	if((!Letter)||(begin->IsDigit())||(RChar::StrChr(Test1(),*begin)))
		goto BeginExtract;

	// Copy result in word, make it lower case and return true.
	if(len>MaxWordLen)
	{
		ptr-=( len - MaxWordLen +1);
		len=MaxWordLen;

		// if the word ends with a " ' " or a " \ " the skip the character.
		if(RChar::StrChr(Test2(),*ptr))
		{
			len--;
		}
	}
	word.Copy(begin,len);
	AddWord(word.ToLower(),weight,idx);
}


//-----------------------------------------------------------------------------
void GTextAnalyse::DetermineLang(void)
{
	double Frac,MinFrac;
	unsigned int i;
	unsigned int* tmp1;
	unsigned int* tmp2;

	LangIndex=cNoRef;
	MinFrac=MinStopWords;
	Lang=0;

	if (!Ndiff)
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
GConcept* GTextAnalyse::GetStemConcept(WordWeight* word)
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
	GConcept w(cNoRef,stem,Lang->GetDict(),0,0,0);
	return(Lang->GetDict()->InsertConcept(&w));	
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ConstructInfos(unsigned int docid)
{
	WordWeight** wrd;
	GWeightInfo* Occur;
	unsigned int i;
	RString stem(MaxWordLen);

	// Insert all the occurences of the valid words
	for(i=Ndiff+1,wrd=Direct;--i;wrd++)
	{
		if(!(*wrd)->NormalStem)
			continue;
		GConcept* concept=GetStemConcept(*wrd);
		if(!concept)
			continue;

		Occur=Infos.GetInsertPtr(concept);
		if(!Occur->GetWeight())
			Vdiff++;
		V+=(*wrd)->Nb;
		(*Occur)+=(*wrd)->Weight;
		if(StoreFullWords)
			StoreWordStemInDatabase(Occur->GetId(), (*wrd)->Word, docid);
	}


	// Verify that each occurences is not under the minimal.
	if(MinOccur<2) return;

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
void GTextAnalyse::Analyze(GDoc *doc, const R::RURI& uri)
{
	RString Name;
	RCursor<RXMLTag> Tags;
	
	// Init part
	Doc=doc;
	Clear();
	
	// Load the xml from the file 
	XMLParser In(this,uri);
	In.Open(RIO::Read);
	In.Close();
		
	
	// Analyse the doc structure.
	if(!FindLang)
	{
		// if Language defined -> Compute LangIndex
		for(CurLangs.Start(),LangIndex=0;CurLangs()!=Lang;CurLangs.Next(),LangIndex++);
	}

	// Determine the Language if necessary. If the language cannot be found -> document is not indexed.
	if(FindLang)
		DetermineLang();
	if(!Lang)
		return;
		
	// Construct Information from the stop words extracted and the XML
	ConstructInfos(doc->GetId());
	IndexXMLPart();
	
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
		GConcept t(cNoRef,Cur()->Name,StructSpace,0,0,0);
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
		RContainer<GConcept,false,true> Stems(10);
		RContainer<GConcept,false,true> Universal(10);
		RCursor<WordWeight> Idx(*Cur());
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


//------------------------------------------------------------------------------
bool GTextAnalyse::StoreWordStemInDatabase(unsigned int stemid, RString word, unsigned int docid)
{
	// Check if the words/stem couple does not already exist
	RQuery* q=Session->GetStorage()->SelectDummyEntry("wordsstems",stemid,word,0,3);
	if(q->GetNb())
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
