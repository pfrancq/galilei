/*

	GALILEI Research Project

	GGroupEvaluatesubprof.h

	Representation of groups of subprofiles and groups of documents.

	(C) 2002 by Julien Lamoral

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
#ifndef GGroupEvaluateSubprofH
#define GGroupEvaluateSubprofH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <tests/ggroupevaluate.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GEvaluateGroupingSubprof provides a way to use container of subprofile
* as the same class.
* @author Lamoral Julien
* @short common representation of subprofiles and document.
*/
class GGroupEvaluateSubprof : public GGroupEvaluate
{
protected:

	/**
	* Language of the group.
	*/
	GLang* Lang;

	/**
	* Container of subprofile in this group.
	*/
	RStd::RContainer<GSubProfile,unsigned int,false,true>* SubProfile;

	/**
	* Pointer to the Galilei session.
	*/
	GSession* Ses;

	/**
	* The id of the group.
	*/
	unsigned int Id;

	/**
	* Name of the group.
	*/
	RString Name;



public:

	/**
	* Constructor.
	*/
	GGroupEvaluateSubprof(unsigned int id,GLang* lang,RStd::RContainer<GSubProfile,unsigned int,false,true>* sub,GSession* ses);

	/**
	* The number of subdoc int the groupment.
	*/
	virtual unsigned int NbPtr(void);

	/**
	* Initialise the container to the first element of this container.
	*/
	virtual void Start(void);

	/**
	* Place the pointer to the next element of this container.
	*/
	virtual void Next(void);

	/**
	* Verify that the pointer is not a the end of the container.
	*/
	virtual bool End(void);

	/**
	* Return the id of the element which is pointed by the container pointer.
	*/
	virtual unsigned int Current();

	/**
	* Return the similarity between 2 elements.
	*/
	virtual double Similarity(unsigned int id1,unsigned int id2);

	/**
	* Return true if the subdoc whit id i is in this group.
	*/
	virtual bool IsIn(unsigned int i);

	/**
	* Return the number of Profile or document in the group
	*/
	virtual int NbProfDoc(void);

	/**
	* Return the id of du subdoc which have the maximum similarity
	* whith the other element of his group.
	*/
	virtual double SumSimilarity(unsigned int id1);

	/**
	* Return the id of the subdoc which have the most relevant similarity.
	*/
	virtual unsigned int RelevantSubDoc(void);

	/**
	* Returns the similarity between a subprofile and his group.
	*/
	virtual double GroupSumSimilarity(unsigned int subdoc);

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroupEvaluateSubprof&);

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroupEvaluateSubprof*);

	/**
	* Return the id of the group
	*/
	virtual unsigned int GetId (void) {return(Id);};
	
	/**
	* Return the name of the group;
	*/
	RString GetName(void){return(Name);};

	/**
	* Destructor.
	*/
	virtual ~GGroupEvaluateSubprof(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
