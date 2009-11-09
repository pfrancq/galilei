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
#include <gsimulator.h>
#include <gsubject.h>
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
class GSimulatorClass : public R::RPrgClass
{
public:
	bool Instance;
	GSession* Session;
	GSimulator* Simulator;
	GInstGALILEIApp* App;

	GSimulatorClass(GInstGALILEIApp* app);
	virtual R::RPrgVar* NewVar(R::RInterpreter* prg,RPrgOutput* o,const R::RString& name,R::RContainer<R::RPrgVar,true,false>& params);
	virtual ~GSimulatorClass(void);
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
	bool DelSession;                     // Must the session be destroyed?
	GInstGALILEIApp* App;                // Application

	GInstSession(const RString& name,GSessionClass* c,GSession* session,RInterpreter* prg,RContainer<R::RPrgVar,true,false>& args,GInstGALILEIApp* app);
	virtual ~GInstSession(void);
};


//------------------------------------------------------------------------------
GInstSession::GInstSession(const RString& name,GSessionClass* c,GSession* session,RInterpreter* prg,RContainer<R::RPrgVar,true,false>& args,GInstGALILEIApp* app)
 : RPrgVarInst(name,c), Session(session), App(app)
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
// class GInstSimulator
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GInstSimulator : public RPrgVarInst
{
public:
	GSession* Session;		             // Session.
	GSimulator* Simulator;               // Simulator.
	double Precision;	             	 // Precision of the current clustering.
	double Recall;			             // Recall of the current clustering.
	double AdjustedRandIndex;		     // Adjusted Rand index of the current clustering.
	double J;		                     // J measure of the current clustering.
	double PercAss;			             // Percentage of correct assignments for the profiles last added.
	bool TrackNewProfiles;	             // When TrackNewProfile is true, the system
	GInstGALILEIApp* App;                // Application

	GInstSimulator(const RString& name,GSimulatorClass* c,GSession* session,GInstGALILEIApp* app);
	virtual ~GInstSimulator(void);
};


//------------------------------------------------------------------------------
GInstSimulator::GInstSimulator(const RString& name,GSimulatorClass* c,GSession* session,GInstGALILEIApp* app)
 : RPrgVarInst(name,c), Session(session), Simulator(session->GetSimulator()), TrackNewProfiles(false), App(app)
{
}


//------------------------------------------------------------------------------
GInstSimulator::~GInstSimulator(void)
{
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
class GPrintOutputI : public RPrgFunc
{
public:
	GPrintOutputI(void) : RPrgFunc("PrintOutput","Print a line in the output file") {}
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
	GStartSimulationI(void) : RPrgFunc("Start","Create a new ideal clustering and new profiles.") {}
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
class GShareDocumentsI : public RPrgFunc
{
public:
	GShareDocumentsI(void) : RPrgFunc("ShareDocuments","Simulate that documents are shared inside the different communities and that the best are assessed.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GComputeTrustI : public RPrgFunc
{
public:
	GComputeTrustI(void) : RPrgFunc("ComputeTrust","Compute the trust.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args);
};


//------------------------------------------------------------------------------
class GComputeSugsI : public RPrgFunc
{
public:
	GComputeSugsI(void) : RPrgFunc("ComputeSugs","ComputeSugs.") {}
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
	GSetSimulationParamI(void) : RPrgFunc("SetParam","Specify a value (2d argument) for a simulation parameter (1st argument).") {}
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
	void CommonTasks(R::RPrgOutput* o,GInstSimulator* Owner);
	GMeasure* Compare;
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
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GGALILEIApp'");

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
	Owner->OFile->SetSeparator(" ");
}


//------------------------------------------------------------------------------
void GPrintOutputI::Run(R::RInterpreter* prg,RPrgOutput*,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstGALILEIApp* Owner=dynamic_cast<GInstGALILEIApp*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GGALILEIApp'");
	if(!Owner->OFile)
		throw RPrgException(prg,"No current output defined");

	RCursor<RPrgVar> Cur(args);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RString Str(Cur()->GetValue(prg));
		if(Str.GetLen()<25)
			Str.SetLen(25," ");
		(*Owner->OFile)<<Str;
	}
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
	if(!GALILEIApp->GetCurrentPlugIn<GProfileCalc>("ProfileCalc"))
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
	if(!GALILEIApp->GetCurrentPlugIn<GGroupProfiles>("GroupProfiles"))
		throw RPrgException(prg,"No Profiles Grouping Method chosen.");
	if(!GALILEIApp->GetCurrentPlugIn<GPlugIn>("CommunityCalc"))
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
	if(!GALILEIApp->GetCurrentPlugIn<GGroupDocs>("GroupDocs"))
		throw RPrgException(prg,"No Documents Grouping Method chosen.");
	if(!GALILEIApp->GetCurrentPlugIn<GTopicCalc>("TopicCalc"))
		throw RPrgException(prg,"No Topic Description Method chosen.");
	Owner->Session->GroupDocs(dynamic_cast<GSlot*>(o));
}


//------------------------------------------------------------------------------
void GStartSimulationI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Create Ideal Groups");
	Owner->Simulator->StartSimulation();
}


//------------------------------------------------------------------------------
void GPerformDegradationI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	// Read Parameters
	GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");
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
	Owner->Simulator->PerformDegradation(What,Nb);

	// Compute J and T and put it in a file
	if(!What)
		return;
	GMeasure* Compare(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Topics Evaluation"));
	if(!Compare)
		throw RPrgException(prg,"'Topics Evaluation' is not a valid evaluation measure");
	Compare->Info(2,&Owner->AdjustedRandIndex);
	Compare->Info(3,&Owner->J);
	if(Owner->App->OFile)
		(*Owner->App->OFile)<<Owner->App->TestName<<Owner->AdjustedRandIndex<<Owner->J<<endl;
	if(Owner->App->GOFile)
		(*Owner->App->GOFile)<<Owner->AdjustedRandIndex<<Owner->J<<endl;
}


//------------------------------------------------------------------------------
void GShareDocumentsI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Share Documents");
	Owner->Simulator->Apply();
	Owner->Simulator->ShareDocuments();
}


//------------------------------------------------------------------------------
void GComputeTrustI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Share Documents");
	Owner->Session->ComputeTrust();
}


//------------------------------------------------------------------------------
void GComputeSugsI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSession'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Share Documents");
	Owner->Session->ComputeSugs();
}


