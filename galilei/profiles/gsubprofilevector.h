/*

	GALILEI Research Project

	GSubProfileVector.h

	Subprofile in the Vector Model - Header.

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
#ifndef GSubProfileVectorH
#define GSubProfileVectorH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <profiles/gsubprofile.h>
#include <infos/gweightinfos.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GSubProfileVector provides a representation for a subprofile described as
* a vector corresponding to a list of pairs {keyword,weight}.
* @author Pascal Francq.
* @short Vector model subprofile.
*/
class GSubProfileVector : public GSubProfile, public GWeightInfos
{
public:

	/**
	* Constructor of the subprofile.
	* @param prof           Profile.
	* @param id             Identifier.
	* @param lang           Language of the subprofile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfileVector(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a,tObjState state,const char* c) throw(bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return("Vector");}

	/**
	* Create a new profile.
	* @param prof           Profile.
	* @param id             Identifier.
	* @param lang           Language of the subprofile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	static GSubProfile* NewSubProfile(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a,tObjState state,const char* c);

	/**
	* Method called when the profiles feedbacks were cleared.
	*/
	virtual void ClearFdbks(void);

	/**
	* See if the subprpfile is defined, i.e. if it is computed. A subprofile
	* that isn't computed, isn't attached.
	*/
	virtual bool IsDefined(void) const;

	/**
	* Add a word with the corresponding weight to the description.
	* @param id             Identificator of the word.
	* @param w              Weight of the word.
	*/
	void AddWord(unsigned int id,double w) throw(bad_alloc);

	/**
	* Add a concept with the corresponding weight to the description.
	* @param id             Identificator of the concept.
	* @param w              Weight of the concept.
	*/
	void AddWordList(unsigned int id,double w) throw(bad_alloc);

	/**
	* Compute the similarity between a subprofile and a document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute the global similarity between a subprofile and a document.
	*/
	virtual double SimilarityIFF(const GDoc* doc) const;

	/**
	* Compute the similarity between subprofiles.
	*/
	virtual double Similarity(const GSubProfile* sub) const;

	/**
	* Compute the global similarity between subprofiles.
	*/
	virtual double SimilarityIFF(const GSubProfile* sub) const;

	/**
	* Compute the similarity between a subprofile and a group.
	*/
	virtual double Similarity(const GGroup* grp) const;

	/**
	* Compute the global similarity between a subprofile and a group.
	*/
	virtual double SimilarityIFF(const GGroup* grp) const;

	/**
	* Get a cursor over the vector.
	* @return GWeightInfoCursor element.
	*/
	GWeightInfoCursor& GetWeightInfoCursor(void);

	/**
	* Get the number of elements of the vector that are not null.
	*/
	unsigned int GetNbNoNull(void) const;

	/**
	* Get the vector representing the sub-profile.
	* @returns Pointer to GWeightInfos.
	*/
	GWeightInfos* GetVector(void) {return(this);}

	/**
	* Update the references of the document.
	*/
	void UpdateRefs(void) const;

	/**
	* Remove the references of the document.
	*/
	void RemoveRefs(void) const;

	/**
	* Destructor.
	*/
	virtual ~GSubProfileVector(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
