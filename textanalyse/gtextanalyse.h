/*

	GALILEI Research Project

	GTextAnalyse.h

	Analyze a document - Header.

	Copyright 2001-2008 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef GTextAnalyseH
#define GTextAnalyseH


//------------------------------------------------------------------------------
// include files for R Project
#include <rdblhashcontainer.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdocanalyse.h>
#include <glang.h>
#include <gdocstruct.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
class cWordOccur
{
public:

	GDocStructNode* Parent;
	size_t Pos;

	cWordOccur(GDocStructNode* parent,size_t pos) : Parent(parent), Pos(pos) {}
	int Compare(const cWordOccur&) const {return(-1);}
};


//-----------------------------------------------------------------------------
/**
 * The cWord class represent a word (sequence of characters found in the
 * document.
 */
class cWord
{
public:

	/**
	 * Word.
	 */
	RString Word;

	/**
	 * Verify if the word is a stopword in different languages.
	 */
	bool* InStop;

	/**
	 * Number of occurrences of the word.
	 */
	size_t Nb;

	/**
	 * Weight of the word (maybe different from the number of occurrences).
	 */
	double Weight;

	/**
	 * Has the word only letters.
	 */
	bool OnlyLetters;

	/**
	 * Must the word be considered as a normal stem.
	 */
	bool NormalStem;

	/**
	 * Occurrences of the word in the XML document.
	 */
	R::RContainer<cWordOccur,true,false> Occurs;

	/**
	 * Create a word.
	 * @param nblangs        Number of languages.
	 * @return
	 */
	cWord(size_t nblangs);

	inline void Clear(void) {Word=RString::Null; Nb=0; Weight=0.0; NormalStem=false; Occurs.Clear();}
	int Compare(const cWord& word) const {return(Word.Compare(word.Word));}
	int Compare(const RString& word) const {return(Word.Compare(word));}
	size_t HashIndex(size_t idx) const {return(Word.HashIndex(idx));}
	~cWord(void);
};


//-----------------------------------------------------------------------------
class cContent : public RContainer<cWord,false,false>
{
public:

	cContent(void) : RContainer<cWord,false,false>(10) {}

	int Compare(const cContent&) const {return(-1);}
};


//-----------------------------------------------------------------------------
/**
 * The IndexTag class represent a tag that, associated with its content, can be
 * used as indexed.
 */
class IndexTag : public RContainer<cContent,true,false>
{
public:

	/**
	 * The tag indexed.
	 */
	RString Name;

	/**
	 * Number of times the tag occurs.
	 */
	size_t Occurs;

	IndexTag(const RString& name) : RContainer<cContent,true,false>(60), Name(name), Occurs(0) {}
	int Compare(const IndexTag& tag) const {return(Name.Compare(tag.Name));}
	int Compare(const RString& tag) const {return(Name.Compare(tag));}
};


//-----------------------------------------------------------------------------
/**
* The GTextAnalyse class provides a method to analyze a document.
* @author Pascal Francq
* @short Vector Model Documents Analyze.
*/
class GTextAnalyse : public GDocAnalyse
{
	/**
	* Current document to analyze.
	*/
	GDoc* Doc;

	/**
	 * Structure computed.
	 */
	GDocStruct* Struct;

	/**
	* Cursor on the different languages defined in the system.
	*/
	R::RCursor<GLang> CurLangs;

	/**
	 * Words appearing in the current document.
	 */
	R::RContainer<cWord,true,false> Words;

	/**
	* Words appearing in the current document but accessible through an hash
	* table.
	*/
	R::RDblHashContainer<cWord,false> Hash;

	/**
	* Information computed.
	*/
	R::RContainer<GWeightInfo,false,true> Infos;

	/**
	 * Index.
	 */
	RContainer<IndexTag,true,true> IndexTags;

	/**
	* Number of words in the document.
	*/
	size_t N;

	/**
	* Number of different words in the document.
	*/
	size_t Ndiff;

	/**
	* Number of words not in the stoplist.
	*/
	size_t Nwords;

	/**
	* Total number of valid words.
	*/
	size_t V;

	/**
	* Number of different valid words.
	*/
	size_t Vdiff;

	/**
	* Number of words of the stoplist for the different languages that are in
	* the document.
	*/
	size_t* Sl;

	/**
	* Number of different words of the stoplist for the different languages
	* that are in the document.
	*/
	size_t* Sldiff;

	/**
	* Number of words of the stoplist of the selected language that are in the
	* document.
	*/
	size_t Sdiff;

	/**
	* Number of different words of the stoplist of the selected language that
	* are in the document.
	*/
	size_t S;

	/**
	* Language actually considered.
	*/
	GLang* Lang;

	/**
	* Determine if the language must be find for the current document.
	*/
	bool FindLang;

	/**
	* Index of the language when defined.
	*/
	size_t LangIndex;

	/**
	* Define if the language are static.
	*/
	bool StaticLang;

