/*

	GALILEI Research Project

	GFilterManagerKDE.cpp

	Manager to handle URL file in a KDE environment - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <gfiltermanagerkde.h>
#include <docs/gfilter.h>
#include <frontend/kde/rqt.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for qt
#include <qstring.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kio/netaccess.h>
#include <kmimemagic.h>



//-----------------------------------------------------------------------------
//
// class GFilterManagerKDE
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GFilterManagerKDE::GFilterManagerKDE(void)
	: GFilterManager()
{
}


//-----------------------------------------------------------------------------
void GALILEI::GFilterManagerKDE::Download(const char* URL,RString& tmpFile)
{
	QString tmp("");

	if(!KIO::NetAccess::download(KURL(URL),tmp))
		throw GException(RString("Can't download url")+URL);

	tmpFile=tmp.latin1();
}


//-----------------------------------------------------------------------------
const char* GALILEI::GFilterManagerKDE::DetermineMIMEType(const char* tmpfile)
{
	const char* res;

	res=GFilterManager::DetermineMIMEType(tmpfile);
	if(res)
		return(res);
	KMimeMagicResult *result = KMimeMagic::self()->findFileType(tmpfile);
	if((!result)||(!result->isValid()))
		return(0);
	return(result->mimeType().latin1());
}


//-----------------------------------------------------------------------------
void GALILEI::GFilterManagerKDE::Delete(RString& tmpFile)
{
	KIO::NetAccess::removeTempFile(ToQString(tmpFile));
}


//-----------------------------------------------------------------------------
GALILEI::GFilterManagerKDE::~GFilterManagerKDE(void)
{
}
