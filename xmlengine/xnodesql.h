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

#ifndef __XNODESQLH__			// Avoids multiple declarations
#define __XNODESQLH__

#include <rstring.h>
using namespace R;

#include "xgeneral.h"

class XNodeSQL
{
private :
	
	int idfile, idnode, child1, nbchild, parent, pos, len;
	RString name;
	
public :
	
	XNodeSQL(int _idfile = -1, int _idnode = -1, int _child1 = -1, int _nbchild = -1, int _parent = -1, int _pos = -1, int _len = -1, RString _name = "");
	XNodeSQL(const XNodeSQL &xnodesql);
	void SetIdFile(int _idfile);
	void SetIdNode(int _idnode);
	void SetChild1(int _child1);
	void SetNbChild(int _nbchild);
	void SetParent(int _parent);
	void SetPos(int _pos);
	void SetLen(int _len);
	void SetName(RString _name);
	int GetIdFile() const;
	int GetIdNode() const;
	int GetChild1() const;
	int GetNbChild() const;
	int GetParent() const;
	int GetPos() const;
	int GetLen() const;
	RString GetName() const;
	int Compare(const XNodeSQL &node) const;
	RString print() const;	
};

#endif
