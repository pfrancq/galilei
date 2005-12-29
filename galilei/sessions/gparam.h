/*

	GALILEI Research Project

	GParam.h

	Parameter - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

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
#ifndef GParamH
#define GParamH


//------------------------------------------------------------------------------
// include files for R Project
#include <rxmltag.h>
#include <rpromcriterion.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GParam class provides a representation for a parameter.
* @author Pascal Francq
* @short Parameter.
*/
class GParam
{
protected:

	/**
	* Name of the parameter.
	*/
	R::RString Name;

public:

	/**
	* Constructor of the parameter.
	* @param n               Name of the parameter.
	*/
	GParam(const char* n);

	/**
	* Compare the names of two parameters (work like the 'strcmp' function of
	* ANSI C/C++).
	* @see R::RContainer.
	* @param p               Paramater.
	* @return int
	*/
	int Compare(const GParam& p) const;

	/**
	* Compare the names of two parameters (work like the 'strcmp' function of
	* ANSI C/C++).
	* @see R::RContainer.
	* @param p               Pointer to a paramater.
	* @return int
	*/
	int Compare(const GParam* p) const;

	/**
	* Compare the name of a parameter with another one (work like the 'strcmp'
	* function of ANSI C/C++).
	* @see R::RContainer.
	* @param n               Name.
	* @return int
	*/
	int Compare(const char* n) const;

	/**
	* Compare the name of a parameter with another one (work like the 'strcmp'
	* function of ANSI C/C++).
	* @see R::RContainer.
	* @param n               Name.
	* @return int
	*/
	int Compare(const R::RString& n) const;

	/**
	* Read a configuration from a XML Tag. By default, it creates a node without
	* child and call ReadAttributes to extract attributes from this tag.
	* @param parent          Parent tag.
	*/
	virtual void ReadConfig(R::RXMLTag* parent);

