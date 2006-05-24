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
#include <rdate.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gtextanalyse.h>
#include <gdoc.h>
#include <gdocxml.h>
#include <gword.h>
#include <gweightinfo.h>
#include <gdict.h>
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
// class Word
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GTextAnalyse::WordWeight
{
public:
	RString Word;
	bool* InStop;
	unsigned int Nb;
	double Weight;
	bool OnlyLetters;

	WordWeight(unsigned int nb);
	inline void Clear(void) {Word=""; Nb=0; Weight=0.0;}

	int Compare(const WordWeight& word) const
		{return(Word.Compare(word.Word));}
	int Compare(const WordWeight* word) const
		{return(Word.Compare(word->Word));}
	int Compare(const RString& word) const
		{return(Word.Compare(word));}

	static char HashIndex(const WordWeight* w)
		{return(RString::HashIndex(w->Word));}
	static char HashIndex(const WordWeight& w)
		{return(RString::HashIndex(w.Word));}
	static char HashIndex(const RString& word)
		{return(RString::HashIndex(word));}

	static char HashIndex2(const WordWeight* w)
		{return(RString::HashIndex2(w->Word));}
	static char HashIndex2(const WordWeight& w)
		{return(RString::HashIndex2(w.Word));}
	static char HashIndex2(const RString& word)
		{return(RString::HashIndex2(word));}

	~WordWeight(void);
};


//-----------------------------------------------------------------------------
GTextAnalyse::WordWeight::WordWeight(unsigned int nb)
	:  Word(MaxWordLen+1), InStop(0)
{
	InStop=new bool[nb];
	Clear();
}


//-----------------------------------------------------------------------------
GTextAnalyse::WordWeight::~WordWeight(void)
{
	if(InStop) delete[] InStop;
}



//-----------------------------------------------------------------------------
//
// class GTextAnalyse
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GTextAnalyse::GTextAnalyse(GFactoryDocAnalyse* fac)
	: GDocAnalyse(fac), Weights(0), Infos(5000,2500), Direct(0), NbDirect(5000),
	  Order(0), NbOrder(5000), Sl(0), Sldiff(0), Lang(0)
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
	Distance=Factory->GetBool("Distance");
	UseExternalLinks=Factory->GetBool("UseExternalLinks");
	Filtering=Factory->GetBool("Filtering");
	NbSameOccur=Factory->GetUInt("NbSameOccur");
	NormalRatio=Factory->GetDouble("NormalRatio");
	PathtoBinary=Factory->Get("PathtoBinary");
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Connect(GSession* session)
{
	WordWeight** ptr;
	GWord** pt;
	unsigned int i;

	// First init
	if(Session) return;
	GDocAnalyse::Connect(session);

	// Create local structures
	CurLangs=GALILEIApp->GetManager<GLangManager>("Lang")->GetFactories();
	Sl=new unsigned int[CurLangs.GetNb()];
	Sldiff=new unsigned int[CurLangs.GetNb()];
	Weights=new RDblHashContainer<WordWeight,27,27,false>(500,250);
	Direct=new WordWeight*[NbDirect];
	for(i=NbDirect+1,ptr=Direct;--i;ptr++)
		(*ptr)=new WordWeight(CurLangs.GetNb());
	Order=new GWord*[NbOrder];
	for(i=NbOrder+1,pt=Order;--i;pt++)
		(*pt)=new GWord();

	//init database
	if(StoreFullWords)
		Session->GetStorage()->CreateDummy("wordsstems");

}


