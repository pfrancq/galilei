/*

	GALILEI Research Project

	GFilterEMail.h

	Filter for Mails - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <sessions/galilei.h>
#include <docs/gfilter.h>



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
	* Determine if blank lines are allowed between commands in
	* the mails.
	*/
	bool BlankLines;

public:

	/**
	* Construct the email filter for a specific email.
	* @param fac            Factory.
	* @param name           Name of the filter.
	*/
	GFilterEMail(GFactoryFilter* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

protected:

	/**
	* Try to extract the command and the info after.
	* @param line           Line to analyse.
	* @param metaData       Pointer to the tag representing the metaData.
	* @return true if a command can be extracted, false if it is the end.
	*/
	bool ExtractCmd(const RString& line);

public:

	/**
	* Analyze an email and fill the XML structure with the information content.
	*
	* Actually, only local files can be treated.
	* @param doc            XML Structure that will represent the email.
	*/
	virtual bool Analyze(GDocXML* doc) throw(std::bad_alloc,GException);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	* Destructor of the email filter.
	*/
	virtual ~GFilterEMail(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
