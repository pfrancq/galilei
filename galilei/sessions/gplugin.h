/*

	GALILEI Research Project

	GPlugin.h

	Generic Plugin - Header.

	Copyright 2003-2204 by the Université libre de Bruxelles.

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
#ifndef GPluginH
#define GPluginH


//------------------------------------------------------------------------------
// include file for R
#include <rstd/rxmltag.h>

//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>
using namespace std;
using namespace R;


//------------------------------------------------------------------------------
// include file for dlopen
#include <dlfcn.h>

//------------------------------------------------------------------------------
// include file for GALILEI
#include <sessions/galilei.h>
#include <sessions/gparams.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GPLugin class provides a template for a generic plugin.
* @author Pascal Francq
* @short Generic Plugin.
*/
template<class factory>
	class GPlugin
{
protected:

	/**
	* Pointer to the factory.
	*/
	factory* Factory;

public:

	/**
	* Constructor of the plugin.
	* @param fac            Factory.
	*/
	GPlugin(factory* fac) : Factory(fac) {}

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void) {}

	/**
	* Get the factory of the plugin.
	*/
	factory* GetFactory(void) const {return(Factory);}

	/**
	* Destructor of the plugin.
	*/
	virtual ~GPlugin(void) {}
};


//------------------------------------------------------------------------------
/**
* Type of a function used to show the about box of a plugin.
*/
typedef void (*About_t)();
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GPLuginPlugin class provides a template for a generic plugin factory. A
* factory handles the loading of the dynamic library containing the plugin.
* @author Pascal Francq
* @short Generic Plugin Factory.
*/
template<class factory,class plugin,class mng>
	class GFactoryPlugin : public GParams
{
public:
	/**
	* Type of a function used to show dialog box of a plugin.
	*/
	typedef void* (*Configure_t)(GFactoryPlugin*);

protected:

	/**
	* URL Manager associated.
	*/
	mng* Mng;

	/**
	* Pointer to the plugin.
	*/
	plugin* Plugin;

	/**
	* Name of the library.
	*/
	R::RString Lib;

	/**
	* Pointer to a function showing the about box.
	*/
	About_t AboutDlg;

	/**
	* Pointer to a function showing the configure box.
	* @param f               Factory of the pluging to configure.
	*/
	void* ConfigDlg;

	/**
	* The Handle(void *) to maintain the (dlopen)handler for the opened library
	*/
	void* Handle;

	/**
	* The Handle(void *) to maintain the (dlopen)handler for the opened library
	*/
	void* HandleDlg;


public:
	/**
	* Constructor.
	* @param m               Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPlugin(mng* m,const char* n,const char* f)
		: GParams(n), Mng(m), Plugin(0), Lib(f), AboutDlg(0), ConfigDlg(0), Handle(0), HandleDlg(0) {}

	/**
	* Get the manager of the factory.
	* @return Pointer to the manager.
	*/
	mng* GetMng(void) const {return(Mng);}

	/**
	* Set the about box.
	* @param dlg             Param to the function.
	*/
	void SetAbout(About_t dlg) {AboutDlg=dlg;}

	/**
	* Set the configure box.
	* @param dlg             Param to the function.
	*/
	void SetConfig(void* dlg) {ConfigDlg=dlg;}

	/**
	* Set the handler of the library.
	* @param handle         Handle of the library.
	*/
	void SetHandler(void* handle) {Handle=handle;}

	/**
	* Set the Handler of the Dlg library.
	* @param handleDlg      Handle of the dialog library
	*/
	void SetDlgHandler(void* handleDlg) {HandleDlg=handleDlg;}

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const factory& f) const {return(Name.Compare(f.Name));}

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const factory* f) const {return(Name.Compare(f->Name));}

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const char* n) const {return(Name.Compare(n));}

	/**
	* Create a plugin.
	*/
	virtual void Create(void) throw(GException)= 0;

	/**
	* Create a plugin.
	*/
	virtual void Delete(void) throw(GException)= 0;

	/**
	* Create a plugin.
	*/
	virtual void Create(GSession*) throw(GException)= 0;

	/**
	* Create a plugin.
	*/
	virtual void Delete(GSession*) throw(GException)= 0;

	/**
	* Show 'about' information.
	*/
	void About(void)
	{
		if(AboutDlg)
			AboutDlg();
	}

	/**
	* Configure the parameters.
	*/
	void Configure(void)
	{
		if(ConfigDlg)
		{
			Configure_t config= (Configure_t) ConfigDlg ;
			config(dynamic_cast<factory*>(this));
		}
	}

	/**
	* Apply the configuration eventually to the plugin.
	*/
	void Apply(void) {if(Plugin) Plugin->ApplyConfig();}

	/**
	* Specify if an about box exist.
	*/
	bool HasAbout(void) const {return(AboutDlg);}

	/**
	* Specify if a configure box exist.
	*/
	bool HasConfigure(void) const {return(ConfigDlg);}

	/**
	* Get the API Version of the plugin.
	* @return C String.
	*/
	virtual const char* GetAPIVersion(void) const=0;

	/**
	* Return the manager of the plugin.
	*/
	mng* GetMng(void) {return(Mng);}

	/**
	* Get the plugin of this factory.
	*/
	plugin* GetPlugin(void) const {return(Plugin);}

	/**
	* Get the library of the plugin.
	*/
	RString GetLib(void) const {return(Lib);}

	/**
	* Return the version of the plugin.
	*/
	const char* GetVersion(void) const {return(Version);}

	/**
	* Read a configuration from a XML Tag.
	* @param parent          Parent Tag.
	*/
	void ReadConfig(R::RXMLTag* parent)
	{
		R::RCursor<R::RXMLTag> Cur=parent->GetXMLTagsCursor();
		R::RXMLTag* tag=0;

		// Find Tag
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if((Cur()->GetName()=="Plugin")&&(Cur()->GetAttrValue("Name")==Name))
			{
				tag=Cur();
				break;
			}
		}
		if(!tag) return;

		// Read Info
		if(tag->GetAttrValue("Enable")=="True")
			Create();
		GParams::ReadConfig(tag);

		// Apply the information.
		Apply();
	}

	/**
	* Save a configuration to a XML Tag.
	* this tag.
	* @param xml             XML Structure.
	* @param parent          Parent tag.
	*/
	void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* parent)
	{
		R::RXMLTag* tag=new R::RXMLTag("Plugin");
		tag->InsertAttr("Name",Name);
		xml->AddTag(parent,tag);
		if(Plugin)
			tag->InsertAttr("Enable","True");
		else
			tag->InsertAttr("Enable","False");
		GParams::SaveConfig(xml,tag);
	}

	/**
	* Destructor.
	*/
	virtual ~GFactoryPlugin(void)
	{
		if(Plugin)
			delete Plugin;

		if (Handle)
			dlclose(Handle);

		if (HandleDlg)
			dlclose(HandleDlg);
	}
};


