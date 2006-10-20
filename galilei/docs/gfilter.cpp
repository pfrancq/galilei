/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <fnmatch.h>


//------------------------------------------------------------------------------
// include files for R Library
#include <rxmlfile.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdocxml.h>
#include <gdoc.h>
#include <gfilter.h>
#include <gslot.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GFilter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilter::GFilter(GFactoryFilter* fac)
	: GPlugin<GFactoryFilter>(fac), Doc(0)
{
}


//---------------------------------------------------------------------------
void GFilter::AddMIME(const char* name)
{
	GetFactory()->GetMng()->AddMIME(name,this);
}


//---------------------------------------------------------------------------
void GFilter::AddMIME(RString name)
{
	GetFactory()->GetMng()->AddMIME(name,this);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeBlock(char* block,RXMLTag* attach)
{
	char* ptr;
	char* hold;
	RXMLTag* sent=0;
	int len,i=0;
	int NbWords;
	bool EndSentence;

	// Look at block
	len=strlen(block);
	if(!len) return(attach);

	// Search Sentences
	ptr=block;
	while(*ptr)
	{
		// Skip leading spaces.
		while((*ptr)&&(isspace(*ptr)))
		{
			ptr++;
			len--;
		}
		block=ptr;

		// Seach sentence
		NbWords=0;
		EndSentence=false;

		// If Only 1 word or no space between a punctation and the next
		// word, no sentence created.
		while((*ptr)&&(!EndSentence))
		{
			// While not ending of a sentence, go through
			while((*ptr)&&(!IsEndSentence(*ptr)))
			{
				// If the next characters are spaces, replace them all by a
				// unique space.
				if(isspace(*ptr))
				{
					(*(ptr++))=' ';    // Space character always a ' '.
					i=(--len);
					hold=ptr;
					while((*ptr)&&(isspace(*ptr)))
					{
						len--;
						ptr++;
					}
					if(i>len)      // Look if there more than one space character.
					{
						memmove(hold,ptr,(len+1)*sizeof(char));
						ptr=hold;
					}
					NbWords++;
				}
				else
				{
					ptr++;
					len--;
				}
			}
			// Verify if it is a correct end of sentence
			if((*ptr)&&((NbWords<1)||(!isspace(*(ptr+1)))))
			{
				// Skip the punctation
				ptr++;
				len--;
			}
			else
				EndSentence=true;      // End of sentence.
		}
		if(*ptr)       // Skip Punctuation.
		{
			len--;
			(*(ptr++))=0;
		}

		// Insert sentence
		if(*block)
		{
			// Verify that it is not a single '.'
			if((!ispunct(*block))||(*(block+1)))
			{
				Doc->AddTag(attach,sent=new RXMLTag("docxml:sentence"));
				sent->AddContent(block);
			}
			block=ptr;
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeBlock(RChar* block,RXMLTag* attach)
{
	RChar* ptr;
	RChar* hold;
	RXMLTag* sent=0;
	int len,i=0;
	int NbWords;
	bool EndSentence;

	// Look at block
	len=RChar::StrLen(block);
	if(!len) return(attach);

	// Search Sentences
	ptr=block;
	while(!ptr->IsNull())
	{
		// Skip leading spaces.
		while((!ptr->IsNull())&&(ptr->IsSpace()))
		{
			ptr++;
			len--;
		}
		block=ptr;

		// Seach sentence
		NbWords=0;
		EndSentence=false;

		// If Only 1 word or no space between a punctation and the next
		// word, no sentence created.
		while((!ptr->IsNull())&&(!EndSentence))
		{
			// While not ending of a sentence, go through
			//while((!ptr->IsNull())&&(!ptr->IsPunct()))
			while((!ptr->IsNull())&&(!IsEndSentence(*ptr)))
			{
				// If the next characters are spaces, replace them all by a
				// unique space.
				if(ptr->IsSpace())
				{
					(*(ptr++))=RChar(' ');    // Space character always a ' '.
					i=(--len);
					hold=ptr;
					while((!ptr->IsNull())&&(ptr->IsSpace()))
					{
						len--;
						ptr++;
					}
					if(i>len)      // Look if there more than one space character.
					{
						memcpy(hold,ptr,(len+1)*sizeof(RChar));
						ptr=hold;
					}
					NbWords++;
				}
				else
				{
					ptr++;
					len--;
				}
			}
			// Verify if it is a correct end of sentence
			if((!ptr->IsNull())&&((NbWords<1)||(!(ptr+1)->IsSpace())))
			{
				// Skip the punctation
				ptr++;
				len--;
			}
			else
				EndSentence=true;      // End of sentence.
		}
		if(!ptr->IsNull())       // Skip Punctuation.
		{
			len--;
			(*(ptr++))=0;
		}

		// Insert sentence
		if(!block->IsNull())
		{
			// Verify that it is not a single '.'
			if((!block->IsPunct())||((block+1)->IsNull()))
			{
				Doc->AddTag(attach,sent=new RXMLTag("docxml:sentence"));
				sent->AddContent(block);
			}
			block=ptr;
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeBlock(const RString& block,RXMLTag* attach)
{
	RXMLTag* sent=0;
	int pos;
	int NbWords;
	bool EndSentence;
	RString sentence;
	RCharCursor Cur(block);

	// Look at block
	if(!block.GetLen()) return(attach);

	// Search Sentences
	while(!Cur.End())
	{
		// Skip leading spaces.
		while((!Cur.End())&&(Cur().IsSpace()))
			Cur.Next();

		// Seach sentence
		NbWords=0;
		EndSentence=false;
		pos=Cur.GetPos();

		// If Only 1 word or no space between a punctation and the next
		// word, no sentence created.
		while((!Cur.End())&&(!EndSentence))
		{
			// While not ending of a sentence, go through
			while((!Cur.End())&&(!IsEndSentence(Cur())))
			{
				// If the next characters are spaces, it is a word
				if(Cur().IsSpace())
				{
					// Add the word
					if(!sentence.IsEmpty())
						sentence+=' ';
					sentence+=block.Mid(pos,Cur.GetPos()-pos);

					// Next word can be read
					while((!Cur.End())&&(Cur().IsSpace()))
						Cur.Next();
					NbWords++;
					pos=Cur.GetPos();
				}
				else
					Cur.Next();
			}

			// Verify if it is a correct end of sentence
			if((!Cur.End())&&(Cur.GetPos()+1<Cur.GetNb())&&(!Cur[Cur.GetPos()+1].IsNull())&&/*((NbWords<1)||*/ (!Cur[Cur.GetPos()+1].IsSpace()))
				Cur.Next(); // Skip the punctation
			else
			{
				EndSentence=true;      // End of sentence.
				if(!sentence.IsEmpty())
					sentence+=' ';
				sentence+=block.Mid(pos,Cur.GetPos()-pos);
				pos=Cur.GetPos();
			}
		}

		if(!Cur.End())
		{
			Cur.Next();
			pos=Cur.GetPos();
		}

		// Insert sentence
		if(!sentence.IsEmpty())
		{
			// Verify that it is not a single '.'
			if((!sentence[static_cast<unsigned int>(0)].IsPunct())||(sentence.GetLen()>1))
			{
				Doc->AddTag(attach,sent=new RXMLTag("docxml:sentence"));
				sent->AddContent(sentence);
			}
			sentence.Clear();
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeKeywords(char* list,char sep,RXMLTag* attach)
{
	char* ptr;
	RXMLTag* kwd=0;
	unsigned int len;

	ptr=list;
	while(*ptr)
	{
		// Skip Spaces.
		while((*ptr)&&(isspace(*ptr)))
			ptr++;
		list=ptr;

		// Search the next keywords.
		len=0;
		while((*ptr)&&(!isspace(*ptr))&&((*ptr)!=sep))
		{
			ptr++;
			len++;
		}
		if(len)
		{
			Doc->AddTag(attach,kwd=new RXMLTag("docxml:keyword"));
			if(*ptr)
				(*(ptr++))=0;          // Skip separator.
			kwd->InsertAttr("docxml:value",list);
		}
		else
		{
			ptr++;
			list=ptr;
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeKeywords(RChar* list,RChar sep,RXMLTag* attach)
{
	RChar* ptr;
	RXMLTag* kwd=0;
	unsigned int len;

	ptr=list;
	while(!ptr->IsNull())
	{
		// Skip Spaces.
		while((!ptr->IsNull())&&(ptr->IsSpace()))
			ptr++;
		list=ptr;

		// Search the next keywords.
		len=0;
		while((!ptr->IsNull())&&(ptr->IsSpace())&&((*ptr)!=sep))
		{
			ptr++;
			len++;
		}
		if(len)
		{
			Doc->AddTag(attach,kwd=new RXMLTag("docxml:keyword"));
			if(!ptr->IsNull())
				(*(ptr++))=0;          // Skip separator.
			kwd->InsertAttr("docxml:value",list);
		}
		else
		{
			ptr++;
			list=ptr;
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
R::RXMLTag* GFilter::AnalyzeKeywords(const RString& list,RChar sep,RXMLTag* attach)
{
	RCharCursor Cur;
	RXMLTag* kwd;
	unsigned int pos;

	Cur.Set(list);
	while(!Cur.End())
	{
		// Skip Spaces.
		while((!Cur.End())&&(Cur().IsSpace()))
			Cur.Next();

		// Search the next keywords.
		pos=Cur.GetPos();
		while((!Cur.End())&&(!Cur().IsSpace())&&(Cur()!=sep))
			Cur.Next();
		if(Cur.GetPos()-pos)
		{
			Doc->AddTag(attach,kwd=new RXMLTag("docxml:keyword"));
			kwd->InsertAttr("docxml:value",list.Mid(pos,Cur.GetPos()-pos));
		}
		else
		{
			if(!Cur.End())
				Cur.Next();
		}
	}
	return(attach);
}


//------------------------------------------------------------------------------
GFilter::~GFilter(void)
{
}



//------------------------------------------------------------------------------
//
// class GFilterManager::GMIMEFilter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFilterManager::GMIMEFilter
{
public:
	RString Name;
	GFilter* Filter;

	GMIMEFilter(const char* n,GFilter* f) : Name(n), Filter(f) {}
	int Compare(const GMIMEFilter* f) const {return(Name.Compare(f->Name));}
	int Compare(const GMIMEFilter& f) const {return(Name.Compare(f.Name));}
	int Compare(const R::RString& t) const {return(Name.Compare(t));}
	int Compare(const char* t) const {return(Name.Compare(t));}
};



//------------------------------------------------------------------------------
//
// class GFilterManager::GMIMEExt
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFilterManager::GMIMEExt
{
public:
	RString Name;
	RString Ext;

	GMIMEExt(const RString& n,const RString& e) : Name(n), Ext(e) {}
	int Compare(const GMIMEExt* f) const {return(Name.Compare(f->Ext));}
	int Compare(const GMIMEExt& f) const {return(Name.Compare(f.Ext));}
	int Compare(const R::RString& e) const {return(Name.Compare(e));}
	int Compare(const char* e) const {return(Name.Compare(e));}
};



//------------------------------------------------------------------------------
//
// class GFilterManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterManager::GFilterManager(void)
	: GPluginManager<GFilterManager,GFactoryFilter,GFilter>("Filter",API_FILTER_VERSION,ptList), MIMES(50,25),
	  Exts(50,25)
{
	// Try to open list of MIME types
	try
	{
		RString MIME;
		RXMLStruct xml;
		RXMLFile File("/etc/galilei/galilei.mimes",&xml);
		R::RCursor<RXMLTag> Cur,Cur2;

		File.Open(R::RIO::Read);
		// Go trough all MIME types
		RXMLTag* Types=xml.GetTag("mimeTypes");
		if(!Types)
			throw GException("MIME type file \"/etc/galilei/galilei.mimes\" is invalid");
		Cur=Types->GetNodes();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			MIME=Cur()->GetAttrValue("code");

			// Go through all file extension
			Cur2=Cur()->GetNodes();
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				Exts.InsertPtr(new GMIMEExt(MIME,Cur2()->GetAttrValue("ext")));
			}
		}
	}
	catch(...)
	{
	}
}


//------------------------------------------------------------------------------
void GFilterManager::Download(const char*,RString&)
{
}


//------------------------------------------------------------------------------
const char* GFilterManager::DetermineMIMEType(const char* tmpfile)
{
	RCursor<GMIMEExt> Cur(Exts);

	// Go through each extension
	for(Cur.Start();!Cur.End();Cur.Next())
		if(fnmatch(Cur()->Ext.Latin1(),tmpfile,0)!=FNM_NOMATCH)
			return(Cur()->Name.Latin1());
	return(0);
}


//------------------------------------------------------------------------------
void GFilterManager::Delete(RString&)
{
}


//------------------------------------------------------------------------------
GDocXML* GFilterManager::CreateDocXML(GDoc* doc,GSlot* slot)
{
	RString tmpFile(250);
	char tmp[250];
	const char* ptr;
	int i;
	GDocXML* xml=0;
	bool Dwn;
	bool Url;
	GMIMEFilter* f=0;
	RString mime;

	// Look for the protocol
	ptr=doc->GetURL();
	i=0;
	while((*ptr)&&(isalnum(*ptr)))
	{
		i++;
		ptr++;
	}

	// If ':' find -> it is an URL
	if(i)
		Url=((*ptr)==':');
	else
		Url=false;

	// if URL and protocol different than 'file' -> Download it
	if(Url&&strncasecmp(doc->GetURL(),"file",i))
	{

		// if the download can't be done an error is then send
		try
		{
			Download(doc->GetURL(),tmpFile);
		}
		catch(GException& e)
		{
			if(!slot) // If not slot -> forward error as GException
				throw GException(e.GetMsg());
			slot->WriteStr(e.GetMsg());
			return(0);
		}
		Dwn=true;
	}
	else
	{
		// If URL skip 'file:'
		if(Url)
		{
			strcpy(tmp,doc->GetURL());
			// Move the all string (include the ending 0) from 5 characters.
			memcpy(tmp,&tmp[5],strlen(tmp)-4);
			tmpFile=tmp;
		}
		else
			tmpFile=doc->GetURL();
		Dwn=false;
	}

	// Verify if the MIME type is defined -> if not try to guess
	mime=doc->GetMIMEType();
	if(mime.IsEmpty())
	{
		mime=DetermineMIMEType(tmpFile);
		if(!mime.IsEmpty())
			doc->SetMIMEType(mime);
	}

	// Create a DocXML.
	xml=new GDocXML(doc->GetURL(),tmpFile);

	// If MIME type defined -> analyze it.
	if(!mime.IsEmpty())
	{
		f=MIMES.GetPtr<const char*>(mime);
		if(f)
			f->Filter->Analyze(xml);
		xml->AddFormat(mime);
	}

	// Delete it
	if(Dwn)
		Delete(tmpFile);

	// Return XML structure
	return(xml);
}


//------------------------------------------------------------------------------
void GFilterManager::AddMIME(const char* mime,GFilter* f)
{
	MIMES.InsertPtr(new GMIMEFilter(mime,f));
}


//------------------------------------------------------------------------------
void GFilterManager::DelMIMES(GFilter* f)
{
	RContainer<GMIMEFilter,false,false> Rem(5,5);

	// Find All MIMES types to deleted
	RCursor<GMIMEFilter> Cur(MIMES);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Filter==f)
			Rem.InsertPtr(Cur());
	}

	// Delete all MIMES
	Cur.Set(Rem);
	for(Cur.Start();!Cur.End();Cur.Next())
		MIMES.DeletePtr(Cur());
}


//------------------------------------------------------------------------------
const char* GFilterManager::GetMIMEType(const char* mime) const
{
	GMIMEFilter* fil;

	if(!mime) return(0);
	fil=MIMES.GetPtr<const char*>(mime);
	if(!fil) return(0);
	return(fil->Name);
}


//------------------------------------------------------------------------------
GFilterManager::~GFilterManager(void)
{
}
