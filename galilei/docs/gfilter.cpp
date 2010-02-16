/*

	GALILEI Research Project

	GFilter.cpp

	Generic Filter for documents - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
// include files for GALILEI
#include <ggalileiapp.h>
#include <gfilter.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GFilter::MetaData
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFilter::MetaData
{
public:

	RString Name;
	RString Value;
	MetaDataType Type;
	RChar Sep;

	MetaData(const RString& name,const RString& val,MetaDataType type,RChar sep) : Name(name), Value(val), Type(type), Sep(sep) {}
	int Compare(const MetaData&) const {return(-1);}
};



//------------------------------------------------------------------------------
//
// class GFilter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilter::GFilter(GPlugInFactory* fac)
	: GPlugIn(fac), MetaDatas(20)
{
}


//---------------------------------------------------------------------------
void GFilter::AddMIME(const char* name)
{
	GALILEIApp->AddMIME(name,this);
}


//---------------------------------------------------------------------------
void GFilter::AddMIME(RString name)
{
	GALILEIApp->AddMIME(name,this);
}


//------------------------------------------------------------------------------
void GFilter::AnalyzeBlock(const RString& block,R::RXMLParser* parser)
{
	size_t pos;
	int NbWords;
	bool EndSentence;
	RString sentence;
	RCharCursor Cur(block);

	// Search Sentences
	while(!Cur.End())
	{
		// Skip leading spaces.
		while((!Cur.End())&&(Cur().IsSpace()))
			Cur.Next();

		// Search sentence
		NbWords=0;
		EndSentence=false;
		pos=Cur.GetPos();

		// If Only 1 word or no space between a punctuation and the next
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
				Cur.Next(); // Skip the punctuation
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
			if((!sentence[static_cast<size_t>(0)].IsPunct())||(sentence.GetLen()>1))
			{
				parser->BeginTag("http://www.otlet-institute.org/docxml","sentence","docxml:sentence");
				parser->Text(sentence);
				parser->EndTag("http://www.otlet-institute.org/docxml","sentence","docxml:sentence");
			}
			sentence.Clear();
		}
	}
}


//------------------------------------------------------------------------------
void GFilter::AnalyzeKeywords(const RString& list,RChar sep,RXMLParser* parser)
{
	RCharCursor Cur;
	size_t pos;

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
			parser->BeginTag("http://www.otlet-institute.org/docxml","keyword","docxml:keyword");
			parser->AddAttribute("http://www.otlet-institute.org/docxml","value","docxml:value");
			parser->Value(list.Mid(pos,Cur.GetPos()-pos));
			parser->EndTag("http://www.otlet-institute.org/docxml","keyword","docxml:keyword");
		}
		else
		{
			if(!Cur.End())
				Cur.Next();
		}
	}
}


//------------------------------------------------------------------------------
void GFilter::Clear(R::RXMLParser* parser)
{
	MetaDatas.Clear();
	parser->InitParser();
}

//------------------------------------------------------------------------------
void GFilter::StartStream(R::RXMLParser* parser)
{
	parser->ResetDepth();
	parser->SetSection(RXMLParser::Header);
	parser->AddEntity("xmlns:rdf","http://www.w3.org/1999/02/22-rdf-syntax-ns#");
	parser->AddEntity("xmlns:dc","http://purl.org/dc/elements/1.1");
	parser->AddEntity("xmlns:docxml","http://www.otlet-institute.org/docxml");
	parser->SetSection(RXMLParser::DOCTYPE);
	parser->SetDocType("rdf:RDF");
	parser->BeginTag("http://www.w3.org/1999/02/22-rdf-syntax-ns#","RDF","rdf:RDF");
	parser->ResetDepth();
	parser->SetSection(RXMLParser::Body);
}


//------------------------------------------------------------------------------
void GFilter::AddDublinCoreMetaData(const R::RString& name,const R::RString& value,MetaDataType type,R::RChar sep)
{
	MetaDatas.InsertPtr(new MetaData(name,value,type,sep));
}


//------------------------------------------------------------------------------
void GFilter::WriteMetaDataStream(R::RXMLParser* parser)
{
	if(!MetaDatas.GetNb())
		return;
	parser->BeginTag("http://www.otlet-institute.org/docxml","metaData","docxml:metaData");
	RCursor<MetaData> Cur(MetaDatas);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		parser->BeginTag("http://purl.org/dc/elements/1.1",Cur()->Name,"dc:"+Cur()->Name);
		switch(Cur()->Type)
		{
			case RawText:
				parser->Text(Cur()->Value);
				break;
			case Sentence:
				AnalyzeBlock(Cur()->Value,parser);
				break;
			case Keywords:
				AnalyzeKeywords(Cur()->Value,Cur()->Sep,parser);
				break;
		}
		parser->EndTag("http://purl.org/dc/elements/1.1",Cur()->Name,"dc:"+Cur()->Name);
	}
	parser->EndTag("http://www.otlet-institute.org/docxml","metaData","docxml:metaData");
	parser->BeginTag("http://www.otlet-institute.org/docxml","content","docxml:content");
}


//------------------------------------------------------------------------------
void GFilter::StartParagraph(R::RXMLParser* parser)
{
	parser->BeginTag("http://www.otlet-institute.org/docxml","p","docxml:p");
}


//------------------------------------------------------------------------------
void GFilter::EndParagraph(R::RXMLParser* parser)
{
	parser->EndTag("http://www.otlet-institute.org/docxml","p","docxml:p");
}


//------------------------------------------------------------------------------
void GFilter::ExtractTextualContent(RTextFile& file,RXMLParser* parser,const RString& text)
{
	RString Line,NextLine;
	bool Read(text.IsEmpty());

	while(!file.End())
	{
		StartParagraph(parser);

		if(Read)
		{
			if(!file.End())
				Line=NextLine;
		}
		else
		{
			Line=text;
			Read=true;
		}

		// Paragraph are supposed to be terminated by at least one blank line
		bool Paragraph(true);
		while((!file.End())&&Paragraph)
		{
			// Read a Line
			NextLine=file.GetLine(false);

			// Look if it is a blank line
			RChar* ptr(NextLine());
			while((!ptr->IsNull())&&(ptr->IsSpace()))
				ptr++;

			// If blank line -> it is the end of a paragraph
			if(ptr->IsNull())
			{
				Paragraph=false;
			}
			else
			{
				if(!Line.IsEmpty())
					Line+=' ';
				Line+=NextLine;
			}
		}
		AnalyzeBlock(Line,parser);
		EndParagraph(parser);
	}
}


//------------------------------------------------------------------------------
void GFilter::EndStream(R::RXMLParser* parser)
{
	parser->EndTag("http://www.otlet-institute.org/docxml","content","docxml:content");
	parser->EndTag("http://www.w3.org/1999/02/22-rdf-syntax-ns#","RDF","rdf:RDF");
}


//------------------------------------------------------------------------------
GFilter::~GFilter(void)
{
}
