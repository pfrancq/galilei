/*

	GALILEI Research Project

	GSessionPrg.cpp

	Program running a Session object  - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
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
#include <gprofilecalc.h>
#include <glinkcalc.h>
#include <gsession.h>
#include <gstorage.h>
#include <gslot.h>
#include <ggalileiapp.h>
#include <gdocanalyze.h>
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

	friend class GGALILEIPrg;
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
	GSession* Session;      // Session associated to the application.
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
	: RPrgVarInst(name,c), Session(0), OFile(0),GOFile(0), SOFile(0),DSOFile(0)
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
	GInstGALILEIApp* App;                // Application

	GInstSession(const RString& name,GSessionClass* c,RInterpreter* prg,RContainer<R::RPrgVar,true,false>& args,GInstGALILEIApp* app);
	virtual ~GInstSession(void);
};


//------------------------------------------------------------------------------
GInstSession::GInstSession(const RString& name,GSessionClass* c,RInterpreter* prg,RContainer<R::RPrgVar,true,false>& args,GInstGALILEIApp* app)
 : RPrgVarInst(name,c), Session(GALILEIApp->GetSession(args[0]->GetValue(prg),false)), App(app)
{
	bool DoDocs=args[1]->GetValue(prg)=="1";
	bool DoProfiles=args[2]->GetValue(prg)=="1";
	bool DoCommunities=args[3]->GetValue(prg)=="1";
	bool DoTopics=args[4]->GetValue(prg)=="1";
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
class SetRand : public RPrgFunc
{
public:
	SetRand(void) : RPrgFunc("SetRand","Set the random generator to a given value.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class SetOutput : public RPrgFunc
{
public:
	SetOutput(void) : RPrgFunc("SetOutput","Specify where the global results should be stored.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class PrintOutput : public RPrgFunc
{
public:
	PrintOutput(void) : RPrgFunc("PrintOutput","Print a line in the output file") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput*,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class SetGraphOutput : public RPrgFunc
{
public:
	SetGraphOutput(void) : RPrgFunc("SetGraphOutput","Specify where the results for graphics should be stored.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class SetStatsOutput : public RPrgFunc
{
public:
	SetStatsOutput(void) : RPrgFunc("SetStatsOutput","Specify where the detailed results should be stored.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class SetTest : public RPrgFunc
{
public:
	SetTest(void) : RPrgFunc("SetTest","Set the current test label to a given string.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class ExecSql : public RPrgFunc
{
public:
	ExecSql(void) : RPrgFunc("ExecSql","Execute a given SQL string.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class ComputeProfiles : public RPrgFunc
{
public:
	ComputeProfiles(void) : RPrgFunc("ComputeProfiles","Compute the profiles.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class GroupProfiles : public RPrgFunc
{
public:
	GroupProfiles(void) : RPrgFunc("GroupProfiles","Group the profiles.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class GroupDocs : public RPrgFunc
{
public:
	GroupDocs(void) : RPrgFunc("GroupDocs","Group the documents.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class StartSimulation : public RPrgFunc
{
public:
	StartSimulation(void) : RPrgFunc("Start","Create a new ideal clustering and new profiles.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class PerformDegradation : public RPrgFunc
{
public:
	PerformDegradation(void) : RPrgFunc("PerformDegradation","Perform a degradation of an ideal clustering.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class ShareDocuments : public RPrgFunc
{
public:
	ShareDocuments(void) : RPrgFunc("ShareDocuments","Simulate that documents are shared inside the different communities and that the best are assessed.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
	{
		ShowInst(this,prg,args);
		GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
		if(!Owner)
			throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

		if(args.GetNb())
			throw RPrgException(prg,"Method needs no parameters.");
		o->WriteStr("Share Documents");
		Owner->Simulator->ApplyParams();
		Owner->Simulator->ShareDocuments();
	}
};


//------------------------------------------------------------------------------
class ComputeTrust : public RPrgFunc
{
public:
	ComputeTrust(void) : RPrgFunc("ComputeTrust","Compute the trust.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class ComputeSugs : public RPrgFunc
{
public:
	ComputeSugs(void) : RPrgFunc("ComputeSugs","ComputeSugs.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class CompareIdeal : public RPrgFunc
{
public:
	CompareIdeal(void) : RPrgFunc("CompareIdeal","Compare the computed clustering with the ideal one.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class SetSimulationParam : public RPrgFunc
{
public:
	SetSimulationParam(void) : RPrgFunc("SetSimulationParam","Specify a value (2d argument) for a simulation parameter (1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput*,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
	{
		ShowInst(this,prg,args);
		GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
		if(!Owner)
			throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");
		if(args.GetNb()!=2)
			throw RPrgException(prg,"Method needs two parameters.");
		Owner->Simulator->FindParam<RParamValue>(args[0]->GetValue(prg))->Set(args[1]->GetValue(prg));
		Owner->Simulator->ApplyParams();
	}
};


//------------------------------------------------------------------------------
class AddIdeal : public RPrgFunc
{
public:
	AddIdeal(void) : RPrgFunc("AddIdeal","Add a new topic to the ideal clustering and create new profiles associated to it.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
	{
		ShowInst(this,prg,args);
		GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
		if(!Owner)
			throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

		if(args.GetNb())
			throw RPrgException(prg,"Method needs no parameters.");
		o->WriteStr("Create New Ideal Group");
		Owner->Simulator->ApplyParams();
		Owner->Simulator->AddSubject();
	}
};


//------------------------------------------------------------------------------
class AddProfiles : public RPrgFunc
{
public:
	AddProfiles(void) : RPrgFunc("AddProfiles","Add new profiles to a randomly chosen topic.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
	{
		ShowInst(this,prg,args);
		GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
		if(!Owner)
			throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

		if(args.GetNb())
			throw RPrgException(prg,"Method needs no parameters.");
		o->WriteStr("Adding Profiles");
		Owner->Simulator->ApplyParams();
		o->WriteStr(RString::Number(Owner->Simulator->AddProfiles())+" new profiles created");
	}
};


//------------------------------------------------------------------------------
class RealLife : public RPrgFunc
{
public:
	GSession* Session;
	char What[2];
	GMeasure* Compare;

	RealLife(void) : RPrgFunc("RealLife","Perform the simulation of a complete system running a given number of step times."), Session(0) {}
	void CommonTasks(RPrgOutput* o,GInstSimulator* Owner)
	{
		GSlot* rec=dynamic_cast<GSlot*>(o);

		// Compute Profiles
		if(rec)
		{
			rec->Interact();
			rec->WriteStr("Compute Profiles: Current Method");
		}
		if(Session->MustBreak()) return;
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
		if(Session->MustBreak()) return;
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
		if(Session->MustBreak()) return;
		if(Owner->App->OFile)
			(*Owner->App->OFile)<<Owner->App->TestName<<Owner->Recall<<Owner->Precision<<Owner->AdjustedRandIndex<<What<<endl;
		if(Owner->App->GOFile)
			(*Owner->App->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->AdjustedRandIndex<<What<<endl;
	}

	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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

		if(Session->MustBreak()) return;

		// Save the information related to the number of profiles created (here it is always 1,1
		// Set the new parameters
		size_t NbMinProf(Owner->Simulator->FindParam<RParamValue>("NbProfMin")->GetUInt());
		size_t NbMaxProf(Owner->Simulator->FindParam<RParamValue>("NbProfMax")->GetUInt());
		double PercSocial(Owner->Simulator->FindParam<RParamValue>("PercSocial")->GetDouble());
		double NbSubjects(Owner->Simulator->FindParam<RParamValue>("NbSubjects")->GetDouble());
		bool RelSubjects(Owner->Simulator->FindParam<RParamValue>("RelSubjects")->GetBool());
		Owner->Simulator->FindParam<RParamValue>("NbProfMin")->Set("1");
		Owner->Simulator->FindParam<RParamValue>("NbProfMax")->Set("1");
		Owner->Simulator->FindParam<RParamValue>("PercSocial")->Set("0.0");
		Owner->Simulator->FindParam<RParamValue>("NbSubjects")->Set("1.0");
		Owner->Simulator->FindParam<RParamValue>("RelSubjects")->Set("false");
		Owner->Simulator->ApplyParams();

		// Initialize the parameters
		GSlot* rec(dynamic_cast<GSlot*>(o));
		RRandom& Random(Owner->Session->GetRandom());
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
				if(NbStep&&(Random.GetValue()<Proba))
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
					NbFdbks=Random.GetValue(MaxFBStep-MinFBStep+1)+MinFBStep;
			}

			// Compute, Group and Compare
			if(Session->MustBreak()) return;
			CommonTasks(o,Owner);
			if(Session->MustBreak()) return;
		}

		// Restore the values saved
		Owner->Simulator->FindParam<RParamValue>("NbProfMin")->SetUInt(NbMinProf);
		Owner->Simulator->FindParam<RParamValue>("NbProfMax")->SetUInt(NbMaxProf);
		Owner->Simulator->FindParam<RParamValue>("PercSocial")->SetDouble(PercSocial);
		Owner->Simulator->FindParam<RParamValue>("NbSubjects")->SetDouble(NbSubjects);
		Owner->Simulator->FindParam<RParamValue>("RelSubjects")->SetBool(RelSubjects);
		Owner->Simulator->ApplyParams();
	}
};


//------------------------------------------------------------------------------
class AddAssessments : public RPrgFunc
{
public:
	AddAssessments(void) : RPrgFunc("AddAssessments","Simulate that the profiles assess a given number of randomly chosen documents.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
	{
		ShowInst(this,prg,args);
		GInstSimulator* Owner=dynamic_cast<GInstSimulator*>(inst);
		if(!Owner)
			throw RPrgException(prg,"'"+inst->GetName()+"' is not an object 'GSimulator'");

		if(args.GetNb())
			throw RPrgException(prg,"Method needs no parameters.");
		o->WriteStr("Adding Assessments");
		Owner->Simulator->ApplyParams();
		Owner->Simulator->AddAssessments();
	}
};


//------------------------------------------------------------------------------
class ResetTime : public RPrgFunc
{
public:
	ResetTime(void) : RPrgFunc("ResetTime","Reset the time counter.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class ComputeTime : public RPrgFunc
{
public:
	ComputeTime(void) : RPrgFunc("ComputeTime","Compute the interval from the last called to ResetTime.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class ForceReCompute: public RPrgFunc
{
public:
	ForceReCompute(void) : RPrgFunc("ForceReCompute","Specify if all objects should be recomputed or if an incremental method should be used.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput*,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class SetSaveResults: public RPrgFunc
{
public:
	SetSaveResults(void) : RPrgFunc("SetSaveResults","Specify if the results must be stored in the current storage.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput*,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
};


//------------------------------------------------------------------------------
class SetPlugInParam : public RPrgFunc
{
public:
	GInstGALILEIApp* App;
	SetPlugInParam(void)
		: RPrgFunc("SetPlugInParam","Specify a value (4th argument) for a parameter (3rd argument) for a plug-in (with name=2nd argument and type=1st argument)."),
		  App(0) {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput*,RPrgVarInst*,R::RContainer<R::RPrgVar,true,false>& args)
	{
		if(!App->Session)
			throw RPrgException(prg,"Method needs a running session.");
		ShowInst(this,prg,args);
		if(args.GetNb()!=4)
			throw RPrgException(prg,"Method needs four parameters.");
		GPlugIn* Config(GALILEIApp->GetPlugIn(args[0]->GetValue(prg),args[1]->GetValue(prg)));
		if(!Config)
			throw RPrgException(prg,"'"+args[1]->GetValue(prg)+"' is not a plug-in for the manager '"+args[0]->GetValue(prg)+"'");
		Config->FindParam<RParamValue>(args[2]->GetValue(prg))->Set(args[3]->GetValue(prg));
		Config->ApplyConfig();
	}
};


//------------------------------------------------------------------------------
class SetCurrentPlugIn : public RPrgFunc
{
public:
	SetCurrentPlugIn(void)
		: RPrgFunc("SetCurrentPlugIn","Specify the current plug-in (2th argument) for a given type(1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst*,R::RContainer<R::RPrgVar,true,false>& args)
	{
		ShowInst(this,prg,args);
		if(args.GetNb()!=2)
			throw RPrgException(prg,"Method needs two parameters.");
		GALILEIApp->SetCurrentPlugIn(args[0]->GetValue(prg),args[1]->GetValue(prg),false);
		o->WriteStr("Current Method for '"+args[0]->GetValue(prg)+"': "+args[1]->GetValue(prg));
	}
};


//------------------------------------------------------------------------------
class SetMeasureParam : public RPrgFunc
{
public:
	GInstGALILEIApp* App;
	SetMeasureParam(void) :
		RPrgFunc("SetMeasureParam","Specify a value (4th argument) for a parameter (3rd argument) for a measure (with name=2nd argument and type=1st argument)."),
		App(0) {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput*,RPrgVarInst*,R::RContainer<R::RPrgVar,true,false>& args)
	{
		if(!App->Session)
			throw RPrgException(prg,"Method needs a running session.");
		ShowInst(this,prg,args);
		if(args.GetNb()!=4)
			throw RPrgException(prg,"Method needs four parameters.");
		GPlugIn* Config(GALILEIApp->GetPlugIn("Measures",args[0]->GetValue(prg),args[1]->GetValue(prg)));
		if(!Config)
			throw RPrgException(prg,"'"+args[1]->GetValue(prg)+"' is not a measure for the type '"+args[0]->GetValue(prg)+"'");
		Config->FindParam<RParamValue>(args[2]->GetValue(prg))->Set(args[3]->GetValue(prg));
		Config->ApplyConfig();
	}
};


//------------------------------------------------------------------------------
class SetCurrentMeasure : public RPrgFunc
{
public:
	SetCurrentMeasure(void) : RPrgFunc("SetCurrentMeasure","Specify the current measure (2th argument) for a given type(1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst*,R::RContainer<R::RPrgVar,true,false>& args)
	{
		ShowInst(this,prg,args);
		if(args.GetNb()!=2)
			throw RPrgException(prg,"Method needs two parameters.");
		GALILEIApp->SetCurrentPlugIn("Measures",args[0]->GetValue(prg),args[1]->GetValue(prg),false);
		o->WriteStr("Current Measure for '"+args[0]->GetValue(prg)+"': "+args[1]->GetValue(prg));
	}
};


//------------------------------------------------------------------------------
class ResetMeasure : public RPrgFunc
{
public:
	ResetMeasure(void) : RPrgFunc("ResetMeasure","Reset the measure (2th argument) for a given type (1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst*,R::RContainer<R::RPrgVar,true,false>& args)
	{
		ShowInst(this,prg,args);
		if(args.GetNb()!=2)
			throw RPrgException(prg,"Method needs two parameters.");
		GMeasure* Mes(GALILEIApp->GetPlugIn<GMeasure>("Measures",args[0]->GetValue(prg),args[1]->GetValue(prg)));
		o->WriteStr("Current Measure for '"+args[0]->GetValue(prg)+"': "+args[1]->GetValue(prg));
		Mes->ReInit();
	}
};


//------------------------------------------------------------------------------
class RunTool : public RPrgFunc
{
public:
	RunTool(void) : RPrgFunc("RunTool","Run a specific tool (1st argument).") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
	{
		ShowInst(this,prg,args);
		GInstSession* Owner=dynamic_cast<GInstSession*>(inst);
		if(args.GetNb()!=2)
			throw RPrgException(prg,"Method needs two parameters.");
		Owner->Session->RunTool(args[0]->GetValue(prg),args[1]->GetValue(prg),dynamic_cast<GSlot*>(o));
	}
};


//------------------------------------------------------------------------------
class AnalyzeDocs : public RPrgFunc
{
public:
	AnalyzeDocs(void) : RPrgFunc("AnalyzeDocs","Analyze the documents.") {}
	virtual void Run(R::RInterpreter* prg,R::RPrgOutput* o,RPrgVarInst* inst,R::RContainer<R::RPrgVar,true,false>& args)
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
		if(!GALILEIApp->GetCurrentPlugIn<GDocAnalyze>("DocAnalyze"))
			throw RPrgException(prg,"No Document Analyzing Method chosen.");
		Owner->Session->AnalyzeDocs(RAM,dynamic_cast<GSlot*>(o));
	}
};



//------------------------------------------------------------------------------
//
// class GSimulatorClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSimulatorClass::GSimulatorClass(GInstGALILEIApp* app)
	: RPrgClass("GSimulator"), Instance(false), App(app)
{
	Methods.InsertPtr(new StartSimulation());
	Methods.InsertPtr(new PerformDegradation());
	Methods.InsertPtr(new ShareDocuments());
	Methods.InsertPtr(new CompareIdeal());
	Methods.InsertPtr(new AddIdeal());
	Methods.InsertPtr(new AddProfiles());
	Methods.InsertPtr(new RealLife());
	Methods.InsertPtr(new AddAssessments());
	Methods.InsertPtr(new SetSimulationParam());
}


//------------------------------------------------------------------------------
RPrgVar* GSimulatorClass::NewVar(RInterpreter* prg,RPrgOutput*,const RString& name,RContainer<RPrgVar,true,false>& params)
{
	if(!App->Session)
		throw RPrgException(prg,"Session not declared in this program");
	if(Instance)
		throw RPrgException(prg,"Only one instance of 'GSimulator' may exist");
	if(params.GetNb())
		throw RPrgException(prg,"Constructor of 'GSimulator' has no parameter");
	RPrgVar* Var=new GInstSimulator(name,this,App->Session,App);
	dynamic_cast<RealLife*>(Methods.GetPtr("RealLife"))->Session=App->Session;
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
	: RPrgClass("GSession"), App(app)
{
	Methods.InsertPtr(new ExecSql());
	Methods.InsertPtr(new AnalyzeDocs());
	Methods.InsertPtr(new ComputeProfiles());
	Methods.InsertPtr(new GroupProfiles());
	Methods.InsertPtr(new GroupDocs());
	Methods.InsertPtr(new SetRand());
	Methods.InsertPtr(new ForceReCompute());
	Methods.InsertPtr(new SetSaveResults());
	Methods.InsertPtr(new ResetMeasure());
	Methods.InsertPtr(new ComputeSugs());
	Methods.InsertPtr(new ComputeTrust());
	Methods.InsertPtr(new RunTool());
}


//------------------------------------------------------------------------------
RPrgVar* GSessionClass::NewVar(RInterpreter* prg,RPrgOutput*,const RString& name,RContainer<RPrgVar,true,false>& params)
{
	if(App->Session)
		throw RPrgException(prg,"Session already declared in this program");
	if(params.GetNb()!=5)
		throw RPrgException(prg,"Constructor of 'GSession' has four parameters");
	GInstSession* Var=new GInstSession(name,this,prg,params,App);
	App->Session=Var->Session;
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
	Methods.InsertPtr(new SetPlugInParam());
	Methods.InsertPtr(new SetCurrentPlugIn());
	Methods.InsertPtr(new SetMeasureParam());
	Methods.InsertPtr(new SetCurrentMeasure());
	Methods.InsertPtr(new SetOutput());
	Methods.InsertPtr(new PrintOutput());
	Methods.InsertPtr(new SetGraphOutput());
	Methods.InsertPtr(new SetStatsOutput());
	Methods.InsertPtr(new SetTest());
	Methods.InsertPtr(new ComputeTime());
	Methods.InsertPtr(new ResetTime());
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
	dynamic_cast<SetPlugInParam*>(ptr->GetMethod("SetPlugInParam"))->App=App;
	dynamic_cast<SetMeasureParam*>(ptr->GetMethod("SetMeasureParam"))->App=App;
	AddClass(new GSessionClass(App));
	AddClass(new GSimulatorClass(App));
}


//------------------------------------------------------------------------------
GGALILEIPrg::~GGALILEIPrg(void)
{
}
