/*

	GALILEI Research Project

	GParams.h

	Set of parameters - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef GParamsH
#define GParamsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gparam.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GParams class provides a representation for a series of parameters.
* @author Pascal Francq
* @short Set of Parameters.
*/
class GParams : public R::RContainer<GParam,unsigned int,true,true>
{
protected:

	/**
	* Name of the list of parameters.
	*/
	R::RString Name;

public:

	/**
	* Construct the list of parameters.
	* @param n               Name of the parameter.
	*/
	GParams(const R::RString& n) throw(std::bad_alloc);

	/**
	* Get the name of the set of parameters.
	*/
	R::RString GetName(void) const {return(Name);}

	/**
	* Read a configuration from a XML Tag.
	* @param tag             Tag.
	*/
	void ReadConfig(R::RXMLTag* tag);

	/**
	* Save a configuration to a XML Tag.
	* this tag.
	* @param xml             XML Structure.
	* @param tag             Tag.
	*/
	void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* tag);

	/**
	* Get the content of the parameter.
	* @param p               Name of the parameter.
	* @return int.
	*/
	int GetInt(const char* p);

	/**
	* Get the content of the parameter.
	* @param p               Name of the parameter.
	* @return unsigned int.
	*/
	unsigned int GetUInt(const char* p);

	/**
	* Get the content of the parameter.
	* @param p               Name of the parameter.
	* @return double.
	*/
	double GetDouble(const char* p);

	/**
	* Get the content of the parameter.
	* @param p               Name of the parameter.
	* @return C string.
	*/
	const char* GetString(const char* p);

	/**
	* Get the content of the parameter.
	* @param p               Name of the parameter.
	* @return Pointer to RPromCriterionParams.
	*/
	const R::RPromCriterionParams* GetProm(const char* p);

	/**
	* Get the content of the parameter.
	* @param p               Name of the parameter.
	* @return bool.
	*/
	bool GetBool(const char* p);

	/**
	* Set the content of the parameter.
	* @param p               Name of the parameter.
	* @param v               Value of the parameter.
	*/
	void Set(const char* p,int v);

	/**
	* Set the content of the parameter.
	* @param p               Name of the parameter.
	* @param v               Value of the parameter.
	*/
	void Set(const char* p,unsigned int v);

	/**
	* Set the content of the parameter.
	* @param p               Name of the parameter.
	* @param v               Value of the parameter.
	*/
	void Set(const char* p,double v);

	/**
	* Set the content of the parameter.
	* @param p               Name of the parameter.
	* @param v               Value of the parameter.
	*/
	void Set(const char* p,const char* v);

	/**
	* Set the content of the parameter.
	* @param p               Name of the parameter.
	* @param v               Value of the parameter.
	*/
	void Set(const char* p,const R::RPromCriterionParams* v);

	/**
	* Set the content of the parameter.
	* @param p               Name of the parameter.
	* @param v               Value of the parameter.
	*/
	void Set(const char* p,bool v);

	/**
	* Destruct the list of parameters.
	*/
	virtual ~GParams(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif

