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
#include <rtimedate/rdate.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward declaration
class GDocXML;
class GSession;
class GDocs;
class GIWordOccurs;
class GIWordOccur;
class GMIMEFilter;
class GLang;
class GProfDoc;
class GDict;


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
	GIWordOccurs* Words;

	/**
	* Total number of words with stoplist.
	*/
	unsigned int TotalWords;

	/**
	* Total number of words without stoplist.
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
	* State of the document.
	*/
	tObjState State;

	/**
	* Type of the document.
	*/
	GMIMEFilter* Type;

	/**
	* Date of last update of document's content.
	*/
	RTimeDate::RDate Updated;

	/**
	* Date of last document's analysis.
	*/
	RTimeDate::RDate Computed;

	/**
	* Profiles which have judge the current document.
	*/
	RContainer<GProfDoc,unsigned,false,true> Fdbks;

	/**
	* Count the number of downloads failed.
	*/
	unsigned int Failed;

public:

	/**
	* Construct the document.
	* @param url            URL of the document.
	* @param title          Name of the document.
	* @param id             Identifier of the document.
	* @param lang           Language of the document.
	* @param t              MIME type of the document.
	* @param u              String representing the date of the last update.
	* @param a              String representing the date of the last analysis.
	* @param f              Number of fails.
	* @param tot            Total number of words in the document with stoplist.
	* @param nb             Total number of words in the document without stoplist.
	* @param nbdiff         Number of different words appearing in the
	*                       document.
	* @param nbf            Number of Feedbacks.
	*/
	GDoc(const char* url,const char* name,unsigned int id,GLang* lang,GMIMEFilter* t,const char* u,const char* a,unsigned int f,unsigned int tot,unsigned int nb,unsigned int nbdiff,unsigned int nbf=100) throw(bad_alloc);

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

	/**
	* Get the date of the last update of the document's content.
	* @returns Pointer to date.
	*/
	const RTimeDate::RDate* GetUpdated(void) const {return(&Updated);}

	/**
	* Get the date of the last analysis of the document.
	* @returns Pointer to date.
	*/
	const RTimeDate::RDate* GetComputed(void) const {return(&Computed);}

	/**
	* Get the MIME type.
	* @returns Pointer to a MIME type.
	*/
	GMIMEFilter* GetMIMEType(void) const {return(Type);}

	/**
	* Return the state of the document.
	* @returns GALILEI::tObjState value.
	*/
	tObjState GetState(void) const {return(State);}

	/**
	* Set the state of the document.
	* @param state          New state.
	*/
	void SetState(tObjState state) {State=state;}

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
	int AnalyseTagForStopKwd(RXML::RXMLTag* tag,GDict* stop) throw(GException);

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
	* @return Total number of words in the documents with stoplist.
	*/
	unsigned int GetTotalWords(void) {return(TotalWords);}

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
	* @return Pointer to the language of the document.
	*/
	GLang* GetLang(void) {return(Lang);}

	/**
	* @return Identificator of the document.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

	/**
	* Get the number of failed for the document.
	* @returns unsigned int.
	*/
	unsigned int GetFailed(void) const {return(Failed);}

	/**
	* Initialise the number of attemps to 0.
	*/
	void InitFailed(void) {Failed=0;}

	/**
	* Increase the number of failed attemp.
	*/
	void IncFailed(void) {Failed++;}

	/**
	* Decrease the number of failed attemp.
	*/
	void DecFailed(void) {Failed--;}

	/**
	* Start the iterator to go trough the word.
	*/
	void WordsStart(void);

	/**
	* Test if the end of the container of words is reached.
	*/
	bool WordsEnd(void) const;

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	void WordsNext(void);

	/**
	* Get the current word.
	* @returns Pointer to the current word.
	*/
	GIWordOccur* GetCurWords(void);

	/**
	* Start the iterator to go trough the profiles judging the document.
	*/
	void ProfilesStart(void);

	/**
	* Test if the end of the container of profiles judging the document is
	* reached.
	*/
	bool ProfilesEnd(void) const;

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	void ProfilesNext(void);

	/**
	* Get the current profile judging the document.
	* @returns Pointer to the current profile.
	*/
	GProfDoc* GetCurProfiles(void);

	/**
	* Add a judgement for this document.
	* @param j              Judgement.
	*/
	void AddJudgement(GProfDoc* j) throw(bad_alloc);

	/**
	* Destruct the document.
	*/
	virtual ~GDoc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
