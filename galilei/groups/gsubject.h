/*

	GALILEI Research Project

	Gsubjects.h

	GSubject - Implementation.

	(C) 1999-2001 by J.Lamoral & D.Wartel

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



//---------------------------------------------------------------------------
#ifndef GSUBJECTH
#define GSUBJECTH


//-----------------------------------------------------------------------------
// include files for R-Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>
#include <rio/rtextfile.h>
#include <rstd/rnode.h>
using namespace RIO;
using namespace RStd;

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <docs/gdoc.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
/** This Class implement a representation for a subject used to create judgment
*  and feedback.
*  @author Julien Lamoral
*  @short  Subject
*/
class GSubject: public RNode<GSubject,false>
{
protected:

	/**
	* The name of the subject.
	*/
	RString Name;

	/**
	* The id of the subject.
	*/
	int Id;

	/**
	* Return true if the subsubject is judged.
	*/
	bool isjudged;

public:

	/**
	* The container of document in the subject.
	*/
	RContainer<GDoc,unsigned,false,true>* urls;

	/**
	* Constructor
	* @param name            The name of the subsubject.
	* @param id              the subject id.
	*/
	GSubject(RString name, int id);

	/**
	* Compare a subject with a given name.
	* @param name           Name used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const char* name) {return(Name.Compare(name));}

	/**
	* Compare two subject.
	* @param id            Tag used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const GSubject *sub) {return(Name.Compare(sub->Name));}

	/**
	* Compare two subject.
	* @param id            Tag used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const GSubject &sub) {return(Name.Compare(sub.Name));}

	/**
	* Comparaison function.
	* @param id            id used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const unsigned int &id) const {return(Id-id);};

	/**
	* Return the name of the Subject.
	* @returns a string containing the name.
	*/
	RString GetName(void) {return(Name);};

	/**
	* Return the id of the Subject.
	* @returns a string containing the name.
	*/
	int GetId(void) {return(Id);};

	/**
	* The language of a subsubject
	*/
	char* lang;

	/**
	* Return isjudged.
	*/
	bool isJudged(void) {return(isjudged);}

	/**
	* Set the bool isjudged.
	*/
	void setIsJudged(bool b) {isjudged=b;}

	/**
	* Set the bool isjudged.
	*/
	int SubSubjectMinId(void);

	/**
	* Destructor.
	*/
	~GSubject(void){};

}; //-------- End of namespace GALIELI ----------------------------------------


//-----------------------------------------------------------------------------

#endif


