/*

	GALILEI Research Project

	PointOfView.h

	Document representing a GALILEI session - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
    Stephane Rideau (srideau@ulb.ac.be).

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
#ifndef GAlgobaseH
#define GAlgobaseH

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/ggroup.h>
#include <postgroups/ginoutputbase.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


// -------------------------------------------------------------------------------
//
//    main class. Manage the algorithm. Calculate the expert kwds
//--------------------------------------------------------------------------------
class GAlgobase : public GGroup, public GInOutputBase
{

private :
       
	/**
	*identification number
	*/   
	unsigned int Id;

	/**
	*identification number provisoire
	*/  
	int Ident;
	
	/**
	*
	*/  
	double Werd;

	/**
	* .current profile 
	*/ 
	GSubProfile * CurrentPrf;

	/**                                       
	* .current profile
	*/ 
	GGroup* CurGrp;

	/**
	* .output standard format
	*/ 
	GStandardInOutPut *DataOut;

   
public :

	/**
	*constructor
	* .
	*/    
	GAlgobase() throw(bad_alloc);

	/**
	*Container kwds
	*/
	R::RContainer<GExpertKwds,unsigned int,false,true> *ChainPov;

	/**
	* container of POV
	* .
	*/
	R::RContainer<GCpov,unsigned int,false,true>* Pov;

	/**
	* langage used
	*/
	GLang *Lng;

	/**
	* nb max of expertkwds
	*/
	unsigned int MaxExpertKwds;

	/**
	* data for current expert kwds
	*/
	GCpov *Data;

	/**
	* compare function
	*/
	GExpertKwds *ListeDoc;

	/**
	* Cursor on subprofiles of a group
	*/
	GSubProfileCursor Sp;

	/**
	* cursor on wweight
	*/
	GIWordWeightCursor Ccur;

	/**
	* Manage the construction profile by profile in a group
	* @return void
	*/
	void BuildPoVGroup(GGroup *grp, GSession *s);

	/**
	* Build the hierarchy for extented point of view
	* @return void
	*/
	void AnalysePoVProfile(GSession *s);

	/**
	* Calculate the number of expertkwds dor the current set
	* @put nb max kwds into maxexpertkwds
	*/
	void EvaluateMaxExpertKwds(void);

	/**
	* calculte the hierarchy for each pov
	* @ Insert the kwds.
	*/
	void CalcHierarchy(GSession *s);

	/**
	* evaluate if current kwds is or is not an expert kwds
	* @ Insert the kwds.
	*/
	void InsertNewExpertKwds(void);

	/**
	* set the current profile
	* @ .
	*/
	void SetCurPrf(GSubProfile * cprf);

	/**
	* get the current profile
	* @ the profile
	*/
	GSubProfile * GetCurPrf(void);

	/**
	* get the current profile
	* @ the profile
	*/
	void SetCurGrp(GGroup * grp);

	/**
	* get the current profile
	* @ the profile
	*/
	GGroup * GetCurGrp(void);

	/**
	* return the best parent
	*/
	int GetBestParent(GCpov *current);

	/**
	* virtual destructor
	*/
	unsigned int GetId(void);

	/**
	* virtual destructor
	*/
	int CalcLevel(GStandardInOutPut *clink);

	/**
	* format de output data
	*/
	void FormatOutData(GSession *);

	/**
	* list of documents for each kwds
	*/
	void SaveDocsList(R::RContainer<GExpertKwds,unsigned,false,true>*);

	/**
	* RContainer of complete and standard descriptions of Experts kxds
	*/
	GInOutputBase *Out;

	/**
	* cursor on the RContainer
	*/
	GCpovCursor& GetGCpovCursor();

	/**
	* virtual destructor
	*/
	~GAlgobase(void){};
};


}   //-------- End of namespace GALILEI----------------------------------------


#endif



