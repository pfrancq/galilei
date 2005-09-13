/*

	GALILEI Research Project

	GMeasure.h

	Generic Measure - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

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
#ifndef GMeasureH
#define GMeasureH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_MEASURES_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GMeasure class provides a representation for a specific measure (ex:
* Similarities between profiles, similarities between documents and groups,
* agreement ratios between profiles, ...).
* @author Pascal Francq
* @short Measures.
*/
class GMeasure : public GPlugin<GFactoryMeasure>
{
public:

	/**
	* Constructor of a measure.
	* @param fac             Plug-in factory.
	*/
	GMeasure(GFactoryMeasure* fac);

	/**
	* Get a measure. Two identificators can be passed when it concern a measure
	* between two objects (ex: Similarities between profiles). A given plug-in
	* can handle multiple measures, a parameter passed specifies the measure
	* asked.
	* @param id1             First identificator.
	* @param id2             Second identificator.
	* @param measure         Type of the measure.
	*/
	virtual double GetMeasure(unsigned int id1=0,unsigned int id2=0,unsigned int measure=0)=0;

	/**
	* Get the minimum of a given measure to be considered as having a sense.
	* This method supposes that each language has a specific value. A given
	* plug-in can handle multiple measures, a parameter passed specifies the
	* measure asked.
	* @param lang            Language.
	* @param measure         Type of the measure.
	*/
	virtual double GetMinMeasure(const GLang* lang,unsigned int measure=0)=0;

	/**
	* Get the minimum of a given measure to be considered as having a sense.
	* This method supposes that all languages have the same value. A given
	* plug-in can handle multiple measures, a parameter passed specifies the
	* measure asked.
	* @param measure         Type of the measure.
	*/
	virtual double GetMinMeasure(unsigned int measure=0)=0;

