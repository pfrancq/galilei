/*

	GALILEI Research Project

	GIRProm.h

	PROMETHEE Kernel for Grouping - Header.

	Copyright 2001-2002 by the Universit� Libre de Bruxelles.

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
#ifndef GXmlPromH
#define GXmlPromH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>
#include <rpromsol.h>
#include <rpromcriterion.h>

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gxmlir.h>
#include <xnode.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GPlugin class provides a representation for a basic "plugin" system for
* the GALILEI sessions.
* @author Pascal Francq
* @short Generic Plugin.
*/
class GXmlProm : public R::RPromKernel
{
	/**
	* Parameters.
	*/
	GXmlParams* Params;
	
	/**
	* Criteria representing the global score of the document containing the fragment  tf-idf
	*/
	R::RPromCriterion* CritDocSc;

	/**
	* Criteria representing the "Type" of a fragment.
	
	R::RPromCriterion* CritType;*/

	/**
	* Criteria representing the "distance" of a fragment.
	*/
	R::RPromCriterion* CritDis;
	/**
	* Criteria representing the frequence of a query keywords
	*
	R::RPromCriterion* CritOcc;*/

	/**
	* Criteria representing the specificity of the fragment
	*/
	R::RPromCriterion* CritSpecif;

	/**
	* Criteria representing the tfief
	**/
	R::RPromCriterion* CritTfief;

	/**
	* Solutions corresponding 
	*/
	R::RPromSol** Sols;

	/**
	* Session.
	*/
	GSession* Session;


public:

	/**
	* Constructor.
	* @param p              Parameters.
	*/
	GXmlProm(GXmlParams* p) throw(std::bad_alloc);


	/**
	* Assign chromosome as a solution.
	* @param s              Solution.
	* @param f              fragment.
	*/
	void Assign(R::RPromSol* s,XNode* f);
	void Assign(RPromSol* s, double criteria_values[4]);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
