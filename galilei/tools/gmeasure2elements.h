/*

	GALILEI Research Project

	GMeasure2Elements.h

	Measures between two elements - Header.

	Copyright 2007 by the Université Libre de Bruxelles.

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
#ifndef GMeasure2ElementsH
#define GMeasure2ElementsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gsignalhandler.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GMeasure2Elements class provides a representation for a set of Similarity
* between Profiles of a given language.
* @author Pascal Francq.
* @short Measures Between Two Elements.
*/
class GMeasure2Elements : public GMeasure, public GSignalHandler
{
public:	

	class Intern;
	
private:

	/**
	 * Internal data.
	 */
	Intern* Data;
	
public:

	/**
	* Constructor of the measurees between two elements.
	* @param fac             Factory of the plug-in.
	* @param min             Must a minimum measure between similar elements be
	*                        computed.
	* @param perlang         Measure are language-dependent (similarities).
	* @param equals          Value of the measure when a given element is
	*                        compared with itself. 
	* @param objs            Type of the elements.
	*/
	GMeasure2Elements(GFactoryMeasure* fac,bool min,bool perlang,double equals,tObjType objs);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Get a measure between two elements. There are maximum four parameters.
	* @param measure         Type of the measure (0).
	* @param lang            Language of the elements (must be skipped if the
	*                        measure is language-independent).        
	* @param id1             Identifier of the first element.
	* @param id2             Identifier of the second element.
	*/	
	virtual void Measure(unsigned int measure,...);
	
	/**
	* Access to the minmum of the measure of two similar elements. There are
	* maximum two parameters.
	* @param measure         Type of the measure (0).
	* @param lang            Language of the elements (must be skipped if the
	*                        measure is language-independent).
	 */	
	virtual void Info(unsigned int info,...);

	/**
	 * Compute the measure for two elements eventually in a given language.
	 * @param lang            Language of the elements.        
	 * @param obj1            Pointer to the first element.
	 * @param obj2            Pointer to the second element.
	 * @code
	 * double MyMeasure::Compute(GLang* lang,void* obj1,void* obj2)
	 * {
	 *    return(static_cast<GSubProfile*>(obj1)->SimilarityIFF(*static_cast<GSubProfile*>(obj2),otSubProfile,lang));
	 * }
	 * @endcode
	 */
	virtual double Compute(GLang* lang,void* obj1,void* obj2)=0;
	
	/**
	* Get a pointer to a given element of null if the identifier does not
	* correspond to an element.
	* @param lang            Language of the element.        
	* @param id              Identifier of the element.
	* @code
	* void* MyMeasure::GetElement(GLang* lang,size_t id)
	* {
	*    return(Session->GetSubProfile(lang,id,false));
	* } 
	* @endcode
	*/
	virtual void* GetElement(GLang* lang,size_t id)=0;
	
	/**
	 * Return the maximum identifier of the elements in a given language.
	 * @param lang            Language of the elements.
	 * @code
	 * size_t MyMeasure::GetMaxElementsId(GLang* lang)
	 * {
	 *    return(Session->GetMaxSubProfileId(lang));
	 * }
	 * @endcode
	 */        
	virtual size_t GetMaxElementsId(GLang* lang)=0;
	
	/**
	* A specific language has changed.
	* @param lang            Language.
	* @param event           Event.
	*/
	virtual void Event(GLang* lang, tEvent event);

	/**
	* A specific document has changed.
	* @param doc             Document.
	* @param event           Event.
	*/
	virtual void Event(GDoc* doc, tEvent event);
	
	/**
	* A specific profile has changed.
	* @param prof            Profile.
	* @param event           Event.
	*/
	virtual void Event(GProfile* prof, tEvent event);
	
	/**
	* A specific subprofile has changed.
	* @param sub             Subprofile.
	* @param event           Event.
	*/
	virtual void Event(GSubProfile* sub, tEvent event);

	/**
	* A specific grou has changed.
	* @param grp             Group.
	* @param event           Event.
	*/
	virtual void Event(GGroup* grp, tEvent event);
	
	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GMeasure2Elements(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
