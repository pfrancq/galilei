/*

	GALILEI Research Project

	GSatSimId.cpp

	Save the different value important to calc stats on documents or profiles .Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#include <tests/gstatsimid.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GSatSimId
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GSatSimId::GSatSimId(unsigned int id,unsigned int grpid)
{
	Id=id;
	GrpId=grpid;
	MinIntra=1;
	MaxInter=0;
	MeanIntra=0;
	MeanInter=0;
	ComptIntra=0;
	ComptInter=0;
	ComptIntraFoire=0;
	ComptInterFoire=0;
}


//-----------------------------------------------------------------------------
void GALILEI::GSatSimId::SetSim(double Sim,unsigned int Grp)
{
	if ((Sim<1)&&(Sim>-1))
	{
		if(Grp==GrpId)
		{
			if(Sim<MinIntra)
			{
				MinIntra=Sim; 
			}
			MeanIntra+=Sim;
			ComptIntra++;
		}
		else
		{
 			if(Sim>MaxInter)
			{
				MaxInter=Sim;
			}
			MeanInter+=Sim;
			ComptInter++;
		}
	}
}


//-----------------------------------------------------------------------------
int GALILEI::GSatSimId::Compare(const GSatSimId& doc) const
{
	return(Id-doc.GetId());
}


//-----------------------------------------------------------------------------
int GALILEI::GSatSimId::Compare(const GSatSimId* doc) const
{
	return(Id-doc->GetId());
}


//-----------------------------------------------------------------------------
int GALILEI::GSatSimId::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
GALILEI::GSatSimId::~GSatSimId(void)
{
}
