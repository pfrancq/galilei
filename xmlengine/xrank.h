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

#ifndef __XRANKH__							// Avoids multiple declarations
#define __XRANKH__

#include "xgeneral.h"

class XRank
{
static int totrefs;
int thisref;
public :

	enum tElem
	{
		TAGNAM = 8,
		ATTNAM = 4,
		ATTVAL = 2,
		TAGVAL = 1,
		NOVAL = 0
	};

private :
	
	tElem type;
	int depth;
	int iddeepest;
	bool relevant;
	bool last;

public :
	
	XRank(const tElem _type, const int _iddeepest);// = NOVAL, const int _depth = 0, const bool _relevant = true);
	XRank(const tElem _type, const int _depth, const int _iddeepest, const bool _relevant);
	XRank(const XRank &xrk);
	int GetIdNode() const;
	tElem GetType() const;
	int GetDepth() const;
	int GetDeepestId() const;
	bool IsRelevant() const;
	bool IsLast() const;
	void SetLast(bool _last);
//	bool IsMe(const int _type, const int _depth, const int _idnoderelevant, const bool _relevant);
	void SetDeepestId(int idnode);
	void AutoInc();
//	void Add(const XScore *_xsc);
//	void AddAutoInc(const XScore *_xsc);
//	void Union(const XScore *s, const XScore *t);
//	void Inter(const XScore *s, const XScore *t);
//	int ComputeScore() const;
	int Compare(const XRank *xrk) const;
//	int Compare(const int _idnode) const;
	bool operator == (const XRank &xrk) const;
//	bool operator < (const XScore &xs) const;
/*	RString print() const
	{
		RString s;
		unsigned int i;
		vector<int>::iterator it;
		
		s = "id:" + itou(idnode);
		s += " cnt:" + itou(cnttype[0]);
		for (i = 1; i < 4; i++)
			s += "-" + itou(cnttype[i]);
		s += " proxim:" + itou(proxim[0]);
		for (i = 1; i < proxim.size(); i++)
			s += "-" + itou(proxim[i]);
		return s;
	}*/
	~XRank();
};

#endif
