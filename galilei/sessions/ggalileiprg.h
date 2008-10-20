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
#include <rprgvar.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GGALILEIPrg provides a class for executing a GALILEI Program. In practice,
* it provides two classes 'GSession' and 'GGALILEIApp', and defines a global
* variable GALILEIApp of the type 'GGALILEIApp'.
*
* The class 'GSession' can be used to manipulate the current GALILEI session
* from a script. Only one instance of this class may be created. If the session
* is not created, it will be created and destroyed by the instance. If the
* session is already created (for example if KGALILEICenter is already
* connected), the session is not destroyed. The class defines several methods:
* - 'GSession(YesDoc,YesProfiles,YesCommunities,YesTopics)': Constructor which
*   may load the documents ('YesDocs'=="1"), the profiles ('YesProfiles'=="1"),
*   the communities ('YesCommunities'=="1") and/or the topics
*   ('YesTopics'=="1").
* -	'SetRand(Value)': Set the random generator to 'Value'.
* - 'ExecSql(SQL)': Execute the 'SQL' command.
* - 'ComputeProfiles()': Compute the profiles.
* - 'GroupProfiles()': Group the profiles.
* - 'GroupDocs()': Group the documents.
* - 'CreateIdeal()': Create a new ideal clustering and new profiles.
* - 'FdbksCycle()': Simulate that documents are shared inside the communities
*   and that the best are assessed.
* - 'CompareIdeal(Type)': Compare the computed 'Type' groups ("Community",
*   "Topic") with the ideal one.
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
*   ('Yes'=="1") or not ('Yes'=="0").
* - 'ClearNewProfiles()': Clear the list of profiles considered as new.
* - 'RunStat(): Run all enabled statistics.
* - 'ForceReCompute(Type)': All objects of a given type ("User","Doc",
*   "Community" or "Topic") should be recomputed.
* - 'SetSaveResults(Yes)': Specify if the results must be stored in the current
*   storage ('Yes'=="1") or not ('Yes'=="0").
* - 'PerformDegradation(What,Nb)': Perform a degradation. If 'What'=="0", the
*   degradation process is launched. If 'What'=="1", the ideal clustering is
*   used as the current one (eventually 'Nb' groups are split (Nb>0) or merged
*   (Nb<0)). If 'What'=="2", a given percantage of documents are changed to
*   another topic.
*
* The class 'GGALILEIApp' is used to manipulate the GALILEI application from a
* script. Several methods are defined to manage configuration parameters of the
* plug-ins:
* - 'SetOutput(FileName)': The global results should be stored in 'FileName'.
* - 'SetGraphOutput(FileName)': The results for graphics should be stored in
*   'FileName'.
* - 'SetStatsOutput(FileName)': The detailed results should be stored in
*   'FileName'.
* - 'SetTest(Label)': Set the current test label to 'Label'.
* - 'ComputeTime()': Compute the interval from the last called to ResetTime().
* - 'ResetTime()': Reset the internal time counter.
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
* @author Pascal Francq
* @short GALILEI Program.
*/
class GGALILEIPrg : public R::RPrg
{
public:

	/**
	* Constructor.
	* @param f              Name of the file.
	* @param o              Generic output class.
	*/
	GGALILEIPrg(R::RString f,R::RPrgOutput* o);

	/**
	* Destruct.
	*/
	virtual ~GGALILEIPrg(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
