/*

	GALILEI Research Project

	GDocAnalyse.h

	Analyse a document - Header.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GDocAnalyseH
#define GDocAnalyseH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <langs/glang.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GDocAnalyse class provides a method to analyse a document.
* @author Pascal Francq
* @short Vector Model Documents Analyse.
*/
class GDocAnalyse
{
	class WordWeight;

	/**
	* Corresponding session;
	*/
	GSession* Session;

	/**
	* Cursor on the different languages defined in the system.
	*/
	GLangCursor CurLangs;

	/**
	* All the word appaering in the current document.
	*/
	RStd::RDblHashContainer<WordWeight,unsigned,27,27,false>* Weights;

	/**
	* Current document to analyse.
	*/
	GDocVector* Doc;

	/**
	* Direct access to the words.
	*/
	WordWeight** Direct;

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
	* Options.
	*/
	GDocOptions* Options;

public:

	/**
	* Constructor.
	* @param s              Session.
	* @param opt            Options.
	*/
	GDocAnalyse(GSession* s,GDocOptions* opt) throw(bad_alloc);

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
	* @param weight         Weights of the words added during this analyze.
	*/
	void AddWord(const char* word,double weight) throw(bad_alloc);

	/**
	* This function construct a word.
	* @param ptr            Pointer that will be moved to the next word.
	* @param word           Where to store the word.
	* @param weight         Weights of the words added during this analyze.
	* @returns true if a word was extract.
	*/
	bool ExtractWord(const char* &ptr,RStd::RString& word,double weight);

	/**
	* Analyse a tag.
	* @param tag            Tag to analyse.
	* @param weight         Weights of the words added during this analyze.
	*/
	void AnalyseTag(RXML::RXMLTag* tag,double weight) throw(GException);

	/**
	* Analyse a link tag.
	* @param tag            Tag to analyse.
	* @param tmpDocs        A pointer to a container of docs to maintain the documents to be added.
	*/
	void AnalyseLinksTag(RXML::RXMLTag* tag,bool externalLinks ,RContainer<GDoc,unsigned int,false,true>* DocsToAdd) throw(GException);

	/**
	* This methods determine the language of the current structure studied,
	* i.e. the language with the maximal number of words of the stop-list
	* contained in the document and with a minimal value for the ratio of
	* stopwords that are in the document.
	*/
	void DetermineLang(void) throw(GException);

	/**
	* Construct the information about the current document and store it in
	* Words.
	*/
	void ConstructInfos(void) throw(GException);

public:

	/**
	* Analyse a XML representation of a document for a session and store the
	* results in this document.
	* @param xml            XML Representation used.
	* @param doc            Corresponding document.
	* @param tmpDocs        A container of docs to maintain the documents ro be added.
	*/
	void Analyse(GDocXML* xml,GDoc* doc,RContainer<GDoc,unsigned int,false,true>* tmpDocs=0) throw(GException);

	/**
	* Analyse a XML representation of a document for a session and computes
	* statistics about it.
	* @param xml            XML Representation used.
	*/
	void ComputeStats(GDocXML* xml) throw(GException);

	/**
	* @return Total number of words in the documents with stoplist.
	*/
	unsigned int GetN(void) {return(N);}

	/**
	* @return Total number of words in the documents.
	*/
	unsigned int GetV(void) {return(V);}

	/**
	* @return Total number of words in the stop-list.
	*/
	unsigned int GetS(void) {return(S);}

	/**
	* @param l              Index of the language.
	* @return Total number of words in the stop-list.
	*/
	unsigned int GetS(unsigned int l) {return(Sl[l]);}

	/**
	* @return Number of different words in the documents.
	*/
	unsigned int GetNdiff(void) {return(Ndiff);}

	/**
	* @return Number of different words in the documents.
	*/
	unsigned int GetVdiff(void) {return(Vdiff);}

	/**
	* @return Number of different words in the stop-list.
	*/
	unsigned int GetSdiff(void) {return(Sdiff);}

	/**
	* @param l              Index of the language.
	* @return Number of different words in the stop-list.
	*/
	unsigned int GetSdiff(unsigned int l) {return(Sldiff[l]);}

	/**
	*  update the feedback of the profiles and subprofiles
	*/
	void UpdateFdbks(GLang* oldlang, GLang* newlang);

	/**
	* Destructor.
	*/
	virtual ~GDocAnalyse(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
