/*

	GALILEI Research Project

	GNodeSet.h

	Concept Node Set - Header.

	Copyright 2004-2012 by Pascal Francq.
   Copyright 2004-2005 by Jean-Baptiste Valsamis.
	Copyright 2005-2009 by Faïza Abbaci.

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
#ifndef GNodeSetH
#define GNodeSetH


//------------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <galilei.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
class GNodeSet : public RContainer<XNode,false, false>
{
	size_t DocId;

public :

	GNodeSet(size_t docid);

	size_t GetDocId(void) const {return(DocId);}

//	void GetSetIdNode(const int idnode, XNodeSet *xns) const;
//	void GetSetIdPath(const int idpath, XNodeSet *xns) const;
//	void PushUp(const int idnode, const XNode &node);
//	void PushUp(const int idnode,const int dis,const int specif,  const XNode &node);
//	void Oper(XConst::tOper op, XNodeSet *p, XNodeSet *q);
//	void Reduce(); //elimine les doublons en fusionnant les types
//	inline void Start();
//	inline void Next();
//	inline bool End();
//	inline void NextId();
//	inline void Last();
//	inline void Prev();
//	inline bool Begin();
//	inline void PrevId();
//	inline void WatchId();
//	inline bool IdChanged();
//	inline XNode *Node();
//	int Compare(const XNodeSet &xns) const;
//	int Compare(const int &_idfile) const;
//	bool operator == (const XNodeSet &xns) const;
//	bool operator < (const XNodeSet &xns) const;
};


//------------------------------------------------------------------------------
#endif
