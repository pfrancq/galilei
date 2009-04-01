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

template<class elem>
XFifoNode<elem>::XFifoNode(elem *_ptr)
{
	ptr = _ptr;
	next = 0;
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

template<class elem>
XFifo<elem>::XFifo()
{
	size = 0;
	first = last = 0;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
template<class elem>
inline void XFifo<elem>::Push(elem *ptr)
{
	if (!size)
		last = (first = new XFifoNode<elem>(ptr));
	else
		last = (last->next = new XFifoNode<elem>(ptr));
	size++;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
template<class elem>
inline elem *XFifo<elem>::PopFirst()
{
	elem *temp;
	XFifoNode<elem> *del;

	if (!first)
		return 0;
	temp = first->ptr;
	del = first;
	first = first->next;
	delete del;
	size--;
	return temp;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
template<class elem>
inline int XFifo<elem>::GetNb()
{
	return size;
}
