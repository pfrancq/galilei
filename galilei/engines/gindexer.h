/*

	GALILEI Research Project

	GIndexer.h

	Dictionary - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef GIndexer_H
#define GIndexer_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rdblhashcontainer.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GIndexer class provides a representation for a dictionary of data, each data
* having is own identificator and name. These data may be stopwords.
* @author Pascal Francq
* @short Dictionary.
*/
class GIndexer : protected R::RDblHashContainer<GWordOccurs,27,27,true>
{
	/**
	* Pointer to the managers holding all the languages handled by the system.
	*/
	GLangManager* Langs;

public:

	/**
	* Constructor of the dictionary.
	* @param ml             Maximal number of data to create at initialisation
	*                       for the different letters.
	* @param langs          Pointer to the languages manager.
	*/
	GIndexer(unsigned ml,GLangManager* langs);

	/**
	* Clear the dictionary.
	*/
	void Clear(void);

	/**
	* Insert a word in the indexer.
	* @param word            Word to insert.
	*/
	void InsertWord(R::RString word);

	/**
	* Delete a given word.
	* @param word            Word to insert.
	*/
	void DeleteData(R::RString word);

	/**
	* Look if a given word is indexed.
	* @param word            Name fo the data to look for.
	* @return true if the data is in the dictionary.
	*/
	bool IsIndexed(const R::RString& word) const;

	/**
	* Get a given data from the dictionary.
	* @param name            Name fo the data to look for.
	* @return Pointer to the data.
	*/
	GWordOccurs* GetWord(const R::RString& word) const;

	/**
	* Run a query given as string.
	* @param query           String containing the query.
	* @param docs            Identificators of the documents responding to the
	*                        query.
	*/
	void RunQuery(R::RString query,R::RContainer<GInfo,true,false>& docs) const;

	/**
	* Destructor of the dictionary.
	*/
	virtual ~GIndexer(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
