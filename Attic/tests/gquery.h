/*

	GALILEI Research Project

	GQuery.h

	Save the different value important to calc stats on documents or profiles  - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#ifndef GQueryH
#define GQueryH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* Find the group most relevant with a query .
* @author Lamoral Julien
* @short Returns most similar groups whith a query.
*/
class GQuery
{

protected:

	/**
	* The words of the query in wordweights.
	*/
	GIWordsWeights* QueryWord;

	/**
	* The current session.
	*/
	GSession* ses;

	/**
	* The current language.
	*/
	GLang* Lang;

	/**
	* the Minimal Similarities to  kepp te group.
	*/
	double MinSim;

	/**
	* Gets called to show the similarity whith groups.
	*/
	void CalcSimGroup(void);

	/**
	* Gets called to show the similarity whith groups.
	*/
	RContainer<GGroup,unsigned int,false,false>* Groups;

	/**
	* if true modify the query and add weights to all words.
	*/
	bool Modify;



public:

	/**
	* Constructor.
	* @param query          the query.
	* @param session        the Galilei Session.
	* @param lang           the Languege of the query.
	* @param MinSim         the Minimal Similarities to  kepp te group.
	* @param MinSim         Modify the query to find all groups?
	*/
	GQuery(GIWordsWeights* query,GSession* session,GLang* lang,double minsim,bool modify);

	/**
	* Place the results container to Start.
	*/
	void ResStart(void);

	/**
	* Place the results container to the next Element.
	*/
	void ResNext(void);

	/**
	* End  of results?
	* @return true if the container is a the end?
	*/
	bool ResEnd(void);

	/**
	* Get The current group.
	* @return The current group.
	*/
	GGroup* GetCurrentGroup(void);

	/**
	* Get The current sim.
	* @return the Similariti betwenn groups and the query.
	*/
	double GetCurrentSim(void);

	/**
	* Destructor for the main view.
	*/
	~GQuery(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
