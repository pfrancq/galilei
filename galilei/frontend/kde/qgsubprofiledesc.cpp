/*

	GALILEI Research Project

	QGSubProfileDesc.cpp

	Widget to show a generic subprofile description - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		

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
// include files for GALILEI
#include <qgsubprofiledesc.h>
#include <profiles/gsubprofile.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class QGSubProfileDesc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

GALILEI::QGSubProfileDesc::QGSubProfileDesc(QWidget* parent,GSession* session)
	: QWidget(parent), Session(session)
{
}


//-----------------------------------------------------------------------------
int GALILEI::QGSubProfileDesc::Compare(const QGSubProfileDesc* sub) const
{
	return(GetType()-sub->GetType());
}


//-----------------------------------------------------------------------------
int GALILEI::QGSubProfileDesc::Compare(const QGSubProfileDesc& sub) const
{
	return(GetType()-sub.GetType());
}


//-----------------------------------------------------------------------------
int GALILEI::QGSubProfileDesc::Compare(const tSubProfileDesc t) const
{
	return(GetType()-t);
}


//-----------------------------------------------------------------------------
GALILEI::QGSubProfileDesc::~QGSubProfileDesc(void)
{
}
