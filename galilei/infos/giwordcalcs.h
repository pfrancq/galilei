/*

	GALILEI Research Project

	gwordcalcs.h

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


#ifndef GIWordCalcsH
#define GIWordCalcsH
//---------------------------------------------------------------------------

//include files for Rainbow

#include <rstd/rcontainer.h>


//---------------------------------------------------------------------------

//include files for GALILEI

#include<gdocs/gdoc.h>
#include<glangs/glang.h>
#include <ginfos/giwordcalc.h>

using namespace GALILEI;

namespace GALILEI{

class GDoc;
class GSession;

//---------------------------------------------------------------------------

class GWordCalcs : public RStd::RContainer<GWordCalc,unsigned,true,true>
{
public:
	double NbWordsDocs;
	GWordCalc **Order;
	GWordCalc **CurOrder;
	GWordCalcs(GLang *lang,GSession *session) throw(bad_alloc);
	int Compare(const GWordCalcs &calcs);
	int Compare(const GWordCalcs *calcs);
	void Analyse(GDoc *doc);
	void EndCalc(void);
	unsigned int NextWord(void);
	virtual ~GWordCalcs(void);

	static int sortOrder(const void *a,const void *b);
};
}
#endif
