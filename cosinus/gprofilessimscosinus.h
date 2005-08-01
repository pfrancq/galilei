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
#include <gprofilessims.h>
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
class GProfilesSimsCosinus : public GProfilesSims, public GSignalHandler
{
	// Internal class
	class GProfilesSim;

	/**
	* Similarities.
	*/
	R::RContainer<GProfilesSim,true,true> Sims;

	bool IDF;

	/**
	* Inverse Frequency Factor must be used to compute the similarities.
	*/
	bool ISF;

	/*
	* Must the sims be stock in a container
	* or be recomputed each time
	*/
	bool Memory;

	/**
	* level under which a similarity is cinsidered as null;
	*/
	double NullSimLevel;

	unsigned int MinSameDocs;
	unsigned int MinDiffDocs;

	bool Debug;

	bool DebugMinSim;

	double MinSim;

	bool AutomaticMinSim;

	double MinAgreement;

	double MinDisagreement;

	bool NeedUpdate;

public:

	/**
	* Constructor of the similarities between subprofiles.
	* @param session         Session.
	* @param iff             Use Inverse Frequency Factor.
	* @param memory      use container to stock sims?
	*/
	GProfilesSimsCosinus(GFactoryProfilesSims* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	*  update the similairties
	*/
	void Update(void);

	double ComputeSim(const GSubProfile* sub1,const GSubProfile* sub2) const;

	/**
	* Get the similarity between two subprofiles.
	* @param sub1            Pointer to the first subprofile.
	* @param sub2            Pointer to the second subprofile.
	*/
	virtual double GetSimilarity(const GSubProfile* sub1,const GSubProfile* sub2);

	double ComputeMinSim(const GLang* lang);

	/**
	* Get the minimum of similarityof the subprofiles, needed by clusteirng
	* algorithms.
	* @param lang            Language.
	* @param deviationrate   factor of the standart deviation.
	*/
	virtual double GetMinSimilarity(const GLang* lang);

	double ComputeDisagree(const GSubProfile* sub1,const GSubProfile* sub2) const;

	/**
	* Return the disagreement ratio between two subprofiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	virtual double GetDisagreementRatio(const GSubProfile* sub1,const GSubProfile* sub2);

	double ComputeAgree(const GSubProfile* sub1,const GSubProfile* sub2) const;

	virtual double GetMinDisagreementRatio(const GLang* lang);

	/**
	* Return the agreement ratio between two subprofiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	virtual double GetAgreementRatio(const GSubProfile* sub1,const GSubProfile* sub2);

	virtual double GetMinAgreementRatio(const GLang* lang);

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
	static void CreateParams(GParams* params);

	/**
	* Destructor.
	*/
	virtual ~GProfilesSimsCosinus(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
