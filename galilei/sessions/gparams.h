/*

	GALILEI Research Project

	GParams.h

	Set of parameters - Header.

	Copyright 2003 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GParams_H
#define GParams_H

//-----------------------------------------------------------------------------
// include files for R Library
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gparam.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GParams class provides a representation for a series of parameters.
* @author Pascal Francq
* @short Set of Parameters.
*/
class GParams : public R::RContainer<GParam,unsigned int,true,true>
{
	/**
	* Name of the list of parameters.
	*/
	R::RString Name;

public:

	/**
	* Construct the list of parameters.
	* @param n               Name of the parameter.
	*/
	GParams(const char* n) throw(bad_alloc);

	/**
	* Read a configuration from a XML Tag.
	* @param parent          Parent tag.
	*/
	void ReadConfig(R::RXMLTag* parent);

	/**
	* Save a configuration to a XML Tag.
	* this tag.
	* @param parent          Parent tag.
	*/
	void SaveConfig(R::RXMLTag* parent);

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
	* @return double.
	*/
	const R::RPromCriterionParams* GetProm(const char* p);

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
	* Destruct the list of parameters.
	*/
	virtual ~GParams(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//------------------------------------------------------------------------------
#endif

