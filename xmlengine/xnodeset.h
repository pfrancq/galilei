/***************************************************************************
 *   Copyright (C) 2005 by Jean-Baptiste Valsamis                          *
 *   jvalsami@ulb.ac.be                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __XNODESETH__						// Avoids multiple declarations
#define __XNODESETH__

#include <vector>
using namespace std;

#include <rcontainer.h>
#include <rcursor.h>
using namespace R;

#include "xgeneral.h"
#include "xnode.h"
//#include "vector.h"
#include <gstorage.h>

class XNodeSet : public RContainer<XNode, true, true>
{
private :

	int idfile;
	int currpos;
	int currid;

public :

	XNodeSet(const int _idfile = 0);
	XNodeSet(const XNodeSet &xns);
	int GetIdFile() const;
	void GetSetIdNode(const int idnode, XNodeSet *xns) const;
	void GetSetIdPath(const int idpath, XNodeSet *xns) const;
	void PushUp(const int idnode, const XNode &node);
	void PushUp(const int idnode,const int dis,const int specif,  const XNode &node);
	void Oper(XConst::tOper op, XNodeSet *p, XNodeSet *q);
	void Reduce(); //elimine les doublons en fusionnant les types
	inline void Start();
	inline void Next();
	inline bool End();
	inline void NextId();
	inline void Last();
	inline void Prev();
	inline bool Begin();
	inline void PrevId();
	inline void WatchId();
	inline bool IdChanged();
	inline XNode *Node();
	int Compare(const XNodeSet &xns) const;
	int Compare(const int &_idfile) const;
	bool operator == (const XNodeSet &xns) const;
	bool operator < (const XNodeSet &xns) const;
	~XNodeSet();
	RString print() const
	{
		RString res;
		RCursor<XNode> c(*this);

		res += "idfile = " + RString::Number(idfile) + "\n";
		for (c.Start(); !c.End(); c.Next())
			res += "      " + c()->print() + "\n";
		return res;
	}
};

#endif