	/**
	* Minimum percentage of words of a document which must be in the stop list
	* of a language to make the language the current one.
	*/
	double MinStopWords;

	/**
	* Minimum number of characters to have a valid word.
	*/
	size_t MinWordSize;

	/**
	* Minimum number of characters to have a valid stem.
	*/
	size_t MinStemSize;

	/**
	* Minimum number of occurrences needed to insert a valid word in the list of
	* information for a document.
	*/
	size_t MinOccur;

	/**
	* Determine if the extracted words may contain other things than letters.
	*/
	bool NonLetterWords;

	/**
	* Filter the word extracted?
	*/
	bool Filtering;

	/**
	* Maximal occurrences of a same word in a row to consider a word non-valid.
	*/
	size_t NbSameOccur;

	/**
	* Value of the ratio of normal letters on the total number of letters to be
	* considered as a valid.
	*/
	double NormalRatio;

	/**
	 * Store pairs (full word,stem) in the database?
	 */
	bool StoreFullWords;

	/**
	 * Extract structure elements?
	 */
	bool ExtractStruct;

	/**
	 * Full indexing of the documents (structure and positions).
	 */
	bool FullIndex;

	/**
	 * Should the structure be considered as normal content (stems)?
	 */
	bool StructIsContent;

	/**
	 * Extract pairs (declarative tags,content)?
	 */
	bool ExtractIndex;

	/**
	 * Maximum number of terms allowed for a declarative tag's content.
	 */
	size_t MaxTerms;

	/**
	 * Maximum depth of a declarative tag
	 */
	size_t MaxDepth;

	/**
	 * Maximal percentage of occurrences of a tag to be considered as index.
	 */
	double MaxPercOccurs;

	/**
	 * Maximal number of occurrences of a tag to be considered as index.
	 */
	double MaxOccurs;

	/**
	 * May declarative tags have child tags?
	 */
	bool ChildTags;

	/**
	 * Weight of each tag and parameter name when they are considered as stems.
	 */
	double WeightStruct;

	/**
	 * Must the attributes values be considered as stems?
	 */
	bool AttrValues;

	/**
	 * Weight of each parameter value when they are considered as stems.
	 */
	double WeightValues;

	/**
	 * Must a default namespace be used if tags are found with no namespace.
	 */
	bool UseDefaultNamespace;

	/**
	 * Default namespace to use.
	 */
	RString DefaultNamespace;

	/**
	 * Number of tags.
	 */
	size_t NbTags;

	/**
	 * Structure space.
	 */
	GConceptType* StructSpace;

	/**
	 * Index space.
	 */
	GConceptType* IndexSpace;

public:

	/**
	* Constructor.
	* @param fac             Factory.
	*/
	GTextAnalyse(GFactoryDocAnalyse* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session);

protected:

	/**
	* Do some cleaning operations before a analyze.
	*/
	void Clear(void);

	/**
	* Add a word to the document.
	* @param word           Word to add.
	* @param weight         Weights of the words added during this analyze.
	* @param normal         Must the word be considered as a normal stem.
	* @
	*/
	void AddWord(const R::RString& word,double weight,cContent* content,GDocStructNode* parent,size_t pos,bool letters);

	/**
	* This method extract the new word from a given string if it responds to
	* the constraints established.
	* @param str            String.
	* @param weight         Weights of the words added during this analyze.
	* @param normal         Must the word be considered as a normal stem.
	* @returns true if a word was extract.
	*/
	void ExtractWord(const R::RString& str,double weight,cContent* content,GDocStructNode* parent,size_t pos);

	/**
	* This methods determine the language of the current structure studied,
	* i.e. the language with the maximal number of words of the stop-list
	* contained in the document and with a minimal value for the ratio of
	* stopwords that are in the document.
	*/
	void DetermineLang(void);

	GConcept* GetStemConcept(cWord* word);

	/**
	* Construct the information about the current document and store it in
	* Words.
	* @param documentid            Corresponding document id.
	*/
	void ConstructInfos(size_t documentid);

	/**
	* Insert into database a couple word/stem
	* @param stemid            Corresponding stemmed word id.
	* @param word            Original word.
	* @param docid            Corresponding document id.
	*/
	bool StoreWordStemInDatabase(size_t stemid, RString word,size_t docid);

public:

	/**
	* Analyze a XML representation of a document for a session and store the
	* results in this document.
	*
	* During the analysis of the document, if links point to document that does
	* not exist in the system, these documents are added in a container.
	* @param xml            XML Representation used.
	* @param doc            Corresponding document.
	* @param native          Specify if the document is a native XML file.
	*/
	virtual void Analyze (GDoc *doc,const R::RURI& uri,bool native);

	/**
	 * Index the XML part.
	 */
	void IndexXMLPart(void);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GTextAnalyse(void);

	friend class XMLParser;
};


//-----------------------------------------------------------------------------
#endif
