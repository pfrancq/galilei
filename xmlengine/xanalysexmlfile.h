/***************************************************************************
 *   Copyright 2004-2009 by the Universit�Libre de Bruxelles.

	Authors:
		Faïza Abbaci (fabbaci@ulb.ac.be)
		                                             *
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

#ifndef __XANALYSEXMLFILEH__	// Avoids multiple declarations
#define __XANALYSEXMLFILEH__

#include <rstring.h>
#include <rcontainer.h>
#include <rcursor.h>
#include <rxmlstruct.h>
#include <rio.h>
#include <rdbmysql.h>
#include <rdblhashcontainer.h>
using namespace R;

#include <gstorage.h>

#include "xgeneral.h"
#include "xwordsql.h"
#include "xnodesql.h"
#include "xtag.h"
#include "xfifo.h"
#include "xxmlfile.h"
#include <ggalileiapp.h>
#include <glang.h>
//#include "xgeneral.h"
using namespace GALILEI;
class XAnalyseXMLFile
{
private :

	RContainer<XWordSQL, true, true> wordslist;
	RContainer<XNodeSQL, true, false> nodeslist;
	RXMLStruct xstruct;
	RString filename;
	GALILEI::GStorage *storage;
	GALILEI::GStorageTag *cmdtag;
	XXMLFile *xfile;

	GLang* Lang;

	int curr_idnode;
	int idfile;
	unsigned long utc;
	unsigned int size;

	void get_property();
	void insertWord(const RString &w);
	void parseWords(const RString &str);
	void getWords(XXMLTag *tag);
	void search_breadth();


public :

	XAnalyseXMLFile(GALILEI::GStorage *_storage, const RString &_filename);
	void AnalyseDoc();
	bool IsFileSync();
	void DropSQL();
	~XAnalyseXMLFile();
};

#endif