	/**
	* Save a configuration to a XML Tag. By default, it supposes that there is
	* only a node without child and call WriteAttributes to add attributes to
	* this tag.
	* @param xml             XML Structure.
	* @param parent          Parent tag.
	*/
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* parent);

	/**
	* Read attributes from a tag. Need to be overloaded in child classes of
	* GParam.
	* @param tag             Tag from which to read.
	*/
	virtual void ReadAttributes(R::RXMLTag* tag);

	/**
	* Write attributes to a tag. Need to be overloaded in child classes of
	* GParam.
	* @param tag             Tag to which to write.
	*/
	virtual void WriteAttributes(R::RXMLTag* tag);

	/**
	* Get the content of the parameter.
	* @return int.
	*/
	virtual int GetInt(void);

	/**
	* Get the content of the parameter.
	* @return unsigned int.
	*/
	virtual unsigned int GetUInt(void);

	/**
	* Get the content of the parameter.
	* @return double.
	*/
	virtual double GetDouble(void);

	/**
	* Get the content of the parameter.
	* @return C string.
	*/
	virtual const char* GetString(void);

	/**
	* Get the content of the parameter.
	* @return Pointer to RPromCriterionParams.
	*/
	virtual const R::RPromCriterionParams* GetProm(void);

	/**
	* Get the content of the parameter.
	* @return bool.
	*/
	virtual bool GetBool(void);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(int v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(unsigned int v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(double v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(const char* v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(const R::RPromCriterionParams* v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(bool v);

	/**
	* Destructor.
	*/
	virtual ~GParam(void);
};


//------------------------------------------------------------------------------
/**
* The GParamInt class provides a representation for a "int" parameter.
* @author Pascal Francq
* @short Integer Parameter.
*/
class GParamInt : public GParam
{
	/**
	* Value fo the parameter.
	*/
	int Value;

public:

	/**
	* Constructor of the parameter
	* @param n               Name of the parameter.
	* @param d               Default value.
	*/
	GParamInt(const char* n,int d=0);

	/**
	* Read attributes from a tag.
	* @param tag             Tag from which to read.
	*/
	virtual void ReadAttributes(R::RXMLTag* tag);

	/**
	* Write attributes to a tag.
	* @param tag             Tag to which to write.
	*/
	virtual void WriteAttributes(R::RXMLTag* tag);

	/**
	* Get the content of the parameter.
	* @return int.
	*/
	virtual int GetInt(void);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(int v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(unsigned int v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(const char* v);

	/**
	* Destructor.
	*/
	virtual ~GParamInt(void);
};


//------------------------------------------------------------------------------
/**
* The GParamInt class provides a representation for a "unsigned int" parameter.
* @author Pascal Francq
* @short Integer Parameter.
*/
class GParamUInt : public GParam
{
	/**
	* Value fo the parameter.
	*/
	unsigned int Value;

public:

	/**
	* Constructor of the parameter
	* @param n               Name of the parameter.
	* @param d               Default value.
	*/
	GParamUInt(const char* n,unsigned int d=0);

	/**
	* Read attributes from a tag.
	* @param tag             Tag from which to read.
	*/
	virtual void ReadAttributes(R::RXMLTag* tag);

	/**
	* Write attributes to a tag.
	* @param tag             Tag to which to write.
	*/
	virtual void WriteAttributes(R::RXMLTag* tag);

	/**
	* Get the content of the parameter.
	* @return unsigned int.
	*/
	virtual unsigned int GetUInt(void);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(unsigned int v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(int v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(const char* v);

	/**
	* Destructor.
	*/
	virtual ~GParamUInt(void);
};


//------------------------------------------------------------------------------
/**
* The GParamDouble class provides a representation for a "double" parameter.
* @author Pascal Francq
* @short Double Parameter.
*/
class GParamDouble : public GParam
{
	/**
	* Value fo the parameter.
	*/
	double Value;

public:

	/**
	* Constructor of the parameter
	* @param n               Name of the parameter.
	* @param d               Default value.
	*/
	GParamDouble(const char* n,double d=0.0);

	/**
	* Read attributes from a tag.
	* @param tag             Tag from which to read.
	*/
	virtual void ReadAttributes(R::RXMLTag* tag);

	/**
	* Write attributes to a tag.
	* @param tag             Tag to which to write.
	*/
	virtual void WriteAttributes(R::RXMLTag* tag);

	/**
	* Get the content of the parameter.
	* @return double.
	*/
	virtual double GetDouble(void);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(double v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(const char* v);

	/**
	* Destructor.
	*/
	virtual ~GParamDouble(void);
};


//------------------------------------------------------------------------------
/**
* The GParamString class provides a representation for a "string" parameter.
* @author Pascal Francq
* @short String Parameter.
*/
class GParamString : public GParam
{
	/**
	* Value fo the parameter.
	*/
	R::RString Value;

public:

	/**
	* Constructor of the parameter
	* @param n               Name of the parameter.
	* @param d               Default value.
	*/
	GParamString(const char* n,const char* d=0);

	/**
	* Read attributes from a tag.
	* @param tag             Tag from which to read.
	*/
	virtual void ReadAttributes(R::RXMLTag* tag);

	/**
	* Write attributes to a tag.
	* @param tag             Tag to which to write.
	*/
	virtual void WriteAttributes(R::RXMLTag* tag);

	/**
	* Get the content of the parameter.
	* @return C string.
	*/
	virtual const char* GetString(void);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(const char* v);

	/**
	* Destructor.
	*/
	virtual ~GParamString(void);
};


//------------------------------------------------------------------------------
/**
* The GParamProm class provides a representation for a "PROMETHEE" parameter.
* @author Pascal Francq
* @short PROMETHEE Parameter.
*/
class GParamProm : public GParam
{
	/**
	* Value fo the parameter.
	*/
	R::RPromCriterionParams Value;

public:

	/**
	* Constructor of the parameter
	* @param n               Name of the parameter.
	* @param d               Default value.
	*/
	GParamProm(const char* n,R::RPromCriterionParams* d=0);

	/**
	* Read attributes from a tag.
	* @param tag             Tag from which to read.
	*/
	virtual void ReadAttributes(R::RXMLTag* tag);

	/**
	* Write attributes to a tag.
	* @param tag             Tag to which to write.
	*/
	virtual void WriteAttributes(R::RXMLTag* tag);

	/**
	* Get the content of the parameter.
	* @return Pointer to RPromCriterionParams.
	*/
	virtual const R::RPromCriterionParams* GetProm(void);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(const R::RPromCriterionParams* v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(const char* v);

	/**
	* Destructor.
	*/
	virtual ~GParamProm(void);
};


//------------------------------------------------------------------------------
/**
* The GParamBool class provides a representation for a "bool" parameter.
* @author Pascal Francq
* @short Bool Parameter.
*/
class GParamBool : public GParam
{
	/**
	* Value fo the parameter.
	*/
	bool Value;

public:

	/**
	* Constructor of the parameter
	* @param n               Name of the parameter.
	* @param d               Default value.
	*/
	GParamBool(const char* n,bool d=false);

	/**
	* Read attributes from a tag.
	* @param tag             Tag from which to read.
	*/
	virtual void ReadAttributes(R::RXMLTag* tag);

	/**
	* Write attributes to a tag.
	* @param tag             Tag to which to write.
	*/
	virtual void WriteAttributes(R::RXMLTag* tag);

	/**
	* Get the content of the parameter.
	* @return bool.
	*/
	virtual bool GetBool(void);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(bool v);

	/**
	* Set the content of the parameter.
	* @param v               Value of the parameter.
	*/
	virtual void Set(const char* v);

	/**
	* Destructor.
	*/
	virtual ~GParamBool(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