//------------------------------------------------------------------------------
void GCompareIdealI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

	if(args.GetNb()!=1)
		throw RPrgException(prg,"Method needs one parameter");
	RString objects=args[0]->GetValue(prg);
	tObjType type(otNoClass);
	if(objects=="Communities Evaluation")
		type=otCommunity;
	if(objects=="Topics Evaluation")
		type=otTopic;
	if(type==otNoClass)
		throw RPrgException(prg,"Compare with unsupported type");
	o->WriteStr("Compare with Ideal "+objects+" Groups");

	GMeasure* Compare(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",objects));
	if(!Compare)
		throw RPrgException(prg,"'"+objects+"' is not a valid evaluation measure");
	Compare->Info(0,&Owner->Recall);
	Compare->Info(1,&Owner->Precision);
	Compare->Info(2,&Owner->AdjustedRandIndex);

	o->WriteStr("Recall: "+RString::Number(Owner->Recall)+"  -  Precision: "+RString::Number(Owner->Precision)+"  -  Total: "+RString::Number(Owner->AdjustedRandIndex));
	if(Owner->App->OFile)
	{
		RString Str;
		Str=Owner->App->TestName;
		if(Str.GetLen()<25)
			Str.SetLen(25," ");
		(*Owner->App->OFile)<<Str;
		Str=RString::Number(Owner->Recall);
		if(Str.GetLen()<25)
			Str.SetLen(25," ");
		(*Owner->App->OFile)<<Str;
		Str=RString::Number(Owner->Precision);
		if(Str.GetLen()<25)
			Str.SetLen(25," ");
		(*Owner->App->OFile)<<Str;
		Str=RString::Number(Owner->AdjustedRandIndex);
		if(Str.GetLen()<25)
			Str.SetLen(25," ");
		(*Owner->App->OFile)<<Str;
		if(Owner->TrackNewProfiles)
		{
			Str=RString::Number(Owner->PercAss);
			if(Str.GetLen()<25)
				Str.SetLen(25," ");
			(*Owner->App->OFile)<<Str;
		}
		(*Owner->App->OFile)<<endl;
	}
	if(Owner->App->GOFile)
	{
		(*Owner->App->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->AdjustedRandIndex;
		if(Owner->TrackNewProfiles)
			(*Owner->App->GOFile)<<Owner->PercAss;
		(*Owner->App->GOFile)<<endl;
	}
}


//------------------------------------------------------------------------------
void GSetSimulationParamI::Run(R::RInterpreter* prg,RPrgOutput*,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");
	if(args.GetNb()!=2)
		throw RPrgException(prg,"Method needs two parameters.");
	Owner->Simulator->GetSession()->Set(args[0]->GetValue(prg),args[1]->GetValue(prg),"Simulator");
	Owner->Simulator->Apply();
}


//------------------------------------------------------------------------------
void GAddIdealI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Create New Ideal Group");
	Owner->Simulator->Apply();
	Owner->Simulator->AddSubject();
}


