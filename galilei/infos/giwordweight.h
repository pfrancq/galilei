/*

	GALILEI Research Project

	GIWordWeight.h

	Weight of a word - Header.

	(C) 2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GIWordWeightH
#define GIWordWeightH


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <infos/giword.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GIWordWeight class provides a representation for a weight of a word in a
* given collection of words, for example a document.
* @author Pascal Francq
* @short Word Weight.
*/
class GIWordWeight : public GIWord
{
	/**
	* Frequence of the word.
	*/
	double Weight;

public:

	/**
	* Constructor.
	* @param id             Identificator of the word.
	*/
	GIWordWeight(const unsigned int id) throw(bad_alloc);

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
	int Compare(const GIWordWeight &calc) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GIWordWeight *calc) const;

	/**
	* Destructor.
	*/
	virtual ~GIWordWeight(void);

	// friend classes
	friend class GIWordsWeights;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
