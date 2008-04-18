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

#ifndef __XQUERYH__								// Avoids multiple declarations
#define __XQUERYH__

#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;

#include <rstring.h>
#include <rtree.h>
#include <rcursor.h>
#include <rxmlfile.h>
using namespace R;

#include <gstorage.h>

#include "xgeneral.h"
#include "xquerynode.h"
#include "xwordsql.h"
#include "xnodesql.h"
#include "xresult.h"
#include "xqueryres.h"
#include "xfifo.h"			// Used by print()

#include "gxmlranking.h"
#include "gxmlir.h"
#include "glang.h"

class XQuery
{

RContainer<RString,  false, false> query_keywords;
private :

	const RString _OPEN, _CLOSE, _OR, _AND, _INC, _SIB, _NAND, /*_NINC, _NSIB, */_NOT, _QUOTE;
	RTree<XQueryNode, true, false> query_tree;
//	RTree<XQueryNode, true, false> *query_tree2;
	RContainer<XResult, true, false> query_result;
	GALILEI::GStorage *storage;
	GALILEI::GStorageTag *cmdtag;


	XConst::tOper get_op(const RString &s, const bool negation = false);
	XConst::tOper get_opnot(const XConst::tOper op);
	bool analyse_query(const RContainer<RString, false, false> &query_list, XQueryNode *parent = 0);
	XQueryRes *get_xqueryres(const RString &word);
	void compute_query(XQueryNode *node);
	void extract_result();
	void find_deepest_node(const int &idfile, const int &idancestor, XQueryNode &currnode, vector<int> *nodelist) const;
	RString gettagcontent(int idfile, RIOFile *file, RString tag);
	int calculate_specificity(int idfile, int idnode); //calcule la specificte d'un noeud
	void extract_query_keywords();
	
public :
	GALILEI::GLang* Lang;

	GALILEI::GXmlParams Params;
	XQuery(GALILEI::GStorage *_storage, GALILEI::GXmlParams p);
	RCursor<XResult> Query(const RContainer<RString, true, false> &keywordlist, R::RString Name);
	//RCursor<XResult> Query(const RTree<XQueryNode, true, false> &keywordtree);
	~XQuery();
	RString print() const;
	RString ShowSol() ;
	void rank_results(R::RString Name);
	//void temporaire();
};

#endif
