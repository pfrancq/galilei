/*

	GALILEI Research Project

	GDocStruct.h

	Structure of the document content - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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


//------------------------------------------------------------------------------
/**
 *  The GVTDRec provides a representation for a sort of VTD record to represent
 *  a XML node (Tag, attribute, value or content).
 *  @author Pascal Francq
 *  @short VTD Record
 */
class GVTDRec
{
public:

	/**
	* The Type enum represents the different types of VTD Record..
	* @short VTD Record Type.
	*/
	enum RecType
	{
		Tag                       /** Tag.*/,
		Attribute                 /** Attribute.*/,
		Value                     /** Value of attribute.*/,
		Content                   /** Content. */
	};

private:

	/**
	* Concept.
	 */
	GConcept* Concept;

	/**
	 * The type of the node.
	 */
	RecType Type;

	/**
	 * Position in the document.
	 */
	size_t Pos;

	/**
	 * Depth and type of of the node. The first four bytes indicates the type,
	 * the rest the depth (maximal depth of 63).
	 */
	char Depth;

public:

	/**
	 * Construct a VTD Record.
	 * @param concept        Concept associated with the record.
	 * @param type           Type of the record.
	 * @param pos            Position in the file.
	 * @param depth          Depth of the record.
	 */
	GVTDRec(GConcept* concept,RecType type,size_t pos,char depth);

	/**
	 * Compare method used by R::RContainer.
	 */
	int Compare(const GVTDRec&) const {return(-1);}

	/**
	 * Get the concept.
	 */
	GConcept* GetConcept(void) const {return(Concept);}

	/**
	 * Get the type.
	 */
	RecType GetType(void) const {return(Type);}

	/**
	 * Get the position in the file.
	 */
	size_t GetPos(void) const {return(Pos);}

	/**
	 * Get the depth of the record.
	 */
	char GetDepth(void) const {return(Depth);}
};


//------------------------------------------------------------------------------
/**
 * The GDocStruct provides a representation for the tree structure of weighted
 * information entities for a given document.
 * @author Pascal Francq
 * @short Structure of Document.
 */
class GDocStruct
{
	class GLC;

	/**
	 * VTD records of the document.
	 */
	R::RContainer<GVTDRec,true,false> Recs;

	/**
	 * Location caches.
	 */
	R::RContainer<GLC,true,false>* LCs;

public:

	/**
	 * Constructor.
	 * @param vtd            Number of VTD records.
	 * @param lc             Number of location caches.
	 */
	GDocStruct(void);

	/**
	 * Copy constructor.
	 * @param docstruct      Original structure.
	 */
	GDocStruct(const GDocStruct& docstruct);

	/**
	 * Constructor.
	 * @param vtd            Number of VTD records.
	 * @param lc             Number of location caches.
	 */
	GDocStruct(size_t vtd,size_t lc);

	/**
	 * Set the sizes of the container if know.
	 * @param vtd            Number of VTD records.
	 * @param lc             Number of location caches.
	 */
	void SetSizes(size_t vtd,size_t lc);

	/**
	 * Get the number of "VTD" record in the structure (0 implies that the
	 * structure is empty).
	 */
	size_t GetNbRecs(void) const;

	/**
	 * Get the number of Location caches for the structure.
	 * @return
	 */
	size_t GetNbLCs(void) const;

	/**
	 * Get the number of entries in a given location cache.
	 * @param level          Level of the location cache.
	 */
	size_t GetNbLCEntries(size_t level) const;

	/**
	 * Set the number of entries at a given location cache.
	 * @param level          Level of the location cache.
	 * @param size           Size.
	 */
	void SetNbLCEntries(size_t level,size_t size) const;

	/**
	 * Add a record to the structure.
	 * @param concept        Concept associated with the record.
	 * @param type           Type of the record.
	 * @param pos            Position in the file.
	 * @param depth          Depth of the record.
	 * @param child          Position of the first child in the next level.
	 * @param nbrecs         An idea of the number of records to store to the
	 *                       given level.
	 * @return Pointer to the record created.
	 */
	GVTDRec* AddRecord(GConcept* concept,GVTDRec::RecType type,size_t pos,char depth,size_t child=SIZE_MAX,size_t nbrecs=0);

	/**
	 * Get a pointer to the record.
	 */
	R::RCursor<GVTDRec> GetRecs(void) const;

	/**
	 * Get the position of the first child in the next level.
	 * @param rec            Record to search from.
	 * @return SIZE_MAX if the tag has no child.
	 */
	size_t GetFirstChild(GVTDRec* rec) const;

	/**
	 * Clear the structure.
	 */
	void Clear(void);

	/**
	 * Destruct the structure.
	 */
	~GDocStruct(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
