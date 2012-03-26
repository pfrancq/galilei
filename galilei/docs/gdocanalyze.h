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
#include <rstack.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <gdescription.h>
#include <gtoken.h>
#include <gconcepttree.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GDocAnalyze class analyzes a given document by coordinating the
 * following steps:
 * -# It determines the filter corresponding to the type of the document to
 *    analyze.
 * -# It uses the current tokenizer to extract the tokens from the text
 *    provided by the filter (child classes of GFilter).
 * -# The tokens are then passed to the analyzers in the order specified in the
 *    configuration to be treated (stemming, filtering, etc.).
 *
 * In practice, it manages the tokens extracted from the documents by the filter
 * and their occurrences (position, depth and syntactic position). Once the
 * analysis steps are finished, it build a vector and a concept tree using the
 * tokens for which a concept is associated.
 *
 * It is supposed that each token of a given type in a document as an unique
 * name and corresponds to one concept only. It is the responsibility of the
 * filter to ensure it.

 * Once a document is analyzed, it send a notification 'DocAnalyzed'. It can be
 * catched by all classes inhereting from R::RObject that become an observator
 * with the command (generally in the constructor):
 * @code
 * InsertObserver(HANDLER(TheClass::Handle),"DocAnalyzed");
 *
 * @endcode
 * A method must then by created:
 * @code
 * void TheClass::Handle(const R::RNotification& notification)
 *	{
 *	   GDocAnalyze* Analyzer(GetData<GDocAnalyze*>(notification));
 *    cout<<Analyzer->GetLang()->GetCode()<<endl;
 * }
 * @endcode
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
class GDocAnalyze : R::RDownload
{
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
	GConceptTree Tree;

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
	R::RContainer<GToken,true,false> MemoryTokens;

   /**
    * Number of tokens from the memory used.
    */
   size_t NbMemoryTokensUsed;

	/**
	 * Memory of occurrences.
	 */
   R::RContainer<GTokenOccur,true,false> MemoryOccurs;

   /**
    * Number of occurrences from the memory used.
    */
   size_t NbMemoryOccursUsed;

	/**
	 * List of tokens currently added ordered.
	 */
	R::RDblHashContainer<GToken,false> OrderTokens;

	/**
	 * List of tokens currently added.
	 */
	R::RContainer<GToken,false,false> Tokens;

	/**
	 * The occurrences of the tokens.
	 */
	R::RContainer<GTokenOccur,false,false> Occurs;

	/**
	 * Top occurrences.
	 */
	R::RContainer<GTokenOccur,false,false> Top;

	/**
	 * A stack representing the "active" tokens at each depth.
	 */
	R::RStack<GTokenOccur,false,true,true> Depths;

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

	/**
	 * Current syntactic position.
    */
	size_t CurSyntacticPos;

	/*
	 * Container of syntactic positions in each document of a given concept.
	 */
	R::RContainer<R::RNumContainer<size_t,false>,true,false> SyntacticPos;

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
	 * @return the session.
	 */
	GSession* GetSession(void) const {return(Session);}

	/**
	 * @return the description that was just computed.
	 */
   const GDescription& GetDescription(void) const {return(Description);}

	/**
	 * @return the tree that was just computed (if asked).
    */
	const GConceptTree& GetTree(void) const {return(Tree);}

	/**
	 * Inform the document analysis process that a potential token is skipped. In
	 * practice, it increments the current syntactic position.
	 *
	 * Typically, it is called by the current tokenizer to indicate that an
	 * existing character sequence is not considered as a valid token.
	 * @return the syntactic position skipped.
    */
	size_t SkipToken(void);

private:

	/**
	 * Create a token with a given name and a given type. In practice, if a free
	 * token exists, it is used.
    * @param token          Token.
    * @param type           Type.
    * @return a pointer to the created token.
    */
	GToken* CreateToken(const R::RString& token,tTokenType type);

public:

	/**
	 * Add a token to the current vector.
	 *
	 * The current syntactic position is incremented by one.
    * @warning This method should only be called by child classes of
    *          GTokenizer.
	 * @param token          Token to add.
	 * @param type           Token type.
	 * @return the occurrence of the token added.
	 */
	GTokenOccur* AddToken(const R::RString& token,tTokenType type);

