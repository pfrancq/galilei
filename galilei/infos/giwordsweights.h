/*

	GALILEI Research Project

	GIWordsWeights.h

	Weights of a list of words - Header.

	(C) 2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$
	
*/



//-----------------------------------------------------------------------------
#ifndef GIWordsWeightsH
#define GIWordsWeightsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/ginfo.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GIWordWeight;
class GDoc;
class GLang;


//-----------------------------------------------------------------------------
/**
* The GIWordsWeights provides a representation for the words weights of a given
* collection of words, for example a document or a set of documents.
* It implements the vector representing a document in the Vector Model.
* @author Pascal Francq
* @short Words Weights.
*/
class GIWordsWeights : public GInfo,public RStd::RContainer<GIWordWeight,unsigned,true,true>
{
	/**
	* Total number of words in the set of documents analysed.
	*/
	double NbWordsDocs;

	/**
	* Languages of the documents.
	*/
	GLang* Lang;

	/**
	* Hold the same list but in decreasing order of frequencies.
	*/
	GIWordWeight** Order;

	/**
	* Pointer to go trough the ordered list.
	*/
	GIWordWeight** CurOrder;

public:

	/**
	* Constructor.
	* @param lang           Language.
	* @param nb             Maximal number of word created at initialisation.
	*/
	GIWordsWeights(GLang *lang,unsigned int nb) throw(bad_alloc);

	/**
	* Return the name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* Return the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GIWordsWeights& c) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GIWordsWeights* c) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GLang* l) const;

private:

	/**
	* Static function used to ordered by frenquecy.
	*/
	static int sortOrder(const void *a,const void *b);

public:

	/**
	* Clear the statistics.
	*/
	void Clear(void);

	/**
	* Analyse a document and udpated the statistics.
	* @param doc            Document to analyse.
	*/
	void Analyse(GDoc* doc);

	/**
	* When all the documents are analysed, call this method to compute the frequencies.
	*/
	void EndCalc(void);

	/**
	* Get the identificator of the next word in the ordered list by frequencies.
	* @returns Identificator of the word.
	*/
	unsigned int NextWord(void);

	/**
	* Look if there a word to treat.
	*/
	bool IsNextWord(void) const {return(*CurOrder);}

	/**
	* Destructor.
	*/
	virtual ~GIWordsWeights(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
