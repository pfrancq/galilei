/* Copyright (C) 1995, Digital Equipment Corporation.         */
/* All rights reserved.                                       */
/* See the file pstotext.txt for a full description.          */
/* Last modified on Fri Oct 11 15:35:24 PDT 1996 by mcjones   */
/* Modified by Pascal Francq auguest 2003 for integration in GALILEI */

typedef char *BUNDLE[];

extern "C" void putbundle(BUNDLE b, FILE *f);
