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
#include <postgroups/gdispatchpov.h>
#include <postgroups/galgobase.h>
using namespace GALILEI;


// //-----------------------------------------------------------------------------
// include files for R Project
 #include <rstd/rcontainer.h>
 using namespace R;



//-----------------------------------------------------------------------------
//
// class GDispatchpov
//
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void GALILEI::GDispatchpov::GeneratePoV(RContainer<GGroups,unsigned int,true,true>* /*groups*/)
{

}


// ----------------------------------------------------------------------------
void GALILEI::GDispatchpov::GeneratePoVfromScratch(GGroup* Grp, GSession *s)
{

	//InputBase *fomatIn;
	//InOutputBase *fomatOut;
	GAlgobase *Calcul;

	cout << "GeneratePoVfromScratch" << endl;   
   
	Calcul = new GAlgobase();
	Calcul->BuildPoVGroup(Grp, s);

}

                                               
// ----------------------------------------------------------------------------
void GALILEI::GDispatchpov::UpdatePovForAPrf(GSubProfile *prfle, GInOutputBase*  /*datainput*/, GSession *s)
{
	GAlgobase *Calcul;

	Calcul = new GAlgobase();

	// test d update ou non ici.... a voir
	// on conserve datainoutput pour des mises a jours conditionnelles ulterieures.
	// force le recalcul dans les parametres.
	Calcul->Ccur = prfle->GetVectorCursor();
	Calcul->SetCurPrf(prfle);
	Calcul->Lng = prfle->GetLang();
	Calcul->AnalysePoVProfile(s);
	Calcul->FormatOutData(s);

	// ecriture base
	cout << "Update Pov ended" << endl;

	
}



