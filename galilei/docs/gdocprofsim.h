 
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
#ifndef GDocProfSimH
#define GDocProfSimH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GDocProfSim class provides a representation for all the similarities
* between the profiles and the documents, i.e. the subprofiles of a given language.
* @author Pascal Francq
* @short Document-Profiles Similarities.
*/
class GDocProfSim
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

public:

	/**
	* Constructor.
	* @param d              Documents of the system.
	* @param s              Subprofiles of the system.
	* @param global         Global approach.
	* @param lang           Lang of the profilesSim
	*/
	GDocProfSim(GDocs* d,RStd::RContainer<GSubProfile,unsigned int,false,true>* s,bool global,GLang* lang=0) throw(bad_alloc);

	/**
	* Constructor.
	* @param d              Documents of the system.
	* @param s              Subprofiles of the system.
	* @param global         Global approach.
	* @param lang           Lang of the profilesSim
	*/
	GDocProfSim(GDocs* d,RStd::RContainer<GSubProfile,unsigned int,false,true>& s,bool global,GLang* lang=0) throw(bad_alloc);


	/**
	* Constructor.
	* @param d              Documents of the system.
	* @param s              Cursor over the Subprofiles.
	* @param global         Global approach.
	*/
	GDocProfSim(GDocCursor* d, GSubProfileCursor* s,bool global, GLang* l) throw(bad_alloc);

	
		/**
	* Constructor.
	* @param d              Documents of the system.
	* @param s              Cursor over the Subprofiles.
	* @param global         Global approach.
	*/
	GDocProfSim(GDocs* d, GSubProfileCursor& s,bool global, GLang* l) throw(bad_alloc);

	
	/**
	* Analyse the similarity of the two subprofiles and insert when necessary.
	*/
	void AnalyseSim(GSims* sim,const GDoc* doc ,const GSubProfile* sub);

	/**
	* Get the similarities between two profiles, i.e. the subprofiles of a same
	* language.
	* @param i              Identificator of the document.
	* @param j              Identificator of the second subprofile.
	* @param users          Pointer to a list of users.
	* @return double.
	*/
	double GetSim(GDocs* docs,GUsers* users, unsigned int i,unsigned int j);

	/**
	* Get the similarities between two profiles, i.e. the subprofiles of a same
	* language.
	* @param doc             Pointer to the document.
	* @param sub             Pointer to second subprofile.
	* @return double.
	*/
	double GetSim(const GDoc* doc ,const GSubProfile* sub);

	/**
	* return the state of the similarity between two profiles
	* @param id1            Identificator of the document
	* @param id2            Identificator of the second profile
	*/
	tObjState GetState(unsigned int doc_id, unsigned int prof_id);

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GLang* l) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GDocProfSim& docProfSim) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GDocProfSim* docProfSim) const;

	/**
	* Determine whether the comparaison is global or local
	*/
	bool IsGlobalSim(void){return(GlobalSim);}

	/**
	* Update the state of the sims between documents and subprofiles. If a document or a If the subprofile has changed
	* the corresponding sim will be set to state="osModified".
	* If the similarity for a given subprofile doesn't exist, the element
	* is created but not computed ( -> state to osModified ).
	* @param docs               The pointer to the list of Documents.
	* @param users              The pointer to the list of users.
	* @param global             use the Global/Local similarity
	*/
	void UpdateDocProfSim(GDocs* docs, GUsers* users,bool global)throw(bad_alloc);

	/**
	* Update the state of the sims between documents and subprofiles. If a document or a subprofile has changed
	* the corresponding sim will be set to state="osModified".
	* If the similarity for a given subprofile doesn't exist, the element
	* is created but not computed ( -> state to osModified )
	* @param docs               The pointer to the list of Documents.
	* @param users              The reference to the list of users.
	* @param global             use the Global/Local similarity
	*/
	void UpdateDocProfSim(GDocs* docs, GUsers& users,bool global)throw(bad_alloc);

	/**
	* update the Analyse de similarity  of the two subprofiles and insert when necessary
	*/
	void AnalyseSim2(GSims* sim,const GSubProfile* sub1,const GSubProfile* sub2);

	/**
	* Destructor.
	*/
	~GDocProfSim(void);
};



//-----------------------------------------------------------------------------
/**
* The GSubProfileCursor class provides a way to go trough a set of subprofiles.
* @short SubProfiles Cursor
*/
CLASSCURSOR(GDocProfSimCursor,GDocProfSim,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
