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

#include "xrank.h"
int XRank::totrefs = 0;

XRank::XRank(const tElem _type, const int _iddeepest)//, const int _depth, const bool _relevant)
{
thisref = totrefs++;
	type = _type;
	depth = 0;
	iddeepest = _iddeepest;
	relevant = true;
__NEW__("XRANK", thisref);
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XRank::XRank(const tElem _type, const int _depth, const int _iddeepest, const bool _relevant)
{
	thisref = totrefs++;
	type = _type;
	depth = _depth;
	iddeepest = _iddeepest;
	relevant = _relevant;
__NEW__("XRANK", thisref);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XRank::XRank(const XRank &xrk)
{
thisref = totrefs++;
	type = xrk.type;
	depth = xrk.depth;
	relevant = xrk.relevant;
	iddeepest = xrk.iddeepest;
__NEW__("XRANK", thisref);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XRank::tElem XRank::GetType() const
{
	return type;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XRank::GetDepth() const
{
	return depth;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XRank::GetDeepestId() const
{
	return iddeepest;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
// bool XRank::IsMe(const int _type, const int _depth, const int _idnoderelevant, const bool _relevant)
// {
// 	if (type == _type)
// 		if (depth == _depth)
// 			if (idnoderelevant == _idnoderelevant)
// 				if (relevant == _relevant)
// 					return true;
// 	return false;
// }

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XRank::IsRelevant() const
{
	return relevant;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XRank::IsLast() const
{
	return last;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XRank::SetLast(bool _last)
{
	last = _last;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XRank::SetDeepestId(int idnode)
{
	iddeepest = idnode;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XRank::AutoInc()
{
	depth++;
	relevant = false;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XRank::Compare(const XRank *xrk) const
{
	return 0;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XRank::operator == (const XRank &xrk) const
{
	if (type == xrk.type)
		if (depth == xrk.depth)
			if (iddeepest == xrk.iddeepest)
				if (relevant == xrk.relevant)
					return true;
	return false;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XRank::~XRank()
{
	__DEL__("XRANK", thisref);
}