	/**
	* Destructor.
	*/
	virtual ~GMeasure(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryMeasure class provides a template for a generic measure factory.
* A factory handles the loading of the dynamic library containing the plugin.
* @author Pascal Francq
* @short Generic Measure Factory.
*/
class GFactoryMeasure : public GFactoryPlugin<GFactoryMeasure,GMeasure,GMeasureManager>
{

	/**
	* Type of plug-in.
	*/
	R::RString Type;

public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param t               Type of the measure.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryMeasure(GMeasureManager* mng,const char* t,const char* n,const char* f);

	/**
	* Get the type of the measure.
	*/
	R::RString GetType(void) const;
};


//------------------------------------------------------------------------------
/*
* The GTypeMeasureManager class provides a manager for all measures of a given
* type.
* @author Pascal Francq
* @short Type of Measures Manager.
*/
class GTypeMeasureManager : public GPluginManager<GTypeMeasureManager,GFactoryMeasure,GMeasure>
{
public:
	GTypeMeasureManager(const RString& type);

	/**
	* Get a measure. Two identificators can be passed when it concern a measure
	* between two objects (ex: Similarities between profiles). A given plug-in
	* can handle multiple measures, a parameter passed specifies the measure
	* asked.
	* @param id1             First identificator.
	* @param id2             Second identificator.
	* @param measure         Type of the measure.
	*/
	virtual double GetMeasure(unsigned int id1=0,unsigned int id2=0,unsigned int measure=0);

	/**
	* Get the minimum of a given measure to be considered as having a sense.
	* This method supposes that each language has a specific value. A given
	* plug-in can handle multiple measures, a parameter passed specifies the
	* measure asked.
	* @param lang            Language.
	* @param measure         Type of the measure.
	*/
	virtual double GetMinMeasure(const GLang* lang,unsigned int measure=0);

	/**
	* Get the minimum of a given measure to be considered as having a sense.
	* This method supposes that all languages have the same value. A given
	* plug-in can handle multiple measures, a parameter passed specifies the
	* measure asked.
	* @param measure         Type of the measure.
	*/
	virtual double GetMinMeasure(unsigned int measure=0);
};


//-----------------------------------------------------------------------------
/**
* The GMeasureManager class provides a measure manager.
* @author Pascal Francq
* @short Measure Manager.
*/
class GMeasureManager : public GGenericPluginManager, private R::RContainer<GTypeMeasureManager,true,true>
{
public:

	/**
	* Constructor for the manager of plug-ins manager.
	*/
	GMeasureManager(void);

	/**
	* Load a plug-in and its dialog boxes.
	* @param dll             Name of the dynamic link library.
	* @param handle          Handle to the library contaioning the plug-in.
	* @param handleDlg       Handle to the library contaioning the dialogs.
	*/
	virtual void Load(const R::RString& dll,void* handle,void* handleDlg);

	/**
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Read config of the manager.
	* @param t               Tag.
	*/
	virtual void ReadConfig(RXMLTag* t);

	/**
	* Store config of the manager.
	* @param xml             XML Structure.
	* @param t               Tag.
	*/
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t);

	/**
	* Register a factory of a plug-in.
	* @param fac             Factory to register.
	*/
	void RegisterFactory(GFactoryMeasure* fac);

	/**
	* Get a pointer to a given factory.
	* @param name            Name of the factory.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the factory, or null/exception if the name does not
	* exist.
	*/
	GFactoryMeasure* GetFactory(const R::RString& type,const R::RString& name,bool need=true) const;

	/**
	* Get the number of factories registered.
	*/
	size_t GetNbFactories(const R::RString& type) const;

	/**
	* Get a cursor over the registered factories.
	*/
	R::RCursor<GFactoryMeasure> GetFactories(const R::RString& type) const;

	/**
	* Get a cursor over the category.
	*/
	R::RCursor<GTypeMeasureManager> GetMeasureCategories(void) const;

	/**
	* Get a pointer to a given factory.
	* @param name            Name of the factory.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the factory, or null/exception if the name does not
	* exist.
	*/
	GTypeMeasureManager* GetMeasureCategory(const R::RString& type,bool need=true) const;

	/**
	* Signal that a plug-in was enabled.
	* @param plug            Plug-in enabled.
	*/
	void EnablePlugIn(GMeasure* plug);

	/**
	* Signal that a plug-in was disabled.
	* @param plug            Plug-in disabled.
	*/
	void DisablePlugIn(GMeasure* plug);

	/**
	* Get the number of enabled plug-ins.
	*/
	size_t GetNbPlugIns(const R::RString& type) const;

	/**
	* Get a pointer to a given plug-in.
	* @param name            Name of the plug-in.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the plug-in, or null/exception if the name does not
	* exist.
	*/
	GMeasure* GetPlugIn(const R::RString& type,const R::RString& name,bool need=true) const;

	/**
	* Get a cursor over the enabled plug-ins.
	*/
	R::RCursor<GMeasure> GetPlugIns(const R::RString& type) const;

	/**
	* Set the current method if the plug-ins must have a selected one.
	* @param name            Name of the method.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	*/
	virtual void SetCurrentMethod(const R::RString& type,const R::RString& name,bool need=true);

	/**
	* Get the current method.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the plugin, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	virtual GMeasure* GetCurrentMethod(const R::RString& type,bool need=true) const;

	/**
	* Get the current factory.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the factory, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	virtual GFactoryMeasure* GetCurrentFactory(const R::RString& type,bool need=true) const;
};


//-------------------------------------------------------------------------------
#define CREATE_MEASURE_FACTORY(type,name,plugin)                                       \
	class TheFactory : public GFactoryMeasure                                          \
{                                                                                      \
	static GFactoryMeasure* Inst;                                                      \
	TheFactory(GMeasureManager* mng,const char* l) : GFactoryMeasure(mng,type,name,l)  \
	{                                                                                  \
		plugin::CreateParams(this);                                                    \
	}                                                                                  \
	virtual ~TheFactory(void) {}                                                       \
public:                                                                                \
	static GFactoryMeasure* CreateInst(GMeasureManager* mng,const char* l)             \
	{                                                                                  \
		if(!Inst)                                                                      \
			Inst = new TheFactory(mng,l);                                              \
		return(Inst);                                                                  \
	}                                                                                  \
	virtual const char* GetAPIVersion(void) const {return(API_MEASURES_VERSION);}      \
		                                                                               \
	virtual GMeasure* NewPlugIn(void)                                                  \
	{                                                                                  \
		return(new plugin(this));                                                      \
	}                                                                                  \
	virtual void DeletePlugIn(GMeasure* plug)                                          \
	{                                                                                  \
		delete plug;                                                                   \
	}                                                                                  \
};                                                                                     \
GFactoryMeasure* TheFactory::Inst = 0;                                                 \
                                                                                       \
extern "C" GFactoryMeasure* FactoryCreate(GMeasureManager* mng,const char* l)          \
{                                                                                      \
	return(TheFactory::CreateInst(mng,l));                                             \
}                                                                                      \
extern "C" const char* LibType(void)                                                   \
{                                                                                      \
	return("Measures");                                                                \
}


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif