/*

	GALILEI Research Project

	GSatSimId.h

	Save the different value important to calc stats on documents or profiles  - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GSatSimIdH
#define GSatSimIdH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GSatSimDoc class provides a way to see similaritys statistic for one Profile or Document.
* @author Lamoral Julien
* @short Similarities Statistics for one profdoc.
*/
class GSatSimId
{

protected:

	/**
	* Identifier of the document.
	*/
	unsigned int Id;

	/**
	* The minimum of intra group similarities.
	*/
	double MinIntra;

	/**
	* The Maximum of inter group similarities.
	*/
	double MaxInter;

	/**
	* The mean of intra group similarities.
	*/
	double MeanIntra;

	/**
	* The mean of inter group similarities.
	*/
	double MeanInter;

	/**
	* The id of group.
	*/
	unsigned int GrpId;

public:

	/**
	* Compteur used for MeanIntra.
	*/
	unsigned int ComptIntra;

	/**
	* Compteur used for MeanInter.
	*/
	unsigned int ComptInter;

	/**
	* Compteur used for MeanIntra.
	*/
	unsigned int ComptIntraFoire;

	/**
	* Compteur used for MeanInter.
	*/
	unsigned int ComptInterFoire;


	/**
	* Constructor.
	* @param id            The  id of the profdoc.
	* @param grpid            The  id of the profdoc.
	*/
	GSatSimId(unsigned int id,unsigned int grpid);

	/**
	* Compare function needed by R::RContainer.
	* @param statsimid      GSatSimId used for the comparaison.
	*/
	int Compare(const GSatSimId& statsimid) const;

	/**
	* Compare function needed by R::RContainer.
	* @param statsimid      Pointer to the GSatSimId used for the comparaison.
	*/
	int Compare(const GSatSimId* statsimid) const;

	/**
	* Compare function needed by R::RContainer.
	* @param id            Identificator used for the comparaison.
	*/
	int Compare(const unsigned id) const;

	/**
	* @return Identificator of the profiles or document.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* @return the Intra group Min Similaritis.
	*/
	double GetMinIntra(void) const {return(MinIntra);}

	/**
	* @return the Inter group Max Similaritis.
	*/
	double GetMaxInter(void) const {return(MaxInter);}
	
	/**
	* @return the Intra group Mean Similaritis.
	*/
	double GetMeanIntra(void) const {return(MeanIntra/ComptIntra);}

	/**
	* @return the Inter group Mean Similaritis.
	*/
	double GetMeanInter(void) const {return(MeanInter/ComptInter);}

	/**
	* @return Identificator of group.
	*/
	unsigned int GetGrpId(void) const {return(GrpId);}

	/**
	* Set the different values with a new sim.
	* @param Sim            The New sim to compare
	*/
	void SetSim(double Sim,unsigned int Grp);

	/**
	* Destructor for the main view.
	*/
	~GSatSimId(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
