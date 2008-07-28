/*

	GALILEI Research Project

	GSessionPrg.h

	Program running a Session object - Header.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GSessionPrgH
#define GSessionPrgH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprgfunc.h>
#include <rprgclass.h>
#include <rprg.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GPrgClassSession provides a the global 'Session' object which can be used
* to manipulate the current GALILEI session from a script. The class defines
* several methods:
* -	'SetRand(Value)': Set the random generator to 'Value'.
* - 'SetOutput(FileName)': The global results should be stored in 'FileName'.
* - 'SetGraphOutput(FileName)': The results for graphics should be stored in
*   'FileName'.
* - 'SetStatsOutput(FileName)': The detailed results should be stored in
*   'FileName'.
* - 'Test(Label)': Set the current test label to 'Label'.
* - 'ExecSql(SQL)': Execute the 'SQL' command.
* - 'ComputeProfiles()': Compute the profiles.
* - 'GroupProfiles()': Group the profiles.
* - 'GroupDocs()': Group the documents.
* - 'CreateIdeal()': Create a new ideal clustering and new profiles.
* - 'FdbksCycle()': Simulate that documents are shared inside the communities
*   and that the best are assessed.
* - 'CompareIdeal()': Compare the computed profiles clustering with the ideal
*   one.
* - 'AddIdeal()': Add a new subject to the ideal clustering and create new
*   profiles associated to it.
* - 'AddProfiles()': Add new profiles to a randomly chosen subject.
* - 'RealLife(MaxStep,MinFBStep,MaxFBStep,Proba)': Perform the simulation of a
*   complete system running 'MaxStep' times. The number of feedback processes
*   to simulate before new profiles are created is [MinFBStep,MaxFBStep]. Each
*   time new profiles must be created, there is a probability of 'Proba' that
*   they will be issued from a new subject.
* - 'AddAssessments()': Simulate that the profiles assess a given number of
*   randomly chosen documents.
* - 'TrackNewProfiles(Yes)': Specify if the new profiles must be tracked
*   ('Yes'="1") or not ('Yes'="0").
* - 'ClearNewProfiles()': Clear the list of profiles considered as new.
* - 'ResetTime()': Reset the internal time counter.
* - 'ComputeTime()': Compute the interval from the last called to ResetTime().
* - 'RunStat(): Run all enabled statistics.
* - 'ForceReCompute(Type)': All objects of a given type ("User','Doc',
*   'Community' or 'Topic') should be recomputed.
* - 'SetSaveResults(Yes)': Specify if the results must be stored in the current
*   storage ('Yes'="1") or not ('Yes'="0").
* @author Pascal Francq
* @short GALILEI Session Script Class.
*/
class GSessionClass : public R::RPrgClass
{
public:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Name of the current test.
	*/
	R::RString TestName;

	/**
	* Output file.
	*/
	R::RTextFile* OFile;

	/**
	* Graph Output file.
	*/
	R::RTextFile* GOFile;

	/**
	* Statistics Output file.
	*/
	R::RTextFile* SOFile;

	/**
	* Documents Statistics Output file.
	*/
	R::RTextFile* DSOFile;

	/**
	* Precision of the current clustering.
	*/
	double Precision;

	/**
	* Recall of the current clustering.
	*/
	double Recall;

	/**
	* Total comparison between for the current clustering.
	*/
	double Total;

	/**
	* Percentage of correct assignments for the profiles last added.
	*/
	double PercAss;

	/**
	* When TrackNewProfile is true, the system
	*/
	bool TrackNewProfiles;

	/**
	* Clock Reference used to measure laps of execution time.
	*/
	time_t ClockRef;

	/**
	* Create a class.
	* @param s              Session.
	*/
	GSessionClass(GSession* s);

	/**
	* Destruct the class.
	*/
	virtual ~GSessionClass(void);
};


//------------------------------------------------------------------------------
/**
* The GGALILEIApp provides a the global 'GAILEIApp' object which can be used to
* manipulate the GALILEI application from a script. Several methods are defined
* to manage configuration parameters of the plug-ins:
* - 'SetSimulationParam(Name,Value)': Assign a value 'Value' to the
*   parameter 'Name' used for simulation purposes.
* - 'SetPlugInParam(Type,PlugIn,Name,Value)': Assign a value 'Value'
*   to the parameter 'Name' of the plug-in 'PlugIn' of the type 'Type'.
* - 'SetCurrentPlugIn(Type,PlugIn)': Set the current plug-in 'PlugIn' for the
*   type 'Type'.
* - 'SetMeasureParam(Type,PlugIn,Name,Value)': Assign a value 'Value'
*   to the parameter 'Name' of the measure 'PlugIn' of the type 'Type'.
* - 'SetCurrentMeasure(Type,PlugIn)': Set the current measure 'PlugIn' for the
*   type 'Type'.
* - 'SetLog(FileName): The log file will be stored in 'FileName'.
*
* Is here an example of a program file used to set some parameters:
* @code
* # For the simulation parameter
* GALILEIApp.SetSimulationParam("NbOK","10");
*
* # For all plug-ins (excepted GMeasure)
* GALILEIApp.SetPlugInParam("GroupProfiles","GCA for Profiles","Population Size","16");
* GALILEIApp.SetCurrentPlugIn("GroupProfiles","GCA for Profiles");
*
* # For GMeasure plug-in
* GALILEIApp.SetMeasureParam("Profiles Agreements","Count Method","MinDocs","7");
* GALILEIApp.SetCurrentMeasure("Profiles Agreements","Count Method");
* @endcode
*
* @author Pascal Francq
* @short GALILEIApp Script Class.
*/
class GGALILEIAppClass : public R::RPrgClass
{
public:
	/**
	* Create a class.
	*/
	GGALILEIAppClass(void);

	/**
	* Destruct the class.
	*/
	virtual ~GGALILEIAppClass(void);
};


//------------------------------------------------------------------------------
/**
* The GSessionPrg provides a class for executing a GALILEI Program.
* @author Pascal Francq
* @short GALILEI Program.
*/
class GSessionPrg : public R::RPrg
{
public:

	/**
	* Constructor.
	* @param f              Name of the file.
	* @param s              Session.
	* @param o              Generic output class.
	*/
	GSessionPrg(R::RString f,GSession* s,R::RPrgOutput* o);

	/**
	* Destruct.
	*/
	virtual ~GSessionPrg(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
