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
#ifndef GPointOfViewH
#define GPointOfViewH

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/ggroup.h>
#include <sessions/gsession.h>
#include <sessions/gsessionmysql.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

class GCpovCursor;
class GOutPutCursor;


// -------------------------------------------------------------------------------
//  class dispatch : main class for the Point of view
//
//--------------------------------------------------------------------------------
class GDispatchpov : public GSession
{

public :


	/**
	*
	*/
	GDispatchpov( RStd::RContainer<GGroups,unsigned int,true,true>* groups  )throw(bad_alloc);

 	/**
	*
	*/
	GDispatchpov( RStd::RContainer<GGroup,unsigned int,true,true>* group ) throw(bad_alloc);

  	/**
	*
	*/
	GDispatchpov( GSubProfile* profile  )throw(bad_alloc);

 	/**
	*
	*/
	GDispatchpov( ) throw(bad_alloc);    

	/**
	*
	*/
	void GeneratePoV(RStd::RContainer<GGroups,unsigned int,true,true>* groups);

	/**
	* build the hierarchy of expert kwds for all the profiles af all the group
	*/
	void GeneratePoVfromScratch(GGroup* grp);

	/**
	* Update the hierarchy of expert kwds for all the profiles af all the groups
	*/
	void UpdatePovForAGrp( );

};


// -------------------------------------------------------------------------------
// class format the input datas for the choosen algorithm
//
//--------------------------------------------------------------------------------
class FormatInputPoV : public GSessionMySQL
{

public :

	/**
	* constructor
	*/
	FormatInputPoV(  )throw(bad_alloc);

 
private :  // fonction de recup des data en entree STANDARD
         

public :   // fonctions virtuelles de formatage des donnees LOCALE

	// set and check the input datas            
	virtual int FormatInBase() const = 0 ;
  
};


// -------------------------------------------------------------------------------
//  class Format the inputs
//
// -------------------------------------------------------------------------------
class InputBase : public FormatInputPoV
{

public :

	/**
	*
	*/
	virtual int FormatInBase();

};


// -------------------------------------------------------------------------------
//    class Description of the expert kwds and their parents
//
// -------------------------------------------------------------------------------
class StandardOutPut
{

public :

	/**
	* identification number
	*/
	 unsigned int Id;
	 
	/**
	* identification number of the group
	*/    
	int GroupId;
	
	/**
	* current profile
	*/
	GSubProfile *SProfile;
	
	/**
	* current group
	*/
	GGroup *Grp;
	
	/**
	* identification number of the Expert kwds
	*/
	unsigned int Idw;
	
	/**
	* parents of the Expert kwds
	*/
	unsigned int Parent[50][50];
	
	/**
	* position in the hierarchy
	*/
	int Level;
   
public :

	/**
	* Constructor
	*/
	StandardOutPut();
	
	/**
	* compare function
	*/      
	int Compare(const StandardOutPut* n) const;
	
	/**
	* compare function
	*/
	int Compare(const StandardOutPut& n) const;
	
	/**
	* compare function
	*/
	int Compare(const unsigned id) const;
	
	/**
	* get the identification number
	*/
	unsigned int GetId() const;
	
	/**
	* Set the value of the parents
	*/
	void SetParent(int position, int par, int freq);
	
	/**
	* Set the value of idw
	*/
	void SetIdw(unsigned int id);
	
	/**
	* Get the value of idw
	*/
	unsigned int GetIdw(void);
	
	/**
	* Set the profile
	*/
	void SetProfile(GSubProfile *Subp);
	
	/**
	* Set the group
	*/
	void SetGroup(GGroup *grp);
	
	/**
	* Set the level of the expert kwds
	*/
	void SetLevel(int level);
	
	/**
	* Get the level of the expert kwds
	*/
	int GetLevel(void);

	/**
	* destructor
	*/
	~StandardOutPut();
};


// -------------------------------------------------------------------------------
//   class Format the output datas of the choosen algorithm
//
// -------------------------------------------------------------------------------
class FormatOutputPoV : public GSessionMySQL
{

public :

	/**
	* constructor
	*/    
	FormatOutputPoV(  )throw(bad_alloc);

public : 

