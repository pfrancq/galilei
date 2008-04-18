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

#include "xquerynode.h"
//FIXME : bAlloc = false. True instead ???
XQueryNode::XQueryNode(XConst::tOper _op) : RNode<XQueryNode, true, false>()
{
	type = OPERATOR;
	value.op = _op;
__NEW__(XQN, 0);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryNode::XQueryNode(XQueryRes *_qres) : RNode<XQueryNode, true, false>()
{
	type = QUERYRES;
	value.qres = _qres;
__NEW__(XQN, 1);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryNode::XQueryNode(RString *_word) : RNode<XQueryNode, true, false>()
{
	type = STRING;
	value.word = _word;
__NEW__(XQN, *_word);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryNode::XQueryNode(const XQueryNode *xqn) : RNode<XQueryNode, true, false>(*xqn)
{
	type = xqn->type;
	if (type == OPERATOR)
		value.op = xqn->value.op;
	else if (type == STRING)
		value.word = new RString(*xqn->value.word);
__NEW__(XQN, "*copy");
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryNode::XQueryNode(const XQueryNode &xqn) : RNode<XQueryNode, true, false>(xqn)
{
	type = xqn.type;
	if (type == OPERATOR)
		value.op = xqn.value.op;
	else if (type == STRING)
		value.word = new RString(*xqn.value.word);
__NEW__(XQN, "&copy");
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQueryNode::clear()
{
	if (type == QUERYRES)
		delete value.qres;
	else if (type == STRING)
		delete value.word;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryNode::tNodeType XQueryNode::GetType()
{
	return type;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XConst::tOper XQueryNode::GetOp()
{
	return value.op;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryRes *XQueryNode::GetQRes()
{
	return value.qres;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XQueryNode::GetWord()
{
	return *value.word;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQueryNode::Set(XConst::tOper _op)
{
	clear();
	type = OPERATOR;
	value.op = _op;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQueryNode::Set(XQueryRes *_qres)
{
	clear();
	type = QUERYRES;
	value.qres = _qres;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQueryNode::Set(RString *_word)
{
	clear();
	type = STRING;
	value.word = _word;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XQueryNode::Compare(const XQueryNode &unused) const
{
	return this - &unused;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryNode::~XQueryNode()
{
	clear();
__DEL__(XQN, 0);
}
