




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
#include "gwordoccur.h"


using namespace Galilei


//---------------------------------------------------------------------------
//
// class GWordOccur
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GWordOccur::GWordOccur(void)
  : GWordRef(),Doc(0),Occur(0)
{
}


//---------------------------------------------------------------------------
GWordOccur::GWordOccur(unsigned id)
  : GWordRef(id),Doc(0),Occur(0)
{
}


//---------------------------------------------------------------------------
int GWordOccur::Compare(const GWordOccur& ref)
{
  return(Id-ref.Id);
}


//---------------------------------------------------------------------------
int GWordOccur::Compare(GWordOccur *ref)
{
  return(Id-ref->Id);
}


//---------------------------------------------------------------------------
int GWordOccur::Compare(unsigned id)
{
  return(Id-id);
}



