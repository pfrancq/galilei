/*

	GALILEI Research Project

	StatFeatures.h

	Features Statistics - Header.

	Copyright 2010 by Pascal Francq (pascal@francq.info).

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
#ifndef StatFeaturesH
#define StatFeaturesH


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <ruri.h>
#include <galilei.h>
#include <gtool.h>
#include <gconcept.h>
#include <rworksheet.h>
#include <gmeasure.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
/**
 * The ConcetData provides a representation for the data associated to a
 * concept.
 * @author Pascal Francq
 */
class ConceptData
{
	/**
	 * Related concept.
	 */
	GConcept* Concept;

	/**
	 * Average weight.
	 */
	double AvgWeight;

	/**
	 * Average Occurrence.
	 */
	double AvgOccurs;

	/**
	 * Maximum weight.
	 */
	double MaxWeight;

	/**
	 * Maximum Occurrence.
	 */
	double MaxOccurs;

public:

	/**
	 * Constructor.
	 * @param concept        Concept.
	 */
	ConceptData(GConcept* concept);

	/**
	 * Compare two concept data
	 * @param concept        Concept to compare with.
	 * @return a value usable by RContainer.
	 */
	int Compare(const GConcept& concept) const {return(Concept->Compare(concept));}

	/**
	 * Treat an concept reference.
	 * @param ref            Concept to treat.
	 */
	void Treat(GConceptRef* ref);

	/**
	 * Add the statistics related to the concept to a statistics matrix.
	 * @param stats          Matrix.
	 */
	void Add(RWorksheet& stats,GMeasure* measure);
};


//-----------------------------------------------------------------------------
/**
* The StatData class provides a method to compute different statistics.
* @author Pascal Francq
*/
class StatFeatures : public GTool
{
	/**
	* Name of the file.
	*/
	RURI Results;

	/**
	 * Current measure used.
	 */
	GMeasure* Measure;

	/**
	 * File containing the results.
	 */
	RTextFile File;

	/**
	 * The data.
	 */
	RContainer<ConceptData,true,true> Data;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	StatFeatures(GSession* session,GPlugInFactory* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Compute the statistics.
	*/
	virtual void Run(GSlot* slot);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);
};


//-----------------------------------------------------------------------------
#endif
