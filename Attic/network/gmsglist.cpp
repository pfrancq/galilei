/*

	GALILEI Research Project

	GMsgList.cpp

	Manager of Information Classes - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <network/gmsglist.h>
using namespace ost;
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GMsgList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GMsgList::GMsgList(unsigned int size) throw(bad_alloc)
	: Buffer(size), Msgs(0), Size(size)
{
	Msgs=new GMsg[Size];
	head = tail = Msgs;
}


//-----------------------------------------------------------------------------
int GMsgList::onPeek(void *data)
{
	static_cast<GMsg*>(data)->Copy(head);
	return(1);
}


//-----------------------------------------------------------------------------
int GMsgList::onWait(void *data)
{
	static_cast<GMsg*>(data)->Copy(head);
	if((head += 1) >= (Msgs+Size))
		head = Msgs;
	return(1);
}


//-----------------------------------------------------------------------------
int GMsgList::onPost(void *data)
{
	tail->Copy(static_cast<GMsg*>(data));
	if((tail += 1) >= (Msgs+Size))
		tail = Msgs;
	return(1);
}


//-----------------------------------------------------------------------------
bool GMsgList::isValid(void)
{
	if(head!=tail)
		return(true);
	return(false);
}


//-----------------------------------------------------------------------------
GMsgList::~GMsgList(void)
{
	if(Msgs) delete[] Msgs;
}
