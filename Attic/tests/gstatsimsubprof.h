/*

	GALILEI Research Project

	SatSimSubProf.h

	Calc the similarity between subprofiles using the ideal groupment - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
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
#ifndef GSatSimSubProfH
#define GSatSimSubProfH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The SatSimSubProf class provides a way to see similaritys between profiles.
* @author Lamoral Julien & Pascal Francq.
* @short Similaritys between profiles.
*/
class GSatSimSubProf
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* The Mean of the Min intra group similarity.
	*/
	double MinIntraM;

	/**
	* The Mean of the Mean intra group similarity.
	*/
	double MeanIntraM;

	/**
	* The Mean of the Max extra group similarity.
	*/
	double MaxExtraM;

	/**
	* The Mean of the Mean extra group similarity.
	*/
	double MeanExtraM;

	/**
	* Ratio Rie.
	*/
	double Rie;

	/**
	* Overlap Factor.
	*/
	double Overlap;

	/**
	* The Container of ideal group.
	*/
	RContainer<GGroups,unsigned int,true,true>* IdealGroups;

public:

	/**
	* Constructor.
	* @param ses            The  galilei session.
	* @param groups         Ideal groupement.
	*/
	GSatSimSubProf(GSession* ses,RContainer<GGroups,unsigned int,true,true>* ideal);

	/**
	* Get the Mean of Min intra group similarity.
	* @returns MinIntraM.
	*/
	double GetMinIntra(void) const {return(MinIntraM);}

	/**
	* Get the Mean of Mean intra group similarity.
	* @returns MeanIntraM.
	*/
	double GetMeanIntra(void) const {return(MeanIntraM);}

	/**
	* Get the Mean of Max extra group similarity.
	* @returns MaxExtraM.
	*/
	double GetMaxInter(void) const {return(MaxExtraM);}

	/**
	* Get the Mean of Mean extra group similarity.
	* @returns MeanExtraM.
	*/
	double GetMeanInter(void) const {return(MeanExtraM);}

	/**
	* Get the Rie factor.
	* @returns Rie factor.
	*/
	double GetRie(void) const {return(Rie);}

	/**
	* Construct the subprofile similarity widget.
	* @param Global         Use Global similarity ?
	*/
	void Run(bool Global);

	/**
	* Destructor for the main view.
	*/
	~GSatSimSubProf(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