//------------------------------------------------------------------------------
void GAddProfilesI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Adding Profiles");
	Owner->Simulator->Apply();
	o->WriteStr(RString::Number(Owner->Simulator->AddProfiles())+" new profiles created");
}


//------------------------------------------------------------------------------
void GRealLifeI::CommonTasks(RPrgOutput* o,GInstSimulator* Owner)
{
	GSlot* rec=dynamic_cast<GSlot*>(o);

	// Compute Profiles
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Compute Profiles: Current Method");
	}
	if(GSession::Break()) return;
	if(GALILEIApp->GetCurrentPlugIn<GLinkCalc>("LinkCalc",false))
		GALILEIApp->GetCurrentPlugIn<GLinkCalc>("LinkCalc")->ApplyConfig();
	GALILEIApp->GetCurrentPlugIn<GProfileCalc>("ProfileCalc")->ApplyConfig();
	Owner->Session->CalcProfiles(rec);

	// Group Profiles
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Group Profiles: Current Method");
	}
	if(GSession::Break()) return;
	GALILEIApp->GetCurrentPlugIn<GGroupProfiles>("GroupProfiles")->ApplyConfig();
	GALILEIApp->GetCurrentPlugIn<GPlugIn>("CommunityCalc")->ApplyConfig();
	Owner->Session->GroupProfiles(rec);

	// Compare Ideal
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Compare with Ideal Groups");
	}
	Compare->Info(0,&Owner->Recall);
	Compare->Info(1,&Owner->Precision);
	Compare->Info(2,&Owner->AdjustedRandIndex);
	if(rec)
		o->WriteStr("Recall: "+RString::Number(Owner->Recall)+"  -  Precision: "+RString::Number(Owner->Precision)+"  -  Total: "+RString::Number(Owner->AdjustedRandIndex));
	if(GSession::Break()) return;
	if(Owner->App->OFile)
		(*Owner->App->OFile)<<Owner->App->TestName<<Owner->Recall<<Owner->Precision<<Owner->AdjustedRandIndex<<What<<endl;
	if(Owner->App->GOFile)
		(*Owner->App->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->AdjustedRandIndex<<What<<endl;
}


