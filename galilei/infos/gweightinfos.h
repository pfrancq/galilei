/*

	GALILEI Research Project

	GWeightInfos.h

	List of weighted information entities - Header.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

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
#ifndef GWeightInfosH
#define GWeightInfosH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GWeightInfos provides a representation for a list of information entities.
* It implements the vector representing a document/subprofile/group in the
* Vector Model.
* @author Pascal Francq
* @short Weighted Information Entity List.
*/
class GWeightInfos : public R::RContainer<GWeightInfo,true,true>
{
public:

	/**
	* Constructor of a list of weighted information entities.
	* @param max            Maximal number of word created at initialisation.
	*/
	GWeightInfos(unsigned int max) throw(std::bad_alloc);

	/**
	* Copy constructor of a list of weighted information entities.
	* @param w              List of weighted information entities.
	*/
	GWeightInfos(const GWeightInfos& w) throw(std::bad_alloc);

	/**
	* Copy constructor of a list of weighted information entities.
	* @param w              Pointer to a list of weighted information entities.
	*/
	GWeightInfos(const GWeightInfos* w) throw(std::bad_alloc);

	/**
	* Assignement operator for lists of weighted information entities.
	* @param w              Source list of weighted information entities.
	*/
	GWeightInfos& operator=(const GWeightInfos& w) throw(std::bad_alloc);

	/**
	* Assignement operator for lists of weighted information entities.
	* @param w              Source list of weighted information entities.
	*/
	GWeightInfos& operator=(const R::RContainer<GWeightInfo,false,true>& w) throw(std::bad_alloc);

	/**
	* Deep copy of a list of weighted information entities.
	* @param src            Source list of weighted information entities.
	*/
	void Copy(const GWeightInfos& src) throw(std::bad_alloc);

	/**
	* Deep copy of a list of weighted information entities.
	* @param src            Source list of weighted information entities.
	*/
	void Copy(const GWeightInfos* src) throw(std::bad_alloc);

	/**
	* Static function used to order the information entities by weights.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int sortOrder(const void* a,const void* b);

	/**
	* Get a Cursor on the weighted information entities.
	*/
	R::RCursor<GWeightInfo> GetWeightInfoCursor(void);

	/**
	* Insert an information.
	* @param info           Weighted information.
	*/
	void InsertInfo(GWeightInfo* info) throw(std::bad_alloc);

	/**
	* Delete an information.
	* @param info           Weighted information.
	*/
	void DeleteInfo(GWeightInfo* info) throw(std::bad_alloc);

	/**
	* Get an information based on its identifier.
	* @param id             Identifier.
	*/
	GWeightInfo* GetInfo(unsigned int id) const throw(std::bad_alloc);

	/**
	* Clear the container.
	*/
	void Clear(void);

	/**
	* Verify if a list is empty.
	* @return bool
	*/
	bool IsEmpty(void) const {return(!NbPtr);}

	/**
	* Compute the maximal weight of the information entities in the list. The
	* list may not be empty.
	* @return double.
	*/
	double GetMaxWeight(void) const throw(GException);

	/**
	* Compute the maximal absolute weight of the information entities in the list.
	* The list may not be empty.
	* @return double.
	*/
	double GetMaxAbsWeight(void) const throw(GException);

	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors build directly
	* from the lists. If one of the list is empty, the similarity is null.
	* @param w              Pointer to a list of weighted information entities.
	*/
	double Similarity(const GWeightInfos* w) const;

	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors build directly
	* from the lists. If one of the list is empty, the similarity is null.
	* @param w              Reference to a list of weighted information entities.
	*/
	double Similarity(const GWeightInfos& w) const
	{return(Similarity(&w));}

	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors. A vector of a
	* list is build using this list and a Inverse Frequence Factor (IFF) of the
	* object type (idf, isf or ivf) for a given information entity space
	* (language). If one of the list is empty, the similarity is null.
	* @param w              Pointer to a list of weighted information entities.
	* @param ObjType        Type of the object.
	* @param lang           Information entity space (Language).
	*/
	double SimilarityIFF(const GWeightInfos* w,tObjType ObjType,GLang* lang) const throw(GException);

	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors. A vector of a
	* list is build using this list and a Inverse Frequence Factor (IFF) of the
	* object type (idf, isf or ivf) for a given information entity space
	* (language). If one of the list is empty, the similarity is null.
	* @param w              Reference to a list of weighted information entities.
	* @param ObjType        Type of the object.
	* @param lang           Information entity space (Language).
	*/
	double SimilarityIFF(const GWeightInfos& w,tObjType ObjType,GLang* lang) const throw(GException)
	{ return(SimilarityIFF(&w,ObjType,lang));}

	/**
	* Compute a boolean similarity between two lists of weighted information
	* entities. The method verifies that at least a given number, nb, of
	* entities are common to the two lists.
	* @param w              Pointer to a list of weighted information entities.
	* @param nb             Minimal number of entities to be common (0 means
	*                       that the lists should be identifical).
	* @return true if there is at least nb number of common entities.
	*/
	bool SimilarityBool(const GWeightInfos* w,unsigned int nb=0) const;

	/**
	* Compute a boolean similarity between two lists of weighted information
	* entities. The method verifies that at least a given number, nb, of
	* entities are common to the two lists.
	* @param w              Pointer to a list of weighted information entities.
	* @param nb             Minimal number of entities to be common (0 means
	*                       that the all the entities of the list given as
	*                       parameter should be present in the calling one).
	* @return true if there is at least nb number of common entities.
	*/
	bool SimilarityBool(const GWeightInfos& w,unsigned int nb=0) const
	{ return(SimilarityBool(&w,nb));}

protected:

	/**
	* Add the references for the information entities of the object type in a
	* information entity space (language). This information is used for the
	* inverse frequency factors.
	* @param ObjType        Type of the reference.
	* @param lang           Information entity space (Language).
	*/
	void AddRefs(tObjType ObjType,GLang* lang) const throw(GException);

	/**
	* Delete the references for the information entities of the object type in a
	* information entity space (language). This information is used for the
	* inverse frequency factors.
	* @param ObjType        Type of the reference.
	* @param lang           Information entity space (Language).
	*/
	void DelRefs(tObjType ObjType,GLang* lang) const throw(GException);

public:

	/**
	* Modify the list by applying for each information entity the Inverse
	* Frequence Factor (IFF) of the object type (idf, isf or ivf) for a given
	* information entity space (language).
	* @param ObjType        Type of the reference.
	* @param lang           Information entity space (Language).
	*/
	void RecomputeIFF(tObjType ObjType,GLang* lang) throw(GException);

	/**
	* Method used to recompute the weights of the information entities using the
	* formula proposed by Salton and Buckley with the current weights used as
	* initial values. Only the information entities referred at least in one
	* object of the corresponding type are used.
	* @param ObjType        Type of the reference.
	* @param lang           Information entity space (Language).
	*/
	void RecomputeQuery(tObjType ObjType,GLang* lang) throw(GException);

	/**
	* Destructor of a list of weighted information entities.
	*/
	virtual ~GWeightInfos(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
