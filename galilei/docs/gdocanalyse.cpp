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
	: Session(s), CurLangs(Session->GetLangs()), Occurs(0), Direct(0),
	  NbDirect(5000), tmpNbDiffStopWords(0), tmpNbStopWords(0), Lang(0), Doc(0)
{
	WordOccur** ptr;
	unsigned int i;

	tmpNbDiffStopWords=new unsigned int[Session->GetNbLangs()];
	tmpNbStopWords=new unsigned int[Session->GetNbLangs()];
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

	memset(tmpNbDiffStopWords,0,sizeof(unsigned int)*Session->GetNbLangs());
	memset(tmpNbStopWords,0,sizeof(unsigned int)*Session->GetNbLangs());
	NbDiffWords=NbWords=NbDiffStopWords=NbStopWords=0;
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

	if(NbDirect==NbDiffWords)
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
		w=Direct[NbDiffWords++];
		Section->InsertPtrOrderAt(w,Index);
		w->OnlyLetters=OnlyLetters;
		strcpy(w->Word,word);

		// Look for each language if the word is in the stop list.
		if(FindLang)
		{
			for(CurLangs.Start(),is=w->InStop,tmp1=tmpNbDiffStopWords,tmp2=tmpNbStopWords;!CurLangs.End();CurLangs.Next(),is++,tmp1++,tmp2++)
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
			for(i=Session->GetNbLangs()+1,is=w->InStop,tmp2=tmpNbStopWords;--i;is++,tmp2++)
			{
				if(*is)
					(*tmp2)++;
			}
		}
	}
	NbWords++;
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

	// If just numbers or special characters or it begins with a number, extract next word.
	if((!Letter)||(isdigit(*begin)))
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
void GALILEI::GDocAnalyse::Analyse(GDocXML* xml,GDoc* doc) throw(GException)
{
	RXMLTag* content;
	GDict* dic;
	double Frac,MinFrac;
	unsigned int i;
	unsigned int* tmp1;
	unsigned int* tmp2;
	WordOccur** wrd;
	RString stem(50);
	unsigned int LangIndex;
	GIWordOccur* Occur;

	// Init Part and verification
	if(!xml)
		throw GException("No XML Structure for document '"+doc->URL+"'");
	Doc=doc;
	Lang=Doc->GetLang();
	MinFrac=Session->GetMinStopWords();
	FindLang=((!Doc->Lang)||(!Session->IsStaticLang()));
	content=xml->GetContent();
	RAssert(content);
	Clear();

	// Analyse the doc structure.
	AnalyseTag(content);

	// Find Language (if necessary) with the maximal number of words of the
	// stoplist contained in the document and with the minimal value for the
	// fraction of stopwords that are in the document.
	if(FindLang)
	{
		Doc->Lang=Lang=0;
		for(CurLangs.Start(),i=0,tmp1=tmpNbDiffStopWords,tmp2=tmpNbStopWords;!CurLangs.End();CurLangs.Next(),tmp1++,tmp2++,i++)
		{
			Frac=((double)(*tmp1))/((double)NbDiffWords);
			if(((*tmp2)>NbStopWords)&&(Frac>=MinFrac))
			{
				Lang=CurLangs();
				NbStopWords=(*tmp2);
				NbDiffStopWords=(*tmp1);
				LangIndex=i;
			}
		}
		if(!Lang) return;
	}
	else
	{
		for(CurLangs.Start(),LangIndex=0;CurLangs()!=Lang;CurLangs.Next(),LangIndex++);
	}

	// Set the Variable of the document
	Doc->Lang=Lang;
	Doc->TotalWords=NbWords;
	Doc->NbWords=NbWords-NbStopWords;
	Doc->NbDiffWords=NbDiffWords-NbDiffStopWords;
	dic=Session->GetDic(Lang);

	// Analyse it
	Doc->Words->Clear();
	for(i=NbDiffWords+1,wrd=Direct;--i;wrd++)
	{
		if((*wrd)->InStop[LangIndex]) continue;
		if((*wrd)->OnlyLetters)
		{
			stem=Doc->Lang->GetStemming((*wrd)->Word);
		}
		else
		{
			if(!Lang->ValidWord((*wrd)->Word))
				continue;
			stem=(*wrd)->Word;
		}
		if(stem.GetLen()>=2)
		{
			Occur=Doc->Words->GetPtr(dic->GetId(stem));
			Occur->AddNbOccurs((*wrd)->Nb);
		}
	}

	// Make it 'Updated' and tell all the profiles that have judge this
	// document that they are 'Modified'.
	Doc->State=osUpdated;
	Doc->Computed.SetToday();
	for(Doc->Fdbks.Start();!Doc->Fdbks.End();Doc->Fdbks.Next())
		Doc->Fdbks()->GetProfile()->SetState(osModified);
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
	if(tmpNbDiffStopWords) delete[] tmpNbDiffStopWords;
	if(tmpNbStopWords) delete[] tmpNbStopWords;
}
