/*

	GALILEI Research Project

	GSubProfiles.h

	List of SubProfiles for a given Language - Implementation.

	Copyright 2003-2005 by the Université Libre de Bruxelles.

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
#ifndef GProfilesSimsCosinusH
#define GProfilesSimsCosinusH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gsignalhandler.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GProfilesSimsCosinus class provides a representation for a set of Similarity between Profiles of
* a given language.
* @author Pascal Francq and Valery Vandaele
* @short SubProfiles.
*/
class GProfilesSimsCosinus : public GMeasure, public GSignalHandler
{
	// Internal class
	class GProfilesSim;

	/**
	* Similarities.
	*/
	R::RContainer<GProfilesSim,true,true> Values;

	/**
	* level under which a similarity is cinsidered as null;
	*/
	double NullSimLevel;

	double MinDisagreement;

	unsigned int MinDiffDocs;

public:

	/**
	* Constructor of the similarities between subprofiles.
	* @param session         Session.
	* @param iff             Use Inverse Frequency Factor.
	* @param memory      use container to stock sims?
	*/
	GProfilesSimsCosinus(GFactoryMeasure* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	virtual double GetMeasure(unsigned int id1,unsigned int id2,unsigned int measure);

	virtual double GetMinMeasure(const GLang* lang,unsigned int measure);

	virtual double GetMinMeasure(unsigned int);

	/**
	* A specific subprofile has changed.
	* @param sub             Subprofile.
	* @param event           Event.
	*/
	virtual void Event(GLang* lang, tEvent event);

	/**
	* A specific subprofile has changed.
	* @param sub             Subprofile.
	* @param event           Event.
	*/
	virtual void Event(GSubProfile* sub, tEvent event);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GProfilesSimsCosinus(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
