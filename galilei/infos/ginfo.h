/*

	GALILEI Research Project

	GInfo.h

	Basic Information - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GInfoH
#define GInfoH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GInfo class provides a representation for a basic information entity.
* @author Pascal Francq.
* @short Basic Information.
*/
class GInfo
{
public:

	/**
	* The different type of information
	*/
	enum GInfoType {infoNothing,infoWord,infoWordOccur,infoWordOccurs,infoWordList,infoWordCalc,infoWordCalcs};

	/**
	* The constructor.
	*/
	GInfo(void);

	/**
	* Compare two information. This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	virtual int Compare(const GInfo* i) const;

	/**
	* Compute a similarity between two information objects. If the two objects
	* are the same, the similarity must be 1 and if there is nothing in common,
	* the similarity must be 0.
	* @return a value between 0 and 1.
	*/
	virtual float Similarity(const GInfo* i) const;

	/**
	* Compute a dissimilarity between two information objects. If the two
	* objects are the same, the dissimilarity must be 0 and if there is nothing
	* in common, the similarity must be 1.
	* @return a value between 0 and 1.
	*/
	virtual float DisSimilarity(const GInfo* i) const;

	/**
	* Return the name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* Return the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* Compare the class of two information objects.
	* @return true if the two objects are from the same class, false else.
	*/
	virtual bool SameClass(const GInfo* i) const;

	/**
	* The destructor.
	*/
	virtual ~GInfo(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
