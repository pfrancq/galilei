/*

	GALILEI Research Project

	GWeightInfos.h

	List of weighted information entities - Header.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <galilei.h>
#include <gsession.h>
#include <gindexer.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GWeightInfos provides a representation for a list of information entities.
* It implements multiple vectors (one vector per concepts space) representing a
* document/profile/community in the extended vector Model.
* @author Pascal Francq
* @short Weighted Information Entities.
*/
class GWeightInfos : public R::RContainer<GWeightInfo,true,true>
{
protected:

	/**
	* State of the object.
	*/
	tObjState State;

public:

	/**
	* Constructor of a list of weighted information entities.
	* @param max            Maximal number of word created at initialization.
	*/
	GWeightInfos(size_t max);

	/**
	* Copy constructor of a list of weighted information entities.
	* @param w              List of weighted information entities.
	*/
	GWeightInfos(const GWeightInfos& w);

	/**
	* Assignment operator for lists of weighted information entities.
	* @param w              Source list of weighted information entities.
	*/
	GWeightInfos& operator=(const GWeightInfos& w);

	/**
	* Assignment operator for lists of weighted information entities.
	* @param w              Source list of weighted information entities.
	*/
	void CopyInfos(const R::RContainer<GWeightInfo,false,true>* infos);

	/**
	* Deep copy of a list of weighted information entities.
	* @param src            Source list of weighted information entities.
	*/
	void Copy(const GWeightInfos& src);

	/**
	* Static function used to order the information entities by weights.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int sortOrder(const void* a,const void* b);

	/**
	* Return the state of the document.
	* @returns tObjState value.
	*/
	inline tObjState GetState(void) const {return(State);}

	/**
	* Set the state of the document.
	* @param state           New state.
	*/
	void SetState(tObjState state);

protected:

	/**
	 * Generic method to load information from the storage.
	 * @param type           Type of the object.
	 * @param id             Identifier of the object.
	 */
	inline void LoadInfos(tObjType type,size_t id)
	{
		GSession* session=GSession::Get();
		if(session)
			session->GetIndexer()->LoadInfos(*this,type,id);
		State=osUpToDate;
	}

	/**
	* Load information from the current storage. This method must be rewritten in
	* the child classes, but the LoadInfos(tObjType,size_t) method can be used.
	*/
	virtual void LoadInfos(void);

public:

	/**
	* Get a Cursor on the weighted information entities.
	*/
	R::RCursor<GWeightInfo> GetInfos(void) const;

	/**
	* Insert an information.
	* @param info           Weighted information.
	*/
	void InsertInfo(GWeightInfo* info);

	/**
	* Delete an information.
	* @param info           Weighted information.
	*/
	void DeleteInfo(GWeightInfo* info);

	/**
	 * Get a pointer to a weighted information based on a given one. If it is
	 * not found, it is created with a null weight.
	 * @param info           Weighted information.
	 */
	GWeightInfo* GetInfo(const GWeightInfo* info);

	/**
	* Clear the container.
	*/
	void Clear(void);

	 /**
 	* Get the number of elements in the container.
 	* @return size_t.
 	*/
 	size_t GetNb(void) const;

	/**
	* @return True if a list is not empty.
	*/
	virtual bool IsDefined(void) const {return(GetNb());}

	/**
	* Compute the maximal weight of the information entities in the list for a
	* given concept type. The list may not be empty.
	* @param type            Type of the concepts.
	* @return double.
	*/
	double GetMaxWeight(GConceptType* type) const;

	/**
	* Compute the maximal absolute weight of the information entities in the
	* list for a given concept type. The list may not be empty.
	* @param type            Type of the concepts.
	* @return double.
	*/
	double GetMaxAbsWeight(GConceptType* type) const;

	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors build directly
	* from the lists. If one of the list is empty, the similarity is null.
	* @param w              Pointer to a list of weighted information entities.
	*/
	double Similarity(const GWeightInfos& w) const;

	/**
	 * @return the inclusion factor of the lists of weighted information
	 * entities. The methods computes a ratio (weighted by the tf-idf factors)
	 * of the elements of the first list being in the second one.
	 * @param w              Pointer to a list of weighted information entities.
	 * @param ObjType        Type of the object.
	 */
	double Inclusion(const GWeightInfos& w,tObjType ObjType) const;

	/**
	* @return the similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors. A vector of a
	* list is build using this list and a Inverse Frequence Factor (IFF) of the
	* object type (idf, isf or ivf) for a given information entity space
	* (language). If one of the list is empty, the similarity is null.
	* @param w              Pointer to a list of weighted information entities.
	* @param ObjType        Type of the object.
	*/
	double SimilarityIFF(const GWeightInfos& w,tObjType ObjType) const;

	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors. A vector of a
	* list is build using this list and two Inverse Frequence Factors (IFF) of
	* the object type (idf, isf or ivf) for a given information entity space
	* (language). If one of the list is empty, the similarity is null.
	* @param w              Reference to a list of weighted information entities.
	* @param ObjType1       First type of the object.
	* @param ObjType2       Second type of the object.
	*/
	double SimilarityIFF2(const GWeightInfos& w,tObjType ObjType1,tObjType ObjType2) const;

	/**
	* Compute a boolean similarity between two lists of weighted information
	* entities. The method verifies that at least a given number, nb, of
	* entities are common to the two lists.
	* @param w              List of weighted information entities.
	* @param nb             Minimal number of entities to be common (0 means
	*                       that the lists should be identical).
	* @return true if there is at least nb number of common entities.
	*/
	bool SimilarityBool(const GWeightInfos& w,size_t nb=0) const;

	/**
	 * Extract all information entities related to a given language an put them
	 * into a given list that will be emptied.
	 * @param dest           Destination list of weighted information entities.
	 * @param lang           Language selected.
	 * @param universal      Should universal concepts be copied.
	 */
	void Extract(GWeightInfos& dest,const GLang* lang,bool universal=true);

protected:

	/**
	* Add the references for the information entities of the object type in a
	* information entity space (language). This information is used for the
	* inverse frequency factors.
	* @param ObjType        Type of the reference.
	*/
	void AddRefs(tObjType ObjType) const;

	/**
	* Delete the references for the information entities of the object type in a
	* information entity space (language). This information is used for the
	* inverse frequency factors.
	* @param ObjType        Type of the reference.
	*/
	void DelRefs(tObjType ObjType) const;

public:

	/**
	* Modify the list by applying for each information entity the Inverse
	* Frequence Factor (IFF) of the object type (idf, isf or ivf).
	* @param ObjType        Type of the reference.
	*/
	void RecomputeIFF(tObjType ObjType);

	/**
	* Method used to recompute the weights of the information entities using the
	* formula proposed by Salton and Buckley with the current weights used as
	* initial values. Only the information entities referred at least in one
	* object of the corresponding type are used.
	* @param ObjType        Type of the reference.
	* @param lang           Information entity space (Language).
	*/
	void RecomputeQuery(tObjType ObjType,GLang* lang);

	/**
	* Destructor of a list of weighted information entities.
	*/
	virtual ~GWeightInfos(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
