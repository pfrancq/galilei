




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
#include "gdocs.h"
#include <gsessions/gsession.h>

using namespace GALILEI



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
        (*doc)->Analyse(ptr);
        (*doc)->Save();
      }
      if(infofunc) infofunc(cInfoNext);
    }
  }
}


//---------------------------------------------------------------------------
void GDocs::Download(void)
{
  unsigned i;
  GDoc **doc;

  for(i=NbPtr+1,doc=Tab;--i;doc++)
    if((*doc)->Calc)
			(*doc)->Download();
}
