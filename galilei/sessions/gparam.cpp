/*

	GALILEI Research Project

	GParam.cpp

	Parameter - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <values.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gparam.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
//
// class GParam
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GParam::GParam(const char* n) throw(bad_alloc)
	: Name(n)
{
}


//------------------------------------------------------------------------------
int GParam::Compare(const GParam& p) const
{
	return(Name.Compare(p.Name));
}


//------------------------------------------------------------------------------
int GParam::Compare(const GParam* p) const
{
	return(Name.Compare(p->Name));
}


//------------------------------------------------------------------------------
int GParam::Compare(const char* n) const
{
	return(Name.Compare(n));
}


//------------------------------------------------------------------------------
int GParam::Compare(const RString& n) const
{
	return(Name.Compare(n));
}


//------------------------------------------------------------------------------
void GParam::ReadConfig(RXMLTag* parent)
{
	RXMLTag* tag;

	tag=parent->GetTag(Name);
	if(!tag) return;
	ReadAttributes(tag);
}


//------------------------------------------------------------------------------
void GParam::SaveConfig(RXMLTag* parent)
{
	RXMLTag* tag;

	tag=new RXMLTag(Name);
	WriteAttributes(tag);
	parent->AddTag(tag);
}


//------------------------------------------------------------------------------
void GParam::ReadAttributes(RXMLTag*)
{
}


//------------------------------------------------------------------------------
void GParam::WriteAttributes(RXMLTag*)
{
}


//------------------------------------------------------------------------------
int GParam::GetInt(void)
{
	return(0);
}


//------------------------------------------------------------------------------
unsigned int GParam::GetUInt(void)
{
	return(0);
}


//------------------------------------------------------------------------------
double GParam::GetDouble(void)
{
	return(0.0);
}


//------------------------------------------------------------------------------
const char* GParam::GetString(void)
{
	return(0);
}


//------------------------------------------------------------------------------
const RPromCriterionParams* GParam::GetProm(void)
{
	return(0);
}


//------------------------------------------------------------------------------
bool GParam::GetBool(void)
{
	return(false);
}


//------------------------------------------------------------------------------
void GParam::Set(int)
{
}


//------------------------------------------------------------------------------
void GParam::Set(unsigned int)
{
}


//------------------------------------------------------------------------------
void GParam::Set(double)
{
}


//------------------------------------------------------------------------------
void GParam::Set(const char*)
{
}


//------------------------------------------------------------------------------
void GParam::Set(const RPromCriterionParams*)
{
}


//------------------------------------------------------------------------------
void GParam::Set(bool)
{
}


//------------------------------------------------------------------------------
GParam::~GParam(void)
{
}



//------------------------------------------------------------------------------
//
// class GParamInt
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GParamInt::GParamInt(const char* n,int d) throw(bad_alloc)
	: GParam(n), Value(d)
{
}


//------------------------------------------------------------------------------
void GParamInt::ReadAttributes(RXMLTag* tag)
{
	const char* val;

	val=tag->GetAttrValue("Value");
	if(!val) return;
	sscanf(val,"%i",&Value);
}


//------------------------------------------------------------------------------
void GParamInt::WriteAttributes(RXMLTag* tag)
{
	char val[20];

	sprintf(val,"%i",Value);
	tag->InsertAttr("Value",val);
}


//------------------------------------------------------------------------------
int GParamInt::GetInt(void)
{
	return(Value);
}


//------------------------------------------------------------------------------
void GParamInt::Set(int v)
{
	Value=v;
}


//------------------------------------------------------------------------------
void GParamInt::Set(unsigned int v)
{
	if (v>MAXINT) return;
	Value=v;
}


//------------------------------------------------------------------------------
GParamInt::~GParamInt(void)
{
}



//------------------------------------------------------------------------------
//
// class GParamUInt
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GParamUInt::GParamUInt(const char* n,unsigned int d) throw(bad_alloc)
	: GParam(n), Value(d)
{
}


//------------------------------------------------------------------------------
void GParamUInt::ReadAttributes(RXMLTag* tag)
{
	const char* val;

	val=tag->GetAttrValue("Value");
	if(!val) return;
	sscanf(val,"%u",&Value);
}


//------------------------------------------------------------------------------
void GParamUInt::WriteAttributes(RXMLTag* tag)
{
	char val[20];

	sprintf(val,"%u",Value);
	tag->InsertAttr("Value",val);
}


//------------------------------------------------------------------------------
unsigned int GParamUInt::GetUInt(void)
{
	return(Value);
}


//------------------------------------------------------------------------------
void GParamUInt::Set(int v)
{
	if(v<0) return;
	Value=v;
}


//------------------------------------------------------------------------------
void GParamUInt::Set(unsigned int v)
{
	Value=v;
}


//------------------------------------------------------------------------------
GParamUInt::~GParamUInt(void)
{
}



//------------------------------------------------------------------------------
//
// class GParamDouble
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GParamDouble::GParamDouble(const char* n,double d) throw(bad_alloc)
	: GParam(n), Value(d)
{
}


//------------------------------------------------------------------------------
void GParamDouble::ReadAttributes(RXMLTag* tag)
{
	const char* val;

	val=tag->GetAttrValue("Value");
	if(!val) return;
	sscanf(val,"%lf",&Value);
}


//------------------------------------------------------------------------------
void GParamDouble::WriteAttributes(RXMLTag* tag)
{
	char val[25];

	sprintf(val,"%f",Value);
	tag->InsertAttr("Value",val);
}


//------------------------------------------------------------------------------
double GParamDouble::GetDouble(void)
{
	return(Value);
}


//------------------------------------------------------------------------------
void GParamDouble::Set(double v)
{
	Value=v;
}


//------------------------------------------------------------------------------
GParamDouble::~GParamDouble(void)
{
}



//------------------------------------------------------------------------------
//
// class GParamString
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GParamString::GParamString(const char* n,const char* d) throw(bad_alloc)
	: GParam(n), Value(d)
{
}


//------------------------------------------------------------------------------
void GParamString::ReadAttributes(RXMLTag* tag)
{
	Value=tag->GetAttrValue("Value");
}


//------------------------------------------------------------------------------
void GParamString::WriteAttributes(RXMLTag* tag)
{
	tag->InsertAttr("Value",Value);
}


//------------------------------------------------------------------------------
const char* GParamString::GetString(void)
{
	return(Value);
}


//------------------------------------------------------------------------------
void GParamString::Set(const char* v)
{
	Value=(*v);
}


//------------------------------------------------------------------------------
GParamString::~GParamString(void)
{
}



//------------------------------------------------------------------------------
//
// class GParamProm
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GParamProm::GParamProm(const char* n,RPromCriterionParams* d) throw(bad_alloc)
	: GParam(n), Value(d)
{
}


//------------------------------------------------------------------------------
void GParamProm::ReadAttributes(RXMLTag* tag)
{
	const char* val;

	val=tag->GetAttrValue("P");
	if(!val) return;
	sscanf(val,"%lf",&Value.P);
	val=tag->GetAttrValue("Q");
	if(!val) return;
	sscanf(val,"%lf",&Value.Q);
	val=tag->GetAttrValue("Weight");
	if(!val) return;
	sscanf(val,"%lf",&Value.Weight);
}


//------------------------------------------------------------------------------
void GParamProm::WriteAttributes(RXMLTag* tag)
{
	char val[25];

	sprintf(val,"%f",Value.P);
	tag->InsertAttr("P",val);
	sprintf(val,"%f",Value.Q);
	tag->InsertAttr("Q",val);
	sprintf(val,"%f",Value.Weight);
	tag->InsertAttr("Weight",val);
}


//------------------------------------------------------------------------------
const RPromCriterionParams* GParamProm::GetProm(void)
{
	return(&Value);
}


//------------------------------------------------------------------------------
void GParamProm::Set(const RPromCriterionParams* v)
{
	Value=(*v);
}

//------------------------------------------------------------------------------
GParamProm::~GParamProm(void)
{
}



//------------------------------------------------------------------------------
//
// class GParamBool
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GParamBool::GParamBool(const char* n,bool d) throw(bad_alloc)
	: GParam(n), Value(d)
{
}


//------------------------------------------------------------------------------
void GParamBool::ReadAttributes(RXMLTag* tag)
{
	const char* val;

	val=tag->GetAttrValue("Value");
	if(!val) return;
	if(strcmp(val,"True")) Value=false; else Value=true;
}


//------------------------------------------------------------------------------
void GParamBool::WriteAttributes(RXMLTag* tag)
{
	if(Value)
		tag->InsertAttr("Value","True");
	else
		tag->InsertAttr("Value","False");
}


//------------------------------------------------------------------------------
bool GParamBool::GetBool(void)
{
	return(Value);
}


//------------------------------------------------------------------------------
void GParamBool::Set(bool v)
{
	Value=v;
}


//------------------------------------------------------------------------------
GParamBool::~GParamBool(void)
{
}
