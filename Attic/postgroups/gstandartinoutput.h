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
#ifndef GStandartInOutputH
#define GStandartInOutputH

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
//    class Description of the expert kwds and their parents
//
// -------------------------------------------------------------------------------
class GStandardInOutPut
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
	GSubProfile * SProfile;

	/**
	* current group
	*/
	GGroup * Grp;

	/**
	* identification number of the Expert kwds
	*/
	unsigned int Idw;

	/**
	* expert kwds
	*/
	char Word[129];

	/**
	* parents of the Expert kwds
	*/
	unsigned int Parent[50][50];

	/**
	* position in the hierarchy
	*/
	int Level;

	/**
	* frequency in xxx/yyy/zzz
	*/
	char Freq[129];

	/**
	* Parents in xxx/yyy/zzz
	*/
	char Parents[129];

	/**
	* reference of a document
	*/
	char DocRef[256];	

public :

	/**
	* Constructor
	*/
	GStandardInOutPut();

	/**
	* Constructor  2 for copy
	*/
	GStandardInOutPut (GStandardInOutPut*);


	/**
	* compare function
	*/
	int Compare(const GStandardInOutPut* n) const;

	/**
	* compare function
	*/
	int Compare(const GStandardInOutPut& n) const;

	/**
	* compare function
	*/
	int Compare(const unsigned id) const;

	/**
	* get the identification number
	*/
	unsigned int GetId() const;

	/**
	* set the identification number
	*/
	void SetId(unsigned int id);

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
	void SetProfile(GSubProfile * Subp);

	/**
	* Gt the profile
	*/
	GSubProfile * GetCurProfile(void);

	/**
	* Set the group
	*/
	void SetGroup(GGroup *grp);

	/**
	* get the group id
	*/
	GGroup *GetGroup(void);

	/**
	* Set the level of the expert kwds
	*/
	void SetLevel(int level);

	/**
	* Get the level of the expert kwds
	*/
	int GetLevel(void);

	/**
	* Get the freq in xxx/yyy/zzz
	*/
	char *GetFreq(void);
	
	/**
	* add a freq in the xxx/yyy/zzz format
	*/
	void AddFreq(int freq);

	/**
	* freq = '\0'
	*/
	void CleanFreq(void);

	/**
	* Get parents in xxx/yyy/zzz
	*/
	char *GetParent(void);
	
	/**
	* Get parents in at the specified position
	*/
	int GetParentPos(int position);
	
	/**
	* add a parent in the xxx/yyy/zzz format
	*/
	void AddParent(int par);

	/**
	* copy an expert kwds into Word
	*/
	void SetWord(char*str);

	/**
	* return an expert kwds into Word
	*/
	char * GetWord(void);

	/**
	* save the reference for a doc
	*/
	void SetDocRef(char *docref);

	/**
	* get the reference for a doc
	*/
	char * GetDocRef(void);

	/**
	* clean parents ( = '\0')
	*/
	void CleanParent(void);
	
	/**
	* destructor
	*/
	~GStandardInOutPut();
};



//-----------------------------------------------------------------------------
/**
* The GCpovcursor class provides a way to go trough a set of OutputBase.
* @short Documents Cursor
*/
CLASSCURSOR(GStandardInOutPutCursor,GStandardInOutPut,unsigned int)

}//-------- End of namespace GALILEI----------------------------------------


#endif



