/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Feedback-based Profile Computing Method - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GProfileCalcFeedbackH
#define GProfileCalcFeedbackH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gprofilecalc.h>
#include <gprofile.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfileCalcFeedback class provides an implementation of the computing
* method "Feedback" inspired from the user relevance methods of the search
* engines. The vector of the profile is computed with a linear combination of
* the vectors of documents judged by the profile.
*
* The different parameters of the method:
* @param MaxNonZero         Maximal number of weights of the profile vector not
*                           to be zero. If this parameter is null, all the
*                           weights are left.
* @param IdfFactor          Specify if for the weights of documents the idf
*                           factor of the vector model theory must be computed.
* @param Relfactor          Factor corresponding to the relevant documents
*                           (OK+N).
* @param Fuzzyfactor        Factor corresponding to the fuzzy documents (KO).
* @param NoRelfactor        Factor corresponding to the no relevant documents
*                           (H).
* @param AddFuzzy           Specify if the fuzzy part must be added or
*                           Subtract.
* @author Pascal Francq
* @short "Feedbacks" Profile Computing Method.
*/
class GProfileCalcFeedback : public GProfileCalc
{
protected:

	/**
	 * Current profile computed.
	 */
	const GProfile* Profile;

	/**
	* Maximal number of the non-zero weights in the vector.
	*/
	size_t MaxNonZero;

	/**
	* Number of the non-zero weights for irrelevant entities in the vector.
	*/
	size_t NegNonZero;

	/**
	* Factor of the vectors corresponding to the relevant part.
	*/
	double RelFactor;

	/**
	* Factor of the vectors corresponding to the fuzzy relevant part.
	*/
	double FuzzyFactor;

	/**
	* Factor of the vectors corresponding to the irrelevant part.
	*/
	double IrrelFactor;

	/**
	* Relevant vectors gathered.
	*/
	R::RContainer<GVector,true,true> VectorsRel;

	/**
	* Irrelevant vectors gathered.
	*/
	R::RContainer<GVector,true,true> VectorsIrrel;

	/**
	* Fuzzy relevant vectors gathered.
	*/
	R::RContainer<GVector,true,true> VectorsFuzzy;

   /**
	 * Internal structure.
	 */
	R::RContainer<GVector,true,true> Internal;

	/**
	* Number of documents assessed by a profile used for the computing.
	*/
	size_t NbDocs;

	/**
	* Number of documents assessed as relevant by a profile used for the computing.
	*/
	size_t NbDocsRel;

	/**
	* Number of documents assessed as fuzzy relevant by a profile used for the computing.
	*/
	size_t NbDocsFuzzy;

	/**
	* Number of documents assessed as irrelevant by a profile used for the computing.
	*/
	size_t NbDocsIrrel;

	/**
	* Ordered vector for current computed profile.
	*/
	const GConceptRef** Order;

	/**
	* Maximal size allocate for a profile.
	*/
	size_t MaxOrderSize;

	/**
	* Incremental mode for the computing.
	*/
	bool IncrementalMode;

public:

	/**
	* Constructor.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GProfileCalcFeedback(GSession* session,GPlugInFactory* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

private:

   /**
    *  Add a series of vector to Internal.
    * @param vectors        Vectors to add.
    * @param factor         Factor.
    * @param nbdocs         Number of documents used to compute vectors.
    */
   void AddVectors(R::RContainer<GVector,true,true>& vectors,double factor,double nbdocs);

	/**
	* Compute the global vectors.
	*/
	void ComputeGlobal(void);

	/**
	* Computes the profile.
	*/
	void ComputeProfile(void);

public:

	/**
	* Compute a profile.
	* @param profile        Profile to compute.
	*/
	virtual void Compute(const GProfile* profile);

	/**
	 * Write the content of the 'Order' vector in a file called 'profileX-Y'
	 * where X is the identifier of the profile and Y the identifier of the
    * vector.
	 * @param dir            Directory where to write.
    * @param nb             Number of elements of order to write.
    * @param concept        Concept associated to a vector.
	 */
	void WriteFile(const RString& dir,size_t nb,GConcept* concept);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Destructor.
	*/
	virtual ~GProfileCalcFeedback(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
