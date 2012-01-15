/*

	GALILEI Research Project

	Thesaurus.h

	Thesaurus Creation - Header.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef ThesaurusH
#define ThesaurusH


//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <robject.h>
#include <gtool.h>
#include <robjh.h>
#include <rnotification.h>
#include <gconcept.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <gnodeinfos.h>


//------------------------------------------------------------------------------
/**
 * The class represents a attribute, i.e. a combination of a vector concept and
 * a concept.
 */
class Attribute
{
public:
   size_t Id;
   GConcept* Vector;
   GConcept* Concept;
   double Weight;

   Attribute(void) : Id(cNoRef), Vector(0), Concept(0), Weight(0.0) {}
   Attribute(GConcept* vector,GConcept* concept) : Id(cNoRef), Vector(vector), Concept(concept) {}
   Attribute(const Attribute& attr) : Id(cNoRef), Vector(attr.Vector), Concept(attr.Concept) {}
   int Compare(const Attribute& attr) const
   {
      int i(CompareIds(Vector->GetId(),attr.Vector->GetId()));
      if(i)
         return(i);
      return(CompareIds(Concept->GetId(),attr.Concept->GetId()));
   }

   static int SortOrder(const void* a,const void* b)
   {
      double af=(*((Attribute**)(a)))->Weight;
      double bf=(*((Attribute**)(b)))->Weight;

      if(af==bf) return(0);
      if(af>bf)
         return(-1);
      else
         return(1);
   }
};


//------------------------------------------------------------------------------
/**
* The Thesaurus provides a plug-in to create a thesaurus
* @author Pascal Francq
* @short Thesaurus Creation.
*/
class Thesaurus : public RObject, public GTool
{
	/**
	* HGA Objects.
	*/
	RContainer<RObjH,true,false> Objs;

	/**
	 * The attributes.
	 */
	RContainer<Attribute,true,true> Attributes;

	/**
	 * The attributes ordered by identifiers.
	 */
	RContainer<Attribute,false,false> AttributesByIds;

	/**
	* Heuristic to used for the GA.
	*/
	RString Heuristic;

	/**
	* Maximum number of generation.
	*/
	unsigned int MaxGen;

	/**
	* Size of the Population.
	*/
	unsigned int PopSize;

	/**
	 * Verify the GA?
	 */
	bool Verify;

	/**
	 * Number of information entities used for the objects.
	 */
	size_t NumInfos;

	/**
	 * Temporary array of concepts used.
	 */
	RContainer<Attribute,true,false> Tmp;

public:

	/**
	* Constructor.
	* @param session         The session.
	* @param fac             Factory.
	*/
	Thesaurus(GSession* session,GPlugInFactory* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	 * Build a particular node
	 * @param node           Node.
	 * @param parent         Parent class.
	 */
	void BuildNode(GNodeInfos* node,GClass* parent);

	/**
	 * Construct the classes from a given solution.
	 * @param Sol            Solution.
	 */
	void ConstructResults(RCursor<GNodeInfos> Sol);

	/**
	 * Print an object.
	 * @param obj            Object.
	 */
	void PrintObj(RObjH* obj);

    /**
	* Run the thesaurus creation.
	*/
	virtual void Run(GSlot* slot);

	/**
	 * Handle a generation
	 */
	void Gen(const RNotification& notification);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Destruct the plug-in.
	*/
	virtual ~Thesaurus(void);
};


//-----------------------------------------------------------------------------
#endif

