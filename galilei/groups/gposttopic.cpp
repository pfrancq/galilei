/*

	GALILEI Research Project

	GPostTopic.cpp

	Generic Post-Topic Computing Method - Implementation.

	Copyright 2003-2008 by the Université Libre de Bruxelles.

	Authors:
		Francq Pascal (pfrancq@ulb.ac.be)
		Vandaele Valéry(vavdaele@ulb.ac.be).


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
#include <gposttopic.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GPostTopic
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPostTopic::GPostTopic(GFactoryPostTopic* fac)
	: GPlugin<GFactoryPostTopic>(fac)
{
}


//------------------------------------------------------------------------------
GPostTopic::~GPostTopic(void)
{
}



//------------------------------------------------------------------------------
//
// class GPostTopicManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPostTopicManager::GPostTopicManager(void)
	: GPluginManager<GPostTopicManager,GFactoryPostTopic,GPostTopic>("PostTopic",API_POSTTOPIC_VERSION,ptOrdered)
{
}


//------------------------------------------------------------------------------
GPostTopicManager::~GPostTopicManager(void)
{
}
