/*

	GALILEI Research Project

	GDocAnalyse.h

	Generic Document Analysis - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef GDocAnalyseH
#define GDocAnalyseH


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
 * The GDocAnalyse class provides a generic method to analyze a document. The
 * internal structures, Infos and Struct, are used by GSession to manage the
 * results (saving, assign them to the documents, etc.). The plug-in is
 * responsible for their management (such as clearing them between two analysis
 * if necessary.
 * @author Pascal Francq
 * @short Generic Document Analysis.
 */
class GDocAnalyse : public GPlugin
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
	GDocAnalyse(GPluginFactory* fac);

	/**
	* Analyze a XML of a document for a session.
	* @param doc             Document to analyze.
	* @param file            File to analyze (may be its XML version).
	* @param native          Specify if the document is a native XML file.
	*/
	virtual void Analyze(const GDoc* doc,const R::RURI& file,bool native)=0;

	/**
	* Destruct the document analyzer.
	*/
	virtual ~GDocAnalyse(void);

	friend class GSession;
};


//-------------------------------------------------------------------------------
#define CREATE_DOCANALYSE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GDocAnalyse,plugin,"DocAnalyse",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
