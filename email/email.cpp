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
// include files for GALILEI
#include <email.h>
#include <gdoc.h>
#include <gdocanalyze.h>



//-----------------------------------------------------------------------------
//
// EMailCmd
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * The EMailCmd class provides a representation for a given class of command.
 */
class GFilterEMail::EMailCmd
{
	RString Cmd;
	RString Data;
public:

	EMailCmd(const RString& cmd,const RString& data) : Cmd(cmd), Data(data) {}
	int Compare(const EMailCmd& cmd) const {return(Cmd.Compare(cmd.Cmd));}
	int Compare(const EMailCmd* cmd) const {return(Cmd.Compare(cmd->Cmd));}
	int Compare(const RString cmd) const {return(Cmd.Compare(cmd));}
	void DoIt(GDocAnalyze* analyzer,const RString& meta,size_t pos)
	{
		if(meta.IsEmpty())
			return;
		analyzer->ExtractDCMI(Data,meta,pos,0);
	}
	virtual ~EMailCmd(void) {}
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
	Cmds.InsertPtr(new EMailCmd("subject","title"));
	Cmds.InsertPtr(new EMailCmd("summary","subject"));
	Cmds.InsertPtr(new EMailCmd("from","creator"));
	Cmds.InsertPtr(new EMailCmd("organization","publisher"));
	Cmds.InsertPtr(new EMailCmd("date","date"));
}


//-----------------------------------------------------------------------------
bool GFilterEMail::ExtractCmd(GDocAnalyze* analyzer,const RString& line,size_t pos)
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
		cmd->DoIt(analyzer,metaData,pos);
		return(true);
	}
	cmd=Cmds.GetPtr(Cmd.Mid(Cmd.GetLen()-12));
	if(cmd)
	{
		cmd->DoIt(analyzer,metaData,pos);
		return(true);
	}
	return(false);
}


//-----------------------------------------------------------------------------
void GFilterEMail::Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file)
{
	bool Header;
	RString Line;

	// Init Part
	RTextFile Src(file);
	Src.SetRemStyle(RTextFile::NoComment);
	Src.Open(R::RIO::Read);

	// Read the headers of an e-mail
	Header=true;        // Suppose there is some headers to read.
	while((!Src.End())&&Header)
	{
		// Analyze the next no-empty line for a command.
		size_t Pos(Src.GetPos());
		Line=Src.GetLine(false);
		if(Line.IsEmpty())
			continue;
		Header=ExtractCmd(analyzer,Line,Pos);
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

	//
	while(!Src.End())
	{
		size_t Pos(Src.GetPos());
		analyzer->ExtractContent(Src.GetLine(false),Pos);
	}

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
