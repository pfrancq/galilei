/*

	GALILEI Research Project

	GSubProfileVector.h

	Sub-Profile Description using a vector - Header.

	(C) 2002 by P. Francq.

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
#include <galilei.h>
#include <profiles/gsubprofile.h>
#include <infos/giwordsweights.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GSubProfileVector provides a representation for a sub-profile
* described as a vector corresponding to a list of pairs {keyword,weight}.
* @author Pascal Francq.
* @short Sub-Profile Vector Description.
*/
class GSubProfileVector : public GSubProfile//, public GIWordsWeights
{
	/**
	* List representing the sub-profile.
	*/
	GIWordsWeights* Vector;

public:

	/**
	* Constructor of the subprofile.
	* @param prof           Profile.
	* @param id             Identifier.
	* @param lang           Language of the subprofile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	GSubProfileVector(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a) throw(bad_alloc);

	/**
	* Create a new profile.
	* @param prof           Profile.
	* @param id             Identifier.
	* @param lang           Language of the subprofile.
	* @param grp            Group.
	* @param a              String representing the date where it was attached.
	*/
	static GSubProfile* NewSubProfile(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a);

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
	* Compute similarity between SubProfiles.
	*/
	virtual double Similarity(const GSubProfile* desc) const;

	/**
	* Compute similarity between SubProfiles using a global approach.
	*/
	virtual double GlobalSimilarity(const GSubProfile* desc) const;

	/**
	* Compute similarity between a subprofile and a document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute similarity between a subprofile and a document using a global
	* approach.
	*/
	virtual double GlobalSimilarity(const GDoc* doc) const;

	/**
	* Get a cursor over the vector.
	* @return GIWordWeightCursor element.
	*/
	GIWordWeightCursor& GetVectorCursor(void);

	/**
	* Get the number of elements of the vector that are not null.
	*/
	unsigned int GetNbNoNull(void) const;

	/**
	* Get the vector representing the sub-profile.
	* @returns Pointer to GIWordsWeights.
	*/
	GIWordsWeights* GetVector(void) {return(Vector);}

	/**
	* Update the references of the document.
	*/
	void UpdateRefs(void) const;

	/**
	* Remove the references of the document.
	*/
	void RemoveRefs(void) const;

	/**
	* @return Pointer to the Language.
	*/
//	GLang* GetLang(void) const {return(GSubProfile::Lang);}

	/**
	* Destructor.
	*/
	virtual ~GSubProfileVector(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
