/*

	GALILEI Research Project

	GGroupEvaluate.h

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
#ifndef GGroupEvaluateH
#define GGroupEvaluateH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GEvaluateGrouping provides a way to use container of subprofile
* or container of document as the same class.
* @author Lamoral Julien
* @short common representation of subprofiles and document.
*/
class GGroupEvaluate
{
protected:

	/**
	* Language of the group.
	*/
	GLang* Lang;

public:

	/**
	* Constructor.
	*/
	GGroupEvaluate(GLang* lang);

	/**
	* The number of subporfile or doc int the groupment
	*/
	virtual unsigned int NbPtr(void)=0;

	/**
	* Initialise the container to the first element of this container.
	*/
	virtual void Start(void)=0;

	/**
	* Place the pointer to the next element of this container.
	*/
	virtual void Next(void)=0;

	/**
	* Verify that the pointer is not a the end of the container.
	*/
	virtual bool End(void)=0;

	/**
	* Return the id of the element which is pointed by the container pointer.
	*/
	virtual unsigned int Current()=0;

	/**
	* Return the similarity between 2 elements.
	*/
	virtual double Similarity(unsigned int id1,unsigned int id2)=0;

	/**
	* Return true if the subdoc whit id i is in this group.
	*/
	virtual bool IsIn(unsigned int i)=0;

	/**
	* return the number of Profile or document in the group
	*/
	virtual int NbProfDoc(void)=0;

	/**
	* Return the id of du subdoc which have the maximum similarity
	* whith the other element of his group.
	*/
	virtual double SumSimilarity(unsigned int id1)=0;

	/**
	* Return the id of the subdoc which have the most relevant similarity.
	*/
	virtual unsigned int RelevantSubDoc(void)=0;

	/**
	* Returns the similarity between a subprofile and his group.
	*/
	virtual double GroupSumSimilarity(unsigned int subdoc)=0;

   	/**
	* Return the id of the group
	*/
	virtual unsigned int GetId (void)=0;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroupEvaluate& ) const;

	/**
	* Compare method needed by RStd::RContainer.
	*/
	int Compare(const GGroupEvaluate* ) const;

	/**
	* Destructor.
	*/
	virtual ~GGroupEvaluate(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