//-----------------------------------------------------------------------------
void GTextAnalyse::Disconnect(GSession* session)
{
	WordWeight** ptr;
	GWord** pt;
	unsigned int i;

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
	GWord** pt;
	unsigned int i;

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
	GWord** ptr;

	if(NbOrder==Nwords)
	{
		ptr=new GWord*[NbOrder+2500];
		memcpy(ptr,Order,NbOrder*sizeof(GWord*));
		delete[] Order;
		Order=ptr;
		for(i=2500+1,ptr=&Order[NbOrder];--i;ptr++)
			(*ptr)=new GWord();
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
void GTextAnalyse::AddWord(const RString word,double weight)
{
	bool Find;
	unsigned int Index;
	WordWeight* w;
	RContainer<WordWeight,false,true>* Section;
	unsigned int i;
	bool *is;
	unsigned int* tmp1;
	unsigned int* tmp2;
	GLang* lang;

	// If word not valid, skip it
	if((Filtering)&&(!ValidWord(word))) return;

	// Find the section of double hash table concerned by the current word.
	//Section=Weights->Hash[WordWeight::HashIndex(word)][WordWeight::HashIndex2(word)];
	Section=(*(*Weights)[WordWeight::HashIndex(word)])[WordWeight::HashIndex2(word)];

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
				lang=CurLangs()->GetPlugin();
				if(!lang) continue;
				(*is)=lang->InStop(word);
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
			for(i=GALILEIApp->GetManager<GLangManager>("Lang")->GetNbFactories()+1,is=w->InStop,tmp2=Sl;--i;is++,tmp2++)
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
	if(Distance)
	{
		// Bug!!!!!!!!!!!
		// if(!w->InStop[LangIndex])
		{
			VerifyOrder();
			delete(Order[Nwords]);
			Order[Nwords++]=new GWord(word);
			//Nwords++;
			if(OnlyLetters)
				Order[Nwords-1]->SetId(1);
			else
				Order[Nwords-1]->SetId(0);
		}
	}
	N++;
	w->Nb++;
	w->Weight+=weight;
}


//-----------------------------------------------------------------------------
bool GTextAnalyse::ExtractWord(const RChar* &ptr/*,RString& word*/,double weight)
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

	// If len null, return (nothing else to extract)
	if(!len) return(false);

	// if not only letters and non-letter words not enabled -> extract next word.
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
	AddWord(word.ToLower(),weight);
	return(true);
}


//-----------------------------------------------------------------------------
void GTextAnalyse::AnalyseTag(RXMLTag* tag,double weight)
{
	const RChar* ptr;

	if(tag->GetName()=="docxml:sentence")
	{
		ptr=tag->GetContent()();
		while(!ptr->IsNull())
			ExtractWord(ptr,weight);
	}
	else
	{
		RCursor<RXMLTag> Cur(tag->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
			AnalyseTag(Cur(),weight);
	}
}


////-----------------------------------------------------------------------------
//void GTextAnalyse::AnalyseLinksTag(RXMLTag* tag,bool externalLinks ,RContainer<GDoc,unsigned int,false,true>* DocsToAdd)
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
//#pragma warn "ici il faut rajouter les proprietes des liens.";
//}


//-----------------------------------------------------------------------------
void GTextAnalyse::AnalyseLinksTag(RXMLTag* tag,bool externalLinks ,RContainer<GDoc,false,true>* DocsToAdd)
{
//	const char* ptr;
//	const char* endPtr;
	RString url;
	const RChar* ptr;
	GDoc* tmpDoc=0;
	bool bUrl;

//	endPtr=ptr=0;
	bUrl=false;

	if (tag->GetName()== "docxml:metaData")
	{
		bUrl=false;

//		type=0;
//		format=0;
		RCursor<RXMLTag> Cur(tag->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if (Cur()->GetName()=="dc:identifier")
			{
				bUrl=true;
				url=Cur()->GetContent();
			}
			if(Cur()->GetName()=="dc:format")
			{
				//format=(*tag)()->GetContent();
			}
			if(Cur()->GetName()=="dc:type")
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
			ptr=url();
			// keep only html links (-> whith html extension)
			while(!ptr->IsNull())
			{
				ptr++;
			}
			while(ptr->Latin1()!='.')
			{
				ptr--;
			}
			if((!RChar::StrCmp(ptr,".html"))||(!RChar::StrCmp(ptr,".htm")))
			{
				tmpDoc = Session->GetDoc(url);   //where ptr is the url.
				if(!tmpDoc)
				{
					if(externalLinks)
					{
						tmpDoc=new GDoc(url,url,cNoRef,0,"text/html",RDate(""),RDate(""),dsToAnalyse,cNoRef);
						Session->AssignId(tmpDoc);
						DocsToAdd->InsertPtr(tmpDoc);

						tmpDoc->SetState(osNotNeeded);
					}
				}
				else
				{
					Doc->InsertLink(tmpDoc);
				}
			}
		}
	}
	else
	{
		RCursor<RXMLTag> Cur(tag->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
			AnalyseLinksTag(Cur(),externalLinks,DocsToAdd);
	}
	#warning Add properties of links
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
		if(!CurLangs()->GetPlugin()) continue;
		Frac=((double)(*tmp1))/((double)Ndiff);
		if(((*tmp2)>S)&&(Frac>=MinFrac))
		{
			Lang=CurLangs()->GetPlugin();
			S=(*tmp2);
			Sdiff=(*tmp1);
			LangIndex=i;
		}
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyse::ConstructInfos(unsigned int docid)
{
	WordWeight** wrd;
	GWeightInfo* Occur;
	unsigned int i;
	RString stem(MaxWordLen);
	GDict* dic;

	// Insert all the occurences of the valid words
	dic=Lang->GetDict();

	for(i=Ndiff+1,wrd=Direct;--i;wrd++)
	{
		// If Stop list -> do not treat it.
		if((*wrd)->InStop[LangIndex]) continue;

		// if len<MinWordSize -> do not treat it.
		if((*wrd)->Word.GetLen()<MinWordSize) continue;

		// If only letters -> apply stemming algorithm.
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
		if(stem.GetLen()>=MinStemSize)
		{
			GWord w(stem);
			Occur=Infos.GetInsertPtr(dic->InsertData(&w));
			if(!Occur->GetWeight())
				Vdiff++;
			V+=(*wrd)->Nb;
			(*Occur)+=(*wrd)->Weight;
			if(StoreFullWords)
				StoreWordStemInDatabase(Occur->GetId(), (*wrd)->Word, docid);

		}
	}

	// Save the order of appearance of the valid words of the document in a binary file.
	if(Distance)
	{
		DIR* dp;
		RString name;
		name=PathtoBinary;
		dp=opendir(name);
		if(!dp)
			throw GException("the specified path doesn't exist.");
		closedir(dp);
		name+=Session->GetStorage()->GetFactory()->GetName();
		dp=opendir(name);
		if(!dp)
			mkdir(name,448);
		closedir(dp);
		name+="/DocumentStuct";
		dp=opendir(name);
		if(!dp)
			mkdir(name,448);
		closedir(dp);
		name+="/Doc";
		name+=RString::Number(Doc->GetId());
		try
		{
			R::RRecFile<GWord,sizeof(unsigned int),false> f(name);
			f.Open(R::RIO::Create);
			for(i=0;i<Nwords;i++)
			{
				if(Order[i]->GetId()==1)
				{
					stem=Lang->GetStemming(Order[i]->GetName());
					if(stem.GetLen()>=MinStemSize)
					{
						GWord w(stem);
						f<<dic->InsertData(&w);
					}
				}
				else
				{
					stem=(Order[i]->GetName());
					GWord w(stem);
					f<<dic->InsertData(&w);
				}
			}
		}
		catch (...)
		{
			throw GException("Cannot create binary files");
		}
	}

	// Verify that each occurences is not under the minimal.
	if(MinOccur<2) return;

#warning Delete pointer from container with a cursor.
	RCursor<GWeightInfo> Cur(Infos);
	for(Cur.Start();!Cur.End();)
	{
		if(Cur()->GetWeight()<MinOccur)
		{
			Infos.DeletePtr(*Cur());
		}
		else
			Cur.Next();
	}
}


//-----------------------------------------------------------------------------
void GTextAnalyse::Analyze(GDocXML* xml,GDoc* doc,RContainer<GDoc,false,true>* tmpDocs)
{
	RXMLTag* content;
	RXMLTag* metadata;
	RXMLTag* link;
	RXMLTag* Title;
	RString Name;
	RCursor<RXMLTag> Tags;

	// Init Part and verification
	Doc=doc;
	if(!xml)
		throw GException("No XML Structure for document '"+Doc->GetURL()+"'");

	Lang=Doc->GetLang();
	FindLang=((!Lang)||(!StaticLang));
	content=xml->GetContent();
	RAssert(content);
	metadata=xml->GetMetaData();
	RAssert(metadata);
	Clear();

	// Analyse the doc structure.
	if(!FindLang)
	{
		// if Language defined -> Compute LangIndex
		for(CurLangs.Start(),LangIndex=0;CurLangs()->GetPlugin()!=Lang;CurLangs.Next(),LangIndex++);
	}
	AnalyseTag(metadata,2.0);
	AnalyseTag(content,1.0);

	// Look if a title meta is defined
	Title=metadata->GetNode("dc:title");
	if(Title)
	{
		Name="";
		Tags=Title->GetNodes();
		for(Tags.Start();!Tags.End();Tags.Next())
		{
			if(Tags()->GetName()!="docxml:sentence") continue;
			Name+=Tags()->GetContent();
		}
		if(!Name.IsEmpty())
			Doc->SetName(Name);
	}

	// Analyse the content of link tags
	link = xml->GetLinks ();
	RAssert(link);
	if(tmpDocs&&UseExternalLinks)
		AnalyseLinksTag(link,UseExternalLinks,tmpDocs);

	// Determine the Language if necessary.
	if(FindLang)
		DetermineLang();

	// Construct Information if languages determined.
	if(Lang)
		ConstructInfos(doc->GetId());

	// Set the Variable of the document
	Doc->Update(Lang,&Infos,true);
}


//------------------------------------------------------------------------------
bool GTextAnalyse::StoreWordStemInDatabase(unsigned int stemid, RString word, unsigned int docid)
{
	//check if the words/stem couple does not already exist
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
	params->InsertParam(new RParamValue("Distance",false));
	params->InsertParam(new RParamValue("UseExternalLinks",false));
	params->InsertParam(new RParamValue("Filtering",true));
	params->InsertParam(new RParamValue("NbSameOccur",3));
	params->InsertParam(new RParamValue("NormalRatio",0.3));
	params->InsertParam(new RParamValue("PathtoBinary","/var/galilei/bin/"));
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

