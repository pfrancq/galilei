/*

	GALILEI Research Project

	GDocAnalyze.h

	Generic Document Analysis - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
#ifndef GDocAnalyzeH
#define GDocAnalyzeH


//------------------------------------------------------------------------------
// include files for R
#include <rdblhashcontainer.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <gdescription.h>
#include <gconcepttree.h>
#include <gtexttoken.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GDocAnalyze class analyzes a given document. In practice, it does the
 * following steps:
 * -# It determines the filter corresponding to the type of the document to
 *    analyze.
 * -# It uses the current tokenizer to extract the tokens from the text
 *    provided by the filter (child classes of GFilter).
 * -# The tokens are then passed to the analyzers in the order specified in the
 *    configuration to be treated (stemming, filtering, etc.).
 *
 * @internal
 * The class manages the list of tokens found in the current document, in the
 * container OrderTokens (where they are ordered by name) and in Tokens, as well
 * as their occurrences (Occurs).
 *
 * To avoid the allocation and deallocation of multiple chucks of memory, all
 * tokens and occurrences are stored once created (in MemoryTokens and
 * MemoryOccurs) and then reused during the next analyzes.
 * @author Pascal Francq
 * @short Document Analysis.
 */
class GDocAnalyze : public R::RDownload
{
protected:

	/**
	 * Current document analyzed.
	 */
	GDoc* Doc;

	/**
	 * Corresponding session.
	 */
	GSession* Session;

	/**
	 * Description to build during the analyze.
	 */
    GDescription Description;

	/**
	 * Concept Tree corresponding to the document.
	 */
	GConceptTree Struct;

	/**
	 * Language associated to the document.
	 */
	GLang* Lang;

	/**
	 * The tokenizer.
	 */
	GTokenizer* Tokenizer;

	/**
	 * The analyzers.
	 */
	R::RCastCursor<GPlugIn,GAnalyzer> Analyzers;

	/**
	 * Concepts related to the Dublin Core Metadata Initiative.
	 */
	GConceptType* DCMI;

   /**
    * Meta-concept representing the default textual content.
    */
   GConcept* Body;

  	/**
	 * Memory of tokens.
	 */
	R::RContainer<GTextToken,true,false> MemoryTokens;

   /**
    * Number of tokens from the memory used.
    */
   size_t NbMemoryTokensUsed;

	/**
	 * Memory of occurrences.
	 */
   R::RContainer<GTextToken::Occurrence,true,false> MemoryOccurs;

   /**
    * Number of occurrences from the memory used.
    */
   size_t NbMemoryOccursUsed;

	/**
	 * List of tokens currently added ordered.
	 */
	R::RDblHashContainer<GTextToken,false> OrderTokens;

	/**
	 * List of tokens currently added.
	 */
	R::RContainer<GTextToken,false,false> Tokens;

	/**
	 * The occurrences of the tokens.
	 */
	R::RContainer<GTextToken::Occurrence,false,false> Occurs;

	/**
	 * Vector for which new concepts should be added.
	 */
	GVector* CurVector;

	/**
	 * Current position.
	 */
	size_t CurPos;

	/**
	 * Current depth.
	 */
	size_t CurDepth;

public:

	/**
	* Constructor of the document analysis method.
	* @param session         Session.
	*/
	GDocAnalyze(GSession* session);

	/**
	 * Get the document currently analyzed.
	 * @return pointer to the document.
	 */
	GDoc* GetDoc(void) const {return(Doc);}

	/**
	 * Add a token to the current vector.
    * @warning This method should only be called by child classes of
    *          GTokenizer.
	 * @param token          Token to add.
	 */
	void AddToken(const R::RString& token);

	/**
	 * Add a concept to a vector associated with a given meta-concept.  If the
	 * vector already exists, the content is added.
	 * @param concept        Concept to add.
	 * @param weight         Weight associate to the concept.
	 * @param metaconcept    Meta-concept of the vector associated to the concept.
	 * @param pos            Position of the concept.
	 * @param depth          Depth of the concept.
	 */
	void AddConcept(GConcept* concept,double weight,GConcept* metaconcept,size_t pos,size_t depth=0);

