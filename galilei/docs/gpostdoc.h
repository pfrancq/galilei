/*

	GALILEI Research Project

	GPostDoc.h

	Generic Documents Post-Analysis Method - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors
		 Kumps Nicolas (nkumps@ulb.ac.be)

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
#ifndef GPostDocH
#define GPostDocH


//------------------------------------------------------------------------------
// include file for GALILEI
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_POSTDOC_VERSION "1.0"


//------------------------------------------------------------------------------
/**
* The GPostDoc provides a representation for a generic documents post-analysis
* method.
* @author Nicolas Kumps
* @short Generic Document Post Analysis Method.
*/
class GPostDoc : public GPlugin<GFactoryPostDoc>
{
protected :

	/**
	* Session.
	*/
	GSession* Session;

public :

	/**
	* Constructor of the documents post-analysis method.
	* @param fac             Factory of the plugin.
	*/
	GPostDoc(GFactoryPostDoc* fac) throw(std::bad_alloc);

	/**
	* Connect to a Session.
	* @param session         Session.
	*/
	virtual void Connect(GSession* session) throw(GException);

	/**
	* Disconnect from a Session.
	* @param session         Session.
	*/
	virtual void Disconnect(GSession* session) throw(GException);

	/**
	* Run the method.
	*/
	virtual void Run(void) throw(GException)=0;

	/**
	* Destructor of the documents post-analysis method.
	*/
	virtual ~GPostDoc(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryPostDoc represent a factory for a given document post analysis
* method.
* @author Nicolas Kumps
* @short Generic Document Post Analysis Factory.
*/
class GFactoryPostDoc : public GFactoryPlugin<GFactoryPostDoc,GPostDoc,GPostDocManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPostDoc(GPostDocManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryPostDoc,GPostDoc,GPostDocManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryPostDoc(void) {}
};


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a document post analysis factory.
*/
typedef GFactoryPostDoc* GFactoryPostDocInit(GPostDocManager*,const char*);


//-------------------------------------------------------------------------------
#define CREATE_POSTDOC_FACTORY(name,C)                                                    \
class TheFactory : public GFactoryPostDoc                                                 \
{                                                                                         \
private:                                                                                  \
	static GFactoryPostDoc* Inst;                                                         \
	TheFactory(GPostDocManager* mng,const char* l) : GFactoryPostDoc(mng,name,l)          \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryPostDoc* CreateInst(GPostDocManager* mng,const char* l)                \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_POSTDOC_VERSION);}          \
	virtual void Create(void) throw(GException)                                           \
	{                                                                                     \
		if(Plugin) return;                                                                \
		Plugin=new C(this);                                                               \
		Plugin->ApplyConfig();                                                            \
	}                                                                                     \
	virtual void Delete(void) throw(GException)                                           \
	{                                                                                     \
		if(!Plugin) return;                                                               \
		delete Plugin;                                                                    \
		Plugin=0;                                                                         \
	}                                                                                     \
	virtual void Create(GSession* ses) throw(GException)                                  \
	{                                                                                     \
		if(!Plugin)                                                                       \
		{                                                                                 \
			Plugin=new C(this);                                                           \
			Plugin->ApplyConfig();                                                        \
		}                                                                                 \
		if(ses)                                                                           \
			Plugin->Connect(ses);                                                         \
	}                                                                                     \
	virtual void Delete(GSession* ses) throw(GException)                                  \
	{                                                                                     \
		if(!Plugin) return;                                                               \
		if(ses)                                                                           \
			Plugin->Disconnect(ses);                                                      \
		delete Plugin;                                                                    \
		Plugin=0;                                                                         \
	}                                                                                     \
};                                                                                        \
                                                                                          \
GFactoryPostDoc* TheFactory::Inst = 0;                                                    \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryPostDoc* FactoryCreate(GPostDocManager* mng,const char* l)                    \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


//------------------------------------------------------------------------------
/**
* The GFactoryPostDocCursor class provides a way to go trough a set of
* factories of documets post-analysis methods.
* @short Documents Post-Analysis Factories Cursor
*/
CLASSCURSOR(GFactoryPostDocCursor,GFactoryPostDoc);


//------------------------------------------------------------------------------
/**
* struture to order factory by their level
*/
struct GFactoryPostDocOrder
{
	GFactoryPostDoc* Fac;
	int Compare(const GFactoryPostDocOrder* fpdo)
	{
		return((Fac->GetUInt("Level"))-(fpdo->Fac->GetUInt("Level")));
	}
};

}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
