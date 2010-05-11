/*

	GALILEI Research Project

	GTextAnalyze.h

	Analyze a document - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GTextAnalyzeH
#define GTextAnalyzeH


//------------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rdblhashcontainer.h>
#include <galilei.h>
#include <gdocanalyze.h>
#include <glang.h>
#include <gxmlindex.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
// Constants
const size_t MaxWordLen=50;


//-----------------------------------------------------------------------------
class cDepth;


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
	 * Corresponding concept.
	 */
	GConcept* Concept;

	/**
	 * Create a word.
	 * @param nblangs        Number of languages.
	 * @return
	 */
	cWord(size_t nblangs) : Word(MaxWordLen+1), InStop(0)
	{
		InStop=new bool[nblangs];
		Clear();
	}

	inline void Clear(void) {Word.SetLen(0); Nb=0; Weight=0.0; OnlyLetters=true; NormalStem=false; Concept=0;}
	int Compare(const cWord& word) const {return(Word.Compare(word.Word));}
	int Compare(const cWord* word) const {return(Word.Compare(word->Word));}
	int Compare(const RString& word) const {return(Word.Compare(word));}
	size_t HashIndex(size_t idx) const {return(Word.HashIndex(idx));}

	~cWord(void) {delete[] InStop;}
};


//-----------------------------------------------------------------------------
class cWords
{
	/**
	 * Words appearing in the current document.
	 */
	R::RContainer<cWord,true,false> Words;

	/**
	* Words appearing in the current document but accessible through an hash
	* table.
	*/
	R::RDblHashContainer<cWord,false> Hash;

	size_t NbWords;

	size_t NbLangs;

public:
	cWords(void);
	void Init(size_t nblangs);
	void Clear(void);
	inline RCursor<cWord> GetWords(void) const;
	inline cWord* GetWord(const RString& word,bool& find);
	inline size_t GetNbWords(void) const {return(NbWords);}
};


//-----------------------------------------------------------------------------
/**
 * The cStructToken class represent a structural token. If it is a tag,
 * associated with its content, it can be used as indexed.
 */
class cStructToken
{
	/**
	 * The tag indexed.
	 */
	RString Name;

	/**
	 * Number of times the tag occurs.
	 */
	size_t Occurs;

	/**
	 * Pointer to the corresponding concept.
	 */
	GConcept* Tag;

public:

	cStructToken(const RString& name) : Name(name), Occurs(0), Tag(0) {}
	int Compare(const cStructToken& tag) const {return(Name.Compare(tag.Name));}
	int Compare(const RString& tag) const {return(Name.Compare(tag));}
	void Clear(void) {Occurs=0; Tag=0;}
	inline RString GetName(void) const {return(Name);}
	inline size_t GetOccurs(void) const {return(Occurs);}
	inline GConcept* GetTag(void) const {return(Tag);}
	friend class cStructTokens;
};


//-----------------------------------------------------------------------------
class cStructTokens : private RContainer<cStructToken,true,true>
{
	size_t NbTags;
public:
	cStructTokens(void);
	void Clear(void);
	inline cStructToken* AddToken(GConceptType* space,const RString& token);
	inline void ChangeTokenNS(cStructToken* token,const RString& uri);
	inline RCursor<cStructToken> GetStructTokens(void) const;
	size_t GetNbTags(void) const {return(NbTags);}
	void Test(void);
};



//-----------------------------------------------------------------------------
/**
 * A node of the file, it can be a word or a structural token
 */
class cNode
{
public:

	/**
	 * Object of the node.
	 */
	union
	{
		cWord* Word;
		cStructToken* Tag;
	} Obj;

	/**
	 * Position in the file.
	 */
	size_t Pos;

	/**
	 * Corresponding record type.
	 */
	GVTDRec::RecType Type;

	/**
	 * Pointer to the depth.
	 */
	cDepth* Depth;

	/**
	 * Position of the first child.
	 */
	size_t Child;

	/**
	 * Number of child.
	 */
	size_t Nb;

	cNode(void);
	void Clear(void);
	void SetTag(cStructToken* tag,size_t pos,cDepth* depth);
	void SetAttr(cStructToken* tag,size_t pos,cDepth* depth);
	void SetAttrValue(cWord* word,size_t pos,cDepth* depth);
	void SetContent(cWord* word,size_t pos,cDepth* depth);
	int Compare(const cNode&) const {return(-1);}
};


//-----------------------------------------------------------------------------
class cDepth : private RContainer<cNode,false,false>
{
	size_t NbNodes;
	cNode* CurTag;  // Current tag
public:
	char Level;
	size_t NbNodesSkipped; // Number of nodes skipped because stopwords
	GXMLIndex* CurIndex;   // Current index.
	size_t NbCurIndex;     // Number of elements associated to the current index.

	cDepth(char level);
	void Clear(void);
	inline RCursor<cNode> GetNodes(void) const;
	int Compare(const cDepth&) const {return(-1);}
	inline size_t GetNbNodes(void) const {return(NbNodes);}
	inline char GetLevel(void) const {return(Level);}
	inline cNode* GetCurTag(void) const {return(CurTag);}
	friend class cDepths;
};


//-----------------------------------------------------------------------------
class cDepths : private RContainer<cDepth,true,false>
{
	size_t NbDepths;
	RContainer<cNode,true,false> Nodes;
	size_t NbNodes;

public:

	cDepths(void);
	void Clear(void);
	inline cDepth* GetDepth(char depth);
	inline RCursor<cDepth> GetDepths(void) const;
	inline cNode* GetNewNode(cDepth* depth,bool tag);
	inline size_t GetNbDepths(void) const {return(NbDepths);}
	inline RCursor<cNode> GetNodes(void) const;
	inline size_t GetNbNodes(void) const {return(NbNodes);}
};


