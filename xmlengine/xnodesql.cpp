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

#include "xnodesql.h"

XNodeSQL::XNodeSQL(int _idfile, int _idnode, int _child1, int _nbchild, int _parent, int _pos, int _len, RString _name)
{
	idfile = _idfile;
	idnode = _idnode;
	child1 = _child1;
	nbchild = _nbchild;
	parent = _parent;
	pos = _pos;
	len = _len;
	name = _name;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XNodeSQL::XNodeSQL(const XNodeSQL &xnodesql)
{
	idfile = xnodesql.idfile;
	idnode = xnodesql.idnode;
	child1 = xnodesql.child1;
	nbchild = xnodesql.nbchild;
	parent = xnodesql.parent;
	pos = xnodesql.pos;
	len = xnodesql.len;
	name = xnodesql.name;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSQL::SetIdFile(int _idfile)
{
	idfile = _idfile;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSQL::SetIdNode(int _idnode)
{
	idnode = _idnode;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSQL::SetChild1(int _child1)
{
	child1 = _child1;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSQL::SetNbChild(int _nbchild)
{
	nbchild = _nbchild;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSQL::SetParent(int _parent)
{
	parent = _parent;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSQL::SetPos(int _pos)
{
	pos = _pos;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSQL::SetLen(int _len)
{
	len = _len;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSQL::SetName(RString _name)
{
	name = _name;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSQL::GetIdFile() const
{
	return idfile;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSQL::GetIdNode() const
{
	return idnode;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSQL::GetChild1() const
{
	return child1;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSQL::GetNbChild() const
{
	return nbchild;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSQL::GetParent() const
{
	return parent;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSQL::GetPos() const
{
	return pos;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSQL::GetLen() const
{
	return len;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XNodeSQL::GetName() const
{
	return name;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSQL::Compare(const XNodeSQL &node) const
{
	return idnode - node.idnode;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XNodeSQL::print() const
{
	RString r;
	char s[100];
	
	sprintf(s, "%3d : %-20s %3d->%3d from %3d @%3d[%3d]", idnode, name.Latin1(), nbchild ? child1 : -1, nbchild ? child1 + nbchild - 1 : -1, parent, pos, len);
	r = s;
	return r;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
