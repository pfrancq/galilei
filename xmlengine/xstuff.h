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

#ifndef __XSTUFFH__				// Avoids multiple declarations
#define __XSTUFFH__

class Int
{
private :
	
	int i;
	
public :
	
	Int(const int &_i = 0);
	int Get() const;
	void Set(const int &_i);
//	int Compare(const int &_i) const;
	int Compare(const Int &_i) const;
	int Compare(const Int *_i) const;
	~Int();
};

class IInt
{
private :
	
	int i, j;
	
public :
	
	IInt(const int &_i = 0, const int &_j = 0);
	int Geti() const;
	int Getj() const;
	void Set(const int &_i, const int &_j);
	int Compare(const int &_i, const int &_j) const;
	int Compare(const IInt *_ii) const;
	~IInt();
};

#endif
