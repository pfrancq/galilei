/*

	GALILEI Research Project

	GOoc.cpp

	Document - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocs/gdoc.h>
#include <gdocs/gdocxml.h>
#include <gsessions/gsession.h>
using namespace GALILEI;
using namespace RXML;



//-----------------------------------------------------------------------------
//
// class GDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDoc::GDoc (const RString& url,const RStd::RString& name,const unsigned int id,GMIMEFilter* t,unsigned int nbdiff) throw(bad_alloc)
  : URL(url), Name(name), Id(id), Words(nbdiff>300?nbdiff:300),NbWords(0),NbDiffWords(nbdiff), Lang(0),
    Calc(true), bSave(false), Type(t)
{
}


//-----------------------------------------------------------------------------
GALILEI::GDoc::GDoc(const RString& url,const RStd::RString& name,const unsigned int id,GLang* lang,GMIMEFilter* t,const unsigned int nb,const unsigned int nbdiff) throw(bad_alloc)
	: URL(url), Name(name), Id(id), Words(nbdiff>300?nbdiff:300),NbWords(nb), NbDiffWords(nbdiff),
	  Lang(lang), Calc(true), bSave(false), Type(t)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const GDoc& doc) const
{
  return(Id-doc.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const GDoc* doc) const
{
  return(Id-doc->Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const unsigned id) const
{
  return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const GLang* lang) const
{
  return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
bool GALILEI::GDoc::ExtractWord(const char* &ptr,RString& word)
{
	unsigned len;
	const char* begin;
	bool Cont;
	bool Letter=false;

BeginExtract:

	// Init Part
	len=0;
	Cont=true;

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
			ptr++;
			len++;
		}
	}

	// If len null, return (nothing else to extract)
	if(!len) return(false);

	// If just numbers or special characters, extract next word.
	if(!Letter)
		goto BeginExtract;

	// Copy result in word, make it lower case and return true.
	word.Copy(begin,len);
	word.StrLwr();
	return(true);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::AnalyseTagForStopKwd(RXMLTag* tag,GDict* stop)
{
	unsigned nb=0;
	const char* ptr;
	RString word(50);

	if(tag->GetName()=="Sentence")
	{
		ptr=tag->GetContent();
		while(*ptr)
		{
			if((ExtractWord(ptr,word))&&(stop->IsIn(word)))
				nb++;
		}
	}
	else
	{
		for(tag->Start();!tag->End();tag->Next())
			nb+=AnalyseTagForStopKwd((*tag)(),stop);
	}
	return(nb);
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::AnalyseContentTag(RXMLTag* tag,GDict* stop,GDict* dic) throw(GException)
{
	cout<<tag->GetName()<<endl;
	const char* ptr;
	RString word(50),stem(50);
	GIWordOccur* Occur;

	if(tag->GetName()=="Sentence")
	{
		ptr=tag->GetContent();
		while(*ptr)
		{
			if(ExtractWord(ptr,word))
			{
				NbWords++;
				if(!stop->IsIn(word))
				{
					stem=Lang->GetStemming(word);
					Occur=Words.GetPtr(dic->GetId(stem));
					if(!Occur->GetNbOccurs()) NbDiffWords++;
					Occur->IncOccurs();
				}
			}
		}
	}
	else
	{
		for(tag->Start();!tag->End();tag->Next())
			AnalyseContentTag((*tag)(),stop,dic);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::Analyse(GDocXML* xml,GSession* session) throw(GException)
{
	RXMLTag* content;
	int max,act;
	GDict* stop;
	GDict* dic;
	RContainerCursor<GLang,unsigned int,true,true> CurLang(session->GetLangs());

	cout<<"Analyse: "<<URL()<<endl;
	content=xml->GetContent();
	RAssert(content);

	// Find Language with the maximal number of words of the stoplist contained
	// in the document.
	max=0;
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		act=AnalyseTagForStopKwd(content,dic=session->GetStop(CurLang()));
		if(act>max)
		{
			Lang=CurLang();
			max=act;
			stop=dic;
		}
	}
	if(!Lang) return;

	// Analyse it
	Words.Clear();
	NbWords=NbDiffWords=0;
	AnalyseContentTag(content,stop,session->GetDic(Lang));
	Calc=false;
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::AddWord(const unsigned int id,const unsigned int nb)
{
	Words.InsertPtr(new GIWordOccur(id,nb));
}


//-----------------------------------------------------------------------------
GALILEI::GDoc::~GDoc(void)
{
}




