/*

	GALILEI Research Project

	Main.cpp

	Main program that updates GALILEI - Implementation.

	Copyright 1999-2010 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for current project
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <iostream>
#include <cstdlib>
#include <stdexcept>
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <rxmlfile.h>
#include <rxmlstruct.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggalileiprg.h>
#include <gslotlog.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
// include files for current project
#include <rungalileiprogram.h>



//------------------------------------------------------------------------------
//
// Main
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	setlocale(LC_CTYPE,"");
	RunGALILEIProgram App(argc,argv);
	App.Run();
	return(EXIT_SUCCESS);
}
