/*
	GALILEI Research Project

	GStatProfDoc.cpp

	Statistics on doc juged by the profiles. - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery(vavdaele@ulb.ac.be)

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
//include file for GALLILEI
#include <gstatprofdoc.h>

#include <infos/glangs.h>
#include <infos/glang.h>
#include <docs/gdoc.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GStatProfDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GStatProfDoc::GStatProfDoc(GSession* ses,R::RTextFile* f)
	: Session(ses), File(f)
{
//	if(File)
//	{
//		(*File)<<"-------------------------------------------------------------------------------------------------"<<endl;
//		(*File)<<"AVG_PJSD : Average number of rofiles having Juged the Same Document."<<endl;;
//		(*File)<<"AVG_PAR : Average of Profiles Agreement Ration."<<endl;
//		(*File)<<"AVG_PDR : Average of Profiles Desagreement Ration."<<endl;
//		(*File)<<"-------------------------------------------------------------------------------------------------"<<endl<<endl;
//		(*File)<<"AVG_PJSD	--	AVG_PAR	--	AVG_PDR"<<endl;
//		(*File)<<endl;
//	}
}


//-----------------------------------------------------------------------------
void GALILEI::GStatProfDoc::WriteLine(void)
{
	if(File)
		(*File)<<"-------------------------------------------------------------------------------------------------"<<endl<<endl;
}


//-----------------------------------------------------------------------------
void GALILEI::GStatProfDoc::Run(void)
{
	GFactoryLangCursor Langs;
	GLang* lang;
	GProfileCursor Profs1,Profs2;
	GDocCursor Docs;

	unsigned int nbProfJugDoc,nbDocs ;
	unsigned int i,j;
	double sum, tmp,nbSame,nbDiff;


	//Initialization
	nbProfJugDoc=nbDocs=0;
	i=j=0;
	sum=tmp=nbSame=nbDiff=0.0;
	MeanNbProf=MeanSame=MeanDiff=0.0;

	Docs = Session->GetDocsCursor();
	Profs1= Session->GetProfilesCursor();
	Profs2= Session->GetProfilesCursor();
	Langs=Session->GetLangs()->GetLangsCursor();

	// Compute the average of number of profiles having juged the same doc.

	for (Docs.Start();!Docs.End(); Docs.Next())
	{
		nbProfJugDoc=0;
		for (Profs1.Start();!Profs1.End(); Profs1.Next())
		{
			// If the profile is defined -> compter +1
			if (Profs1()->GetFeedback( Docs() ))
			{
				nbProfJugDoc++;
			}
		}
		sum += nbProfJugDoc;
		//(*File) << Docs()->GetId()<<"     "<<nbProfJugDoc<<endl;
		nbDocs++;
	}

	MeanNbProf = sum/ nbDocs;
	cout << " Mean of profiles juged doc "<<MeanNbProf <<endl;


	// compute Agreement-Desagreement Ratio
	sum=0;
	for (Langs.Start(); !Langs.End(); Langs.Next() )
	{
		lang=Langs()->GetPlugin();
		if(!lang) continue;
		for(Profs1.Start(),i=0,j=Profs1.GetNb();--j;Profs1.Next(),i++)
		{
//			if(!Profs()->GetSubProfile()->GetProfile()->IsSocial())
//				NoSocialSubProfiles.InsertPtr(Profs1());
			for(Profs2.GoTo(i+1);!Profs2.End();Profs2.Next())
			{
				tmp=Profs1()->GetSubProfile(lang)->GetCommonDocs(Profs2()->GetSubProfile(lang));
				nbSame=Profs1()->GetSubProfile(lang)->GetCommonOKDocs(Profs2()->GetSubProfile(lang));
				nbDiff=Profs1()->GetSubProfile(lang)->GetCommonDiffDocs(Profs2()->GetSubProfile(lang));

				if(tmp)
				{
					nbSame /=tmp;
					MeanSame += nbSame;
				
					nbDiff /= tmp;
					MeanDiff += nbDiff;
					sum++;
				}
			}
		}
	}
	MeanSame /= sum;
	MeanDiff /=sum;

	if(File)
		(*File)<< MeanNbProf<<"		"<<MeanSame<<"		"<<MeanDiff<<endl;
}


//-----------------------------------------------------------------------------
GALILEI::GStatProfDoc::~GStatProfDoc(void)
{
}


