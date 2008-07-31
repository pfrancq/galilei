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

#include "xqueryres.h"

XQueryRes::XQueryRes() : RContainer<XNodeSet, true, true> (10)
{
	__NEW__(XQR, 0);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQueryRes::Oper(XConst::tOper op, const XQueryRes *p, const XQueryRes *q)
{
	XNodeSet *r;																// Items to add
	RCursor<XNodeSet> *cmin, *cmax;												// Temporary
	RCursor<XNodeSet> cp(*p);
	RCursor<XNodeSet> cq(*q);
	
	cp.Start();
	cq.Start();
	while (!cp.End() && !cq.End())												// While there are elements in both sets
	{																			// REM : sets are orderer according to idfile
		if (*cp() == *cq())														// If equality of idfile
		{
			r = new XNodeSet(cp()->GetIdFile());							// Will be desallocated by XQueryRes
			(*cp()).Reduce();
			(*cq()).Reduce();
			r->Oper(op, cp(), cq());											// Performs same operator on XNodeSet
			if (r->GetNb())														// Maybe no result (e.g. when op is NOT)
				InsertPtr(r);													// Adds it
			else
				delete r;
			cp.Next();															// Next for both sets
			cq.Next();
		}
		else																	// If inequality
		{
			*cp() < *cq() ? cmin = &cp, cmax = &cq : cmin = &cq, cmax = &cp;	// Gets idfile min and max between p and q
			switch (op)
			{
			case XConst::NAND :
				if (cmin != &cp)
					break;
			case XConst::OR :
					(*cmin)()->Reduce();
					InsertPtr(new XNodeSet(*(*cmin)()));						// Adds its XNodeSet, will be desallocated by XQueryRes
				break;
			default : break;													// Avoids warning
			}
			cmin->Next();														// And increments
		}
	}
	cp.End() ? cmin = &cq : cmin = &cp;											// If one set is ended, goes on with the other
	switch (op)
	{
	case XConst::NAND :
		if (cmin == &cq)
			break;
	case XConst::OR :
		for (; !cmin->End(); cmin->Next())
		{
				(*cmin)()->Reduce();
				InsertPtr(new XNodeSet(*(*cmin)()));								// Adds its XNodeSet, will be desallocated by XQueryRes
		}
		break;
	default : break;															// Avoids warning
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryRes::~XQueryRes()
{
	__DEL__(XQR, 0);
}
