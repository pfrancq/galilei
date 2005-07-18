/*
	GALILEI Research Project

	GStatProfDoc.h

	Statistics on doc juged by the profiles. - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
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



//-----------------------------------------------------------------------------
#ifndef GStatProfDocH
#define GStatProfDocH


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GStatProfDoc class provides statistics on the documents juged by the profiles. 
* @author  Vandaele Valery
* @short Statisctics on profiles and documents.
*/
class GStatProfDoc
{
protected :

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* The mean of the number of profiles which has juged a same document.
	*/
	double MeanNbProf;

	/**
	* Mean of Agreement Ratio between profiles.
	*/
	double MeanSame;

	/**
	* Mean of Desagreement Ratio between profiles.
	*/
	double MeanDiff;
	
	/**
	* Statistics Output file.
	*/
	R::RTextFile* File;


public :

	/**
	* [param description]
	*/
	//[type] [param name];

public :

	/**
	* Constructor of GStatProfDoc
	* @param ses            The  galilei session.
	* @param f              File.
	*/
	GStatProfDoc(GSession* ses,R::RTextFile* f);

	/**
	* Construct the Mean of number of profiles having juged common docs.
	*/
	void Run(GStatsCalc* calc,R::RXMLStruct* xml,R::RXMLTag* tag);

	/**
	* Get The mean of number of profile having juged a same document.
	*/
//	double GetMeanNbProf(void) const {return (MeanNbProf); }

	/*
	* Get the average of the Agreement ratio between profiles.
	*/
//	double GetMeanSame(void) const {return (MeanSame);}

	/*
	* Get the average of the Desagreement Ration between profiles.
	*/
//	double GetMeanDiff(void) const {return (MeanDiff);}

	/*
	* Write a line to the output File.
	*/
	void WriteLine(void);

	
	/**
	* destructor of GStatProfDoc
	* @param1 [param1 name]    [param1 description]
	*/
	~GStatProfDoc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------


//-----------------------------------------------------------------------------
#endif
