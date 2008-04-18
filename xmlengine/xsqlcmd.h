/***************************************************************************
 *   Copyright (C) 2005 by Jean-Baptiste Valsamis                          *
 *   jvalsami@ulb.ac.be                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef __XSQLCMDH__			// Avoids multiple declarations
#define __XSQLCMDH__

#include <gstorage.h>
#include <ggalileiapp.h>
#include "xnodesql.h"
using namespace GALILEI;


/**
* Initialize all the commands
*/
class XSQLCmd
{
public:

	static void Init();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Clear all the tables used by the XML search engine
*/
class XSQLClearXMLTables : public GStorageCmd
{
public:

	XSQLClearXMLTables();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLClearXMLTables();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Create tables filemanager, nodelist and wordlist if they do not exist. These tables, used by the XML
* search engenis, index the structure of XML files
*/
class XSQLCreateXMLTables : public GStorageCmd
{
public:

	XSQLCreateXMLTables();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLCreateXMLTables();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Get the id of the xml file if it matches with url and optionally utc and size
* @param url            Location of the file
* @param utc            Time of last writing access (number of seconds since 1st Jan 1970)
* @param size           Size of the file
* @return               The id of the file if synchronized, otherwise -1.
*/
class XSQLGetIdFile : public GStorageCmd
{
public:

	XSQLGetIdFile();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetIdFile();
};
/**
* Get the size of a document from filemanager table
* @param idfile          the id of the document
* @return               The size of this file
*/
class XSQLGetSizeFile : public GStorageCmd
{
public:

	XSQLGetSizeFile();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetSizeFile();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Gets the string representing nodes of xml trees in table WordList for the keyword word.
* @param word           Keyword from which the string is retrieved
* @return               The string containing nodes of xml trees
*/
class XSQLGetLocalisations : public GStorageCmd
{
public:

	XSQLGetLocalisations();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetLocalisations();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Gets nb occurences of all words in a file
* @param file
* @return               The string containing nodes of xml trees
*/
class  XSQLGetTotalWordInFile : public GStorageCmd
{
public:

