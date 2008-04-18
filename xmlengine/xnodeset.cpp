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

#include "xnodeset.h"
#include "xstuff.h"

XNodeSet::XNodeSet(const int _idfile) : RContainer<XNode, true, true>(20)
{
	idfile = _idfile;
	__NEW__(XNS, idfile);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XNodeSet::XNodeSet(const XNodeSet &xns) : RContainer<XNode, true, true>(xns)
{
	idfile = xns.idfile;
	__NEW__(XNS, -idfile);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSet::GetIdFile() const
{
	return idfile;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::GetSetIdNode(const int idnode, XNodeSet *xns) const
{
	RCursor<XNode> curs(*this);
	
	xns->idfile = idfile;
	for (curs.Start(); !curs.End(); curs.Next())
		if (curs()->GetIdNode() == idnode)
		{
			xns->InsertPtr(new XNode(*curs()));
			return;
		}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::GetSetIdPath(const int idpath, XNodeSet *xns) const
{
	RCursor<XNode> curs(*this);
	
	xns->idfile = idfile;
	for (curs.Start(); !curs.End(); curs.Next())
		if (curs()->GetIdPath() == idpath)
			xns->InsertPtr(new XNode(*curs()));
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::PushUp(const int idnode, const XNode &node)
{
	XNode *nres;
	
	nres = new XNode(node);
	nres->PushUp(idnode);
	InsertPtr(nres);
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::PushUp(const int idnode, const int dis, const int specif, const XNode &node)
{
	XNode *nres;
	
	nres = new XNode(node);
	nres->PushUp(idnode);
	nres->distance=dis;
	nres->specificity=specif;
	InsertPtr(nres);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::Oper(XConst::tOper op, XNodeSet *p, XNodeSet *q)
{
	XNodeSet *h;																// Pointer to highest p, q
	XNodeSet ns;
	XNode *xn;
	vector<int>::iterator it;
	int i;
	bool itemadded;
	RContainer<Int, true, true> pidpathset(10);									// Set of idpath added
	RContainer<Int, true, true> qidpathset(10);									// Set of idpath to add
	RContainer<Int, true, true> idnodeset(10);
	RCursor<Int> cint;
	
	if (op == XConst::OR)
	{
		// on rajoute les �l�ment de p qui ne sont pas dans this	
		if (p->GetNb())
			for (p->Start();!p->End(); p->Next() ){
				xn = p->Node();
				xn = GetPtr(p->Node());
				if (xn) {
					i=0;
					xn->distance += p->Node()->distance; 
					//for (it = p->Node()->types.begin(); it < p->Node()->types.end();  it++){
					//	xn->types.push_back(p->Node()->types[i]); 
					//	i++;
					//}

				} else InsertPtr( new  XNode(*p->Node()) );
			}
		// on rajoute les �l�ment de q qui ne sont pas dans this
		if (q->GetNb())
			for (q->Start();!q->End(); q->Next() ){
				xn = q->Node();
				xn = GetPtr(q->Node());
				if (xn) {
					i=0;
					xn->distance += p->Node()->distance; 
// 					for (it = q->Node()->types.begin(); it < q->Node()->types.end();  it++){
// 						xn->types.push_back(q->Node()->types[i]); 
// 						i++;
// 					}
				} else InsertPtr( new  XNode(*q->Node()) );
			}
		
	}	
	else
	{
		p->Last();
		q->Last();
		while (!p->Begin() && !q->Begin())
		{
			
			if (*p->Node() == *q->Node())
			{
				switch (op)
				{
				case XConst::AND :
					// Faiza : On cherche d'abord si ce noeud (p/q->node()  existe d�j�  dans this
						xn = q->Node();
						xn = GetPtr(q->Node());
						//si oui on rajoute les types
						if (xn) {
							i=0;
							xn->distance += q->Node()->distance;
// 							for (it = q->Node()->types.begin(); it < q->Node()->types.end();  it++){
// 								xn->types.push_back(q->Node()->types[i]); 
// 								i++;
// 							}
						} else { //sinon on fusionne les types de p et q dans p et on insert p
							i=0;

							p->Node()->distance += q->Node()->distance;
							InsertPtr( new  XNode(*p->Node()) );
						}
					idnodeset.InsertPtr(new Int(p->Node()->GetIdNode()));
					p->PrevId();
					q->PrevId();
					break;
				case XConst::INC :
					for (p->WatchId(), itemadded = false; !p->IdChanged(); p->Prev())
						if (p->Node()->IsLast())
						{
							pidpathset.InsertPtr(new Int(p->Node()->GetIdPath()));
							itemadded = true;
						}
					if (itemadded)
						for (q->WatchId(); !q->IdChanged(); q->Prev())
							qidpathset.InsertPtr(new Int(q->Node()->GetIdPath()));
						else
							q->PrevId();
				default :
					p->PrevId();
					q->PrevId();
					break;
				}
			}
			else
			{
				h = *p->Node() < *q->Node() ? q : p;
				switch (op)
				{
				case XConst::NAND :
					if (h == p)
						for (h->WatchId(); !h->IdChanged(); h->Prev())
							InsertPtr(new XNode(*h->Node()));
					else
						h->PrevId();
//					idnodeset.InsertPtr(new Int((*p)[ip]->GetIdNode()));
					break;
				default :
					h->PrevId();
					break;
				}
			}
		}
	}
	switch (op)
	{
	case XConst::AND :
		for (i = idnodeset.GetNb() - 1; i > -1; i--)
		{
//			ns.Clear();
			p->GetSetIdNode(idnodeset[i]->Get(), &ns);
			q->GetSetIdNode(idnodeset[i]->Get(), &ns);
			if (i == idnodeset.GetNb() - 1)
				for (ns.Start(); !ns.End(); ns.Next())
					ns.Node()->SetLast(true);
//			(*this) += ns;
		}
		break;
	case XConst::INC :
		for (p->Start(); !p->End(); p->Next())
			if (pidpathset.IsIn(p->Node()->GetIdPath()))
				InsertPtr(new XNode(*p->Node()));
		for (q->Start(); !q->End(); q->Next())
			if (qidpathset.IsIn(q->Node()->GetIdPath()))
				InsertPtr(new XNode(*q->Node()));
	default : break;															// Avoids warning
	}
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::Reduce()
{
//ici on fusionen les infos des xnodes qui ont le m�me id les types sont concat�n�es et les distance on prend le min
XNode *xn;
vector<int>::iterator it;
int i, t;

	if (GetNb())
		for (Start();!End(); Next() ){
			xn = GetPtr(Node());
			//si oui on rajoute les types
			if ((xn) && !(xn==Node())) {
				i=0;
				if (xn->distance > Node()->distance) 					
					xn->distance = Node()->distance;
// 				for (it = Node()->types.begin(); it < Node()->types.end();  it++){
// 					t = Node()->types[i];
// 					xn->types.push_back(t); 
// 					i++;
// 				}
				DeletePtr(*Node());
				Prev();
			}
			
		}
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::Start()
{
	currpos = 0;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::Next()
{
	currpos++;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XNodeSet::End()
{
	return currpos == GetNb();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::NextId()
{
	WatchId();
	for (currpos++; !End(); currpos++)
		if (currid != (*this)[currpos]->GetIdNode())
			break;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::Last()
{
	currpos = GetNb() - 1;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::Prev()
{
	currpos--;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XNodeSet::Begin()
{
	return currpos == -1;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNodeSet::PrevId()
{
	WatchId();
	for (currpos--; !Begin(); currpos--)
		if (currid != (*this)[currpos]->GetIdNode())
			break;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
inline void XNodeSet::WatchId()
{
	currid = (*this)[currpos]->GetIdNode();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XNodeSet::IdChanged()
{
	if (!Begin() && !End())
		if (currid == (*this)[currpos]->GetIdNode())
			return false;
	return true;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XNode *XNodeSet::Node()
{
	return (*this)[currpos];
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSet::Compare(const int &_idfile) const
{
	return idfile - _idfile;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNodeSet::Compare(const XNodeSet &xns) const
{
	return idfile - xns.idfile;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XNodeSet::operator == (const XNodeSet &xns) const
{
	return idfile == xns.idfile;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XNodeSet::operator < (const XNodeSet &xns) const
{
	return idfile < xns.idfile;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XNodeSet::~XNodeSet()
{
	__DEL__(XNS, idfile);
}
