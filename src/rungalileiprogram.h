/*

	GALILEI Research Project

	RunGALILEIProgram.h

	Command - Header.

	Copyright 2009 by Pascal Francq (pascal@francq.info).
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
#ifndef RunGALILEIProgramH
#define RunGALILEIProgramH


//------------------------------------------------------------------------------
// include files for GALILEI Project
#include <galilei.h>
#include <ggalileiapp.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
// include files for current project
#include <command.h>


//------------------------------------------------------------------------------
class RunGALILEIProgram : public GGALILEIApp, public GSlot
{
	RContainer<RString,true,false> Params;
	RContainer<Command,true,true> Cmds;

public:

	RString CurCat;
	RString CurList;
	RString CurPlugin;

public:

	RunGALILEIProgram(int argc, char *argv[]);

	void Run(void);

	/**
	* The treatment for a specific document will begin.
	* @param doc             Document.
	*/
	virtual void NextDoc(const GDoc* doc);

	/**
	* The treatment for a specific document will begin.
	* @param prof            Profile.
	*/
	virtual void NextProfile(const GProfile* prof);

	/**
	* Method called when executing a sequence of instruction to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const R::RString& str);

	/**
	* Start a job.
	* @param job             Description of the job.
	*/
	virtual void StartJob(const R::RString job);

	/**
	* Forward a warning.
	* @param msg             Message.
	*/
	virtual void Warning(R::RString msg);

	/**
	* Forward an error.
	* @param msg             Message.
	*/
	virtual void Error(R::RString msg);

	/**
	* Forward an alert.
	* @param msg             Message.
	*/
	virtual void Alert(R::RString msg);

	void Help(void);
};



//------------------------------------------------------------------------------
#endif
