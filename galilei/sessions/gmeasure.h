/*

	GALILEI Research Project

	GMeasure.h

	Generic Measure - Implementation.

	Copyright 2005-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2005-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++
#include <stdarg.h>


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
	* The measure must be re-initialized, i.e. all values must be considered
	* as dirty.
	*/
	virtual void ReInit(void);

	/**
	* Get a measure. Each plug-in can handle several types of measure (first
	* parameter of the method). The other parameters (including eventually the
	* result of the measure) depend of the particular plug-in. This means that
	* the caller must exactly know which parameters are requested by a
	* particular plug-in. This method is intended to be called in an intensive
	* way (in contrary of Info)
	* @param measure         Type of the measure.
	*
	* Suppose that the method takes two int arguments and should return a
	* double that represents the product (measure=1) or the sum (measure=2). This
	* method should be written:
	* @code
	* void Measure(int measure,...)
	* {
	* 	va_list ap;
	* 	va_start(ap,measure);
	* 	int i=va_arg(ap,int);
	* 	int j=va_arg(ap,int);
	* 	double* d=va_arg(ap,double*);
	* 	switch(measure)
	* 	{
	* 		case 1 : (*d)=i*j; break;
	* 		case 2 : (*d)=i+j; break;
	* 	}
	* 	va_end(ap);
	* }
	* @endcode
	* The method should be call:
	* @code
	* GMeasure* ptr;
	* double d;
	* ...
	* ptr->Measure(1,3,4,&d);
	* cout<<d<<endl;
	* @endcode
	*/
	virtual void Measure(size_t measure,...)=0;

	/**
	* Exchange a particular information with the plug-in. The other parameters
	* (including eventually the result of the measure) depend of the particular
	* plug-in. This means that the caller must exactly know which parameters
	* are requested by a particular plug-in. This method is not intended to be
	* called in an intensive way (in contrary of Measure).
	* @param info           Information to get.
	*
	* Suppose the function should simply return a double. This method should
	* be written:
	* @code
	* void Info(int info,...)
	* {
	* 	va_list ap;
	* 	double* d=va_arg(ap,double*);
	* 	switch(info)
	* 	{
	* 		case 1 : (*d)=2; break;
	* 		case 2 : (*d)=3; break;
	* 	}
	* 	va_end(ap);
	* }
	* @endcode
	* The method should be call:
	* @code
	* GMeasure* ptr;
	* double d;
	* ...
	* ptr->Info(1,&d);
	* cout<<d<<endl;
	* @endcode
	 */
	virtual void Info(size_t info,...);

	/**
	* Destruct the measure.
	*/
	virtual ~GMeasure(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryMeasure class provides a template for a generic measure factory.
* A factory handles the loading of the dynamic library containing the plug-in.
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
	* @param mng             Manager of the plug-in.
	* @param t               Type of the measure  ('/' are replaced by '-' for
	*                        the configuration file).
	* @param n               Name of the Factory/Plug-in ('/' are replaced by
	*                        '-' for the configuration file).
	* @param f               Library of the Factory/Plug-in.
	*/
	GFactoryMeasure(GMeasureManager* mng,const R::RString& t,const R::RString& n,const R::RString& f);

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
	GTypeMeasureManager(const R::RString& type);

	/**
	 * Get a pointer to the configuration structure for a specific plug-in.
	 * @param name           Name of the plug-in.
	 * @param type           Type of the factory
	 * @return Pointer to R::RConfig.
	 */
	virtual R::RConfig* GetConfig(const R::RString& name,const R::RString&) const {return(GetFactory(name,false));}

	/**
	 * Get a pointer to the configuration structure for a specific plug-in.
	 * @param name           Name of the plug-in.
	 * @return Pointer to R::RConfig.
	 */
	virtual R::RConfig* GetConfig(const R::RString& name) const {return(GetFactory(name));}

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
	* @param handle          Handle to the library containing the plug-in.
	* @param handleDlg       Handle to the library containing the dialogs.
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
	* Create the configuration parameters.
	* @param config          Configuration structure.
	*/
	virtual void CreateConfig(R::RConfig* config);

	/**
	* Read configuration parameters of the manager.
	* @param config          Configuration structure.
	*/
	virtual void ReadConfig(R::RConfig* config);

	/**
	* @param config          Configuration structure.
	*/
	virtual void SaveConfig(R::RConfig* config);

	/**
	* Register a factory of a plug-in.
	* @param fac             Factory to register.
	*/
	void RegisterFactory(GFactoryMeasure* fac);

	/**
	* Get a pointer to a given factory.
	* @param type            Type of the factory.
	* @param name            Name of the factory.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the factory, or null/exception if the name does not
	* exist.
	*/
	GFactoryMeasure* GetFactory(const R::RString& type,const R::RString& name,bool need=true) const;

	/**
	* Get the number of factories registered.
	* @param type            Type of the factory.
	*/
	size_t GetNbFactories(const R::RString& type) const;

	/**
	* Get a cursor over the registered factories.
	* @param type            Type of the factory.
	*/
	R::RCursor<GFactoryMeasure> GetFactories(const R::RString& type) const;

	/**
	* Get a cursor over the category.
	*/
	R::RCursor<GTypeMeasureManager> GetMeasureCategories(void) const;

	/**
	* Get a pointer to a given factory.
	* @param type            Type of the factory.
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
	* @param type            Type of the factory.
	*/
	size_t GetNbPlugIns(const R::RString& type) const;

	/**
	* Get a pointer to a given plug-in.
	* @param type            Type of the factory.
	* @param name            Name of the plug-in.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the plug-in, or null/exception if the name does not
	* exist.
	*/
	GMeasure* GetPlugIn(const R::RString& type,const R::RString& name,bool need=true) const;

	/**
	* Get a cursor over the enabled plug-ins.
	* @param type            Type of the factory.
	*/
	R::RCursor<GMeasure> GetPlugIns(const R::RString& type) const;

	/**
	 * Get a pointer to the configuration structure for a specific plug-in.
	 * @param name           Name of the plug-in.
	 * @param type           Type of the factory
	 * @return Pointer to R::RConfig.
	 */
	virtual R::RConfig* GetConfig(const R::RString& name,const R::RString& type) const {return(GetFactory(type,name,false));}

	/**
	 * Apply the configuration for a plug-in corresponding to a given
	 * R::RConfig.
	 * @param config         Pointer to the configuration.
	 */
	virtual void ApplyConfig(R::RConfig* config);

	/**
	* Set the current method if the plug-ins must have a selected one.
	* @param type            Type of the factory.
	* @param name            Name of the method.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	*/
	virtual void SetCurrentMethod(const R::RString& type,const R::RString& name,bool need=true);

	/**
	* Set the current method if the plug-ins must have a selected one.
	* @param name            Name of the method.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	*/
	virtual void SetCurrentMethod(const R::RString&,bool) {throw GException("No type specified");}

	/**
	* Get the current method.
	* @param type            Type of the factory.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the plug-in, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	virtual GMeasure* GetCurrentMethod(const R::RString& type,bool need=true) const;

	/**
	* Get the current factory.
	* @param type            Type of the factory.
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
