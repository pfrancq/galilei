/*

	GALILEI Research Project

	GSessionPrg.h

	Program to run on a Session - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GSessionPrgH
#define GSessionPrgH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rio/rtextfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gprgvar.h>
#include <sessions/gprgclass.h>
#include <profiles/ggetfeedback.h>
#include <groups/gidealgroup.h>
#include <tests/gmixidealgroups.h>
#include <tests/gstatsimsubprof.h>
#include <tests/gstatsimdoc.h>
#include <sessions/gprginst.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
/**
* The GPrgSessionClass provides a class for a GALILEI session.
* @author Pascal Francq
* @short GALILEI Session.
*/
class GPrgClassSession : public GPrgClass
{
public:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* Name of the current test.
	*/
	RStd::RString TestName;

	/**
	* Output file.
	*/
	RIO::RTextFile* OFile;

	/**
	* Graph Output file.
	*/
	RIO::RTextFile* GOFile;

	/**
	* Statistics Output file.
	*/
	RIO::RTextFile* SOFile;

	/**
	* Container used to create subjects catagories.
	*/
	RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* Parents;

	/**
	* Precision of the current groupement.
	*/
	double Precision;

	/**
	* Recall of the current groupement.
	*/
	double Recall;

	/**
	* Total comparaison between for the current groupment.
	*/
	double Total;

	/**
	* First profile computing has be done.
	*/
	bool FirstProfile;

	/**
	* First groupment has be done.
	*/
	bool FirstGroup;

	/**
	* When Autosave is true, after each computing, the results is saved.
	*/
	bool AutoSave;

	/**
	* Create a class.
	* @param s              Session.
	*/
	GPrgClassSession(GSession* s) throw(bad_alloc);

	/**
	*/
	virtual ~GPrgClassSession(void);
};


//-----------------------------------------------------------------------------
class GSM : public GPrgFunc
{
public:
	char tmp[300];
	GPrgClassSession* Owner;
	GSM(const char* name,GPrgClassSession* o)
		: GPrgFunc(name), Owner(o) {}
};


//-----------------------------------------------------------------------------
class GOutputI : public GSM
{
public:
	GOutputI(GPrgClassSession* o) : GSM("SetOutput",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GGOutputI : public GSM
{
public:
	GGOutputI(GPrgClassSession* o) : GSM("SetGraphOutput",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSOutputI : public GSM
{
public:
	GSOutputI(GPrgClassSession* o) : GSM("SetStatsOutput",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSetAutoSaveI : public GSM
{
public:
	GSetAutoSaveI(GPrgClassSession* o) : GSM("SetAutoSave",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GTestI : public GSM      
{
public:
	GTestI(GPrgClassSession* o) : GSM("Test",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GLogI : public GSM
{
public:
	GLogI(GPrgClassSession* o) : GSM("Log",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSqlI : public GSM
{
public:
	GSqlI(GPrgClassSession* o) : GSM("ExecSql",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GModifyProfilesI : public GSM
{
public:
	GModifyProfilesI(GPrgClassSession* o) : GSM("ModifyProfiles",o) {}
	virtual void Run(GSessionPrg*,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GComputeProfilesI : public GSM
{
public:
	GComputeProfilesI(GPrgClassSession* o) : GSM("ComputeProfiles",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GGroupProfilesI : public GSM
{
public:
	GGroupProfilesI(GPrgClassSession* o) : GSM("GroupProfiles",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GCreateIdealI : public GSM
{
	GIdealGroup IdealMethod;
public:
	GCreateIdealI(GPrgClassSession* o) : GSM("CreateIdeal",o),IdealMethod(Owner->Session) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GMixIdealI : public GSM
{
public:
	GMixIdealI(GPrgClassSession* o) : GSM("MixIdeal",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GFdbksCycleI : public GSM
{
	GGetFeedback FdbksMethod;
public:
	GFdbksCycleI(GPrgClassSession* o) : GSM("FdbksCycle",o),FdbksMethod(Owner->Session) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GCompareIdealI : public GSM
{
public:
	GCompareIdealI(GPrgClassSession* o) : GSM("CompareIdeal",o) {}
	virtual void Run(GSessionPrg*,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GStatsProfilesI : public GSM
{
public:
	GStatsProfilesI(GPrgClassSession* o) : GSM("StatsProfiles",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GStatsDocsI : public GSM
{
	GStatSimDoc DocStats;
public:
	GStatsDocsI(GPrgClassSession* o) : GSM("StatsDocs",o), DocStats(Owner->Session) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSetComputingParamI : public GSM
{
public:
	GSetComputingParamI(GPrgClassSession* o) : GSM("SetComputingParam",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSetGroupingParamI : public GSM
{
public:
	GSetGroupingParamI(GPrgClassSession* o) : GSM("SetGroupingParam",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GRunQueriesI : public GSM
{
public:
	GRunQueriesI(GPrgClassSession* o) : GSM("RunQueries",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
/**
* The GSessionPrg provides a class for executing a GALILEI Program.
* @author Pascal Francq
* @short GALILEI Program.
*/
class GSessionPrg
{
protected:

	/**
	* Name of the file to execute.
	*/
	RStd::RString FileName;

	/**
	* Receiver of the signal.
	*/
	GSlot* Rec;

	/**
	* List of all "Instructions" to execute.
	*/
	RStd::RContainer<GPrgInst,unsigned int,true,false> Insts;

	/**
	* Variables.
	*/
	RStd::RContainer<GPrgVar,unsigned int,true,false> Vars;

	/**
	* Classes.
	*/
	RStd::RContainer<GPrgClass,unsigned int,true,false> Classes;

	/**
	* Program file.
	*/
	RIO::RTextFile Prg;

	/**
	* Temporary string.
	*/
	char tmp[500];

	/**
	* Must a line be readed;
	*/
	bool ReadLine;

public:

	/**
	* Constructor.
	* @param f              Name of the file.
	* @param s              Session.
	* @param r              Receiver.
	*/
	GSessionPrg(RString f,GSession* s,GSlot* r) throw(bad_alloc,GException);

	/**
	* Count the number of tabs at the begin of a line of source code.
	* @param line           Line to analyze.
	* @returns Instruction to insert.
	*/
	static unsigned int CountTabs(char* line);

	/**
	* Analyse a line of source code.
	* @param prg            File containing the program.
	* @returns Instruction to insert.
	*/
	GPrgInst* AnalyseLine(RIO::RTextFile& prg) throw(bad_alloc,GException);

	/**
	* Analyse a parameter.
	* @param param          Parameter to analyse.
	* @returns Variable created.
	*/
	static GPrgVar* AnalyseParam(char* &param) throw(bad_alloc,GException);

	/**
	* Execute a "program" for the current session.
	*/
	void Exec(void) throw(GException);

	/**
	* Add a variable.
	*/
	void AddVar(GPrgVar* var) throw(bad_alloc,GException);

	/**
	* Del a variable.
	*/
	void DelVar(GPrgVar* var) throw(bad_alloc,GException);

	/**
	* Get the value of the variable.
	* @param var            Name of the variable.
	*/
	virtual const char* GetValue(const char* var) throw(GException);

	/**
	* Destructor.
	*/
	virtual ~GSessionPrg(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