	/**
	 * Add a token of a given type and representing a concept. It is added to a
	 * vector associated with a given meta-concept.
	 *
	 * The current syntactic position is incremented by one.
	 * @param token          Token to add. The name must be unique for a given
	 *                       document whatever its type.
	 * @param type           Token type.
	 * @param concept        Concept to add.
	 * @param weight         Weight associate to the concept.
	 * @param metaconcept    Meta-concept of the vector associated to the concept.
	 * @param pos            Position of the concept.
	 * @param depth          Depth of the concept.
	 * @param spos           Syntactic position. If SIZE_MAX, the token is
	 *                       supposed to be next the previous one.
	 * @return the occurrence of the token added.
	 */
	GTokenOccur* AddToken(const R::RString& token,tTokenType type,GConcept* concept,double weight,GConcept* metaconcept,size_t pos,size_t depth=0,size_t spos=SIZE_MAX);

	/**
	 * Extract some tokens of a given text, and add them to a vector associated
    * with a given meta-concept.  If the vector already exists, the content is
	 * added.
	 *
	 * The current syntactic position is incremented by the number of tokens
	 * extracted.
	 * @param text           Text to add.
	 * @param metaconcept    Meta-concept of the vector associated to the text.
	 * @param pos            Position of the text.
	 * @param depth          Depth of the text.
	 * @param spos           Syntactic position. If SIZE_MAX, the token is
	 *                       supposed to be next the previous one.
	 */
	void ExtractTextual(const R::RString& text,GConcept* metaconcept,size_t pos,size_t depth=0,size_t spos=SIZE_MAX);

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
	 * The current syntactic position is incremented by the number of tokens
	 * extracted.
	 * @param element        Element of the DCMI (without namespace and/or
	 *                       prefix).
	 * @param value          Value of the metadata.
	 * @param pos            Position of the metadata.
	 * @param depth          Depth of the metadata.
	 * @param spos           Syntactic position. If SIZE_MAX, the token is
	 *                       supposed to be next the previous one.
	 */
	void ExtractDCMI(const R::RString& element,const R::RString& value,size_t pos,size_t depth=0,size_t spos=SIZE_MAX);

   /**
    * Extract some tokens from a given text, and add them to the 'body'
    * meta-concept. Each time the method is called, the content is added to the
	 * vector corresponding to the 'body' meta-concept.
	 *
 	 * The current syntactic position is incremented by the number of tokens
	 * extracted.
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
	R::RCursor<GToken> GetTokens(void) const {return(R::RCursor<GToken>(Tokens));}

	/**
	 * Get a cursor over the occurrences of the different tokens extracted
	 * as they appear in the document.
	 * @return a cursor.
	 */
	R::RCursor<GTokenOccur> GetOccurs(void) const {return(R::RCursor<GTokenOccur>(Occurs));}

	/**
	 * Delete a given token. In practice, it modifies its type to ttDeleted.
	 * @warning This method may modified the cursor over the tokens.
	 * @param token          Token to delete.
	 */
	void DeleteToken(GToken* token);

	/**
	 * Replace a given token by a given value (for example a word by its stem).
	 * If it new value corresponds to an existing token, the occurrences are
	 * merged and the type of the current token is set to ttDeleted.
	 * @warning This method may modified the cursor over the tokens.
	 * @param token          Token to replace.
	 * @param value          New value.
	 */
	void ReplaceToken(GToken* token,R::RString value);

	/**
	 * Move a token occurrence associated to a particular token to another one
	 * given by a value. If it new value corresponds to an existing token, the
	 * occurrence is added. If the current token has no more occurrences, its
	 * type is set to ttDeleted.
	 * @warning This method may modified the cursor over the tokens.
	 * @param occur          Token occurrence to change.
	 * @param value          New value.
	 */
	void MoveToken(GTokenOccur* occur,R::RString value);

	/**
	 * Move a token occurrence associated to a particular token to another
	 * existing concept. If necessary, a new token is created. If the current
	 * token has no more occurrences its type is set to ttDeleted.
	 * @warning This method may modified the cursor over the tokens.
	 * @param occur          Token occurrence to change.
	 * @param concept        Concept.
	 */
	void MoveToken(GTokenOccur* occur,GConcept* concept);

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
     * Create the descriptions.
     */
    void BuildTensor(void);

    /**
     * Build the concept tree starting with a given token occurrence.
	  * @param parent        Parent node.
	  * @param occur         Token occurrence.
     */
    void BuildTree(GConceptNode* parent,GTokenOccur* occur);

    /**
     * Print the concept tree starting with a given token occurrence.
	  * @param occur         Token occurrence.
     */
    void Print(GTokenOccur* occur);

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
	friend class GObjects<GDoc>;
};


//-------------------------------------------------------------------------------
#define CREATE_DOCANALYZE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GDocAnalyze,plugin,"DocAnalyze",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
