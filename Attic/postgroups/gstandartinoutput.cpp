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
#include <postgroups/gstandartinoutput.h>
//#include <sessions/gsessionmysql.h>
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
// class GStandardInOutPut
//
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
GALILEI::GStandardInOutPut::GStandardInOutPut(void)
{
	GroupId = 0;
	SProfile = NULL;
	Idw = 0;
	memset(Parent,0,sizeof(Parent));
	Level = 0; 
}


// ----------------------------------------------------------------------------
GALILEI::GStandardInOutPut::GStandardInOutPut(GStandardInOutPut *tmp)
{
int i,j;

  Id = tmp->Id;
  //GroupId = tmp->GroupId;
  SProfile = tmp->SProfile;
 // Grp = tmp->Grp;
  Idw = tmp->Idw;
  strcpy(Word,tmp->Word);
  for(i = 0; i< 50; i++)
  	for(j = 0; j < 50; j++)
   		Parent[i][j] = tmp->Parent[i][j];
	Level = tmp->Level;
	strcpy(DocRef,tmp->DocRef);
  
}


// ----------------------------------------------------------------------------
int GALILEI::GStandardInOutPut::Compare(const GStandardInOutPut* n) const
{
	return(Id-(n->GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::GStandardInOutPut::Compare(const GStandardInOutPut& n) const
{
	return(Id-(n.GetId()));
}


// ----------------------------------------------------------------------------
int GALILEI::GStandardInOutPut::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
unsigned GALILEI::GStandardInOutPut::GetId() const
{
	return(Id);
}


//-----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::SetId(unsigned int id)
{
	Id = id;
}

// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::SetIdw(unsigned int id)
{
	Idw = id;
}


// ----------------------------------------------------------------------------
unsigned int GALILEI::GStandardInOutPut::GetIdw(void)
{
	return Idw;
}

// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::SetParent(int position, int value, int freq)
{
	Parent[position][0] = value;
	Parent[position][1] = freq;
}

// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::SetProfile(GSubProfile *sprofile)
{
	SProfile = sprofile;
}

// ----------------------------------------------------------------------------
GSubProfile * GALILEI::GStandardInOutPut::GetCurProfile(void)
{
	return SProfile;
}


// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::SetGroup(GGroup * grp)
{
	Grp = grp;
}


// ----------------------------------------------------------------------------
GGroup * GALILEI::GStandardInOutPut::GetGroup(void)
{
	return Grp;
}


// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::SetLevel(int level)
{
	Level = level;
}


// ----------------------------------------------------------------------------
int GALILEI::GStandardInOutPut::GetLevel(void)
{
	return Level;
}


// ----------------------------------------------------------------------------
char * GALILEI::GStandardInOutPut::GetFreq(void)
{
	return Freq;
}


// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::AddFreq(int freq)
{
  if(strlen(Freq) > 0)
		strcat(Freq,"/");
	strcat(Freq,itou(freq));
}


// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::CleanFreq(void)
{
	memset(Freq,0,129);
}


// ----------------------------------------------------------------------------
char * GALILEI::GStandardInOutPut::GetParent(void)
{
	return Parents;
}


// ----------------------------------------------------------------------------
int GALILEI::GStandardInOutPut::GetParentPos(int position)
{
	if(position < 50)
		return Parent[position][0];
	else
		return 0;
}


// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::AddParent(int par)
{
	if(strlen(Parents) > 0)
		strcat(Parents,"/");
	strcat(Parents,itou(par));
}


// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::CleanParent(void)
{
	memset(Parents,0,129);
}


// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::SetWord(char*str)
{
	memset(Word,0,129);
	strcpy(Word,str);
}


// ----------------------------------------------------------------------------
char * GALILEI::GStandardInOutPut::GetWord()
{
	return Word;
}

// ----------------------------------------------------------------------------
void GALILEI::GStandardInOutPut::SetDocRef(char *docref)
{
	memset(DocRef,0,256);
	strcpy(DocRef,docref);
}

// ----------------------------------------------------------------------------
char * GALILEI::GStandardInOutPut::GetDocRef(void)
{
	 return DocRef;
}



// ----------------------------------------------------------------------------
GALILEI::GStandardInOutPut::~GStandardInOutPut(void)
{
}






