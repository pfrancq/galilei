/*

	GALILEI Research Project

	GProfilesDocsSimsCosinus.cpp

	Similarities between documents and subprofiles - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

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
#include <gsubprofilesdocssims.h>


//------------------------------------------------------------------------------
//
// class GSubProfilesDocsSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double GSubProfilesDocsSims::Compute(GLang* lang,size_t id1,size_t id2)
{
	GSubProfile* sub=Session->GetSubProfile(lang,id1,true,false);
	GDoc* doc=Session->GetDoc(id2,true,false);

	if(doc->GetLang()!=sub->GetLang())
		throw GException("Cannot compare a document and group of a different language");
	return(sub->SimilarityIFF2(*doc,otSubProfile,otDoc,lang));	
}

//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Profiles/Documents Similarities","Cosinus Method",GSubProfilesDocsSims)
