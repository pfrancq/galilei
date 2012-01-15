/*

	GALILEI Research Project

	EMail.h

	Filter for Mails - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gfilter.h>

using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
/**
* The GFilterEMail class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
* @short EMails' Filter.
*/
class GFilterEMail : public GFilter
{
    class EMailCmd;

	/**
	 * All available commands.
	 */
	RContainer<EMailCmd,true,true> Cmds;

public:

	/**
	* Construct the email filter.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GFilterEMail(GSession* session,GPlugInFactory* fac);

protected:

	/**
   * Try to extract the command and the info after.
   * @param analyzer        Analyzer.
	* @param line            Line to analyze.
   * @param pos             Position of the line.
	* @return true if a command can be extracted, false if it is the end.
	*/
	bool ExtractCmd(GDocAnalyze* analyzer,const RString& line,size_t pos);

public:

	/**
	* Analyze a document with a given URI.
	* @param analyzer        Analyzer.
	* @param doc             Document to analyze.
	* @param file            File to analyze (eventually a local copy of a
	*                        remote document).
	*/
	virtual void Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file);

	/**
	* Destructor of the email filter.
	*/
	virtual ~GFilterEMail(void);
};


//-----------------------------------------------------------------------------
#endif
