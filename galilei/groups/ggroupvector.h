/*

	GALILEI Research Project

	GGroupVector.h

	Group in the Vector Model - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GGroupVectorH
#define GGroupVectorH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggroup.h>
#include <infos/gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GGroupVector provides a representation for a group in the vector model.
* @author Pascal Francq.
* @short Vector Group.
*/
class GGroupVector : public GGroup, public GWeightInfos
{
public:

	/**
	* Construct a group with a specific identificator.
	* @param id             Identificator.
	* @param lang           Language.
	* @param com            Community.
	*/
	GGroupVector(unsigned int id,GLang* lang,bool com) throw(std::bad_alloc);

	/**
	* Construct a group with an invalid identificator.
	* @param lang           Language.
	* @param com            Community.
	*/
	GGroupVector(GLang* lang,bool com) throw(std::bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return("Vector");}

	/**
	* Verify if the group is defined.
	* @return bool.
	*/
	virtual bool IsDefined(void) const;

	/**
	* Add a given information to the group.
	* @param info            Pointer to the information.
	*/
	virtual void AddInfo(GWeightInfo* info) throw(std::bad_alloc);

	/**
	* Get a cursor over the vector.
	* @return GWeightInfoCursor element.
	*/
	GWeightInfoCursor& GetWeightInfoCursor(void);

	/**
	* Compute the similarity between a group and a document.
	* @param doc             Pointer to a document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute a similarity between a group and a document using a Inverse
	* Frequence Factor (IFF).
	* @param doc             Pointer to a document.
	*/
	virtual double SimilarityIFF(const GDoc* doc) const throw(GException);

	/**
	* Compute the similarity between a group and subprofile.
	* @param sub             Pointer to a subprofile.
	*/
	virtual double Similarity(const GSubProfile* sub) const;

	/**
	* Compute a similarity between a group and a subprofile using a Inverse
	* Frequence Factor (IFF).
	* @param sub             Pointer to a subprofile.
	*/
	virtual double SimilarityIFF(const GSubProfile* sub) const throw(GException);

	/**
	* Compute the similarity between groups.
	* @param grp             Pointer to a group.
	*/
	virtual double Similarity(const GGroup* grp) const;

	/**
	* Compute a similarity between two groups using a Inverse Frequence Factor
	* (IFF).
	* @param grp             Pointer to a group.
	*/
	virtual double SimilarityIFF(const GGroup* grp) const throw(GException);

	/**
	* Update the references of the group. This method does the update only for
	* groups which are community.
	*/
	void UpdateRefs(void) const throw(GException);

	/**
	* Remove the references of the group. This method does the update only for
	* groups which are community.
	*/
	void RemoveRefs(void) const throw(GException);

	/**
	* Destructor of a group.
	*/
	~GGroupVector(void) throw(GException);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
