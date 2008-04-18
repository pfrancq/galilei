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

#ifndef __XWORDITEMH__			// Avoids multiple declaration
#define __XWORDITEMH__

#include <vector>
using namespace std;

#include <rstring.h>
#include <rcontainer.h>
using namespace R;

#include "xgeneral.h"

class XWordSQL
{
private :
	
	vector<int> id_nodes;
	RString word;

public :
	
	XWordSQL(RString w, int id);
	RString GetWord() const;
	RString GetNodes();
	int Compare(const RString &w) const;
	int Compare(const XWordSQL &w) const;
	void AddNode(int id);
	RString print();
};

#endif
