/*

	GALILEI Research Project

	GParams.cpp

	Set of parameters - Implementation.

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
// include files for GALILEI
#include <sessions/gparams.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
//
// class GParams
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GParams::GParams(const char* n) throw(bad_alloc)
	: RContainer<GParam,unsigned int,true,true>(10,5), Name(n)
{
}


//------------------------------------------------------------------------------
const char* GParams::GetName(void) const
{
	return(Name());
}


//------------------------------------------------------------------------------
void GParams::ReadConfig(RXMLTag* tag)
{
	GParamCursor Cur;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ReadConfig(tag);
}


//------------------------------------------------------------------------------
void GParams::SaveConfig(RXMLTag* tag)
{
	GParamCursor Cur;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->SaveConfig(tag);
}


//------------------------------------------------------------------------------
int GParams::GetInt(const char* p)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return(0);
	return(param->GetInt());
}


//------------------------------------------------------------------------------
unsigned int GParams::GetUInt(const char* p)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return(0);
	return(param->GetUInt());
}


//------------------------------------------------------------------------------
double GParams::GetDouble(const char* p)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return(0.0);
	return(param->GetDouble());
}


//------------------------------------------------------------------------------
const char* GParams::GetString(const char* p)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return(0);
	return(param->GetString());
}


//------------------------------------------------------------------------------
const RPromCriterionParams* GParams::GetProm(const char* p)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return(0);
	return(param->GetProm());
}


//------------------------------------------------------------------------------
bool GParams::GetBool(const char* p)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return(0);
	return(param->GetBool());
}


//------------------------------------------------------------------------------
void GParams::Set(const char* p,int v)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return;
	param->Set(v);
}


//------------------------------------------------------------------------------
void GParams::Set(const char* p,unsigned int v)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return;
	param->Set(v);
}


//------------------------------------------------------------------------------
void GParams::Set(const char* p,double v)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return;
	param->Set(v);
}


//------------------------------------------------------------------------------
void GParams::Set(const char* p,const char* v)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return;
	param->Set(v);
}


//------------------------------------------------------------------------------
void GParams::Set(const char* p,const RPromCriterionParams* v)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return;
	param->Set(v);
}


//------------------------------------------------------------------------------
void GParams::Set(const char* p,bool v)
{
	GParam* param;

	param=GetPtr<const char*>(p);
	if(!param) return;
	param->Set(v);
}


//------------------------------------------------------------------------------
R::RString GParams::GetName(void)
{
	return Name;
}


//------------------------------------------------------------------------------
GParams::~GParams(void)
{
}
