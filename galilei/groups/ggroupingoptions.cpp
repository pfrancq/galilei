/*

	GALILEI Research Project

	GGroupingOptions.cpp

	Options for the grouping methods - Implementation.

	(C) 2002 by P. Francq.

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
// include files for GALILEI
#include <groups/ggroupingoptions.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GGroupingOptions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupingOptions::GGroupingOptions(void)
	: Method(pgNothing), FullSim(true), LevelSim(0.4), GAPopSize(16),
	  GAMaxGen(20), GAStep(false), GAStepGen(10)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingOptions::GGroupingOptions(const GGroupingOptions& opt)
	: Method(opt.Method), FullSim(opt.FullSim), LevelSim(opt.LevelSim),
	  GAPopSize(opt.GAPopSize),GAMaxGen(opt.GAMaxGen), GAStep(opt.GAStep),
	  GAStepGen(opt.GAStepGen)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroupingOptions::GGroupingOptions(const GGroupingOptions* opt)
	: Method(opt->Method), FullSim(opt->FullSim), LevelSim(opt->LevelSim),
	  GAPopSize(opt->GAPopSize),GAMaxGen(opt->GAMaxGen), GAStep(opt->GAStep),
	  GAStepGen(opt->GAStepGen)
{
}


//-----------------------------------------------------------------------------
GGroupingOptions& GALILEI::GGroupingOptions::operator=(const GGroupingOptions& opt)
{
	Method=opt.Method;
	FullSim=opt.FullSim;
	LevelSim=opt.LevelSim;
	GAPopSize=opt.GAPopSize;
	GAMaxGen=opt.GAMaxGen;
	GAStep=opt.GAStep;
	GAStepGen=opt.GAStepGen;
	return(*this);
}
