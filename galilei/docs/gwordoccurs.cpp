




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
// include files for HyperPRISME
#include "gwordoccurs.h"


using namespace GALILEI





//---------------------------------------------------------------------------
//
// GWordOccurs
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GWordOccurs::GWordOccurs(GDoc *owner,unsigned nb) throw(bad_alloc)
	: RContainer<GWordOccur,unsigned,true,true>(nb+nb/2,nb/2), Owner(owner)
{
}


//---------------------------------------------------------------------------
void GWordOccurs::Insert(unsigned id,GDoc *doc) throw(bad_alloc)
{
  GWordOccur *ptr;

  ptr=new GWordOccur(id);
  ptr->Doc=doc;
  InsertPtr(ptr);
}


//---------------------------------------------------------------------------
GWordOccur* GWordOccurs::GetPtr(unsigned id,GDoc *doc) throw(bad_alloc)
{
  GWordOccur *ptr;

  ptr=GetInsertPtr<unsigned>(id);
  if(!ptr->Doc) ptr->Doc=doc;
  return(ptr);
}



