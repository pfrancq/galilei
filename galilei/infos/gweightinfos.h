/*

	GALILEI Research Project

	GWeightInfos.h

	Weights of a list of words - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

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
#ifndef GWeightInfosH
#define GWeightInfosH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/ginfo.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GWeightInfos provides a representation for the words weights of a given
* collection of words, for example a document or a set of documents.
* It implements the vector representing a document/profile/group in the Vector
* Model.
* @author Pascal Francq
* @short Vector in Words Space.
*/
class GWeightInfos : public R::RContainer<GWeightInfo,unsigned,true,true>
{
	/**
	* Total number of words in the set of documents analysed.
	*/
	double NbWordsDocs;

public:

	/**
	* Constructor.
	* @param nb             Maximal number of word created at initialisation.
	*/
	GWeightInfos(unsigned int nb) throw(bad_alloc);

	/**
	* The assignement operator.
	* @param src            Vector used as source.
	*/
	GWeightInfos& operator=(const GWeightInfos& src) throw(bad_alloc);

	/**
	* Static function used to ordered by frenquecy.
	*/
	static int sortOrder(const void *a,const void *b);

	/**
	* Clear the statistics.
	*/
	void Clear(void);

	/**
	* Look if the list is empty.
	*/
	bool IsEmpty(void) const {return(!NbPtr);}

	/**
	* Compute the maximal weights in this list.
	* @return double.
	*/
	double GetMaxWeight(void) const;

	/**
	* Compute similarity between two vectors.
	* @param w              Vector to compare with.
	*/
	double Similarity(const GWeightInfos* w) const;

	/**
	* Compute similarity between two vectors by using the idf factors rather
	* than the weights.
	* @param w              Vector to compare with.
	* @param ObjType        Type of the reference.
	* @param lang           Language.
	*/
	double SimilarityIdfP(const GWeightInfos* w,tObjType ObjType,GLang* lang) const;

	/**
	* Compute similarity between two vectors by using the idf factors rather
	* than the weights.
	* @param w              Vector to compare with.
	* @param ObjType        Type of the reference.
	* @param lang           Language.
	*/
	double SimilarityIdf(const GWeightInfos* w,tObjType ObjType,GLang* lang) const;

	/**
	* Add the references for the words contained in the vector for a given
	* object type.
	* @param ObjType        Type of the reference.
	* @param dic            Dictionnary.
	*/
	void AddRefs(tObjType ObjType,GDict* dic) const;

	/**
	* Delete the references for the words contained in the vector for a given
	* object type.
	* @param ObjType        Type of the reference.
	* @param dic            Dictionnary.
	*/
	void DelRefs(tObjType ObjType,GDict* dic) const;

	/**
	* Transform the vector from a independent weight to the form compatible
	* with the vector model.
	* @param ObjType        Type of the reference.
	* @param lang           Language.
	*/
	void Transform(tObjType ObjType,GLang* lang);

	/**
	* Method used to recompute the weights of the terms like the formula
	* proposed by Salton and Buckley using the current weights as initial
	* values. Only the index terms refered at least in one group are taken.
	* @param ObjType        Type of the reference.
	* @param lang           Language.
	*/
	void ModifyQueryGroups(tObjType ObjType,GLang* lang);

	/**
	* Method used to recompute the weights of the terms like the formula
	* proposed by Salton and Buckley using the current weights as initial
	* values.
	* @param ObjType        Type of the reference.
	* @param lang           Language.
	*/
	void ModifyQuery(tObjType ObjType,GLang* lang);

	/**
	* Destructor.
	*/
	virtual ~GWeightInfos(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
