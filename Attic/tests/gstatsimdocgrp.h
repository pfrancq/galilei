/*

	GALILEI Research Project

	GSatSimDocGrp.h

	Calc the similarity between subprofiles using the ideal groupment - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

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
#ifndef GStatSimDocGrpH
#define GStatSimDocGrpH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The SatSimDocGrp class provides a way to see similaritys between profiles.
* @author Lamoral Julien & Pascal Francq.
* @short Subprofiles Similarities Statistics.
*/
class GStatSimDocGrp
{
protected:

	class SimGroup;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* The Mean of the Mean intra min group similarity.
	*/
	double MeanIntraMin;

	/**
	* The Mean of the Mean extra max group similarity.
	*/
	double MeanExtraMax;

	/**
	* The Mean of the Mean intra group similarity.
	*/
	double MeanIntraM;

	/**
	* The Mean of the Mean extra group similarity.
	*/
	double MeanExtraM;

	/**
	* Ratio Rie.
	*/
	double Rie;

	/**
	* Cost function.
	*/
	double J;

	/**
	* Overlap Factor.
	*/
	double Overlap;

	/**
	* Overlap Group Factor.
	*/
	double GrpOverlap;

	/**
	* The Container of ideal group.
	*/
	RContainer<GGroups,unsigned int,true,true>* IdealGroups;

	/**
	* Global similaity used if true.
	*/
	bool Global;

	/**
	* Relevant profile of groups or Gravitation methodes.
	*/
	bool relevant;


public:

	/**
	* Constructor.
	* @param ses            The  galilei session.
	* @param groups         Ideal groupement.
	*/
	GStatSimDocGrp(GSession* ses,RContainer<GGroups,unsigned int,true,true>* ideal);

	/**
	* Static function used to ordered by similarity.
	*/
	static int sortOrder(const void *a,const void *b);

	/**
	* Get the average intra-group similarity.
	* @returns MeanIntraM.
	*/
	double GetAvgIntra(void) const {return(MeanIntraM);}

	/**
	* Get the average inter-group similarity.
	* @returns MeanExtraM.
	*/
	double GetAvgInter(void) const {return(MeanExtraM);}

	/**
	* Get the avevarge overlap factor.
	* @returns AVGol.
	*/
	double GetAVGol(void) const {return(Overlap);}

	/**
	* Get the avevarge  group overlap factor.
	* @returns AVGol.
	*/
	double GetAVGGrpol(void) const {return(GrpOverlap);}

	/**
	* Get the Rie factor.
	* @returns Rie factor.
	*/
	double GetRie(void) const {return(Rie);}

	/**
	* Get the Rie factor.
	* @returns Rie factor.
	*/
	double GetJ(void) const {return(J);}

	/**
	* Launch the statistique program.
	*/
	void Run(void);

	/**
	* Set the settings for the method using a string.
	* "Global similarity if 1 normal else"
	* @param s              C string coding the settings.
	*/
	void SetSettings(const char* s);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	char* GetSettings(void);

	/**
	* Destructor for the main view.
	*/
	~GStatSimDocGrp(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
