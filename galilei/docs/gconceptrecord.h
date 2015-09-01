/*

	GALILEI Research Project

	GDocRecord.h

	Concept Record - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef GConceptRecordH
#define GConceptRecordH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * The GConceptRecord provides a representation for a "concept record", i.a. an
 * occurrence of a given concept in a given object (such as a document).
 * @short Concept Record.
 */
class GConceptRecord
{
protected:

	/**
	 * Type of the token corresponding to the node.
	 */
	tTokenType Type;

	/**
	* Concept Identifier.
	 */
	size_t ConceptId;

	/**
	 * Syntactic position in the document.
	 */
	size_t SyntacticPos;

	/**
	 * Position in the document (in byte).
	 */
	size_t Pos;

	/**
	 * Syntactic depth of the node.
	 */
	size_t SyntacticDepth;

	/**
	 * Index of the node.
	 */
	size_t Index;

public:

	/**
	 * Construct a concept node.
	 */
	GConceptRecord(void);

	/**
	 * Construct a concept node.
	 * @rec                  Record.
	 */
	GConceptRecord(const GConceptRecord& rec);

	/**
	 * Construct a concept node.
	 * @param conceptid      Identifier of the concept.
	 */
	GConceptRecord(size_t conceptid);

	/**
	 * Construct a concept node.
	 * @param type           Type of the token.
	 * @param conceptid      Identifier of the concept.
	 * @param synpos         Syntactic position.
	 * @param pos            Position in the file.
	 * @param syndepth       Syntactic depth of the record.
	 * @param idx            Index of the record.
	 */
	GConceptRecord(tTokenType type,size_t conceptid,size_t synpos,size_t pos,size_t syndepth,size_t idx);

	/**
	 * Compare two nodes regarding their depths and positions
    * @param node           Node to compare with.
    * @return a number compatible with R::RContainer.
    */
	int Compare(const GConceptRecord& node) const;

	/**
    * @return the type of the token corresponding to the node.
    */
	tTokenType GetType(void) const {return(Type);}

	/**
	 * @return the concept identifier.
	 */
	size_t GetConceptId(void) const {return(ConceptId);}

	/**
	 * @return the syntactic position in the file.
	 */
	size_t GetSyntacticPos(void) const {return(SyntacticPos);}

	/**
	 * @return the position in the file.
	 */
	size_t GetPos(void) const {return(Pos);}

	/**
	 * @return the depth of the record.
	 */
	size_t GetSyntacticDepth(void) const {return(SyntacticDepth);}

	/**
	 * @return the index of the node.
	 */
	size_t GetIndex(void) const {return(Index);}

	/**
	 * Assignment record.
    * @param rec            Record which contain will be assign.
    * @return the record assigned.
    */
	GConceptRecord& operator=(const GConceptRecord& rec);

	/**
	 * Equality operator. Each field must be identical (except the index).
    * @param rec            Record to compare with.
    * @return true if the two records are identical.
    */
	bool operator==(const GConceptRecord& rec) const;

	/**
	 * Inequality operator. At least one field must be different (except the
	 * index).
    * @param rec            Record to compare with.
    * @return true if the two records are different.
    */
	bool operator!=(const GConceptRecord& rec) const;

	/**
	 * Simply print the node information on the screen;
	 */
	void Print(void) const;

	/**
	 * Destructor.
	 */
	virtual ~GConceptRecord(void);

	friend class GObjects<GDoc,hDocs>;
};


//------------------------------------------------------------------------------
/**
 * The GConceptRecordIterator class provides an iterator to parse all the child
 * records of a given record. For each child record found, the Treat method is
 * called.
 *
 * Here is an example of a code that computes the total number of child records
 * for a given record.
 * @code
 * class Iterator : public GConceptRecordIterator
 * {
 *   size_t& Nb;
 *   public:
 *   Iterator(GDoc* doc,const GConceptRecord& rec,size_t& nb)
 *     : GConceptRecordIterator(doc,rec), Nb(nb)	{}
 *
 *   virtual void Treat(GConceptRecord& child)
 *   {
 *     Nb+=GetDoc()->GetSession()->GetNbTotalChildRecords(GetDoc(),child);
 *   }
 *};
 *
 * // Suppose a record Rec of a document Doc
 * size_t Nb;
 * Iterator(Doc,Rec,Nb).Parse();
 * @endcode
 * @short Concept Record Iterator.
 */
class GConceptRecordIterator
{
	/**
	 * Document corresponding to the record.
	 */
	GDoc* Doc;

	/**
	 * Record for which the children must be parsed.
    */
	const GConceptRecord& Record;

public:

	/**
	 * Constructor of the iterator.
    * @param doc            Document.
    * @param parent         Parent record.
    */
	GConceptRecordIterator(GDoc* doc,const GConceptRecord& parent);

	/**
	 * Get the document corresponding to the record.
	 * @return a pointer to a GDoc.
	 */
	GDoc* GetDoc(void) const {return(Doc);}

	/**
	 * Get the record for which the children must be parsed.
	 * @return a pointer to a GConceptRecord.
    */
	const GConceptRecord& GetRecord(void) const {return(Record);}

	/**
	 * Parse the iterator.
    */
	void Parse(void);

private:

	/**
	 * Method called for each child record found.
    * @param child          Child record found.
    */
	virtual void Treat(GConceptRecord& child)=0;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
