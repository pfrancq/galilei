/*

	Wikipedia Plug-Ins

	Wikipedia.cpp

	Build a GALILEI database for Wikipedia - Header

	Copyright 2012 by Pascal Francq (pascal@francq.info).

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
// include files for R/GALILEI
#include <rxmlparser.h>
#include <gtool.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for current project
#include <../wikitoken/wikitoken.h>


//------------------------------------------------------------------------------
/**
 * The Wikipedia tool uses dumps from Wikipedia to update the database with the
 * documents, their descriptions and the users who modify them.
 * @short Wikipedia Dump Extractor.
 */
class Wikipedia : public GTool
{
	/**
	 * Directory where the Wikipedia files are stored.
	 */
	RString Dir;

	/**
	 * Date of the dump.
	 */
	RDate Dump;

	/**
	 * Must the articles be imported.
	 */
	bool ImportArticles;

	/**
	 * Number of articles to treat. If null, all articles are treated.
	 */
	size_t NbArticles;

	/**
	 * Must the revisions be imported?
	 */
	bool ImportRevisions;

	/**
	 * Must all the revisions from the treated articles must be imported?
	 */
	bool ImportAllRevisions;

	/**
	 * Number of revisions to treat. If null, all contributions are treated.
	 */
	size_t NbRevisions;

	/**
	 * Must the external URI be extracted from the wiki?
	 */
	bool ExtractExternalURI;

	/**
	 * Must all the users with the same username be merged?
	 */
	bool MergeUsers;

	/**
	 * String representing the date of the dump.
	 */
	RString DumpStr;

	/**
	 * Force all Wikipedia articles to be analyzed.
	 */
	bool ForceAnalyze;

	/**
	 * Number of articles treated.
	 */
	size_t NbTreatedArticles;

	/**
	 * Number of analyzed articles.
	 */
	size_t NbAnalyzedArticles;

	/**
	 * Number of feedbacks.
	 */
	size_t NbFdbks;

	/**
	 * Slot
	 */
	GSlot* Slot;

	/**
	 * Document corresponding to the current Wikipedia articles.
	 */
	GDoc* Article;

	/**
	 * Status of the article in GALILEI.
	 */
	enum {New,Update,OK} Status;

	/**
	 * Title of the article.
	 */
	RString ArticleTitle;

	/**
	 * Date where the article was updated in Wikipedia.
	 */
	RDate ArticleUpdated;

	/**
	 * Pointer to the English language.
	 */
	GLang* LangEn;

	/**
	 * Possible states of a Wikipedia article content.
	 */
	enum tState {sText,sLink,sCite,sRef,sComment,sTable,sDiv,sTag,sExternalLink,sCode};

	/**
	 * State of the parser.
	 */
	tState State;

	/**
	 * Position of the current token.
	 */
	size_t CurPos;

	/**
	 * Syntactic position of the current token.
	 */
	size_t CurSyntacticPos;

	/**
	 * Pointer to the current section in the notice.
	 */
	WikiToken* CurSection;

	/**
	 * Token that holds the current link. Depending of the link, it is inserted or
	 * not in the list of tokens to be treated by the parser.
	 */
	WikiToken CurLink;

	/**
	 * Can a link contain another link ?
	 */
	bool IncludedLinks;

	/**
	 * Character 1 used as latest for a Bracket
    */
	char BracketCar1;

	/**
	 * Character 2 used as latest for a Bracket
    */
	char BracketCar2;

	/**
	 * Number of brackets that are imbricate.
    */
	size_t NbBrackets;

	/**
	 * Current tag.
    */
	RString CurTag;

	/**
	 * Is a <page> tag currently treated.
	 */
	bool InPage;

	/**
	 * Is a <text> tag currently treated.
	 */
	bool InText;

	/**
	 * Number of lines currently treated.
    */
	off_t NbLines;

	/**
	 * Current position.
	 */
	off_t Pos;

	/**
	 * Position of the first character of a line readed.
	 */
	off_t Act;

	/**
	 * Size of the text currently treated.
	 */
	off_t Size;

	/**
	 * Is the current page a redirection ?
    */
	bool Redirect;

	/**
	 * Line currently treated.
	 */
	RString Line;

	/**
	 * Temporary string.
	 */
	RString Tmp;

	/**
	 * Content of the current page.
    */
	RString Content;

public:

	/**
	 * Constructor.
    * @param session        Session calling the tool.
    * @param fac            Factory that manages the plug-in.
    */
	Wikipedia(GSession* session,GPlugInFactory* fac);

	/**
	 * Get the number of articles to treat.
	 * @return a size_t
	 */
	size_t GetNbArticles(void) const {return(NbArticles);}

	/**
	 * Get the number of revisions to treat.
	 * @return a size_t
	 */
	size_t GetNbRevisions(void) const {return(NbRevisions);}

	/**
	 * Must all Wikipedia articles be (re)analyzed ?
	 * @return true if yes.
	 */
	bool MustForceAnalyze(void) const {return(ForceAnalyze);}

	/**
	 * Must the external URI be extracted from the wiki?
	 * @return true if yes.
	 */
	bool MustExtractExternalURI(void) const {return(ExtractExternalURI);}

	/**
	 * Must all the users with the same username be merged?
	 * @return true if yes.
	 */
	bool MustMergeUsers(void) const {return(MergeUsers);}

	/**
	 * Must all the revisions from the treated articles must be imported?
	 * @return true if yes.
	 */
	bool MustImportAllRevisions(void) const {return(ImportAllRevisions);}

	/**
	 * Get the slot.
	 * @return a pointer to a GSlot.
	 */
	GSlot* GetSlot(void) const {return(Slot);}

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Compute the statistics.
	*/
	virtual void Run(GSlot* slot);

	/**
	 * Analyze the Wikipedia pages.
    */
	void AnalyzePages(void);

	/**
	 * Analyze the current line as a content between <page> and </page>.
    */
	void AnalyzeInPage(void);

	/**
	 * Create a link if necessary.
	 * @return true if a link was created.
    */
	inline bool CreateLink(void);

	/**
	 * Treat the current line that is a wiki.
    */
	void TreatWiki(void);

	/**
	 * Analyze the Wikipedia logs.
	 */
	void AnalyzeLogs(void);

	/**
	 * Extract a string between tags.
    * @param line
    * @param text
	 */
	void ExtractFromTag(const RString& line,RString& text) const;

	/**
	 * Look if a notice has a corresponding document in GALILEI. If not, it is
	 * created if asked. The status of the document is updated.
	 * @param create         If true, the document is created if it does not
	 *                       exist.
    */
	void LookDoc(bool create);

	/**
	 * Get a profile with a given identifier. If not existing, it is created.
    * @param profid         Profile identifier.
	 * @param name           Name of the profile.
    * @return a pointer to a profile.
    */
	GProfile* GetProfile(size_t profid,RString& name);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	 * Destructor.
    */
	virtual ~Wikipedia(void);

	friend class Pages;
};
