/*

	GALILEI Research Project

	GProfilesDocsSimsCosinus.h

	Similarities between documents and subprofiles - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

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
#ifndef GProfilesDocsSimsCosinusH
#define GProfilesDocsSimsCosinusH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gsignalhandler.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GProfilesDocsSimsCosinus class provides a representation for the similarities between
* documents and profiles.
* @author Pascal Francq and Valery Vandaele
* @short SubProfiles-Documents Similarities.
*/
class GProfilesDocsSimsCosinus : public GMeasure, public GSignalHandler
{
	// Internal class
	class GDocProfSim;

	/**
	* Similarities.
	*/
 	R::RContainer<GDocProfSim,true,true> Sims;

	bool IDF;

	/**
	* Inverse Frequency Factor must be used to compute the similarities.
	*/
	bool ISF;

	bool Debug;

	bool DebugMinSim;


	/**
	* Must the sims be stock in a container
	* or be recomputed each time
	*/
	bool Memory;

	/**
	* level under which a similarity is cinsidered as null;
	*/
	double NullSimLevel;

	double MinSim;

	bool AutomaticMinSim;

	bool NeedUpdate;

public:

	/**
	* Constructor of the similarities between documents and subprofiles.
	* @param session         Session.
	* @param iff             Use Inverse Frequency Factor.
	* @param memory      use container to stock sims?
	*/
	GProfilesDocsSimsCosinus(GFactoryMeasure* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	*  update the similairties
	*/
	void Update(void);

	/**
	* returns the minimum level for a similarity not to be null.
	*/
	double GetNullSimLevel(void) {return NullSimLevel;}

	double ComputeSim(const GDoc* doc ,const GSubProfile* sub) const;

	virtual double GetMeasure(unsigned int id1,unsigned int id2,unsigned int measure);

	virtual double GetMinMeasure(const GLang* lang,unsigned int measure);

	virtual double GetMinMeasure(unsigned int) {return(0.0);}

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
	* Destructor of the similarities between documents and subprofiles.
	*/
	virtual ~GProfilesDocsSimsCosinus(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
