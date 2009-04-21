/*

	GALILEI Research Project

	GBasicSession.h

	Basic Session - Implementation.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
// include files GALILEI
#include <gbasicsession.h>
#include <ggalileiapp.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GBasicSession
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GBasicSession::GBasicSession(void)
	: Storage(0), SaveResults(true)
{
	GFactoryStorage* fac(GALILEIApp->GetManager<GStorageManager>("Storage")->GetCurrentFactory());
	if(!fac)
		throw GException("No Storage");

	// Delete the storage if necessary and re-create it
	if(fac->GetPlugin())
		fac->Delete();
	fac->Create();
	Storage=fac->GetPlugin();
}


//------------------------------------------------------------------------------
GStorage* GBasicSession::GetStorage(void) const
{
	if(!Storage)
		throw GException("No storage");
	return(Storage);
}


//------------------------------------------------------------------------------
GBasicSession::~GBasicSession(void)
{
	// Delete the storage
	Storage->GetFactory()->Delete();
}
