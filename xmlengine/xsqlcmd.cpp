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


/*#include <stdlib.h>
#include <ctype.h>
#include <gdict.h>
#include <glang.h>
#include <ginfo.h>
#include <ginfolist.h>
#include <gword.h>
#include <gwordlist.h>
#include <gweightinfo.h>
#include <gweightinfos.h>
#include <glink.h>
#include <gsugs.h>
#include <guser.h>
#include <gprofile.h>
#include <gsubprofile.h>
#include <gmysql.h>
#include <gslot.h>
#include <ggroup.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <gsession.h>
#include <ggroupshistory.h>
#include <gweightinfoshistory.h>
#include <gindexer.h>
#include <gwordoccurs.h>
#include <gpluginmanagers.h>*/

#include "xsqlcmd.h"

using namespace GALILEI;
using namespace R;

void XSQLCmd::Init()
{
	GStorageCmd *cmd;
	cmd = new XSQLClearXMLTables();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLCreateXMLTables();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetIdFile();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetSizeFile();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetLocalisations();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetLocalisationsList();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetLocalisationsListSize();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetNbOccInFile();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetNodeAttr();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetIdNodes();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetNbNodes();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetTotalWordInFile();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLFullSpecifField();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetUrlFile();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetAllXMLFiles();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLAddXMLFile();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLAddXMLNode();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLAddXMLWord();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLUpdateXMLWord();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLDropFile();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLComputeTF();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLComputeN();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLCompute_n();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLComputeD();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLCompute_d();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
	cmd = new XSQLGetSpecif();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd))
		delete cmd;
