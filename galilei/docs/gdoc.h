/*

	GALILEI Research Project

	gdoc.h

	Document - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GDocH
#define GDocH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>
using namespace RStd;
#include <rxml/rxmltag.h>
using namespace RXML;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glangs/gdict.h>
#include <ginfos/giwordoccurs.h>
#include <glangs/glang.h>
//#include <gdocs/gdocxml.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward declaration
class GDocXML;
class GSession;
class GDocs;


//-----------------------------------------------------------------------------
/**
* The GDoc class provides a representation of the analysis of a document.
* @author Pascal Francq
*/
class GDoc
{
	/**
	* URL of the document.
	*/
	RString URL;

	/**
	* Identifier of the document.
	*/
	unsigned Id;

	/**
	* Information about the words containted in the document. Actually, only
	* the number of occurences of the words are used.
	*/
	GIWordOccurs Words;

	/**
	* Total number of words.
	*/
	unsigned NbWords;

	/**
	* Number of different words.
	*/
	unsigned NbDiffWords;

	/**
	* Pointer to the language.
	*/
	GLang* Lang;

	/**
	* Must the doc be analysed?
	*/
	bool Calc;

	/**
	* Must the doc be saved?
	*/
	bool bSave;

public:

	/**
	* Construct the document.
	* @param url            URL of the document.
	* @param id             Identifier of the document.
	* @param nbdiff         Number of different words appearing in the
	*                       document.
	*/
	GDoc(const RString& url,const unsigned int id,const unsigned int nbdiff=500) throw(bad_alloc);

	/**
	* Construct the document.
	* @param url            URL of the document.
	* @param id             Identifier of the document.
	* @param lang           Language of the document.
	* @param nb             Total number of words in the document.
	* @param nbdiff         Number of different words appearing in the
	*                       document.
	*/
	GDoc(const RString& url,const unsigned int id,GLang* lang,const unsigned int nb,const unsigned int nbdiff) throw(bad_alloc);

	/**
	* Compare function needed by RStd::RContainer.
	* @param doc            Document used for the comparaison.
	*/
	int Compare(const GDoc& doc);

	/**
	* Compare function needed by RStd::RContainer.
	* @param doc            Pointer to the document used for the comparaison.
	*/
	int Compare(const GDoc* doc);

	/**
	* Compare function needed by RStd::RContainer.
	* @param id            Identificator used for the comparaison.
	*/
	int Compare(const unsigned id);

protected:

	/**
	* This function construct a word.
	* @param ptr            Pointer that will be moved to the next word.
	* @param word           Where to store the word.
	* @returns true if a word was extract.
	*/
	bool ExtractWord(const char* &ptr,RString& word);

public:

	/**
	* Analyse a tag to find how much words are in a stoplist.
	* @param tag            Tag to analyse.
	* @param stop           Stoplist to use.
	*/
	int AnalyseTagForStopKwd(RXMLTag* tag,GDict* stop);

	/**
	* Analyse a content tag.
	* @param tag            Tag to analyse.
	* @param stop           Stoplist to use.
	* @param dic            Dictionnary to use.
	*/
	void AnalyseContentTag(RXMLTag* tag,GDict* stop,GDict* dic) throw(GException);

	/**
	* Analyse a XML representation of a document for a session and store the
	* results in this document.
	* @param xml            XML Representation used.
	* @param session        Corresponding session.
	*/
	void Analyse(GDocXML* xml,GSession* session) throw(GException);

	/**
	* Add a word with a certain occurences in the document.
	* @param id             Identificator of the word.
	* @param nb             Occurences of the word.
	*/
	void AddWord(const unsigned int id,const unsigned int nb);

	/**
	* @return Total number of words in the documents.
	*/
	unsigned int GetNbWords(void) {return(NbWords);}

	/**
	* @return Number of different words in the documents.
	*/
	unsigned int GetNbDiffWords(void) {return(NbDiffWords);}

	/**
	* Destruct the document.
	*/
	virtual ~GDoc(void);

	// friend classes
	friend class GWordCalcs;
	friend class GDocs;
};




}  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
