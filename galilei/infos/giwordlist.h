/*

	GALILEI Research Project

	GIWordList.h

	Represents a list of words' references - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GIWordListH
#define GIWordListH


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
class GIWord;


//-----------------------------------------------------------------------------
/**
* The GIWordList class provides a representation for a list of words'
*references. In particular, the list has an identifier, which can be used for
* storing it.
* @author Pascal Francq
* @short List of words' references.
*/
class GIWordList : public GInfo,public RStd::RContainer<GIWord,unsigned,true,true>
{
	/**
	* Identifier of the list.
	*/
	unsigned Id;

public:

	/**
	* Constructor of the list.
	*/
	GIWordList(void) throw(bad_alloc);

	/**
	* Return the name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* Return the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* Get the identifier of the list.
	*  @returns Identifier.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier of the list.
	* @param id             Identifier to assign.
	*/
	void SetId(const unsigned int id) {Id=id;}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
