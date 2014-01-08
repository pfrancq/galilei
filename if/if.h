/*

	GALILEI Research Project

	TfIdf.cpp

	Inverse Frequencies - Header.

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
#include <gdoc.h>
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
class If : public RObject, public GMeasure
{
	/**
	 * Measures dirty ?
	 */
	bool Dirty;

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
	RVectorBool tmpTypes;

	/**
	 * Temporary vector (mostly used to remember the concepts already encountered).
	 */
	RVectorBool tmpConcepts;

	/**
	 * Type of objects for which the IF is computed.
	 */
	enum eType
	{
		tDoc=0,               /** Documents. */
		tTopic=1,             /** Topics.*/
		tClass=2,             /** Classes.*/
		tProfile=3,           /** Profiles.*/
		tCommunity=4          /** Communities.*/
	};

public:

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
	* Access to several information related to the evaluation of a given
	* feature. If necessary, the measures are updated.
	* @param measure         Type of the information to take :
	*                        - case '0': Idf.
	*                        - case '1': Mutual information.
	*
	* The class supposes that two more parameter is passed:
	* - Identifier of the feature (size_t).
	* - A pointer to a variable of type double that will contain the result.
	*/
	virtual void Measure(size_t measure,...);

	/**
	 * Get a given information.
	 * @param info           Information asked :
	 * 						 - cNoRef : Number of features weights.
	 *                       - Other : Name of the features weights.
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
//	void HandleAddTopic(const R::RNotification& notification);
//
//	/**
//	 * This method handles the notifications that imply that the tf/idf factors
//	 * must be updated by removing the references of the sender.
//	 * @param notification   Notification received.
//	 */
//	void HandleDelTopic(const R::RNotification& notification);
//
//	/**
//	 * This method handles the notifications that imply that the tf/idf factors
//	 * must be updated by adding the references of the sender.
//	 * @param notification   Notification received.
//	 */
//	void HandleAddClass(const R::RNotification& notification);
//
//	/**
//	 * This method handles the notifications that imply that the tf/idf factors
//	 * must be updated by removing the references of the sender.
//	 * @param notification   Notification received.
//	 */
//	void HandleDelClass(const R::RNotification& notification);
//
//	/**
//	 * This method handles the notifications that imply that the tf/idf factors
//	 * must be updated by adding the references of the sender.
//	 * @param notification   Notification received.
//	 */
//	void HandleAddProfile(const R::RNotification& notification);
//
//	/**
//	 * This method handles the notifications that imply that the tf/idf factors
//	 * must be updated by removing the references of the sender.
//	 * @param notification   Notification received.
//	 */
//	void HandleDelProfile(const R::RNotification& notification);
//
//	/**
//	 * This method handles the notifications that imply that the tf/idf factors
//	 * must be updated by adding the references of the sender.
//	 * @param notification   Notification received.
//	 */
//	void HandleAddCommunity(const R::RNotification& notification);
//
//	/**
//	 * This method handles the notifications that imply that the tf/idf factors
//	 * must be updated by removing the references of the sender.
//	 * @param notification   Notification received.
//	 */
//	void HandleDelCommunity(const R::RNotification& notification);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);
};


//------------------------------------------------------------------------------
#endif
