/*

	GALILEI Research Project

	GSubProfileVector.h

	Subprofile in the Vector Model - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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
#ifndef GSubProfileVectorH
#define GSubProfileVectorH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <profiles/gsubprofile.h>
#include <infos/gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GSubProfileVector provides a representation for a subprofile described as
* a vector corresponding to a list of pairs {keyword,weight}.
* @author Pascal Francq.
* @short Vector Subprofile.
*/
class GSubProfileVector : public GSubProfile, public GWeightInfos
{
public:

	/**
	* Constructor of the subprofile.
	* @param prof            Profile.
	* @param id              Identifier.
	* @param lang            Language of the subprofile.
	* @param grp             Group.
	* @param a               String representing the date where it was attached.
	* @param u               String representing the date of the last updated.
	* @param c               String representing the date of the last computation.
	*/
	GSubProfileVector(GProfile* prof,unsigned int id,GLang* lang,GGroup* grp,const char* a,const char* u,const char* c) throw(std::bad_alloc);

	/**
	* Constructor of the subprofile.
	* @param session         Session.
	* @param prof            Profile.
	* @param lang            Language of the subprofile.
	*/
	GSubProfileVector(GSession* session,GProfile* prof,GLang* lang) throw(std::bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return("Vector");}

	/**
	* Clear all the assessments of the subprofile.
	*/
	virtual void ClearFdbks(void) throw(std::bad_alloc,GException);

	/**
	* Verify if the subprofile is defined, i.e. if it is computed.
	* @return bool.
	*/
	virtual bool IsDefined(void) const;

	/**
	* Get the identificator of the subprofile.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Add a given information to the document.
	* @param info            Pointer to the information.
	*/
	virtual void AddInfo(GWeightInfo* info) throw(std::bad_alloc);

	/**
	* Compute the similarity between a subprofile and a document.
	* @param doc             Pointer to a document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute the similarity between a subprofile and a document using a Inverse
	* Frequence Factor (IFF).
	* @param doc             Pointer to a document.
	*/
	virtual double SimilarityIFF(const GDoc* doc) const throw(GException);

	/**
	* Compute the similarity between subprofiles.
	* @param sub             Pointer to a subprofile.
	*/
	virtual double Similarity(const GSubProfile* sub) const;

	/**
	* Compute the similarity between subprofiles using a Inverse Frequence
	* Factor (IFF).
	* @param sub             Pointer to a subprofile.
	*/
	virtual double SimilarityIFF(const GSubProfile* sub) const throw(GException);

	/**
	* Compute the similarity between a subprofile and a group.
	* @param grp             Pointer to a group.
	*/
	virtual double Similarity(const GGroup* grp) const;

	/**
	* Compute the similarity between a subprofile and a group using a Inverse
	* Frequence Factor (IFF).
	* @param grp             Pointer to a group.
	*/
	virtual double SimilarityIFF(const GGroup* grp) const throw(GException);

	/**
	* Get a cursor over the vector.
	* @return GWeightInfoCursor element.
	*/
	GWeightInfoCursor GetWeightInfoCursor(void);

	/**
	* Get the number of elements of the vector that are not null.
	*/
	unsigned int GetNbNoNull(void) const;

	/**
	* Update the references of the document.
	*/
	void UpdateRefs(void) const throw(GException);

	/**
	* Remove the references of the document.
	*/
	void RemoveRefs(void) const throw(GException);

	/**
	* Destructor of a subprofile represented by a vector.
	*/
	virtual ~GSubProfileVector(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
