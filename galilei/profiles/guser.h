/*

	GALILEI Research Project

	guser.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GUserH
#define GUserH


//-----------------------------------------------------------------------------
//include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gprofiles/gprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward declaration
class GUsers;


//-----------------------------------------------------------------------------
// class GUser

class GUser : public RStd::RContainer<GProfile,unsigned,true,true>
{
public:

	GUsers *Owner;
	/**
	* Identifier
	*/
	unsigned Id;
	
	/*
	* Name
	*/
	RString Name;
	
	/**
	* Number of session
	*/
	unsigned SessNum;    // Number of session

	/**
	* Construct a GUser
	* @param owner			parent owners
	*/
	GUser(GUsers *owner) throw(bad_alloc);
	
	/** comparaison functions
	*/
	int Compare(const unsigned int id);
	
	/** comparaison functions
	*/
	int Compare(const GUser &user);
	
	/** comparaison functions
	*/
	int Compare(const GUser *user);
	
	/** virtual Load function
	*/
//	virtual void Load(void) throw(bad_alloc,GException)=0;

};

}  //-------- End of namespace GALILEI ----------------------------------------

//-----------------------------------------------------------------------------

#endif


