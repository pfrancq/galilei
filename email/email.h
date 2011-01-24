/*

	GALILEI Research Project

	EMail.h

	Filter for Mails - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
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
// forward declaration
class GFilterEMail;


//-----------------------------------------------------------------------------
/**
 * The EMailCmd class provides a representation for a given class of command.
 */
class EMailCmd
{
protected:
	RString Cmd;
	GFilterEMail* Filter;
public:

	EMailCmd(GFilterEMail* filter,const RString& cmd) : Cmd(cmd),Filter(filter) {}
	int Compare(const EMailCmd& cmd) const {return(Cmd.Compare(cmd.Cmd));}
	int Compare(const EMailCmd* cmd) const {return(Cmd.Compare(cmd->Cmd));}
	int Compare(const RString cmd) const {return(Cmd.Compare(cmd));}
	virtual void DoIt(const RString&) {}
	virtual ~EMailCmd(void) {}
};


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
	 * All available commands.
	 */
	RContainer<EMailCmd,true,true> Cmds;

public:

	/**
	* Construct the email filter for a specific email.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GFilterEMail(GSession* session,GPlugInFactory* fac);

protected:

	/**
	* Try to extract the command and the info after.
	* @param line            Line to analyze.
	* @return true if a command can be extracted, false if it is the end.
	*/
	bool ExtractCmd(const RString& line);

public:

	/**
	* Analyze a document with a given URI for which a DocXML must be created.
	* This method must be re-implemented by all filters.
	* @param doc             Document to analyze.
	* @param uri             URI of the file to analyze.
	* @param parser          Current parser of the XML stream.
	* @param rec             Receiver for the signals.
	*/
	virtual void Analyze(GDoc* doc,const RURI& uri,RXMLParser* parser,GSlot* rec);

	/**
	* Create the parameters.
	* @param fac             Factory.
	*/
	static void CreateParams(GPlugInFactory* fac);

	/**
	* Destructor of the email filter.
	*/
	virtual ~GFilterEMail(void);
};


//-----------------------------------------------------------------------------
#endif
