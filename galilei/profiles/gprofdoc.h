/*

	GALILEI Research Project

	gprofdoc.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GProfDocH
#define GProfDocH


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <gdocs/gdoc.h>


//-----------------------------------------------------------------------------
namespace GALILEI {
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GProfDoc
{
public:
	GDoc *Doc;
    char Fdbk;

	/** constructor of GProfDoc
	*/
    GProfDoc(GDoc *doc,char fdbk);

	/** comparaison functions
	*/
	int Compare(const GProfDoc &profdoc);
	int Compare(const GProfDoc *profdoc);
};


}  //-------- End of namespace GALILEI ----------------------------------------

//-----------------------------------------------------------------------------
#endif
