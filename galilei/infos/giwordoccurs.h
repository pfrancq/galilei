/*

	GALILEI Research Project

	giwordoccurs.h

	Container of word occurencies - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GIWordOccursH
#define GIWordOccursH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <ginfos/giwordoccur.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GWordOccurs class provides a representation for a list of words with
* their occurences.
*/
class GIWordOccurs : public GInfo,public RStd::RContainer<GIWordOccur,unsigned,true,true>
{
public:
	/**
	* Constructor.
	* @param nb             Number of words in the container.
	*/
	GIWordOccurs(unsigned nb) throw(bad_alloc);

	/**
	* Return the name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* Return the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* Return a pointer to a word's occurence. If the word doesn't exist, it is
	* created and inserted.
	* @param id             Identificator of the word.
	* @returns Pointer to the word.
	*/
	GIWordOccur* GetPtr(unsigned id) throw(bad_alloc);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
