/*

	GALILEI Research Project

	GIWord.h

	Word Information - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GIWordH
#define GIWordH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/ginfo.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GInfo class provides a representation for a basic information entity.
* @author Pascal Francq.
* @short Word Information.
*/
class GIWord : public GInfo
{
protected:

	/**
	* The identificator of a word.
	*/
	unsigned int Id;

public:

	/**
	* The constructor.
	*/
	GIWord(unsigned int id);

	/**
	* Compare two information. This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	virtual int Compare(const GInfo* i) const;

	/**
	* Compare two word's references.
	* @param i              Pointer to the word's reference ued for the
	*                       comparaison.
	*/
	int Compare(const GIWord* i) {return(Id-i->Id);}

	/**
	* Compare two word's references.
	* @param i              Word's reference ued for the comparaison.
	*/
	int Compare(const GIWord& i) {return(Id-i.Id);}

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
	* @returns Name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* @returns Type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* @returns Identificator of the word.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* The destructor.
	*/
	virtual ~GIWord(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
