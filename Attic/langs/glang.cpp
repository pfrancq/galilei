/*

	GALILEI Research Project

	GLang.cpp

	Basic Language - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++1
#include <string.h>
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangs/glang.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GLang
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GLang::GLang(const RString& lang,const char* code) throw(bad_alloc)
	: RLang(lang,code), Activ(true), SkipWords(50,20)
{
	SkipWords.InsertPtr(new SkipWord("min"));
	SkipWords.InsertPtr(new SkipWord("sec"));
	SkipWords.InsertPtr(new SkipWord("msec"));
	SkipWords.InsertPtr(new SkipWord("s"));
	SkipWords.InsertPtr(new SkipWord("nm"));
	SkipWords.InsertPtr(new SkipWord("mm"));
	SkipWords.InsertPtr(new SkipWord("cm"));
	SkipWords.InsertPtr(new SkipWord("dm"));
	SkipWords.InsertPtr(new SkipWord("m"));
	SkipWords.InsertPtr(new SkipWord("dam"));
	SkipWords.InsertPtr(new SkipWord("km"));
	SkipWords.InsertPtr(new SkipWord("mg"));
	SkipWords.InsertPtr(new SkipWord("cg"));
	SkipWords.InsertPtr(new SkipWord("dg"));
	SkipWords.InsertPtr(new SkipWord("g"));
	SkipWords.InsertPtr(new SkipWord("gr"));
	SkipWords.InsertPtr(new SkipWord("dag"));
	SkipWords.InsertPtr(new SkipWord("hg"));
	SkipWords.InsertPtr(new SkipWord("kg"));
	SkipWords.InsertPtr(new SkipWord("t"));
	SkipWords.InsertPtr(new SkipWord("ml"));
	SkipWords.InsertPtr(new SkipWord("cl"));
	SkipWords.InsertPtr(new SkipWord("dl"));
	SkipWords.InsertPtr(new SkipWord("dal"));
	SkipWords.InsertPtr(new SkipWord("hl"));
	SkipWords.InsertPtr(new SkipWord("in"));
	SkipWords.InsertPtr(new SkipWord("ft"));
	SkipWords.InsertPtr(new SkipWord("yd"));
	SkipWords.InsertPtr(new SkipWord("ml"));
}


//-----------------------------------------------------------------------------
int GALILEI::GLang::Compare(const GLang& lang) const
{
	return(strcmp(Code,lang.Code));
}


//-----------------------------------------------------------------------------
int GALILEI::GLang::Compare(const GLang* lang) const
{
	return(strcmp(Code,lang->Code));
}


//-----------------------------------------------------------------------------
int GALILEI::GLang::Compare(const char* code) const
{
	return(strcmp(Code,code));
}


//-------------------------------------------------------------------------------
RString& GALILEI::GLang::GetStemming(const RString& _kwd)
{
	RString *res=RString::GetString();

	(*res)=_kwd;
	res->StrLwr();
	return((*res));
}


//-----------------------------------------------------------------------------
bool GALILEI::GLang::ValidWord(const RStd::RString& kwd)
{
	char look[10];
	const char* ptr=kwd();
	char* ptr2;
	char len;
	bool v;

	// The first character must be a number.
	if(!isdigit(*ptr)) return(true);

	// Analyse word
	v=true;
	while(*ptr)
	{
		// Look for a number
		while((*ptr)&&(isdigit(*ptr)))
			ptr++;
		if(!(*ptr)) return(true);     // If not number found -> Valid word

		// put letters in look with maximal 10
		ptr2=look;
		len=0;
		while((*ptr)&&(!isdigit(*ptr))&&(!ispunct(*ptr)))
		{
			if(len<9)
				(*(ptr2++))=(*ptr);
			ptr++;
		}
		if(len<9)
		{
			// Look if it is a skipword
			(*ptr2)=0;
			v=!(SkipWords.IsIn<const char*>(look));
			if(v) return(true);
		}
	}
	return(v);
}


//-----------------------------------------------------------------------------
GALILEI::GLang::~GLang(void)
{
}
