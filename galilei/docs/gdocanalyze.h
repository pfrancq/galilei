/*

	GALILEI Research Project

	GDocAnalyze.h

	Generic Document Analysis - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
#ifndef GDocAnalyzeH
#define GDocAnalyzeH


//------------------------------------------------------------------------------
// include files for R
#include <rxmlparser.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <gweightinfos.h>
#include <gdocstruct.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GDocAnalyze class provides a generic method to analyze a document. The
 * internal structures, Infos and Struct, are used by GSession to manage the
 * results (saving, assign them to the documents, etc.). The plug-in is
 * responsible for their management (such as clearing them between two analysis
 * if necessary.
 * @author Pascal Francq
 * @short Generic Document Analysis.
 */
class GDocAnalyze : public GPlugIn, public R::RXMLParser
{
protected:

	/**
	 * Vector that should contain the structure after the analyze.
	 */
	GWeightInfos Infos;

	/**
	 * Structure that should contain the structure after the analyze.
	 */
	GDocStruct Struct;

	/**
	 * Language associated to the document.
	 */
	GLang* Lang;

public:

	/**
	* Constructor of the document analysis method.
	* @param fac             Factory of the plug-in.
	*/
	GDocAnalyze(GPlugInFactory* fac);

	/**
	 * Prepare the analyze of a document. The method is called by GSession
	 * before the XML document is read.
	 * @param doc             Document to analyze.
	 * @param native          Specify if the document is a native XML file.
	 */
	virtual void PrepareAnalyze(const GDoc* doc,bool native)=0;

	/**
	* Terminate the analysis of a XML document. The method is called by
	* GSession after the XML document is read.
	*/
	virtual void TerminateAnalyze(void)=0;

	/**
	* Destruct the document analyzer.
	*/
	virtual ~GDocAnalyze(void);

	friend class GSession;
};


//-------------------------------------------------------------------------------
#define CREATE_DOCANALYZE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GDocAnalyze,plugin,"DocAnalyze",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