	XSQLGetTotalWordInFile();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetTotalWordInFile();
};
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* For
* @param localisation_id  
* @return             a vector (idfile, idnode, idfile, idnode,...) associated with a localisation_id in 'localisations' table (toutes les occurences d'un term dont id=localisations_id dans toute la collection)
*/
class XSQLGetLocalisationsList : public GStorageCmd
{
public:

	XSQLGetLocalisationsList();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetLocalisationsList();
};
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* For
* @param localisation_id  
* @return             a vector (idfile, idnode, idfile, idnode,...) associated with a localisation_id in 'localisations' table (nbre d'occurence d'un terme dans toutes la collection)
*/
class XSQLGetLocalisationsListSize : public GStorageCmd
{
public:

	XSQLGetLocalisationsListSize();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetLocalisationsListSize();
};
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Gets the string representing nodes of xml trees in table WordList for the keyword word.
* @param word           Keyword from which the string is retrieved
* @return               The string containing nodes of xml trees
*/
class XSQLGetNbOccInFile : public GStorageCmd
{
public:

	XSQLGetNbOccInFile();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetNbOccInFile();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Gets the attributes of node idnode from file idfile. Set attribute address to 0 to skip it
* @param idfile         The id of the file that contains the node
* @param idnode         The id of the node from which the parent has to be found
* @param name           Retrieve the name of the node
* @param idparent       Retrieve the id of the parent of the node
* @param idchild1       Retrieve the id of the first child of the node
* @param nbchild        Retrieve the number of children of the node
* @param filepos        Retrieve the byte position of the contain of tag described by the node in the xml file
* @param filelen        Retrieve the number of bytes of the tag contain
*/
class XSQLGetNodeAttr : public GStorageCmd
{
public:

	XSQLGetNodeAttr();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetNodeAttr();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Gets the id of nodes named "name".
* @param idfile         The id of the file that contains the node
* @param name           The name of the node from which the parent has to be found
* @param idnodes        The vector that contains the ids having the node named "name"
* @param size           The size of the vector idnodes
*/
class XSQLGetIdNodes : public GStorageCmd
{
public:

	XSQLGetIdNodes();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetIdNodes();
};
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Gets the number of nodes of idfile
* @param idfile         The id of the file 

*/
class XSQLGetNbNodes : public GStorageCmd
{
public:

	XSQLGetNbNodes();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetNbNodes();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* One shot function
*Full specif field of nodelist table in a db
* @param idfile         The id of the file that contains the node
* @param idnode        the node 
* @param specif        the specificity of the node : nb nodes 
*/
class XSQLFullSpecifField : public GStorageCmd
{
public:

	XSQLFullSpecifField();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLFullSpecifField();

};
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Gets the url of the file that has an id idfile.
* @param idfile         The id of the file
* @return               The url of the file
*/
class XSQLGetUrlFile : public GStorageCmd
{
public:

	XSQLGetUrlFile();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetUrlFile();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Gets all the urls of xml files saved in tables
* @param rcont          Container in which url will be inserted
*/
class XSQLGetAllXMLFiles : public GStorageCmd
{
public:

	XSQLGetAllXMLFiles();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetAllXMLFiles();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Adds an xml file in the filemanager table.
* @param url            Location of the file
* @param utc            Time of last writing access (number of seconds since 1st Jan 1970)
* @param size           Size of the file
* @return               The id that has been attributed
*/
class XSQLAddXMLFile : public GStorageCmd
{
public:

	XSQLAddXMLFile();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLAddXMLFile();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Adds a node from an xml file into nodelist table.
* @param idfile         The id of the file
* @param idnode         The id of the node
* @param name           The name of the node
* @param parent         The id of the parent node
* @param idchild1       The id of the first child of the node
* @param nbchild        The number of children of the node
* @param idfile         Location of the file
* @param filepos        The byte position of the contain of tag described by the node in the xml file
* @param filelen        The number of bytes of the tag contain
*/
class XSQLAddXMLNode : public GStorageCmd
{
public:

	XSQLAddXMLNode();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLAddXMLNode();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Adds a word found in an xml file into wordlist table.
* @param idfile         The id of the file
* @param word           The word of the file
* @param string_nodes   The set of nodes in which appear the word in the whole xml file
*/
class XSQLAddXMLWord : public GStorageCmd
{
public:

	XSQLAddXMLWord();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLAddXMLWord();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Adds a word found in an xml file into wordlist table.
* @param idfile         The id of the file
* @param word           The word of the file
* @param string_nodes   The set of nodes in which appear the word in the whole xml file
*/
class XSQLUpdateXMLWord : public GStorageCmd
{
public:

	XSQLUpdateXMLWord();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLUpdateXMLWord();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Drops the file from all tables
* @param idfile         The id of the file
*/
class XSQLDropFile : public GStorageCmd
{
public:

	XSQLDropFile();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLDropFile();
};
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Compute TF of a term word in a file idfile
* @param idfile         The id of the file
* @param word 			The word
*/
class XSQLComputeTF : public GStorageCmd
{
public:

	XSQLComputeTF();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLComputeTF();
};
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Compute D total number of documents in the collection 
*/
class XSQLComputeD : public GStorageCmd
{
public:

	XSQLComputeD();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLComputeD();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Compute d number of documents containing a term t
* @param word         word 
*/
class XSQLCompute_d : public GStorageCmd
{
public:

	XSQLCompute_d();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLCompute_d();
};//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Compute N total number of nodes in the collection 
*/
class XSQLComputeN : public GStorageCmd
{
public:

	XSQLComputeN();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLComputeN();
};

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Compute n number of nodes containing a term t
* @param word         word 
*/
class XSQLCompute_n : public GStorageCmd
{
public:

	XSQLCompute_n();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLCompute_n();
};
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

/**
* Get the field specif of the nodelist table for the node idnode of the file idfile
* @param idfile         The id of the file
* @param idnode 			 the node
*/
class XSQLGetSpecif : public GStorageCmd
{
public:

	XSQLGetSpecif();
	void Run(GStorage *storage, const GStorageTag &inst, void *caller);
	~XSQLGetSpecif();
};

#endif
