/*
	GALILEI Research Project

	GLinkCalc.h

	Generic Link Method - Header.

	Copyright 2003 by the Universit� Libre de Bruxelles.

	Authors
		 Vandaele Valery (vavdaele@ulb.ac.be)

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GLinkCalcH
#define GLinkCalcH


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <galilei.h>
#include <docs/glinks.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLinkCalc provides a representation for a generic method to compute links.
* @author  Vandaele Valery
* @short generic link computation.
*/
class GLinkCalc
{
private :

	/**
	* Name of the link method.
	*/
	R::RString ComputingName;
	
protected :

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Container of outgoing links.
	*/
	R::RContainer<GLinks,unsigned int,true,true>* Links_Out;

	/**
	* remain status of the graph
	*/
	bool Inited;

public :

	/**
	* constructor of GLinkCalc
	* @param name           Name.
	* @param session        Session
	*/
	GLinkCalc(const char* name,GSession* session) throw(bad_alloc);

	/**
	* Compute a profile using link method
	* @params Prof       The profile to compute.
	*/
	virtual void Compute(GSubProfile* subprof)=0;

	/**
	* Init the algorithm for link computation
	*/
	void InitGraph(void);

	/**
	* Add a new doc (not present in the initAlgo phase).
	*/
	void AddDoc(GDoc* doc);

	/**
	* Get the name of the computing method.
	* @returns Pointer to a C string.
	*/
	const char* GetComputingName(void) const {return(ComputingName);}

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void) {return("");}

	/**
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*) {}

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GLinkCalc& lnkCal) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GLinkCalc* lnkCalc) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const char* name) const;

	/**
	* destructor of GLinkCalc
	*/
	virtual ~GLinkCalc(void);
};

//-----------------------------------------------------------------------------
/**
* The GLinkCalcCursor class provides a way to go trough a set of Link
* description method.
* @short Link Description Methods Cursor
*/
CLASSCURSOR(GLinkCalcCursor,GLinkCalc,unsigned int)

}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
