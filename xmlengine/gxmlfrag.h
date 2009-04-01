/*

	GALILEI Research Project

	GXMLFrag.h

	Chromosome for an IR Problem - Header

	Copyright 2004-2009 by the Universit�Libre de Bruxelles.

	Authors:
		Faïza Abbaci (fabbaci@ulb.ac.be).

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
#ifndef GXMLFragH
#define GXMLFragH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromkernel.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gxmlprom.h>
#include <gxmlir.h>

//-----------------------------------------------------------------------------
namespace GALILEI {
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GFrag class provides a representation for a result : fragment of an xml document .
* @author Fa�za Abbaci
* @short Xml fragment.
*/
class GXmlFrag : public R::RContainer<R::RPromCritValue, true, true>
{
public:
	/**
	* Identier of the XmlFrag.
	*/
	unsigned int ID;


	/**
	* Value of the "Document Score" criterion.tf-idf
	*/
	double CritDocSc;

	/**
	* Value of the "Type" criterion.
	*/
	double CritType;

	/**
	* Value of the "Type and Distance" criterion.
	*/
	double CritDis;

	/**
	* Value of the criterion "Occurrences".
	*
	double CritOcc;*/

	/**
	* Value of the criterion "Specificity".
	*/
	double CritSpecif;

	/**
	* Value of the criterion "TF_IEF".
	*/
	double CritTfief;


	/**
	* Global Factor
	*/
	double Global;




	/**
	* Construct the chromosome.
	* @param inst           The instance of the problem.
	* @param id             The identificator of the chromosome.
	*/
	GXmlFrag(unsigned int id);


	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const GXmlFrag* c) const;


	/**
	* Get the global value for the chromosome.
	*/
	double GetGlobal(void) const {return(Global);}




	/**
	* Get the value of the "Document Score" criterion for the fragment.
	* @returns double.
	*/
	double GetCritDocSc(void) const {return(CritDocSc);}

	/**
	* Get the value of the "Type" criterion for the fragment.
	* @returns double.
	*/
	double GetCritType(void) const {return(CritType);}

	/**
	* Get the value of the "Distance" criterion for the fragment.
	* @returns double.
	*/
	double GetCritDis(void) const {return(CritDis);}

	/**
	* Get the value of the "Occurrences" criterion for the fragment.
	* @returns double.
	*
	double GetCritOcc(void) const {return(CritOcc);}*/

	/**
	* Get the value of the "Specificity" criterion for the fragment.
	* @returns double.
	*/
	double GetCritSpecif(void) const {return(CritSpecif);}

	/**
	* Get the value of the "TFIEF" criterion for the fragment.
	* @returns double.
	*/
	double GetCritTfief(void) const {return(CritTfief);}

	/**
	* Destructor.
	*/
	virtual ~GXmlFrag(void);

	// friend classes
	friend class GXmlProm;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
