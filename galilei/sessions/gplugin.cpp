/*

	GALILEI Research Project

	GPlugin.cpp

	Generic Plugin - Implementation.

	Copyright 2003-2004 by the Université libre de Bruxelles.

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
#include <sessions/gplugin.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// General function
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void FindPlugins(const RString& dir,RContainer<RString,true,true>& plugins,RContainer<RString,true,true>& dlgs)
{
	DIR* dp;
	struct dirent* ep;
	RString Path(dir);
	RString Name;
	int len;

	dp=opendir(Path);
	Path+="/";
	if(!dp) return;
	while((ep=readdir(dp)))
	{
		// Name og the 'file"
		Name=Path+ep->d_name;

		// Look if it is a directoy
		if(ep->d_type==DT_DIR)
		{
			// If not '.' and '..' -> goes though it
/*			if((Name!=".")&&(Name!=".."))
				FindPlugins(Name,plugins,dlgs);*/
			continue;
		}

		// Must be a regular file
		if(ep->d_type==DT_REG)
			continue;

		len=strlen(ep->d_name);
		if(Name.GetLen()<3)
			continue;

		if(strcmp(&ep->d_name[len-3],".so")) continue;

		// Is it a dialog box?
		if((len>7)&&(!strcmp(&ep->d_name[len-7],"_dlg.so")))
		{
			dlgs.InsertPtr(new RString(Name));
		}

		// Msut be the main plugin
		plugins.InsertPtr(new RString(Name));
	}
	closedir(dp);
}
