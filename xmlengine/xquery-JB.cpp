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

#include "xquery.h"
#include "xanalysexmlfile.h"
#include "xstuff.h"

XQuery::XQuery(GALILEI::GStorage *_storage) :
_OPEN("("), _CLOSE(")"), _OR("OR"), _AND("AND"), _INC("INC"), _SIB("SIB"), _NAND("NAND"),
/*_NINC("NINC"), _NSIB("NSIB"),*/_NOT("NOT"), query_tree(50, 25), query_result(40, 20)
{
	storage = _storage;
	XNode::ResetPathRefs();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XConst::tOper XQuery::get_op(const RString &s, const bool negation)
{
	RString sup(s.ToUpper());
	if (sup == _OR)
		return XConst::OR;
	if (sup == _AND)
		return negation ? XConst::NAND : XConst::AND;
	if (sup == _INC)
		return negation ? XConst::NINC : XConst::INC;
	if (sup == _SIB)
		return negation ? XConst::NSIB : XConst::SIB;
	if (sup == _NOT)
		return XConst::NOT;
	if (sup == _NAND)
		return XConst::NAND;
	return XConst::NOP;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XConst::tOper XQuery::get_opnot(const XConst::tOper op)
{
	if (op == XConst::INC)
		return XConst::NINC;
	if (op == XConst::SIB)
		return XConst::NSIB;
	return XConst::NAND;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XQuery::analyse_query(const RContainer<RString, false, false> &query_list_noalloc, XQueryNode *parent)
{
	int count, sleft, eleft, sright, pos;
	RContainer<RString, false, false> left(query_list_noalloc.GetNb()), right(query_list_noalloc.GetNb());
	RCursor<RString> curs;
	XConst::tOper op;
	XQueryNode *child = 0;
	bool isneg, leftnright, testl, testr;
	
	pos = query_list_noalloc.GetNb();											// Goes to the end of the container
	if (!pos)																	// If container is empty
		return false;															// Sends error
	else if (pos == 1)															// If there is only one word
		if (*query_list_noalloc[0] == _OPEN || *query_list_noalloc[0] == _CLOSE || get_op(*query_list_noalloc[0]) != XConst::NOP)
			return false;														// If the word is ( ) or oper : error
		else																	// Otherwise it is a word
			query_tree.InsertNode(parent, new XQueryNode(new RString (*query_list_noalloc[0])));
	else																		// Otherwise, container has many words
	{
		 
		leftnright = false;														// Set initial values
		eleft = -1;
		sleft = 0;
		sright = pos;
		curs.Set(query_list_noalloc);
		curs.GoTo(--pos);														// Begin by the end of container !!!
		if (get_op(*curs()) != XConst::NOP)										// Removes last operators if any
		{																		//  e.g. "kwrd1 AND kwrd2 AND OR"
			while (get_op(*curs()) != XConst::NOP && pos > 0)
				curs.GoTo(--pos);
			eleft = pos;														// Reruns analysis without last operators
			child = parent;														//  e.g. "kwrd1 AND kwrd2"
		}
		else if (_CLOSE == *curs())												// If closing ')' encountered
		{
			for (pos--, count = 1; pos > -1 && count; pos--)					// Search the matching opening '('
			{
				curs.GoTo(pos);
				if (_CLOSE == *curs())											// If opening parenthesis
					count++;													// Increase counter
				else if (_OPEN == *curs())										// If closing parenthesis
					count--;													// Decrease counter
			}
			pos++;
			if (!pos || count)													// Either whole expr between '(' ')'
			{																	//  e.g. "(kwrd1 AND kwrd2)"
				sleft = (!count);												//  or no opening '(' (count > 0)
				eleft = query_list_noalloc.GetNb() - 2;							//  e.g. "kwrd1 AND kwrd2)"
				child = parent;													// So reruns analysis without '(' ')'
			}																	//  e.g. "kwrd1 AND kwrd2"
		}
		if (eleft == -1)														// If eleft has initial value, no rerun
		{
			leftnright = true;													// Supposes having "kwrd1 AND kwrd2"
			sright = pos;														// Begin of right expression
			isneg = false;
			curs.GoTo(--pos);
			while (get_op(*curs()) == XConst::NOT)								// Looks for operator NOT
			{																	// Caution : "NOT NOT" = "NOT" !!!
				isneg = true;
				if (!pos)														// Avoids expression like "NOT kwrd"
					return false;
				curs.GoTo(--pos);
			}
			op = get_op(*curs(), isneg);										// An operator is supposed here
			eleft = pos;														// Set end of left expression
			if (op != XConst::NOP)												// If operator found, the end of lext expr
				eleft--;														//  must be readjusted before the operator
			else																// If no operator found, the AND is implicitly
				op = get_op(_AND, isneg);										//  assigned
			child = new XQueryNode(op);											// Create a tree node with the operator
			query_tree.InsertNode(parent, child);								// Put the node as child of parameter *parent
		}
		for (curs.Start(), count = 0; !curs.End(); curs.Next(), count++)		// Create two subcontainers, one for left expr
			if (count >= sleft && count <= eleft)								//  and one for the right one
				left.InsertPtr(curs());
			else if (count >= sright)
				right.InsertPtr(curs());
		testl = analyse_query(left, child);										// Analyse left
		if (leftnright)															// If the analysis of 2 expr. Indeed, when parenthesis,
			testr = analyse_query(right, child);								//  there is only left expr e.g. "(kwrd)" => "kwrd"
		if (!testl || (leftnright && !testr))									// If error occured
		{
			if (testl || (leftnright && testr))									// If only in one expr => there is only one child
			{																	//  that will be pushed instead of the parent
				query_tree.InsertNode(parent, new XQueryNode((*child)[0]));		//  e.g. "( AND kwrd" => "kwrd"
				query_tree.DeleteNode((*child)[0]);
				query_tree.DeleteNode(child);
			}
			else																// If both expr are false, do nothing
			{
				query_tree.DeleteNode(child);
				return false;													
			}
		}
	}
	return true;																// Returns success
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryRes *XQuery::get_xqueryres(const RString &word)							// Constructs an XQueryRes from a keyword
{
	XQueryRes *qres;
	XNodeSet *nset;
	XNode *xnd;
	XConst::tElem type;
	RString path;
	RContainer<RString, true, false> splitpath(10);
	RCursor<RString> cspath;
	int pos, nextpos, realpos, idnode, idfile;
	XNodeSQL nodesql;
	
	qres = new XQueryRes;
//FIXME	path = storage->GetStringNodes(word);										// Get string containing paths for word
	cmdtag = new GALILEI::GStorageTag("GetStringNodes");
	cmdtag->InsertAttr("word", word);
	storage->ExecuteCmd(*cmdtag, &path);
	delete cmdtag;

	path.Split(splitpath, '#');													// A '#' delimits nodes belonging to same file
	cspath.Set(splitpath);														//  the format is "#f1:n1;n2#f2:m1;m2..."
	for (cspath.Start(); !cspath.End(); cspath.Next())							//  NOTE : the substrings fk:n1;n2... are ordered
	{																			//  along fk !
		pos = cspath()->Find(':', 0);											// Searchs the ':' : the number before is the
		idfile = atoi(cspath()->Mid(0, pos).Latin1());							//  idfile
		nset = new XNodeSet(idfile);											// Will be desallocated by XQueryRes
		pos++;																	// Gets position after the ':' i.e. 1st idnode
		do
		{
			nextpos = cspath()->Find(';', pos);									// Gets the next ';'. If not exists, sets realpos to
			realpos = nextpos != -1 ? nextpos : cspath()->GetLen();				//  the length of cspath() (ie the '\0' after digits)
			idnode = atoi(cspath()->Mid(pos, realpos - pos).Latin1());			// Gets idnode between the ';' (or ':') and ';' (or '\0')
//FIXME			storage->GetNodeAttr(idfile, idnode, &nodename, &idparent, &ch1, 0, 0, 0);
			cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
			cmdtag->InsertAttr("idfile", RString::Number(idfile));
			cmdtag->InsertAttr("idnode", RString::Number(idnode));
			storage->ExecuteCmd(*cmdtag, &nodesql);
			delete cmdtag;
			// Gets the type of node
			if (nodesql.GetName() == word)										// If node name = keyword, then the keyword is either
				if (nodesql.GetChild1() == 0)									//  an attribute or a tag. The distinction is made
					type = XConst::ATTNAM;										//  by looking the ch1 value : it is set to 0 for
				else															//  the attributes
					type = XConst::TAGNAM;
			else																// Same as above bit with the content/value
				if (nodesql.GetChild1() == 0)
					type = XConst::ATTVAL;
				else
					type = XConst::TAGVAL;
			if (type == XConst::ATTNAM || type == XConst::ATTVAL)				// If attribute, then get the id of the node
				idnode = nodesql.GetParent();									//  that represent the tag containing the attr
			nset->InsertPtr(new XNode(idnode, type));							// Desallocated by XNodeSet
			pos = realpos + 1;													// Position after the ';'
		} while (nextpos != -1);												// While there is a ';'
		// Adds the parents
		for (pos = 0; pos < nset->GetNb(); pos++)								// DO NOT use Start() and Next() because we
		{																		//  add element in pset during the for loop
			xnd = (*nset)[nset->GetNb() - 1 - pos];								// Begins with the END! of the container
			if (xnd->GetIdNode())												// If not root node (id = 0, hasn't any parent)
			{
//FIXME				storage->GetNodeAttr(idfile, xnd->GetIdNode(), 0, &idparent, 0, 0, 0, 0);
				cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
				cmdtag->InsertAttr("idfile", RString::Number(idfile));
				cmdtag->InsertAttr("idnode", RString::Number(xnd->GetIdNode()));
				storage->ExecuteCmd(*cmdtag, &nodesql);
				delete cmdtag;
				nset->PushUp(nodesql.GetParent(), *xnd);
				//cout << "noeud rajouté " << RString::Number(nodesql.GetParent()) << endl;
			}
		}
		qres->InsertPtr(nset);													// Add XNodeSet in the XQueryRes
	}
	return qres;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQuery::compute_query(XQueryNode *node)									// Resolves the query tree
{
	XQueryRes *qres;
	
	switch (node->GetType())													// Selecting type of node
	{
	case XQueryNode::STRING :													// Node type : String
		node->Set(get_xqueryres(node->GetWord()));								// Find the nodes in MySQL where the word occurs
		break;																	//  and add it as a XQueryRes
	case XQueryNode::OPERATOR :													// Operator
		qres = new XQueryRes;													// Create empty set (filled according to the oper)
		compute_query((*node)[0]);												// Compute left part of operator
		if (node->GetOp() == XConst::OR || (*node)[0]->GetQRes()->GetNb())
		{
			compute_query((*node)[1]);											// Compute right part and compute union
			qres->Oper(node->GetOp(), (*node)[0]->GetQRes(), (*node)[1]->GetQRes());
		}
		else
			(*node)[1]->Set(new XQueryRes);										//  then intersection is empty too
		node->Set(qres);														// Add the XQueryRes to current node
	default : break;															// Avoids warning
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQuery::extract_result()
{
	RCursor<XNodeSet> cnset;
	RCursor<XNode> cnode;
	RCursor<XResult> c1, c2;
	RContainer<XNodeSQL, true, true> contentfilerefs(20);
	RCursor<XNodeSQL> cnodesql;
	RString snippet, url, attributes, content, title;
	RIOFile *readingfile;
	XNodeSet rootset;
	XAnalyseXMLFile *analysexml;
	XNodeSQL nodesql;
	unsigned int idfile;
	int imax, max, rank;
	unsigned int count_ch;
//	unsigned int fpos, flen, ch1, nbch;
	bool sync;

	cnset.Set(*query_tree.GetTop()->GetQRes());							// Get the top node of the query tree since
	for (cnset.Start(); !cnset.End(); cnset.Next())								//  it contains matching documents
	{																			// Go through each document
		rootset.Clear();														// Resets container that will keep every XNode
		cnset()->GetSetIdNode(0, &rootset);										//  refering to the root node of XML doc
		if (!rootset.GetNb())
			continue;
		cnode.Set(rootset);
		idfile = cnset()->GetIdFile();											// Takes references of matched file
//FIXME		url = storage->GetUrlFile(idfile);
		cmdtag = new GALILEI::GStorageTag("GetUrlFile");
		cmdtag->InsertAttr("idfile", RString::Number(idfile));
		storage->ExecuteCmd(*cmdtag, &url);
		delete cmdtag;
		readingfile = new RIOFile(url);
		try
		{
			readingfile->Open(RIO::Read);
		}
		catch (...)
		{
		}
		if (readingfile->IsOpen())
		{
			analysexml = new XAnalyseXMLFile(storage, url);						// Verifies if the document is synchronized with
			sync = analysexml->IsFileSync();									//  the time it has been analyzed
			delete analysexml;
		}
		snippet = "";
		if (readingfile->IsOpen() && sync)										// If everything ok
		{
			contentfilerefs.Clear();
			rank = 0;
			for (cnode.Start(); !cnode.End(); cnode.Next())
			{
				rank += cnode()->ComputeRank();
//FIXME				storage->GetNodeAttr(idfile, cnode()->GetDeepestId(), &name, 0, &ch1, &nbch, &fpos, &flen);
				cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
				cmdtag->InsertAttr("idfile", RString::Number(idfile));
				cmdtag->InsertAttr("idnode", RString::Number(cnode()->GetDeepestId()));
				storage->ExecuteCmd(*cmdtag, &nodesql);
				delete cmdtag;
				contentfilerefs.InsertPtr(new XNodeSQL(nodesql));
//FIXME				contentfilerefs.InsertPtr(new XNodeSQL(0, cnode()->GetDeepestId(), ch1, nbch, 0, fpos, flen, name));
			}
			cnodesql.Set(contentfilerefs);
			for (cnodesql.Start(); !cnodesql.End(); cnodesql.Next())			// For each reference in a file
			{																	//  begins the read procedure
				content = "";
				attributes = "";
				if (cnodesql()->GetNbChild())									// Looks for attrubtes
				{
					count_ch = 0;												// Reset the counter to children
					do
					{
//FIXME						storage->GetNodeAttr(idfile, cnodesql()->GetChild1() + nbch, &name, 0, &ch1, 0, &fpos, &flen);
						cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
						cmdtag->InsertAttr("idfile", RString::Number(idfile));
						cmdtag->InsertAttr("idnode", RString::Number(cnodesql()->GetChild1() + count_ch));//TODO nbch??? FIXED
						storage->ExecuteCmd(*cmdtag, &nodesql);
						delete cmdtag;
						if (!nodesql.GetChild1())								// If current child is attribute
						{														// Retrieve its value
							char *buff = new char[nodesql.GetLen() + 1];
							readingfile->Seek(nodesql.GetPos());
							readingfile->Read(buff, nodesql.GetLen());
							buff[nodesql.GetLen()] = '\0';
							attributes += " " + nodesql.GetName() + " = \"" + buff + "\"";
							delete [] buff;
							count_ch++;
						}
					} while (!nodesql.GetChild1() && count_ch < cnodesql()->GetNbChild());
				}																// Until no more attribute or no more children
				if (cnodesql()->GetLen())										// If tag has a content
				{
					char *buff = new char[cnodesql()->GetLen() + 1];
					readingfile->Seek(cnodesql()->GetPos());
					readingfile->Read(buff, cnodesql()->GetLen());
					buff[cnodesql()->GetLen()] = '\0';
					content = "    " + RString(buff) + "\n";
					delete [] buff;
				}
//				if (content.GetLen() || attributes.GetLen())
//				{
				if (snippet.GetLen())
					snippet += "\n";
				snippet += "<" + cnodesql()->GetName() + attributes + ">\n";
				snippet += content;
				snippet += "</" + cnodesql()->GetName() + ">";
//				}
			}
			RString tmp;
			tmp = gettagcontent(idfile, readingfile, "title");
			if (tmp != "")
				title = tmp;
			else
				title = "No title for now!";
			tmp = gettagcontent(idfile, readingfile, "description");
			if (tmp != "")
				snippet = tmp;
			tmp = gettagcontent(idfile, readingfile, "url");
			if (tmp != "")
				url = tmp;
			
		}
		else
		{
			if (sync)
				snippet = "File doesn't exist anymore! Please update the database to view result for this file.";
			else
				snippet = "File changed! Please update the database to view result for this file.";
		}
		delete readingfile;
		query_result.InsertPtr(new XResult(idfile, title, url, snippet, rank));
	}
	c1.Set(query_result);
	c2.Set(query_result);
	c1.Start();
	c2.Start();
	for (; !c1.End(); c1.Next())
	{
		imax = c1.GetPos();
		max = c1()->GetScore();
		for (c2.GoTo(c1.GetPos()); !c2.End(); c2.Next())
		{
			if (max < c2()->GetScore())
			{
				imax = c2.GetPos();
				max = c2()->GetScore();
			}
		}
		if (imax != c1.GetPos())
			query_result.Exchange(c1.GetPos(), imax);
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RCursor<XResult> XQuery::Query(const RTree<XQueryNode, true, false> &keywordtree)
{
	__OPEN__();																	// DEBUG!
	RCursor<XResult> res;														// Cursor to the results of the query
	
	query_tree.Copy(keywordtree);
	cout << print().Latin1() << endl;
	compute_query(query_tree.GetTop());
	cout << ShowSol().Latin1() << endl;
	extract_result();
	res.Set(query_result);
	return res;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RCursor<XResult> XQuery::Query(const RContainer<RString, true, false> &keywordlist)
{
	__OPEN__();																	// DEBUG!
	RCursor<XResult> res;														// Cursor to the results of the query
	RContainer<RString, false, false> keywordlist_noalloc(keywordlist);
	bool test;
			
	test = analyse_query(keywordlist_noalloc);
	if (!test)
		cout << "ERROR !!!!!!"<<endl;
	else
	{
	    cout << "la requête dans xquery::query " ;
 		cout << print().Latin1() << endl;
		compute_query(query_tree.GetTop());
		cout << ShowSol().Latin1() << endl;
		extract_result();
		res.Set(query_result);
	}
	return res;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XQuery::gettagcontent(int idfile, RIOFile *file, RString tag)
{
	vector<int> vint;
	int size;
	RString res;
	XNodeSQL nodesql;
	
//FIXME	storage->GetIdNodes(idfile, tag, vint, &size);
	cmdtag = new GALILEI::GStorageTag("GetIdNodes");
	cmdtag->InsertAttr("idfile", RString::Number(idfile));
	cmdtag->InsertAttr("name", tag);
	storage->ExecuteCmd(*cmdtag, &vint);
	delete cmdtag;
	if (vint.size() == 1)
	{
//FIXME		storage->GetNodeAttr(idfile, vint[0], 0, 0, 0, 0, &fpos, &flen);
		cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
		cmdtag->InsertAttr("idfile", RString::Number(idfile));
		cmdtag->InsertAttr("idnode", RString::Number(vint[0]));
		storage->ExecuteCmd(*cmdtag, &nodesql);
		delete cmdtag;
		char *buff = new char[nodesql.GetLen() + 1];
		file->Seek(nodesql.GetPos());
		file->Read(buff, nodesql.GetLen());
		buff[nodesql.GetLen()] = '\0';
		res = RString(buff);
		delete [] buff;
	}
	return res;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQuery::~XQuery()
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XQuery::print() const
{
	XQueryNode *tag, *tmpqn;
	XFifo<XQueryNode> fifo;
	RCursor<XQueryNode> curs;
	RContainer<XQueryNode, true, false> tmp(10);
	RString s;
	int i;
	
	fifo.Push(query_tree.GetTop());										// Put the first node in Fifo
	while (fifo.GetNb())														// While there are nodes from Fifo
	{
		tag = fifo.PopFirst();
		curs = tag->GetNodes();
		for (i = 0, curs.Start(); !curs.End(); i++, curs.Next())
			fifo.Push(curs());
		if (i == 1)
		{
			tmpqn = new XQueryNode(new RString("?"));
			tmp.InsertPtr(tmpqn);
			fifo.Push(tmpqn);
		}
		else if (i > 2)
		{
			cout << "ERROR!" << endl;
		}
		if (tag->GetType() == XQueryNode::OPERATOR)
		{
			if (tag->GetOp() == XConst::NOT)
				s += " NOT ";
			if (tag->GetOp() == XConst::OR)
				s += " OR ";
			if (tag->GetOp() == XConst::AND)
				s += " AND ";
			if (tag->GetOp() == XConst::INC)
				s += " INC ";
			if (tag->GetOp() == XConst::SIB)
				s += " SIB ";
			if (tag->GetOp() == XConst::NAND)
				s += " NAND ";
			if (tag->GetOp() == XConst::NINC)
				s += " NINC ";
			if (tag->GetOp() == XConst::NSIB)
				s += " NSIB ";
		}
		else
			s += " \"" + tag->GetWord() + "\" ";
	}
	s += "\n";
	return s;
}
	
//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XQuery::ShowSol() const
{
	RCursor<XNode> cnset;
	RCursor<XNodeSet> cqres;
	RString res, file;
	unsigned int idfile;
	
	cqres.Set(*query_tree.GetTop()->GetQRes());
	for (cqres.Start(); !cqres.End(); cqres.Next())
	{
		idfile = cqres()->GetIdFile();
		cnset.Set(*cqres());
		if (cnset.GetNb())
			res += "file : " + RString::Number(idfile) + "\n";
		for (cnset.Start(); !cnset.End(); cnset.Next())
		{
			res += cnset()->print() + " score = " + RString::Number(cnset()->ComputeRank()) + "\n";
		}
	}
	return res;
}
