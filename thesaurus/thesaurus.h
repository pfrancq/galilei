/*

	GALILEI Research Project

	Thesaurus.h

	Thesaurus Creation - Header.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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
#include <gposttopic.h>
#include <robjh.h>
#include <rnotification.h>
#include <gconcept.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <gnodeinfos.h>


//-----------------------------------------------------------------------------
/**
* The class Word represents a word and an id.
*/
class Word
{
public:
	size_t Id;
	GConcept* Concept;

	Word(GConcept* concept) : Id(cNoRef), Concept(concept) {}
	int Compare(const Word& concept) const {return(Concept->GetId()-concept.Concept->GetId());}
	int Compare(const GConcept* concept) const {return(Concept->GetId()-concept->GetId());}
};


//------------------------------------------------------------------------------
/**
* The Thesaurus provides a plug-in to create a thesaurus
* @author Pascal Francq
* @short Thesaurus Creation.
*/
class Thesaurus  : public RObject, public GPostTopic
{
	/**
	* HGA Objects.
	*/
	RContainer<RObjH,true,false> Objs;

	/**
	 * The words.
	 */
	RContainer<Word,true,true> Words;

	/**
	 * The words.
	 */
	RContainer<Word,false,false> WordsByIds;

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
	 * Array of concepts used.
	 */
	RContainer<GWeightInfo,true,false> Concepts;

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
	virtual void Run(void);

	/**
	 * Handle a generation
	 */
	void Gen(const RNotification& notification);

	/**
	* Create the parameters.
	*/
	static void CreateParams(GPlugInFactory* fac);

	/**
	* Destruct the plug-in.
	*/
	virtual ~Thesaurus(void);
};


//-----------------------------------------------------------------------------
#endif

