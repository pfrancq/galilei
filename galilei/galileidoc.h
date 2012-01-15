/*

	GALILEI Research Project

	GALILEIDoc.h

	GALILEI Documentation - Header.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef GALILEIDOCH
#define GALILEIDOCH


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * \mainpage GALILEI Research Project
 *
 * \section intro_sec Introduction
 *
 * GALILEI is a research and development platform in the field of information
 * science.
 *
 * \section galilei_main_classes_sec Main classes
 * The main classes are:
 * - GGALILEIApp.
 * - GSession.
 *
 * A new application using the GALILEI platform should define a class
 * inheriting from GGALILEIApp. This class will manage the plug-ins and their
 * configurations. A global variable GALILEIApp is created.
 *
 * To work on a given "world" (a given set of documents and profiles), the
 * application should create a session through GALILEIApp.CreateSession. To
 * delete the session, GALILEIApp.DeleteSession should be called.
 */


//------------------------------------------------------------------------------
/**
* \namespace GALILEI
* \brief GALILEI Classes.
*
* This namespace declares all the classes implemented during the GALILEI
* project.
*/


}  //-------- End of namespace GALILEI -----------------------------------------

//------------------------------------------------------------------------------
#endif
