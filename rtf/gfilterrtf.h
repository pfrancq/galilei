/*

	GALILEI Research Project

	GFilterRTF.h

	A RTF filter - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Kumps Nicolas (nkumps@ulb.ac.be)

	Version $Revision$

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
#ifndef GFilterRTFH
#define GFilterRTFH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <docs/gfilter.h>
#include <codetochar.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilterRTF class provides a representation of a RTF document to filter in a
* RTF structure.
* @author Nicolas Kumps
* @short RTF's Filter.
*/
class GFilterRTF: public GFilter
{
	class Tag
	{
	public:
		enum tTag{tNULL,tSKIP,tSKIPTO,tEND,tTITLE,tAUTHOR,tOPERATOR,tSUBJECT,tPAR};
		R::RString Name;
		R::RString XMLName;
		tTag Type;
		bool Head;
		int Level;
		bool Ins;

		Tag(const char* n,const char* x,tTag t,bool h,int l,bool i)
			: Name(n), XMLName(x), Type(t),Head(h), Level(l),Ins(i) {}
		int Compare(const Tag* t) const {return(Name.Compare(t->Name));}
		int Compare(const Tag& t) const {return(Name.Compare(t.Name));}
		int Compare(const char* t) const {return(Name.Compare(t));}

	};

	/**
	* Header Tags.
	*/
	R::RContainer<Tag,unsigned int,true,true>* Tags;

	/**
	* Buffer containing all the document.
	*/
	char* Buffer;

	/**
	* Pointer to the current position in the buffer.
	*/
	char* Pos;

	/**
	* Pointer to the beginning of the block actually treated.
	*/
	char* Begin;

	/**
	* Pointer to parameters of the current tag.
	*/
	char* Params;

	/**
	* The container of XML code
	*/
	R::RContainer<CodeToChar,unsigned int,true,true> Chars;

public:

	/**
	* Construct the XML filter for a specific XML document.
	* @param fac            Factory.
	* @param name           Name of the filter.
	*/
	GFilterRTF(GFactoryFilter* fac);

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc) throw(std::bad_alloc,GException);

protected:

	/**
	*Function who init the container of code and correspondant char
	*/
	void InitCharContainer(void);

	/**
	* This function skip spaces.
	*/
	inline void SkipSpaces(void)
	{while((*Pos)&&isspace(*Pos)) Pos++;}

	/**
	* Function who extract the commands.
	*/
	bool ExtractCmd(Tag::tTag* tagtype);

	/**
	* Function use to skip an unuse tag.
	*/
	void SkipTag(void);

	/**
	* Function who delete the command.
	*/
	void DeleteCmd(void);

	/**
	* This function return true if replace code by the corresponding character.
	* Ex.: \u232\'00\'E8; by 'è'.
	*/
	bool ReplaceCode(void);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	*The destructor
	*/
	virtual ~GFilterRTF(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
