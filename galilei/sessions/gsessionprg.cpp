/*

	GALILEI Research Project

	GSessionPrg.cpp

	Program running a Session object  - Implementation.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

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
#include <rprg/rprgvar.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsessionprg.h>
#include <infos/glang.h>
#include <groups/ggrouping.h>
#include <groups/ggroupcalc.h>
#include <groups/gsubjects.h>
#include <profiles/gprofilecalc.h>
#include <docs/glinkcalc.h>
#include <sessions/gstatscalc.h>
#include <sessions/gsession.h>
#include <sessions/gstorage.h>
#include <sessions/gslot.h>
#include <sessions/gmixidealgroups.h>
#include <sessions/gpluginmanagers.h>
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
	GSM(const char* name,GPrgClassSession* o)
		: RPrgFunc(name), Owner(o) {}
};


//------------------------------------------------------------------------------
class GSetRandI : public GSM
{
public:
	GSetRandI(GPrgClassSession* o) : GSM("SetRand",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};

//------------------------------------------------------------------------------
class GOutputI : public GSM
{
public:
	GOutputI(GPrgClassSession* o) : GSM("SetOutput",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GGOutputI : public GSM
{
public:
	GGOutputI(GPrgClassSession* o) : GSM("SetGraphOutput",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GSOutputI : public GSM
{
public:
	GSOutputI(GPrgClassSession* o) : GSM("SetStatsOutput",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GSetLinksMethodI : public GSM
{
public:
	GSetLinksMethodI(GPrgClassSession* o) : GSM("SetLinksMethod",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GSetAutoSaveI : public GSM
{
public:
	GSetAutoSaveI(GPrgClassSession* o) : GSM("SetAutoSave",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};

//------------------------------------------------------------------------------
class GSetAutoSaveLinksI : public GSM
{
public:
	GSetAutoSaveLinksI(GPrgClassSession* o) : GSM("SetAutoSaveLinks",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GTestI : public GSM
{
public:
	GTestI(GPrgClassSession* o) : GSM("Test",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GLogI : public GSM
{
public:
	GLogI(GPrgClassSession* o) : GSM("Log",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GSqlI : public GSM
{
public:
	GSqlI(GPrgClassSession* o) : GSM("ExecSql",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GModifyProfilesI : public GSM
{
public:
	GModifyProfilesI(GPrgClassSession* o) : GSM("ModifyProfiles",o) {}
	virtual void Run(R::RPrg*,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GComputeProfilesI : public GSM
{
public:
	GComputeProfilesI(GPrgClassSession* o) : GSM("ComputeProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GGroupProfilesI : public GSM
{
public:
	GGroupProfilesI(GPrgClassSession* o) : GSM("GroupProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};

//------------------------------------------------------------------------------
class GCreateIdealI : public GSM
{
public:
	GCreateIdealI(GPrgClassSession* o) : GSM("CreateIdeal",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GMixIdealI : public GSM
{
public:
	GMixIdealI(GPrgClassSession* o) : GSM("MixIdeal",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GFdbksCycleI : public GSM
{
public:
	GFdbksCycleI(GPrgClassSession* o) : GSM("FdbksCycle",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GCompareIdealI : public GSM
{
public:
	GCompareIdealI(GPrgClassSession* o) : GSM("CompareIdeal",o) {}
	virtual void Run(R::RPrg*,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GSetSubjectsParamI : public GSM
{
public:
	GSetSubjectsParamI(GPrgClassSession* o) : GSM("SetSubjectsParam",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GSetComputingParamI : public GSM
{
public:
	GSetComputingParamI(GPrgClassSession* o) : GSM("SetComputingParam",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GSetGroupingParamI : public GSM
{
public:
	GSetGroupingParamI(GPrgClassSession* o) : GSM("SetGroupingParam",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GAddIdealI : public GSM
{
public:
	GAddIdealI(GPrgClassSession* o) : GSM("AddIdeal",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GAddProfilesI : public GSM
{
public:
	GAddProfilesI(GPrgClassSession* o) : GSM("AddProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GRealLifeI : public GSM
{
	char What[2];
	void CommonTasks(R::RPrgOutput* o) throw(RException);
public:
	GRealLifeI(GPrgClassSession* o) : GSM("RealLife",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GAddAssessmentsI : public GSM
{
public:
	GAddAssessmentsI(GPrgClassSession* o) : GSM("AddAssessments",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GTrackNewProfilesI : public GSM
{
public:
	GTrackNewProfilesI(GPrgClassSession* o) : GSM("TrackNewProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GClearNewProfilesI : public GSM
{
public:
	GClearNewProfilesI(GPrgClassSession* o) : GSM("ClearNewProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GResetTimeI : public GSM
{
public:
	GResetTimeI(GPrgClassSession* o) : GSM("ResetTime",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GComputeTimeI : public GSM
{
public:
	GComputeTimeI(GPrgClassSession* o) : GSM("ComputeTime",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GSetStatParamI: public GSM
{
public:
	GSetStatParamI(GPrgClassSession* o) : GSM("SetStatParam",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};


//------------------------------------------------------------------------------
class GRunStatI: public GSM
{
public:
	GRunStatI(GPrgClassSession* o) : GSM("RunStat",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,true,false>* args) throw(RException);
};



//------------------------------------------------------------------------------
//
// Instructions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GSetRandI::Run(RPrg* prg,RPrgOutput* o,RContainer<RPrgVar,true,false>* args) throw(RException)
{
	int Rand;

	if(args->GetNb()!=1)
		throw RException("The rand value must be specified.");
	sprintf(tmp,"Set Rand value '%s'",(*args)[0]->GetValue(prg));
	o->WriteStr(tmp);
	Rand=atoi((*args)[0]->GetValue(prg));
	if(Rand!=0)
	{
		Owner->Session->SetCurrentRandom(Rand);
	}

}


//------------------------------------------------------------------------------
void GOutputI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("A filename must be specified.");
	sprintf(tmp,"Create Output file '%s'",(*args)[0]->GetValue(prg));
	o->WriteStr(tmp);
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
void GGOutputI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("A filename must be specified.");
	sprintf(tmp,"Create Graph Output file '%s'",(*args)[0]->GetValue(prg));
	o->WriteStr(tmp);
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
void GSOutputI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("A filename must be specified.");
	sprintf(tmp,"Create Statistics Output file '%s'",(*args)[0]->GetValue(prg));
	o->WriteStr(tmp);
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
void GSetLinksMethodI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("The method needs one parameter to specify the name of the link computing method (or \"None\").");
	sprintf(tmp,"Link Computing Method: %s",(*args)[0]->GetValue(prg));
	o->WriteStr(tmp);
	(dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->SetCurrentMethod((*args)[0]->GetValue(prg));
}



//------------------------------------------------------------------------------
void GSetAutoSaveI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("The method needs one parameter (\"0\" or \"1\") to specify if the results must be stored.");
	if(((*args)[0]->GetValue(prg))[0]=='0')
	{
		o->WriteStr("Set AutoSave: false");
		Owner->AutoSave=false;
	}
	else
	{
		o->WriteStr("Set AutoSave: true");
		Owner->AutoSave=true;
	}
}

//------------------------------------------------------------------------------
void GSetAutoSaveLinksI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("The method needs one parameter (\"0\" or \"1\") to specify if the links results must be stored.");
	if(((*args)[0]->GetValue(prg))[0]=='0')
	{
		o->WriteStr("Set AutoSaveLinks: false");
		Owner->AutoSaveLinks=false;
	}
	else
	{
		o->WriteStr("Set AutoSaveLinks: true");
		Owner->AutoSaveLinks=true;
	}
}


//------------------------------------------------------------------------------
void GTestI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("The method needs the name of the test.");
	if(args->GetNb())
	sprintf(tmp,"Current Test Name '%s'",(*args)[0]->GetValue(prg));
	o->WriteStr(tmp);
	Owner->TestName=(*args)[0]->GetValue(prg);
}



//------------------------------------------------------------------------------
void GLogI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("The method needs the name of the log file.");
	sprintf(tmp,"Create Log file '%s'",(*args)[0]->GetValue(prg));
	o->WriteStr(tmp);
}


//------------------------------------------------------------------------------
void GSqlI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("The method needs the name of the SQL file.");
	sprintf(tmp,"Execute Sql file '%s'",(*args)[0]->GetValue(prg));
	o->WriteStr(tmp);
	Owner->Session->GetStorage()->ExecuteData((*args)[0]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GModifyProfilesI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb())
		throw RException("The method needs no parameters.");
	Owner->FirstProfile=false;
	o->WriteStr("Profiles are considered as modified");
}


//------------------------------------------------------------------------------
void GComputeProfilesI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()>1)
		throw RException("The method needs maximum one parameter.");
	if(args->GetNb()==1)
		sprintf(tmp,"Compute Profiles: Method=\"%s\"",(*args)[0]->GetValue(prg));
	else
		strcpy(tmp,"Compute Profiles: Current Method");
	o->WriteStr(tmp);
	if(args->GetNb()==1)
		(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->SetCurrentMethod((*args)[0]->GetValue(prg));
	if((dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod())
		(dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod()->ApplyConfig();
	if(!(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod())
		throw GException (" No Profiling Method chosen.");
	(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod()->ApplyConfig();
	Owner->Session->CalcProfiles(dynamic_cast<GSlot*>(o),Owner->FirstProfile,Owner->AutoSave,Owner->AutoSaveLinks);
	if(!Owner->FirstProfile) Owner->FirstProfile=true;
}


//------------------------------------------------------------------------------
void GGroupProfilesI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()>1)
		throw RException("The method needs maximum one parameter.");
	if(args->GetNb()==1)
		sprintf(tmp,"Group Profiles: Method=\"%s\" ",(*args)[0]->GetValue(prg));
	else
		strcpy(tmp,"Group Profiles: Current Method");
	o->WriteStr(tmp);
	if(args->GetNb()==1)
		(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->SetCurrentMethod((*args)[0]->GetValue(prg));
	if(Owner->Session->GetSubjects()->GetIdealGroups())
	{
		GGrouping* algo=(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->GetCurrentMethod();
		algo->SetIdealGroups(Owner->Session->GetSubjects()->GetIdealGroups());
	}
	if(!(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->GetCurrentMethod())
		throw RException (" No Grouping Method chosen.");
	(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->GetCurrentMethod()->ApplyConfig();
	if(!(dynamic_cast<GGroupCalcManager*>(GPluginManagers::PluginManagers.GetManager("GroupCalc")))->GetCurrentMethod())
		throw RException (" No Group Description Method chosen.");
	(dynamic_cast<GGroupCalcManager*>(GPluginManagers::PluginManagers.GetManager("GroupCalc")))->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GroupingProfiles(dynamic_cast<GSlot*>(o),Owner->FirstGroup,Owner->AutoSave);
	if(!Owner->FirstGroup) Owner->FirstGroup=true;
}


//------------------------------------------------------------------------------
void GCreateIdealI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Create Ideal Groups");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->CreateIdeal(Owner->AutoSave);
	Owner->FirstGroup=Owner->FirstProfile=false;
}


//------------------------------------------------------------------------------
void GMixIdealI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()>1)
		throw RException("The method needs maximum one parameter.");
	if(args->GetNb()==1)
		sprintf(tmp,"Creating Mixed Groups: Settings=\"%s\"",(*args)[0]->GetValue(prg));
	else
		sprintf(tmp,"Creating Mixed Groups");
	o->WriteStr(tmp);
	GMixIdealGroups mix(Owner->Session,Owner->Session->GetSubjects()->GetIdealGroups());
	if(args->GetNb()==1)
		mix.SetSettings((*args)[0]->GetValue(prg));
	mix.Run(0);
}


//------------------------------------------------------------------------------
void GFdbksCycleI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Create Feedbacks Cycle");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->FdbksCycle(Owner->AutoSave);
}


//------------------------------------------------------------------------------
void GCompareIdealI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
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
void GSetSubjectsParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=2)
		throw RException("Method needs two parameters.");
	Owner->Session->GetSubjects()->Set((*args)[0]->GetValue(prg),(*args)[1]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GSetComputingParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	GProfileCalc* calc;

	if(args->GetNb()!=2)
		throw RException("Method needs two parameters");
	calc=(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod();
	if(!calc)
		throw RException("No profiling method selected.");
	calc->GetFactory()->Set((*args)[0]->GetValue(prg),(*args)[1]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GSetGroupingParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	GGrouping* calc;

	if(args->GetNb()!=2)
		throw RException("Method needs two parameters.");
	calc=(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->GetCurrentMethod();
	if(!calc)
		throw RException("No grouping computing method selected.");
	calc->GetFactory()->Set((*args)[0]->GetValue(prg),(*args)[1]->GetValue(prg));
}


//------------------------------------------------------------------------------
void GAddIdealI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Create New Ideal Group");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->AddTopic(Owner->AutoSave);
}


//------------------------------------------------------------------------------
void GAddProfilesI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Adding Profiles");
	Owner->Session->GetSubjects()->Apply();
	sprintf(tmp, "%u new profiles created",Owner->Session->GetSubjects()->AddProfiles(Owner->AutoSave));
	o->WriteStr(tmp);
}


//------------------------------------------------------------------------------
void GRealLifeI::CommonTasks(RPrgOutput* o) throw(RException)
{
	GSlot* rec=dynamic_cast<GSlot*>(o);

	// Compute Profiles
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Compute Profiles: Current Method");
	}
	if(GSession::Break()) return;
	if((dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod())
		(dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod()->ApplyConfig();
	(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod()->ApplyConfig();
	Owner->Session->CalcProfiles(rec,Owner->FirstProfile,Owner->AutoSave,Owner->AutoSaveLinks);
	if(!Owner->FirstProfile) Owner->FirstProfile=true;

	// Group Profiles
	if(rec)
	{
		rec->Interact();
		rec->WriteStr("Group Profiles: Current Method");
	}
	if(GSession::Break()) return;
	if(Owner->Session->GetSubjects()->GetIdealGroups())
	{
		GGrouping* algo=(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->GetCurrentMethod();
		algo->SetIdealGroups(Owner->Session->GetSubjects()->GetIdealGroups());
	}
	(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->GetCurrentMethod()->ApplyConfig();
	(dynamic_cast<GGroupCalcManager*>(GPluginManagers::PluginManagers.GetManager("GroupCalc")))->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GroupingProfiles(rec,Owner->FirstGroup,Owner->AutoSave);
	if(!Owner->FirstGroup) Owner->FirstGroup=true;

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
void GRealLifeI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
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

	if(args->GetNb()!=5)
		throw RException("Method needs five parameters.");
	sprintf(tmp,"Real Life: Settings=\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"",
	        (*args)[0]->GetValue(prg),(*args)[1]->GetValue(prg),(*args)[2]->GetValue(prg),
	        (*args)[3]->GetValue(prg),(*args)[4]->GetValue(prg));
	if(rec)
	{
		rec->Interact();
		rec->WriteStr(tmp);
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
			Owner->TestName=itou(NbStep);

			// Create Feedbacks
			if(rec)
			{
				rec->Interact();
				rec->WriteStr("Create Feedbacks Cycle");
			}
			if(GSession::Break()) return;
			What[0]='F';
			Owner->Session->GetSubjects()->Apply();
			Owner->Session->GetSubjects()->FdbksCycle(Owner->AutoSave);
			CommonTasks(o);

			// Verify Nb Steps
			if(NbStep>MaxStep) break;
		}

		// Verify Nb Steps
		if(NbStep>MaxStep) break;
		NbStep++;

		// Set TestName
		Owner->TestName=itou(NbStep);

		// Create 1 new profile
		if(rec)
		{
			rec->Interact();
			rec->WriteStr("Create 1 new profile");
		}
		if(GSession::Break()) return;
		if(Random->Value()<Proba)
		{
			// Create One profile of an existing topic
			nbminprof=Owner->Session->GetSubjects()->GetUInt("NbProfMin");
			nbmaxprof=Owner->Session->GetSubjects()->GetUInt("NbProfMax");
			Owner->Session->GetSubjects()->Set("NbProfMin",1);
			Owner->Session->GetSubjects()->Set("NbProfMax",1);
			Owner->Session->GetSubjects()->Apply();
			NewProf=Owner->Session->GetSubjects()->AddProfiles(Owner->AutoSave);
			Owner->Session->GetSubjects()->Set("NbProfMin",nbminprof);
			Owner->Session->GetSubjects()->Set("NbProfMax",nbmaxprof);

			if(NewProf)
				What[0]='E';
			else
			{
				What[0]='F';
				Owner->Session->GetSubjects()->Apply();
				Owner->Session->GetSubjects()->FdbksCycle(Owner->AutoSave);
			}
		}
		else
		{
			// Create one profile of a new topic
			Owner->Session->GetSubjects()->Apply();
			if(Owner->Session->GetSubjects()->AddTopic(Owner->AutoSave))
			{
				What[0]='N';
			}
			else
			{
				Proba=1.0;
				nbminprof=Owner->Session->GetSubjects()->GetUInt("NbProfMin");
				nbmaxprof=Owner->Session->GetSubjects()->GetUInt("NbProfMax");
				Owner->Session->GetSubjects()->Set("NbProfMin",1);
				Owner->Session->GetSubjects()->Set("NbProfMax",1);
				Owner->Session->GetSubjects()->Apply();
				NewProf=Owner->Session->GetSubjects()->AddProfiles(Owner->AutoSave);
				Owner->Session->GetSubjects()->Set("NbProfMin",nbminprof);
				Owner->Session->GetSubjects()->Set("NbProfMax",nbmaxprof);

				if(NewProf)
					What[0]='E';
				else
				{
					What[0]='F';
					Owner->Session->GetSubjects()->Apply();
					Owner->Session->GetSubjects()->FdbksCycle(Owner->AutoSave);
				}
			}
		}
		Owner->Session->GetSubjects()->Apply();
		CommonTasks(o);

		// Verify Nb Steps
		if(NbStep>MaxStep) break;
	}
}


//------------------------------------------------------------------------------
void GAddAssessmentsI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb())
		throw RException("Method needs no parameters.");
	o->WriteStr("Adding Assessments");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->AddAssessments(Owner->AutoSave);
}


//------------------------------------------------------------------------------
void GTrackNewProfilesI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb()!=1)
		throw RException("The method needs one parameter (\"0\" or \"1\") to specify if the profiles must be tracked.");
	if(args->GetNb())
	{
		if((((*args)[0]->GetValue(prg))[0]=='0')||((*args)[0]->GetValue(prg)=="true"))
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
void GClearNewProfilesI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb())
		throw RException("Method needs no parameter.");
	o->WriteStr("Clear New Profiles");
	Owner->Session->GetSubjects()->ClearLastAdded();
}


//------------------------------------------------------------------------------
void GResetTimeI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	if(args->GetNb())
		throw RException("Method needs no parameter.");
	o->WriteStr("Reset Time");
	Owner->ClockRef=time(0);
}


//------------------------------------------------------------------------------
void GComputeTimeI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,true,false>* args) throw(RException)
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
void GSetStatParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	GStatsCalc* calc;
	GStatsCalcManager* mng;

	if(args->GetNb()!=3)
		throw RException("Method needs three parameters");
	mng=dynamic_cast<GStatsCalcManager*>(GPluginManagers::PluginManagers.GetManager("StatsCalc"));
	if(!mng)
		throw RException("No statistics method selected.");
	calc=mng->GetPlugIn((*args)[0]->GetValue(prg));
	if(!calc)
		throw RException("No statistics method selected.");
	calc->GetFactory()->Set((*args)[1]->GetValue(prg),(*args)[2]->GetValue(prg));
	calc->ApplyConfig();
}


//------------------------------------------------------------------------------
void GRunStatI::Run(R::RPrg*,RPrgOutput*,R::RContainer<RPrgVar,true,false>* args) throw(RException)
{
	GStatsCalc* Calc;
	RXMLStruct xml;
	RXMLTag* Root;
	int i;

	if(args->GetNb())
		throw RException("Method needs no parameter");

	GStatsCalcManager* Mng=(dynamic_cast<GStatsCalcManager*>(GPluginManagers::PluginManagers.GetManager("StatsCalc")));
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
//
// class GPrgClassSession
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPrgClassSession::GPrgClassSession(GSession* s) throw(std::bad_alloc)
	: RPrgClass("Session"), Session(s), OFile(0),
	  GOFile(0), SOFile(0),DSOFile(0),  AutoSave(false),AutoSaveLinks(false), TrackNewProfiles(false)
{
	Methods.InsertPtr(new GOutputI(this));
	Methods.InsertPtr(new GGOutputI(this));
	Methods.InsertPtr(new GSOutputI(this));
	Methods.InsertPtr(new GSetLinksMethodI(this));
	Methods.InsertPtr(new GSetAutoSaveLinksI(this));
	Methods.InsertPtr(new GSetAutoSaveI(this));
	Methods.InsertPtr(new GTestI(this));
	Methods.InsertPtr(new GLogI(this));
	Methods.InsertPtr(new GSqlI(this));
	Methods.InsertPtr(new GModifyProfilesI(this));
	Methods.InsertPtr(new GComputeProfilesI(this));
	Methods.InsertPtr(new GGroupProfilesI(this));
	Methods.InsertPtr(new GCreateIdealI(this));
	Methods.InsertPtr(new GMixIdealI(this));
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
GSessionPrg::GSessionPrg(RString f,GSession* s,RPrgOutput* o) throw(std::bad_alloc,GException)
	: RPrg(f,o)
{
	Classes.InsertPtr(new GPrgClassSession(s));
	Load();
}


//------------------------------------------------------------------------------
GSessionPrg::~GSessionPrg(void)
{
}
