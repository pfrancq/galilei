/*

	GALILEI Research Project

	GFilterHTML.h

	A HTML filter - Header.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

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


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


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
	* @param fac            Factory.
	*/
	GFilterHTML(GFactoryFilter* fac);

	/**
	* Analyze a document with a given URI that was downloaded in a local
	* temporary file and for which a DocXML must be created.
	* @param uri             URI of the file to analyze.
	* @param file            Local file to analyze.
	* @param docxml          Local file that will containing the DocXML.
	*/
	virtual void Analyze(const RURI& uri,const RString& file,const RString& docxml);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	*The destructor
	*/
	virtual ~GFilterHTML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
