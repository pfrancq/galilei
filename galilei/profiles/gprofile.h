/*

	GALILEI Research Project

	GProfile.h

	Profile - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GProfileH
#define GProfileH


//-----------------------------------------------------------------------------
//include file for ANSI C/C++
#include <time.h>


//-----------------------------------------------------------------------------
//include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>
#include <rtimedate/rdate.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/gsubprofile.h>
#include <gprofiles/gprofdoc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class GUser;


//-----------------------------------------------------------------------------
/**
* The GProfile class provides a representation of a profile. In fact, it is a
* container of subprofiles, each subprofile corresponding to a language.
* @author Pascal Francq
* @short Profile.
*/
class GProfile : public RStd::RContainer<GSubProfile,unsigned,true,true>
{
	/**
	* The owner of the profile.
	*/
	GUser* Owner;

	/**
	* Identificator of the profile.
	*/
	unsigned Id;

	/**
	* Name of the profile.
	*/
	RStd::RString Name;

	/**
	* State of the profile.
	*/
	tObjState State;

	/**
	* Documents juged by profile.
	*/
	RContainer<GProfDoc,unsigned,false,true> Fdbks;

	/**
	* Date of Update.
	*/
	RTimeDate::RDate Updated;

	/**
	* Date of last document's analysis.
	*/
	RTimeDate::RDate Computed;

public:

    /**
	* Constructor og GProfile
	* @param owner          User of the profile.
	* @param id             Identificator of the profile.
	* @param name           Name of the profile.
	* @param u              String representing the date of updated.
	* @param a              String representing the date of the last analysis.
	* @param nb             Number of subprofiles.
	* @param nbf            Number of Feedbacks.
	*/
	GProfile(GUser *owner,const unsigned int id,const char* name,const char* u,const char* a,unsigned int nb,unsigned int nbf=100) throw(bad_alloc);

	/**
	* Comparaison function
	*/
	int Compare(const unsigned int &id) const;
	
	/**
	* Comparaison function
	*/
	int Compare(const GProfile &profile) const;
	
	/**
	* Comparaison function
	*/
	int Compare(const GProfile *profile) const;

	/**
	* Get the identificator of the profile.
	* @return Identificator.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

	/**
	* Get the name of the profile.
	* @return Pointer to a C String.
	*/
	const char* GetName(void) const {return(Name());}

	/**
	* Get the user of the profile.
	* @return Pointer to the user.
	*/
	const GUser* GetUser(void) const {return(Owner);}

	/**
	* Get the subprofile corresponding to a given language.
	* @param lang           Pointer to the language.
	* @return Pointer to the subprofile.
	*/
	const GSubProfile* GetSubProfile(const GLang* lang) const;

	/**
	* Get the date of the last update of the profile's.
	* @returns Pointer to date.
	*/
	const RTimeDate::RDate* GetUpdated(void) const {return(&Updated);}

	/**
	* Get the date of the last analysis of the profile.
	* @returns Pointer to date.
	*/
	const RTimeDate::RDate* GetComputed(void) const {return(&Computed);}

	/**
	* Return the state of the profile.
	* @returns GALILEI::tObjState value.
	*/
	tObjState GetState(void) const {return(State);}

	/**
	* Set the state of the profile.
	* @param state          New state.
	*/
	void SetState(tObjState state) {State=state;}

	/**
	* Tell the profile that its updated is finished.
	*/
	void UpdateFinished(void);

	/**
	* Start the iterator to go trough the documents judged.
	*/
	inline void DocsStart(void)
		{Fdbks.Start();}

	/**
	* Test if the end of the container of documents judged is reached.
	*/
	inline bool DocsEnd(void) const
		{return(Fdbks.End());}

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	inline void DocsNext(void)
		{Fdbks.Next();}

	/**
	* Get the current document judge.
	* @returns Pointer to the current document udge.
	*/
	GProfDoc* GetCurDocs(void) {return(Fdbks());}

	/**
	* Add a judgement for this profile.
	* @param j              Judgement.
	*/
	void AddJudgement(GProfDoc* j) throw(bad_alloc)
		{ Fdbks.InsertPtr(j); }
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
