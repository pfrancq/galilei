/*

	GALILEI Research Project

	PS.h

	A PostScript filter - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Universit√© Libre de Bruxelles (ULB).
	Copyright 1995-1998 by Digital Equipment Corporation.
	See the file pstotext.txt for a full description.

	Modified by:
		Last modified on Fri Oct 16 16:27:54 PDT 1998 by mcjones
		Modified on Thu Nov 16 13:33:13 PST 1995 by deutsch
		Modified on 27-MAY-1998 13:08 by Hunter Goatley
 			Ported to VMS.  Various minor changes to allow it to work on
 			both VAX and Alpha (VAX C and DEC C).  VMS pipes don't work
			right, so the GS output is dumped to a temporary file that's
 			read, instead of reading from pipes (which is, of course, how
 			VMS implements pipes anyway).  Also added -output option.
		Pascal Francq (pascal@francq.info)
			Create Filter for GALILEI

*/



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#ifdef VMS
	#include "vms.h"
#else
	#include <sys/param.h>
	#include <sys/wait.h>
#endif
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ps.h>
#include <gdoc.h>
#include <rdownload.h>
#include <gdocanalyze.h>


//------------------------------------------------------------------------------
// include files for pstotext
#define NEED_PROTO 1
#include <bundle.h>
#include <ocr.h>
#include <rot270.h>
#include <rot90.h>
#include <ptotdll.h>


//------------------------------------------------------------------------------
// extern structure
extern BUNDLE ocr, rot270, rot90;


//------------------------------------------------------------------------------
// declare global variables;
const unsigned int CharInc=200000;
const unsigned int LineLen=2000;
static bool cork = false;
static bool debugfilter = false;
static const char *gs_cmd = "gs";
static char *cmd; /* = argv[0] */
static enum
{
  portrait,
  landscape,
  landscapeOther
} orientation = portrait;

static bool bboxes = false;

#ifdef VMS
	#define OCRPATH "pstotext_dir:pstotext-ocr.ps"
	#define ROT270PATH "pstotext_dir:pstotext-rot270.ps"
	#define ROT90PATH "pstotext_dir:pstotext-rot90.ps"
#else
	#define OCRPATH "/tmp/,pstotext-ocr.ps"
	#define ROT270PATH "/tmp/,pstotext-rot270.ps"
	#define ROT90PATH "/tmp/,pstotext-rot90.ps"
#endif

static char *make_temp(BUNDLE b)
{
  /* Return pathname of temporary file containing bundle "b".  Caller
     should unlink file (and, technically, free pathname). */
  FILE *f;

#if defined(_BSD_SOURCE)   || defined(__APPLE__)
	static char path[250]="";
	FILE *sfp;
	int fd(-1);

	#ifdef VMS
		strncpy(path, "SYS$SCRATCH:.ps2tXXXXXX", sizeof(path));
	#else
		strncpy(path, "/tmp/,ps2tXXXXXX", sizeof(path));
	#endif
	if ((fd = mkstemp(path)) == -1 || (sfp = fdopen(fd, "w+")) == NULL)
	{
		if (fd != -1)
		{
			unlink(path);
			close(fd);
		}
	   mThrowGException(strerror(errno));
	}
	close(fd);
#else
	#ifdef VMS
		char *path = tempnam("SYS$SCRATCH:", ".ps2t");
	#else
		char *path = tempnam("/tmp", ",ps2t");
	#endif
#endif

  f = fopen(path, "w");
  if (f==NULL) {perror(cmd); exit(1);}
  putbundle(b, f);
  fclose(f);
  return path;
}

static const char *ocr_path = NULL, *rotate_path = NULL;
static FILE *gs = NULL;
static void *instance; /* pstotext state */
#ifdef VMS
static char *cmdfile = NULL, *gsoutfile = NULL;
#endif

static int cleanup() {
  int gsstatus, status = 0;
  pstotextExit(instance);
  if (gs!=NULL) {
#ifdef VMS
    gsstatus = fclose(gs);
#else
    gsstatus = pclose(gs);
#endif
    if (WIFEXITED(gsstatus)) {
      if (WEXITSTATUS(gsstatus)!=0) status = 3;
      else if (WIFSIGNALED(gsstatus)) status = 4;
    }
  }
  if ((rotate_path!=NULL) & (strcmp(rotate_path, "")!=0)) unlink(rotate_path);
  if (ocr_path!=NULL) unlink(ocr_path);
#ifdef VMS
  if (cmdfile!=NULL) unlink(cmdfile);
  if (gsoutfile!=NULL) unlink(gsoutfile);
#endif
  return status;
}



//------------------------------------------------------------------------------
//
// class GFilterPS
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterPS::GFilterPS(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac)
{
	AddMIME("application/x-postscript");
	AddMIME("application/postscript");
}


//------------------------------------------------------------------------------
void GFilterPS::StrToBuffer(const char* str)
{
	for(;(*str);)
	{
		if(CharLen==CharMax)
		{
			RChar* tmp;

			CharMax+=CharInc;
			tmp=new RChar[CharMax];
			memcpy(tmp,CharBuffer,CharLen*sizeof(RChar));
			delete[] CharBuffer;
			CharBuffer=tmp;
		}
		CharBuffer[CharLen++]=(*(str++));
	}
}


