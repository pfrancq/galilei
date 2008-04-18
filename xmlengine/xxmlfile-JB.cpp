
/***************************************************************************
 *   Copyright (C) 2005 by Jean-Baptiste Valsamis                          *
 *   jvalsami@@ulb.ac.be                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "xxmlfile.h"

XXMLFile::XXMLFile(const RString &name, RXMLStruct *xmlstruct, const RString &encoding)
: RXMLFile(name, xmlstruct, encoding)
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XXMLFile::XXMLFile(const RString &name, RXMLStruct &xmlstruct, const RString &encoding)
: RXMLFile(name, xmlstruct, encoding)
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XXMLFile::XXMLFile(RIOFile &file, RXMLStruct *xmlstruct, const RString &encoding)
: RXMLFile(file, xmlstruct, encoding)
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XXMLFile::XXMLFile(RIOFile &file, RXMLStruct &xmlstruct, const RString &encoding)
: RXMLFile(file, xmlstruct, encoding)
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XXMLFile::Open(RIO::ModeType mode)
{
	RTextFile::Open(mode);
	switch (Mode)
	{
		case RIO::Read :
			load_header();
			load_next_tag();
			break;
		default :
			RXMLFile::Open(mode);
	};
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XXMLFile::load_header()
{
	RString Content;
	RContainer<RXMLAttr, false, true> Attrs(10);
	RXMLAttr* Attr;
	RCursor<RXMLAttr> curs_attrs;
	
	// Skip Spaces and comments
	SkipSpaces();
	// Search after "<? xml"
	if (CurString("<?"))
	{
		// Skip <?
		Next(); Next();

		// Search for parameters until ?> is found
		load_attributes(Attrs, '?', '>');
		if ((Attr=Attrs.GetPtr<const char*>("version")))
			XMLStruct->SetVersion(Attr->GetValue());
		if ((Attr=Attrs.GetPtr<const char*>("encoding")))
			SetEncoding(Attr->GetValue());

		// Skip ?>
		Next(); Next();
	}
	curs_attrs.Set(Attrs);
	for (curs_attrs.Start(); !curs_attrs.End(); curs_attrs.Next())
		delete curs_attrs();
	// Skip Spaces
	SkipSpaces();

	// Search after "!DOCTYPE"
	if (CurString("<!"))
	{
		Next(); Next(); // Skip <?
		if (!CurString("DOCTYPE", false))
{
//FIXME			throw RIOException(this, "Wrong DOCTYPE command");
cout << "Wrong DOCTYPE command" << endl;
return;
}

		// Skip DOCTYPE
		for (int i = 8; --i;)
			Next();

		// Read the DocType
		SkipSpaces();
		while ((!Cur.IsNull()) && (Cur != RChar('>')) && (!Cur.IsSpace()))
		{
			Content += Cur;
			Next();
		}
		SetDocType(XMLToString(Content));
		while ((!Cur.IsNull()) && (Cur != RChar('>')))
			Next();
		Next();
	}

	// Skip Spaces and comments
	SkipSpaces();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XXMLFile::load_next_tag()
{
	RString attrn,attrv;
	RString TagName;
	RString Contains;
	RContainer<RXMLAttr, false, true> Attrs(10);
	RChar What;
	RString Code;
	bool CDATA;
	unsigned int bytepos;

	if ((Cur != RChar('<')) || (GetNextCur() == RChar('/')))
{
cout << "Not a tag" << endl;
//FIXME throw eInvalidXMLFile(this, "Not a tag");
}

	// Read name of the tag
	Next(); // Skip <
	SkipSpaces();
	while ((!Cur.IsNull()) && (!Cur.IsSpace()) && (Cur != RChar('>')) && (Cur != RChar('/')))
	{
		TagName += Cur;
		Next();
	}
	SkipSpaces();

	// Read Attributes
	load_attributes(Attrs);

	// It is a closing tag?
	CurTagClosing = (Cur == RChar('/'));
	if (CurTagClosing)
		Next();

	// Treat the tag
	begin_tag(TagName, Attrs);
	// Verify if it has sub-tags
	if (CurTagClosing)
	{
		end_tag(TagName);
		Next(); // Skip >
		SkipSpaces();
		return;
	}
	// Treat sub-tags
	Next();
	SkipSpaces();

	while ((!Cur.IsNull()) && ((Cur != RChar('<')) || (GetNextCur() != RChar('/'))))
	{
		if((Cur == RChar('<')) && (GetNextCur() != RChar('/')))
		{
			// It is a tag -> read it
			load_next_tag();
		}
		else
		{
			// It is content -> read it as long as there is no open tag.
			bytepos = TotalLen - Len;
			Contains.Clear();
			CDATA = true; // Suppose that first '<' found is a "<![CDATA["
			while (CDATA)
			{
				// Read text until '<'
				while ((!Cur.IsNull()) && (Cur != RChar('<')))
					add_next_char(Contains);

				// Look if the next '<' is the beginning of "<![CDATA["
				CDATA = CurString("<![CDATA[",true);
				if (CDATA)
				{
					// Skip <![CDATA[
					for (int i = 10; --i;)
						Next();

					// Read until ']]>' is found
					while ((!Cur.IsNull()) && (!CurString("]]>")))
						add_next_char(Contains);

					// Skip ]]>
					for (int i = 4; --i;)
						Next();
				}
			}
			Contains = Contains.Trim();
			Text(XMLToString(Contains));
			XCurTag = dynamic_cast<XXMLTag *> (CurTag);
			XCurTag->SetByte(bytepos, Contains.GetLen());
			SkipSpaces();
		}
	}
	// Read the close tag
	CurTagClosing = true;
	Next(); Next();  // Normal character
	TagName.Clear();
	SkipSpaces();
	while ((!Cur.IsNull()) && (!Cur.IsSpace()) && (Cur!=RChar('>')))
	{
		TagName += Cur;
		Next();
	}
	SkipSpaces();
	Next(); // Skip >
	end_tag(TagName);
	SkipSpaces();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XXMLFile::load_attributes(RContainer<RXMLAttr, false, true> &attrs, RChar EndTag1, RChar EndTag2)
{
	RString attrn, attrv;
	RChar What;
	bool Quotes;
	unsigned int bytepos;
	XXMLAttr *attrtmp;
	
	while ((!Cur.IsNull()) && (Cur!=EndTag1) && (Cur!=EndTag2))
	{
		// Read the Name
		attrn.Clear();
		while ((!Cur.IsNull()) && (!Cur.IsSpace()) && (Cur != RChar('=')) && (Cur != RChar('>')))
		{
			attrn += Cur;
			Next();
		}

		// Determine if a value is assign
		SkipSpaces();
		if (Cur == RChar('='))
		{
			// A value is assigned
			Next();  // Skip =
			SkipSpaces();

			// Determine if the parameter is delimited by quotes
			What = Cur;
			Quotes = ((What == RChar('\'')) || (What == RChar('"')));

			// Read the parameter
			attrv.Clear();
			if(Quotes)
			{
				Next(); // Skip the quote
				bytepos = TotalLen - Len;
				// Read until the next quote is found
				while ((!Cur.IsNull()) && (Cur!=What))
				{
					attrv += Cur;
					Next();
				}
				Next();
			}
			else
			{
				// If Quote must be used -> generate an exeception
				if(OnlyQuote())
{
cout << "Quote must be used to delimit the parameter value in a tag." << endl;
//FIXME				throw RIOException(this, "Quote must be used to delimit the parameter value in a tag.");
}

				// Read until a space or the end of the tag
				bytepos = TotalLen - Len;
				while ((!Cur.IsNull()) && (!Cur.IsSpace()) && (!(((Cur == EndTag1) && (GetNextCur() == EndTag2)) || (Cur == EndTag2))))
				{
					attrv += Cur;
					Next();
				}
			}
			attrtmp = new XXMLAttr(attrn, XMLToString(attrv));
			SkipSpaces();
		}
		else
			attrtmp = new XXMLAttr(attrn, "");
		attrtmp->SetByte(bytepos, attrv.GetLen());
		attrs.InsertPtr(attrtmp);
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XXMLFile::begin_tag(const RString &name, RContainer<RXMLAttr, false, true> &attrs)
{
	XXMLTag *tag;
	RCursor<RXMLAttr> Cur;

	// Create the tag
	tag = new XXMLTag(name);
	Cur.Set(attrs);
	for (Cur.Start(); !Cur.End(); Cur.Next())
		tag->InsertAttr(Cur());//new XXMLAttr(Cur()->GetName(), Cur()->GetValue()));
	// If no top tag -> insert it
	if (!XMLStruct->GetTop())
	{
		// Test if is a XML document (DocType=true) or a HTML document (DocType=false)
		if (!DocType.IsEmpty())
		{
			// Name of the Tag must be name of the DocType
			if (tag->GetName() != DocType)
{
cout << "Not a valid XML file" << endl;
//FIXME				throw RIOException(this, "Not a valid XML file");
}
		}
		else
		{
			RString TopName = tag->GetName().ToLower();

			// Is it a HTML file?
			// -> If Not, consider the first tag is the DOCTYPE
			if (TopName != "html")
				DocType = TopName;
				//throw RIOException(this,"Not a valid HTML file");
		}
	}

	// Add the tag the XML structure and make it the current tag
	XCurTag = dynamic_cast<XXMLTag *> (CurTag);
	XMLStruct->AddTag(CurTag, tag);
	CurTag = tag;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XXMLFile::end_tag(const RString& name)
{
	if (CurTag->GetName() != name)
{
cout<<"Found closing tag ??? while closing tag ??? was expected." << endl;
//FIXME		throw RIOException(this, "Found closing tag '" + name + "' while closing tag '" + CurTag->GetName() + "' was expected.");
}
	CurTag = CurTag->GetParent();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XXMLFile::add_next_char(RString &str)
{
	// If it is an eol character, skip it with the SkipEol
	if (RTextFile::Eol(Cur))
	{
		str += '\n';
		SkipEol();
	}
	else
	{
		// Normal character
		str += Cur;
		Next();
	}
}

