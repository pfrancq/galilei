/*

	GALILEI Research Project

	GFilterEMail.h

	Filter for Mails - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

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
#ifndef GFilterEMailH
#define GFilterEMailH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gfilter.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilterEMail class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
* @short EMails' Filter.
*/
class GFilterEMail : public GFilter
{
	/**
	* Buffer containing all the email.
	*/
	char* Buffer;

	/**
	* Pointer to the current position in the buffer.
	*/
	char* Pos;

	/**
	* Pointer to the beginning of the block actually treated.
	*/
	char* Begin;

	/**
	* Determine if blank lines are allowed between commands in
	* the mails.
	*/
	bool BlankLines;

public:

	/**
	* Construct the email filter for a specific email.
	* @param mng            Manager.
	*/
	GFilterEMail(GURLManager* mng,bool b=false);

protected:

	/**
	* This function skip spaces.
	*/
	inline void SkipSpaces(void)
	{while((*Pos)&&isspace(*Pos)) Pos++;}

	/**
	* Try to extract the command and the info after.
	* @param line           Line to analyse.
	* @param metaData       Pointer to the tag representing the metaData.
	* @return true if a command can be extracted, false if it is the end.
	*/
	bool ExtractCmd(char* line,R::RXMLTag* metaData);

public:

	/**
	* Analyze an email and fill the XML structure with the information content.
	*
	* Actually, only local files can be treated.
	* @param doc            XML Structure that will represent the email.
	*/
	virtual bool Analyze(GDocXML* doc);

	/**
	* Determine if blank lines are allowed between two commands.
	* @return true if it is allowed.
	*/
	bool IsBlankLines(void);

	/**
	* Select if blank lines are allowed between two commands.
	* @param                    Allowed or not?
	*/
	void SetBlankLines(bool b);

	/**
	* Destructor of the email filter.
	*/
	virtual ~GFilterEMail(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
