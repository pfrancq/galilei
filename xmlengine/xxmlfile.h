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

#ifndef __XXMLFILEH__			// Avoids multiple declarations
#define __XXMLFILEH__

#include <fcntl.h>
using namespace std;
#include <rxmlparser.h>
#include <rxmlstruct.h>

using namespace R;

#include "xgeneral.h"
#include "xtag.h"

class XXMLFile : public RXMLParser
{
	size_t Len,TotalLen;

protected:
	/**
	* The structure associated with the XML file.
	*/
	RXMLStruct* XMLStruct;

private :

	//XXMLTag *XCurTag;

	/**
	* Current tag treated.
	*/

	RXMLTag* CurTag;
	/**
	* Current attribute treated.
	*/

	RXMLAttr* CurAttr;

	/**
	* Name of an attribute in the headers.
	*/

	RString AttrName;

	XXMLTag *XCurTag;

	virtual void BeginTag(const RString& namespaceURI, const RString& lName, const RString& name);
	virtual void EndTag(const RString& namespaceURI, const RString& lName, const RString& name);
	virtual void Value(const RString& value);
	virtual void Text(const RString& text);
	virtual void AddAttribute(const RString& namespaceURI, const RString& lName, const RString& name);
	virtual void AddEntity(const RString& name,const RString& value);

public :
	XXMLFile(const RURI& uri, RXMLStruct *xmlstruct, const RString &encoding = "UTF-8");
	XXMLFile(RIOFile &file, RXMLStruct *xmlstruct, const RString &encoding = "UTF-8");
	void Open(RIO::ModeType mode);
};

#endif