//------------------------------------------------------------------------------
void GFilterPS::Analyze(GDocAnalyze* analyzer,const GDoc*,const R::RURI& file)
{
	bool Paragraph;
	char gs_cmdline[2*MAXPATHLEN];
	int status;
	#ifdef VMS
		FILE *cfile;
	#endif

	ocr_path = make_temp(ocr);

	switch (orientation)
	{
		case portrait: rotate_path = ""; break;
		case landscape: rotate_path = make_temp(rot270); break;
		case landscapeOther: rotate_path = make_temp(rot90); break;
	}

	sprintf(gs_cmdline,
			#ifdef VMS
				"%s -r72 \"-dNODISPLAY\" \"-dFIXEDMEDIA\" \"-dDELAYBIND\" \"-dWRITESYSTEMDICT\" %s \"-dNOPAUSE\" %s %s %s",
			#else
				"%s -r72 -dNODISPLAY -dFIXEDMEDIA -dDELAYBIND -dWRITESYSTEMDICT %s -dNOPAUSE %s %s %s",
			#endif
			gs_cmd,
			(debugfilter ? "" : "-q"),
			rotate_path,
			ocr_path,
			file().ToLatin1()
			);
	if (debugfilter)
		cerr<<gs_cmdline<<endl;

	#ifdef VMS
		cmdfile = tempnam("SYS$SCRATCH:","PS2TGS");
		gsoutfile = tempnam("SYS$SCRATCH:","GSRES");
		if((cfile = fopen(cmdfile,"w")) == NULL) {perror(cmd);exit(1);}
		fprintf (cfile, "$ define/user sys$output %s\n", gsoutfile);
		fprintf (cfile, "$ %s\n", gs_cmdline);
		fprintf (cfile, "$ deletee/nolog %s;*\n", cmdfile);
		fputs ("$ exit\n", cfile);
		fclose (cfile);
		sprintf(gs_cmdline, "@%s.", cmdfile);
		system(gs_cmdline);
		if ((gs = fopen(gsoutfile, "r")) == NULL)
		{
			cerr<<"Error opening output file"<<gsoutfile<<" from GS command"<<endl;
			perror(cmd);
			return(false);
  		}
	#else
		gs = popen(gs_cmdline, "r");
		if (gs==0)
		{
			perror(cmd);
			mThrowGException("Not valid PS file");
		}
	#endif

	status = pstotextInit(&instance);
	if (status)
	{
		cerr<<cmd<<": internal error "<<status<<endl;
		mThrowGException("Not valid PS file");
	}
	if (cork)
	{
		status = pstotextSetCork(instance, true);
		if (status)
		{
			cerr<<cmd<<": internal error "<<status<<endl;
			mThrowGException("Not valid PS file");
		}
	}

	// Must create the buffer
	CharMax=CharInc;
	CharLen=0;
	CharBuffer=new RChar[CharMax];

	// Fill the structure
	while(true)
	{
		char line[LineLen];
		char *pre, *word, *post;
		int llx, lly, urx, ury;
		if (fgets(line, LineLen, gs)==NULL) break;
		if (debugfilter) fputs(line, stderr);
		status = pstotextFilter(instance, line, &pre, &word, &post, &llx, &lly, &urx, &ury);
		if(status)
		{
			cerr<<cmd<<": internal error "<<status<<endl;
			delete[] CharBuffer;
			mThrowGException("Not valid PS file");
		}
		if(word)
		{
			if (!bboxes)
				StrToBuffer(pre);

			StrToBuffer(word);

			if (!bboxes)
			{
				StrToBuffer(post);
				if(debugfilter) cerr<<endl;
			}
		}
	}
	status = cleanup();
	if(status)
	{
		cerr<<cmd<<": internal error "<<status<<endl;
		delete[] CharBuffer;
		mThrowGException("Not valid PS file");
	}

	// Look for the content
	Begin=Pos=CharBuffer; // Remember the first line which is not a command
	SkipSpaces();
	Pos=Begin;
	while(!Pos->IsNull())
	{
		SkipSpaces();
		Begin=Pos;
		// Paragraph are supposed to be terminated by at least one blank line
		Paragraph=true;
		while((!Pos->IsNull())&&Paragraph)
		{
			// Read a Line
			while((!Pos->IsNull())&&(Pos->Latin1()!='\n')&&(Pos->Latin1()!='\r'))
				Pos++;
			Pos++;      // Skip the '\n'.

			// Skip Spaces and Tab
			while((!Pos->IsNull())&&((Pos->Latin1()==' ')||(Pos->Latin1()=='\t')))
				Pos++;

			// Look if the next lines is a blank one.
			if(Pos->Latin1()=='\n')
			{
				(*(Pos++))=0;      // Mark the end of the paragraph.
				Paragraph=false;
			}
		}
		analyzer->ExtractDefaultText(Begin,0);
	}

	// Clean up
	if (instance)
	{
		free(instance);
	}
	delete[] CharBuffer;
}


//------------------------------------------------------------------------------
R::RString GFilterPS::GetTextFragment(GDoc*,size_t,size_t)
{
	return("Cannot extract text fragment");
}


//------------------------------------------------------------------------------
GFilterPS::~GFilterPS()
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("PostScript Filter","PostScript Filter",GFilterPS)
