/*

	GALILEI Research Project

	GTextAnalyse.h

	Analyse a document - Header.

	Copyright 2001-2007 by the Université libre de Bruxelles.

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


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdocanalyse.h>
#include <glang.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
* The GTextAnalyse class provides a method to analyse a document.
* @author Pascal Francq
* @short Vector Model Documents Analyse.
*/
class GTextAnalyse : public GDocAnalyse
{
	class WordWeight;

	/**
	* Current document to analyse.
	*/
	GDoc* Doc;
	
	/**
	* Cursor on the different languages defined in the system.
	*/
	R::RCursor<GFactoryLang> CurLangs;

	/**
	* All the word appearing in the current document.
	*/
	R::RDblHashContainer<WordWeight,false>* Weights;

	/**
	* Information computed.
	*/
	R::RContainer<GWeightInfo,false,true> Infos;

	/**
	* Direct access to the words.
	*/
	WordWeight** Direct;

	/**
	* Number of elements allocated in Direct;
	*/
	unsigned int NbDirect;

	/**
	* Order of the words in the document.
	*/
	GConcept** Order;

	/**
	* Number of elements allocated in Order.
	*/
	unsigned int NbOrder;

	/**
	* Number of words in the document.
	*/
	unsigned int N;

	/**
	* Number of different words in the document.
	*/
	unsigned int Ndiff;

	/**
	* Number of words not in the stoplist.
	*/
	unsigned int Nwords;

	/**
	* Total number of valid words.
	*/
	unsigned int V;

	/**
	* Number of different valid words.
	*/
	unsigned int Vdiff;

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
	* Determine if the language must be find for the current document.
	*/
	bool FindLang;

	/**
	* Determine if the current word has only letters.
	*/
	bool OnlyLetters;

	/**
	* Index of the language when defined.
	*/
	unsigned int LangIndex;

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
	unsigned int MinWordSize;

	/**
	* Minimum number of characters to have a valid stem.
	*/
	unsigned int MinStemSize;

	/**
	* Minimum number of occurences needed to insert a valid word in the list of
	* information for a document.
	*/
	unsigned int MinOccur;

	/**
	* Determine if the extracted words may contain other things than letters.
	*/
	bool NonLetterWords;

	/**
	* Filter the word extracted?
	*/
	bool Filtering;

	/**
	* Maximal occurences of a same word in a row to consider a word non-valid.
	*/
	unsigned int NbSameOccur;

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
	 * Should the structure be considered as normal content (stems)?
	 */
	bool StructIsContent;
	
	/**
	 * Extract pairs (declarative tags,content)?
	 */
	bool ExtractIndex;
	
	/**
	 * Maximum number of stems allowed for a declartive tag's content.
	 */
	unsigned int MaxStems;
	
	/**
	 * Maximum depth of a declative tag
	 */
	unsigned int MaxDepth;
	
	/**
	 * May declarative tags have child tags?
	 */
	bool ChildTags;
	
	double WeightStruct;
	
	bool AttrValues;
	
	double WeightValues;
	
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
	* Do some cleaning operations before a analyse.
	*/
	void Clear(void);

	/**
	* Verify the size of direct and reallocate when necessary.
	*/
	void VerifyDirect(void);

	/**
	* See if a given word is a valid one, don't content text and numbers that
	* are to skip.
	*/
	bool ValidWord(const R::RString kwd);

	/**
	* Verify the size of direct and reallocate when necessary.
	*/
	void VerifyOrder(void);

	/**
	* Add a word to the document.
	* @param word           Word to add.
	* @param weight         Weights of the words added during this analyze.
	*/
	void AddWord(const R::RString word,double weight);

	/**
	* This function construct a word.
	* @param ptr            Pointer that will be moved to the next word.
	* @param weight         Weights of the words added during this analyze.
	* @returns true if a word was extract.
	*/
	bool ExtractWord(const R::RChar* &ptr,double weight);

	/**
	* This methods determine the language of the current structure studied,
	* i.e. the language with the maximal number of words of the stop-list
	* contained in the document and with a minimal value for the ratio of
	* stopwords that are in the document.
	*/
	void DetermineLang(void);

	/**
	* Construct the information about the current document and store it in
	* Words.
	* @param documentid            Corresponding document id.
	*/
	void ConstructInfos(unsigned int documentid);

	/**
	* Insert into database a couple word/stem
	* @param stemid            Corresponding stemmed word id.
	* @param word            Original word.
	* @param docid            Corresponding document id.
	*/
	bool StoreWordStemInDatabase(unsigned int stemid, RString word, unsigned int docid);

public:

	/**
	* Analyse a XML representation of a document for a session and store the
	* results in this document.
	*
	* During the analysis of the document, if links point to document that does
	* not exist in the system, these documents are added in a container.
	* @param xml            XML Representation used.
	* @param doc            Corresponding document.
	* @param tmpDocs        Container that will hold the added docs.
	*/
	virtual void Analyze (GDoc *doc, const R::RURI& uri);

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