	/**
	* build the standard output
	*/
	virtual int FormatOutBase() const =0 ;     
};



// ---------------------------------------------------------------------------------------
// class format the outputs
//
// ---------------------------------------------------------------------------------------
class OutputBase
{

public :

	/**
	* constructor
	*/
	OutputBase();

	/**
	* build the standard outputs
	*/	
	int FormatOutBase();

	/**
	* RContainer of standard outputs
	*/
	RStd::RContainer<StandardOutPut,unsigned,false,true>* StdOutPut;

	/**
	* Cursor on the Rcontainer that contains the outputs of the ExpertKwds
	*/
	GOutPutCursor& GetGOutPutCursor(); 
};


// -------------------------------------------------------------------------------
//
//     class CommPoV
//
//--------------------------------------------------------------------------------
//class CommPoV : public GSessionMySQL
//{
//public :
//	// constructeur
//	CommPoV( ) throw(bad_alloc);
//
//	// objet de requette return 1 ok   I/O
//	int CommGetPoV(char *);
//	int CommPutPoV(char *);
//	//test si le calcul des POV a deja ete fait une fois
//	bool AlreadyExist();
//
//protected :
//
//};
 


// -------------------------------------------------------------------------------
// 
//       
//--------------------------------------------------------------------------------
//class GenerateTree
//{
//    // constructeur
//       GenerateTree(  )throw(bad_alloc);
//
//       ~GenerateTree() {} ;
//
//
//};


// -------------------------------------------------------------------------------
//
//     Description of the Expert Keywords (+ hierarchy)
//--------------------------------------------------------------------------------
class ExpertKwds
{

private :

	/**
	* identification number
	*/
	unsigned int Id;

	/**
	* parent of the Expert kwds  (max 5)
	*/
	int Parent[5];

public :

	/**
	* constructor
	*/
	ExpertKwds(void)throw(bad_alloc);

	/**
	* constructor  2
	*/
	ExpertKwds(unsigned id)throw(bad_alloc);

	/**
	* Rontainer of expert kwds
	*/
	//     RStd::RContainer<GCpov,unsigned,false,true>* GCpovs;

	/**
	* compare function
	*/
	int Compare(const ExpertKwds* ) const;

	/**
	* compare function
	*/
	int Compare(const ExpertKwds& n) const;

	/**
	* compare function
	*/
	int Compare(const unsigned id) const;

	/**
	* get the identification number
	*/
	unsigned int GetId() const;

	/**
	* Set the identification number
	*/
	void SetId(unsigned int);

	/**
	* destructor
	*/
	~ExpertKwds();
};


// -------------------------------------------------------------------------------
//    Description of the expert kwds abd tools to calculate them.
//    Intermediate stage
//--------------------------------------------------------------------------------
class GCpov
{

private :

	/**
	* Identification number
	*/
	unsigned int Id;

	/**
	* IDF factor for a kwds
	*/
	double IdfFactor;

	/**
	* id of the current kwds
	*/
	int Idw;

	/**
	* relative distance
	*/
	double Rdist;

	/**
	* maximum of 5 parents
	*/
	int Parent[5][2];

	/**
	* number of parents
	*/
	int NbParent;   

public :

	/**
	* constructor
	*/
	GCpov();

	/**
	* constructor  2
	*/
	GCpov(unsigned int);

	/**
	* compare function
	*/
	RStd::RContainer<ExpertKwds,unsigned,false,true>* IsInDocs;

	/**
	* compare funct  ion
	*/
	int Compare(const GCpov* ) const;

	/**
	* compare function
	*/
	int Compare(const GCpov& n) const;

	/**
	* compare function
	*/
	int Compare(const unsigned id) const;

	/**
	* return the Id 
	*/
	unsigned int GetId(void) const ;

	/**
	* set the Id
	*/
	void SetId(unsigned int);

	/**
	* increment id step 1
	*/
	void IncrId(void);

	/**
	* Insert the idf factor
	*/
	void InsertIdfFactor(double value);

	/**
	* Insert kwds id
	*/  
	void InsertIdw(int);

	/**
	* get the idf factor
	*/
	double Getidfac(void);

