/*

	GALILEI Research Project

	GPrgInstFor.cpp

	"for" Instruction - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gprginstfor.h>
#include <sessions/gsessionprg.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GPrgInstFor
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgInstFor::GPrgInstFor(char* line) throw(bad_alloc)
	: GPrgInst(), Values(20,10), Insts(50,25)
{
	char* var;
	GPrgVar* r;

	// read name of variable
	var=line;
	while((*line)&&((*line)!=' '))
		line++;
	(*(line++))=0;
	Var=var;

	// Skip "in "
	while((*line)&&((*line)!=' '))
		line++;
	(*(line++))=0;
	while((*line)&&((*line)==' '))
		line++;

	// Read Values
	while((*line))
	{
		r=GSessionPrg::AnalyseParam(0,line);
		if(r)
			Values.InsertPtr(r);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GPrgInstFor::AddInst(GPrgInst* ins) throw(bad_alloc)
{
	Insts.InsertPtr(ins);
}


//-----------------------------------------------------------------------------
void GALILEI::GPrgInstFor::Run(GSessionPrg* prg,GSlot* r) throw(GException)
{
	for(Values.Start();!Values.End();Values.Next())
	{
		for(Insts.Start();!Insts.End();Insts.Next())
		{
			Insts()->Run(prg,r);
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GPrgInstFor::~GPrgInstFor(void)
{
}
