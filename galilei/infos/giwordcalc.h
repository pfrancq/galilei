/*

	GALILEI Research Project

	GIWordCalc.h

	Frequence of a word in a set of documents - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GIWordCalcH
#define GIWordCalcH


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <ginfos/giword.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GIWordCalc class provides a representation for a frequence of a word in a
* set of documents.
* @author Pascal Francq
* @short Word's Frequence in documents' set.
*/
class GIWordCalc : public GIWord
{
	/**
	* Frequence of the word.
	*/
	double Freq;

public:

	/**
	* Constructor.
	* @param id             Identificator of the word.
	*/
	GIWordCalc(const unsigned int id) throw(bad_alloc);

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
	int Compare(const GIWordCalc &calc) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GIWordCalc *calc) const;

	/**
	* Destructor.
	*/
	virtual ~GIWordCalc(void);

	// friend classes
	friend class GIWordCalcs;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
