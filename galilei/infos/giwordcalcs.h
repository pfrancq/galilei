/*

	GALILEI Research Project

	GIWordcalcs.h

	Frequences of words appearing in a set a documents - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$
	
*/



//-----------------------------------------------------------------------------
#ifndef GIWordCalcsH
#define GIWordCalcsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/ginfo.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward class declaration
class GIWordCalc;
class GDoc;
class GLang;


//-----------------------------------------------------------------------------
/**
* The GIWordCalcs provides a representation for the frequences of words that
* are appearing in a set of documents. This documents must all be of the same
* language.
* @author Pascal Francq
* @short Frequences of words appearing in a set a documents.
*/
class GIWordCalcs : public GInfo,public RStd::RContainer<GIWordCalc,unsigned,true,true>
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
	GIWordCalc** Order;

	/**
	* Pointer to go trough the ordered list.
	*/
	GIWordCalc** CurOrder;

public:

	/**
	* Constructor.
	* @param lang           Language.
	* @param nb             Maximal number of word created at initialisation.
	*/
	GIWordCalcs(GLang *lang,unsigned int nb) throw(bad_alloc);

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
	int Compare(const GIWordCalcs& c) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GIWordCalcs* c) const;

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
	virtual ~GIWordCalcs(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
