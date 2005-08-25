/*

	GALILEI Research Project

	GProfilesDocsSims.cpp

	Generic Computing of Similarities between profiles and documents - Implementation.

	Copyright 2003-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

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
// include files for GALILEI
#include <gprofilesdocssims.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GProfilesDocsSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesDocsSims::GProfilesDocsSims(GFactoryProfilesDocsSims* fac)
	: GPlugin<GFactoryProfilesDocsSims>(fac)
{
}


//------------------------------------------------------------------------------
GProfilesDocsSims::~GProfilesDocsSims(void)
{
}



//------------------------------------------------------------------------------
//
// class GProfilesDocsSimsManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesDocsSimsManager::GProfilesDocsSimsManager(void)
	: GPluginManager<GProfilesDocsSimsManager,GFactoryProfilesDocsSims,GProfilesDocsSims>("ProfilesDocsSims",API_PROFILESDOCSSIMS_VERSION,ptSelect)
{
}


//------------------------------------------------------------------------------
double GProfilesDocsSimsManager::GetSimilarity(const GDoc* doc,const GSubProfile* sub)
{
	if(!Current)
		throw GException("No profiles/documents similarities plug-in selected");
	return(Current->GetSimilarity(doc,sub));
}


//------------------------------------------------------------------------------
double GProfilesDocsSimsManager::GetMinSimilarity(const GLang* lang)
{
	if(!Current)
		throw GException("No profiles/documents similarities plug-in selected");
	return(Current->GetMinSimilarity(lang));
}


//------------------------------------------------------------------------------
GProfilesDocsSimsManager::~GProfilesDocsSimsManager(void)
{
}
