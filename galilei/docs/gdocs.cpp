/*

	GALILEI Research Project

	gdocs.cpp

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




//---------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <string.h>
#include <fstream.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


//---------------------------------------------------------------------------
// include files for Galilei
#include <gdocs/gdocs.h>
#include <gsessions/gsession.h>
using namespace GALILEI;



//---------------------------------------------------------------------------
//
// class GDocs
//
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------

GDocs::GDocs(unsigned int nb,bool alldocs,GSession *session) throw(bad_alloc)
  : RContainer<GDoc,unsigned,true,false>(nb+(nb/2),nb/2),AllDocs(alldocs),Session(session)

{
}


//---------------------------------------------------------------------------
void GDocs::Analyse(URLFunc *urlfunc,InfoFunc *infofunc) throw(GException)
{
  char *ptr;
  unsigned i;
  GDoc **doc;

  if(!urlfunc)
    throw(GException("Error in ""GDocs::Analyse"": No callback function for loading URL"));
  for(i=NbPtr+1,doc=Tab;--i;doc++)
  {
    if((*doc)->Calc)
    {
      ptr=urlfunc((*doc)->URL());
      if(ptr)
      {
//        (*doc)->Analyse(ptr);
    //    (*doc)->Save();
      }
      if(infofunc) infofunc(cInfoNext);
    }
  }
}
