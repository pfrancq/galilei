/*

	GALILEI Research Project

	HTML.h

	A HTML filter - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2003 by Valery Vandaele.
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
#ifndef GFilterHTMLH
#define GFilterHTMLH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gfilter.h>
#include <rhtmlfile.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
/**
* The GFilterHTML class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
* @short HTML's Filter.
*/
class GFilterHTML: public GFilter
{
public:

	/**
	* Construct the HTML filter for a specific HTML document.
	* @param session         Session.
	* @param fac            Factory.
	*/
	GFilterHTML(GSession* session,GPlugInFactory* fac);

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
	*The destructor
	*/
	virtual ~GFilterHTML(void);
};


//-----------------------------------------------------------------------------
#endif
