/*

	GALILEI Research Project

	GALILEI.h

	GALILEI Header - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GALILEIH
#define GALILEIH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rstring.h>


/**
* \namespace GALILEI
* \brief GALILEI Classes.
*
* This namespace declares all the classes implemented during the GALILEI
* project.
*/


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// General constant
const unsigned cNoRef=0xFFFFFFFF;        // A undefined identificator reference


//-----------------------------------------------------------------------------
/**
* Represent different methods to compute the profiles.
*/
enum tProfileCompute{pcNothing,pcStatistical};


//-----------------------------------------------------------------------------
/**
* The GException class provides a basic representation for an exception.
* @author Pascal Francq
* @short Basic Exception.
*/
class GException
{
	/**
	* Message holding some information.
	*/
	RStd::RString Msg;

	/**
	* A code representing the exception.
	*/
	unsigned int Code;

public:

	/**
	* Predefined Constants for Errors.
	*/
	enum eErrorType{NoDefinedError=0,OpenSessionError=1,Languages=2,Dic=3,Docs=4};

	/**
	* Construct an exception.
	* @param str                      Message of the error.
	*/
	GException(const RStd::RString& str) throw(bad_alloc)
		: Msg(str), Code(NoDefinedError) {}

	/**
	* Construct an exception.
	* @param code                     Code of the error.
	* @param str                      Message of the error.
	*/
	GException(const unsigned code,const RStd::RString& str) throw(bad_alloc)
		: Msg(str), Code(code) {}

	/**
	* Get the content of the exception.
	* @returns Pointer to a C String.
	*/
	const char* GetMsg(void) const {return(Msg());}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