	/**
	* calculate the relative distance and insert it.
	*/
	void SetRdist(double rd);

	/**
	* get kwds id
	*/
	unsigned int GetIdw(void);

	/**
	* delete all parents
	*/
	void CleanParent(void);

	/**
	* Set the number of parents
	*/
	void SetNbParent(int nbparents);

	/**
	* Get the number of parents
	*/
	int GetNbParent(void);

	/**
	* add a parent in the array
	*/
	int AddParent(int value, int freq);

	/**
	* remplace a parent in the array
	*/
	void RemplaceParent(int position, int newparent, int freq);

	/**
	* return the parent number :position:
	*/
	int GetOneParent(int position);

	/**
	* return the parent frequency :position:
	*/
	int GetFreqParent(int position);

	/**
	* return all the parents 
	*/
	int GetAllParent(void);

	/**
	* add/remplace a parent
	*/
	void AddRemplaceParent(int idparent, int freq);

	/**
	* destructor
	*/
	~GCpov(){};

 };
 

// -------------------------------------------------------------------------------
//
//    main class. Manage the algorithm. Calculate the expert kwds
//--------------------------------------------------------------------------------
class GAlgobase : public GGroup
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
	GSubProfile *CurrentPrf;

	/**
	* .current profile
	*/ 
	GGroup* CurGrp;

	/**
	* .output standard format
	*/ 
	StandardOutPut *DataOut;

   
public :

	/**
	*constructor
	* .
	*/    
	GAlgobase() throw(bad_alloc);

	/**
	*Container kwds
	*/
	RStd::RContainer<ExpertKwds,unsigned int,false,true> *ChainPov;

	/**
	* container of POV
	* .
	*/
	RStd::RContainer<GCpov,unsigned int,false,true>* Pov;

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
	ExpertKwds *ListeDoc;

	/**
	* Cursor on subprofiles of a group
	*/
	GSubProfileCursor Sp;

	/**
	* cursor on wweight
	*/
	GIWordWeightCursor Ccur;

	/**
	* get the profile vector
	*/
	GProfileCalcVector *ProfileVect;

	/**
	* Manage the construction profile by profile in a group
	* @return void
	*/
	void BuildPoVGroup(GGroup *grp);

	/**
	* Build the hierarchy for extented point of view
	* @return void
	*/
	void AnalysePoVProfile(void);

	/**
	* Calculate the number of expertkwds dor the current set
	* @put nb max kwds into maxexpertkwds
	*/
	void EvaluateMaxExpertKwds(void);

	/**
	* calculte the hierarchy for each pov
	* @ Insert the kwds.
	*/
	void CalcHierarchy(void);

	/**
	* evaluate if current kwds is or is not an expert kwds
	* @ Insert the kwds.
	*/
	void InsertNewExpertKwds(void);

	/**
	* set the current profile
	* @ .
	*/
	void SetCurPrf(GSubProfile *cprf);

	/**
	* get the current profile
	* @ the profile
	*/
	GSubProfile *GetCurPrf(void);

	/**
	* get the current profile
	* @ the profile
	*/
	void SetCurGrp(GGroup *grp);

	/**
	* get the current profile
	* @ the profile
	*/
	GGroup *GetCurGrp(void);

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
	void CalcLevel(StandardOutPut *clink);

	/**
	* format de output data
	*/
	void FormatOutData(void);

	/**
	* RContainer of complete and standard descriptions of Experts kxds
	*/
	OutputBase *Out;

	/**
	* cursor on the RContainer
	*/
	GCpovCursor& GetGCpovCursor();

	/**
	* virtual destructor
	*/
	~GAlgobase(void){};
};


//-----------------------------------------------------------------------------
/**
* The GCpovcursor class provides a way to go trough a set of GCpov.
* @short Documents Cursor
*/
CLASSCURSOR(GCpovCursor,GCpov,unsigned int)


//-----------------------------------------------------------------------------
/**
* The GCpovcursor class provides a way to go trough a set of OutputBase.
* @short Documents Cursor
*/
CLASSCURSOR(GOutPutCursor,StandardOutPut,unsigned int)

}//-------- End of namespace GALILEI----------------------------------------


#endif