	/**
	 * Extract some tokens of a given text, and add them to a vector associated
    * with a given meta-concept.  If the vector already exists, the content is
	 * added.
	 * @param text           Text to add.
	 * @param metaconcept    Meta-concept of the vector associated to the text.
	 * @param pos            Position of the text.
	 * @param depth          Depth of the text.
	 */
	void ExtractTextual(const R::RString& text,GConcept* metaconcept,size_t pos,size_t depth=0);

	/**
	 * Extract some tokens of a given text, and add them to a vector associated
    * with a given metadata defined by the Dublin core. In practice, to each
    * metadata corresponds one vector. Several contents associated with a given
    * metadata are simply added.
	 *
	 * The only allowed elements are: contributor, coverage, creator, date,
	 * description, format, identifier, language, publisher, relation, rights,
	 * source, subject, title, type.
	 *
	 * Typically, this method is called from a filter.
	 * @code
	 * void MyFilter::Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file)
	 * {
	 * 	...
	 * 	analyzer->AddDCMI("title","This is the title of the document");
	 * 	...
	 * }
	 * @endcode
	 * @param element        Element of the DCMI (without namespace and/or
	 *                       prefix).
	 * @param value          Value of the metadata.
	 * @param pos            Position of the value.
	 * @param depth          Depth of the value.
	 */
	void ExtractDCMI(const R::RString& element,const R::RString& value,size_t pos,size_t depth=0);

   /**
    * Extract some tokens from a given text, and add them to the 'body'
    * meta-concept. Each time the method is called, the content is added to the
	 * vector corresponding to the 'body' meta-concept.
    * @param content        Content.
	 * @param pos            Position of the content.
	 * @param depth          Depth of the content.
    */
   void ExtractBody(const R::RString& content,size_t pos,size_t depth=0);

	/**
	 * Assign the plug-ins. An exception is generated if no plug-ins are
	 * defined.
	 */
	void AssignPlugIns(void);

	/**
	 * Get a cursor over the tokens extracted. The order of the container
	 * reflects the order of the first occurrence of each token.
	 * @return a cursor.
	 */
	R::RCursor<GTextToken> GetTokens(void) const {return(R::RCursor<GTextToken>(Tokens));}

	/**
	 * Get a cursor over the occurrences of the different tokens extracted
	 * as they appear in the document.
	 * @return a cursor.
	 */
	R::RCursor<GTextToken::Occurrence> GetOccurs(void) const {return(R::RCursor<GTextToken::Occurrence>(Occurs));}

	/**
	 * Delete a given token.
	 * @warning This method may modified the cursor over the tokens.
	 * @param token          Token to delete.
	 */
	void DeleteToken(GTextToken* token);

	/**
	 * Replace a given token by a given value (for example a word by its stem).
	 * @warning This method may modified the cursor over the tokens.
	 * @param token          Token to replace.
	 * @param value          New value.
	 * @return true if the current token was replaced and false if it was
	 *         deleted (its occurrences being merge with an existing token).
	 */
	bool ReplaceToken(GTextToken* token,R::RString value);

	/**
	 * Get the language actually determined.
	 */
	GLang* GetLang(void) const {return(Lang);}

	/**
	 * Set the language for the document currently analyzed.
	 * @param lang
	 */
	void SetLang(GLang* lang);

private:

    /**
     * Create the vectors and the description
     */
    void PostAnalyze(void);

public:

	/**
	* Analyze a document.
	* @param doc             Pointer to the document to analyze.
	* @param ram             Must the description of the document be maintained
	*                        in RAM?
	*/
	void Analyze(GDoc* doc,bool ram=true);

	/**
	* Destruct the document analyzer.
	*/
	virtual ~GDocAnalyze(void);

	friend class GSession;
};


//-------------------------------------------------------------------------------
#define CREATE_DOCANALYZE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GDocAnalyze,plugin,"DocAnalyze",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
