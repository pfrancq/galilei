/*

	GALILEI Research Project

	GIWordOccur.h

	Word Occurences Information - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GIWordOccurH
#define GIWordOccurH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/giword.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GIWordOccur class provides a representation for the number of occurencies
* of a word in the document.
* @author Pascal Francq.
* @short Word's Occurences Information.
*/
class GIWordOccur : public GIWord
{
protected:

	/**
	* The number of occurences of the word.
	*/
	unsigned int Occurs;

public:

	/**
	* The constructor.
	* @param id             Identificator of the word.
	*/
	GIWordOccur(const unsigned int id);

	/**
	* The constructor.
	* @param id             Identificator of the word.
	* @param nb             Occurences of the word.
	*/
	GIWordOccur(const unsigned int id,const unsigned int nb);

	/**
	* Compare two information. This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	virtual int Compare(const GInfo* i) const;

	/**
	* Compare two word (occurences). This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	int Compare(const GIWordOccur* i) const
		{return(Id-i->Id);}

	/**
	* Compare two word (occurences). This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	int Compare(const GIWordOccur& i) const
		{return(Id-i.Id);}

	/**
	* Compare two word (occurences). This function is needed by the container.
	* @return 0 if i refers the same object, +1 else.
	* @see RContainer
	*/
	int Compare(const unsigned int id) const
		{return(Id-id);}

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
	* @returns the name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* @returns the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* @returns the number of occurences of the word.
	*/
	unsigned int GetNbOccurs(void) const {return(Occurs);}

	/**
	* Set the number of occurences of a word.
	* @param occurs         Number of occurences.
	*/
	void AddNbOccurs(unsigned int occurs) {Occurs+=occurs;}

	/**
	* Increase the number of occurences of the word.
	*/
	void IncOccurs(void) {Occurs++;}

	/**
	* The destructor.
	*/
	virtual ~GIWordOccur(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
