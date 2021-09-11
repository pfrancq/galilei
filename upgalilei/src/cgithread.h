/*

	GALILEI Research Project

	CGIThread.h

	CGI Thread - Header.

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
#ifndef CGIThreadH
#define CGIThreadH


//------------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <galilei.h>
#include <rthread.h>


//------------------------------------------------------------------------------
// forward declaration
struct FCGX_Request;
class HTML;
class RunGALILEIProgram;

//------------------------------------------------------------------------------
/**
 * The CGIThread class provides a thread to process a CGI call.
 * @short CGI Thread
 */
class CGIThread : public R::RThread
{
	enum tCmd {cNoCmd,cSearch,cTest};

	/**
	 * GALILEI App.
	 */
	RunGALILEIProgram* App;

public:

	/**
	 * Construct the thread.
    * @param app            Application.
    * @param id             Identifier of the thread.
    */
   CGIThread(RunGALILEIProgram* app,int id);

private:

	/**
	 *
    * @return
    */
	bool LocalCall(FCGX_Request& request);

	/**
	 *
    * @param ptr
    * @param cmd
    */
	void ParseParam(char* &ptr,R::RString& cmd);

	/**
	 *
    * @param ptr
    * @param val
    */
	void ParseValue(char* &ptr,R::RString& val);

	/**
	 * Parse the request to find the session name, the command name and,
	 * eventually some parameters.
    * @param request        Request received.
    * @param session        Session (set by the method).
    * @param param          Parameters (set by the method).
    * @return the type of command or nothing if an error occurs.
    */
	tCmd ParseQuery(FCGX_Request& request,R::RString& session,R::RString& param);

	/**
	 * Write environment variables.
    * @param html           HTML.
    * @param envp           Environment variables.
    */
	void WriteEnv(HTML& html,const char* const* envp);

	/**
	 * Perform a search.
	 * @param html           HTML.
	 * @param session        Name of the session.
	 * @param query          Query.
	 */
	void Search(HTML& html,const R::RString& session,const R::RString& query);

	/**
	 * Run the thread. In practice, it waits until a request is send. It parses
	 * the request and treat it if it is a "search" or a "test".
    */
   virtual void Run(void);
};


//------------------------------------------------------------------------------
#endif
