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
#ifndef GProfilesBehaviourH
#define GProfilesBehaviourH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//
// class GBehaviour
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GBehaviour
{

	/**
	* identier of the second profile.
	*/
	unsigned int Id;

	/*
	* agreement ratio.
	*/
	double AgreementRatio;

	/*
	* desagreement ratio.
	*/
	double DisAgreementRatio;

	/*
	* state of the behaviour
	*/
	tObjState State;




public:

	/**
	* construtor
	*/
	GBehaviour(unsigned int id,double agree, double disagree,tObjState state = osUpToDate) : Id(id), AgreementRatio(agree),
DisAgreementRatio(disagree),State(state)  {}
	/**
	* comparison function
	*/
	int Compare(const GBehaviour* b) const {return(Id-b->Id);}
	int Compare(const GBehaviour& b) const {return(Id-b.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}

	/*
	* Set the value of the agreement ratio
	*/
	void SetAgreementRatio(double agree) {AgreementRatio=agree;}

	/*
	* Set the value of the disagreement ratio
	*/
	void SetDisAgreementRatio(double disagree) {DisAgreementRatio=disagree;}

	/*
	* Set the value of the agreement ratio
	*/
	double GetAgreementRatio(void) {return AgreementRatio;}

	/*
	* Set the value of the disagreement ratio
	*/
	double GetDisAgreementRatio(void) {return DisAgreementRatio;}

	/**
	* return the state of the behaviour.
	*/
	tObjState GetState(void) {return State;}

	/**
	* set the state of the behaviour.
	*/
	 void SetState(tObjState s) {State=s;}

	 /**
	 * compute the agreement and disagreement ratio.
	 */
	 void Compute(void);

	/**
	* destructor
	*/
	~GBehaviour(void){;}
};



//-----------------------------------------------------------------------------
//
// class GBehaviours
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GBehaviours : public R::RContainer<GBehaviour,unsigned int,true,false>
{
public:

	/**
	* identifier of the first profile.
	*/
	unsigned int Id;

	/**
	* constructor
	*/
	GBehaviours(unsigned int id,unsigned int max) throw(bad_alloc);

	/**
	* comparison functions
	*/
	int Compare(const GBehaviours* b) const {return(Id-b->Id);}
	int Compare(const GBehaviours& b) const {return(Id-b.Id);}
	int Compare(const unsigned int id) const {return(Id-id);}


	/**
	* destructor
	*/
	~GBehaviours(void){;}
};


//-----------------------------------------------------------------------------
/**
* The GProfilesSim class provides a representation for all the similarities
* between the profiles, i.e. the subprofiles of a given language.
* @author Pascal Francq
* @short Profiles Similarities.
*/
class GProfilesBehaviour
{
	/**
	* The similarities.
	*/
	R::RContainer<GBehaviours,unsigned int,true,false>* Behaviours;

	/**
	* Global similarities used?
	*/
	bool GlobalSim;

	/**
	*  lang of the profiles sim.
	*/
	GLang* Lang;

	/**
	* list of all modified profiles whom sim have to be updated.
	*/
	unsigned int* ModifiedProfs;

	/**
	* number of modified subprofiles.
	*/
	unsigned int NbModified;

	/**
	* minimum of documents to tale the same behaviour into account
	*/
	unsigned int MinSameDocs;

	/**
	* minimum of documents to tale the diff behaviour into account
	*/
	unsigned int MinDiffDocs;

public:

	/**
	* Constructor.
	* @param s              Subprofiles of the system.
	* @param global         Global approach.
	* @param lang           Lang of the profilesSim
	*/
	GProfilesBehaviour(R::RContainer<GSubProfile,unsigned int,false,true>* s,GLang* lang, unsigned int minsamedocs, unsigned int mindiffdocs) throw(bad_alloc);

	/**
	* Analyse the behaviourof the two subprofiles and insert it in behaviours if necessary.
	*/
	void AnalyseBehaviour(GBehaviours* behaviours,GSubProfile* sub1,GSubProfile* sub2);

	/**
	* Get the agreement between two profiles, i.e. the subprofiles of a same
	* language.
	* @param s1             Pointer to the first subprofile.
	* @param s2             Pointer to second subprofile.
	* @return double.
	*/
	double GetAgreementRatio(GSubProfile* s1,GSubProfile* s2);

	/**
	* Get the disagreement between two profiles, i.e. the subprofiles of a same
	* language.
	* @param s1             Pointer to the first subprofile.
	* @param s2             Pointer to second subprofile.
	* @param threshold       minimum of common documents
	* @return double.
	*/
	double GetDisAgreementRatio(GSubProfile* s1,GSubProfile* s2);

	/**
	* return the state of the behaviour between two profiles
	* @param id1            Identificator of the first subprofile
	* @param id2            Identificator of the second profile
	*/
	tObjState GetState(unsigned int id1, unsigned int id2);

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GLang* l) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GProfilesBehaviour& profilesBehaviour) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GProfilesBehaviour* profilesBehaviour) const;

	/**
	* Update the state of the profiles behaviours : If the subprofile has changed
	* the corresponding sim will be set to state="osModified".
	* If the similarity for a given subprofile doesn't exist, the element
	* is created but not computed ( -> state to osModified ).
	* @param users              The pointer to the list of users.
	* @param global             use the Global/Local similarity
	*/
	void UpdateProfBehaviour(void) throw(bad_alloc);

	/**
	* add a modified profile to the table.
	* @param id             id of the subprofile to add.
	*/
	void AddModifiedProfile(unsigned int id);

	/**
	* Destructor.
	*/
	~GProfilesBehaviour(void);
};



//-----------------------------------------------------------------------------
/**
* The GSubProfileCursor class provides a way to go trough a set of subprofiles.
* @short SubProfiles Cursor
*/
CLASSCURSOR(GProfilesBehaviourCursor,GProfilesSim,unsigned int)



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
