/*

	GALILEI Research Project

	GDocAnalyse.cpp

	Analyse a document - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
#include <gdocs/gdocanalyse.h>
#include <gdocs/gdoc.h>
#include <gdocs/gdocxml.h>
#include <glangs/gword.h>
#include <ginfos/giwordoccur.h>
#include <ginfos/giwordoccurs.h>
#include <glangs/gdict.h>
#include <glangs/glang.h>
#include <filters/gmimefilter.h>
#include <gprofiles/gprofile.h>
#include <gprofiles/gprofdoc.h>
#include <gsessions/gsession.h>
using namespace GALILEI;
using namespace RXML;
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class Word
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GDocAnalyse::WordOccur
{
public:
	char Word[250];
	bool* InStop;
	unsigned int Nb;
	bool OnlyLetters;

	WordOccur(unsigned int nb) throw(bad_alloc);
	inline void Clear(void) {(*Word)=0; Nb=0;}

	int Compare(const WordOccur& word) const
		{return(strcmp(Word,word.Word));}
	int Compare(const WordOccur* word) const
		{return(strcmp(Word,word->Word));}
	int Compare(const char* word) const
		{return(strcmp(Word,word));}

	static char HashIndex(const WordOccur* w)
		{return(RStd::RString::HashIndex(w->Word));}
	static char HashIndex(const WordOccur& w)
		{return(RStd::RString::HashIndex(w.Word));}
	static char HashIndex(const char* word)
		{return(RStd::RString::HashIndex(word));}

	static char HashIndex2(const WordOccur* w)
		{return(RStd::RString::HashIndex2(w->Word));}
	static char HashIndex2(const WordOccur& w)
		{return(RStd::RString::HashIndex2(w.Word));}
	static char HashIndex2(const char* word)
		{return(RStd::RString::HashIndex2(word));}

	~WordOccur(void);
};


//-----------------------------------------------------------------------------
GALILEI::GDocAnalyse::WordOccur::WordOccur(unsigned int nb) throw(bad_alloc)
	: InStop(0)
{
	InStop=new bool[nb];
	Clear();
}


//-----------------------------------------------------------------------------
GALILEI::GDocAnalyse::WordOccur::~WordOccur(void)
{
	if(InStop) delete[] InStop;
}



//-----------------------------------------------------------------------------
//
// class GDocAnalyse
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocAnalyse::GDocAnalyse(GSession* s) throw(bad_alloc)
	: Session(s), CurLangs(Session->GetLangs()), Occurs(0), Words(0), Direct(0),
	  NbDirect(5000), Sl(0), Sldiff(0), Lang(0)
{
	WordOccur** ptr;
	unsigned int i;

	Sl=new unsigned int[Session->GetNbLangs()];
	Sldiff=new unsigned int[Session->GetNbLangs()];
	Occurs=new RDblHashContainer<WordOccur,unsigned,27,27,false>(500,250);
	Direct=new WordOccur*[NbDirect];
	for(i=NbDirect+1,ptr=Direct;--i;ptr++)
		(*ptr)=new WordOccur(Session->GetNbLangs());
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::Clear(void)
{
	WordOccur** ptr;
	unsigned int i,j;
	RContainer<WordOccur,unsigned int,false,true>*** ptr1;
	RContainer<WordOccur,unsigned int,false,true>** ptr2;

	memset(Sl,0,sizeof(unsigned int)*Session->GetNbLangs());
	memset(Sldiff,0,sizeof(unsigned int)*Session->GetNbLangs());
	N=Ndiff=V=Vdiff=S=Sdiff=0;
	for(i=NbDirect+1,ptr=Direct;--i;ptr++)
		(*ptr)->Clear();
	for(i=27+1,ptr1=Occurs->Hash;--i;ptr1++)
		for(j=27+1,ptr2=*ptr1;--j;ptr2++)
    {
		(*ptr2)->LastPtr=(*ptr2)->NbPtr=0;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::VerifyDirect(void) throw(bad_alloc)
{
	unsigned int i;
	WordOccur** ptr;

	if(NbDirect==Ndiff)
	{
		ptr=new WordOccur*[NbDirect+2500];
		memcpy(ptr,Direct,NbDirect*sizeof(WordOccur*));
		delete[] Direct;
		Direct=ptr;
		for(i=2500+1,ptr=&Direct[NbDirect];--i;ptr++)
			(*ptr)=new WordOccur(Session->GetNbLangs());
		NbDirect+=2500;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::AddWord(const char* word) throw(bad_alloc)
{
	bool Find;
	unsigned int Index;
	WordOccur* w;
	RContainer<WordOccur,unsigned int,false,true>* Section;
	unsigned int i;
	bool *is;
	unsigned int* tmp1;
	unsigned int* tmp2;

	// Find the section of double hash table concerned by the current word.
	Section=Occurs->Hash[WordOccur::HashIndex(word)][WordOccur::HashIndex2(word)];

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
	}
	N++;
	w->Nb++;
}


//-----------------------------------------------------------------------------
bool GALILEI::GDocAnalyse::ExtractWord(const char* &ptr,RString& word)
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
	if(len<Session->GetMinWordSize())
	{
		Letter=false;
		goto BeginExtract;
	}

	// If just numbers or special characters or it doesn't begin with a letter, extract next word.
	if((!Letter)||(isdigit(*begin))||(strchr("-.@\\/",*begin)))
		goto BeginExtract;

	// Copy result in word, make it lower case and return true.
	word.Copy(begin,len);
	word.StrLwr();
	AddWord(word());
	return(true);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::AnalyseTag(RXMLTag* tag) throw(GException)
{
	const char* ptr;
	RString word(50);

	if(tag->GetName()=="Sentence")
	{
		ptr=tag->GetContent();
		while(*ptr)
			ExtractWord(ptr,word);
	}
	else
	{
		for(tag->Start();!tag->End();tag->Next())
			AnalyseTag((*tag)());
	}
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GDocAnalyse::DetermineLang(void) throw(GException)
{
	unsigned int LangIndex=cNoRef;
	double Frac,MinFrac;
	unsigned int i;
	unsigned int* tmp1;
	unsigned int* tmp2;

	MinFrac=Session->GetMinStopWords();
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
	return(LangIndex);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::ConstructInfos(unsigned int LangIndex) throw(GException)
{
	WordOccur** wrd;
	GIWordOccur** Tab;
	GIWordOccur* Occur;
	unsigned int i,MinOccur;
	RString stem(50);
	GDict* dic;

	// Insert all the occurences of the valid words
	dic=Session->GetDic(Lang);
	if(Words)
		Words->Clear();
	else
		Words=new GIWordOccurs(Ndiff);
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
		if(stem.GetLen()>=Session->GetMinStemSize())
		{
			Occur=Words->GetPtr(dic->GetId(stem));
			if(!Occur->GetNbOccurs())
				Vdiff++;
			V+=(*wrd)->Nb;
			Occur->AddNbOccurs((*wrd)->Nb);
		}
	}

	// Verify that each occurences is not under the minimal.
	MinOccur=Session->GetMinOccur();
	if(MinOccur<2) return;
	for(i=Words->NbPtr+1,Tab=Words->Tab;--i;Tab++)
	{
		Occur=(*Tab);
		if(Occur->GetNbOccurs()<MinOccur)
		{
			Words->DeletePtr(Occur);
			Tab--;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::Analyse(GDocXML* xml,GDoc* doc) throw(GException)
{
	RXMLTag* content;
	unsigned int LangIndex;

	// Init Part and verification
	if(!xml)
		throw GException("No XML Structure for document '"+doc->URL+"'");
	Lang=doc->GetLang();
	FindLang=((!doc->Lang)||(!Session->IsStaticLang()));
	content=xml->GetContent();
	RAssert(content);

	// Analyse the doc structure.
	Clear();
	AnalyseTag(content);

	// Determine the Language if necessary.
	if(FindLang)
		LangIndex=DetermineLang();
	else
	{
		for(CurLangs.Start(),LangIndex=0;CurLangs()!=Lang;CurLangs.Next(),LangIndex++);
	}
	if(!Lang)
	{
		doc->Lang=0;
		if(doc->Words)
			doc->Words->Clear();
		return;
	}

	// Construct Information
	ConstructInfos(LangIndex);

	// Set the Variable of the document
	doc->Lang=Lang;
	doc->N=N;
	doc->Ndiff=Ndiff;
	doc->V=V;
	doc->Vdiff=Vdiff;
	if(doc->Words)
		delete doc->Words;

	// Make it 'Updated' and tell all the profiles that have judge this
	// document that they are 'Modified'.
	doc->Words=Words;
	Words=0;
	doc->State=osUpdated;
	doc->Computed.SetToday();
	for(doc->Fdbks.Start();!doc->Fdbks.End();doc->Fdbks.Next())
		doc->Fdbks()->GetProfile()->SetState(osModified);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocAnalyse::ComputeStats(GDocXML* xml) throw(GException)
{
	RXMLTag* content;
	unsigned int LangIndex;

	// Init Part and verification
	if(!xml)
		throw GException("No XML Structure");
	content=xml->GetContent();
	RAssert(content);
	Lang=0;
	FindLang=true;
	Clear();

	// Analyse the doc structure.
	AnalyseTag(content);

	// Compute everything for current structure.
	LangIndex=DetermineLang();
	if(!Lang) return;
	ConstructInfos(LangIndex);
}


//-----------------------------------------------------------------------------
GALILEI::GDocAnalyse::~GDocAnalyse(void)
{
	WordOccur** ptr;
	unsigned int i;

	if(Occurs) delete Occurs;
	if(Direct)
	{
		for(i=NbDirect+1,ptr=Direct;--i;ptr++)
			delete(*ptr);
		delete[] Direct;
	}
	if(Sldiff) delete[] Sldiff;
	if(Sl) delete[] Sl;
	if(Words) delete Words;
}
