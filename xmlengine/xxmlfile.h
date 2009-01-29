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

#include <rxmlfile.h>
#include <rtextfile.h>
using namespace R;

#include "xgeneral.h"
#include "xtag.h"

class XXMLFile : public RXMLFile
{
	size_t Len,TotalLen;

private :

	XXMLTag *XCurTag;

	void load_header();
	void  load_next_tag();
	void  load_attributes(RContainer<RXMLAttr, false, true> &attrs, RChar EndTag1 = '/', RChar EndTag2 = '>');
	void begin_tag(const RString &name, RContainer<RXMLAttr, false, true> &attrs);
	void end_tag(const RString& name);
	void add_next_char(RString &str);

public :

	XXMLFile(const RString &name, RXMLStruct *xmlstruct, const RString &encoding = "UTF-8");
	XXMLFile(RIOFile &file, RXMLStruct *xmlstruct, const RString &encoding = "UTF-8");
	void Open(RIO::ModeType mode);
};

#endif
