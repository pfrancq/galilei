/*

	GALILEI Research Project

	GIR.h

	GA for the Information Retrieval Problem - Header.

	Copyright 2004-2009 by the Universit�Libre de Bruxelles.

	Authors:
		Faïza Abbaci (fabbaci@ulb.ac.be)

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
#ifndef GIRH
#define GIRH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <rpromcriterion.h>
#include <rfitness.h>
#include <rgga.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <rparam.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------
class GXmlFrag;
//-----------------------------------------------------------------------------
// Forward class declaration

class GXmlParams;


//-----------------------------------------------------------------------------
/**
* The GIRParam represents xmlranking params for promethee.
* @short GIR Parameters.
*/
class GXmlParams
{
public:

	/**
		* Parameter for the criterion "tfidf".
		*/
		R::RParam* ParamsDocSc;

		/**
		* Parameter for the criterion "Type".

		R::RParam*  ParamsType;*/

		/**
		* Parameter for the criterion "Dis".
		*/
		R::RParam*  ParamsDis;

		/**
		* Parameter for the criterion ""Occ".
		*
		R::RParam*  ParamsOcc;*/

		/**
		* Parameter for the criterion "Specif".
		*/
		R::RParam*  ParamsSpecif;

		/**
		* Parameter for the criterion "Tfief".
		**/
		R::RParam* ParamsTfief;


		/**
		* Constructor.
		*/
		GXmlParams(void);


};


}  //------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