//------------------------------------------------------------------------------
void GRealLifeI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

	Compare=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Communities Evaluation");
	if(!Compare)
		throw RPrgException(prg,"'Communities Evaluation' is not a valid evaluation measure");

	if(args.GetNb()!=4)
		throw RPrgException(prg,"Method needs four parameters.");

	if(GSession::Break()) return;

	// Save the information related to the number of profiles created (here it is always 1,1
	// Set the new parameters
	size_t NbMinProf(Owner->Simulator->GetSession()->GetUInt("NbProfMin"));
	size_t NbMaxProf(Owner->Simulator->GetSession()->GetUInt("NbProfMax"));
	double PercSocial(Owner->Simulator->GetSession()->GetUInt("PercSocial"));
	double NbSubjects(Owner->Simulator->GetSession()->GetUInt("NbSubjects"));
	bool RelSubjects(Owner->Simulator->GetSession()->GetUInt("RelSubjects"));
	Owner->Simulator->GetSession()->Set("NbProfMin","1");
	Owner->Simulator->GetSession()->Set("NbProfMax","1");
	Owner->Simulator->GetSession()->Set("PercSocial","0.0");
	Owner->Simulator->GetSession()->Set("NbSubjects","1.0");
	Owner->Simulator->GetSession()->Set("RelSubjectsl","false");
	Owner->Simulator->Apply();

	// Initialize the parameters
	GSlot* rec(dynamic_cast<GSlot*>(o));
	RRandom* Random(Owner->Session->GetRandom());
	size_t MaxStep(atoi(args[0]->GetValue(prg)));                  // Maximum number of steps.
	size_t MinFBStep(atoi(args[1]->GetValue(prg)));                // Minimum number of feedbacks before a new profile is created.
	size_t MaxFBStep(atoi(args[2]->GetValue(prg))-MinFBStep+1);    // Maximum number of feedbacks before a new profile is created.
	if(MaxFBStep<MinFBStep)
		MaxFBStep=MinFBStep;
	double Proba(atof(args[3]->GetValue(prg)));                    // Probability to create a profile of an existing topic
	size_t NbFdbks(0);                                             // Number of feedbacks left to perform before a profile must be created.
	What[1]=0;

	// Perform the real life
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Real Life: "+args[0]->GetValue(prg)+","+args[1]->GetValue(prg)+","+args[2]->GetValue(prg)+","+args[3]->GetValue(prg));
	}
	Owner->Simulator->StartSimulation(false);
	for(size_t NbStep=0;NbStep<MaxStep;NbStep++)
	{
		// The step performed
		Owner->App->TestName=RString::Number(NbStep);

		// Look the number of feedbacks left
		if(NbFdbks)
		{
			// One feedback left done
			NbFdbks--;
			if(rec)
			{
				rec->Interact();
				rec->WriteStr("Create Feedbacks Cycle");
			}

			What[0]='F';
			Owner->Simulator->ShareDocuments();
		}
		else
		{
			// Create a new profile
			if(rec)
			{
				rec->Interact();
				rec->WriteStr("Create 1 new profile");
			}

			// Look if new topic or existing one
			What[0]='-';   // Suppose no profile can be created
			if(NbStep&&(Random->GetValue()<Proba))
			{
				// Create One profile of an existing topic
				if(Owner->Simulator->AddProfiles())
					What[0]='E';
			}
			else
			{
				// Create one profile of a new topic
				if(Owner->Simulator->AddSubject())
				{
					What[0]='N';
				}
				else
				{
					Proba=1.0;  // Cannot create any new topic
					if(Owner->Simulator->AddProfiles())
						What[0]='E';
				}
			}

			// How much feedbacks now ?
			if(NbStep)     // No feedback cycle alone
				NbFdbks=Random->GetValue(MaxFBStep-MinFBStep+1)+MinFBStep;
		}

		// Compute, Group and Compare
		if(GSession::Break()) return;
		CommonTasks(o,Owner);
		if(GSession::Break()) return;
	}

	// Restore the values saved
	Owner->Simulator->GetSession()->SetUInt("NbProfMin",NbMinProf);
	Owner->Simulator->GetSession()->SetUInt("NbProfMax",NbMaxProf);
	Owner->Simulator->GetSession()->SetUInt("PercSocial",PercSocial);
	Owner->Simulator->GetSession()->SetUInt("NbSubjects",NbSubjects);
	Owner->Simulator->GetSession()->SetUInt("RelSubjects",RelSubjects);
	Owner->Simulator->Apply();
}


//------------------------------------------------------------------------------
void GAddAssessmentsI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
	if(!Owner)
		throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameters.");
	o->WriteStr("Adding Assessments");
	Owner->Simulator->Apply();
	Owner->Simulator->AddAssessments();
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

	if(args.GetNb())
		throw RPrgException(prg,"Method needs no parameter");

	// Create the root node
	RXMLStruct xml;
	RXMLTag* Root(new RXMLTag("Statistics"));
	xml.AddTag(0,Root);

	// Compute the statistics
	int i;
	R::RCastCursor<GPlugIn,GStatsCalc> Cur(GALILEIApp->GetPlugIns<GStatsCalc>("StatsCalc"));
	for(Cur.Start(),i=1;!Cur.End();Cur.Next(),i++)
		Cur()->Compute(&xml,*Root);
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
	GPlugInFactory* Config(GALILEIApp->GetFactory(args[0]->GetValue(prg),args[1]->GetValue(prg)));
	if(!Config)
		throw RPrgException(prg,"'"+args[1]->GetValue(prg)+"' is not a plug-in for the manager '"+args[0]->GetValue(prg)+"'");
	Config->Set(args[2]->GetValue(prg),args[3]->GetValue(prg));
	Config->Apply();
}


