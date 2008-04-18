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

#include "xwordsql.h"

XWordSQL::XWordSQL(RString w, int id)
{
	word = w;
	id_nodes.push_back(id);
	//FIXME cout <<"faiza id pushed " << id <<endl;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XWordSQL::GetWord() const
{
	return word;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XWordSQL::GetNodes()
{
	RString s;
	int k, size;
	 
	size = id_nodes.size();
	k = 0;
	if (k < size)
	{
		s += RString::Number(id_nodes[k++]);
		while (k < size)
			s += ";" + RString::Number(id_nodes[k++]);
	}
	return s;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XWordSQL::Compare(const RString &w) const
{
	return (word.Compare(w));
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XWordSQL::Compare(const XWordSQL &w) const
{
	return (Compare(w.word));
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XWordSQL::AddNode(const int id)
{
	vector<int>::iterator iter;
	if (*(id_nodes.end()-1) < id)													// The id are sorted ! so if the last id is smaller than the
		id_nodes.push_back(id);													//  new one, then the new one does not belong to the vector
/*	iter = find(id_nodes.begin(), id_nodes.end(), id);
	if (iter == id_nodes.end())
		id_nodes.push_back(id);*/
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XWordSQL::print()
{
	RString s;
	int fill;
	 
	fill = 20 - word.GetLen();
	s = word;
	while (fill-- > 0)
		s += " ";
	s += ": " + GetNodes();
	return s;
}
