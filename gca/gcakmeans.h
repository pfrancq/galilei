/*

	GALILEI Research Project

	GCAkMeans.h

	kMeans - Header.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).

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


//-----------------------------------------------------------------------------
#ifndef GCAkMeansH
#define GCAkMeansH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <gca.h>
#include <rgroupingkmeans.h>


//-----------------------------------------------------------------------------
/**
 * The GroupDoc provides a representation for a group holding documents (used
 * by kMeansDoc).
 * @author Pascal Francq
 * @short kMeans Document Group
 */
class CGroupDoc : public R::RGroup<CGroupDoc,GDoc,CGroupsDoc>
{
public:

	/**
	 * Constructor.
	 * @param owner          Owner of the group.
	 * @param id             Identifier of the group.
	 */
	CGroupDoc(CGroupsDoc* owner,size_t id) : R::RGroup<CGroupDoc,GDoc,CGroupsDoc>(owner,id) {}
};


//-----------------------------------------------------------------------------
/**
 * A Set of document groups.
 */
class CGroupsDoc : public R::RGroups<CGroupDoc,GDoc,CGroupsDoc>
{
public:
	CGroupsDoc(R::RCursor<GDoc> objs,size_t max) : R::RGroups<CGroupDoc,GDoc,CGroupsDoc>(objs,max) {}
};


//-----------------------------------------------------------------------------
class kMeansDoc : public R::RGroupingKMeans<CGroupDoc,GDoc,CGroupsDoc>
{
	GMeasure* Measure;
public:
	kMeansDoc(const R::RString& n,R::RRandom* r,R::RCursor<GDoc> objs,R::RDebug* debug=0);

	double Similarity(const GDoc* obj1,const GDoc* obj2);
};


//-----------------------------------------------------------------------------
/**
 * The GroupProfile provides a representation for a group holding profiles (used
 * by kMeansProfile).
 * @author Pascal Francq
 * @short kMeans Profile Group
 */
class CGroupProfile : public R::RGroup<CGroupProfile,GProfile,CGroupsProfile>
{
public:

	/**
	 * Constructor.
	 * @param owner          Owner of the group.
	 * @param id             Identifier of the group.
	 */
	CGroupProfile(CGroupsProfile* owner,size_t id) : R::RGroup<CGroupProfile,GProfile,CGroupsProfile>(owner,id) {}
};


//-----------------------------------------------------------------------------
/**
 * A Set of profile groups.
 */
class CGroupsProfile : public R::RGroups<CGroupProfile,GProfile,CGroupsProfile>
{
public:
	CGroupsProfile(R::RCursor<GProfile> objs,size_t max) : R::RGroups<CGroupProfile,GProfile,CGroupsProfile>(objs,max) {}
};


//-----------------------------------------------------------------------------
class kMeansProfile : public R::RGroupingKMeans<CGroupProfile,GProfile,CGroupsProfile>
{
	GMeasure* Measure;
public:
	kMeansProfile(const R::RString& n,R::RRandom* r,R::RCursor<GProfile> objs,R::RDebug* debug=0);

	double Similarity(const GProfile* obj1,const GProfile* obj2);
};


//-----------------------------------------------------------------------------
#endif