//------------------------------------------------------------------------------
/**
* Starting from a directory, this function looks for all shared libraries in the
* sub-dirs and add them in the containers.
*/
void FindPlugins(const R::RString& dir,R::RContainer<R::RString,true,true>& plugins,R::RContainer<R::RString,true,true>& dlgs);


//------------------------------------------------------------------------------
/**
* Template function to load a given type of plugins in a given directory.
* @param factory            Generic Factory.
* @param factoryInit        Type of the function used to initialize a factory.
* @param manager            Type of the manager.
* @param mng                Pointer to the manager.
* @param path               Path of the directory to look at.
* @param API_version        The API version of the plugins handled.
* @param dlg                Specify if the dialog boxes shared libraries must be
*                           loaded (true) or not (false).
*/
template<class factory,class factoryInit,class manager>
	void LoadPlugins(manager* mng, const char* path, const char* API_version,bool dlg) throw(std::bad_alloc, GException)
{

	DIR* dp;
	struct dirent* ep;
	RString Path(path);
	RString Msg;
	RString Name;
	char DlgLib[100];
	int len;
	bool found=false;
	bool end=false;

	while (!end)
	{
		dp=opendir(Path);
		Path+="/";
		if(!dp) return;
		while((ep=readdir(dp)))
		{
			len=strlen(ep->d_name);
			if(len<3) continue;
			if(strcmp(&ep->d_name[len-3],".so")) continue;
			if((len>7)&&(!strcmp(&ep->d_name[len-7],"_dlg.so"))) continue;
			try
			{

				// Create the factory and insert it
				Name=Path+ep->d_name;
				void *handle=dlopen(Name,RTLD_LAZY);

				if (handle == NULL)
				{
					char *error=dlerror();
					Msg+="ERROR :";
					Msg+=error;
					continue;
				}

				factoryInit* initFac= (factoryInit*) dlsym(handle,"FactoryCreate");
				char *error=NULL;
				if((error=dlerror())!=NULL)
				{
					Msg+="ERROR :";
					Msg+=error;
					continue;
				}

				factory *myfactory= initFac(mng ,ep->d_name);

				if(strcmp(API_version,myfactory->GetAPIVersion()))
				{
					Msg+=ep->d_name;
					Msg+=" - Plugin not compatible with API Version\n";
					continue;
				}
				mng->InsertPtr(myfactory);
				found=true;

				// Look if dialog boxes are available
				if(!dlg) continue;
				// LOAD DLG PLUGINS
				try
				{
					strcpy(DlgLib,Name);
					DlgLib[Name.GetLen()-3]=0;
					strcat(DlgLib,"_dlg.so");

					void* handleDLG=dlopen(DlgLib,RTLD_LAZY);
					if (handleDLG == NULL)
					{
						continue;
					}

					// -- Get Symbol on the About method
					About_t about = (About_t) dlsym(handleDLG,"About");
					char *error=NULL;
					if((error=dlerror())!=NULL)
					{
						continue;
					}

					//register method
					myfactory->SetAbout(about);


					// -- Get Symbol on the Configure  method
					void* config= dlsym(handleDLG,"Configure");
			 		char *errorDlg=NULL;
					if((errorDlg=dlerror())!=NULL)
					{
						continue;
					}

					//register method
					myfactory->SetConfig(config);
				}
				catch(...)
				{
				}
			}
			catch(std::exception& e)
			{
				Msg+=ep->d_name;
				Msg+=" - ";
				Msg+=e.what();
				Msg+="\n";
			}
		}

		//if the libaries are found then stop the search
		if (found)
		{
			end=true;
		}
		// else continue the search in the sub dir ".libs"
		else
		{
			Path+=".libs";
		}
		closedir(dp);
	}
	// If something in Msg -> error
	if(Msg.GetLen())
		throw(GException(Msg));
}


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
