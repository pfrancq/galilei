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
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocs/gdoc.h>
#include <gdocs/gdocxml.h>
#include <gsessions/gsession.h>
using namespace GALILEI;
using namespace RXML;
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class GDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDoc::GDoc(const char* url,const char* name,unsigned int id,GLang* lang,GMIMEFilter* t,const char* u,const char* a,unsigned int f,unsigned int nb,unsigned int nbdiff,unsigned int nbf) throw(bad_alloc)
	: URL(url), Name(name), Id(id), Words(nbdiff>300?nbdiff:300),NbWords(nb), NbDiffWords(nbdiff),
	  Lang(lang), Type(t), Updated(u), Computed(a), Fdbks(nbf+nbf/2,nbf/2), Failed(f)
{
	if(Updated>Computed)
	{
		if(Computed==RDate::null)
			State=osCreated;
		else
			State=osModified;
	}
	else
		State=osUpToDate;
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

	// if len=1, extract next word.
	if(len==1)
	{
		Letter=false;
		goto BeginExtract;
	}

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
			if((ExtractWord(ptr,word))&&(stop->IsIn<const char*>(word())))
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
				if(!stop->IsIn<const char*>(word()))
				{
					stem=Lang->GetStemming(word);
					if(stem.GetLen()>=2)
					{
						Occur=Words.GetPtr(dic->GetId(stem));
						if(!Occur->GetNbOccurs()) NbDiffWords++;
						Occur->IncOccurs();
					}
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

	// Verify that xml structure exists.
	if(!xml)
		throw GException("No XML Structure for document '"+URL+"'");

	if((State==osUpToDate)||(State==osUpdated)) return;

	RContainerCursor<GLang,unsigned int,true,true> CurLang(session->GetLangs());

	content=xml->GetContent();
	RAssert(content);

	// Find Language with the maximal number of words of the stoplist contained
	// in the document.
	if(Lang&&session->IsStaticLang())
	{
		stop=session->GetStop(Lang);
	}
	else
	{
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
	}

	// Analyse it
	Words.Clear();
	NbWords=NbDiffWords=0;
	AnalyseContentTag(content,stop,session->GetDic(Lang));

	// Make it 'Updated' and tell all the profiles that have judge this
	// document that they are 'Modified'.
	State=osUpdated;
	Computed.SetToday();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		Fdbks()->GetProfile()->SetState(osModified);
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




