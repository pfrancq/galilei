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

#include "xnode.h"

int XNode::path_refs = 0;

//______________________________________________________________________________
//------------------------------------------------------------------------------
XNode::XNode()
{
__NEW__(XND, -1);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XNode::XNode(const int _idnode, const XConst::tElem _type, const int _dis, const int _specif)
{
	idnode = _idnode;
//	type = _type;
	distance = _dis;
	specificity = _specif;
	idpath = path_refs++;
	depth = 0;
	deepestid = idnode;
	last = true;
//	types.push_back(TransfType());
__NEW__(XND, _idnode);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XNode::XNode(const XNode &node)
{
	idnode = node.idnode;
	idpath = node.idpath;
//	type = node.type;
	distance = node.distance;
	specificity = node.specificity;
	depth = node.depth;
	deepestid = node.deepestid;
	last = node.last;
//	types=node.types;
__NEW__(XND, idnode, idpath);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNode::ResetPathRefs()
{
	path_refs = 0;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::GetIdNode() const
{
	return idnode;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::GetIdPath() const
{
	return idpath;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XConst::tElem XNode::GetType() const
{
	return type;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::GetDepth() const
{
	return depth;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::GetDeepestId() const
{
	return deepestid;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XNode::IsLast() const
{
	return last;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNode::SetLast(bool _last)
{
	last = _last;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNode::PushUp(const int _idnode)
{
	idnode = _idnode;
	depth++;
	last = false;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::ComputeRank() const
{
//faiza � revoir pour calculer le score d'un noeud	
int res = 0;
	
	switch (type)
	{
	case XConst::ATTNAM :
		res += 1;
		break;
	case XConst::ATTVAL :
		res += 2;
		break;
	case XConst::TAGNAM :
		res += 4;
		break;
	case XConst::TAGVAL :
		res += 3;
		break;
	default : break;															// Avoids warning
	}
	res -= depth;
	return res;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::TransfType()
{
	
	switch (type)
	{
	case XConst::ATTNAM :
		return 2;
		break;
	case XConst::ATTVAL :
		return 3;
		break;
	case XConst::TAGNAM :
		return 1;
		break;
	case XConst::TAGVAL :
		return 4;
		break;
	default : 
		return 0 ;
		break;															// Avoids warning
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::Compare(const XNode &xnd) const
{
	if (idnode == xnd.idnode)
		return idpath - xnd.idpath;
	return idnode - xnd.idnode;
}
//Faiza : compare deux noeuds en comparant seulement le idnode
int XNode::Compare(const XNode *xnd) const
{
	if (idnode == xnd->idnode)
		return 0;
	return idnode - xnd->idnode;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::Type()
{


int t =0;	
int j = 0 ;
int i = 0 ;

if (types.size()==1) t +=1;
else
for (i=0; i<types.size()-1; i++)
	for (j=i+1; j<types.size(); j++)
		if (types[i]+types[j] == 5)
			t+= 3;
		else
			if (types[i]==types[j])
				t+= 2;
			else t += 1;
return t;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::Distance()
{
return distance;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
int XNode::Specificity()
{
return specificity;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNode::Compute_Promethee_Criteria(double tf_idf, double tf_ief){
	
	CritDocSc= tf_idf;
	//CritType = Type();
	CritDis= Distance();
//	CritOcc=0;
	CritSpecif =Specificity();
	CritTfief =  tf_ief;
//cout << "faizaaaaaaaaaaaaa" << " tf_idf " << CritDocSc << " type " << CritType << " dis " << CritDis << " specif " << CritSpecif << endl;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XNode::operator == (const XNode &xs) const
{
	return idnode == xs.idnode;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XNode::operator = (const XNode &xs) const
{
	return ( idnode == xs.idnode && idpath == xs.idpath);
	
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XNode::operator < (const XNode &xs) const
{
	return idnode < xs.idnode;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XNode::print() {
vector<int>::iterator it;

	char str[20] = "";
	sprintf(str, "%f", GetRank());
	return RString(str);
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XNode::print2() {
vector<int>::iterator it;

char str[20], str2[20]  = "";
sprintf(str, "%f", CritDocSc);
sprintf(str2, "%f", CritTfief);	

return  RString(str)+" "+RString(str2)+" "+" "+RString::Number(CritDis)+" "+RString::Number(CritSpecif);
	
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
double XNode::GetRank() {
	return Rank;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
void XNode::SetRank(double _Rank) {
	Rank = _Rank;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XNode::~ XNode()
{
__DEL__(XND, idnode, idpath);
}
