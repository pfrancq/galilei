/*
 * gmem.h
 *
 * Memory routines with out-of-memory checking.
 *
 * Copyright 1996-2003 Glyph & Cog, LLC
 */

#ifndef GMEM_H
#define GMEM_H

#include <stdio.h>
#include <stdlib.h>

/*
#define gmalloc(size) malloc(size)
#define gfree(ptr) ::free(ptr)
#define grealloc(ptr,size) realloc(ptr,size)
#define gMemReport(f)
#define copyString(str) strdup(str)*/

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Same as malloc, but prints error message and exits if malloc()
 * returns NULL.
 */
extern void *gmalloc(int size);

/*
 * Same as realloc, but prints error message and exits if realloc()
 * returns NULL.  If <p> is NULL, calls malloc instead of realloc().
 */
extern void *grealloc(void *p, int size);

/*
 * Same as free, but checks for and ignores NULL pointers.
 */
extern void gfree(void *p);

#ifdef DEBUG_MEM
/*
 * Report on unfreed memory.
 */
extern void gMemReport(FILE *f);
#else
#define gMemReport(f)
#endif

/*
 * Allocate memory and copy a string into it.
 */
extern char *copyString(const char *s);

#ifdef __cplusplus
}
#endif

#endif
