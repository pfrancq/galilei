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
#ifndef GCpovH
#define GCpovH

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
	* maximum of 50 parents
	*/
	int Parent[50][2];

	/**
	* number of parents
	*/
	int NbParent;

	/**
	* number of parents
	*/
	char TheWord[128];	

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
	R::RContainer<GExpertKwds,unsigned,false,true>* IsInDocs;

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
	* set the kwd
	*/
	void SetWord(char *w);

	/**
	* get the kwd
	*/
	char * GetWord(void);
	
	/**
	* destructor
	*/
	~GCpov(){};

 };
 


//-----------------------------------------------------------------------------
/**
* The GCpovcursor class provides a way to go trough a set of GCpov.
* @short Documents Cursor
*/
CLASSCURSOR(GCpovCursor,GCpov,unsigned int)


}//-------- End of namespace GALILEI----------------------------------------


#endif



