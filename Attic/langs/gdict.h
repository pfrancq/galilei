/*

	GALILEI Research Project

	GDic.h

	Dictionnary - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#ifndef GDictH
#define GDictH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rdblhashcontainer.h>
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GDict class provides a representation for a dictionnary.
* @author Pascal Francq
* @short Dictionnary.
*/
class GDict : public R::RDblHashContainer<GWord,unsigned,27,27,true>
{
	/**
	* Session responsible for the dictionnary.
	*/
	GSession* Session;

	/**
	* Direct access to the words using an identificator.
	*/
	GWord** Direct;

	unsigned NbGroupsList;

	/**
	* Maximal value of the identificator handled by Direct.
	*/
	unsigned MaxId;

	/**
	* Highest value of the identificator.
	*/
	unsigned UsedId;

	/**
	* Pointer to the language corresponding to the dictionnary.
	*/
	GLang *Lang;

	/**
	* Name of the dictionnary. This can be the name of a file or a table in a
	* database.
	*/
	R::RString Name;

	/**
	* Description of the dictionnary.
	*/
	R::RString Desc;

	/**
	* State of the dictionnary.
	*/
	bool Loaded;

	/**
	* Is this dictionnary a stop list or not.
	*/
	bool Stop;

	/**
	* Number of references in documents.
	*/
	unsigned int NbRefDocs[2];

	/**
	* Number of references in subprofiles.
	*/
	unsigned int NbRefSubProfiles[2];

	/**
	* Number of references in groups.
	*/
	unsigned int NbRefGroups[2];

public:

	R::RContainer<GWordList,unsigned,true,false> GroupsList;

	/**
	* Constructor of the dictionnary.
	* @param s              Session.
	* @param name           Name of the dictionnary.
	* @param desc           Description of the dictionnary.
	* @param lang           Corresponding language.
	* @param m              Total maximal number of words to create at
	*                       initialisation.
	* @param ml             Maximal number of words to create at initialisation
	*                       for the different letters.
	* @param st             Is it a Stoplist?.
	*/
	GDict(GSession* s,const R::RString& name,const R::RString& desc,GLang* lang,unsigned m,unsigned ml,bool st) throw(bad_alloc);

	/**
	* Clear the dictionnary.
	*/
	void Clear(void);

private:

	/**
	* Put a word in direct.
	* @param word           Pointer to the word to insert.
	*/
	void PutDirect(GWord* word) throw(bad_alloc);

public:

	/**
	* Put a string corresponding to a word and an identificator in the
	* dictionnary.
	*/
	void Put(unsigned id,const R::RString& word) throw(bad_alloc);

	/**
	* Insert a list of words in the dictionnary.
	*/
	void InsertNewWordList(GWordList& wordlist,bool save);

	/**
	* Get the identificator of the word corresponding to a string. If the word
	* doesn't exist in the dictionnary, it is created.
	* @return Identificator of the word.
	*/
	unsigned int GetId(const R::RString& word) throw(bad_alloc);

	/**
	* Get the maximal number of word actually supposed.
	*/
	unsigned int GetMaxId(void) const {return(MaxId);}

	/**
	* Get the list of the words currently defined in the dictionnary.
	* @returns const Pointer to array of GWord*.
	*/
	GWord** GetWords(void) const {return(Direct);}

	/**
	* Get thye name of the dictionnary.
	* @returns Pointer a C string.
	*/
	const char* GetName(void) const {return(Name());}

	/**
	* Get the language of the subprofile.
	* @return Pointer to the language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* @return Word given by an identificator.
	*/
	const char* GetWord(const unsigned int id) const;

	/**
	* Get the element with a specific identificator.
	* @param id             Identificator.
	* @return Pointer to a GWord.
	*/
	GWord* GetElement(const unsigned int id) const;

	/**
	* Get the number of group of information entities.
	* @returns Number of groups.
	*/
	unsigned int GetNbGroupsList(void) const;

	/**
	* Test if the dictionnary is a stop list.
	* @returns true if it is a stop list.
	*/
	bool IsStopList(void) const;

	/**
	* Compare function used in the R::RContainer.
	*/
	int Compare(const GDict* dict) const;

	/**
	* Compare function used in the RStd::RContainer.
	*/
	int Compare(const GDict& dict) const;

	/**
	* Compare function used in the RStd::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Increase the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	*/
	void IncRef(unsigned int id,tObjType ObjType,GWordType WordType);

	/**
	* Decrease the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	*/
	void DecRef(unsigned int id,tObjType ObjType,GWordType WordType);

	/**
	* Get the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(unsigned int id,tObjType ObjType);

	/**
	* Increase the number of objects using the dictionnary.
	* @param ObjType        Type of the reference.
	*/
	void IncRef(tObjType ObjType,GWordType WordType);

	/**
	* Decrease the number of objects using the dictionnary.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	*/
	void DecRef(tObjType ObjType,GWordType WordType);

	/**
	* Get the total number of references.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(tObjType ObjType,GWordType WordType);

	/**
	* Destructor of the dictionnary.
	*/
	virtual ~GDict(void);
};



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
