/*

	GALILEI Research Project

	GLang.h

	Basic Language - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

*/



//-----------------------------------------------------------------------------
#ifndef GLangH
#define GLangH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>
#include <rinter/rlang.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLang class provides a representation for a basic language. The virtual
* function GetStemming must be implemented for the different languages.
* @author Pascal Francq.
* @short Generic Language.
*/
class GLang : public RInter::RLang
{
protected:

	/**
	* Defines if the language is activ.
	*/
	bool Activ;

	/**
	* This class represents words that must be skipped when numbers are
	* presented. For example: 16nd, or 12h20.
	*/
	class SkipWord
	{
	public:
		RString Word;
	
		SkipWord(const char* w) : Word(w) {}
		int Compare(const SkipWord* w)
			{return(Word.Compare(w->Word));}
		int Compare(const SkipWord& w)
			{return(Word.Compare(w.Word));}
		int Compare(const char* w)
			{return(Word.Compare(w));}
	};

	RStd::RContainer<SkipWord,unsigned int,true,true> SkipWords;

public:

	/**
	* Constructor of a language.
	* @param lang           Name of the language.
	* @param code           Code of the language.
	*/
	GLang(const RStd::RString& lang,const char* code) throw(bad_alloc);

	/**
	* Comparaison function used by the Container.
	* @param lang           Pointer of the language used for comparaison.
	*/
	int Compare(const GLang& lang) const;

	/**
	* Comparaison function used by the Container.
	* @param lang           Language used of the comparaison.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	* @param code           Code used for the comparaison.
	*/
	int Compare(const char* code) const;

	/**
	* Function that return stemming of a word.
	* @param kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	virtual RString& GetStemming(const RStd::RString& kwd);

	/**
	* See if a given word is a valid one, don't content text and numbers that
	* are to skip.
	*/
	bool ValidWord(const RStd::RString& kwd);

	/**
	* Destructor.
	*/
	virtual ~GLang(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
