/*

	GALILEI Research Project

	PointOfView.cpp

	Compare a ideal groupement with a computed one - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Stphane Rideau (srideau@ulb.ac.be).

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



// ----------------------------------------------------------------------------
//include file for GALILEI
#include <postgroups/gcpov.h>
#include <postgroups/gexpertkwds.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
#include <profiles/gprofdoc.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofilevector.h>
#include <langs/glang.h>
#include <langs/gdict.h>
#include <docs/gdocvector.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rrecfile.h>
#include <rstd/rcursor.h>
#include <rdb/rmysql.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class GCpov
//
//-----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
GALILEI::GCpov::GCpov()
{
	Id = GetId();
	IdfFactor = 0.0;
	Idw = 0;
	Rdist = 0.0;
	NbParent = 50;    // a parametrer dans input
       
	IsInDocs = new RContainer<GExpertKwds,unsigned int,false,true>(10,5);
}


// ----------------------------------------------------------------------------
GALILEI::GCpov::GCpov(unsigned int id)
   : Id(id)
{
	IdfFactor = 0.0;
	Idw = 0;
	Rdist = 0.0;
	NbParent = 50;    // a parametrer dans input
          
	IsInDocs = new RContainer<GExpertKwds,unsigned int,false,true>(10,5);
}



   
// ----------------------------------------------------------------------------
void GALILEI::GCpov::InsertIdfFactor(double idfvalue)
{
	IdfFactor = idfvalue;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::InsertIdw(int kwdsvalue)
{
	Idw = kwdsvalue;
}


// ----------------------------------------------------------------------------
double GALILEI::GCpov::Getidfac()
{
	return(IdfFactor);
}


// ----------------------------------------------------------------------------
unsigned int GALILEI::GCpov::GetIdw()
{
	return(Idw);
}


// ----------------------------------------------------------------------------
unsigned int GALILEI::GCpov::GetId() const
{
	return(Id);
}

// ----------------------------------------------------------------------------
void GALILEI::GCpov::SetId(unsigned int id)
{
	Id = id;
}

// ----------------------------------------------------------------------------
int GALILEI::GCpov::Compare(const GCpov* n) const
{
	return(Id-(n->GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::Compare(const GCpov& n) const
{
	return(Id-(n.GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::Compare(const unsigned id) const
{
	return(Id-id);
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::SetRdist(double dr)
{
	Rdist = dr;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::IncrId()
{
	Id++;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::SetNbParent(int NbParents)
{
	NbParent = NbParents;
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::GetNbParent(void)
{
	return NbParent;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::CleanParent()
{
	int i,j;

	for(i = 0; i < NbParent; i++)
		for(j = 0; j < 2; j++)
			Parent[i][j] = 0;
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::AddParent(int value, int freq)
{
	int i;

	for(i = 0; i < NbParent; i++)
	{
		if(Parent[i][0]==0)
		{
			Parent[i][0] = value;
			Parent[i][1] = freq;
			return 1;
		}
	}
	return 0;     
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::RemplaceParent(int position, int value, int freq)
{
	Parent[position][0] = value;
	Parent[position][1] = freq;
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::AddRemplaceParent(int IdParent, int freq )
{
	int i;

	for( i = 0; i < NbParent; i++)
	{
		if(Parent[i][1] < freq)
		{
			Parent[i][0] = IdParent;
			Parent[i][1] = freq;
			break;
		}
	}
}


// ----------------------------------------------------------------------------
void GALILEI::GCpov::SetWord(char *w)
{
	memset(TheWord,0,strlen(TheWord));
	strcpy(TheWord,w);
}

char * GALILEI::GCpov::GetWord(void)
{
   return TheWord;
}



// ----------------------------------------------------------------------------
int GALILEI::GCpov::GetOneParent(int position)
{
	return Parent[position][0];
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::GetFreqParent(int position)
{
	return Parent[position][1];
}


// ----------------------------------------------------------------------------
int GALILEI::GCpov::GetAllParent(void)
{
	return 1;
}


