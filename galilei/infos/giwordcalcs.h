/*

	GALILEI Research Project

	GIWordcalcs.h

	Frequences of words appearing in a set a documents - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$
	
*/



//-----------------------------------------------------------------------------
#ifndef GIWordCalcsH
#define GIWordCalcsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ginfos/giwordcalc.h>
#include <gdocs/gdoc.h>
#include <glangs/glang.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GIWordCalcs provides a representation for the frequences of words that
* are appearing in a set of documents. This documents must all be of the same
* language.
* @author Pascal Francq
* @short Frequences of words appearing in a set a documents.
*/
class GIWordCalcs : public GInfo,public RStd::RContainer<GIWordCalc,unsigned,true,true>
{
	/**
	* Total number of words in the set of documents analysed.
	*/
	double NbWordsDocs;

	/**
	* Languages of the documents.
	*/
	GLang* Lang;

	/**
	* Hold the same list but in decreasing order of frequencies.
	*/
	GIWordCalc** Order;

	/**
	* Pointer to go trough the ordered list.
	*/
	GIWordCalc** CurOrder;

public:

	/**
	* Constructor.
	* @param lang           Language.
	* @param nb             Maximal number of word created at initialisation.
	*/
	GIWordCalcs(GLang *lang,unsigned int nb) throw(bad_alloc);

	/**
	* Return the name of the class.
	*/
	virtual const RStd::RString ClassName(void) const;

	/**
	* Return the type of the information.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GIWordCalcs& c) const
		{return(Lang->Compare(c.Lang));}

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GIWordCalcs* c) const
		{return(Lang->Compare(c->Lang));}

	/**
	* Compare method used by RStd::RContainer.
	*/
	int Compare(const GLang* l) const
		{return(Lang->Compare(l));}

private:

	/**
	* Static function used to ordered by frenquecy.
	*/
	static int sortOrder(const void *a,const void *b);

public:

	/**
	*/
	void Clear(void);
	void Analyse(GDoc *doc);
	void EndCalc(void);

	/**
	* Get the identificator of the next word in the ordered list by frequencies.
	* @returns Identificator of the word.
	*/
	unsigned int NextWord(void);

	/**
	* Destructor.
	*/
	virtual ~GIWordCalcs(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
