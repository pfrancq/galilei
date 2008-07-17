/*

	GALILEI Research Project

	GXMLIndex.h

	XML Index - Header.

	Copyright 2007 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GXMLIndex_H
#define GXMLIndex_H


//------------------------------------------------------------------------------
// include files for R
#include <rvectorint.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gconcept.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GXMLIndex class provides a representation for a XML index. It is composed
* of a XML structure element and a list of stems. Its "name" is defined by:
* @code
* xml_struct_id#stem_lang_code:stem1_id:stem2_id:stem3_id
* @endcode
* @author Pascal Francq
* @short Concept
*/
class GXMLIndex : public GConcept
{
	/**
	 * XML tag representing the first part of the index.
	 */
	GConcept* XMLTag;

	/**
	 * Dictionnary of the stems.
	 */
	GConceptType* Lang;

	/**
	 * Universal terms representing the second part of the index.
	 */
	R::RContainer<GConcept,false,true> Universal;

	/**
	 * Stems representing the third part of the index.
	 */
	R::RContainer<GConcept,false,true> Stems;

public:

	/**
	* Constructor of a concept.
	*/
	GXMLIndex(void);

	/**
	 * Copy constructor.
	 * @param index          Original index.
	 */
	GXMLIndex(const GXMLIndex* index);

	/**
	* Constructor of a XML Index. The full definition of the concept is deduced
	* from its name.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	*/
	GXMLIndex(const R::RString& name,GConceptType* type);

	/**
	 * Constructor of a XML Index.
	 * @param type            Type of the concept.
	 * @param tag            Pointer to the tag.
	 * @param lang           Language of the stems.
	 * @param stems          Universal stems.
	 * @param stems          Language-based stems.
	 */
	GXMLIndex(GConceptType* type,GConcept* tag,GLang* lang,R::RContainer<GConcept,false,true>& uni,R::RContainer<GConcept,false,true>& stems);

	/**
	* Constructor of a XML Index. The full definition of the concept is deduced
	* from its name.
	* @param id              Identifier.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	* @param refdocs         Number of documents referenced.
	* @param refsubprofiles  Number of subprofiles referenced.
	* @param refgroups       Number of groups referenced.
	*/
	GXMLIndex(size_t id,const R::RString& name,GConceptType* type,size_t refdocs,size_t refsubprofiles,size_t refgroups);

protected:

	/**
	 * Build the full definition of the XML index based on its name.
	 */
	void BuildDef(void);

public:

	/**
	 * Do a deep copy of the current concept.
	 * @return Pointer to a new element created.
	 */
	virtual GConcept* DeepCopy(void) const;

	/**
	* Clear all the information of the data.
	*/
	virtual void Clear(void);

	/**
	 * Get the tag implied in this index.
	 */
	GConcept* GetXMLTag(void) const {return(XMLTag);}

	/**
	 * Get the language associated with this stem.
	 */
	GConceptType* GetLangSpace(void) const {return(Lang);}

	/**
	 * Get the universal terms.
	 */
	R::RCursor<GConcept> GetUniversalTerms(void) const;

	/**
	 * Get the language-based stems.
	 */
	R::RCursor<GConcept> GetStems(void) const;

	/**
	 * This method computes the similarity between two XML index. In practice,
	 * both index must have the same tag. Than a raw comparison is done between
	 * the two lists of stems.
	 * @param index          Second XML index.
	 * @returns Number between 0 (nothing in common) and 1 (same XML index).
	 */
	double GetSimilarity(const GXMLIndex* index) const;

	/**
	* Desctuctor of a concept.
	*/
	virtual ~GXMLIndex(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
