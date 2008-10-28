/*

	GALILEI Research Project

	GDocStruct.h

	Structure of the document content - Header.

	Copyright 2008 by the Université Libre de Bruxelles.

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
#ifndef GDocStructH
#define GDocStructH


//------------------------------------------------------------------------------
// include files for R Project
#include <rnode.h>
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

class VTR
{
public:

	/**
	* The Type enum represents the different types of nodes in a XML structure.
	* @short XML Node Type
	*/
	enum NodeType
	{
		Tag                       /** Tag.*/,
		Attribute                 /** Attribute.*/,
		Value                     /** Value of attribute.*/,
		Content                   /** Content. */
	};

private:

	/**
	* Weighted information entity.
	 */
	GWeightInfo* Info;

	/**
	 * Position in the document.
	 */
	size_t Pos;

	/**
	 * Index of in the location cache.
	 */
	size_t Child;

	/**
	 * Depth and type of of the node. The first four bytes indicates the type,
	 * the rest the depth (maximal depth of 63).
	 */
	char TypeDepth;

public:
};

class LocationCache
{
	/**
	 * Index of the first child. A null index indicates no child.
	 */
	size_t Child;
public:
};


//------------------------------------------------------------------------------
/**
 * The GOccurInfo class provides a representation for an occurrence of an
 * weighted information entity.
 * @author Pascal Francq
 * @short Occurrence of a Weighted Information Entity.
 */
class GOccurInfo
{
protected:

	/**
	 * Weighted information entity.
	 */
	GWeightInfo* Info;

	/**
	 * Position of the occurrence.
	 */
	size_t Pos;

public:

	/**
	 * Constructor.
	 * @param info           Information entity.
	 * @param pos            Position.
	 */
	GOccurInfo(GWeightInfo* info,size_t pos);

	/**
	 * Compare two elements.
	 * @param info           Element to compare with.
	 * @see R::RContainer
	 */
	int Compare(const GOccurInfo& info) const;

	/**
	 * Get the corresponding information entity.
	 */
	GWeightInfo* GetInfo(void) const {return(Info);}

	/**
	 * Get the position of the node in the XML file.
	 */
	size_t GetPos(void) const {return(Pos);}

	/**
	 * Print information on the standard output.
	 */
	void Print(void);
};


//------------------------------------------------------------------------------
/**
 * The GOccursInfo represent a list of occurrences of a given information entity
 * in a document.
 * @author Pascal Francq
 * @short Occurrences of a Weighted Information Entity.
 */
class GOccursInfo : public R::RContainer<GOccurInfo,false,false>
{
	/**
	 * Weighted information entity.
	 */
	GWeightInfo* Info;

public:

	/**
	 * Constructor.
	 * @param info           Information entity.
	 */
	GOccursInfo(GWeightInfo* info);

	/**
	 * Compare two elements.
	 * @param info           Element to compare with.
	 * @see R::RContainer
	 */
	int Compare(const GOccursInfo& info) const;

	/**
	 * Compare two elements.
	 * @param concept        Concept to compare with.
	 * @see R::RContainer
	 */
	int Compare(const GConcept& concept) const;
};


//------------------------------------------------------------------------------
/**
 * The GDocStructNode provides a representation for a XML node of a given
 * document.
 * @author Pascal Francq
 * @short Node of a Document Structure.
 */
class GDocStructNode : public GOccurInfo, public R::RNode<GDocStructNode,true,false>
{
public:

	/**
	* The Type enum represents the different types of nodes in a XML structure.
	* @short XML Node Type
	*/
	enum NodeType
	{
		Root                      /** Root node (Not the root XML tag).*/,
		Tag                       /** Tag.*/,
		Attribute                 /** Attribute.*/
	};

private:

	/**
	 * Type of the node.
	 */
	NodeType Type;

	/**
	 * Content of the node.
	 */
	R::RContainer<GOccurInfo,true,false> Content;

public:

	/**
	 * Constructor.
	 * @param info           Information entity.
	 * @param pos            Position.
	 * @param type           Type of the node.
	 */
	GDocStructNode(GWeightInfo* info,size_t pos,NodeType type);

	/**
	 * Get the type of the node.
	 */
	NodeType GetType(void) const {return(Type);}

	/**
	 * Insert a content to the node.
	 * @param info           Information to insert.
	 */
	void InsertContent(GOccurInfo* info);

	/**
	 * Get the content (text or value) of the node.
	 */
	R::RCursor<GOccurInfo> GetContent(void) const;

	/**
	 * Print information on the standard output.
	 */
	void Print(void);
};


//------------------------------------------------------------------------------
/**
 * The GDocStruct provides a representation for the tree structure of weighted
 * information entities for a given document.
 * @author Pascal Francq
 * @short Structure of Document.
 */
class GDocStruct : private R::RTree<GDocStructNode,true,false>
{
	/**
	 * Information entities.
	 */
	R::RContainer<GOccursInfo,true,true> Infos;

public:

	/**
	 * Constructor.
	 * @nb                   Number of entities in the document.
	 */
	GDocStruct(size_t nb);

	/**
	 * Get the top node.
	 */
	GDocStructNode* GetTop(void) const {return(R::RTree<GDocStructNode,true,false>::GetTop());}

	/**
	 * Get the total number of nodes.
	 */
	size_t GetNbNodes(void) const {return(R::RTree<GDocStructNode,true,false>::GetNbNodes());}

	/**
	 * Get the list of occurrences of a concept in the document.
	 * @param concept        Concept.
	 */
	R::RCursor<GOccurInfo> GetOccurences(const GConcept* concept) const;

	/**
	 * Insert an occurrence of node in the document.
	 * @param parent         Parent node.
	 * @param occur          Occurrence to insert.
	 */
	void InsertNode(GDocStructNode* parent,GDocStructNode* occur);

	/**
	 * Insert an occurrence of a specific content the document.
	 * @param parent         Parent node.
	 * @param info           Information entity.
	 * @param pos            Position.
	 */
	void InsertContent(GDocStructNode* parent,GWeightInfo* info,size_t pos);

	/**
	 * Print the content of the structure on the standard output.
	 * @param ptr            Pointer of the node where to start. A null pointer
	 *                       represents the top node.
	 */
	void Print(GDocStructNode* ptr=0);

	/**
	 * Clear the structure.
	 */
	void Clear(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
