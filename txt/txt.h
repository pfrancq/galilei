/*

	GALILEI Research Project

	TXT.h

	Filter for text Files - Header.

	Copyright 2001 by Valery Vandaele.
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
#ifndef GFilterTXTH
#define GFilterTXTH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gfilter.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
/**
* The GFilterTXT class provides a representation of a document to filter in a
* XML structure.
* @author Vandaele Valery
* @short TXT Filter.
*/
class GFilterTXT : public GFilter
{

public:

	/**
	* Construct the TXT filter.
	* @param session         Session.
	* @param fac            Factory.
	*/
	GFilterTXT(GSession* session,GPlugInFactory* fac);

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
	* Create the parameters.
	* @param fac             Factory.
	*/
	static void CreateParams(GPlugInFactory* fac);

	/**
	* Destructor of the TXT filter.
	*/
	virtual ~GFilterTXT(void);
};


//-----------------------------------------------------------------------------
#endif
