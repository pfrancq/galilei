/*

	GALILEI Research Project

	GFilterTXT.h

	Filter for TXT Files - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Vandaele Valery(vavdaele@ulb.ac.be).

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
#ifndef GFilterTXTH
#define GFilterTXTH


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
* The GFilterTXT class provides a representation of a document to filter in a
* XML structure.
* @author Vandaele Valery
* @short TXT Filter.
*/
class GFilterTXT : public GFilter
{
	/**
	* Buffer containing all the txt file.
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
	* the txts.
	*/
//	bool BlankLines;

public:

	/**
	* Construct the TXT filter for a specific TXT File.
	* @param mng            Manager.
	*/
	GFilterTXT(GURLManager* mng,bool b=false);

protected:

	/**
	* This function skip spaces.
	*/
	inline void SkipSpaces(void)
	{while((*Pos)&&isspace(*Pos)) Pos++;}

public:

	/**
	* Analyze a txt and fill the XML structure with the information content.
	*
	* Actually, only local files can be treated.
	* @param doc            XML Structure that will represent the email.
	*/
	virtual bool Analyze(GDocXML* doc);

	/**
	* Determine if blank lines are allowed between two commands.
	* @return true if it is allowed.
	*/
	//bool IsBlankLines(void);

	/**
	* Select if blank lines are allowed between two commands.
	* @param                    Allowed or not?
	*/
	//void SetBlankLines(bool b);

	/**
	* Destructor of the TXT filter.
	*/
	virtual ~GFilterTXT(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

 
