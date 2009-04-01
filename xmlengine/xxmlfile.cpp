/***************************************************************************
 *   Copyright (C) 2005 by Jean-Baptiste Valsamis                          *
 *   jvalsami@ulb.ac.be                                                    *
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


//------------------------------------------------------------------------------

XXMLFile::XXMLFile(const RURI& uri, RXMLStruct* xmlstruct,const RString &encoding)
: RXMLParser(uri,encoding), XMLStruct(xmlstruct), CurTag(0)
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XXMLFile::XXMLFile(RIOFile &file, RXMLStruct *xmlstruct, const RString &encoding)
: RXMLParser(file,encoding), XMLStruct(xmlstruct), CurTag(0)
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
			CurTag=0;
			CurAttr=0;
			XMLStruct->Clear(); // Make sure the xml structure is empty
			RXMLParser::Open(mode);
			break;
		default :
			RXMLParser::Open(mode);
	};
}

//______________________________________________________________________________
//------------------------------------------------------------------------------

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XXMLFile::BeginTag(const RString& namespaceURI, const RString& , const RString& name)
{
		XXMLTag *xtag;


		// Create the tag
		xtag = new XXMLTag(name);
		xtag->SetByte(GetPos(),GetPos()); //the second GetPos will have another value when we get the endtag
		XMLStruct->AddTag(CurTag, xtag);

		// If no top tag -> insert it
		if(!XMLStruct->GetTop())
		{
			// Test if is a XML document (DocType=true) or a HTML document (DocType=false)
			if(!DocType.IsEmpty())
			{
				// Name of the Tag must be name of the DocType
				if(xtag->GetName()!=DocType)
					throw RIOException(this,"Not a valid XML file");
			}
			else
			{
				RString TopName=xtag->GetName().ToLower();

				// Is it a HTML file?
				// -> If Not, consider the first tag is the DOCTYPE
				if(TopName!="html")
					DocType=TopName;
					//throw RIOException(this,"Not a valid HTML file");
			}
		}

		// Make the tag the current one
		CurTag=xtag;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XXMLFile::EndTag(const RString& , const RString& , const RString& )
{
	int pos = dynamic_cast<XXMLTag *> (CurTag)->GetPos();
	int len = GetPos() - pos;

	dynamic_cast<XXMLTag *> (CurTag)->SetByte(pos, len);
	CurTag = CurTag->GetParent();
}

//------------------------------------------------------------------------------

void XXMLFile::Value(const RString& value)
{
	int pos = dynamic_cast<XXMLAttr *> (CurAttr)->GetPos();
	int len = GetPos() - pos;
	dynamic_cast<XXMLAttr *> (CurAttr)->SetByte(pos, len);

	if(CurAttr)
		CurAttr->AddValue(value);
	else
	{
		if(AttrName=="version")
			XMLStruct->SetVersion(value);
		if(AttrName=="encoding")
			SetEncoding(value);
	}
}
//------------------------------------------------------------------------------
void XXMLFile::Text(const RString& text)
{
	CurTag->AddContent(text);
}
//------------------------------------------------------------------------------
void XXMLFile::AddAttribute(const RString& namespaceURI,const RString& lName, const RString&)
{
	XXMLAttr* attrtmp;
	attrtmp = new XXMLAttr(lName, "");
	attrtmp->SetByte(GetPos(),GetPos()); //the second GetPos() will be fixed when the value of the attribute is encountred

	if(CurTag)
		CurTag->InsertAttr(attrtmp);//CurAttr=XMLStruct->NewAttr(lName,namespaceURI));
	else
		AttrName=lName;

	CurAttr = attrtmp;
}

void XXMLFile::AddEntity(const RString& name,const RString& value)
{
	XMLStruct->InsertEntity(name,value);
}
//______________________________________________________________________________
//------------------------------------------------------------------------------



