/*

	GALILEI Research Project

	GWord.h

	Word - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GWordH
#define GWordH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GWord class provides a representation for a word.
* @author Pascal Francq
* @short Word.
*/
class GWord
{
	/**
	* Identificator of the word.
	*/
	unsigned Id;

	/**
	* The word.
	*/
	RStd::RString Word;

public:

	/**
	* Default constructor for a word. The word is not a valid one.
	*/
	GWord(void) throw(bad_alloc);

	/**
	* Construct a word but without to know which will be its identificator.
	* @param word           Word.
	*/
	GWord(const RStd::RString& word) throw(bad_alloc);

	/**
	* Construct a word.
	* @param id             Identificator of the word.
	* @param word           Word.
	*/
	GWord(unsigned id,const RStd::RString& word) throw(bad_alloc);

	/**
	* Copy constructor.
	* @param word           Word used as reference.
	*/
	GWord(const GWord& word) throw(bad_alloc);

	/**
	* Compare method used by RStd::RContainer and RStd::RHashContainer.
	*/
	int Compare(const GWord& word) const;

	/**
	* Compare method used by RStd::RContainer and RStd::RHashContainer.
	*/
	int Compare(const GWord* word) const;

	/**
	* Compare method used by RStd::RContainer and RStd::RHashContainer.
	*/
	int Compare(const RStd::RString& word) const;

	/**
	* Compare method used by RStd::RContainer and RStd::RHashContainer.
	*/
	int Compare(const unsigned int id) const;

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static char HashIndex(const GWord* w)
		{return(RStd::RString::HashIndex(w->Word));}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static char HashIndex(const GWord& w)
		{return(RStd::RString::HashIndex(w.Word));}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static char HashIndex(const char* word)
		{return(RStd::RString::HashIndex(word));}

	/**
	* @return Identificator of the word.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* @return A C string representing the word.
	*/
	const char* GetWord(void) const {return(Word);}

	// friend classes
	friend class GDict;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
