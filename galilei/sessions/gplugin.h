/*

	GALILEI Research Project

	GPlugin.h

	Generic Plugin - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
// include file for LibTool--
#include <ltmm/loader.hh>


//------------------------------------------------------------------------------
// include file for GALILEI
#include <sessions/gparams.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GPLugin class provides a generic plugin.
* @author Pascal Francq
* @short Generic plugin.
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
	* Construct the plugin.
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
template<class factory,class plugin,class mng>
	class GFactoryPlugin : public GParams
{
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
	ltmm::symbol* AboutDlg;

	/**
	* Pointer to a function showing the configure box.
	* @param f               Factory of the pluging to configure.
	*/
	ltmm::symbol* ConfigDlg;

public:
	/**
	* Constructor.
	* @param m               Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPlugin(mng* m,const char* n,const char* f)
		: GParams(n), Mng(m), Plugin(0), Lib(f), AboutDlg(0), ConfigDlg(0) {}

	/**
	* Set the about box.
	* @param dlg             Param to the function.
	*/
	void SetAbout(ltmm::symbol* dlg) {AboutDlg=dlg;}

	/**
	* Set the configure box.
	* @param dlg             Param to the function.
	*/
	void SetConfig(ltmm::symbol* dlg) {ConfigDlg=dlg;}

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
	* Show 'about' information.
	*/
	void About(void)
	{
		if(AboutDlg)
			((void(*)(void))(AboutDlg->ptr()))();
	}

	/**
	* Configure the parameters.
	*/
	void Configure(void)
	{
		if(ConfigDlg)
			((void(*)(factory*))(ConfigDlg->ptr()))(dynamic_cast<factory*>(this));
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
	const char* GetLib(void) const {return(Lib);}

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
		R::RXMLTagCursor Cur=parent->GetXMLTagsCursor();
		RXMLTag* tag=0;

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
	}

	/**
	* Save a configuration to a XML Tag.
	* this tag.
	* @param parent          Parent tag.
	*/
	void SaveConfig(R::RXMLTag* parent)
	{
		RXMLTag* tag=new R::RXMLTag("Plugin");
		tag->InsertAttr("Name",Name);
		parent->AddTag(tag);
		if(Plugin)
			tag->InsertAttr("Enable","True");
		else
			tag->InsertAttr("Enable","False");
		GParams::SaveConfig(tag);
	}

	/**
	* Destructor.
	*/
	virtual ~GFactoryPlugin(void) {if(Plugin) delete Plugin;}
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