//-----------------------------------------------------------------------------
/**
 * The GTextAnalyze class provides a method to analyze a document. The plug-in
 * maintains several lists of elements through the analysis of a XML documents:
 * - Words: The list of each word appearing in the document.
 * - StructTokens: The list of structural tokens appearing in the document.
 * - Depths: The different information associated to each depth of the
 *   document.
 * @author Pascal Francq
 * @short XML Documents Analyze.
 */
class GTextAnalyze : public GDocAnalyze
{
	/**
	* Cursor on the different languages defined in the system.
	*/
	R::RCastCursor<GPlugIn,GLang> CurLangs;

	/**
	 * Words.
	 */
	cWords Words;

	/**
	 * Index.
	 */
	cStructTokens StructTokens;

	/**
	 * Different depths of the XML file analyzed.
	 */
	cDepths Depths;

	/**
	* Number of words in the document.
	*/
	size_t N;

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
	 * Number of languages treated.
	 */
	size_t NbLangs;

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
	* Determine if the language must be find for the current document.
	*/
	bool FindLang;

	/**
	* Index of the language when defined.
	*/
	size_t LangIndex;

	/**
	 * Specify if the current document must be full index. It is the case if
	 * full indexing is asked, and the document is a native XML.
	 */
	bool MustFullIndex;

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
	char MaxDepth;

	/**
	 * Maximal number of occurrences of a tag to be considered as index.
	 */
	size_t MaxOccurs;

	/**
	 * May declarative tags have child tags?
	 */
	bool ChildTags;

	/**
	 * Should the meta-tag be detected or must they be defined.
	 */
	bool DetectMetaTag;

	/**
	 * Name of the file containing the meta-tags.
	 */
	RString MetaTagFile;

	/**
	 * Meta tags.
	 */
	RContainer<RString,true,true> MetaTags;

	/**
	 * Weight of each tag and parameter name when they are considered as stems.
	 */
	double WeightStruct;

	/**
	 * Must the attributes values be considered as stems?
	 */
	bool ExtractValues;

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
	 * Structure space.
	 */
	GConceptType* StructSpace;

	/**
	 * Index space.
	 */
	GConceptType* IndexSpace;

	/**
	 * Space of the language.
	 */
	GConceptType* LangSpace;

	/**
	 * Is the current tag a title?
	 */
	bool IsTitle;

	/**
	 * Is the current tag an identifier ?
	 */
	bool IsIdentifier;

	/**
	 * Temporary String;
	 */
	RString tmpStr;

	/**
	 * Last tag inserted.
	 */
	cStructToken* LastInsertTag;

	/**
	 * Native XML file.
	 */
	bool Native;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GTextAnalyze(GSession* session,GPlugInFactory* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Initialize the plug-in.
	*/
	virtual void Init(void);

	/**
	* Do some cleaning operations before a analyze.
	*/
	void Clear(void);

	// Overload default function
	virtual void BeginTag(const RString& namespaceURI,const RString& lName,const RString& name);
	virtual void ResolveNamespace(const RString& namespaceURI);
	virtual void AddAttribute(const RString& namespaceURI, const RString& lName, const RString& name);
	virtual void Value(const RString& value);
	virtual void EndTag(const RString& namespaceURI,const RString& lName,const RString& name);
	virtual void Text(const RString& text);

	/**
	 * Transform a code into a character. It call first the valid codes for
	 * XML, if the code is not valid it is replaced by a space.
	 * @param code           String representing the code.
	 * @return Character.
	 */
	virtual RChar CodeToChar(RString& code);

	/**
	* Add a valid word to the document.
	* @param word           Word to add.
	* @param weight         Weights of the words added during this analyze.
	* @param normal         Must the word be considered as a normal stem.
	* @
	*/
	void AddWord(const RString& word,double weight,bool letters,GVTDRec::RecType type);

	/**
	* This method extract valid words from a string.
	* @param str            String.
	* @param weight         Weights of the words added during this analyze.
	* @param normal         Must the word be considered as a normal stem.
	* @returns true if a word was extract.
	*/
	void ExtractValidWords(const RString& str,double weight,GVTDRec::RecType type);

	/**
	* This methods determine the language of the current structure studied,
	* i.e. the language with the maximal number of words of the stop-list
	* contained in the document and with a minimal value for the ratio of
	* stopwords that are in the document.
	*/
	void DetermineLang(void);

	/**
	 * Get a pointer to the concept that is inserted corresponding to a word.
	 * @param word           Word.
	 * @return Pointer to the concept or null if the word is not valid with
	 * respect to the constraints.
	 */
	GConcept* GetStemConcept(cWord* word);

	/**
	* Construct the information about the current document and store it in
	* Words.
	*/
	void ConstructInfos(void);

	/**
	 * Index the XML part.
	 */
	void IndexXMLPart(void);

	/**
	 * Prepare the analyze of a document. The method is called by 'GSession'
	 * before the XML document is read.
	 * @param doc             Document to analyze.
	 * @param native          Specify if the document is a native XML file.
	 */
	virtual void PrepareAnalyze(const GDoc* doc,bool native);

	/**
	* Terminate the analysis of a XML document. The method is called by
	* GSession after the XML document is read.
	*/
	virtual void TerminateAnalyze(void);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	 * @return true if the analyze of the current tag must be stopped.
	 */
	inline bool StopAnalyseTag(void) const
	{
		return((!Native) ||  ((!MustFullIndex)&&(   (!ExtractIndex)    || (    ExtractIndex&&(DetectMetaTag&&(GetCurrentDepth()>MaxDepth)) ) )) );
	}

	/**
	* Destruct.
	*/
	virtual ~GTextAnalyze(void);
};


//-----------------------------------------------------------------------------
#endif
