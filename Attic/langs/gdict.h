/*

	GALILEI Research Project

	GDic.h

	Dictionnary - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GDictH
#define GDictH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rhashcontainer.h>
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangs/gword.h>
#include <glangs/glang.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GSession;


//-----------------------------------------------------------------------------
/**
* The GDict class provides a representation for a dictionnary.
* @author Pascal Francq
* @short Dictionnary.
*/
class GDict : public RStd::RHashContainer<GWord,unsigned,27,true>
{
	/**
	* Session responsible for the dictionnary.
	*/
	GSession* Session;

	/**
	* Direct access to the words using an identificator.
	*/
	GWord** Direct;

	/**
	* Maximal value of the identificator handled by Direct.
	*/
	unsigned MaxId;

	/**
	* Highest value of the identificator.
	*/
	unsigned UsedId;

	/**
	* Pointer to the language corresponding to the dictionnary.
	*/
	GLang *Lang;

	/**
	* Name of the dictionnary. This can be the name of a file or a table in a
	* database.
	*/
	RString Name;

	/**
	* Description of the dictionnary.
	*/
	RString Desc;

	/**
	* State of the dictionnary.
	*/
	bool Loaded;


public:

	/**
	* Constructor of the dictionnary.
	* @param session        Session.
	* @param name           Name of the dictionnary.
	* @param desc           Description of the dictionnary.
	* @param lang           Corresponding language.
	* @param max            Total maximal number of words to create at
	*                       initialisation.
	* @param maxletters     Maximal number of words to create at initialisation
	*                       for the different letters.
	*/
	GDict(GSession* session,const RStd::RString& name,const RStd::RString& desc,GLang* lang,unsigned max,unsigned maxletter) throw(bad_alloc);

	/**
	* Clear the dictionnary.
	*/
	void Clear(void);

private:

	/**
	* Put a word in direct.
	* @param word           Pointer to the word to insert.
	*/
	void PutDirect(GWord* word) throw(bad_alloc);

public:

	/**
	* Put a string corresponding to a word and an identificator in the
	* dictionnary.
	*/
	inline void Put(unsigned id,const RStd::RString& word) throw(bad_alloc)
	{
		GWord Word(id,word),*ptr;
		ptr=GetInsertPtr<GWord>(Word,true);
		PutDirect(ptr);
	}

	/**
	* Get the identificator of the word corresponding to a string. If the word
	* doesn't exist in the dictionnary, it is created.
	* @return Identificator of the word.
	*/
	unsigned int GetId(const RStd::RString& word) throw(bad_alloc);

	/**
	* Get the maximal number of word actually supposed.
	*/
	unsigned int GetMaxId(void) const {return(MaxId);}

	/**
	* Get thye name of the dictionnary.
	* @returns Pointer a C string.
	*/
	const char* GetName(void) const {return(Name());}

	/**
	* @return Word given by an identificator.
	*/
	const char* GetWord(const unsigned int id) const
		{return(Direct[id]->Word());}

	/**
	* Compare function used in the RStd::RContainer.
	*/
	int Compare(const GDict* dict) const;

	/**
	* Compare function used in the RStd::RContainer.
	*/
	int Compare(const GDict& dict) const;

	/**
	* Compare function used in the RStd::RContainer.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Destructor of the dictionnary.
	*/
	virtual ~GDict(void);
};



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
