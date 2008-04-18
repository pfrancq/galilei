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

#include "xanalyser.h"

XAnalyser::XAnalyser(GALILEI::GStorage *_storage, void (*_output)(const char *s)) : insert(660000), exist(660000)
//, globalWordList(100000)
{
	storage = _storage;
	output = _output;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XAnalyser::print(const RString &s) const
{
	if (output)
		output(s.Latin1());
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XAnalyser::AddPath(RString path, const bool &include_subfolders)
{
	DIR *dir;
	dirent *dp;
	struct stat m;
	RString str, name, *pop;
	XFifo<RString> fifodir;
	
	if (path[path.GetLen() - 1] != '/')											// Formatting paths by ending them with a '/'
		path += "/";
	dir = opendir(path.Latin1());												// Open directory
	print(RString("Entering ") + path.Latin1() + "...");
	if (dir == 0)																// If error, do nothing
		return;
	while ((dp = readdir(dir)) != 0)											// While there are still files in the directory
	{
		if (!strcmp(dp->d_name, "..") || !strcmp(dp->d_name, "."))				// Check for implicit directories '/.' and '/..'
			continue;															//  skip them
		else
		{
			str = path + dp->d_name;											// Constructs url : directory + filename
			stat(str.Latin1(), &m);												// Checks element (ie dir or file) properties
			if (include_subfolders && S_ISDIR(m.st_mode))						// If element is a subfolder and subfolder checking is on
				fifodir.Push(new RString(str + "/"));							//  then push it in wait list
			else if (S_ISREG(m.st_mode))										// If it is a regular file
			{
				name = dp->d_name;												// Get the filename (without the directory)
				if (name.FindStr(".xml", name.GetLen() - 4, false) != -1)		// If file ends by '.xml'
				{
					insert.InsertPtr(new RString(str));							//  then put it in the RContainer of files
					//print(RString("\tFound : ") + name.Latin1());
				}
			}
		}
	}
	while (pop = fifodir.PopFirst())											// For all directories in the wait list
	{
		AddPath(*pop, true);													//  redo the operation
		delete pop;
	}
	print(RString("Leaving ") + path.Latin1() + "...");
	closedir(dir);
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void XAnalyser::Update()
{
	RCursor<RString> curs;
	XAnalyseXMLFile *xmlfile;
	RString *str;

	print("================================================");
	cmdtag = new GALILEI::GStorageTag("CreateXMLTables");
	storage->ExecuteCmd(*cmdtag, 0);
	delete cmdtag;
	cmdtag = new GALILEI::GStorageTag("GetAllXMLFiles");
	storage->ExecuteCmd(*cmdtag, &exist);//FIXME :: pb new RContainer!!! FIXED
	delete cmdtag;
	cout<<"start sql"<<endl;
	curs.Set(exist);		
														// Cursor that go through indexed files
	for (curs.Start(); !curs.End(); curs.Next())
	{
		xmlfile = new XAnalyseXMLFile(storage, *curs());
		str = insert.GetPtr(*curs());
		if (str)
		{
			if (xmlfile->IsFileSync())											// If files are the same
			{
				print(RString("Skipped : ") + curs()->Latin1());
				insert.DeletePtr(*str);											// Removes from file to insert
			}
			else
			{
				print(RString("#Changed : ") + curs()->Latin1());
				xmlfile->DropSQL();												// Removes from the database
			}
		}
		else																	// If file does not exist anymore
		{
			print(RString("-Obsolete : ") + curs()->Latin1());
			xmlfile->DropSQL();													// Removes from the database
		}
		delete xmlfile;
	}
	curs.Set(insert);
	print("================================================");
	for (curs.Start(); !curs.End(); curs.Next())								// Here we have all the files to analyse in the RContainer insert
	{
		
		xmlfile = new XAnalyseXMLFile(storage, *curs() );
		//print(RString("Analyse : ") + curs()->Latin1() + "...");
		xmlfile->AnalyseDoc();													// Analyses the XML file
		delete xmlfile;
		//cout << "faiza globalwordlist " << globalWordList.GetNb() << endl;
	}
cout<<"====> Update DONE !!! <====" << endl;
}
