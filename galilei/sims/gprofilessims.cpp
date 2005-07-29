/*

	GALILEI Research Project

	GProfilesSims.cpp

	Generic Computing of Similarities between profiles - Implementation.

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
#include <gprofilessims.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GProfilesSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesSims::GProfilesSims(GFactoryProfilesSims* fac)
	: GPlugin<GFactoryProfilesSims>(fac)
{
}


//------------------------------------------------------------------------------
GProfilesSims::~GProfilesSims(void)
{
}



//------------------------------------------------------------------------------
//
// class GProfilesSimsManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfilesSimsManager::GProfilesSimsManager(void)
	: GPluginManager<GProfilesSimsManager,GFactoryProfilesSims,GProfilesSims>("ProfilesSims",API_PROFILESSIMS_VERSION,ptSelect)
{
}


//------------------------------------------------------------------------------
void GProfilesSimsManager::Connect(GSession* session)
{
	GPluginManager<GProfilesSimsManager,GFactoryProfilesSims,GProfilesSims>::Connect(session);
	if(session)
		session->SetSims(this);
}


//------------------------------------------------------------------------------
void GProfilesSimsManager::Disconnect(GSession* session)
{
	GPluginManager<GProfilesSimsManager,GFactoryProfilesSims,GProfilesSims>::Disconnect(session);
}


//------------------------------------------------------------------------------
double GProfilesSimsManager::GetSimilarity(const GSubProfile* sub1,const GSubProfile* sub2)
{
	if(!Current)
		throw GException("No profiles similarities plug-in selected");
	return(Current->GetSimilarity(sub1,sub2));
}


//------------------------------------------------------------------------------
double GProfilesSimsManager::GetMinSimilarity(const GLang* lang)
{
	if(!Current)
		throw GException("No profiles similarities plug-in selected");
	return(Current->GetMinSimilarity(lang));
}


//------------------------------------------------------------------------------
double GProfilesSimsManager::GetDisagreementRatio(const GSubProfile* sub1,const GSubProfile* sub2)
{
	if(!Current)
		throw GException("No profiles similarities plug-in selected");
	return(Current->GetDisagreementRatio(sub1,sub2));
}


//------------------------------------------------------------------------------
double GProfilesSimsManager::GetMinDisagreementRatio(const GLang* lang)
{
	if(!Current)
		throw GException("No profiles similarities plug-in selected");
	return(Current->GetMinDisagreementRatio(lang));
}


//------------------------------------------------------------------------------
double GProfilesSimsManager::GetAgreementRatio(const GSubProfile* sub1,const GSubProfile* sub2)
{
	if(!Current)
		throw GException("No profiles similarities plug-in selected");
	return(Current->GetAgreementRatio(sub1,sub2));
}


//------------------------------------------------------------------------------
double GProfilesSimsManager::GetMinAgreementRatio(const GLang* lang)
{
	if(!Current)
		throw GException("No profiles similarities plug-in selected");
	return(Current->GetMinAgreementRatio(lang));
}


//------------------------------------------------------------------------------
GProfilesSimsManager::~GProfilesSimsManager(void)
{
}
