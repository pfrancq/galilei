/*

	GALILEI Research Project

	RunGALILEIProgram.h

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
#ifndef RunGALILEIProgramH
#define RunGALILEIProgramH


//------------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <ggalileiapp.h>


//------------------------------------------------------------------------------
// include files for current project
#include <cgithread.h>


//------------------------------------------------------------------------------
// Constant
const size_t NbThreads=20;


//------------------------------------------------------------------------------
class RunGALILEIProgram : public GALILEI::GGALILEIApp
{
	/**
	 * Threads.
	 */
	CGIThread* Threads[NbThreads];

	/**
	 * Debug file.
	 */
	R::RTextFile Debug;

	/**
	 * Mutex on the debug file.
    */
	R::RMutex mDebug;

public:

	/**
	 * Constructor of the application.
    * @param argc           Number of arguments.
    * @param argv           Arguments.
    */
	RunGALILEIProgram(int argc, char *argv[],const R::RURI& debug=R::RString::Null);

	/**
	 * Write some string in the debug file if it is open.
    * @param str
    */
	void Write(const R::RString& str);

	/**
	 * Initialise the application.
    */
	virtual void Init(void);

	/**
	 * Run the application
    */
	virtual void Run(void);
};



//------------------------------------------------------------------------------
#endif
