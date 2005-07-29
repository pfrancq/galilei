/*

	GALILEI Research Project

	GGroupsDocsSimsCosinus.h

	Similarities between documents and groups - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GGroupsDocsSimsCosinusH
#define GGroupsDocsSimsCosinusH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggroupsdocssims.h>
#include <gsignalhandler.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GGroupsDocsSimsCosinus class provides a representation for the similarities between
* groups and profiles.
* @author Pascal Francq and Valery Vandaele
* @short Groups-Documents Similarities.
*/
class GGroupsDocsSimsCosinus : public GGroupsDocsSims, public GSignalHandler
{
	// Internal class
	class GDocGrpSim;

	/**
	* Similarities.
	*/
 	R::RContainer<GDocGrpSim,true,true> Sims;

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
	GGroupsDocsSimsCosinus(GFactoryGroupsDocsSims* fac);

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

	/**
	* Get the similarity between a document and a subprofile.
	* @param doc             Pointer to the document.
	* @param sub             Pointer to the subprofile.
	*/
	virtual double GetSimilarity(const GDoc* doc,const GGroup* grp);

	virtual double GetMinSimilarity(const GLang* lang);

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
	* A specific group has changed.
	* @param grp             Group.
	* @param event           Event.
	*/
	virtual void Event(GGroup* grp, tEvent event);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	* Destructor of the similarities between documents and subprofiles.
	*/
	virtual ~GGroupsDocsSimsCosinus(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
