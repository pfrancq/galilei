/*
	GALILEI Research Project

	GStatProfDoc.h

	Statistics on documents assessed by profiles - Header.

	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
	* The mean of the number of profiles which has assessed a same document.
	*/
	double MeanNbProf;

	/**
	* Mean of Agreement Ratio between profiles.
	*/
	double MeanSame;

	/**
	* Mean of Disagreement Ratio between profiles.
	*/
	double MeanDiff;

	/**
	* Statistics Output file.
	*/
	R::RTextFile& File;

	/**
	*/
	GMeasure* Measure;

public :

	/**
	* Constructor of GStatProfDoc
	* @param ses            The  galilei session.
	* @param f              File.
	*/
	GStatProfDoc(GSession* ses,R::RTextFile& f);

	/**
	* Construct the Mean of number of profiles having juged common docs.
	*/
	void Run(void);

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
};


}  //-------- End of namespace GALILEI ----------------------------------


//-----------------------------------------------------------------------------
#endif
