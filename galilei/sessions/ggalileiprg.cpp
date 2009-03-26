/*

	GALILEI Research Project

	GSessionPrg.cpp

	Program running a Session object  - Implementation.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#define SHOWINST 0


//------------------------------------------------------------------------------
// include files for R Project
#include <rprgvarinst.h>
#include <rprgvar.h>
#include <rprgfunc.h>
#include <rprgclass.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggalileiprg.h>
#include <glang.h>
#include <ggroupprofiles.h>
#include <ggroupdocs.h>
#include <gcommunitycalc.h>
#include <gtopiccalc.h>
#include <gsubjects.h>
#include <gprofilecalc.h>
#include <glinkcalc.h>
#include <gstatscalc.h>
#include <gsession.h>
#include <gstorage.h>
#include <gslot.h>
#include <ggalileiapp.h>
#include <gdocanalyse.h>
#include <gmeasure.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// forward declaration
class GInstGALILEIApp;



//------------------------------------------------------------------------------
//
// Classes
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSessionClass : public R::RPrgClass
{
public:
	bool Instance;
	GInstGALILEIApp* App;

	GSessionClass(GInstGALILEIApp* app);
	virtual R::RPrgVar* NewVar(R::RInterpreter* prg,RPrgOutput* o,const R::RString& name,R::RContainer<R::RPrgVar,true,false>& params);
	virtual ~GSessionClass(void);
};


//------------------------------------------------------------------------------
class GGALILEIAppClass : public R::RPrgClass
{
public:
	GGALILEIAppClass(void);
	virtual R::RPrgVar* NewVar(R::RInterpreter* prg,RPrgOutput* o,const R::RString& name,R::RContainer<R::RPrgVar,true,false>& params);
	virtual ~GGALILEIAppClass(void);
};



//------------------------------------------------------------------------------
//
// class GInstGALILEIApp
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GInstGALILEIApp : public RPrgVarInst
{
public:
	RString TestName;  		// Name of the current test.
	RTextFile* OFile;  		// Output file.
	RTextFile* GOFile;  	// Graph Output file.
	RTextFile* SOFile;  	// Statistics Output file.
	RTextFile* DSOFile;	    // Documents Statistics Output file.
	time_t ClockRef; 		// Clock Reference used to measure laps of execution time.

	GInstGALILEIApp(const RString& name,RPrgClass* c);
	virtual ~GInstGALILEIApp(void);
};


//------------------------------------------------------------------------------
GInstGALILEIApp::GInstGALILEIApp(const RString& name,RPrgClass* c)
	: RPrgVarInst(name,c), OFile(0),GOFile(0), SOFile(0),DSOFile(0)
{
}


//------------------------------------------------------------------------------
GInstGALILEIApp::~GInstGALILEIApp(void)
{
	if(OFile)
		delete OFile;
	if(GOFile)
		delete GOFile;
	if(SOFile)
		delete SOFile;
	if(DSOFile)
		delete DSOFile;
}



//------------------------------------------------------------------------------
//
// class GInstSession
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GInstSession : public RPrgVarInst
{
public:
	GSession* Session;		             // Session.
	double Precision;	             	 // Precision of the current clustering.
	double Recall;			             // Recall of the current clustering.
	double Total;			             // Total comparison between for the current clustering.
	double PercAss;			             // Percentage of correct assignments for the profiles last added.
	bool TrackNewProfiles;	             // When TrackNewProfile is true, the system
	bool DelSession;                     // Must the session be destroyed?
	GInstGALILEIApp* App;                // Application

	GInstSession(const RString& name,GSessionClass* c,GSession* session,RInterpreter* prg,RContainer<R::RPrgVar,true,false>& args,GInstGALILEIApp* app);
	virtual ~GInstSession(void);
};


//------------------------------------------------------------------------------
GInstSession::GInstSession(const RString& name,GSessionClass* c,GSession* session,RInterpreter* prg,RContainer<R::RPrgVar,true,false>& args,GInstGALILEIApp* app)
 : RPrgVarInst(name,c), Session(session), TrackNewProfiles(false), App(app)
{
	if(args.GetNb()!=4)
		throw RPrgException(prg,"Constructor needs four parameters");

	if(!session)
	{
		GALILEIApp->CreateSession();
		Session=GALILEIApp->GetSession();
		DelSession=true;
	}
	else
		DelSession=false;

	bool DoDocs=args[0]->GetValue(prg)=="1";
	bool DoProfiles=args[1]->GetValue(prg)=="1";
	bool DoCommunities=args[2]->GetValue(prg)=="1";
	bool DoTopics=args[3]->GetValue(prg)=="1";
	if(DoTopics)
		Session->LoadTopics();
	if(DoDocs||DoProfiles||DoTopics)
		Session->LoadDocs();
	if(DoCommunities)
		Session->LoadCommunities();
	if(DoProfiles||DoCommunities)
		Session->LoadUsers();
}


//------------------------------------------------------------------------------
GInstSession::~GInstSession(void)
{
	if(DelSession)
	{
		GALILEIApp->DeleteSession();
		dynamic_cast<GSessionClass*>(GetClass())->Instance=false;
	}
}



//------------------------------------------------------------------------------
//
// Declare all the instructions of the class 'GSession' and 'GALILEIApp'.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSetRandI : public RPrgFunc
{
public:
	GSetRandI(void) : RPrgFunc("SetRand","Set the random generator to a given value.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GOutputI : public RPrgFunc
{
public:
	GOutputI(void) : RPrgFunc("SetOutput","Specify where the global results should be stored.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GGOutputI : public RPrgFunc
{
public:
	GGOutputI(void) : RPrgFunc("SetGraphOutput","Specify where the results for graphics should be stored.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSOutputI : public RPrgFunc
{
public:
	GSOutputI(void) : RPrgFunc("SetStatsOutput","Specify where the detailed results should be stored.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSetTestI : public RPrgFunc
{
public:
	GSetTestI(void) : RPrgFunc("SetTest","Set the current test label to a given string.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSetLogI : public RPrgFunc
{
public:
	GSetLogI(void) : RPrgFunc("SetLog","Specify a log file.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GExecSqlI : public RPrgFunc
{
public:
	GExecSqlI(void) : RPrgFunc("ExecSql","Execute a given SQL string.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GComputeProfilesI : public RPrgFunc
{
public:
	GComputeProfilesI(void) : RPrgFunc("ComputeProfiles","Compute the profiles.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GGroupProfilesI : public RPrgFunc
{
public:
	GGroupProfilesI(void) : RPrgFunc("GroupProfiles","Group the profiles.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GGroupDocsI : public RPrgFunc
{
public:
	GGroupDocsI(void) : RPrgFunc("GroupDocs","Group the documents.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GStartSimulationI : public RPrgFunc
{
public:
	GStartSimulationI(void) : RPrgFunc("StartSimulation","Create a new ideal clustering and new profiles.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GPerformDegradationI : public RPrgFunc
{
public:
	GPerformDegradationI(void) : RPrgFunc("PerformDegradation","Perform a degradation of an ideal clustering.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GFdbksCycleI : public RPrgFunc
{
public:
	GFdbksCycleI(void) : RPrgFunc("FdbksCycle","Simulate that documents are shared inside the different communities and that the best are assessed.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GCompareIdealI : public RPrgFunc
{
public:
	GCompareIdealI(void) : RPrgFunc("CompareIdeal","Compare the computed clustering with the ideal one.") {}
	virtual void Run(R::RInterpreter*,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSetSimulationParamI : public RPrgFunc
{
public:
	GSetSimulationParamI(void) : RPrgFunc("SetSimulationParam","Specify a value (2d argument) for a simulation parameter (1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GAddIdealI : public RPrgFunc
{
public:
	GAddIdealI(void) : RPrgFunc("AddIdeal","Add a new topic to the ideal clustering and create new profiles associated to it.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GAddProfilesI : public RPrgFunc
{
public:
	GAddProfilesI(void) : RPrgFunc("AddProfiles","Add new profiles to a randomly chosen topic.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GRealLifeI : public RPrgFunc
{
	char What[2];
	void CommonTasks(R::RPrgOutput* o,GInstSession* Owner);
public:
	GRealLifeI(void) : RPrgFunc("RealLife","Perform the simulation of a complete system running a given number of step times.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GAddAssessmentsI : public RPrgFunc
{
public:
	GAddAssessmentsI(void) : RPrgFunc("AddAssessments","Simulate that the profiles assess a given number of randomly chosen documents.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GTrackNewProfilesI : public RPrgFunc
{
public:
	GTrackNewProfilesI(void) : RPrgFunc("TrackNewProfiles","Specify if the new profiles must be tracked.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GClearNewProfilesI : public RPrgFunc
{
public:
	GClearNewProfilesI(void) : RPrgFunc("ClearNewProfiles","Clear the list of profiles considered as new.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GResetTimeI : public RPrgFunc
{
public:
	GResetTimeI(void) : RPrgFunc("ResetTime","Reset the time counter.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GComputeTimeI : public RPrgFunc
{
public:
	GComputeTimeI(void) : RPrgFunc("ComputeTime","Compute the interval from the last called to ResetTime.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSetStatParamI: public RPrgFunc
{
public:
	GSetStatParamI(void) : RPrgFunc("SetStatParam","Specify a value (2d argument) for a statistics parameter (1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GRunStatI: public RPrgFunc
{
public:
	GRunStatI(void) : RPrgFunc("RunStat","Run all enabled statistics.") {}
	void Print(R::RPrgOutput* o,RXMLTag* tag,int depth);
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GForceReComputeI: public RPrgFunc
{
public:
	GForceReComputeI(void) : RPrgFunc("ForceReCompute","Specify if all objects should be recomputed or if an incremental method should be used.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSetSaveResultsI: public RPrgFunc
{
public:
	GSetSaveResultsI(void) : RPrgFunc("SetSaveResults","Specify if the results must be stored in the current storage.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSetPlugInParamI : public RPrgFunc
{
public:
	GSetPlugInParamI(void) : RPrgFunc("SetPlugInParam","Specify a value (4th argument) for a parameter (3rd argument) for a plug-in (with name=2nd argument and type=1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSetCurrentPlugInI : public RPrgFunc
{
public:
	GSetCurrentPlugInI(void) : RPrgFunc("SetCurrentPlugIn","Specify the current plug-in (2th argument) for a given type(1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSetMeasureParamI : public RPrgFunc
{
public:
	GSetMeasureParamI(void) : RPrgFunc("SetMeasureParam","Specify a value (4th argument) for a parameter (3rd argument) for a measure (with name=2nd argument and type=1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GSetCurrentMeasureI : public RPrgFunc
{
public:
	GSetCurrentMeasureI(void) : RPrgFunc("SetCurrentMeasure","Specify the current measure (2th argument) for a given type(1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GResetMeasureI : public RPrgFunc
{
public:
	GResetMeasureI(void) : RPrgFunc("ResetMeasure","Reset the measure (2th argument) for a given type(1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GAnalyzeDocsI : public RPrgFunc
{
public:
	GAnalyzeDocsI(void) : RPrgFunc("AnalyzeDocs","Analyze the documents.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};



//------------------------------------------------------------------------------
//
// Instructions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#if SHOWINST
inline void ShowInst(RPrgFunc* func,RInterpreter* prg,RContainer<RPrgVar,true,false>& args)
{
		cout<<func->GetName()<<" ";
		RCursor<RPrgVar> A(args);
		for(A.Start();!A.End();A.Next())
			cout<<A()->GetValue(prg)<<" ";
		cout<<endl;
}
#else
inline void ShowInst(RPrgFunc*,RInterpreter*,RContainer<RPrgVar,true,false>&)
{
}
#endif


//------------------------------------------------------------------------------
void GSetRandI::Run(RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	int Rand;

	if(args.GetNb()!=1)
		throw RPrgException(prg,"The rand value must be specified.");
	o->WriteStr("Set Rand value: "+args[0]->GetValue(prg));
	Rand=atoi(args[0]->GetValue(prg));
	if(Rand!=0)
	{
		Owner->Session->SetCurrentRandom(Rand);
	}
}


//------------------------------------------------------------------------------
void GOutputI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstGALILEIApp* Owner=dynamic_cast<GInstGALILEIApp*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"A filename must be specified.");
	o->WriteStr("Create Output file: "+args[0]->GetValue(prg));
	if(Owner->OFile)
	{
		delete Owner->OFile;
		Owner->OFile=0;
	}
	Owner->OFile=new RTextFile((args[0]->GetValue(prg)));
	Owner->OFile->Open(RIO::Create);
	Owner->OFile->SetSeparator("\t");
	(*Owner->OFile)<<"Sets"<<"Recall"<<"Precision"<<"Total";
	(*Owner->OFile)<<endl;
}


//------------------------------------------------------------------------------
void GGOutputI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstGALILEIApp* Owner=dynamic_cast<GInstGALILEIApp*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"A filename must be specified.");
	o->WriteStr("Create Graph Output file: "+args[0]->GetValue(prg));
	if(Owner->GOFile)
	{
		delete Owner->GOFile;
		Owner->GOFile=0;
	}
	Owner->GOFile=new RTextFile(args[0]->GetValue(prg));
	Owner->GOFile->Open(RIO::Create);
	Owner->GOFile->SetSeparator("\t");
}


//------------------------------------------------------------------------------
void GSOutputI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstGALILEIApp* Owner=dynamic_cast<GInstGALILEIApp*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"A filename must be specified.");
	o->WriteStr("Create Statistics Output file: "+args[0]->GetValue(prg));
	if(Owner->SOFile)
	{
		delete Owner->SOFile;
		Owner->SOFile=0;
	}
	Owner->SOFile=new RTextFile(args[0]->GetValue(prg));
	Owner->SOFile->Open(RIO::Create);
	Owner->SOFile->SetSeparator("\t");
}


//------------------------------------------------------------------------------
void GSetTestI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstGALILEIApp* Owner=dynamic_cast<GInstGALILEIApp*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"The method needs the name of the test.");
	o->WriteStr("Current Test Name: "+args[0]->GetValue(prg));
	Owner->TestName=args[0]->GetValue(prg);
}



//------------------------------------------------------------------------------
void GSetLogI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"The method needs the name of the log file.");
	o->WriteStr("Create Log file: "+args[0]->GetValue(prg));
	GALILEIApp->SetLogFileName(args[0]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GExecSqlI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"The method needs the name of the SQL file.");
	o->WriteStr("Execute Sql file: "+args[0]->GetValue(prg));
	Owner->Session->GetStorage()->ExecuteData(args[0]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GComputeProfilesI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");
	if(args.GetNb())
		throw RPrgException(prg,"The method needs no parameter.");
	o->WriteStr("Compute Profiles");
	if(!GALILEIApp->GetManager<GProfileCalcManager>("ProfileCalc")->GetCurrentMethod())
		throw RPrgException(prg,"No Profiling Method chosen.");
	Owner->Session->CalcProfiles(dynamic_cast<GSlot*>(o));
}


//------------------------------------------------------------------------------
void GGroupProfilesI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");
	if(args.GetNb())
		throw RPrgException(prg,"The method needs no parameter.");
	o->WriteStr("Group Profiles");
	if(!GALILEIApp->GetManager<GGroupProfilesManager>("GroupProfiles")->GetCurrentMethod())
		throw RPrgException(prg,"No Profiles Grouping Method chosen.");
	if(!GALILEIApp->GetManager<GCommunityCalcManager>("CommunityCalc")->GetCurrentMethod())
		throw RPrgException(prg,"No Community Description Method chosen.");
	Owner->Session->GroupProfiles(dynamic_cast<GSlot*>(o));
}


//------------------------------------------------------------------------------
void GGroupDocsI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"The method needs no parameter.");
	o->WriteStr("Group Documents");
	if(!GALILEIApp->GetManager<GGroupDocsManager>("GroupDocs")->GetCurrentMethod())
		throw RPrgException(prg,"No Documents Grouping Method chosen.");
	if(!GALILEIApp->GetManager<GTopicCalcManager>("TopicCalc")->GetCurrentMethod())
		throw RPrgException(prg,"No Topic Description Method chosen.");
	Owner->Session->GroupDocs(dynamic_cast<GSlot*>(o));
}


//------------------------------------------------------------------------------
void GStartSimulationI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Create Ideal Groups");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->StartSimulation();
}


//------------------------------------------------------------------------------
void GPerformDegradationI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	// Read Parameters
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");
	if((args.GetNb()!=1)&&(args.GetNb()!=2))
		throw RPrgException(prg,"Method needs at least one parameter.");
	bool b;
	char What=args[0]->GetValue(prg).ToChar(b);
	int Nb;
	if(What==1)
	{
		if(args.GetNb()!=2)
			throw RPrgException(prg,"Method needs two parameters if the first one is 1.");
		Nb=args[1]->GetValue(prg).ToInt(b);
	}
	else
		Nb=0;

	// Perform the degradation
	if((!What)&&o)
		o->WriteStr("Global Init");
	if((What==1)&&o)
	{
		o->WriteStr("Perform degradation: "+RString::Number(Nb));
	}
	Owner->Session->GetSubjects()->PerformDegradation(What,Nb);

	// Compute J and T and put it in a file
	if(!What)
		return;
	double J=Owner->Session->GetSubjects()->GetJ(otTopic);
	double T=Owner->Session->GetSubjects()->GetTotal(otTopic);
	if(Owner->App->OFile)
		(*Owner->App->OFile)<<Owner->App->TestName<<T<<J<<endl;
	if(Owner->App->GOFile)
		(*Owner->App->GOFile)<<T<<J<<endl;
}


//------------------------------------------------------------------------------
void GFdbksCycleI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Create Feedbacks Cycle");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->DocumentSharing();
}


//------------------------------------------------------------------------------
void GCompareIdealI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"Method needs one parameter.");
	RString objects=args[0]->GetValue(prg);
	tObjType type(otNoClass);
	if(objects=="Community")
		type=otCommunity;
	if(objects=="Topic")
		type=otTopic;
	if(type==otNoClass)
		throw RPrgException(prg,"Compare with unsupported type");
	o->WriteStr("Compare with Ideal "+objects+" Groups");
	Owner->Session->GetSubjects()->Compare(type);
	Owner->Precision=Owner->Session->GetSubjects()->GetPrecision(type);
	Owner->Recall=Owner->Session->GetSubjects()->GetRecall(type);
	Owner->Total=Owner->Session->GetSubjects()->GetTotal(type);
	if(Owner->TrackNewProfiles)
	{
		Owner->PercAss=Owner->Session->GetSubjects()->ComputePercAss();
		o->WriteStr("Recall: "+RString::Number(Owner->Recall)+"  -  Precision: "+RString::Number(Owner->Precision)+"  -  Total: "+RString::Number(Owner->Total)+"  -  New: "+RString::Number(Owner->PercAss));
	}
	else
		o->WriteStr("Recall: "+RString::Number(Owner->Recall)+"  -  Precision: "+RString::Number(Owner->Precision)+"  -  Total: "+RString::Number(Owner->Total));
	if(Owner->App->OFile)
	{
		(*Owner->App->OFile)<<Owner->App->TestName<<Owner->Recall<<Owner->Precision<<Owner->Total;
		if(Owner->TrackNewProfiles)
			(*Owner->App->OFile)<<Owner->PercAss;
		(*Owner->App->OFile)<<endl;
	}
	if(Owner->App->GOFile)
	{
		(*Owner->App->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->Total;
		if(Owner->TrackNewProfiles)
			(*Owner->App->GOFile)<<Owner->PercAss;
		(*Owner->App->GOFile)<<endl;
	}
}


//------------------------------------------------------------------------------
void GSetSimulationParamI::Run(R::RInterpreter* prg,RPrgOutput*,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=2)
		throw RPrgException(prg,"Method needs two parameters.");
	GALILEIApp->GetGALILEIConfig()->Set(args[0]->GetValue(prg),args[1]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GAddIdealI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Create New Ideal Group");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->AddTopic();
}


//------------------------------------------------------------------------------
void GAddProfilesI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Adding Profiles");
	Owner->Session->GetSubjects()->Apply();
	o->WriteStr(RString::Number(Owner->Session->GetSubjects()->AddProfiles())+" new profiles created");
}


//------------------------------------------------------------------------------
void GRealLifeI::CommonTasks(RPrgOutput* o,GInstSession* Owner)
{
	GSlot* rec=dynamic_cast<GSlot*>(o);

	// Compute Profiles
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Compute Profiles: Current Method");
	}
	if(GSession::Break()) return;
	if(GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod(false))
		GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod(false)->ApplyConfig();
	GALILEIApp->GetManager<GProfileCalcManager>("ProfileCalc")->GetCurrentMethod()->ApplyConfig();
	Owner->Session->CalcProfiles(rec);

	// Group Profiles
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Group Profiles: Current Method");
	}
	if(GSession::Break()) return;
	GALILEIApp->GetManager<GGroupProfilesManager>("GroupProfiles")->GetCurrentMethod()->ApplyConfig();
	GALILEIApp->GetManager<GCommunityCalcManager>("CommunityCalc")->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GroupProfiles(rec);

	// Compare Ideal
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Compare with Ideal Groups");
	}
	Owner->Session->GetSubjects()->Compare(otCommunity);
	Owner->Precision=Owner->Session->GetSubjects()->GetPrecision(otCommunity);
	Owner->Recall=Owner->Session->GetSubjects()->GetRecall(otCommunity);
	Owner->Total=Owner->Session->GetSubjects()->GetTotal(otCommunity);
	if(rec)
		o->WriteStr("Recall: "+RString::Number(Owner->Recall)+"  -  Precision: "+RString::Number(Owner->Precision)+"  -  Total: "+RString::Number(Owner->Total));
	if(GSession::Break()) return;
	if(Owner->App->OFile)
		(*Owner->App->OFile)<<Owner->App->TestName<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
	if(Owner->App->GOFile)
		(*Owner->App->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
}


//------------------------------------------------------------------------------
void GRealLifeI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	size_t MaxStep;
	size_t MinFBStep;
	size_t MaxFBStep;
	size_t NbStep;
	double Proba;
	RRandom* Random;
	size_t nb;
	size_t NewProf;
	size_t nbminprof,nbmaxprof;
	GSlot* rec=dynamic_cast<GSlot*>(o);

	if(args.GetNb()!=4)
		throw RPrgException(prg,"Method needs four parameters.");
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Real Life: "+args[0]->GetValue(prg)+","+args[1]->GetValue(prg)+","+args[2]->GetValue(prg)+","+args[3]->GetValue(prg));
	}
	if(GSession::Break()) return;
	Random=Owner->Session->GetRandom();
	MaxStep=atoi(args[0]->GetValue(prg));
	MinFBStep=atoi(args[1]->GetValue(prg));
	MaxFBStep=atoi(args[2]->GetValue(prg))-MinFBStep+1;
	Proba=atof(args[3]->GetValue(prg));
	What[1]=0;
	for(NbStep=0;;)
	{
		// Feedback process
		for(nb=Random->GetValue(MaxFBStep)+MinFBStep+1;--nb;)
		{
			// Set TestName
			NbStep++;
			Owner->App->TestName=RString::Number(NbStep);

			// Create Feedbacks
			if(rec)
			{
				rec->Interact();
				rec->WriteStr("Create Feedbacks Cycle");
			}
			if(GSession::Break()) return;
			What[0]='F';
			Owner->Session->GetSubjects()->Apply();
			Owner->Session->GetSubjects()->DocumentSharing();
			CommonTasks(o,Owner);

			// Verify Nb Steps
			if(NbStep>MaxStep) break;
		}

		// Verify Nb Steps
		if(NbStep>MaxStep) break;
		NbStep++;

		// Set TestName
		Owner->App->TestName=RString::Number(NbStep);

		// Create 1 new profile
		if(rec)
		{
			rec->Interact();
			rec->WriteStr("Create 1 new profile");
		}
		if(GSession::Break()) return;

		// One profile must be created
		nbminprof=GALILEIApp->GetGALILEIConfig()->GetUInt("NbProfMin");
		nbmaxprof=GALILEIApp->GetGALILEIConfig()->GetUInt("NbProfMax");
		GALILEIApp->GetGALILEIConfig()->Set("NbProfMin",1);
		GALILEIApp->GetGALILEIConfig()->Set("NbProfMax",1);
		GALILEIApp->Apply();

		// Look if new topic or existing one
		if(Random->GetValue()<Proba)
		{
			// Create One profile of an existing topic
			NewProf=Owner->Session->GetSubjects()->AddProfiles();
			GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMin",nbminprof);
			GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMax",nbmaxprof);

			if(NewProf)
				What[0]='E';
		}
		else
		{
			// Create one profile of a new topic
			if(Owner->Session->GetSubjects()->AddTopic())
			{
				What[0]='N';
				NewProf=1;
			}
			else
			{
				Proba=1.0;  // Cannot create any new topic
				NewProf=Owner->Session->GetSubjects()->AddProfiles();
				GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMin",nbminprof);
				GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMax",nbmaxprof);

				if(NewProf)
					What[0]='E';
			}
		}
		GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMin",nbminprof);
		GALILEIApp->GetGALILEIConfig()->SetUInt("NbProfMax",nbmaxprof);
		GALILEIApp->Apply();

		// If no profile was created -> normal feedback cycle
		if(!NewProf)
		{
			What[0]='F';
			Owner->Session->GetSubjects()->Apply();
			Owner->Session->GetSubjects()->DocumentSharing();
		}

		// Compute, Group and Compare
		CommonTasks(o,Owner);

		// Verify Nb Steps
		if(NbStep>MaxStep) break;
	}
}


//------------------------------------------------------------------------------
void GAddAssessmentsI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Adding Assessments");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->AddAssessments();
}


//------------------------------------------------------------------------------
void GTrackNewProfilesI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"The method needs one parameter (\"0\" or \"1\") to specify if the profiles must be tracked.");
	if(args.GetNb())
	{
		if((args[0]->GetValue(prg))=="0")
		{
			o->WriteStr("Track New Profiles: false");
			Owner->TrackNewProfiles=false;
		}
		else
		{
			o->WriteStr("Track New Profiles: true");
			Owner->TrackNewProfiles=true;
		}
	}
}


//------------------------------------------------------------------------------
void GClearNewProfilesI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameter.");
	o->WriteStr("Clear New Profiles");
	Owner->Session->GetSubjects()->ClearLastAdded();
}


//------------------------------------------------------------------------------
void GResetTimeI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstGALILEIApp* Owner=dynamic_cast<GInstGALILEIApp*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameter.");
	o->WriteStr("Reset Time");
	Owner->ClockRef=time(0);
}


//------------------------------------------------------------------------------
void GComputeTimeI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstGALILEIApp* Owner=dynamic_cast<GInstGALILEIApp*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");
	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameter.");
	time_t end(time(0));
	double cpu_time;
	cpu_time=difftime(end,Owner->ClockRef);
	o->WriteStr("Elapsed Time "+RString::Number(cpu_time));
}


//------------------------------------------------------------------------------
void GRunStatI::Print(R::RPrgOutput* o,RXMLTag* tag,int depth)
{
	RString p;
	for(int i=0;i<depth;i++)
		p+="  ";
	p+=tag->GetName();
	RString v=tag->GetAttrValue("Value");
	if(!v.IsEmpty())
		p+="="+v;
	o->WriteStr(p);
	RCursor<RXMLTag> Cur(tag->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		Print(o,Cur(),depth+1);
}


//------------------------------------------------------------------------------
void GRunStatI::Run(R::RInterpreter* prg,RPrgOutput* out,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	GStatsCalc* Calc;
	RXMLStruct xml;
	RXMLTag* Root;
	int i;

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameter");

	GStatsCalcManager* Mng=GALILEIApp->GetManager<GStatsCalcManager>("StatsCalc");
	if(!Mng)
		throw RPrgException(prg,"No manager for the statistics plug-ins");

	// Create the root node
	Root=new RXMLTag("Statistics");
	xml.AddTag(0,Root);

	// Compute the statistics
	R::RCursor<GFactoryStatsCalc> Cur(Mng->GetFactories());
	for(Cur.Start(),i=1;!Cur.End();Cur.Next(),i++)
	{
		Calc=Cur()->GetPlugin();
		if(Calc)
		{
			Calc->Compute(&xml,*Root);
		}
	}
	if(out)
		Print(out,Root,0);
}


//------------------------------------------------------------------------------
void GForceReComputeI::Run(R::RInterpreter* prg,RPrgOutput*,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	tObjType type=otNoClass;

	if(args.GetNb()!=1)
		throw RPrgException(prg,"Method needs one parameter");
	RString objects=args[0]->GetValue(prg);
	if(objects=="User")
		type=otUser;
	if(objects=="Doc")
		type=otDoc;
	if(objects=="Community")
		type=otCommunity;
	if(objects=="Topic")
		type=otTopic;
	Owner->Session->ForceReCompute(type);
}


//------------------------------------------------------------------------------
void GSetSaveResultsI::Run(R::RInterpreter* prg,RPrgOutput*,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");
	if(args.GetNb()!=1)
		throw RPrgException(prg,"Method needs one parameter");
	if((args[0]->GetValue(prg))=="0")
		Owner->Session->SetSaveResults(false);
	else
		Owner->Session->SetSaveResults(true);
}


//------------------------------------------------------------------------------
void GSetPlugInParamI::Run(R::RInterpreter* prg,RPrgOutput*,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=4)
		throw RPrgException(prg,"Method needs four parameters.");
	GGenericPluginManager* Mng=GALILEIApp->GetManager<GGenericPluginManager>(args[0]->GetValue(prg));
	if(!Mng)
		throw RPrgException(prg,"'"+args[0]->GetValue(prg)+"' is not a valid plug-in manager");
	RConfig* Config(Mng->GetConfig(args[1]->GetValue(prg)));
	if(!Config)
		throw RPrgException(prg,"'"+args[1]->GetValue(prg)+"' is not a plug-in for the manager '"+args[0]->GetValue(prg)+"'");
	Config->Set(args[2]->GetValue(prg),args[3]->GetValue(prg));
	Mng->ApplyConfig(Config);
}


//------------------------------------------------------------------------------
void GSetCurrentPlugInI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=2)
		throw RPrgException(prg,"Method needs two parameters.");
	GGenericPluginManager* Mng=GALILEIApp->GetManager<GGenericPluginManager>(args[0]->GetValue(prg));
	if(!Mng)
		throw RPrgException(prg,"'"+args[0]->GetValue(prg)+"' is not a valid plug-in manager");
	o->WriteStr("Current Method for '"+args[0]->GetValue(prg)+"': "+args[1]->GetValue(prg));
	Mng->SetCurrentMethod(args[1]->GetValue(prg),false);
}


//------------------------------------------------------------------------------
void GSetMeasureParamI::Run(R::RInterpreter* prg,RPrgOutput*,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=4)
		throw RPrgException(prg,"Method needs four parameters.");
	GMeasureManager* Mng=GALILEIApp->GetManager<GMeasureManager>("Measures");
	GTypeMeasureManager* Type=Mng->GetMeasureCategory(args[0]->GetValue(prg),false);
	if(!Type)
		throw RPrgException(prg,"'"+args[0]->GetValue(prg)+"' is not a valid measure type");
	RConfig* Config(Type->GetConfig(args[1]->GetValue(prg)));
	if(!Config)
		throw RPrgException(prg,"'"+args[1]->GetValue(prg)+"' is not a measure for the type '"+args[0]->GetValue(prg)+"'");
	Config->Set(args[2]->GetValue(prg),args[3]->GetValue(prg));
	Mng->ApplyConfig(Config);
}


//------------------------------------------------------------------------------
void GSetCurrentMeasureI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=2)
		throw RPrgException(prg,"Method needs two parameters.");
	GMeasureManager* Mng=GALILEIApp->GetManager<GMeasureManager>("Measures");
	GTypeMeasureManager* Type=Mng->GetMeasureCategory(args[0]->GetValue(prg),false);
	if(!Type)
		throw RPrgException(prg,"'"+args[0]->GetValue(prg)+"' is not a valid measure type");
	o->WriteStr("Current Measure for '"+args[0]->GetValue(prg)+"': "+args[1]->GetValue(prg));
	Type->SetCurrentMethod(args[1]->GetValue(prg),false);
}


//------------------------------------------------------------------------------
void GResetMeasureI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=2)
		throw RPrgException(prg,"Method needs two parameters.");
	GMeasureManager* Mng=GALILEIApp->GetManager<GMeasureManager>("Measures");
	GTypeMeasureManager* Type=Mng->GetMeasureCategory(args[0]->GetValue(prg),false);
	if(!Type)
		throw RPrgException(prg,"'"+args[0]->GetValue(prg)+"' is not a valid measure type");
	o->WriteStr("Current Measure for '"+args[0]->GetValue(prg)+"': "+args[1]->GetValue(prg));
	GMeasure* Mes=Type->GetPlugIn(args[1]->GetValue(prg),false);
	Mes->ReInit();
}


//------------------------------------------------------------------------------
void GAnalyzeDocsI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"Method needs one parameter");
	bool RAM;
	if((args[0]->GetValue(prg))=="0")
		RAM=false;
	else
		RAM=true;
	if(args.GetNb())
		throw RPrgException(prg,"The method needs no parameter.");
	o->WriteStr("Analyze Documents");
	if(!GALILEIApp->GetManager<GDocAnalyseManager>("DocAnalyse")->GetCurrentMethod())
		throw RPrgException(prg,"No Document Analyzing Method chosen.");
	Owner->Session->AnalyseDocs(RAM,dynamic_cast<GSlot*>(o));
}



//------------------------------------------------------------------------------
//
// class GSessionClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSessionClass::GSessionClass(GInstGALILEIApp* app)
	: RPrgClass("GSession"), Instance(false), App(app)
{
	Methods.InsertPtr(new GExecSqlI());
	Methods.InsertPtr(new GAnalyzeDocsI());
	Methods.InsertPtr(new GComputeProfilesI());
	Methods.InsertPtr(new GGroupProfilesI());
	Methods.InsertPtr(new GGroupDocsI());
	Methods.InsertPtr(new GStartSimulationI());
	Methods.InsertPtr(new GPerformDegradationI());
	Methods.InsertPtr(new GFdbksCycleI());
	Methods.InsertPtr(new GCompareIdealI());
	Methods.InsertPtr(new GAddIdealI());
	Methods.InsertPtr(new GAddProfilesI());
	Methods.InsertPtr(new GRealLifeI());
	Methods.InsertPtr(new GAddAssessmentsI());
	Methods.InsertPtr(new GTrackNewProfilesI());
	Methods.InsertPtr(new GClearNewProfilesI());
	Methods.InsertPtr(new GSetRandI());
	Methods.InsertPtr(new GRunStatI());
	Methods.InsertPtr(new GForceReComputeI());
	Methods.InsertPtr(new GSetSaveResultsI());
	Methods.InsertPtr(new GResetMeasureI());
}


//------------------------------------------------------------------------------
RPrgVar* GSessionClass::NewVar(RInterpreter* prg,RPrgOutput*,const RString& name,RContainer<RPrgVar,true,false>& params)
{
	if(Instance)
		throw RPrgException(prg,"Only one instance of 'GSession' may exist");
	if(params.GetNb()!=4)
		throw RPrgException(prg,"Constructor of 'GSession' has four parameters");
	RPrgVar* Var=new GInstSession(name,this,GALILEIApp->GetSession(),prg,params,App);
	Instance=true;
	return(Var);
}


//------------------------------------------------------------------------------
GSessionClass::~GSessionClass(void)
{
}



//------------------------------------------------------------------------------
//
// GGALILEIAppClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGALILEIAppClass::GGALILEIAppClass(void)
	: RPrgClass("GGALILEIApp")
{
	Methods.InsertPtr(new GSetSimulationParamI());
	Methods.InsertPtr(new GSetPlugInParamI());
	Methods.InsertPtr(new GSetCurrentPlugInI());
	Methods.InsertPtr(new GSetMeasureParamI());
	Methods.InsertPtr(new GSetCurrentMeasureI());
	Methods.InsertPtr(new GSetLogI());
	Methods.InsertPtr(new GOutputI());
	Methods.InsertPtr(new GGOutputI());
	Methods.InsertPtr(new GSOutputI());
	Methods.InsertPtr(new GSetTestI());
	Methods.InsertPtr(new GComputeTimeI());
	Methods.InsertPtr(new GResetTimeI());
}


//------------------------------------------------------------------------------
RPrgVar* GGALILEIAppClass::NewVar(RInterpreter* prg,RPrgOutput*,const RString&,RContainer<RPrgVar,true,false>&)
{
	throw RPrgException(prg,"Multiple instance of GGALILEIApp are not allowed");
}


//------------------------------------------------------------------------------
GGALILEIAppClass::~GGALILEIAppClass(void)
{
}



//------------------------------------------------------------------------------
//
// GGALILEIPrg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGALILEIPrg::GGALILEIPrg(RPrgOutput* o)
	: RInterpreter(o)
{
	GGALILEIAppClass* ptr;
	AddClass(ptr=new GGALILEIAppClass());
	GInstGALILEIApp* App;
	AddVar(App=new GInstGALILEIApp("GALILEIApp",ptr));
	AddClass(new GSessionClass(App));
}


//------------------------------------------------------------------------------
GGALILEIPrg::~GGALILEIPrg(void)
{
}
