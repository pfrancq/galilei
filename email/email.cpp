/*

	GALILEI Research Project

	EMail.cpp

	Filter for Mails - Implementation.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for R Project
#include <rxmlfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI Plugins
#include <email.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>


//-----------------------------------------------------------------------------
//
// Command classes
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CmdSubject : public EMailCmd
{
public:
	CmdSubject(GFilterEMail* filter) : EMailCmd(filter,"subject") {}
	virtual void DoIt(const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AddDublinCoreMetaData("title",meta,GFilter::Sentence);
	}
};


//-----------------------------------------------------------------------------
class CmdSummary : public EMailCmd
{
public:
	CmdSummary(GFilterEMail* filter) : EMailCmd(filter,"summary") {}
	virtual void DoIt(const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AddDublinCoreMetaData("subject",meta,GFilter::Sentence);
	}
};


//-----------------------------------------------------------------------------
class CmdFrom : public EMailCmd
{
public:
	CmdFrom(GFilterEMail* filter) : EMailCmd(filter,"from") {}
	virtual void DoIt(const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AddDublinCoreMetaData("creator",meta,GFilter::Sentence);
	}
};


//-----------------------------------------------------------------------------
class CmdOrganization : public EMailCmd
{
public:
	CmdOrganization(GFilterEMail* filter) : EMailCmd(filter,"organization") {}
	virtual void DoIt(const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AddDublinCoreMetaData("publisher",meta,GFilter::Sentence);
	}
};


//-----------------------------------------------------------------------------
class CmdDate : public EMailCmd
{
public:
	CmdDate(GFilterEMail* filter) : EMailCmd(filter,"date") {}
	virtual void DoIt(const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AddDublinCoreMetaData("date",meta,GFilter::Sentence);
	}
};


//-----------------------------------------------------------------------------
class CmdArchiveName : public EMailCmd
{
public:
	CmdArchiveName(GFilterEMail* filter) : EMailCmd(filter,"archive-name") {}
};


//-----------------------------------------------------------------------------
class CmdLastModified: public EMailCmd
{
public:
	CmdLastModified(GFilterEMail* filter) : EMailCmd(filter,"last-modified") {}
};


//-----------------------------------------------------------------------------
class CmdVersion : public EMailCmd
{
public:
	CmdVersion(GFilterEMail* filter) : EMailCmd(filter,"version") {}
};



//-----------------------------------------------------------------------------
//
// class GFilterEMail
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterEMail::GFilterEMail(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac), Cmds(20)
{
	AddMIME("text/email");
	AddMIME("message/rfc822");
	Cmds.InsertPtr(new CmdSubject(this));
	Cmds.InsertPtr(new CmdSummary(this));
	Cmds.InsertPtr(new CmdFrom(this));
	Cmds.InsertPtr(new CmdOrganization(this));
	Cmds.InsertPtr(new CmdDate(this));
	Cmds.InsertPtr(new CmdArchiveName(this));
	Cmds.InsertPtr(new CmdLastModified(this));
	Cmds.InsertPtr(new CmdVersion(this));
}


//-----------------------------------------------------------------------------
bool GFilterEMail::ExtractCmd(const RString& line)
{
	const RChar* ptr(line());
	size_t len=0;

	// Look an the first word until ':' or spaces
	while((!ptr->IsNull())&&(!ptr->IsSpace())&&((*ptr)!=':'))
	{
		ptr++;
		len++;
	}

	// Skip Spaces.
	while((!ptr->IsNull())&&ptr->IsSpace())
		ptr++;

	// If the next character is not a ':', this is not a cmd.
	if((ptr->IsNull())||((*ptr)!=':'))
		return(false);
	ptr++; // Skip ':'

	// Mark end of command and put it in lower case and Skip ':'
	RString Cmd(line.Mid(0,len).ToLower());

	// After skipped the spaces, and separate the command pointed by line and
	// the information pointed by ptr.
	while((!ptr->IsNull())&&(!ptr->IsSpace()))
		ptr++;

	// Analyze the different metaData possible.
	RString metaData(ptr);
	EMailCmd* cmd(Cmds.GetPtr(Cmd));
	if(cmd)
	{
		cmd->DoIt(metaData);
		return(true);
	}
	cmd=Cmds.GetPtr(Cmd.Mid(Cmd.GetLen()-12));
	if(cmd)
	{
		cmd->DoIt(metaData);
		return(true);
	}
	return(false);
}


//-----------------------------------------------------------------------------
void GFilterEMail::Analyze(GDoc* doc,const RURI& uri,RXMLParser* parser,GSlot*)
{
	bool Header;
	RString Line;

	// Init Part
	StartStream(parser);
	RTextFile Src(uri);
	Src.SetRemStyle(RTextFile::NoComment);
	Src.Open(R::RIO::Read);
	if(Src.End())
	{
		EndStream(parser);
		return;
	}

	// Create the metaData tag and the first information
	AddDublinCoreMetaData("identifier",doc->GetURL()());

	// Email have at the beginning information on each line.
	Header=true;        // There headers to read.
	while((!Src.End())&&Header)
	{
		// Analyze the next no-empty line for a command.
		Line=Src.GetLine(false);
		if(Line.IsEmpty())
			continue;
		Header=ExtractCmd(Line);
	}
	WriteMetaDataStream(parser);
	if(Src.End())
	{
		EndStream(parser);
		return;
	}

	// Look if the email is signed.
/*		findstr=strstr(Begin,("-----BEGIN PGP SIGNED MESSAGE-----"));
		if(findstr)
		{
			Begin=findstr+strlen("-----BEGIN PGP SIGNED MESSAGE-----")+1;   // Skip this;
			// Search end of mail
			findstr=strstr(Begin,"-----BEGIN PGP SIGNATURE-----");
			if(findstr)
				(*findstr)=0;
		}
		else
			Begin[strlen(Begin)-1]='\n';*/

	ExtractTextualContent(Src,parser,Line);
	EndStream(parser);
}


//------------------------------------------------------------------------------
void GFilterEMail::CreateParams(GPlugInFactory*)
{
}


//-----------------------------------------------------------------------------
GFilterEMail::~GFilterEMail(void)
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("EMail Filter","EMail Filter",GFilterEMail)
