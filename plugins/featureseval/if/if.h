/*

	GALILEI Research Project

	If.cpp

	Tf/Idf Feature Weighting Method - Header.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef IfH
#define IfH


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
class If;


//------------------------------------------------------------------------------
/**
 * This is a structure to associated to a given description set. In practice,
 * it computes for each concept the number of descriptions of the set that
 * contain it and its IF factor.
 */
class IfData : public GDescriptionSetData
{
public:

   /**
    * The inverse frequency factors.
    */
   R::RVector IF;

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
	If* Calc;

	/**
	 * Initialize the class.
    * @param session        Session.
	 * @param calc           Computing method.
    */
	IfData(GSession* session,If* calc);

	/**
	 * Compute the if factors for a description set.
    * @param set            Set.
    */
	void Compute(GDescriptionSet* set);
};


//------------------------------------------------------------------------------
/**
 * This plug-in implements the Tf/Idf weighting method for all the concepts.
 * The formula to compute the weight, w_ij of a concept c_i for an object o_j is
 * given by:
 *
 * w_ij= (o_i,j/max_j(o_i,j)) * log(|O_i|/|O_i,i|)
 *
 * where o_i,j is the occurrences of the concept in the object description,
 * |O_i| the number of objects that contains at least one concept of the same
 * type as c_i in their descriptions, and |O_i,i| the number of those having
 * c_i in their description.
 *
 * This means that:
 * -# The weights are computed independently for the different object types
 *    (documents, profiles, etc.).
 * -# The weights are computed related to the concept types. The numerator of
 *    the expression of the logarithm refers to the number of objects having a
 *    given concept type in their description (rather than the total number of
 *    objects in theclassical formula).
 * @short Tf/Idf Weighting Method
 */
class If : public RObject, public GMeasure
{
public:

	/**
	 * Concept References.
	 */
	RMatrix ConceptRef;

	/**
	 * Concept Inverse Factors.
	 */
	RMatrix ConceptIf;

	/**
	 * Store the concept references.
	 */
	RMatrixStorage ConceptsFile;

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
	 * Temporary vector (mostly used to remember the concepts already encountered).
	 */
	RBoolVector tmpConcepts;

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
	If(GSession* session,GPlugInFactory* fac);

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
	 * Alter the IF factor for a given concept.
    * @param concept        Concept.
    * @param add            Concept is added or deleted.
	 * @param idx            Type of the vector.
	 * @param ref            Vector of concept references.
	 * @param typeref        Vector of concept type references.
	 * @param factors        Vector of IF factors.
    */
	inline void Alter(const GConcept* concept,bool add,eType idx,RVector* ref,RVector* typeref,RVector* factors);

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
	 * Virtual method inherits from R::RObject and that must be re-implemented
	 * in all child classes.
	 * @return Name of the class.
	 */
	virtual R::RCString GetClassName(void) const {return("TfIdf");}

	/**
	 * Get the IF factor for a given concept. If necessary, it is recomputed.
    * @param concept        Concept.
    * @param idx            Object type.
    * @return the IF factor.
    */
	inline double GetIf(const GConcept* concept,eType idx);

	/**
	 * Weight a description by the global tf*if factors.
    * @param desc           Description.
    * @param idx            Object type.
    */
	inline void ComputeTfIdf(GDescription* desc,eType idx);

	/**
	 * Weight a description by the local tf*if factors.
    * @param desc           Description.
	 * @param set            Description set.
    * @param idx            Object type.
    */
	inline void ComputeTfIdf(GDescription* desc,GDescriptionSet* set,eType idx);

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
	template<class cObj> void ComputeIF(const cObj* obj,eType type);

	/**
	 * Reset the IF factors.
    */
	virtual void Reset(void);
};


//------------------------------------------------------------------------------
#endif
