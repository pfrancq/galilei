/*
	GALILEI Research Project

	GStatProfDoc.cpp

	Statistics on documents assessed by profiles - Implementation.

	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 by the Universit√© Libre de Bruxelles (ULB).

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
//include file for GALILEI
#include <glang.h>
#include <gdoc.h>
#include <gprofile.h>
#include <gsession.h>
#include <ggalileiapp.h>


//------------------------------------------------------------------------------
// include files for statistics
#include <gstatprofdoc.h>
using namespace GALILEI;
using namespace std;
using namespace R;



//-----------------------------------------------------------------------------
//
// GStatProfDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GStatProfDoc::GStatProfDoc(GSession* ses,R::RTextFile& f)
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
void GStatProfDoc::WriteLine(void)
{
	File<<"-------------------------------------------------------------------------------------------------"<<endl<<endl;
}


//-----------------------------------------------------------------------------
void GStatProfDoc::Run(void)
{
	//Initialization
	size_t nbProfJugDoc(0),nbDocs(0);
	size_t i(0),j(0);
	double sum(0.0);
	R::RCursor<GDoc> Docs(Session->GetObjs(pDoc));
	R::RCursor<GProfile> Profs1(Session->GetObjs(pProfile));
	R::RCursor<GProfile> Profs2(Session->GetObjs(pProfile));

	// Compute the average of number of profiles having juged the same doc.
	for(Docs.Start();!Docs.End(); Docs.Next())
	{
		nbProfJugDoc=0;
		for (Profs1.Start();!Profs1.End(); Profs1.Next())
		{
			// If the profile is defined -> compter +1
			if (Profs1()->GetFdbk(Docs()->GetId()))
			{
				nbProfJugDoc++;
			}
		}
		sum+=static_cast<double>(nbProfJugDoc);
		//(*File) << Docs()->GetId()<<"     "<<nbProfJugDoc<<endl;
		nbDocs++;
	}
	if(nbDocs)
	{
		MeanNbProf=sum/static_cast<double>(nbDocs);
		File<<"MeanNbProfAssess="<<MeanNbProf<<endl;
//	cout << " Mean of profiles juged doc "<<MeanNbProf <<endl;
	}

	// compute Agreement-Desagreement Ratio
	sum=0;
	for(Profs1.Start(),i=0,j=Profs1.GetNb();--j;Profs1.Next(),i++)
	{
//			if(!Profs()->GetSubProfile()->GetProfile()->IsSocial())
//				NoSocialSubProfiles.InsertPtr(Profs1());
		for(Profs2.GoTo(i+1);!Profs2.End();Profs2.Next())
		{
			MeanSame+=Profs1()->GetAgreementRatio(Profs2(),1);
			MeanDiff+=Profs1()->GetDisagreementRatio(Profs2(),1);
			sum++;
		}
	}

	if(sum)
	{
		MeanSame /= sum;
		MeanDiff /=sum;
	}

	File<< MeanNbProf<<"		"<<MeanSame<<"		"<<MeanDiff<<endl;
}
