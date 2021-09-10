/*

	GALILEI Research Project

	LogEntropy.cpp

	Log Entropy Feature Weighting Method - Header.

	Copyright 2013-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef LogEntropyH
#define LogEntropyH


//-----------------------------------------------------------------------------
// include files for R Project
#include <robject.h>
#include <rcontainer.h>
#include <rmatrix.h>
#include <rmatrixstorage.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gdescriptionset.h>
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
// forward declaration
class LogEntropy;


//------------------------------------------------------------------------------
/**
 * This is a structure to associated to a given description set. In practice,
 * it computes for each concept the number of descriptions of the set that
 * contain it and its IF factor.
 */
class LogEntropyData : public GDescriptionSetData
{
public:

   /**
    * The entropy (and, temporally, the sum of the products).
    */
   R::RVector Entropy;

   /**
    * The sum of occurrences.
    */
   R::RVector Sum;

   /**
    * The number of references of the different type.
    */
   R::RVector Refs;

	/**
	 * Session.
	 */
	GSession* Session;

	/**
	 * Computing method;
	 */
	LogEntropy* Calc;

	/**
	 * Initialize the class.
    * @param session        Session.
	 * @param calc           Computing method.
    */
	LogEntropyData(GSession* session,LogEntropy* calc);

	/**
	 * Compute the if factors for a description set.
    * @param set            Set.
    */
	void Compute(GDescriptionSet* set);
};


//------------------------------------------------------------------------------
/**
 * This plug-in implements the log/entropy weighting methods for all the
 * concepts. It allows to compute the weight, w_ij of a concept c_i for an
 * object o_j.
 *
 * First, let us define s_i as the sum of the occurrences of a given concept in
 * the: s_i=sum_j (o_ij) documents
 *
 * The weight is then given by
 *
 * w_ij= log(1+o_i,j) * [1 + ( sum_j( (o_ij/s_i))*log(o_ij/s_i)) ) / (log(|O_i|+1)) )]
 *
 * It should be notice that :
 *
 * sum_j ( (o_ij/s_i) * log(o_ij/s_i) )=1/s_i*sum_j (o_ij*(log(o_ij)-log(s_i)))
 *                                     =1/s_i*sum_j(o_ij*log(o_ij))-log(s_i)
 *
 * where o_i,j is the occurrences of the concept in the object description, and
 * |O_i| the number of objects that contains at least one concept of the same
 * type as c_i in their descriptions.
 *
 * This means that:
 * -# The weights are computed independently for the different object types
 *    (documents, profiles, etc.).
 * -# The weights are computed related to the concept types. The expression of
 *    the logarithm at the denominator refers to the number of objects having a
 *    given concept type in their description (rather than the total number of
 *    objects in the classical formula).
 * @short Log/Entropy Weighting Method
 */
class LogEntropy : public RObject, public GMeasure
{
public:

	/**
	 * Sum for a concept of the product of its occurrences and their logarithm.
	 */
	RMatrix ConceptProd;

	/**
	 * Sum of the occurrences of a concept in all the objects.
	 */
	RMatrix ConceptSum;

	/**
	 * Concept Entropy.
	 */
	RMatrix ConceptEntropy;

	/**
	 * Store the concept sums of product.
	 */
	RMatrixStorage ConceptsProdFile;

	/**
	 * Store the concept sum of occurrences.
	 */
	RMatrixStorage ConceptsSumFile;

	/**
	 * Concept Type References.
	 */
	RMatrix ConceptTypeRef;

	/**
	 * Store the concept Type References.
	 */
	RMatrixStorage ConceptTypesFile;

	/**
	 * Temporary vector (mostly used to remember the types already encountered).
	 */
	RBoolVector tmpTypes;

	/**
	 * Type of objects for which the weighting is computed.
	 */
	enum eType
	{
		tDoc=0,               /** Documents. */
		tTopic=1,             /** Topics.*/
		tClass=2,             /** Classes.*/
		tProfile=3,           /** Profiles.*/
		tCommunity=4,         /** Communities.*/
		tUnknown              /** Unknown */
	};

