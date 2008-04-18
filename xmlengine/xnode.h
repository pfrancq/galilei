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

#ifndef __XNODEH__							// Avoids multiple declarations
#define __XNODEH__

#include <rcontainer.h>
#include <rcursor.h>
using namespace R;

#include "xgeneral.h"

#include "xwordsql.h"
#include "xnodesql.h"
#include <gstorage.h>

class XNode
{
private :
	
	int idnode;
	int idpath;
	XConst::tElem type;
	int depth;
	int deepestid;
	bool last;
	double Rank;
	static int path_refs;

public :
	
	vector<int> types; //faiza : type of the nodes  used only when computing a query
	int distance; //faiza : distance between this node and the node where the word occurs
	int specificity; //faiza : specificity of an xnode = number of nodes it contains

	XNode();
	XNode(const int _idnode, const XConst::tElem _type);
	XNode(const int _idnode, const XConst::tElem _type, const int _dis, const int _specif);
	XNode(const XNode &node);
	static void ResetPathRefs();
	int GetIdNode() const;
	int GetIdPath() const;
	XConst::tElem GetType() const;
	int GetDepth() const;
	int GetDeepestId() const;
	bool IsLast() const;
	void SetLast(bool _last);
	void PushUp(const int _idnode);
	int TransfType();
	double GetRank();
	void SetRank(double _Rank);
	/**
	* Return Type criterion value.
	*/
	int Type();
	/**
	* Return Distance criterion value.
	*/
	int Distance();
	/**
	* Return Specificity riterion value.
	*/
	int Specificity();
	/**
	* Value of the "Document Score" criterion.
	*/
	double CritDocSc;
	
	/**
	* Value of the "Type" criterion.
	double CritType;*/

	/**
	* Value of the "Type and Distance" criterion.
	*/
	double CritDis;

	/**
	* Value of the criterion "Occurrences".
	*
	double CritOcc;*/

	/**
	* Value of the criterion "Specificity".
	*/
	double CritSpecif;

	/**
	* Value of the criterion "Tfief".
	**/
	double CritTfief;

	
	
	int ComputeRank() const;


	int Compare(const XNode &xnd) const;
	int Compare(const XNode *xnd) const;
	int Compare(const int _idnode) const;
	void Compute_Promethee_Criteria(double tf_idf, double tf_ief);
	bool operator == (const XNode &xn) const;
	bool operator = (const XNode &xn) const;
	bool operator < (const XNode &xn) const;
	~XNode();
	RString print();
	RString print2();
};

#endif
