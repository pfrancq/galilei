/*

	GALILEI Research Project

	GDocAnalyse.cpp

	Analyse a document - Implementation.

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


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocanalyse.h>
#include <docs/gdocvector.h>
#include <docs/gdocoptions.h>
#include <docs/gdocxml.h>
#include <langs/gword.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
#include <langs/gdict.h>
#include <langs/glang.h>
#include <filters/gmimefilter.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace RXML;
using namespace RTimeDate;


//-----------------------------------------------------------------------------
// Constance
const unsigned int MaxWordLen=50;



//-----------------------------------------------------------------------------
//
// class Word
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GDocAnalyse::WordWeight
{
public:
	char Word[MaxWordLen+1];
	bool* InStop;
	unsigned int Nb;
	double Weight;
	bool OnlyLetters;

	WordWeight(unsigned int nb) throw(bad_alloc);
	inline void Clear(void) {(*Word)=0; Nb=0; Weight=0.0;}

	int Compare(const WordWeight& word) const
		{return(strcmp(Word,word.Word));}
	int Compare(const WordWeight* word) const
		{return(strcmp(Word,word->Word));}
	int Compare(const char* word) const
		{return(strcmp(Word,word));}

	static char HashIndex(const WordWeight* w)
		{return(RStd::RString::HashIndex(w->Word));}
	static char HashIndex(const WordWeight& w)
		{return(RStd::RString::HashIndex(w.Word));}
	static char HashIndex(const char* word)
		{return(RStd::RString::HashIndex(word));}

	static char HashIndex2(const WordWeight* w)
		{return(RStd::RString::HashIndex2(w->Word));}
	static char HashIndex2(const WordWeight& w)
		{return(RStd::RString::HashIndex2(w.Word));}
	static char HashIndex2(const char* word)
		{return(RStd::RString::HashIndex2(word));}

	~WordWeight(void);
};


//-----------------------------------------------------------------------------
GALILEI::GDocAnalyse::WordWeight::WordWeight(unsigned int nb) throw(bad_alloc)
	: InStop(0)
{
	InStop=new bool[nb];
	Clear();
}


//-----------------------------------------------------------------------------
GALILEI::GDocAnalyse::WordWeight::~WordWeight(void)
{
	if(InStop) delete[] InStop;
}



//-----------------------------------------------------------------------------
//
// class GDocAnalyse
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocAnalyse::GDocAnalyse(GSession* s,GDocOptions* opt) throw(bad_alloc)
	: Session(s), Weights(0), Doc(0), Direct(0),
	  NbDirect(5000), Sl(0), Sldiff(0), Lang(0), Options(opt)
{
	WordWeight** ptr;
	unsigned int i;

	CurLangs.Set(Session->GetLangs());
	Sl=new unsigned int[Session->GetNbLangs()];
	Sldiff=new unsigned int[Session->GetNbLangs()];
	Weights=new RDblHashContainer<WordWeight,unsigned,27,27,false>(500,250);
	Direct=new WordWeight*[NbDirect];
	for(i=NbDirect+1,ptr=Direct;--i;ptr++)
		(*ptr)=new WordWeight(Session->GetNbLangs());
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::Clear(void)
{
	WordWeight** ptr;
	unsigned int i,j;
	RContainer<WordWeight,unsigned int,false,true>*** ptr1;
	RContainer<WordWeight,unsigned int,false,true>** ptr2;

	memset(Sl,0,sizeof(unsigned int)*Session->GetNbLangs());
	memset(Sldiff,0,sizeof(unsigned int)*Session->GetNbLangs());
	N=Ndiff=V=Vdiff=S=Sdiff=0;
	for(i=NbDirect+1,ptr=Direct;--i;ptr++)
		(*ptr)->Clear();
	for(i=27+1,ptr1=Weights->Hash;--i;ptr1++)
		for(j=27+1,ptr2=*ptr1;--j;ptr2++)
    {
		(*ptr2)->LastPtr=(*ptr2)->NbPtr=0;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::VerifyDirect(void) throw(bad_alloc)
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
			(*ptr)=new WordWeight(Session->GetNbLangs());
		NbDirect+=2500;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::AddWord(const char* word,double weight) throw(bad_alloc)
{
	bool Find;
	unsigned int Index;
	WordWeight* w;
	RContainer<WordWeight,unsigned int,false,true>* Section;
	unsigned int i;
	bool *is;
	unsigned int* tmp1;
	unsigned int* tmp2;

	// Find the section of double hash table concerned by the current word.
	Section=Weights->Hash[WordWeight::HashIndex(word)][WordWeight::HashIndex2(word)];

	// Find the index where the word is or must be.
	Index=Section->GetId<const char*>(word,Find);

	// If the word wasn't found, insert it and look for each language if it's
	// in the corresponding stoplist.
	if(!Find)
	{
		// If Direct to small, extend it.
		VerifyDirect();

		// Create the word and insert it in the Occurs.
		w=Direct[Ndiff++];
		Section->InsertPtrOrderAt(w,Index);
		w->OnlyLetters=OnlyLetters;
		strcpy(w->Word,word);

		// Look for each language if the word is in the stop list.
		if(FindLang)
		{
			for(CurLangs.Start(),is=w->InStop,tmp1=Sldiff,tmp2=Sl;!CurLangs.End();CurLangs.Next(),is++,tmp1++,tmp2++)
			{
				(*is)=Session->GetStop(CurLangs())->IsIn<const char*>(word);
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
			if(Session->GetStop(Lang)->IsIn<const char*>(word))
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
		w=Section->Tab[Index];
		if(FindLang)
		{
			for(i=Session->GetNbLangs()+1,is=w->InStop,tmp2=Sl;--i;is++,tmp2++)
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
	N++;
	w->Nb++;
	w->Weight+=weight;
}


//-----------------------------------------------------------------------------
bool GALILEI::GDocAnalyse::ExtractWord(const char* &ptr,RString& word,double weight)
{
	unsigned len;
	const char* begin;
	bool Cont;
	bool Letter=false;

BeginExtract:

	// Init Part
	len=0;
	Cont=true;
	OnlyLetters=true;

	// Skip spaces and punctation.
	while((*ptr)&&(!isalnum(*ptr)))
		ptr++;
	begin=ptr;

	// Read the word
	while((*ptr)&&Cont)
	{
		// Read word --> look for a non alphanumeric character.
		while((*ptr)&&(isalnum(*ptr)))
		{
			if(!isdigit(*ptr))
				Letter=true;
			else
				OnlyLetters=false;
			len++;
			ptr++;
		}
		Cont=false;   // Normally, end of the word.
		if(!(*ptr)) continue;

		// If the next character is in {-./\@} and no blank space after,
		// then continue the word.
		if((strchr("-.@\\/",*ptr))&&(isalnum(*(ptr+1))))
		{
			Cont=true;
			OnlyLetters=false;
			ptr++;
			len++;
		}
	}

	// If len null, return (nothing else to extract)
	if(!len) return(false);

	// if len<MinWordSize, extract next word.
	if(len<Options->MinWordSize)
	{
		Letter=false;
		goto BeginExtract;
	}

	// if not only letters and non-letter words not enabled -> extract next word.
	if((!OnlyLetters)&&(!Options->NonLetterWords))
		goto BeginExtract;

	// If just numbers or special characters or it doesn't begin with a letter, extract next word.
	if((!Letter)||(isdigit(*begin))||(strchr("-.@\\/",*begin)))
		goto BeginExtract;

	// Copy result in word, make it lower case and return true.
	if(len>MaxWordLen)
	{
		ptr-=( len - MaxWordLen +1);
		len=MaxWordLen;
			// if the word ends with a " ' " or a " \ " the skip the character.
		if ( strchr("\'\\",*(ptr)) )
		{
			len--;
		}
	}
	word.Copy(begin,len);
	word.StrLwr();
	AddWord(word(),weight);
	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::AnalyseTag(RXMLTag* tag,double weight) throw(GException)
{
	const char* ptr;
	RString word(50);

	if(tag->GetName()=="docxml:sentence")
	{
		ptr=tag->GetContent();
		while(*ptr)
			ExtractWord(ptr,word,weight);
	}
	else
	{
		for(tag->Start();!tag->End();tag->Next())
			AnalyseTag((*tag)(),weight);
	}
}


////-----------------------------------------------------------------------------
//void GALILEI::GDocAnalyse::AnalyseLinksTag(RXMLTag* tag,bool externalLinks ,RContainer<GDoc,unsigned int,false,true>* DocsToAdd) throw(GException)
//{
//	const char* ptr;
//	const char* endPtr;
//	GDoc* tmpDoc=0;
//
//	endPtr=ptr=0;
//
//	if (tag->GetName()=="dc:identifier")
//	{
//		ptr=tag->GetContent();
//
//		// keep only html links (-> whith html extension)
//		endPtr=ptr;
//		while (*endPtr)
//		{
//			endPtr++;
//		}
//		while ((*endPtr)!='.')
//		{
//			endPtr--;
//		}
//		if ((!strncasecmp(endPtr,".html",5)) || (!strncasecmp(endPtr,".htm",4)))
//		{
//			tmpDoc = Session->GetDoc(ptr);   //where ptr is the url.
//			if (! tmpDoc)
//			{
//				if (externalLinks)
//				{
//					DocsToAdd->InsertPtr(tmpDoc=Session->NewDoc(ptr,ptr,"text/html"));
//					tmpDoc->SetState(osNotNeeded);
//				}
//			}
//			else
//			{
//				Doc->InsertLink(tmpDoc);
//			}
//		}
//	}
//	else
//	{
//		for (tag->Start();!tag->End();tag->Next())
//			AnalyseLinksTag((*tag)(),externalLinks,DocsToAdd);
//	}
//#pragma warn "ici il faut rajouter les proprietes des liens." ;
//}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::AnalyseLinksTag(RXMLTag* tag,bool externalLinks ,RContainer<GDoc,unsigned int,false,true>* DocsToAdd) throw(GException)
{
	const char* ptr;
	const char* endPtr;
	char* url;
	char* type;
	char* format;
	GDoc* tmpDoc=0;
	bool bUrl;

	endPtr=ptr=0;
	bUrl=false;
	url=new char[250];
	type=new char[100];
	format=new char[100];
	
	if (tag->GetName()== "docxml:metaData")
	{
		bUrl=false;
		
		type=0;
		format=0;
		for (tag->Start();!tag->End();tag->Next())
		{
			if ((*tag)()->GetName()=="dc:identifier")
			{
				bUrl=true;
				url=(*tag)()->GetContent();
			}
			if ((*tag)()->GetName()=="dc:format")
			{
				//format=(*tag)()->GetContent();
			}
			if ((*tag)()->GetName()=="dc:type")
			{
//				type=(*tag)()->GetContent();
//				if (!strcmp(type,"REFRESH"))
//				{
//					if (!Options->UseRedirection)
//					{
						bUrl=false;
//					}
//				}
			}
		}
		if (bUrl) 
		{
			ptr=strdup(url);
			// keep only html links (-> whith html extension)
			endPtr=ptr;
			while (*endPtr)
			{
				endPtr++;
			}
			while ((*endPtr)!='.')
			{
				endPtr--;
			}
			if ((!strncasecmp(endPtr,".html",5)) || (!strncasecmp(endPtr,".htm",4)))
			{
				tmpDoc = Session->GetDoc(ptr);   //where ptr is the url.
				if (! tmpDoc)
				{
					if (externalLinks)
					{
						DocsToAdd->InsertPtr(tmpDoc=Session->NewDoc(ptr,ptr,"text/html"));
						tmpDoc->SetState(osNotNeeded);
					}
				}
				else
				{
					Doc->InsertLink(tmpDoc,format,type);
				}
			}
		}
	}
	else
	{
		for (tag->Start();!tag->End();tag->Next())
			AnalyseLinksTag((*tag)(),externalLinks,DocsToAdd);
	}
#pragma warn "ici il faut rajouter les proprietes des liens." ;
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::DetermineLang(void) throw(GException)
{
	double Frac,MinFrac;
	unsigned int i;
	unsigned int* tmp1;
	unsigned int* tmp2;

	LangIndex=cNoRef;
	MinFrac=Options->MinStopWords;
	Lang=0;
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
void GALILEI::GDocAnalyse::ConstructInfos(void) throw(GException)
{
	WordWeight** wrd;
	GIWordWeight** Tab;
	GIWordWeight* Occur;
	unsigned int i,MinOccur;
	RString stem(50);
	GDict* dic;

	// Insert all the occurences of the valid words
	dic=Session->GetDic(Lang);

	for(i=Ndiff+1,wrd=Direct;--i;wrd++)
	{
		if((*wrd)->InStop[LangIndex]) continue;
		if((*wrd)->OnlyLetters)
		{
			stem=Lang->GetStemming((*wrd)->Word);
		}
		else
		{
			// Not necessary because when not begin with a letter, word not extract
			// if(!Lang->ValidWord((*wrd)->Word))
			//	continue;
			stem=(*wrd)->Word;
		}
		if(stem.GetLen()>=Options->MinStemSize)
		{
			Occur=Doc->GetInsertPtr(dic->GetId(stem));
			if(!Occur->GetWeight())
				Vdiff++;
			V+=(*wrd)->Nb;
			Occur->AddWeight((*wrd)->Weight);
		}
	}


	// Verify that each occurences is not under the minimal.
	MinOccur=Options->MinOccur;
	if(MinOccur<2) return;
	for(i=Doc->NbPtr+1,Tab=Doc->Tab;--i;Tab++)
	{
		Occur=(*Tab);
		if(Occur->GetWeight()<MinOccur)
		{
			Doc->DeletePtr(Occur);
			Tab--;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::Analyse(GDocXML* xml,GDoc* doc,RContainer<GDoc,unsigned int,false,true>* tmpDocs)throw(GException)
{
	RXMLTag* content;
	RXMLTag* metadata;
	RXMLTag* link;

	// Init Part and verification
	if(!xml)
		throw GException("No XML Structure for document '"+RString(doc->GetURL())+"'");
	Lang=doc->GetLang();
	FindLang=((!Lang)||(!Options->StaticLang));
	content=xml->GetContent();
	RAssert(content);
	metadata=xml->GetMetaData();
	RAssert(metadata);
	Doc=dynamic_cast<GDocVector*>(doc);

	// Analyse the doc structure.
	Clear();
	Doc->ClearInfos(FindLang);
	if(!FindLang)
	{
		// if Language defined -> Compute LangIndex
		for(CurLangs.Start(),LangIndex=0;CurLangs()!=Lang;CurLangs.Next(),LangIndex++);
	}
	AnalyseTag(metadata,2.0);
	AnalyseTag(content,1.0);

	// if option link is active -> Analyse the content of link tags
	if (Options->UseLink)
	{
		link = xml->GetLinks ();
		RAssert(link);
		AnalyseLinksTag(link,Options->UseExternalLink,tmpDocs);
	}

	// Determine the Language if necessary.
	if(FindLang)
	{
		DetermineLang();
		if(!Lang) return;
	}

	// Construct Information
	ConstructInfos();

	// Set the Variable of the document
	doc->SetInfos(Lang,N,Ndiff,V,Vdiff);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::ComputeStats(GDocXML* xml) throw(GException)
{
	RXMLTag* content;

	// Init Part and verification
	if(!xml)
		throw GException("No XML Structure");
	content=xml->GetContent();
	RAssert(content);
	Lang=0;
	FindLang=true;
	Clear();

	// Analyse the doc structure.
	AnalyseTag(content,1.0);

	// Compute everything for current structure.
	DetermineLang();
	if(!Lang) return;
	ConstructInfos();
}


//-----------------------------------------------------------------------------
GALILEI::GDocAnalyse::~GDocAnalyse(void)
{
	WordWeight** ptr;
	unsigned int i;

	if(Weights) delete Weights;
	if(Direct)
	{
		for(i=NbDirect+1,ptr=Direct;--i;ptr++)
			delete(*ptr);
		delete[] Direct;
	}
	if(Sldiff) delete[] Sldiff;
	if(Sl) delete[] Sl;
}
