/* Copyright (C) 1995, Digital Equipment Corporation.         */
/* All rights reserved.                                       */
/* See the file pstotext.txt for a full description.          */
/* Last modified on Thu Aug  1 11:32:09 PDT 1996 by mcjones   */
/* Modified by Pascal Francq auguest 2003 for integration in GALILEI */


#include <stdio.h>
#include "bundle.h"

void putbundle(BUNDLE b, FILE *f)
{
  const char **ppLine = b;
  for (ppLine = b; *ppLine!=NULL; ppLine++) {
    fputs(*ppLine, f);
  }
}
