/*

	GALILEI Research Project

	GProfilesSim.h

	Similarities between Profiles - Header

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GProfilesSimH
#define GProfilesSimH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GProfilesSim class provides a representation for all the similarities
* between the profiles, i.e. the subprofiles of a given language.
* @author Pascal Francq
* @short Profiles Similarities.
*/
class GProfilesSim
{
	class GSims;

	/**
	* The similarities.
	*/
	RStd::RContainer<GSims,unsigned int,true,true> Sims;

	/**
	* Global similarities used?
	*/
	bool GlobalSim;

	/**
	*  lang of the profiles sim.
	*/
	GLang* Lang;
	
	/**
	* mean of similarites
	*/
	double MeanSim;
	
	/**
	* standart deviation of similarities
	*/
	double Deviation;

	/**
	*  old number of comparison in computation of Mean and Deviation.
	*/
	unsigned int OldNbComp;

public:

	/**
	* Constructor.
	* @param s              Subprofiles of the system.
	* @param global         Global approach.
	* @param lang           Lang of the profilesSim
	*/
	GProfilesSim(RStd::RContainer<GSubProfile,unsigned int,false,true>* s,bool global,GLang* lang=0) throw(bad_alloc);

	/**
	* Constructor.
	* @param s              Subprofiles of the system.
	* @param global         Global approach.
	* @param lang           Lang of the profilesSim
	*/
	GProfilesSim(RStd::RContainer<GSubProfile,unsigned int,false,true>& s,bool global,GLang* lang=0) throw(bad_alloc);

	/**
	* Constructor.
	* @param s              Cursor over the Subprofiles.
	* @param global         Global approach.
	*/
	GProfilesSim(GSubProfileCursor& s,bool global) throw(bad_alloc);

	/**
	* Analyse the similarity of the two subprofiles and insert when necessary.
	*/
	void AnalyseSim(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2);

	/**
	* Get the similarities between two profiles, i.e. the subprofiles of a same
	* language.
	* @param i              Identificator of the first subprofile.
	* @param j              Identificator of the second subprofile.
	* @param users          Pointer to a list of users.
	* @return double.
	*/
	double GetSim(GUsers* users, unsigned int i,unsigned int j);

	/**
	* Get the similarities between two profiles, i.e. the subprofiles of a same
	* language.
	* @param s1             Pointer to the first subprofile.
	* @param s2             Pointer to second subprofile.
	* @return double.
	*/
	double GetSim(const GSubProfile* s1,const GSubProfile* s2);

	/**
	* return the state of the similarity between two profiles
	* @param id1            Identificator of the first subprofile
	* @param id2            Identificator of the second profile
	*/
	tObjState GetState(unsigned int id1, unsigned int id2);

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GLang* l) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GProfilesSim& profilesSim) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GProfilesSim* profilesSim) const;

	/**
	* Determine whether the comparaison is global or local
	*/
	bool IsGlobalSim(void){return(GlobalSim);}

	/**
	* Update the state of the profiles sims : If the subprofile has changed
	* the corresponding sim will be set to state="osModified".
	* If the similarity for a given subprofile doesn't exist, the element
	* is created but not computed ( -> state to osModified ).
	* @param users              The pointer to the list of users.
	* @param global             use the Global/Local similarity
	* @param lang               the language used by the subProfiles
	*/
	void UpdateProfSim(GUsers* users,bool global,GLang* lang)throw(bad_alloc);

	/**
	* Update the state of the profiles sims : If the subprofile has changed
	* the corresponding sim will be set to state="osModified".
	* If the similarity for a given subprofile doesn't exist, the element
	* is created but not computed ( -> state to osModified )
	* @param users              The reference to the list of users.
	* @param global             use the Global/Local similarity
	* @param lang               the language used by the subProfiles
	*/
	void UpdateProfSim(GUsers& users ,bool global,GLang* lang)throw(bad_alloc);

	/**
	* update the Analyse de similarity  of the two subprofiles and insert when necessary
	*/
	void AnalyseSim2(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2);

	/**
	* update and get the deviation od similarities
	*/
	void UpdateDeviationAndMeanSim(RStd::RContainer<GSubProfile,unsigned int,false,true>* suprofile);

	/**
	* returns mean of similaritries
	* dont forget to 'UpdateDeviatonAdMeanSIm" if you want the updated mean.
	*/ 
	double GetMeanSim(void){return(MeanSim);}

	/**
	* returns mean of similaritries
	*/ 
	double GetDeviation(void){return(Deviation);}

	/**
	* Destructor.
	*/
	~GProfilesSim(void);
};



//-----------------------------------------------------------------------------
/**
* The GSubProfileCursor class provides a way to go trough a set of subprofiles.
* @short SubProfiles Cursor
*/
CLASSCURSOR(GProfilesSimCursor,GProfilesSim,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
