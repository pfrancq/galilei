/*

	GALILEI Research Project

	GDocVector.h

	Document in the Vector model - Header.

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
#ifndef GDocVectorH
#define GDocVectorH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdoc.h>
#include <infos/gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDocVector class provides a representation of a document in the vector
* model.
* @author Pascal Francq
* @short Vector Document.
*/
class GDocVector : public GDoc, public GWeightInfos
{
public:

	/**
	* Construct the document.
	* @param url            URL of the document.
	* @param title          Name of the document.
	* @param id             Identifier of the document.
	* @param lang           Language of the document.
	* @param mime           MIME type of the document.
	* @param u              String representing the date of the last update.
	* @param a              String representing the date of the last analysis.
	* @param f              Number of fails.
	* @param nbf            Number of Feedbacks.
	*/
	GDocVector(const char* url,const char* name,unsigned int id,GLang* lang,const char* mime,const char* u,const char* a,unsigned int f,unsigned int nbf=100) throw(bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return("Vector");}

	/**
	* Verify if the document is defined.
	* @return bool.
	*/
	virtual bool IsDefined(void) const;

	/**
	* Get the identificator of the document.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* This function clears the information related to the document.
	* @param l              Must the language be removed.
	*/
	virtual void ClearInfos(bool l);

	/**
	* Add a given information to the document.
	* @param info            Pointer to the information.
	*/
	virtual void AddInfo(GWeightInfo* info) throw(bad_alloc);

	/**
	* Get a Cursor on the weights of the document.
	* @return GWordWeightCursor.
	*/
	GWeightInfoCursor& GetWeightInfoCursor(void);

	/**
	* Compute a similarity between two documents.
	* @param doc             Pointer to a document.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute a similarity between two documents using a Inverse Frequence
	* Factor (IFF).
	* @param doc             Pointer to a document.
	*/
	virtual double SimilarityIFF(const GDoc* doc) const;

	/**
	* Compute a similarity between a document and a subprofile.
	* @param sub             Pointer to a subprofile.
	*/
	virtual double Similarity(const GSubProfile* sub) const;

	/**
	* Compute a similarity between a document and a subprofile using a Inverse
	* Frequence Factor (IFF).
	* @param sub             Pointer to a subprofile.
	*/
	virtual double SimilarityIFF(const GSubProfile* sub) const;

	/**
	* Compute a similarity between a document and a group.
	* @param grp             Pointer to a group.
	*/
	virtual double Similarity(const GGroup* grp) const;

	/**
	* Compute a similarity between a document and a group using a Inverse
	* Frequence Factor (IFF).
	*/
	virtual double SimilarityIFF(const GGroup* grp) const;

	/**
	* Update the references of the document.
	*/
	void UpdateRefs(void) const throw(GException);

	/**
	* Remove the references of the document.
	*/
	void RemoveRefs(void) const throw(GException);

	/**
	* Destruct the document.
	*/
	virtual ~GDocVector(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
