/*

	GALILEI Research Project

	GSubProfiles.cpp

	List of SubProfiles for a given Language - Implementation.

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
// include standard api files
#include <math.h>


//------------------------------------------------------------------------------
// include files for R
#include <rvectorint.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gprofilesagree.h>
#include <gprofile.h>



//------------------------------------------------------------------------------
//
// class GProfilesAgreement
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double GProfilesAgreement::Compute(GProfile* prof1,GProfile* prof2)
{
	double agree=0.0;
	unsigned int nbsame;
	double nbcommon;

	nbsame=prof1->GetCommonOKDocs(prof2);
	nbcommon=double(prof1->GetCommonDocs(prof2));
	if(nbcommon>=MinDocs)
		agree=nbsame/nbcommon;
	return(agree);
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles Agreements","Count Method",GProfilesAgreement)
