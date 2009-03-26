/*

	GALILEI Research Project

	EMail.cpp

	Filter for Mails - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
#include <gdocxml.h>


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
	virtual void DoIt(GDocXML* doc,const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AnalyzeBlock(meta,doc->AddTitle(RString::Null,0));
	}
};


//-----------------------------------------------------------------------------
class CmdSummary : public EMailCmd
{
public:
	CmdSummary(GFilterEMail* filter) : EMailCmd(filter,"summary") {}
	virtual void DoIt(GDocXML* doc,const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AnalyzeBlock(meta,doc->AddSubject(RString::Null,0));
	}
};


//-----------------------------------------------------------------------------
class CmdFrom : public EMailCmd
{
public:
	CmdFrom(GFilterEMail* filter) : EMailCmd(filter,"from") {}
	virtual void DoIt(GDocXML* doc,const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AnalyzeBlock(meta,doc->AddCreator(RString::Null,0));
	}
};


//-----------------------------------------------------------------------------
class CmdOrganization : public EMailCmd
{
public:
	CmdOrganization(GFilterEMail* filter) : EMailCmd(filter,"organization") {}
	virtual void DoIt(GDocXML* doc,const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AnalyzeBlock(meta,doc->AddPublisher(RString::Null,0));
	}
};


//-----------------------------------------------------------------------------
class CmdDate : public EMailCmd
{
public:
	CmdDate(GFilterEMail* filter) : EMailCmd(filter,"date") {}
	virtual void DoIt(GDocXML* doc,const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		doc->AddDate(meta);
	}
};


//-----------------------------------------------------------------------------
class CmdKeywords : public EMailCmd
{
public:
	CmdKeywords(GFilterEMail* filter) : EMailCmd(filter,"subject") {}
	virtual void DoIt(GDocXML* doc,const RString& meta)
	{
		if(meta.IsEmpty())
			return;
		Filter->AnalyzeKeywords(meta,',',doc->AddSubject(RString::Null,0));
	}
};



//-----------------------------------------------------------------------------
//
// class GFilterEMail
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterEMail::GFilterEMail(GFactoryFilter* fac)
	: GFilter(fac), BlankLines(false), Cmds(20)
{
	AddMIME("text/email");
	Cmds.InsertPtr(new CmdSubject(this));
	Cmds.InsertPtr(new CmdSummary(this));
	Cmds.InsertPtr(new CmdFrom(this));
	Cmds.InsertPtr(new CmdOrganization(this));
	Cmds.InsertPtr(new CmdDate(this));
	Cmds.InsertPtr(new CmdKeywords(this));
}


//-----------------------------------------------------------------------------
void GFilterEMail::ApplyConfig(void)
{
	BlankLines=Factory->GetBool("BlankLines");
}


//-----------------------------------------------------------------------------
bool GFilterEMail::ExtractCmd(const RString& line)
{
	RString Cmd(line.GetLen());
	const RChar* ptr=line();
	size_t len=0;

	// Look an the first word until ':' or spaces
	while((!ptr->IsNull())&&((*ptr)!=' ')&&((*ptr)!='\t')&&((*ptr)!=':'))
	{
		ptr++;
		len++;
	}

	// Skip Spaces.
	while((!ptr->IsNull())&&(((*ptr)==' ')||((*ptr)==RChar('\t'))))
		ptr++;

	// If the next character is not a ':', this is not a cmd.
	if((ptr->IsNull())||((*ptr)!=':'))
		return(false);

	// Mark end of command and put it in lowercase and Skip ':'
	Cmd=line.Mid(0,len).ToLower();
	ptr++;

	// After skipped the spaces, and separate the command pointed by line and
	// the information pointed by ptr.
	while((!ptr->IsNull())&&(((*ptr)==' ')||((*ptr)==RChar('\t'))))
		ptr++;

	// Analyse the different metaData possible.
	RString metaData(ptr);
	EMailCmd* cmd=Cmds.GetPtr(Cmd);
	if(cmd)
		cmd->DoIt(Doc,metaData);

	return(true);
}


//-----------------------------------------------------------------------------
void GFilterEMail::Analyze(const RURI&,const RURI& file,const RURI& docxml)
{
	RXMLTag* part;
	RXMLTag* tag;
	bool Header;
	bool Paragraph;
	bool Read;
	RString NextLine;
	RString Line;
	const RChar* ptr;

	// Init Part
	Doc=new GDocXML(docxml,file);
	RTextFile Src(file);
	Src.Open(R::RIO::Read);

	// Create the metaData tag and the first information
	part=Doc->GetMetaData();
	Doc->AddIdentifier(Doc->GetURL()());

	// Email have at the beginning information on each line.
	Header=true;        // There headers to read.
	Read=true;
	while((!Src.End())&&Header)
	{
		// Read a line
		if(Read)
			Line=Src.GetLine(false);

		// Add the next lines beginning with a space if the line is not empty
		if(!Src.End())
		{
			NextLine=Src.GetLine(false);
			while((!NextLine.IsEmpty())&&(NextLine()->IsSpace())&&(!Src.End()))
			{
				Line+=NextLine;
				NextLine=Src.GetLine(false);
			}
		}

		// Analyse the line for a command.
		Header=ExtractCmd(Line);
		if(Header)
		{
			if(BlankLines)
			{
				while((NextLine.IsEmpty())&&(!Src.End()))
					NextLine=Src.GetLine(false);
			}

			// Next line becomes the current one
			Line=NextLine;
		}
		Read=false;
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

	// Look for the content
	part=Doc->GetContent();

	while((!Read)||(!Src.End()))
	{
		Doc->AddTag(part,tag=new RXMLTag("docxml:p"));

		// If necessary -> read a new line f
		if(!Read)
			Read=true;
		else
		{
			if(!Src.End())
				Line=NextLine;
			if(!Src.End())
				NextLine=Src.GetLine(false);
		}

		// Paragraph are supposed to be terminated by at least one blank line
		Paragraph=true;
		while((!Src.End())&&Paragraph)
		{
			// Look if it is a blank line
			ptr=NextLine();
			while((!ptr->IsNull())&&(ptr->IsSpace()))
				ptr++;

			// If blank line -> it is the end of a paragraph
			if(ptr->IsNull())
				Paragraph=false;
			else
				Line+=NextLine;

			// Read a Line
			NextLine=Src.GetLine(false);
		}
		AnalyzeBlock(Line,tag);
	}

	// Save the structure and delete everything
	RXMLFile Out(docxml,Doc);
	Out.Open(RIO::Create);
	delete Doc;
}


//------------------------------------------------------------------------------
void GFilterEMail::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("BlankLines",false));
}


//-----------------------------------------------------------------------------
GFilterEMail::~GFilterEMail(void)
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("EMail Filter",GFilterEMail)
