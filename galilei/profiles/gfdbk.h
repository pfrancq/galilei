/*

	GALILEI Research Project

	GFdbk.h

	Feedback - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GFdbkH
#define GFdbkH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GFdbk class provides a representation for feedback on a document. It can
* be an explicit assessment of a user on the relevance on a document, a
* computed assessment or a relation.
* @author Pascal Francq
* @short Feedback on a Document.
*/
class GFdbk
{
	/**
	* Identifier of the document.
	*/
	size_t DocId;

	/**
	* Type of the feedback.
	*/
	tFdbkType Fdbk;

	/**
	* When the feedback was done.
	*/
	R::RDate Done;

	/**
	* Date of the last computation of the document when it was modified.
	*/
	R::RDate Computed;

public:

	/**
	* Constructor.
	* @param docid           Identifier of the document.
	* @param fdbk            Feedback type.
	* @param done            Date.
	* @param computed        Last computation.
	*/
	GFdbk(size_t docid,tFdbkType fdbk,const R::RDate& done,const R::RDate& computed);

	/**
	* Compare two assessments to order them using the document identifier.
	* @see R::RContainer
	* @param fdbk            Feedback.
	* @return int
	*/
	int Compare(const GFdbk& fdbk) const;

	/**
	* Compare the document assessed with another document using their
	* identifiers.
	* @see R::RContainer
	* @param id              Identifier of the document.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* Associate a new feedback to the current document.
	* @param fdbk            Feedback type.
	* @param date            Date.
	*/
	void NewFdbk(tFdbkType fdbk,const R::RDate& date);

	/**
	* @return the feedback type over the document.
	*/
	tFdbkType GetFdbk(void) const {return(Fdbk);}

	/**
	* @return the identifier of the document.
	*/
	size_t GetDocId(void) const {return(DocId);}

	/**
	* @return the date of the feedback on the document.
	*/
	R::RDate GetDone(void) const;

	/**
	* @return the date of last computation of the document.
	*/
	R::RDate GetComputed(void) const;

	/**
	* Must the feedback be used to compute a profile.
	* @param profile         profile.
	*/
	bool MustUse(const GProfile* profile) const;

	/**
	* The document on which the feedback was made has been updated.
	*/
	void HasUpdate(void);

	/**
	* Create an "erroneous feedback" with a given percentage. The percentage
	* represents the number of feedbacks that will be changed in comparison to
	* the original.
	*
	* The changed feedback depends on the original feedback:
	* - If the original feedback is relevant, the changed feedback has a
	*   probability of 0.75 to be fuzzy relevant and a probability of 0.25 to be
	*   irrelevant.
	* - If the original feedback is fuzzy relevant, the changed feedback  has
	*   a probability of 0.5 to be relevant and a probability of 0.5 to be
	*   irrelevant.
	* - If the original feedback is irrelevant, the changed feedback has a
	*   probability of 0.75 to be fuzzy relevant and a probability of 0.25 to be
	*   relevant.
	* @param fdbk            Original feedback type.
	* @param PercErr         Percentage of error.
	* @param rand            Pointer to the random number generator to use.
	* @return a feedback feedback type.
	*/
	static tFdbkType ErrorFdbk(tFdbkType fdbk,double PercErr,R::RRandom& rand);

	/**
	* Destruct the feedback.
	*/
	~GFdbk(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
