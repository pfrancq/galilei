/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Feedback-based Profile Computing Method - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
#include <gweightinfos.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfileCalcFeedback class provides an implementation of the computing
* method "Feedback" inspired from the user relevance methods of the search
* engines. The vector of the profile is computed with a linear combinaison of
* the vectors of documents judged by the profile.
*
* The different parameters of the method:
* @param MaxNonZero         Maximal number of weights of the profile vector not
*                           to be zero. If this paramater is null, all the
*                           weights are left.
* @param IdfFactor          Specify if for the weights of documents the idf
*                           factor of the vector model theory must be computed.
* @param Relfactor          Factor corresponding to the relevant documents
*                           (OK+N).
* @param Fuzzyfactor        Factor corresponding to the fuzzy documents (KO).
* @param NoRelfactor        Factor corresponding to the no relevant documents
*                           (H).
* @param AddFuzzy           Specify if the fuzzy part must be added or
*                           substract.
* @author Pascal Francq
* @short "Feedbacks" Profile Computing Method.
*/
class GProfileCalcFeedback : public GProfileCalc
{
protected:

	/**
	 * Current profile computed.
	 */
	GProfile* Profile;

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
	* Global vector computed.
	*/
	GWeightInfos Vectors;

	/**
	* Global vector computed.
	*/
	GWeightInfos VectorsIrrel;

	/**
	* Global vector computed.
	*/
	GWeightInfos VectorsFuzzy;

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
	const GWeightInfo** Order;

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
	*/
	GProfileCalcFeedback(GFactoryProfileCalc* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session);

private:

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
	virtual void Compute(GProfile* profile);

	/**
	 * Write the content of the 'Order' vector in a file called 'subprofileX'
	 * where X is the identifier of the subprofile in a directory.
	 * @param dir            Directory where to write.
	 */
	void WriteFile(const RString& dir);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GProfileCalcFeedback(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
