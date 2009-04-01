/***************************************************************************
 *   Copyright 2004-2009 by the Universit�Libre de Bruxelles.

	Authors:
		Faïza Abbaci (fabbaci@ulb.ac.be)                                      *
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

#include "xanalysexmlfile.h"
#include "xgeneral.h"
#include <ctime>

XAnalyseXMLFile::XAnalyseXMLFile(GALILEI::GStorage *_storage, const RString &_filename):
wordslist(1000), nodeslist(600)
{
	storage = _storage;
	filename = _filename;

}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XAnalyseXMLFile::AnalyseDoc()
{
	RCursor<XNodeSQL> cnl;														// List of the nodes of the XML file
	RCursor<XWordSQL> cwl;														// List of the words appearing in the XML file
	time_t temps_actD;
	time_t temps_actF;

	time(&temps_actD);
	xfile = new XXMLFile(filename, &xstruct);		// Gets the XML structure in a RTree
	cout << "faiza le fichier à traiter est  " << filename <<  endl;
	xfile->Open(RIO::Read);														// The structure is caught when file is opened
	xfile->Close();
	Lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn("en");
	delete xfile;																// The important thing is xstruct and not xfile !!!
	get_property();
	cmdtag = new GALILEI::GStorageTag("AddXMLFile");
	cmdtag->InsertAttr("url", filename);
	cmdtag->InsertAttr("utc", RString::Number(utc));
	cmdtag->InsertAttr("size", RString::Number(size));
	storage->ExecuteCmd(*cmdtag, &idfile);
	delete cmdtag;
	search_breadth();															// Performs a breadth search. (Oh, really ?)
	cnl.Set(nodeslist);
	for (cnl.Start(); !cnl.End(); cnl.Next())									// For each node of the file, add it in the nodelist table
	{
		cmdtag = new GALILEI::GStorageTag("AddXMLNode");
		cmdtag->InsertAttr("idfile", RString::Number(idfile));
		cmdtag->InsertAttr("idnode", RString::Number(cnl()->GetIdNode()));
		cmdtag->InsertAttr("name", cnl()->GetName());
		cmdtag->InsertAttr("idparent", RString::Number(cnl()->GetParent()));
		cmdtag->InsertAttr("idchild1", RString::Number(cnl()->GetChild1()));
		cmdtag->InsertAttr("nbchild", RString::Number(cnl()->GetNbChild()));
		cmdtag->InsertAttr("filepos", RString::Number(cnl()->GetPos()));
		cmdtag->InsertAttr("filelen", RString::Number(cnl()->GetLen()));
		storage->ExecuteCmd(*cmdtag, 0);
		delete cmdtag;
	}
	cwl.Set(wordslist);
	for (cwl.Start(); !cwl.End(); cwl.Next())									// For each word of the file, add it in the wordlist table
	{
		cmdtag = new GALILEI::GStorageTag("AddXMLWord");
		cmdtag->InsertAttr("idfile", RString::Number(idfile));
		cmdtag->InsertAttr("word", cwl()->GetWord());
		cmdtag->InsertAttr("nodes", cwl()->GetNodes());
		storage->ExecuteCmd(*cmdtag, 0);
		delete cmdtag;
	}
	time(&temps_actF);
	if (difftime(temps_actF,temps_actD))
	cout << "temps pris par analyseDoc pour le fichier  " << idfile << "----------- "<< difftime(temps_actF,temps_actD) << endl;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
bool XAnalyseXMLFile::IsFileSync()												// Compares with the utc and size stored in the database
{
	int idfile;

	get_property();
	cmdtag = new GALILEI::GStorageTag("GetIdFile");
	cmdtag->InsertAttr("url", filename);
	cmdtag->InsertAttr("utc", RString::Number(utc));
	cmdtag->InsertAttr("size", RString::Number(size));
	storage->ExecuteCmd(*cmdtag, &idfile);
	delete cmdtag;
	return idfile != -1;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XAnalyseXMLFile::get_property()											// Gets utc and size of the file
{
	struct stat st_stat;
	int handle;

	utc = size = 0;
	handle = open(filename.Latin1(), O_RDONLY);
	if (handle != -1)
	{
		fstat(handle, &st_stat);
		utc = st_stat.st_mtim.tv_sec;
		size = st_stat.st_size;
		close(handle);
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XAnalyseXMLFile::insertWord(const RString &w)								// Inserts word w with path id
{
	XWordSQL *wi;
	RString wlower(w.ToLower());												// Sets to lower case


	//cout << "faiza les mots -----------------------------------" << wlower << endl;

	if (!Lang->InStop(wlower)){
		RString stem=Lang->GetStemming(wlower);

		if (!wordslist.IsIn<RString>(stem))										// If word not exists
			wordslist.InsertPtr(new XWordSQL(stem, curr_idnode));					// Creates and adds it to the list
		else
		{																			// Otherwise :
			wi = wordslist.GetPtr<RString>(stem);									// Gets the WordItem
			wi->AddNode(curr_idnode);												// And adds it a node id

		}
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XAnalyseXMLFile::parseWords(const RString &str)							// Finds each word in paragraphs
{
	int startword, len;
	size_t i = 0;

	startword = 0;
	len = str.GetLen();
	for (i = 0; i < len; i++)
	{
		if (str[i].IsSpace() || str[i].IsPunct())
		{
			if (i > startword)
				insertWord(str.Mid(startword, i - startword).Latin1());
			startword = i + 1;
		}
	}
	if (i > startword)
		insertWord(str.Mid(startword, i - startword).Latin1());
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XAnalyseXMLFile::search_breadth()
{
	int *curr_idparent, child1;
	XNodeSQL *tmpnode;
	XFifo<int> parentsfifo;
	XFifo<XXMLElem> tagsfifo;
	XXMLElem *elem;
	XXMLTag *tag;
	RCursor<RXMLTag> curs_tag;
	RCursor<RXMLAttr> curs_attr;



	RXMLTag *tage = xstruct.GetTop();
	tagsfifo.Push(dynamic_cast<XXMLTag *> (xstruct.GetTop()));					// Put the first node (ie the root node) in Fifo
	parentsfifo.Push(new int(idfile));											// Note : the idparent of root node is the idfile. Why ? Don't remember... Yes I remember now! It is to set a a value for curr_idparent!!!
																				// Note the new int... See explaination at end of this function!

	for (curr_idnode = 0; tagsfifo.GetNb(); curr_idnode++)						// While there are still nodes inside Fifo
	{																			//  curr_idpath is the id node
		elem = tagsfifo.PopFirst();												// Gets the first node inside the fifo
		//cout << "faiza getlen = " << elem->GetLen() << " get name => "<< elem->Getname() << endl;
		curr_idparent = parentsfifo.PopFirst();									//  and the id of its parent
		tmpnode = new XNodeSQL();												// This is an object representation of what will appear in the database
		tmpnode->SetIdFile(idfile);												// Sets the id file
		tmpnode->SetIdNode(curr_idnode);			// Sets the id of the node
		//cout << elem->Getnb() << endl;
		tmpnode->SetNbChild(elem->Getnb());										// Sets the number of children
		if (elem->GetType() == XXMLElem::TAG)									// If element is a tag
			if (elem->Getnb())													// And it has children
				child1 = curr_idnode + tagsfifo.GetNb() + 1;					// Then compute id of 1st child
			else
				child1 = 1;														// Otherwise, sets to 1 (for a tag)
		else																	// If not a tag (ie an attribute)
			child1 = 0;															// Sets 1st child to 0
		tmpnode->SetChild1(child1);												// Sets the first child so computed
		tmpnode->SetParent(*curr_idparent);										// Sets the id of parent node
		tmpnode->SetName(elem->Getname().ToLower());							// Sets the name in lowercase
		tmpnode->SetPos(elem->GetPos());										// Sets the starting position in the file (so seek(pos) reach the starting position)
		tmpnode->SetLen(elem->GetLen());										// Sets the number of chars of between start and stop tag in the file
		nodeslist.InsertPtr(tmpnode);											// Insert the node in the RContainer
		insertWord(elem->Getname());											// Insert the word in the RContainer of words appearing in the file

		parseWords(elem->Getvalue());											// Gets the words of value (ie tag content or attribute value)
		if (elem->GetType() == XXMLElem::TAG)									// If the node is a tag (ie and not an attribute)
		{
			tag = dynamic_cast<XXMLTag *> (elem);
			curs_attr = tag->GetAttrs();										// What to do with attributes (they are considered as special children)
			for (curs_attr.Start(); !curs_attr.End(); curs_attr.Next())
			{
				tagsfifo.Push(dynamic_cast<XXMLElem *> (curs_attr()));			// Adds attributes in fifo
				parentsfifo.Push(new int(curr_idnode));							//  and the id of parent! Note the new
			}
			curs_tag = tag->GetNodes();											// What to do with children
			for (curs_tag.Start(); !curs_tag.End(); curs_tag.Next())
			{
				tagsfifo.Push(dynamic_cast<XXMLElem *> (curs_tag()));			// Adds children in fifo
				parentsfifo.Push(new int(curr_idnode));							//  and the id of parent! Note the new
			}
		}
		delete curr_idparent;													// Use of ptrs in fifo !
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XAnalyseXMLFile::DropSQL()
{
	int idfile;

	cmdtag = new GALILEI::GStorageTag("GetIdFile");
	cmdtag->InsertAttr("url", filename);
	storage->ExecuteCmd(*cmdtag, &idfile);
	delete cmdtag;
	cmdtag = new GALILEI::GStorageTag("DropFile");
	cmdtag->InsertAttr("idfile", RString::Number(idfile));
	storage->ExecuteCmd(*cmdtag, 0);
	delete cmdtag;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XAnalyseXMLFile::~XAnalyseXMLFile()
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
/*void XAnalyseXMLFile::ShowSolution()
{
	RCursor<XWordItem> curswl(wordslist);
	RCursor<XNodeSQL> cursnl(nodeslist);

	cout << "+==========" << endl << "| WORDSLIST" << endl << "+----------" << endl;
	for (curswl.Start(); !curswl.End(); curswl.Next())
		cout << "| " << curswl()->print().Latin1() << endl;
	cout << "+==========" << endl << "| NODESLIST" << endl << "+----------" << endl;
	for (cursnl.Start(); !cursnl.End(); cursnl.Next())
		cout << "| " << cursnl()->print().Latin1() << endl;
	cout << "_____________________________" << endl;
}
*/
