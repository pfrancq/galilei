/*

	GALILEI Research Project

	GLangs.h

	List of the languages - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GLangsH
#define GLangsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangs/glang.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLangs class provides a representation for a list of languages that are
* handle by the system.
* @author Pascal Francq
* @short Languages.
*/
class GLangs : public RStd::RContainer<GLang,unsigned,true,true>
{
public:
	/**
	* Constructor.
	* @param nb             Number of Languages.
	*/
	GLangs(unsigned nb) throw(bad_alloc);

	/**
	* Get a pointer to a language while having its code.
	* @param code             Code of the language.
	* @returns Pointer to the language.
	*/
	GLang* GetLang(const char* code);

	/**
	* Destructor.
	*/
	virtual ~GLangs(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
