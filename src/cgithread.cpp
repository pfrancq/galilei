/*

	GALILEI Research Project

	CGIThread.cpp

	CGI Thread - Implementation.

	Copyright 2009-2011 by Pascal Francq (pascal@francq.info).
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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <unistd.h>


//------------------------------------------------------------------------------
// include files for FastCGI
#include <fcgiapp.h>


//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <gsession.h>
#include <gdoc.h>
#include <gmetaengine.h>
#include <gdocfragmentranks.h>


//------------------------------------------------------------------------------
// include files for current project
#include <cgithread.h>
#include <html.h>
#include <rungalileiprogram.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
// Extern variables
extern char** environ;


//------------------------------------------------------------------------------
//
// Class CGIThread
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CGIThread::CGIThread(RunGALILEIProgram* app,int id)
	: RThread(id), App(app)
{
}


//------------------------------------------------------------------------------
bool CGIThread::LocalCall(FCGX_Request& request)
{
	return(strcmp(FCGX_GetParam("REMOTE_ADDR",request.envp),"127.0.0.1")==0);
}


//------------------------------------------------------------------------------
void CGIThread::ParseParam(char* &ptr,RString& cmd)
{
	cmd.Clear();
	for(;(*ptr)&&((*ptr)!='=');ptr++)
	{
		cmd+=(*ptr);
	}
	if(*ptr)
		ptr++;
}


//------------------------------------------------------------------------------
void CGIThread::ParseValue(char* &ptr,RString& val)
{
	val.Clear();
	for(;(*ptr)&&((*ptr)!='&');ptr++)
	{
		if((*ptr)=='+')
			val+=' ';
		else if((*ptr)=='%')
		{
			// Convert
			// %28 %29 %26 %7E %7C %2B . %3A
			// ( ) & ~ | + . :
			ptr++;
			if((*ptr)=='2')
			{
				ptr++;
				if((*ptr)=='8')
					val+='(';
				else if((*ptr)=='9')
					val+=')';
				else if((*ptr)=='6')
					val+='&';
				else if((*ptr)=='B')
					val+='+';
			}
			else if((*ptr)=='3')
			{
				ptr++;
				if((*ptr)=='A')
					val+=':';
			}
			else if((*ptr)=='7')
			{
				ptr++;
				if((*ptr)=='E')
					val+='~';
				else if((*ptr)=='C')
					val+='|';
			}
		}
		else
			val+=(*ptr);
	}
	if(*ptr)
		ptr++;
}


//------------------------------------------------------------------------------
CGIThread::tCmd CGIThread::ParseQuery(FCGX_Request& request,RString& session,RString& param)
{
	RString Param;
	Param.SetLen(200);

	char* ptr(FCGX_GetParam("REQUEST_URI",request.envp));

	// Search begin of query
	while((*ptr)&&((*ptr)!='?'))
		ptr++;
	if(*ptr)
		ptr++;

	// Get the session
	ParseParam(ptr,Param);
	if(Param!="session")
	{
		param="Not session specified";
		return(cNoCmd);
	}
	ParseValue(ptr,Param);
	session=Param;

	// Get the command
	ParseParam(ptr,Param);
	if(Param!="cmd")
	{
		param="No command specified";
		return(cNoCmd);
	}

	// Get the command
	ParseValue(ptr,Param);
	if(Param=="search")
	{
		// Get the query
		ParseParam(ptr,Param);
		if(Param!="query")
		{
			param="No query specified";
			return(cNoCmd);
		}
		ParseValue(ptr,Param);
		param=Param;
		return(cSearch);
	}
	else if(Param=="test")
	{
		if(LocalCall(request))
			return(cTest);
		param="'"+Param+"' is not a valid command";
		return(cNoCmd);
	}
	else
	{
		param="'"+Param+"' is not a valid command";
		return(cNoCmd);
	}

	param="Unknown error";
	return(cNoCmd);
}


//------------------------------------------------------------------------------
void CGIThread::WriteEnv(HTML& html,const char* const* envp)
{
	html.Write(0,"<PRE>");
	for(;*envp;++envp)
		html.Write(1,*envp);
   html.Write(0,"</PRE>");
}


//------------------------------------------------------------------------------
void CGIThread::Search(HTML& html,const R::RString& session,const R::RString& query)
{
	html.WriteTitle(query+" - MediSearch");
	html.WriteForm(session,query);
	try
	{
		bool Create(true);
		GSession* Session(App->GetSession(session,Create,0,"/etc/galilei/sessions"));
		if(Create)
		{
				Session->LoadObjs(pClass);
				Session->LoadObjs(pTopic);
				Session->LoadObjs(pDoc);
				Session->LoadObjs(pCommunity);
				Session->LoadObjs(pUser);
		}
		Session->RequestMetaEngine(query,GetId());
		GMetaEngine* Meta(GALILEIApp->GetCurrentPlugIn<GMetaEngine>("MetaEngine"));

		RCursor<GDocFragmentRanks> Cur(Meta->GetResults(GetId()));
		size_t i;
		if(Cur.GetNb()==0)
			html.WriteP(0,"No Results");
		else
		{
			html.WriteP(0,RString::Number(Cur.GetNb())+" results","font-size: 70%; color: gray;");
			html.Write(0,"\t<hr width='95%%' align='center'/>\r\n");
		}
		for(Cur.Start(),i=0;(!Cur.End())&&(i<20);Cur.Next(),i++)
		{
			html.Write(0,"<p>");
			html.WriteDiv(1,Cur()->GetFragment()->GetDoc()->GetTitle(),"color: blue;");
			html.WriteDiv(1,Cur()->GetFragment()->GetDoc()->GetName(),"font-size: 90%; color: green;");
			RString Fragment;
			try
			{
				Fragment=Cur()->GetFragment()->GetFragment(200);
				if(Fragment[Fragment.GetLen()-1]!='.')
					Fragment+=" ...";
			}
			catch(...)
			{
				Fragment="!!!Extract not found in the document!!!";
			}
			html.WriteDiv(1,Fragment,"font-size: 70%;");
			RString Info("Patient Age: "+RString::Number(rand()%(80-10)+10)+" ; Hospital Stay: "+RString::Number(rand()%(10-1)+1)+"d");
			html.WriteDiv(2,Info,"color: red; font-size: 90%");
			html.Write(1,"</div>");
			html.Write(0,"</p>");
		}
		Session->ClearMetaEngine(GetId());
	}
	catch(RException& e)
	{
		html.WriteP(0,"Error: "+e.GetMsg());
	}
	catch(...)
	{
		html.WriteP(0,"Unknown Error");
	}
	html.Write(0,"\t<hr width='95%%' align='center'/>\r\n");
	html.WriteP(0,"Process: "+RString::Number(getpid())+"; Thread: "+RString::Number(GetId()),"font-size: 60%;");
}


//------------------------------------------------------------------------------
void CGIThread::Run(void)
{
	int rc;
	FCGX_Request request;

	FCGX_InitRequest(&request, 0, 0);
	for(;;)
	{
		static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;

		// Some platforms require accept() serialization, some don't..
		pthread_mutex_lock(&accept_mutex);
		rc = FCGX_Accept_r(&request);
		pthread_mutex_unlock(&accept_mutex);

		if(rc<0)
			break;

		App->Write("Query send to process "+RString::Number(getpid())+" and thread "+RString::Number(GetId()));

		// Parse the things
		RString Session,Param;
		HTML html(request);
		switch(ParseQuery(request,Session,Param))
		{
			case cNoCmd:
				html.WriteTitle("Error");
				html.WriteH1("Error");
				html.WriteP(0,Param);
				break;
			case cSearch:
				Search(html,Session,Param);
				break;
			case cTest:
			{
				char* server_name(FCGX_GetParam("SERVER_NAME", request.envp));
				html.WriteTitle("Test - MediSearch");
				html.WriteH1("Information");
				html.WriteP(0,"Process: "+RString::Number(getpid()));
				html.WriteP(0,"Thread: "+RString::Number(GetId()));
				html.WriteP(0,"Server: "+RString(server_name ? server_name : "?"));
				html.WriteH1("Request");
				WriteEnv(html,request.envp);
				html.WriteH1("General");
				WriteEnv(html,environ);
				break;
			}
		}
	 }
}

