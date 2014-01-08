/*

	GALILEI Research Project

	ChoquetSim.h

	Choquet Similarity - Header.

	Copyright 2013-2014 by Pascal Francq (pascal@francq.info).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef ChoquetSimH
#define ChoquetSimH


//-----------------------------------------------------------------------------
// General variables
const size_t NbCriteria(4);
const size_t cText(0);
const size_t cMetadata(1);
const size_t cSemantic(2);
const size_t cLink(3);


//------------------------------------------------------------------------------
// Information receive by the similarity plug-in
struct sInfo
{
	double Text;
	double Metadata;
	double Semantic;
	double Link;
};


//-----------------------------------------------------------------------------
#endif
