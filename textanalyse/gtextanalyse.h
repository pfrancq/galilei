/*

	GALILEI Research Project

	GDocAnalyse.h

	Analyse a document - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GDocAnalyseH
#define GDocAnalyseH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rdblhashcontainer.h>
#include <rstd/rcontainercursor.h>
#include <rxml/rxmltag.h>



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward declaration
class GDocXML;
class GDoc;
class GIWordOccurs;
class GIWordOccur;
class GMIMEFilter;
class GLang;
class GProfDoc;
class GDict;
class GSession;


//-----------------------------------------------------------------------------
/**
* The GDocAnalyse class provides a method to analyse a document.
* @author Pascal Francq
* @short Document's Analyse.
*/
class GDocAnalyse
{
	class WordOccur;

	/**
	* Corresponding session;
	*/
	GSession* Session;

	/**
	* Cursor on the different languages defined in the system.
	*/
	RContainerCursor<GLang,unsigned,true,true> CurLangs;

	/**
	* All the word appaering in the current document.
	*/
	RStd::RDblHashContainer<WordOccur,unsigned,27,27,false>* Occurs;

	/**
	* Direct access to the words.
	*/
	WordOccur** Direct;

	/**
	* Number of elements allocated in Direct;
	*/
	unsigned int NbDirect;

	/**
	* Number of words in the document.
	*/
	unsigned int N;

	/**
	* Number of different words in the document.
	*/
	unsigned int Ndiff;

	/**
	* Number of words of the stoplist for the different languages that are in
	* the document.
	*/
	unsigned int* Sl;

	/**
	* Number of different words of the stoplist for the different languages
	* that are in the document.
	*/
	unsigned int* Sldiff;

	/**
	* Number of words of the stoplist of the selected language that are in the
	* document.
	*/
	unsigned int Sdiff;

	/**
	* Number of different words of the stoplist of the selected language that
	* are in the document.
	*/
	unsigned int S;

	/**
	* Language actually considered.
	*/
	GLang* Lang;

	/**
	* Document actually Treated.
	*/
	GDoc* Doc;

	/**
	* Determine if the language must be find for the current document.
	*/
	bool FindLang;

	/**
	* Determine if the current word has only letters.
	*/
	bool OnlyLetters;

public:

	/**
	* Constructor.
	* @param s              Session.
	*/
	GDocAnalyse(GSession* s) throw(bad_alloc);

protected:

	/**
	* Do some cleaning operations before a analyse.
	*/
	void Clear(void);

	/**
	* Verify the size of direct and reallocate when necessary.
	*/
	void VerifyDirect(void) throw(bad_alloc);

	/**
	* Add a word to the document.
	* @param word           Word to add.
	*/
	void AddWord(const char* word) throw(bad_alloc);

	/**
	* This function construct a word.
	* @param ptr            Pointer that will be moved to the next word.
	* @param word           Where to store the word.
	* @returns true if a word was extract.
	*/
	bool ExtractWord(const char* &ptr,RStd::RString& word);

public:

	/**
	* Analyse a tag.
	* @param tag            Tag to analyse.
	*/
	void AnalyseTag(RXML::RXMLTag* tag) throw(GException);

	/**
	* Analyse a XML representation of a document for a session and store the
	* results in this document.
	* @param xml            XML Representation used.
	* @param doc            Corresponding document.
	*/
	void Analyse(GDocXML* xml,GDoc* doc) throw(GException);

	/**
	* Analyse a XML representation of a document for a session and computes
	* statistics about it.
	* @param xml            XML Representation used.
	*/
	void ComputeStats(GDocXML* xml) throw(GException);

public:

	/**
	* Destructor.
	*/
	virtual ~GDocAnalyse(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