	/**
	 * Constructor of the measure.
	 * @param session        Session.
	 * @param fac            Factory.
	 */
	LogEntropy(GSession* session,GPlugInFactory* fac);

	/**
	 * Virtual method inherits from R::RObject and that must be re-implemented
	 * in all child classes.
	 * @return Name of the class.
	 */
	virtual R::RCString GetClassName(void) const {return("LogEntropy");}

	/**
	 * Initialize the measure. In practice, the files storing the matrices are
	 * opened and the matrices are opened.
    */
	virtual void Init(void);

	/**
	 * Close the measure for the current session. In practice, the files storing
	 * the matrices are closed.
    */
	virtual void Done(void);

	/**
	 * Alter the frequencies for a given concept reference.
    * @param freq           Concept reference.
    * @param add            Concept is added or deleted.
	 * @param idx            Type of the vector.
	 * @param prod           Sum of the Product of the concept occurrences and
	 *                       their logarithm.
	 * @param sum            Concept Sum of their occurrences.
	 * @param typeref        Concept Type references.
	 * @param entropy        Concept entropies.
    */
	inline void Alter(const GConceptRef* ref,bool add,eType idx,RVector* prod,RVector* sum,RVector* typeref,RVector* entropy);

	/**
	 * Add the reference of a given description.
    * @param vectors         Vectors.
    * @param idx             Type of the vector.
    */
	void Add(R::RConstCursor<GVector> vectors,eType idx);

	/**
	 * Delete the reference of a given description.
    * @param vectors         Vectors.
    * @param idx             Type of the vector.
    */
	void Del(R::RConstCursor<GVector> vectors,eType idx);

	/**
	 * Get the entropy for a given concept. If necessary, it is recomputed.
    * @param concept        Concept.
    * @param idx            Object type.
    * @return the entropy.
    */
	inline double GetEntropy(const GConcept* concept,eType idx);

	/**
	 * Weight a description by the global entropies.
    * @param desc           Description.
    * @param idx            Object type.
    */
	inline void ComputeEntropy(GDescription* desc,eType idx);

	/**
	 * Weight a description by the local entropies.
    * @param desc           Description.
	 * @param set            Description set.
    * @param idx            Object type.
    */
	inline void ComputeEntropy(GDescription* desc,GDescriptionSet* set,eType idx);

	/**
	* Access to several information related to the evaluation of a given
	* feature. If necessary, some computations are updated.
	* @param measure
	* @parblock
	* Type of the information to take :
	* - case '0': if factor for a given concept and object type.
	* - case '1': Weight a description by the global tf*if factors.
	* - case '2': Weight a description by the local tf*if factors.
	* - case '4': Number of references for a given concept and object type.
	* @endparblock
	*
	* The measure supposes that more parameters are passed:
	* - A pointer to a GConcept (0), to a GDescription (1).
	* - An object type.
	* - A pointer to a variable of type double (0) or a pointer to a
	*   GDescriptionSet (2).
	*/
	virtual void Measure(size_t measure,...);

	/**
	 * Get a given information. In practice, for 0, it returns the name of the
	 * weighting method.
	 * @param info           Information asked.
	 */
	virtual void Info(size_t info,...);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by adding the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleAddDoc(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by removing the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleDelDoc(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by adding the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleAddTopic(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by removing the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleDelTopic(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by adding the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleAddClass(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by removing the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleDelClass(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by adding the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleAddProfile(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by removing the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleDelProfile(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by adding the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleAddCommunity(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by removing the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleDelCommunity(const R::RNotification& notification);

	/**
	 * Handle a notification that an object type must be reinitialized.
    * @param notification   Notification.
    */
	void HandleReInit(const R::RNotification& notification);

	/**
	 * ReInit the IF factors for a given type.
    * @param type           Type.
    */
	void ReInit(eType type);

	/**
	 * Compute the IF factors for a particular object type.
	 * @param obj            Pseudo-paramter.
	 * @param type           Type.
    */
	template<class cObj> void ComputeEntropy(const cObj* obj,eType type);

	/**
	 * Reset the IF factors.
    */
	virtual void Reset(void);
};


//------------------------------------------------------------------------------
#endif
