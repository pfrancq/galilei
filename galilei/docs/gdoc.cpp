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
GALILEI::GDoc::GDoc (const RString& url,const RStd::RString& name,const unsigned int id,const unsigned int nbdiff) throw(bad_alloc)
  : URL(url), Name(name), Id(id),Words(nbdiff),NbWords(0),NbDiffWords(), Lang(0),
    Calc(true), bSave(false)
{
}


//-----------------------------------------------------------------------------
GALILEI::GDoc::GDoc(const RString& url,const RStd::RString& name,const unsigned int id,GLang* lang,const unsigned int nb,const unsigned int nbdiff) throw(bad_alloc)
	: URL(url), Name(name), Id(id), Words(nbdiff),NbWords(nb), NbDiffWords(nbdiff),
	  Lang(lang), Calc(true), bSave(false)
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
	unsigned len=0;
	const char* begin;
	bool Cont=true;

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
			len++;
			ptr++;
		}
		Cont=false;   // Normally, end of the word.
		if(!(*ptr)) continue;

		// If the next character is in {-'./\@} and no blank space after,
		// then continue the word.
		if((strchr("\'-.@\\/",*ptr))&&(isalnum(*(ptr+1))))
		{
			Cont=true;
			ptr++;
			len++;
		}
	}

	// If len is not null, copy result in word and return true, else return
	// false.
	if(!len) return(false);
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
	RString word(50);
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
					Occur=Words.GetPtr(dic->GetId(word));
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
	cout<<"Nb Words: "<<AnalyseTagForStopKwd(content,0)<<endl;
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




