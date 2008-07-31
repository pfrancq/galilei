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
// include files for ANSI C/C++
#include <stdlib.h>
#include <iostream.h>


#include "xquery.h"
#include "xanalysexmlfile.h"
#include "xstuff.h"
#include "sstream" 
#include "gxmlranking.h"

using namespace GALILEI;

XQuery::XQuery(GALILEI::GStorage *_storage, GXmlParams p ):
_OPEN("("), _CLOSE(")"), _OR("OR"), _AND("AND"), _INC("INC"), _SIB("SIB"), _NAND("NAND"),_NOT("NOT"), query_tree(50, 25), query_result(40, 20), query_keywords(30,15)
{
	storage = _storage;
	XNode::ResetPathRefs();
	Params = p;
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
	Lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn("en");
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
			query_tree.InsertNode(parent, new XQueryNode(new RString (Lang->GetStemming(*query_list_noalloc[0]))));
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
int XQuery::calculate_specificity(int idfile, int idnode)
{
int specif = 0;

/////// Si la BD contient le champ specif dans la table nodelist alors utiliser ce code pour r�cup�rer la valeur de ce champ
/*	cmdtag = new GALILEI::GStorageTag("GetSpecif");
	cmdtag->InsertAttr("idfile", RString::Number(idfile));
	cmdtag->InsertAttr("idnode", RString::Number(idnode));
	storage->ExecuteCmd(*cmdtag, &specif);
	delete cmdtag;
	return specif;*/

/////// Sinon le calculer avec le code ci dessous
 XNodeSQL nodesql;
 bool leaf = false;
 int i, nbchildren = 0;
 	cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
 	cmdtag->InsertAttr("idfile", RString::Number(idfile));
 	cmdtag->InsertAttr("idnode", RString::Number(idnode));
 	storage->ExecuteCmd(*cmdtag, &nodesql);
 	delete cmdtag;
 	if (nodesql.GetNbChild()==0) nbchildren = 1;
 	else
 	for (i=nodesql.GetChild1(); i< nodesql.GetChild1()+nodesql.GetNbChild(); i++)
 		 		nbchildren +=  calculate_specificity(idfile, i);
return nbchildren;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XQueryRes *XQuery::get_xqueryres(const RString &word)							// Constructs an XQueryRes from a keyword
{
	XQueryRes *qres;
	XNodeSet *nset;
	XNode *xnd;
	XConst::tElem type;
	RString localisations;
	RContainer<RString, true, false> splitpath(10);
	vector<int> localisationsList;
	int pos, idnode, idnode2, idfile, dis, specif;
	XNodeSQL nodesql;
	bool root;

	qres = new XQueryRes;
//FIXME	path = storage->GetStringNodes(word);									
	cmdtag = new GALILEI::GStorageTag("GetLocalisations");
	cmdtag->InsertAttr("word", word);
	storage->ExecuteCmd(*cmdtag, &localisations);
	
	delete cmdtag;
	cmdtag = new GALILEI::GStorageTag("GetLocalisationsList");
	cmdtag->InsertAttr("id", localisations);
	storage->ExecuteCmd(*cmdtag, &localisationsList);
	delete cmdtag;	
														// 
	for (int i=0; i<localisationsList.size(); i++)							//  localisationsList = idfile idnode idfile idnode
	{																			// 
		idfile = localisationsList[i];	

		nset = new XNodeSet(idfile);
		dis = 0 ; 
		do{
			i++;
			idnode = localisationsList[i];
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
			 if (type == XConst::ATTNAM || type == XConst::ATTVAL)				// If attribute, then get the id of the node  that represent the tag containing the attr
			     idnode = nodesql.GetParent();	
			specif = 1;															//specificty of the first element is 1
			xnd = new XNode(idnode, type, dis, specif ); 
			nset->InsertPtr(xnd);	 			// Desallocated by XNodeSet
			//add parents
			/*root = false;
			idnode2 = idnode;
			while (!root){
				cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
				cmdtag->InsertAttr("idfile", RString::Number(idfile));
				cmdtag->InsertAttr("idnode", RString::Number(idnode2));
				storage->ExecuteCmd(*cmdtag, &nodesql);
				delete cmdtag;
				dis++;
				specif =calculate_specificity(idfile,nodesql.GetParent()) ;
				nset->PushUp(nodesql.GetParent(), dis , specif,  *xnd);
				idnode2=nodesql.GetParent();
				if (idnode2==0) root=true;
			}
			dis=0;*/
			//end add parents
			i++;
				
		}while (idfile == localisationsList[i]);		
		nset->Reduce();
		qres->InsertPtr(nset);		
		i--;											// Add XNodeSet in the XQueryRes
	}
	return qres;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQuery::compute_query(XQueryNode *node)									// Resolves the query tree
{
	XQueryRes *qres;
	//Lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn("en");
	RString stem;

	switch (node->GetType())													// Selecting type of node
	{
	case XQueryNode::STRING :													// Node type : String
		//stem = Lang->GetStemming(node->GetWord().ToLower());										
		node->Set(get_xqueryres(node->GetWord()));								// Find the nodes in MySQL where the word occurs
		break;																	//  and add it as a XQueryRes
	case XQueryNode::OPERATOR :										// 	if (GetNb())
// 		for (Start();!End(); Next() ){
// 			xn = Node();
// 			xn = GetPtr(Node());
// 			//si oui on rajoute les types
// 			if ((xn) && !(xn==Node())) {
// 				i=0;
// 				for (it = Node()->types.begin(); it < Node()->types.end();  it++)
// 					xn->types.push_back(Node()->types[i]); 
// 				DeletePtr(Node());
// 				Prev();
// 			} 
// 	}			// Operator
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
//NORMAL//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQuery::rank_results(R::RString Name)									
{
	RCursor<XNodeSet> cqres;
	GXmlRanking *gxml_rank;


	cqres.Set(*query_tree.GetTop()->GetQRes());
	gxml_rank = new GXmlRanking(storage, Params);
	gxml_rank->Compute(cqres, query_keywords, Name);	
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
//void XQuery::rank_results(R::RString Name)									
//{
//	RCursor<XNodeSet> cqres;
//	GXmlRanking *gxml_rank;
//
//	gxml_rank = new GXmlRanking(storage, Params);
//	gxml_rank->Compute(Name);	
//}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XQuery::extract_result()
{
	RCursor<XNodeSet> cnset;
	RCursor<XNode> cnode;
	RCursor<XResult> c1, c2;
	RContainer<XNodeSQL, true, true> contentfilerefs(20);
	RCursor<XNodeSQL> cnodesql;
	RString snippet, url,url1, attributes, content, title;
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
		cnode.Clear();														// Resets container that will keep every XNode
		cnode.Set(*cnset());if (!cnode.GetNb())
			continue;
		idfile = cnset()->GetIdFile();											// Takes references of matched file
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
//  			analysexml = new XAnalyseXMLFile(storage, url);						// Verifies if the document is synchronized with
//  			sync = analysexml->IsFileSync();									//  the time it has been analyzed
//  			delete analysexml;
				sync = true;
		}
		
		if (readingfile->IsOpen() && sync)										// If everything ok
		{
			contentfilerefs.Clear();
			rank = 0;
			for (cnode.Start(); !cnode.End(); cnode.Next())   //les noeuds pertinents sont retir�s de la table sql nodelist  
			{
				//rank += cnode()->ComputeRank();
				cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
				cmdtag->InsertAttr("idfile", RString::Number(idfile));
				cmdtag->InsertAttr("idnode", RString::Number(cnode()->GetIdNode()));
				storage->ExecuteCmd(*cmdtag, &nodesql);
				delete cmdtag;
				contentfilerefs.InsertPtr(new XNodeSQL(nodesql));
//FIXME				contentfilerefs.InsertPtr(new XNodeSQL(0, cnode()->GetDeepestId(), ch1, nbch, 0, fpos, flen, name));
				//cout << "faiza cnode " << endl;
			}
			cnodesql.Set(contentfilerefs);
			int i =0;

			for (cnodesql.Start(); !cnodesql.End(); cnodesql.Next())			// For each reference in a file
			{																	//  begins the read procedure									
				i++;			
				content = "";
				attributes = "";
							char *buff = new char[cnodesql()->GetLen()+1];
							readingfile->Seek(cnodesql()->GetPos());
							readingfile->Read(buff, cnodesql()->GetLen());
				snippet = "    " + RString(buff) + "\n";
				delete [] buff;
				string str = ""; 								//used to convert i to string
				ostringstream ostr;
				ostr << i;
				str = ostr.str(); 
				url1 = url + str;						//differenciate url of the differents elements of the same document otherwise the  
														//metaengine shows only the first element because it does not replicate urls 
				//retreive Rank of the node
				cnode.GoTo(i-1);
				query_result.InsertPtr(new XResult(idfile, title, url1, snippet, cnode()->GetRank()));
				snippet = "";
			}
		} 
		else
		{
			if (sync)
				snippet = "File doesn't exist anymore! Please update the database to view result for this file.";
			else
				snippet = "File changed! Please update the database to view result for this file.";
		
		//cout << "faiza nbre de query_result insert " <<endl;
		query_result.InsertPtr(new XResult(idfile, title, url, snippet, 0));
		}
		delete readingfile;
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
// RCursor<XResult> XQuery::Query(const RTree<XQueryNode, true, false> &keywordtree)
// {
// 	__OPEN__();																	// DEBUG!
// 	RCursor<XResult> res;														// Cursor to the results of the query
// 	
// 	query_tree.Copy(keywordtree);
// 	cout << print().Latin1() << endl;
// 	compute_query(query_tree.GetTop());
// 	rank_results();
// 	cout << ShowSol().Latin1() << endl;
// 	extract_result();
// 	res.Set(query_result);
// 	return res;
// }

//______________________________________________________________________________
//------------------------------------------------------------------------------
// void XQuery::temporaire()
// {
// int nbnodes, specif, rien;
// 	for (int idfilet=122; idfilet<16810; idfilet++)
// 	{
// 	
// 		cmdtag = new GALILEI::GStorageTag("GetNbNodes");
// 		cmdtag->InsertAttr("idfile", RString::Number(idfilet));
// 		storage->ExecuteCmd(*cmdtag, &nbnodes);
// 		cout << " id file = " << idfilet << endl;
// 		delete cmdtag;
// 		for (int idnodet=0; idnodet<nbnodes; idnodet++)
// 		{
// 			specif = calculate_specificity(idfilet, idnodet);
// 			//cout << "specif de idfile "<< idfas ilet << " idnode " << idnodet << " specif " << specif << endl;
// 			cmdtag = new GALILEI::GStorageTag("FullSpecifField");
// 			cmdtag->InsertAttr("specif", RString::Number(specif));
// 			cmdtag->InsertAttr("idfile", RString::Number(idfilet));
// 			cmdtag->InsertAttr("idnode", RString::Number(idnodet));
// 			storage->ExecuteCmd(*cmdtag, &rien);
// 			delete cmdtag;
// 		}
// 			
// 	}
// }


RCursor<XResult> XQuery::Query(const RContainer<RString, true, false> &keywordlist, R::RString Name)
{
	__OPEN__();																	// DEBUG!
	RCursor<XResult> res;														// Cursor to the results of the query
	RContainer<RString, false, false> keywordlist_noalloc(keywordlist);
	bool test;
	R::RTextFile* resultsFile;	

	test = analyse_query(keywordlist_noalloc);
try
		{
			resultsFile=new RTextFile(Name);
			resultsFile->Open(RIO::Append);
		}
		catch(...)
		{
			resultsFile=0;
		}
////////////////////////////////
//temporaire();
////////////////////////////////

	extract_query_keywords(); // remplissage de quer_keywords
	if (!test)
		cout << "ERROR !!!!!!"<<endl;
	else
	{
	    //cout << "faiza la requ�te dans xquery::query "<<endl ;
 		//cout << print().Latin1() << endl;
		compute_query(query_tree.GetTop());
		//NORMAL
		 rank_results(Name);
		//rank_results(Name);
		if (resultsFile)
			(*resultsFile)<< ShowSol().Latin1() << "\n";
		else
			cout << "probl�me avec le fichier de sortie " << endl;
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
void XQuery::extract_query_keywords() 
{
	Lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn("en");
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
			
		}
		if (tag->GetType() != XQueryNode::OPERATOR)
		{
			query_keywords.InsertPtr(new RString(Lang->GetStemming(tag->GetWord().ToLower())));
		}
	}


}
//______________________________________________________________________________
//------------------------------------------------------------------------------
RString XQuery::ShowSol() 
{
	RCursor<XNode> cnset;
	RCursor<XNodeSet> cqres;
	RString resTotal, resFile, resNode, file;
	unsigned int idfile, idnode;
	XNodeSQL nodesql;	
	vector<int> brothersIds;
	cqres.Set(*query_tree.GetTop()->GetQRes());
	resTotal = "" ;
	resNode="";
	resFile ="";
	unsigned int i=0;
	for (cqres.Start(); !cqres.End(); cqres.Next())
	{
		idfile = cqres()->GetIdFile();

		cmdtag = new GALILEI::GStorageTag("GetUrlFile");
		cmdtag->InsertAttr("idfile", RString::Number(idfile));
		storage->ExecuteCmd(*cmdtag, &file);
		delete cmdtag;
		resFile= file+" ; " ;

		cnset.Set(*cqres());
		if (cnset.GetNb())
			for (cnset.Start(); !cnset.End(); cnset.Next())
			{
				
				resNode= "";			
		
				idnode = cnset()->GetIdNode();
				//De l�  : 
				cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
				cmdtag->InsertAttr("idfile", RString::Number(idfile));
				cmdtag->InsertAttr("idnode", RString::Number(idnode));
				storage->ExecuteCmd(*cmdtag, &nodesql);
				delete cmdtag;	
					//on cherche l'�ge du noeud
				cmdtag = new GALILEI::GStorageTag("GetIdNodes");
				cmdtag->InsertAttr("idfile", RString::Number(idfile));
				cmdtag->InsertAttr("name", nodesql.GetName());
				cmdtag->InsertAttr("parent", RString::Number(nodesql.GetParent()));
				storage->ExecuteCmd(*cmdtag, &brothersIds);
				delete cmdtag;
				for (i=0; brothersIds[i]!=nodesql.GetIdNode() ; i++){}
									
				resNode = nodesql.GetName()+"["+RString::Number(i+1)+"]/";
				brothersIds.clear();
					//fin recherche �ge d'un noeud. un elet sec a un age x si il est le x ieme fils qui s'appelle sec 

				if (idnode != 0 )
				{
					bool root = false;
					while (!root)
					{
						cmdtag = new GALILEI::GStorageTag("GetNodeAttr");
						cmdtag->InsertAttr("idfile", RString::Number(idfile));
						cmdtag->InsertAttr("idnode", RString::Number(nodesql.GetParent()));
						storage->ExecuteCmd(*cmdtag, &nodesql);
						delete cmdtag;	
						if (nodesql.GetIdNode() == 0 )
							root = true;
						//on cherche l'�ge du noeud
						cmdtag = new GALILEI::GStorageTag("GetIdNodes");
						cmdtag->InsertAttr("idfile", RString::Number(idfile));
						cmdtag->InsertAttr("name", nodesql.GetName());
						cmdtag->InsertAttr("parent", RString::Number(nodesql.GetParent()));
						storage->ExecuteCmd(*cmdtag, &brothersIds);
						delete cmdtag;	
						for (i=0; brothersIds[i]!=nodesql.GetIdNode() ; i++){}
						resNode = nodesql.GetName()+"["+RString::Number(i+1)+"]/"+ resNode;
						brothersIds.clear();
						//fin recherche �ge d'un noeud. un elet sec a un age x si il est le x ieme fils qui s'appelle sec 
					}
					cout << endl;
				}
				//Jusqu'l� : pour inex on cherchait le nom complet de node 
				//resTotal += resFile +  resNode+" ; "+cnset()->print2()+ "\n";
				resTotal += resFile +  resNode+" ; "+cnset()->print()+ "\n";
			}
		
	}
	return resTotal;
}
