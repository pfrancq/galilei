/*

	GALILEI Research Project

	GSatSimDoc.h

	Similarities between Documents - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GStatSimDocH
#define GStatSimDocH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GStatSimSubProf class provides statistics on the documents based on their
* similarities.
* @author Pascal Francq.
* @short Documents Similarities.
*/
class GStatSimDoc
{
protected:
	class LocalStat;

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* The Mean of the Mean intra group similarity (Local).
	*/
	double MeanIntraML;

	/**
	* The Mean of the Mean extra group similarity (Local).
	*/
	double MeanExtraML;

	/**
	* Ratio Rie (Local).
	*/
	double RieL;

	/**
	* Global Overlap Factor (Local).
	*/
	double GOverlapL;

	/**
	* Overlap Factor (Local).
	*/
	double OverlapL;

	/**
	* The Mean of the Mean intra group similarity (Global).
	*/
	double MeanIntraMG;

	/**
	* The Mean of the Mean extra group similarity (Global).
	*/
	double MeanExtraMG;

	/**
	* Ratio Rie (Global).
	*/
	double RieG;

	/**
	* Global Overlap Factor (Global).
	*/
	double GOverlapG;

	/**
	* Overlap Factor (Global).
	*/
	double OverlapG;

	/**
	* Global similaity must be calculated.
	*/
	bool Global;

	/**
	* Local similaity must be calculated.
	*/
	bool Local;

	/**
	* Statistics Output file.
	*/
	R::RTextFile* File;

	/**
	* Container of statistics on the subjects.
	*/
	R::RContainer<LocalStat,unsigned int,true,true> Sub;

public:

	/**
	* Constructor.
	* @param ses            The  galilei session.
	* @param f              File.
	* @param g              Global Similarity.
	* @param l              Local Similarity.
	*/
	GStatSimDoc(GSession* ses,R::RTextFile* f,bool g,bool l);

	/**
	* Get the average intra-group similarity (Local).
	* @returns MeanIntraM.
	*/
	double GetAvgIntraL(void) const {return(MeanIntraML);}

	/**
	* Get the average inter-group similarity (Local).
	* @returns MeanExtraM.
	*/
	double GetAvgInterL(void) const {return(MeanExtraML);}

	/**
	* Get the avevarge overlap factor (Local).
	* @returns AVGol.
	*/
	double GetAVGolL(void) const {return(OverlapL);}

	/**
	* Get the avevarge global overlap factor (Local).
	* @returns AVGol.
	*/
	double GetAVGGolL(void) const {return(GOverlapL);}

	/**
	* Get the Rie factor (Local).
	* @returns Rie factor.
	*/
	double GetRieL(void) const {return(RieL);}

	/**
	* Get the average intra-group similarity (Global).
	* @returns MeanIntraM.
	*/
	double GetAvgIntraG(void) const {return(MeanIntraMG);}

	/**
	* Get the average inter-group similarity (Global).
	* @returns MeanExtraM.
	*/
	double GetAvgInterG(void) const {return(MeanExtraMG);}

	/**
	* Get the avevarge global overlap factor (Global).
	* @returns AVGol.
	*/
	double GetAVGGolG(void) const {return(GOverlapG);}

	/**
	* Get the avevarge overlap factor (Global).
	* @returns AVGol.
	*/
	double GetAVGolG(void) const {return(OverlapG);}

	/**
	* Get the Rie factor (Global).
	* @returns Rie factor.
	*/
	double GetRieG(void) const {return(RieG);}

	/**
	* Construct the Docs Similarity.
	*/
	void Run(void);

	/**
	* Destructor for the main view.
	*/
	~GStatSimDoc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
