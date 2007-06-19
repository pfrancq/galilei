/*

	GALILEI Research Project

	GProfilesGeneric.h

	List of SubProfiles for a given Language - Implementation.

	Copyright 2007-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

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
#ifndef GProfilesGenericH
#define GProfilesGenericH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gsignalhandler.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
/**
* The GProfilesSimsCosinus class provides a representation for a set of Similarity between Profiles of
* a given language.
* @author Pascal Francq and Valery Vandaele
* @short Profiles Agreement.
*/
class GProfilesGeneric : public GMeasure, public GSignalHandler
{
protected:

	/**
	* level under which a similarity is cinsidered as null;
	*/
	double NullSimLevel;

	double MinAgreement;

	unsigned int MinDocs;

	bool Memory;

	double** Values;
	size_t NbLines;
	bool NeedUpdate;
	RVectorInt<true> Created;
	RVectorInt<true> Modified;
	RVectorInt<true> Deleted;

public:

	/**
	* Constructor of the similarities between subprofiles.
	*/
	GProfilesGeneric(GFactoryMeasure* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	virtual double Compute(GProfile* sub1,GProfile* sub2)=0;

	void Update(void);

	/**
	 * Measure(0,lang,id1,id2,result*);
	 */
	virtual void Measure(unsigned int measure,...);

	/**
	* A specific profile has changed.
	* @param sub             Subprofile.
	* @param event           Event.
	*/
	virtual void Event(GProfile* sub, tEvent event);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GProfilesGeneric(void);
};


//------------------------------------------------------------------------------
#endif
