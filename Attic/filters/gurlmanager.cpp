/*

	R Project Library

	gurlmanager.cpp

	Generic Manager to handle URL file - Implementation.

	(C) 2001 by Pascal Francq

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
#include <urlmanagers/gurlmanager.h>
using namespace GALILEI;




//-----------------------------------------------------------------------------
//
// class GURLManager
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GURLManager::GURLManager(void)
{
}


//-----------------------------------------------------------------------------
bool GALILEI::GURLManager::Download(const char* URL,RString& tmpFile)
{
	return(false);
}


//-----------------------------------------------------------------------------
void GALILEI::GURLManager::Delete(RString& tmpFile)
{
}


//-----------------------------------------------------------------------------
GDocXML* GALILEI::GURLManager::CreateDocXML(const char* URL,const char* mime)
{
	RString tmpFile(50);
	char* ptr;
	bool toDown;
	int i;

	// Verify it is a local file (file:/) or nothing.
	ptr=URL;
	i=0;
	while((*ptr)&&(isalnum(*ptr))
	{
		i++;
		ptr++;
	}
	if(i&&((*ptr)==':')&&(strncmp(ptr,"file",i)))
	{
		if(!Download(URL,tmpFile))
			return;
	}

	// Analyse it.

	// Delete it
	if(tmpFile.GetLen())
		Delete(tmpFile);
}


//-----------------------------------------------------------------------------
GDocXML* GALILEI::GURLManager::CreateDocXML(const char* URL)
{
}


//-----------------------------------------------------------------------------
GALILEI::GURLManager::~GURLManager(void)
{
}
