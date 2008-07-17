/*

	GALILEI Research Project

	GMeasure.cpp

	Generic Measure - Implementation.

	Copyright 2005-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GMeasure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMeasure::GMeasure(GFactoryMeasure* fac)
	: GPlugin<GFactoryMeasure>(fac)
{
}


//------------------------------------------------------------------------------
void GMeasure::Info(size_t,...)
{
}


//------------------------------------------------------------------------------
GMeasure::~GMeasure(void)
{
}



//-----------------------------------------------------------------------------
//
// class GFactoryMeasure
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFactoryMeasure::GFactoryMeasure(GMeasureManager* m,const RString& t,const RString& n,const RString& f)
	: GFactoryPlugin<GFactoryMeasure,GMeasure,GMeasureManager>(m,n,f), Type(t)
{
	SetConfigInfos("lib/galilei/plugins/Measures/"+t,f.Mid(0,f.GetLen()-3));
}


//-----------------------------------------------------------------------------
R::RString GFactoryMeasure::GetType(void) const
{
	return(Type);
}



//------------------------------------------------------------------------------
//
// class GTypeMeasureManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GTypeMeasureManager::GTypeMeasureManager(const RString& type)
	: GPluginManager<GTypeMeasureManager,GFactoryMeasure,GMeasure>(type,API_MEASURES_VERSION,ptSelect)
{
}



//-----------------------------------------------------------------------------
//
// class GMeasureManager
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GMeasureManager::GMeasureManager(void)
	: GGenericPluginManager("Measures",API_MEASURES_VERSION,ptLists), R::RContainer<GTypeMeasureManager,true,true>(20,10)
{
}


//-----------------------------------------------------------------------------
void GMeasureManager::Load(const R::RString& dll,void* handle,void* handleDlg)
{
	typedef GFactoryMeasure* FactoryInit(GMeasureManager*,const char*);

	char* error;

	// Try to create the factory
	FactoryInit* initFac= (FactoryInit*)(dlsym(handle,"FactoryCreate"));
	error=dlerror();
	if(error)
	{
		cerr<<error<<endl;
		return;
	}
	GFactoryMeasure *myfactory= initFac(dynamic_cast<GMeasureManager*>(this),dll);

	// Verify the versions of the factory and the session
	if(Version.Compare(myfactory->GetAPIVersion()))
		return;

	// Register main plugin
	RegisterFactory(myfactory);

	// Try to create the dialogs if necessary
	if(!handleDlg)
		return;
	About_t about = (About_t)(dlsym(handleDlg,"About"));
	error=dlerror();
	if(!error)
		myfactory->SetAbout(about);
	void* config= dlsym(handleDlg,"Configure");
	error=dlerror();
	if(!error)
		myfactory->SetConfig(config);
}


//-----------------------------------------------------------------------------
void GMeasureManager::Connect(GSession* session)
{
	RCursor<GTypeMeasureManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Connect(session);
}


//-----------------------------------------------------------------------------
void GMeasureManager::Disconnect(GSession* session)
{
	RCursor<GTypeMeasureManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Disconnect(session);
}


//------------------------------------------------------------------------------
void GMeasureManager::CreateConfig(R::RConfig* config)
{
	config->InsertParam(new RParamStruct(Name),"Plugins");
}


//------------------------------------------------------------------------------
void GMeasureManager::ReadConfig(R::RConfig* config)
{
	RParamStruct* ptr=config->FindParam<RParamStruct>("Measures","Plugins");
	RCursor<GTypeMeasureManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RParamValue* param=ptr->Get<RParamValue>(Cur()->GetName());
		Cur()->SetCurrentMethod(param->Get(),false);
	}
}


//------------------------------------------------------------------------------
void GMeasureManager::SaveConfig(R::RConfig* config)
{
	RParamStruct* ptr=config->FindParam<RParamStruct>("Measures","Plugins");
	RCursor<GTypeMeasureManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RParamValue* param=ptr->Get<RParamValue>(Cur()->GetName());
		RString Default;
		if(Cur()->GetCurrentMethod(false))
			Default=Cur()->GetCurrentMethod(false)->GetFactory()->GetName();
		else
			Default="None";
		param->Set(Default);
	}
}


//-----------------------------------------------------------------------------
void GMeasureManager::RegisterFactory(GFactoryMeasure* fac)
{
	GTypeMeasureManager* Manager=GetInsertPtr(fac->GetType());
	Manager->RegisterFactory(fac);

	// Look if a param exist in the config structure
	RConfig* Config=dynamic_cast<GGALILEIApp*>(App)->GetGALILEIConfig();
	RParamStruct* param=Config->FindParam<RParamStruct>("Measures","Plugins");
	RParam* Exist=param->Get<RParam>(fac->GetType());
	if(!Exist)
		param->Insert(new RParamValue(fac->GetType(),"None"));
}


//-----------------------------------------------------------------------------
GFactoryMeasure* GMeasureManager::GetFactory(const R::RString& type,const R::RString& name,bool need) const
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if((!Manager)&&need)
		throw GException(RString("No type '")+type+"' availaible for "+Name+".");
	if(!Manager)
		return(0);
	return(Manager->GetFactory(name,need));
}


//-----------------------------------------------------------------------------
size_t GMeasureManager::GetNbFactories(const R::RString& type) const
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if(!Manager)
		return(0);
	return(Manager->GetNbFactories());
}


//-----------------------------------------------------------------------------
R::RCursor<GFactoryMeasure> GMeasureManager::GetFactories(const R::RString& type) const
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if(!Manager)
		return(R::RCursor<GFactoryMeasure>());
	return(Manager->GetFactories());
}


//-----------------------------------------------------------------------------
R::RCursor<GTypeMeasureManager> GMeasureManager::GetMeasureCategories(void) const
{
	return(R::RCursor<GTypeMeasureManager>(*this));
}


//-----------------------------------------------------------------------------
GTypeMeasureManager* GMeasureManager::GetMeasureCategory(const R::RString& type,bool need) const
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if((!Manager)&&need)
		throw GException(RString("No type '")+type+"' availaible for "+Name+".");
	return(Manager);
}



//-----------------------------------------------------------------------------
void GMeasureManager::EnablePlugIn(GMeasure* plug)
{
	GTypeMeasureManager* Manager=GetPtr(plug->GetFactory()->GetType());
	if(!Manager)
		throw GException(RString("No type '")+plug->GetFactory()->GetType()+"' availaible for "+Name+".");
	Manager->EnablePlugIn(plug);
}


//-----------------------------------------------------------------------------
void GMeasureManager::DisablePlugIn(GMeasure* plug)
{
	GTypeMeasureManager* Manager=GetPtr(plug->GetFactory()->GetType());
	if(!Manager)
		throw GException(RString("No type '")+plug->GetFactory()->GetType()+"' availaible for "+Name+".");
	Manager->DisablePlugIn(plug);
}


//-----------------------------------------------------------------------------
GMeasure* GMeasureManager::GetPlugIn(const R::RString& type,const R::RString& name,bool need) const
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if((!Manager)&&need)
		throw GException(RString("No type '")+type+"' availaible for "+Name+".");
	if(!Manager)
		return(0);
	return(Manager->GetPlugIn(name,need));
}


//-----------------------------------------------------------------------------
size_t GMeasureManager::GetNbPlugIns(const R::RString& type) const
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if(!Manager)
		return(0);
	return(Manager->GetNbPlugIns());
}


//-----------------------------------------------------------------------------
R::RCursor<GMeasure> GMeasureManager::GetPlugIns(const R::RString& type) const
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if(!Manager)
		return(R::RCursor<GMeasure>());
	return(Manager->GetPlugIns());
}


//------------------------------------------------------------------------------
void GMeasureManager::SetCurrentMethod(const R::RString& type,const R::RString& name,bool need)
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if((!Manager)&&need)
		throw GException(RString("No type '")+type+"' availaible for "+Name+".");
	if(Manager)
		Manager->SetCurrentMethod(name,need);
}


//------------------------------------------------------------------------------
GMeasure* GMeasureManager::GetCurrentMethod(const R::RString& type,bool need) const
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if((!Manager)&&need)
		throw GException(RString("No type '")+type+"' availaible for "+Name+".");
	if(!Manager)
		return(0);
	return(Manager->GetCurrentMethod(need));
}


//------------------------------------------------------------------------------
GFactoryMeasure* GMeasureManager::GetCurrentFactory(const R::RString& type,bool need) const
{
	GTypeMeasureManager* Manager=GetPtr(type);
	if((!Manager)&&need)
		throw GException(RString("No type '")+type+"' availaible for "+Name+".");
	if(!Manager)
		return(0);
	return(Manager->GetCurrentFactory(need));
}
