/*

	GALILEI Research Project

	Command.h

	Command - Header.

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
#ifndef CommandH
#define CommandH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rdbmysql.h>
using namespace R;


//------------------------------------------------------------------------------
// forward declaration
class RunGALILEIProgram;


//------------------------------------------------------------------------------
class Command
{
protected:

	/**
	 * Name of the command.
	 */
	RString Name;

	/**
	 * Description of the command.
	 */
	RString Desc;

	/**
	 * Program.
	 */
	RunGALILEIProgram* Prg;

public:

	/**
	 * Construct a new command.
	 * @param prg            GALILEI Program.
	 * @param name           Name of the command.
	 * @param desc           Description of the command.
	 * @return
	 */
	Command(RunGALILEIProgram* prg,const RString& name,const RString& desc);

	/**
	 * Run the command.
	 * @param params         Parameters.
	 */
	virtual void Run(const RContainer<RString,true,false>& params)=0;

	int Compare(const Command& cmd) const;
	int Compare(const RString& cmd) const;

	/**
	 * Destruct the command.
	 * @return
	 */
	virtual ~Command(void);
};


//------------------------------------------------------------------------------
class CreateMySQL : public Command
{
public:
	CreateMySQL(RunGALILEIProgram* prg) : Command(prg,"createmysql","") {}
	RString GetConceptType(const RString& name,const RString& desc,RDb* Db);
	virtual void Run(const RContainer<RString,true,false>& params);
};


class Show : public Command
{
public:
	Show(RunGALILEIProgram* prg) : Command(prg,"show","") {}
	virtual void Run(const RContainer<RString,true,false>& params);
};


class Set : public Command
{
public:
	Set(RunGALILEIProgram* prg) : Command(prg,"set","") {}
	virtual void Run(const RContainer<RString,true,false>& params);
};


//------------------------------------------------------------------------------
#endif
