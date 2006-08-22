/*

	GALILEI Research Project

	GSessionPrg.cpp

	Program running a Session object  - Implementation.

	Copyright 2002-2005 by the UniversitÃ© Libre de Bruxelles.

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
// include files for R Project
#include <rprgvar.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gsessionprg.h>
#include <glang.h>
#include <ggrouping.h>
#include <ggroupcalc.h>
#include <gsubjects.h>
#include <gprofilecalc.h>
#include <glinkcalc.h>
#include <gstatscalc.h>
#include <gsession.h>
#include <gstorage.h>
#include <gslot.h>
#include <ggalileiapp.h>
#include <gmeasure.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Declare all the instructions of the class 'Session'.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSM : public R::RPrgFunc
{
public:
	char tmp[3000];
	GPrgClassSession* Owner;
	GSM(const RString& name,const RString& desc,GPrgClassSession* o)
		: RPrgFunc(name,desc), Owner(o) {}
};


//------------------------------------------------------------------------------
class GSetRandI : public GSM
{
public:
	GSetRandI(GPrgClassSession* o) : GSM("SetRand","Set the random generator to a given value.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GOutputI : public GSM
{
public:
	GOutputI(GPrgClassSession* o) : GSM("SetOutput","Specify where the global results should be stored.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GGOutputI : public GSM
{
public:
	GGOutputI(GPrgClassSession* o) : GSM("SetGraphOutput","Specify where the results for graphics should be stored.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GSOutputI : public GSM
{
public:
	GSOutputI(GPrgClassSession* o) : GSM("SetStatsOutput","Specify where the detailled results should be stored.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GSetLinksMethodI : public GSM
{
public:
	GSetLinksMethodI(GPrgClassSession* o) : GSM("SetLinksMethod","Set the current method used to exploit the links.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GTestI : public GSM
{
public:
	GTestI(GPrgClassSession* o) : GSM("Test","Set the current test label to a given string.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GLogI : public GSM
{
public:
	GLogI(GPrgClassSession* o) : GSM("Log","Specify a log file.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GSqlI : public GSM
{
public:
	GSqlI(GPrgClassSession* o) : GSM("ExecSql","Execute a given SQL string.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GComputeProfilesI : public GSM
{
public:
	GComputeProfilesI(GPrgClassSession* o) : GSM("ComputeProfiles","Compute the profiles.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GGroupProfilesI : public GSM
{
public:
	GGroupProfilesI(GPrgClassSession* o) : GSM("GroupProfiles","Group the (sub)profiles.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GCreateIdealI : public GSM
{
public:
	GCreateIdealI(GPrgClassSession* o) : GSM("CreateIdeal","Create a new ideal clustering and new profiles.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GFdbksCycleI : public GSM
{
public:
	GFdbksCycleI(GPrgClassSession* o) : GSM("FdbksCycle","Simulate that documents are shared inside the different communities and that the best are assessed.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GCompareIdealI : public GSM
{
public:
	GCompareIdealI(GPrgClassSession* o) : GSM("CompareIdeal","Compare the computed clustering with the ideal one.",o) {}
	virtual void Run(R::RPrg*,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GSetSubjectsParamI : public GSM
{
public:
	GSetSubjectsParamI(GPrgClassSession* o) : GSM("SetSubjectsParam","Specify a value (2d argument) for a simulation parameter (1st argument).",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GSetComputingParamI : public GSM
{
public:
	GSetComputingParamI(GPrgClassSession* o) : GSM("SetComputingParam","Specify a value (2d argument) for a profiles computing parameter (1st argument).",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GSetGroupingParamI : public GSM
{
public:
	GSetGroupingParamI(GPrgClassSession* o) : GSM("SetGroupingParam","Specify a value (2d argument) for a profiles grouping parameter (1st argument).",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GAddIdealI : public GSM
{
public:
	GAddIdealI(GPrgClassSession* o) : GSM("AddIdeal","Add a new topic to the ideal clustering and create new profiles associated to it.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GAddProfilesI : public GSM
{
public:
	GAddProfilesI(GPrgClassSession* o) : GSM("AddProfiles","Add new profiles to a randomly chosen topic.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GRealLifeI : public GSM
{
	char What[2];
	void CommonTasks(R::RPrgOutput* o);
public:
	GRealLifeI(GPrgClassSession* o) : GSM("RealLife","Perform the simulation of a comlete system running a given number of step times.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GAddAssessmentsI : public GSM
{
public:
	GAddAssessmentsI(GPrgClassSession* o) : GSM("AddAssessments","Simulate that the profiles assess a given number of randomly chosen documents.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GTrackNewProfilesI : public GSM
{
public:
	GTrackNewProfilesI(GPrgClassSession* o) : GSM("TrackNewProfiles","Specify if the new profiles must be tracked.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GClearNewProfilesI : public GSM
{
public:
	GClearNewProfilesI(GPrgClassSession* o) : GSM("ClearNewProfiles","Clear the list of profiles considered as new.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GResetTimeI : public GSM
{
public:
	GResetTimeI(GPrgClassSession* o) : GSM("ResetTime","Reset the time counter.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GComputeTimeI : public GSM
{
public:
	GComputeTimeI(GPrgClassSession* o) : GSM("ComputeTime","Compute the interval from the last called to ResetTime.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GSetStatParamI: public GSM
{
public:
	GSetStatParamI(GPrgClassSession* o) : GSM("SetStatParam","Specify a value (2d argument) for a statistics parameter (1st argument).",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GRunStatI: public GSM
{
public:
	GRunStatI(GPrgClassSession* o) : GSM("RunStat","Run all enabled statistics.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GForceReComputeI: public GSM
{
public:
	GForceReComputeI(GPrgClassSession* o) : GSM("ForceReCompute","Specify if all objects should be recomputed or if an incremental method should be used.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
class GSetSaveResultsI: public GSM
{
public:
	GSetSaveResultsI(GPrgClassSession* o) : GSM("SetSaveResults","Specify if the results must be stored in the current storage.",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};


//------------------------------------------------------------------------------
// (c) 2006, Michaël Noiret.
class GSetMeasuresParamI : public GSM
{
public:
	GSetMeasuresParamI(GPrgClassSession* o) : GSM("SetMeasuresParam","Specify a value (4th argument) for a parameter (3rd argument) for a measure (with name=2nd argument and type=1st argument) .",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args);
};



//------------------------------------------------------------------------------
//
// Instructions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GSetRandI::Run(RPrg* prg,RPrgOutput* o,RContainer<RPrgVar,true,false>* args)
{
	int Rand;

	if(args->GetNb()!=1)
		throw RException("The rand value must be specified.");
	o->WriteStr("Set Rand value: "+(*args)[0]->GetValue(prg));
	Rand=atoi((*args)[0]->GetValue(prg));
	if(Rand!=0)
	{
		Owner->Session->SetCurrentRandom(Rand);
	}

}


//------------------------------------------------------------------------------
void GOutputI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=1)
		throw RException("A filename must be specified.");
	o->WriteStr("Create Output file: "+(*args)[0]->GetValue(prg));
	if(Owner->OFile)
	{
		delete Owner->OFile;
		Owner->OFile=0;
	}
	Owner->OFile=new RTextFile(((*args)[0]->GetValue(prg)));
	Owner->OFile->Open(RIO::Create);
	Owner->OFile->SetSeparator("\t");
	(*Owner->OFile)<<"Sets"<<"Recall"<<"Precision"<<"Total";
	if(Owner->TrackNewProfiles)
	{
		(*Owner->OFile)<<"TrackNew";
	}
	(*Owner->OFile)<<endl;
}


//------------------------------------------------------------------------------
void GGOutputI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=1)
		throw RException("A filename must be specified.");
	o->WriteStr("Create Graph Output file: "+(*args)[0]->GetValue(prg));
	if(Owner->GOFile)
	{
		delete Owner->GOFile;
		Owner->GOFile=0;
	}
	Owner->GOFile=new RTextFile((*args)[0]->GetValue(prg));
	Owner->GOFile->Open(RIO::Create);
	Owner->GOFile->SetSeparator("\t");
}


//------------------------------------------------------------------------------
void GSOutputI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=1)
		throw RException("A filename must be specified.");
	o->WriteStr("Create Statistics Output file: "+(*args)[0]->GetValue(prg));
	if(Owner->SOFile)
	{
		delete Owner->SOFile;
		Owner->SOFile=0;
	}
	Owner->SOFile=new RTextFile((*args)[0]->GetValue(prg));
	Owner->SOFile->Open(RIO::Create);
	Owner->SOFile->SetSeparator("\t");
}


//------------------------------------------------------------------------------
void GSetLinksMethodI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=1)
		throw RException("The method needs one parameter to specify the name of the link computing method (or \"None\").");
	o->WriteStr("Link Computing Method: "+(*args)[0]->GetValue(prg));
	GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc")->SetCurrentMethod((*args)[0]->GetValue(prg),false);
}


//------------------------------------------------------------------------------
void GTestI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=1)
		throw RException("The method needs the name of the test.");
	if(args->GetNb())
	o->WriteStr("Current Test Name: "+(*args)[0]->GetValue(prg));
	Owner->TestName=(*args)[0]->GetValue(prg);
}



//------------------------------------------------------------------------------
void GLogI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=1)
		throw RException("The method needs the name of the log file.");
	o->WriteStr("Create Log file: "+(*args)[0]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GSqlI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=1)
		throw RException("The method needs the name of the SQL file.");
	o->WriteStr("Execute Sql file: "+(*args)[0]->GetValue(prg));
	Owner->Session->GetStorage()->ExecuteData((*args)[0]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GComputeProfilesI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	RString tmp;

	if(args->GetNb()>1)
		throw RException("The method needs maximum one parameter.");
	if(args->GetNb()==1)
		tmp="Compute Profiles with method: "+(*args)[0]->GetValue(prg);
	else
		tmp="Compute Profiles with current method";
	o->WriteStr(tmp);
	if(args->GetNb()==1)
		GALILEIApp->GetManager<GProfileCalcManager>("ProfileCalc")->SetCurrentMethod((*args)[0]->GetValue(prg),false);
	if(GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod(false))
		GALILEIApp->GetManager<GLinkCalcManager>("LinkCalc")->GetCurrentMethod(false)->ApplyConfig();
	if(!GALILEIApp->GetManager<GProfileCalcManager>("ProfileCalc")->GetCurrentMethod())
		throw GException (" No Profiling Method chosen.");
	GALILEIApp->GetManager<GProfileCalcManager>("ProfileCalc")->GetCurrentMethod()->ApplyConfig();
	Owner->Session->CalcProfiles(dynamic_cast<GSlot*>(o));
}


//------------------------------------------------------------------------------
void GGroupProfilesI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	RString tmp;
	if(args->GetNb()>1)
		throw RException("The method needs maximum one parameter.");
	if(args->GetNb()==1)
		tmp="Group Profiles with method:"+(*args)[0]->GetValue(prg);
	else
		tmp="Group Profiles with current method";
	o->WriteStr(tmp);
	if(args->GetNb()==1)
		GALILEIApp->GetManager<GGroupingManager>("Grouping")->SetCurrentMethod((*args)[0]->GetValue(prg));
	if(!GALILEIApp->GetManager<GGroupingManager>("Grouping")->GetCurrentMethod())
		throw RException (" No Grouping Method chosen.");
	GALILEIApp->GetManager<GGroupingManager>("Grouping")->GetCurrentMethod()->ApplyConfig();
	if(!GALILEIApp->GetManager<GGroupCalcManager>("GroupCalc")->GetCurrentMethod())
		throw RException (" No Group Description Method chosen.");
	GALILEIApp->GetManager<GGroupCalcManager>("GroupCalc")->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GroupingProfiles(dynamic_cast<GSlot*>(o));
}


//------------------------------------------------------------------------------
void GCreateIdealI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Create Ideal Groups");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->CreateIdeal(Owner->Session->MustSaveResults());
}


//------------------------------------------------------------------------------
void GFdbksCycleI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Create Feedbacks Cycle");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->DocumentSharing();
}


//------------------------------------------------------------------------------
void GCompareIdealI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Compare with Ideal Groups");
	Owner->Session->GetSubjects()->Compare();
	Owner->Precision=Owner->Session->GetSubjects()->GetPrecision();
	Owner->Recall=Owner->Session->GetSubjects()->GetRecall();
	Owner->Total=Owner->Session->GetSubjects()->GetTotal();
	if(Owner->TrackNewProfiles)
	{
		Owner->PercAss=Owner->Session->GetSubjects()->ComputePercAss();
		sprintf(tmp,"Recall: %f  -  Precision: %f  -  Total: %f  -  New: %f",Owner->Recall,Owner->Precision,Owner->Total,Owner->PercAss);
	}
	else
		sprintf(tmp,"Recall: %f  -  Precision: %f  -  Total: %f",Owner->Recall,Owner->Precision,Owner->Total);
	o->WriteStr(tmp);
	if(Owner->OFile)
	{
		(*Owner->OFile)<<Owner->TestName<<Owner->Recall<<Owner->Precision<<Owner->Total;
		if(Owner->TrackNewProfiles)
			(*Owner->OFile)<<Owner->PercAss;
		(*Owner->OFile)<<endl;
	}
	if(Owner->GOFile)
	{
		(*Owner->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->Total;
		if(Owner->TrackNewProfiles)
			(*Owner->GOFile)<<Owner->PercAss;
		(*Owner->GOFile)<<endl;
	}
}


//------------------------------------------------------------------------------
void GSetSubjectsParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=2)
		throw RException("Method needs two parameters.");
	GALILEIApp->GetGALILEIConfig()->Set((*args)[0]->GetValue(prg),(*args)[1]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GSetComputingParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args)
{
	GProfileCalc* calc;

	if(args->GetNb()!=2)
		throw RException("Method needs two parameters");
	calc=GALILEIApp->GetManager<GProfileCalcManager>("ProfileCalc")->GetCurrentMethod();
	if(!calc)
		throw RException("No profiling method selected.");
	calc->GetFactory()->Set((*args)[0]->GetValue(prg),(*args)[1]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GSetGroupingParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args)
{
	GGrouping* calc;

	if(args->GetNb()!=2)
		throw RException("Method needs two parameters.");
	calc=GALILEIApp->GetManager<GGroupingManager>("Grouping")->GetCurrentMethod();
	if(!calc)
		throw RException("No grouping computing method selected.");
	calc->GetFactory()->Set((*args)[0]->GetValue(prg),(*args)[1]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GAddIdealI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Create New Ideal Group");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->AddTopic();
}


//------------------------------------------------------------------------------
void GAddProfilesI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Adding Profiles");
	Owner->Session->GetSubjects()->Apply();
	sprintf(tmp, "%u new profiles created",Owner->Session->GetSubjects()->AddProfiles());
	o->WriteStr(tmp);
}


//------------------------------------------------------------------------------
void GRealLifeI::CommonTasks(RPrgOutput* o)
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
	GALILEIApp->GetManager<GGroupingManager>("Grouping")->GetCurrentMethod()->ApplyConfig();
	GALILEIApp->GetManager<GGroupCalcManager>("GroupCalc")->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GroupingProfiles(rec);

	// Compare Ideal
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Compare with Ideal Groups");
	}
	Owner->Session->GetSubjects()->Compare();
	Owner->Precision=Owner->Session->GetSubjects()->GetPrecision();
	Owner->Recall=Owner->Session->GetSubjects()->GetRecall();
	Owner->Total=Owner->Session->GetSubjects()->GetTotal();
	if(rec)
	{
		rec->Interact();
		sprintf(tmp,"Recall: %f  -  Precision: %f  -  Total: %f",Owner->Recall,Owner->Precision,Owner->Total);
		rec->WriteStr(tmp);
	}
	if(GSession::Break()) return;
	if(Owner->OFile)
		(*Owner->OFile)<<Owner->TestName<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
	if(Owner->GOFile)
		(*Owner->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
}


//------------------------------------------------------------------------------
void GRealLifeI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	unsigned int MaxStep;
	unsigned int MinFBStep;
	unsigned int MaxFBStep;
	unsigned int NbStep;
	double Proba;
	RRandom* Random;
	unsigned int nb;
	unsigned int NewProf;
	unsigned int nbminprof,nbmaxprof;
	GSlot* rec=dynamic_cast<GSlot*>(o);

	if(args->GetNb()!=4)
		throw RException("Method needs four parameters.");
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Real Life: "+(*args)[0]->GetValue(prg)+","+(*args)[1]->GetValue(prg)+","+(*args)[2]->GetValue(prg)+","+(*args)[3]->GetValue(prg));
	}
	if(GSession::Break()) return;
	Random=Owner->Session->GetRandom();
	MaxStep=atoi((*args)[0]->GetValue(prg));
	MinFBStep=atoi((*args)[1]->GetValue(prg));
	MaxFBStep=atoi((*args)[2]->GetValue(prg))-MinFBStep+1;
	Proba=atof((*args)[3]->GetValue(prg));
	What[1]=0;
	for(NbStep=0;;)
	{
		// Feedback process
		for(nb=Random->Value(MaxFBStep)+MinFBStep+1;--nb;)
		{
			// Set TestName
			NbStep++;
			Owner->TestName=RString::Number(NbStep);

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
			CommonTasks(o);

			// Verify Nb Steps
			if(NbStep>MaxStep) break;
		}

		// Verify Nb Steps
		if(NbStep>MaxStep) break;
		NbStep++;

		// Set TestName
		Owner->TestName=RString::Number(NbStep);

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
		if(Random->Value()<Proba)
		{
			// Create One profile of an existing topic
			NewProf=Owner->Session->GetSubjects()->AddProfiles();
			GALILEIApp->GetGALILEIConfig()->Set("NbProfMin",nbminprof);
			GALILEIApp->GetGALILEIConfig()->Set("NbProfMax",nbmaxprof);

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
				GALILEIApp->GetGALILEIConfig()->Set("NbProfMin",nbminprof);
				GALILEIApp->GetGALILEIConfig()->Set("NbProfMax",nbmaxprof);

				if(NewProf)
					What[0]='E';
			}
		}
		GALILEIApp->GetGALILEIConfig()->Set("NbProfMin",nbminprof);
		GALILEIApp->GetGALILEIConfig()->Set("NbProfMax",nbmaxprof);
		GALILEIApp->Apply();

		// If no profile was created -> normal feedback cycle
		if(!NewProf)
		{
			What[0]='F';
			Owner->Session->GetSubjects()->Apply();
			Owner->Session->GetSubjects()->DocumentSharing();
		}

		// Compute, Group and Compare
		CommonTasks(o);

		// Verify Nb Steps
		if(NbStep>MaxStep) break;
	}
}


//------------------------------------------------------------------------------
void GAddAssessmentsI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Adding Assessments");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->AddAssessments();
}


//------------------------------------------------------------------------------
void GTrackNewProfilesI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=1)
		throw RException("The method needs one parameter (\"0\" or \"1\") to specify if the profiles must be tracked.");
	if(args->GetNb())
	{
		if((((*args)[0]->GetValue(prg))=="0")||((*args)[0]->GetValue(prg)=="true"))
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
void GClearNewProfilesI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb())
		throw RException("Method needs no parameter.");
	o->WriteStr("Clear New Profiles");
	Owner->Session->GetSubjects()->ClearLastAdded();
}


//------------------------------------------------------------------------------
void GResetTimeI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb())
		throw RException("Method needs no parameter.");
	o->WriteStr("Reset Time");
	Owner->ClockRef=time(0);
}


//------------------------------------------------------------------------------
void GComputeTimeI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args)
{
	time_t end;
	double cpu_time;

	end=time(0);
	if(args->GetNb())
		throw RException("Method needs no parameter.");
	cpu_time=difftime(end,Owner->ClockRef);
	sprintf(tmp,"Ellapsed Time %f",cpu_time);
	o->WriteStr(tmp);
}


//------------------------------------------------------------------------------
void GSetStatParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args)
{
	GStatsCalc* calc;
	GStatsCalcManager* mng;

	if(args->GetNb()!=3)
		throw RException("Method needs three parameters");
	mng=GALILEIApp->GetManager<GStatsCalcManager>("StatsCalc");
	if(!mng)
		throw RException("No statistics method selected.");
	calc=mng->GetPlugIn((*args)[0]->GetValue(prg));
	if(!calc)
		throw RException("No statistics method selected.");
	calc->GetFactory()->Set((*args)[1]->GetValue(prg),(*args)[2]->GetValue(prg));
	calc->ApplyConfig();
}


//------------------------------------------------------------------------------
void GRunStatI::Run(R::RPrg*,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args)
{
	GStatsCalc* Calc;
	RXMLStruct xml;
	RXMLTag* Root;
	int i;

	if(args->GetNb())
		throw RException("Method needs no parameter");

	GStatsCalcManager* Mng=GALILEIApp->GetManager<GStatsCalcManager>("StatsCalc");
	if(!Mng)
		throw RException("No manager for the statistics plug-ins");

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
}


//------------------------------------------------------------------------------
void GForceReComputeI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args)
{
	tObjType type=otNoClass;

	if(args->GetNb()!=1)
		throw RException("Method needs one parameter");
	RString objects=(*args)[0]->GetValue(prg);
	if(objects=="Users")
		type=otUsers;
	if(objects=="Docs")
		type=otDocs;
	if(objects=="Groups")
		type=otGroups;
	Owner->Session->ForceReCompute(type);
}


//------------------------------------------------------------------------------
void GSetSaveResultsI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=1)
		throw RException("Method needs one parameter");
	if(((*args)[0]->GetValue(prg))=="0")
		Owner->Session->SetSaveResults(false);
	else
		Owner->Session->SetSaveResults(true);
}


//------------------------------------------------------------------------------
// (c) 2006, Michaël Noiret.
void GSetMeasuresParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args)
{
	if(args->GetNb()!=4)
		throw RException("Method needs four parameters.");

	GMeasureManager* manager=GALILEIApp->GetManager<GMeasureManager>("Measures");
	if(!manager)
		throw RException("No mesaures manager.");
	GMeasure* measure=manager->GetCurrentMethod((*args)[0]->GetValue(prg));
	if(!measure)
		throw RException("No method selected for this type of measure.");
	manager->GetFactory((*args)[0]->GetValue(prg),(*args)[1]->GetValue(prg))->Set((*args)[2]->GetValue(prg),(*args)[3]->GetValue(prg));
	measure->ApplyConfig();
}



//------------------------------------------------------------------------------
//
// class GPrgClassSession
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPrgClassSession::GPrgClassSession(GSession* s)
	: RPrgClass("Session"), Session(s), OFile(0),
	  GOFile(0), SOFile(0),DSOFile(0),  TrackNewProfiles(false)
{
	Methods.InsertPtr(new GOutputI(this));
	Methods.InsertPtr(new GGOutputI(this));
	Methods.InsertPtr(new GSOutputI(this));
	Methods.InsertPtr(new GSetLinksMethodI(this));
	Methods.InsertPtr(new GTestI(this));
	Methods.InsertPtr(new GLogI(this));
	Methods.InsertPtr(new GSqlI(this));
	Methods.InsertPtr(new GComputeProfilesI(this));
	Methods.InsertPtr(new GGroupProfilesI(this));
	Methods.InsertPtr(new GCreateIdealI(this));
	Methods.InsertPtr(new GFdbksCycleI(this));
	Methods.InsertPtr(new GCompareIdealI(this));
	Methods.InsertPtr(new GSetSubjectsParamI(this));
	Methods.InsertPtr(new GSetComputingParamI(this));
	Methods.InsertPtr(new GSetGroupingParamI(this));
	Methods.InsertPtr(new GAddIdealI(this));
	Methods.InsertPtr(new GAddProfilesI(this));
	Methods.InsertPtr(new GRealLifeI(this));
	Methods.InsertPtr(new GAddAssessmentsI(this));
	Methods.InsertPtr(new GTrackNewProfilesI(this));
	Methods.InsertPtr(new GClearNewProfilesI(this));
	Methods.InsertPtr(new GResetTimeI(this));
	Methods.InsertPtr(new GComputeTimeI(this));
	Methods.InsertPtr(new GSetRandI(this));
	Methods.InsertPtr(new GSetStatParamI(this));
	Methods.InsertPtr(new GRunStatI(this));
	Methods.InsertPtr(new GForceReComputeI(this));
	Methods.InsertPtr(new GSetSaveResultsI(this));
	Methods.InsertPtr(new GSetMeasuresParamI(this));
}


//------------------------------------------------------------------------------
GPrgClassSession::~GPrgClassSession(void)
{
	if(OFile)
		delete OFile;
	if(GOFile)
	{
		delete GOFile;
		GOFile=0;
	}
	if(SOFile)
	{
		delete SOFile;
		SOFile=0;
	}
	if(DSOFile)
	{
		delete DSOFile;
		DSOFile=0;
	}
}



//------------------------------------------------------------------------------
//
// GSessionPrg
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSessionPrg::GSessionPrg(RString f,GSession* s,RPrgOutput* o)
	: RPrg(f,o)
{
	Classes.InsertPtr(new GPrgClassSession(s));
}


//------------------------------------------------------------------------------
GSessionPrg::~GSessionPrg(void)
{
}
