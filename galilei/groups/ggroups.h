/*

	GALILEI Research Project

	gwordref.h

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

#ifndef GGroupsH
#define GGroupsH
//-----------------------------------------------------------------------------
//include files for GALILEI
#include<ggroups/ggroup.h>

#include<ggroups/ggroupslangs.h>
using namespace GALILEI;

//-----------------------------------------------------------------------------
//include files for Rainbow
#include <rstd/rcontainer.h>
//-----------------------------------------------------------------------------
 namespace GALILEI{
class GGroup;
class GGroupsLangs;
//-----------------------------------------------------------------------------
/**
*/
class GGroups : public RStd::RContainer<GGroup,unsigned int,true,false>
{
public:
	GGroupsLangs *Owner;	
	GLang *Lang;

	GGroups(GGroupsLangs *owner,GLang *lang) throw(bad_alloc);
	int Compare(const GGroups &groups);
	int Compare(const GGroups *groups);	
	//virtual void Load(void)=0;
	//virtual void Save(void)=0;
	//virtual GGroup* NewGroup(void)=0;
	void Calc(void);
};
}
#endif
