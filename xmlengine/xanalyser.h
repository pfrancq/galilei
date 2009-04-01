/***************************************************************************
 *   Copyright 2004-2009 by the Universit�Libre de Bruxelles.

	Authors:
		Faïza Abbaci (fabbaci@ulb.ac.be)                                                  *
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

#ifndef __XANALYSERH__			// Avoids multiple declarations
#define __XANALYSERH__

#include <fcntl.h>
#include <dirent.h>
using namespace std;

#include <rstring.h>
using namespace R;

#include "xanalysexmlfile.h"
#include "xgeneral.h"

class XAnalyser
{
private :

	GALILEI::GStorageTag *cmdtag;
	RContainer<RString, true, true> insert;
	RContainer<RString, true, true> exist;
	GALILEI::GStorage *storage;
	void (*output)(const char *s);

	void print(const RString &s) const;

public :

	XAnalyser(GALILEI::GStorage *_storage, void (*_output)(const char *s) = 0);
	void AddPath(RString path, const bool &include_subfolders);
	void Update();
};
#endif
