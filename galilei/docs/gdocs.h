/*

	GALILEI Research Project

	gdocs.h

	Basic Information - Implementation.

	(C) 2001 by P. Francq.

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

//---------------------------------------------------------------------------
#ifndef GDocsH
#define GDocsH


//---------------------------------------------------------------------------
// include files for Rainbow

#include <rstd/rcontainer.h>
#include <rstd/rstring.h>
//#include  <HyperPRISME.h>
using namespace RStd;


//---------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>
#include <gsessions/gsession.h>
#include <glangs/glangs.h>
#include <gprofiles/gprofile.h>
#include <gdocs/gdoc.h>
#include <gexceptions.h>
#include <glib.h>

using namespace GALILEI;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
namespace GALILEI{

class GSession;
class GLangs;
class GProfile;
//---------------------------------------------------------------------------
// class GDocs

class GDocs : public RContainer<GDoc,unsigned,true,false>
{
public:
	bool AllDocs;             // All Docs must be Loaded?

	GSession *Session;        // Languages

	GDocs(unsigned int nb,bool alldocs,GSession *session) throw(bad_alloc);
	virtual void Load(GLangs*) throw(bad_alloc,GException)=0;
	virtual void Load(GLangs*,GProfile *) throw(bad_alloc,GException)=0;
	void Analyse(URLFunc *urlfunc,InfoFunc *infofunc) throw(GException);
	void Download(void);
	virtual ~GDocs(void) {}

};




 }  //-------- End of namespace Galilei-----------------------------------

//---------------------------------------------------------------------------
#endif
