/*

	GALILEI Research Project

	GDoc.cpp

	Document - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>
#include <docs/gdocxml.h>
#include <infos/giwordoccur.h>
#include <infos/giwordoccurs.h>
#include <langs/gword.h>
#include <langs/gdict.h>
#include <langs/glang.h>
#include <filters/gmimefilter.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace RXML;
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class GDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDoc::GDoc(const char* url,const char* name,unsigned int id,GLang* lang,GMIMEFilter* t,const char* u,const char* a,unsigned int f,unsigned int n,unsigned int ndiff,unsigned int v,unsigned int vdiff,unsigned int nbf) throw(bad_alloc)
	: URL(url), Name(name), Id(id), Words(0), N(n), V(v), Ndiff(ndiff), Vdiff(vdiff),
	  Lang(lang), Type(t), Updated(u), Computed(a), Fdbks(nbf+nbf/2,nbf/2), Failed(f)
{
	if(Updated>Computed)
	{
		if(Computed==RDate::null)
			State=osCreated;
		else
			State=osModified;
	}
	else
		State=osUpToDate;
	Words=new GIWordOccurs(vdiff>600?vdiff:600);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const GDoc& doc) const
{
	return(Id-doc.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const GDoc* doc) const
{
	return(Id-doc->Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GDoc::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::AddWord(const unsigned int id,const unsigned int nb)
{
	Words->InsertPtr(new GIWordOccur(id,nb));
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::WordsStart(void)
{
	Words->Start();
}


//-----------------------------------------------------------------------------
bool GALILEI::GDoc::WordsEnd(void) const
{
	return(Words->End());
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::WordsNext(void)
{
	Words->Next();
}


//-----------------------------------------------------------------------------
GIWordOccur* GALILEI::GDoc::GetCurWords(void)
{
	return((*Words)());
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::ProfilesStart(void)
{
	Fdbks.Start();
}


//-----------------------------------------------------------------------------
bool GALILEI::GDoc::ProfilesEnd(void) const
{
	return(Fdbks.End());
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::ProfilesNext(void)
{
	Fdbks.Next();
}


//-----------------------------------------------------------------------------
GProfDoc* GALILEI::GDoc::GetCurProfiles(void)
{
	return(Fdbks());
}


//-----------------------------------------------------------------------------
void GALILEI::GDoc::AddJudgement(GProfDoc* j) throw(bad_alloc)
{
	Fdbks.InsertPtr(j);
}


//-----------------------------------------------------------------------------
GALILEI::GDoc::~GDoc(void)
{
	if(Words) delete Words;
}