//------------------------------------------------------------------------------
void GSetCurrentPlugInI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=2)
		throw RPrgException(prg,"Method needs two parameters.");
	GALILEIApp->SetCurrentPlugIn(args[0]->GetValue(prg),args[1]->GetValue(prg),false);
	o->WriteStr("Current Method for '"+args[0]->GetValue(prg)+"': "+args[1]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GSetMeasureParamI::Run(R::RInterpreter* prg,RPrgOutput*,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=4)
		throw RPrgException(prg,"Method needs four parameters.");
	GPlugInFactory* Config(GALILEIApp->GetFactory("Measures",args[0]->GetValue(prg),args[1]->GetValue(prg)));
	if(!Config)
		throw RPrgException(prg,"'"+args[1]->GetValue(prg)+"' is not a measure for the type '"+args[0]->GetValue(prg)+"'");
	Config->Set(args[2]->GetValue(prg),args[3]->GetValue(prg));
	Config->Apply();
}


//------------------------------------------------------------------------------
void GSetCurrentMeasureI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=2)
		throw RPrgException(prg,"Method needs two parameters.");
	GALILEIApp->SetCurrentPlugIn("Measures",args[0]->GetValue(prg),args[1]->GetValue(prg),false);
	o->WriteStr("Current Measure for '"+args[0]->GetValue(prg)+"': "+args[1]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GResetMeasureI::Run(R::RInterpreter* prg,RPrgOutput* o,RPrgVarInst*,R::RContainer<RPrgVar,true,false>& args)
{
	ShowInst(this,prg,args);
	if(args.GetNb()!=2)
		throw RPrgException(prg,"Method needs two parameters.");
	GMeasure* Mes(GALILEIApp->GetPlugIn<GMeasure>("Measures",args[0]->GetValue(prg),args[1]->GetValue(prg)));
	o->WriteStr("Current Measure for '"+args[0]->GetValue(prg)+"': "+args[1]->GetValue(prg));
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
	if(!GALILEIApp->GetCurrentPlugIn<GDocAnalyse>("DocAnalyse"))
		throw RPrgException(prg,"No Document Analyzing Method chosen.");
	Owner->Session->AnalyseDocs(RAM,dynamic_cast<GSlot*>(o));
}



//------------------------------------------------------------------------------
//
// class GSimulatorClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSimulatorClass::GSimulatorClass(GInstGALILEIApp* app)
	: RPrgClass("GSimulator"), Instance(false), App(app)
{
	Methods.InsertPtr(new GStartSimulationI());
	Methods.InsertPtr(new GPerformDegradationI());
	Methods.InsertPtr(new GShareDocumentsI());
	Methods.InsertPtr(new GCompareIdealI());
	Methods.InsertPtr(new GAddIdealI());
	Methods.InsertPtr(new GAddProfilesI());
	Methods.InsertPtr(new GRealLifeI());
	Methods.InsertPtr(new GAddAssessmentsI());
	Methods.InsertPtr(new GSetSimulationParamI());
}


//------------------------------------------------------------------------------
RPrgVar* GSimulatorClass::NewVar(RInterpreter* prg,RPrgOutput*,const RString& name,RContainer<RPrgVar,true,false>& params)
{
	if(Instance)
		throw RPrgException(prg,"Only one instance of 'GSimulator' may exist");
	if(params.GetNb())
		throw RPrgException(prg,"Constructor of 'GSimulator' has no parameter");
	RPrgVar* Var=new GInstSimulator(name,this,GALILEIApp->GetSession(),App);
	Instance=true;
	return(Var);
}


//------------------------------------------------------------------------------
GSimulatorClass::~GSimulatorClass(void)
{
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
	Methods.InsertPtr(new GSetRandI());
	Methods.InsertPtr(new GRunStatI());
	Methods.InsertPtr(new GForceReComputeI());
	Methods.InsertPtr(new GSetSaveResultsI());
	Methods.InsertPtr(new GResetMeasureI());
	Methods.InsertPtr(new GComputeSugsI());
	Methods.InsertPtr(new GComputeTrustI());
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
	Methods.InsertPtr(new GSetPlugInParamI());
	Methods.InsertPtr(new GSetCurrentPlugInI());
	Methods.InsertPtr(new GSetMeasureParamI());
	Methods.InsertPtr(new GSetCurrentMeasureI());
	Methods.InsertPtr(new GSetLogI());
	Methods.InsertPtr(new GOutputI());
	Methods.InsertPtr(new GPrintOutputI());
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
	AddClass(new GSimulatorClass(App));
}


//------------------------------------------------------------------------------
GGALILEIPrg::~GGALILEIPrg(void)
{
}
