/*

	GALILEI Research Project

	GGroupsDocsSimsCosinus.h

	Similarities between documents and groups - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

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
#ifndef GGenericSimsH
#define GGenericSimsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gsignalhandler.h>
#include <ggalileiapp.h>
#include <glang.h>
#include <gsession.h>
#include <gdoc.h>
#include <ggroup.h>
#include <gsubprofile.h>
#include <gmeasure2elements.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
/**
* The GenericSims class provides a representation for the similarities between
* groups and profiles.
* @author Pascal Francq and Valery Vandaele
* @short Groups-Documents Similarities.
*/
class GGenericSims : public GMeasure, public GSignalHandler
{
	// Internal class
	class GElementSims;

	/**
	* Similarities.
	*/
 	R::RContainer<GElementSims,true,true> Sims;

	/**
	* Must the sims be stock in a container
	* or be recomputed each time
	*/
	bool Memory;

	/**
	* level under which a similarity is cinsidered as null;
	*/
	double NullSimLevel;

	bool Doc;
	
	bool Group;
	
	bool SubProfile;
	
public:

	/**
	* Constructor of the similarities between documents and subprofiles.
	*/
	GGenericSims(GFactoryMeasure* fac,bool d,bool g,bool s);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	virtual double Compute(GLang* lang,size_t id1,size_t id2)=0;
	
	/**
	* returns the minimum level for a similarity not to be null.
	*/
	double GetNullSimLevel(void) {return NullSimLevel;}

	virtual void Measure(unsigned int measure,...);

	virtual size_t GetMaxId1(GLang* lang)=0;

	/**
	* A specific subprofile has changed.
	* @param sub             Subprofile.
	* @param event           Event.
	*/
	virtual void Event(GLang* lang, tEvent event);

	/**
	* A specific document has changed.
	* @param doc             Subprofile.
	* @param event           Event.
	*/
	virtual void Event(GDoc* doc, tEvent event);

	/**
	* A specific group has changed.
	* @param grp             Group.
	* @param event           Event.
	*/
	virtual void Event(GGroup* grp, tEvent event);

	/**
	* A specific group has changed.
	* @param grp             Group.
	* @param event           Event.
	*/
	virtual void Event(GSubProfile* sub, tEvent event);
	
	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor of the similarities between documents and subprofiles.
	*/
	virtual ~GGenericSims(void);
};


//------------------------------------------------------------------------------
class GDiffSims : public GMeasure2Elements
{
protected:
	
	unsigned int SimType;
	
	double Factor;
	
	GWeightInfos* vec1;
	GWeightInfos* vec2;
	GConceptType* Lang;
	
public:
	GDiffSims(GFactoryMeasure* fact,bool min,tObjType objs);
	
	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);
	
	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors. A vector of a
	* list is build using this list and a Inverse Frequence Factor (IFF) of the
	* object type (idf, isf or ivf) for a given information entity space
	* (language). If one of the list is empty, the similarity is null.
	* @param l1             First list of weighted information entities.
	* @param l2             Second list of weighted information entities.
	* @param ObjType        Type of the object.
	*/
	double SimilarityIFFMV(void) const;

	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors. A vector of a
	* list is build using this list and a Inverse Frequence Factor (IFF) of the
	* object type (idf, isf or ivf) for a given information entity space
	* (language). If one of the list is empty, the similarity is null.
	* @param l1             First list of weighted information entities.
	* @param l2             Second list of weighted information entities.
	* @param ObjType        Type of the object.
	*/
	double SimilarityIFFL(void) const;
	
	/**
	 */
	double Compute(GLang* lang,void* obj1,void* obj2);
	static void CreateParams(RConfig* params);
};


//------------------------------------------------------------------------------
#endif
