/*

	GALILEI Research Project

	GDoc.h

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
#include <rstd/rstring.h>
#include <rxml/rxmltag.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glangs/gdict.h>
#include <ginfos/giwordoccurs.h>
#include <glangs/glang.h>
#include <gdocs/gmimetype.h>



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
* @short Document.
*/
class GDoc
{
	/**
	* URL of the document.
	*/
	RStd::RString URL;

	/**
	* Name of the document.
	*/
	RStd::RString Name;

	/**
	* Identifier of the document.
	*/
	unsigned int Id;

	/**
	* Information about the words containted in the document. Actually, only
	* the number of occurences of the words are used.
	*/
	GIWordOccurs Words;

	/**
	* Total number of words.
	*/
	unsigned int NbWords;

	/**
	* Number of different words.
	*/
	unsigned int NbDiffWords;

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
	GDoc(const RStd::RString& url,const RStd::RString& name,const unsigned int id,unsigned int nbdiff=500) throw(bad_alloc);

	/**
	* Construct the document.
	* @param url            URL of the document.
	* @param id             Identifier of the document.
	* @param lang           Language of the document.
	* @param nb             Total number of words in the document.
	* @param nbdiff         Number of different words appearing in the
	*                       document.
	*/
	GDoc(const RStd::RString& url,const RStd::RString& name,const unsigned int id,GLang* lang,const unsigned int nb,const unsigned int nbdiff) throw(bad_alloc);

	/**
	* Compare function needed by RStd::RContainer.
	* @param doc            Document used for the comparaison.
	*/
	int Compare(const GDoc& doc) const;

	/**
	* Compare function needed by RStd::RContainer.
	* @param doc            Pointer to the document used for the comparaison.
	*/
	int Compare(const GDoc* doc) const;

	/**
	* Compare function needed by RStd::RContainer.
	* @param id            Identificator used for the comparaison.
	*/
	int Compare(const unsigned id) const;

	/**
	* Compare function needed by RStd::RContainer.
	* @param lang           Language used for the comparaison.
	*/
	int Compare(const GLang*) const;

	/**
	* Get the URL.
	* @returns Pointer to a C string.
	*/
	const char* GetURL(void) const {return(URL());}

	/**
	* Get the Name.
	* @returns Pointer to a C string.
	*/
	const char* GetName(void) const {return(Name());}

protected:

	/**
	* This function construct a word.
	* @param ptr            Pointer that will be moved to the next word.
	* @param word           Where to store the word.
	* @returns true if a word was extract.
	*/
	bool ExtractWord(const char* &ptr,RStd::RString& word);

public:

	/**
	* Analyse a tag to find how much words are in a stoplist.
	* @param tag            Tag to analyse.
	* @param stop           Stoplist to use.
	*/
	int AnalyseTagForStopKwd(RXML::RXMLTag* tag,GDict* stop);

	/**
	* Analyse a content tag.
	* @param tag            Tag to analyse.
	* @param stop           Stoplist to use.
	* @param dic            Dictionnary to use.
	*/
	void AnalyseContentTag(RXML::RXMLTag* tag,GDict* stop,GDict* dic) throw(GException);

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
	* @return Pointer to the Language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* @return True if the document has to be calc.
	*/
	bool MustAnalyse(void) {return(Calc);}

	/**
	* @return Pointer to the language of the document.
	*/
	GLang* GetLang(void) {return(Lang);}

	/**
	* @return Identificator of the document.
	*/
	unsigned int GetId(void) {return(Id);}

	/**
	* Start the iterator to go trough the word.
	*/
	inline void WordsStart(void)
		{Words.Start();}

	/**
	* Test if the end of the container of words is reached.
	*/
	inline bool WordsEnd(void) const
		{return(Words.End());}

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	inline void WordsNext(void)
		{Words.Next();}

	/**
	* Get the current word.
	* @returns Pointer to the current word.
	*/
	GIWordOccur* GetCurWords(void) {return(Words());}

	/**
	* Destruct the document.
	*/
	virtual ~GDoc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