/*	XSQLCreateXMLTable *cmd02 = new XSQLCreateXMLTable();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd02))
		delete cmd02;
	XSQLGetIdFile *cmd03 = new XSQLGetIdFile();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd03))
		delete cmd03;
	XSQLGetLocalisations *cmd04 = new XSQLGetLocalisations();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd04))
		delete cmd04;
	XSQLGetNodeAttr *cmd05 = new XSQLGetNodeAttr();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd05))
		delete cmd05;
	XSQLGetIdNodes *cmd06 = new XSQLGetIdNodes();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd06))
		delete cmd06;
	XSQLGetUrlFile *cmd07 = new XSQLGetUrlFile();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(cmd07))
		delete cmd07;
*/
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLClearXMLTables::XSQLClearXMLTables() : GStorageCmd("ClearXMLTables", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLClearXMLTables::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "DROP TABLE filemanager";
	RQuery(db, sqlcmd);
	sqlcmd = "DROP TABLE wordlist";
	RQuery(db, sqlcmd);
	sqlcmd = "DROP TABLE nodelist";
	RQuery(db, sqlcmd);
	sqlcmd = "DROP TABLE localisations";
	RQuery(db, sqlcmd);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLClearXMLTables::~XSQLClearXMLTables()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLCreateXMLTables::XSQLCreateXMLTables() : GStorageCmd("CreateXMLTables", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLCreateXMLTables::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	// wordlist (word, localisations)
	sqlcmd = "CREATE TABLE IF NOT EXISTS wordlist (word TEXT, localisations BIGINT UNSIGNED AUTO_INCREMENT, PRIMARY KEY(localisations), INDEX index_name (word(25)))";
	RQuery(db, sqlcmd);
	//sqlcmd = "CREATE INDEX index_name ON wordlist (word(20))";
	// nodelist (idfile, id, name, parent, child1, nbchild, bytepos, bytelen)
	sqlcmd = "CREATE TABLE IF NOT EXISTS nodelist (idfile BIGINT UNSIGNED, id INT UNSIGNED, ";
	sqlcmd += "name TEXT, parent INT UNSIGNED, child1 INT UNSIGNED, nbchild INT UNSIGNED, ";
	sqlcmd += "bytepos BIGINT UNSIGNED, bytelen BIGINT UNSIGNED, INDEX index_node (id, idfile))";
	RQuery(db, sqlcmd);
	sqlcmd = "CREATE TABLE IF NOT EXISTS localisations (id BIGINT UNSIGNED, idfile BIGINT UNSIGNED, idnode INT UNSIGNED) ";
	RQuery(db, sqlcmd);
	// filemanager (id, url, utc, size)
	sqlcmd = "CREATE TABLE IF NOT EXISTS filemanager (id BIGINT UNSIGNED, ";
	sqlcmd += "url TEXT, utc BIGINT UNSIGNED, size BIGINT UNSIGNED)";
	RQuery(db, sqlcmd);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLCreateXMLTables::~XSQLCreateXMLTables()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLGetIdFile::XSQLGetIdFile() : GStorageCmd("GetIdFile", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
//int GStorageMySQL::GetIdFile(const RString &url, const unsigned long &utc, const unsigned int &size)
void XSQLGetIdFile::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	*static_cast<int *>(caller) = -1;
	sqlcmd = "SELECT id FROM filemanager WHERE url = \"" + inst.GetAttrValue("url") + "\"";
	if (inst.IsAttrDefined("utc"))
		sqlcmd += " AND utc = " + inst.GetAttrValue("utc");
	if (inst.IsAttrDefined("size"))
		sqlcmd += " AND size = " + inst.GetAttrValue("size");
	RQuery rq(db, sqlcmd);
	rq.Start();
	if (!rq.End())
		*static_cast<int *>(caller) = atoi(rq[0].Latin1());
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetIdFile::~XSQLGetIdFile()
{
}
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLGetSizeFile::XSQLGetSizeFile() : GStorageCmd("GetSizeFile", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
//int GStorageMySQL::GetIdFile(const RString &url, const unsigned long &utc, const unsigned int &size)
void XSQLGetSizeFile::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	*static_cast<int *>(caller) = -1;
	sqlcmd = "SELECT size FROM filemanager WHERE id = \"" + inst.GetAttrValue("idfile") + "\"";

	
	RQuery rq(db, sqlcmd);
	rq.Start();
	if (!rq.End())
		*static_cast<int *>(caller) = atoi(rq[0].Latin1());
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetSizeFile::~XSQLGetSizeFile()
{
}
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLGetLocalisations::XSQLGetLocalisations() : GStorageCmd("GetLocalisations", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLGetLocalisations::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	
	sqlcmd = "SELECT localisations FROM wordlist WHERE word = \"" + inst.GetAttrValue("word") + "\"";
	RQuery rq(db, sqlcmd);
	
	rq.Start();
	if (!rq.End())
		*static_cast<RString *>(caller) = rq[0];
	
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetLocalisations::~XSQLGetLocalisations()
{
}
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLGetNbOccInFile::XSQLGetNbOccInFile() : GStorageCmd("GetNbOccInFile", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLGetNbOccInFile::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	*static_cast<int *>(caller) = -1;
	sqlcmd = "SELECT COUNT(*) FROM localisations WHERE id= \"" + inst.GetAttrValue("id") + "\" && idfile = "+inst.GetAttrValue("idfile") ;
	//cout << sqlcmd << endl;
	RQuery rq(db, sqlcmd);
	rq.Start();
	if (!rq.End())
		*static_cast<int *>(caller) = atoi(rq[0].Latin1());
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetNbOccInFile::~XSQLGetNbOccInFile()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLGetLocalisationsList::XSQLGetLocalisationsList() : GStorageCmd("GetLocalisationsList", "MySQL")
{
}
//------------------------------------------------------------------------------
void XSQLGetLocalisationsList::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "SELECT idfile, idnode FROM localisations WHERE id= \"" + inst.GetAttrValue("id") + "\"";
	//cout << "faizaa sql cmd " << sqlcmd  << endl;

	RQuery rq(db, sqlcmd);
	for (rq.Start(); !rq.End(); rq.Next()){
		//on insert d'abord le idfile
		static_cast<vector<int> *>(caller)->push_back(atoi(rq[0].Latin1()));
		//suivi du idnode
		static_cast<vector<int> *>(caller)->push_back(atoi(rq[1].Latin1()));
	}//on retourne donc un vecteur de string tel que � la position x(ximpair) il y'a le idfile et � x+1 le idnode 

}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetLocalisationsList::~XSQLGetLocalisationsList()
{
}
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLGetLocalisationsListSize::XSQLGetLocalisationsListSize() : GStorageCmd("GetLocalisationsListSize", "MySQL")
{
}
//------------------------------------------------------------------------------
void XSQLGetLocalisationsListSize::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	*static_cast<int *>(caller) = -1;
	sqlcmd = "SELECT COUNT(*) FROM localisations WHERE id= \"" + inst.GetAttrValue("id") + "\"";
	
	//cout << sqlcmd << endl;

	RQuery rq(db, sqlcmd);
	rq.Start();
	if (!rq.End())
	*static_cast<int *>(caller) = atoi(rq[0].Latin1());

}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetLocalisationsListSize::~XSQLGetLocalisationsListSize()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================
XSQLGetNodeAttr::XSQLGetNodeAttr() : GStorageCmd("GetNodeAttr", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLGetNodeAttr::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "SELECT name, parent, child1, nbchild, bytepos, bytelen FROM nodelist WHERE idfile = " + inst.GetAttrValue("idfile") + " AND id = " + inst.GetAttrValue("idnode");
	RQuery rq(db, sqlcmd);
	rq.Start();
	//cout << "faizaaa     sql " <<sqlcmd << "result " << rq[0].Latin1() << endl;
	if (!rq.End()) {
		static_cast<XNodeSQL *>(caller)->SetIdFile(atoi(inst.GetAttrValue("idfile").Latin1()));
		static_cast<XNodeSQL *>(caller)->SetIdNode(atoi(inst.GetAttrValue("idnode").Latin1()));
		static_cast<XNodeSQL *>(caller)->SetName(rq[0].Latin1());
		static_cast<XNodeSQL *>(caller)->SetParent(atoi(rq[1].Latin1()));
		static_cast<XNodeSQL *>(caller)->SetChild1(atoi(rq[2].Latin1()));
		static_cast<XNodeSQL *>(caller)->SetNbChild(atoi(rq[3].Latin1()));
		static_cast<XNodeSQL *>(caller)->SetPos(atoi(rq[4].Latin1()));
		static_cast<XNodeSQL *>(caller)->SetLen(atoi(rq[5].Latin1()));
	}
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetNodeAttr::~XSQLGetNodeAttr()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================
XSQLGetIdNodes::XSQLGetIdNodes() : GStorageCmd("GetIdNodes", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLGetIdNodes::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "SELECT id FROM nodelist WHERE idfile = " + inst.GetAttrValue("idfile") + " AND name = \"" + inst.GetAttrValue("name") + "\"" +" AND parent =" + inst.GetAttrValue("parent");
	RQuery rq(db, sqlcmd);
	for (rq.Start(); !rq.End(); rq.Next())
		static_cast<vector<int> *>(caller)->push_back(atoi(rq[0].Latin1()));
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetIdNodes::~XSQLGetIdNodes()
{
}
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================
XSQLGetNbNodes::XSQLGetNbNodes() : GStorageCmd("GetNbNodes", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLGetNbNodes::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "SELECT * FROM nodelist WHERE idfile = " + inst.GetAttrValue("idfile") ;
	RQuery rq(db, sqlcmd);
	*static_cast<int *>(caller)=	rq.GetNb();
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetNbNodes::~XSQLGetNbNodes()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================
XSQLGetTotalWordInFile::XSQLGetTotalWordInFile() : GStorageCmd("GetTotalWordInFile", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLGetTotalWordInFile::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "SELECT COUNT(*) FROM localisations WHERE idfile = " + inst.GetAttrValue("idfile") ;
	RQuery rq(db, sqlcmd);
	*static_cast<int *>(caller)=	rq.GetNb();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetTotalWordInFile::~XSQLGetTotalWordInFile()
{
}//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============ONE SHOT================================================================
XSQLFullSpecifField::XSQLFullSpecifField() : GStorageCmd("FullSpecifField", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLFullSpecifField::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "UPDATE nodelist SET specif= " + inst.GetAttrValue("specif") + " WHERE idfile=" + inst.GetAttrValue("idfile") +" AND id="+ inst.GetAttrValue("idnode") ;
	RQuery rq(db, sqlcmd);
	*static_cast<int *>(caller)=rq.GetNb();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLFullSpecifField::~XSQLFullSpecifField()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLGetUrlFile::XSQLGetUrlFile() : GStorageCmd("GetUrlFile", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLGetUrlFile::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "SELECT url FROM filemanager WHERE id = " + inst.GetAttrValue("idfile");
	RQuery rq(db, sqlcmd);
	rq.Start();
	*static_cast<RString *>(caller) = rq[0];
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetUrlFile::~XSQLGetUrlFile()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLGetAllXMLFiles::XSQLGetAllXMLFiles() : GStorageCmd("GetAllXMLFiles", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLGetAllXMLFiles::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;
	RContainer<RString, true, true> *res;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "SELECT url FROM filemanager";
	RQuery rq(db, sqlcmd);
	for (rq.Start(); !rq.End(); rq.Next())
		static_cast<RContainer<RString, true, true> *>(caller)->InsertPtr(new RString(rq[0]));
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetAllXMLFiles::~XSQLGetAllXMLFiles()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLAddXMLFile::XSQLAddXMLFile() : GStorageCmd("AddXMLFile", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLAddXMLFile::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;
	RQuery *rq;
	int start, stop, mean;
	bool ok = true;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "SELECT COUNT(*) FROM filemanager";
	rq = new RQuery(db, sqlcmd);
	rq->Start();
	*static_cast<int *>(caller) = atoi((*rq)[0]);
	delete rq;
	sqlcmd = "SELECT id FROM filemanager WHERE id = " + RString::Number(*static_cast<int *>(caller));
	rq = new RQuery(db, sqlcmd);
	if (rq->GetNb())
	{
		ok = false;																// Start dichotomous search
		start = 0;																// Set initial conditions
		stop = *static_cast<int *>(caller) - 1;									// - 1 because we know that id res is busy id
	}
	delete rq;
	while (!ok)
	{
		mean = (stop + start) / 2;												// Takes the middle between start and stop
		sqlcmd = "SELECT COUNT(*) FROM filemanager WHERE id BETWEEN " + RString::Number(start) + " AND " + RString::Number(mean);
		rq = new RQuery(db, sqlcmd);											// Counts the nb of id between start and mean
		rq->Start();
		if (atoi((*rq)[0].Latin1()) == mean - start + 1)						// If the nb of elem found = nb of id
			start = mean + 1;													// Seek in second half
		else
			stop = mean;														// Seek in first half
		delete rq;
		if (start == stop)														// Solution found when convergence reached
		{
			ok = true;
			*static_cast<int *>(caller) = start;
		}
	}
	sqlcmd = "INSERT INTO filemanager VALUES(" + RString::Number(*static_cast<int *>(caller)) + ", \"";
	sqlcmd += inst.GetAttrValue("url") + "\", " + inst.GetAttrValue("utc") + ", " + inst.GetAttrValue("size") + ");";
	RQuery(db, sqlcmd);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLAddXMLFile::~XSQLAddXMLFile()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLAddXMLNode::XSQLAddXMLNode() : GStorageCmd("AddXMLNode", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLAddXMLNode::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = "INSERT INTO nodelist VALUES(" + inst.GetAttrValue("idfile") + ", " + inst.GetAttrValue("idnode") + ", \"" + inst.GetAttrValue("name") + "\", ";
	sqlcmd += inst.GetAttrValue("idparent") + ", " + inst.GetAttrValue("idchild1") + ", " + inst.GetAttrValue("nbchild") + ", " + inst.GetAttrValue("filepos") + ", ";
	sqlcmd += inst.GetAttrValue("filelen") + ")";
	RQuery(db, sqlcmd);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLAddXMLNode::~XSQLAddXMLNode()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLAddXMLWord::XSQLAddXMLWord() : GStorageCmd("AddXMLWord", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLAddXMLWord::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd,  word, idfile, localisation, nodes;
	RDb *db;
	int pos1, pos2;
	
	GStorageTag *cmdtag;
	RContainer<RString, true, false> splitnodes(1000);	
	RCursor<RString> cnodes;

	db = static_cast<RDb*> (storage->GetInfos());
	idfile = inst.GetAttrValue("idfile").Latin1();
	word = inst.GetAttrValue("word");
	nodes = inst.GetAttrValue("nodes");
	cmdtag = new GALILEI::GStorageTag("GetLocalisations");
	cmdtag->InsertAttr("word", word);
	storage->ExecuteCmd(*cmdtag, &localisation);
	delete cmdtag;
	if (localisation.GetLen())
	{	nodes.Split(splitnodes, ';');
		cnodes.Set(splitnodes);											
		for (cnodes.Start(); !cnodes.End(); cnodes.Next())	//  NOTE : the substrings fk:n1;n2... are ordered
		{												
			sqlcmd = "INSERT INTO localisations VALUES(\""+localisation+"\","+ idfile+",\""+(cnodes()->Latin1())+"\")";	
			RQuery(db, sqlcmd);
		}
	}else{
		
		sqlcmd = "INSERT INTO wordlist (word) VALUES(\""+word+"\")";
		RQuery(db, sqlcmd);
		cmdtag = new GALILEI::GStorageTag("GetLocalisations");
		cmdtag->InsertAttr("word", word);
		storage->ExecuteCmd(*cmdtag, &localisation);
		delete cmdtag;
		nodes.Split(splitnodes, ';');
		cnodes.Set(splitnodes);											
		for (cnodes.Start(); !cnodes.End(); cnodes.Next())	//  NOTE : the substrings fk:n1;n2... are ordered
		{												
			sqlcmd = "INSERT INTO localisations VALUES(\""+localisation+"\","+ idfile+",\""+(cnodes()->Latin1())+"\")";	
			RQuery(db, sqlcmd);
		}
	}
	
}
/*void XSQLAddXMLWord::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd, res, word, nodes;
	RDb *db;
	unsigned int currid, idfile;
	int pos1, pos2;
	
	db = static_cast<RDb*> (storage->GetInfos());
	idfile = atoi(inst.GetAttrValue("idfile").Latin1());
	word = inst.GetAttrValue("word");
	nodes = inst.GetAttrValue("nodes");


		sqlcmd = "INSERT INTO wordlist VALUES(\"" + word + "\", ";
		sqlcmd += "\"#" + RString::Number(idfile) + ":" + nodes + "\")";
//cout << "faiza mysql        " << sqlcmd  << endl;
	RQuery(db, sqlcmd);
}*/

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLAddXMLWord::~XSQLAddXMLWord()
{
}
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLUpdateXMLWord::XSQLUpdateXMLWord() : GStorageCmd("UpdateXMLWord", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLUpdateXMLWord::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd, res, word, localisations;
	RDb *db;
	unsigned int currid, idfile;
	int pos1, pos2;
	

	db = static_cast<RDb*> (storage->GetInfos());
	idfile = atoi(inst.GetAttrValue("idfile").Latin1());
	word = inst.GetAttrValue("word");
	localisations = inst.GetAttrValue("localisations");

	
		res = "#" + RString::Number(idfile) + ":" + localisations;
		sqlcmd = "UPDATE wordlist SET localisations = CONCAT(lo, \""   + res + "\") WHERE word = \"" + word + "\"";
		//cout << "faiza mysql        " << sqlcmd  << endl;
	
	RQuery(db, sqlcmd);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLUpdateXMLWord::~XSQLUpdateXMLWord()
{
}

//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLDropFile::XSQLDropFile() : GStorageCmd("DropFile", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLDropFile::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;
	RString substr, newstr, idfile;
	int start, stop;

	db = static_cast<RDb*> (storage->GetInfos());
	idfile = inst.GetAttrValue("idfile");
	// Delete from filemanager
	sqlcmd = "DELETE FROM filemanager WHERE id = " + idfile;
	RQuery(db, sqlcmd);
	// Delete from nodelistGetIdNodes(
	sqlcmd = "DELETE FROM nodelist WHERE idfile = " + idfile;
	RQuery(db, sqlcmd);
	// Delete from wordlist
	substr = "#" + idfile + ":";
	sqlcmd = "SELECT word, localisations FROM wordlist WHERE LOCATE(\"" + substr + "\", localisations) > 0";
	RQuery rq(db, sqlcmd);
	for (rq.Start(); !rq.End(); rq.Next())
	{
		newstr = rq[1];
		start = newstr.FindStr(substr, 0);
		stop = newstr.Find('#', start + 1);
		newstr = newstr.Mid(0, start) + newstr.Mid(stop);
		if (newstr.IsEmpty())
			sqlcmd = "DELETE FROM wordlist WHERE word = \"" + rq[0] + "\"";
		else
			sqlcmd = "UPDATE wordlist SET localisations = \"" + newstr + "\" WHERE word = \"" + rq[0] + "\"";
		RQuery(db, sqlcmd);
	}
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLDropFile::~XSQLDropFile()
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------

XSQLComputeTF::XSQLComputeTF() : GStorageCmd("ComputeTF", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLComputeTF::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{   

	RString localisation, sqlcmd;
	RDb *db;
	RString idfile, str, word;
	int  nb=1;
	int t;
	GStorageTag *cmdtag;

	db = static_cast<RDb*> (storage->GetInfos());

	
	idfile =  inst.GetAttrValue("idfile").Latin1();
	word = inst.GetAttrValue("word");
	cmdtag = new GALILEI::GStorageTag("GetLocalisations");
	cmdtag->InsertAttr("word", word);
	storage->ExecuteCmd(*cmdtag, &localisation);
	delete cmdtag;

	//cmdtag = new GALILEI::GStorageTag("GetTotalWordInFile"); Norlamalement c ca le nombre de terme que contient un document mais lent � calculer
	cmdtag = new GALILEI::GStorageTag("GetSizeFile"); //on le remplace par la taille du fichier
	cmdtag->InsertAttr("idfile", idfile);
	storage->ExecuteCmd(*cmdtag, &nb);
	delete cmdtag;

	if (localisation.GetLen())
	{	
		cmdtag = new GALILEI::GStorageTag("GetNbOccInFile");
		cmdtag->InsertAttr("id", localisation);
		cmdtag->InsertAttr("idfile", idfile);
		storage->ExecuteCmd(*cmdtag, &t);
		delete cmdtag;	
	if (nb==0) nb=1;
		*static_cast<double *>(caller) = (double)t / (double)nb; 
	}
	
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLComputeTF::~XSQLComputeTF()
{
}
//______________________________________________________________________________
//------------------------------------------------------------------------------

XSQLComputeD::XSQLComputeD() : GStorageCmd("ComputeD", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLComputeD::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	/*RString sqlcmd;
	RDb *db;
	int D;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = " SELECT COUNT(*) FROM filemanager ";
	
	RQuery rq(db, sqlcmd);
	rq.Start();
	if (!rq.End())
		*static_cast<int *>(caller) = atoi(rq[0].Latin1());*/

	*static_cast<int *>(caller) = 16810;
		
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLComputeD::~XSQLComputeD()
{
}
//______________________________________________________________________________
//------------------------------------------------------------------------------

XSQLCompute_d::XSQLCompute_d() : GStorageCmd("Compute_d", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLCompute_d::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	
	RString localisation, sqlcmd;
	RDb *db;
	RString str, word;
	int idfile, d=0;
	vector<int> localisationsList;
	GStorageTag *cmdtag;

	db = static_cast<RDb*> (storage->GetInfos());

	word = inst.GetAttrValue("word");
	cmdtag = new GALILEI::GStorageTag("GetLocalisations");
	cmdtag->InsertAttr("word", word);
	storage->ExecuteCmd(*cmdtag, &localisation);
	delete cmdtag;
	if (localisation.GetLen())
	{	
		cmdtag = new GALILEI::GStorageTag("GetLocalisationsList");
		cmdtag->InsertAttr("id", localisation);
		storage->ExecuteCmd(*cmdtag, &localisationsList);
		delete cmdtag;	
		d=0;
		for (int i=0; i<localisationsList.size(); i++)							//  localisationsList = idfile idnode idfile idnode
		{	
			d++;																		// 
			idfile = localisationsList[i];	
			i++; i++;
			while (i<localisationsList.size() && localisationsList[i]==idfile){
				i++; i++;
			}
			i--;
			
		}

		*static_cast<int *>(caller) = d; // n ici = nombre de documents où apparait le terme word dans toute la collection
	}
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLCompute_d::~XSQLCompute_d()
{
}
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================
XSQLComputeN::XSQLComputeN() : GStorageCmd("ComputeN", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLComputeN::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	/*RString sqlcmd;
	RDb *db;
	int N;

	db = static_cast<RDb*> (storage->GetInfos());
	sqlcmd = " SELECT COUNT(*) FROM nodelist ";
	
	RQuery rq(db, sqlcmd);
	rq.Start();
	if (!rq.End())
		*static_cast<int *>(caller) = atoi(rq[0].Latin1());*/
		
		*static_cast<int *>(caller) = 7646004 ;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLComputeN::~XSQLComputeN()
{
}
//______________________________________________________________________________
//------------------------------------------------------------------------------

XSQLCompute_n::XSQLCompute_n() : GStorageCmd("Compute_n", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLCompute_n::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	
	RString localisation, sqlcmd;
	RDb *db;
	RString str, word;
	int start, stop, pos, cpt=0;
	int localisationsListSize;
	GStorageTag *cmdtag;

	db = static_cast<RDb*> (storage->GetInfos());

	word = inst.GetAttrValue("word");
	cmdtag = new GALILEI::GStorageTag("GetLocalisations");
	cmdtag->InsertAttr("word", word);
	storage->ExecuteCmd(*cmdtag, &localisation);
	delete cmdtag;
	if (localisation.GetLen())
	{	
		cmdtag = new GALILEI::GStorageTag("GetLocalisationsListSize");
		cmdtag->InsertAttr("id", localisation);
		storage->ExecuteCmd(*cmdtag, &localisationsListSize);
		delete cmdtag;	
		*static_cast<double *>(caller) = (double) localisationsListSize/(double) 2;
	}
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLCompute_n::~XSQLCompute_n()
{
}
//==============================================================================
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//==============================================================================

XSQLGetSpecif::XSQLGetSpecif() : GStorageCmd("GetSpecif", "MySQL")
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XSQLGetSpecif::Run(GStorage *storage, const GStorageTag &inst, void *caller)
{
	RString sqlcmd;
	RDb *db;

	db = static_cast<RDb*> (storage->GetInfos());
	
	sqlcmd = "SELECT specif FROM nodelist WHERE idfile = " + inst.GetAttrValue("idfile") + " AND id = " + inst.GetAttrValue("idnode");
	RQuery rq(db, sqlcmd);
	rq.Start();
	if (!rq.End())
		*static_cast<int *>(caller) = atoi(rq[0].Latin1());
	
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
XSQLGetSpecif::~XSQLGetSpecif()
{
}
