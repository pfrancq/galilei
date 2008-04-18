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

#ifndef __XQUERYNODEH__							// Avoids multiple declarations
#define __XQUERYNODEH__

#include <rnode.h>
#include <rstring.h>
using namespace R;

#include "xgeneral.h"
#include "xqueryres.h"

class XQueryNode : public RNode<XQueryNode, true, false>
//FIXME false ou true pour bAlloc (arg2)???
{
public :

	enum tNodeType
	{
		OPERATOR,
		QUERYRES,
		STRING
	};

private :
	
	tNodeType type;
	union
	{
		XConst::tOper op;
		XQueryRes *qres;						// Need ptr in a union
		RString *word;							// Need ptr in a union
	} value;
	
	void clear();
	
public :
	
	XQueryNode(XConst::tOper _op);
	XQueryNode(XQueryRes *_qres);
	XQueryNode(RString *_word);
	XQueryNode(const XQueryNode *xqn);
	XQueryNode(const XQueryNode &xqn);
	tNodeType GetType();
	XConst::tOper GetOp();
	XQueryRes *GetQRes();
	RString GetWord();
	void Set(XConst::tOper _op);
	void Set(XQueryRes *_qres);
	void Set(RString *_word);
	int Compare(const XQueryNode &unused) const;
	~XQueryNode();
	friend class RTree<XQueryNode, true, false>;
	friend class RTree<XQueryNode, false, false>;
};

#endif
