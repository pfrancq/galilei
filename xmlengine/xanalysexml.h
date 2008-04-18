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

#ifndef __XANALYSEXMLH__		// Avoids multiple declarations
#define __XANALYSEXMLH__

#include <iostream>
#include <fcntl.h>
#include <dirent.h>
using namespace std;

#include <rstring.h>
#include <rcontainer.h>
#include <rcursor.h>
#include <rxmlstruct.h>
#include <rio.h>
#include <rmysql.h>
using namespace R;

#include "xgeneral.h"
//#include "xpathxml.h"
#include "xworditem.h"
#include "xnodesql.h"
#include "xtag.h"
#include "xfifo.h"
#include "xxmlfile.h"
#include "xsqlfunc.h"
//#include "xgeneral.h"

class XAnalyseXML
{
private :

	RContainer<XWordItem, true, true> wordslist;
	RContainer<XNodeSQL, true, false> nodeslist;
	RXMLStruct xstruct;
	XXMLFile xfile;
	int curr_idpath;
	int curr_idfile;
	unsigned long utc;
	unsigned int size;

	void insertWord(const RString &w);
	void sparseWords(const RString &str);
	void getWords(XXMLTag *tag);

public :

	XAnalyseXML(const RString &filename);
	void GetFileProperty(const RString &filename);
	void AnalyseDoc(const RString &filename);
	bool file_sync(const RString &filename);
//	void SearchDepth();
	void search_breadth();
	void CreateSQL();
	void DropSQL();
	void Update(const RString &path, const bool include_subdir = true);
	~XAnalyseXML();
	void ShowSolution();
};

#endif
