               /*

	GALILEI Research Project

	GSessionPrg.cpp

	Program to run on a Session - Implementation.

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
// include files for R Project
/*#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>*/
#include <rprg/rprgvar.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsessionprg.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <groups/ggrouping.h>
#include <groups/ggroupingmanager.h>
#include <groups/ggroupcalcmanager.h>
#include <groups/ggroupcalc.h>
#include <groups/gsubjecttree.h>
#include <profiles/gprofilecalc.h>
#include <profiles/gprofilecalcmanager.h>
#include <docs/glinkcalc.h>
#include <docs/glinkcalcmanager.h>
#include <sessions/gsession.h>
#include <sessions/gslot.h>
#include <sessions/gmixidealgroups.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// Declare all the instructions of the class 'Session'.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GSM : public R::RPrgFunc
{
public:
	char tmp[3000];
	GPrgClassSession* Owner;
	GSM(const char* name,GPrgClassSession* o)
		: RPrgFunc(name), Owner(o) {}
};


//-----------------------------------------------------------------------------
class GSetRandI : public GSM
{
public:
	GSetRandI(GPrgClassSession* o) : GSM("SetRand",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};

//-----------------------------------------------------------------------------
class GOutputI : public GSM
{
public:
	GOutputI(GPrgClassSession* o) : GSM("SetOutput",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GGOutputI : public GSM
{
public:
	GGOutputI(GPrgClassSession* o) : GSM("SetGraphOutput",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSOutputI : public GSM
{
public:
	GSOutputI(GPrgClassSession* o) : GSM("SetStatsOutput",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSetLinksMethodI : public GSM
{
public:
	GSetLinksMethodI(GPrgClassSession* o) : GSM("SetLinksMethod",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSetAutoSaveI : public GSM
{
public:
	GSetAutoSaveI(GPrgClassSession* o) : GSM("SetAutoSave",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GTestI : public GSM
{
public:
	GTestI(GPrgClassSession* o) : GSM("Test",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GLogI : public GSM
{
public:
	GLogI(GPrgClassSession* o) : GSM("Log",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSqlI : public GSM
{
public:
	GSqlI(GPrgClassSession* o) : GSM("ExecSql",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GModifyProfilesI : public GSM
{
public:
	GModifyProfilesI(GPrgClassSession* o) : GSM("ModifyProfiles",o) {}
	virtual void Run(R::RPrg*,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GComputeProfilesI : public GSM
{
public:
	GComputeProfilesI(GPrgClassSession* o) : GSM("ComputeProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GGroupProfilesI : public GSM
{
public:
	GGroupProfilesI(GPrgClassSession* o) : GSM("GroupProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};

//-----------------------------------------------------------------------------
class GCreateIdealI : public GSM
{
public:
	GCreateIdealI(GPrgClassSession* o) : GSM("CreateIdeal",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GMixIdealI : public GSM
{
public:
	GMixIdealI(GPrgClassSession* o) : GSM("MixIdeal",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GFdbksCycleI : public GSM
{
public:
	GFdbksCycleI(GPrgClassSession* o) : GSM("FdbksCycle",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GCompareIdealI : public GSM
{
public:
	GCompareIdealI(GPrgClassSession* o) : GSM("CompareIdeal",o) {}
	virtual void Run(R::RPrg*,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSetSubjectsParamI : public GSM
{
public:
	GSetSubjectsParamI(GPrgClassSession* o) : GSM("SetSubjectsParam",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSetComputingParamI : public GSM
{
public:
	GSetComputingParamI(GPrgClassSession* o) : GSM("SetComputingParam",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GSetGroupingParamI : public GSM
{
public:
	GSetGroupingParamI(GPrgClassSession* o) : GSM("SetGroupingParam",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GAddIdealI : public GSM
{
public:
	GAddIdealI(GPrgClassSession* o) : GSM("AddIdeal",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GAddProfilesI : public GSM
{
public:
	GAddProfilesI(GPrgClassSession* o) : GSM("AddProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GRealLifeI : public GSM
{
	char What[2];
	bool History;
	void CommonTasks(R::RPrgOutput* o) throw(GException);
public:
	GRealLifeI(GPrgClassSession* o) : GSM("RealLife",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GAddAssessmentsI : public GSM
{
public:
	GAddAssessmentsI(GPrgClassSession* o) : GSM("AddAssessments",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GTrackNewProfilesI : public GSM
{
public:
	GTrackNewProfilesI(GPrgClassSession* o) : GSM("TrackNewProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GClearNewProfilesI : public GSM
{
public:
	GClearNewProfilesI(GPrgClassSession* o) : GSM("ClearNewProfiles",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GStoreInHistoryI : public GSM
{
public:
	GStoreInHistoryI(GPrgClassSession* o) : GSM("StoreInHistory",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GResetTimeI : public GSM
{
public:
	GResetTimeI(GPrgClassSession* o) : GSM("ResetTime",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GComputeTimeI : public GSM
{
public:
	GComputeTimeI(GPrgClassSession* o) : GSM("ComputeTime",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};


//-----------------------------------------------------------------------------
class GWordsClusteringI : public GSM
{
public:
	GWordsClusteringI(GPrgClassSession* o) : GSM("WordsClustering",o) {}
	virtual void Run(R::RPrg* prg,R::RPrgOutput* o,R::RContainer<R::RPrgVar,unsigned int,true,false>* args) throw(GException);
};



//-----------------------------------------------------------------------------
//
// Instructions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void GSetRandI::Run(RPrg* prg,RPrgOutput* o,RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	int Rand;

	if(args->NbPtr!=1)
		throw GException("The rand value must be specified.");
	sprintf(tmp,"Set Rand value '%s'",args->Tab[0]->GetValue(prg));
	o->WriteStr(tmp);
	Rand=atoi(args->Tab[0]->GetValue(prg));
	if(Rand!=0)
	{
		Owner->Session->SetCurrentRandom(Rand);
	}

}


//-----------------------------------------------------------------------------
void GOutputI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("A filename must be specified.");
	sprintf(tmp,"Create Output file '%s'",args->Tab[0]->GetValue(prg));
	o->WriteStr(tmp);
	if(Owner->OFile)
	{
		delete Owner->OFile;
		Owner->OFile=0;
	}
	Owner->OFile=new RTextFile((args->Tab[0]->GetValue(prg)),Create);
	Owner->OFile->SetSeparator("\t");
	(*Owner->OFile)<<"Sets"<<"Recall"<<"Precision"<<"Total";
	if(Owner->TrackNewProfiles)
	{
		(*Owner->OFile)<<"TrackNew";
	}
	(*Owner->OFile)<<endl;
}


//-----------------------------------------------------------------------------
void GGOutputI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("A filename must be specified.");
	sprintf(tmp,"Create Graph Output file '%s'",args->Tab[0]->GetValue(prg));
	o->WriteStr(tmp);
	if(Owner->GOFile)
	{
		delete Owner->GOFile;
		Owner->GOFile=0;
	}
	Owner->GOFile=new RTextFile(args->Tab[0]->GetValue(prg),Create);
	Owner->GOFile->SetSeparator("\t");
}


//-----------------------------------------------------------------------------
void GSOutputI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("A filename must be specified.");
	sprintf(tmp,"Create Statistics Output file '%s'",args->Tab[0]->GetValue(prg));
	o->WriteStr(tmp);
	if(Owner->SOFile)
	{
		delete Owner->SOFile;
		Owner->SOFile=0;
	}
	Owner->SOFile=new RTextFile(args->Tab[0]->GetValue(prg),Create);
	Owner->SOFile->SetSeparator("\t");
}


//-----------------------------------------------------------------------------
void GSetLinksMethodI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs one parameter to specify the name of the link computing method (or \"None\").");
	sprintf(tmp,"Link Computing Method: %s",args->Tab[0]->GetValue(prg));
	o->WriteStr(tmp);
	Owner->Session->GetLinkCalcMng()->SetCurrentMethod(args->Tab[0]->GetValue(prg));
}



//-----------------------------------------------------------------------------
void GSetAutoSaveI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs one parameter (\"0\" or \"1\") to specify if the results must be stored.");
	if((args->Tab[0]->GetValue(prg))[0]=='0')
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


//-----------------------------------------------------------------------------
void GTestI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs the name of the test.");
	if(args->NbPtr)
	sprintf(tmp,"Current Test Name '%s'",args->Tab[0]->GetValue(prg));
	o->WriteStr(tmp);
	Owner->TestName=args->Tab[0]->GetValue(prg);
}



//-----------------------------------------------------------------------------
void GLogI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs the name of the log file.");
	sprintf(tmp,"Create Log file '%s'",args->Tab[0]->GetValue(prg));
	o->WriteStr(tmp);
}


//-----------------------------------------------------------------------------
void GSqlI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs the name of the SQL file.");
	sprintf(tmp,"Execute Sql file '%s'",args->Tab[0]->GetValue(prg));
	o->WriteStr(tmp);
	Owner->Session->ExecuteData(args->Tab[0]->GetValue(prg));
}


//-----------------------------------------------------------------------------
void GModifyProfilesI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("The method needs no parameters.");
	Owner->FirstProfile=false;
	o->WriteStr("Profiles are considered as modified");
}


//-----------------------------------------------------------------------------
void GComputeProfilesI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr>1)
		throw GException("The method needs maximum one parameter.");
	if(args->NbPtr==1)
		sprintf(tmp,"Compute Profiles: Method=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Compute Profiles: Current Method");
	o->WriteStr(tmp);
	if(args->NbPtr==1)
		Owner->Session->GetProfilingMng()->SetCurrentMethod(args->Tab[0]->GetValue(prg));
	if(Owner->Session->GetLinkCalcMng()->GetCurrentMethod())
		Owner->Session->GetLinkCalcMng()->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GetProfilingMng()->GetCurrentMethod()->ApplyConfig();
	Owner->Session->CalcProfiles(dynamic_cast<GSlot*>(o),Owner->FirstProfile,Owner->AutoSave);
	if(!Owner->FirstProfile) Owner->FirstProfile=true;
}


//-----------------------------------------------------------------------------
void GGroupProfilesI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr>1)
		throw GException("The method needs maximum one parameter.");
	if(args->NbPtr==1)
		sprintf(tmp,"Group Profiles: Method=\"%s\" ",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Group Profiles: Current Method");
	o->WriteStr(tmp);
	if(args->NbPtr==1)
		Owner->Session->GetGroupingMng()->SetCurrentMethod(args->Tab[0]->GetValue(prg));
	if(Owner->Session->GetSubjects()->GetIdealGroups())
	{
		GGrouping* algo=Owner->Session->GetGroupingMng()->GetCurrentMethod();
		algo->SetIdealGroups(Owner->Session->GetSubjects()->GetIdealGroups());
	}
	Owner->Session->GetGroupingMng()->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GetGroupCalcMng()->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GroupingProfiles(dynamic_cast<GSlot*>(o),Owner->FirstGroup,Owner->AutoSave);
	if(!Owner->FirstGroup) Owner->FirstGroup=true;
}


//-----------------------------------------------------------------------------
void GCreateIdealI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	o->WriteStr("Create Ideal Groups");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->CreateIdeal(Owner->AutoSave);
	Owner->FirstGroup=Owner->FirstProfile=false;
}


//-----------------------------------------------------------------------------
void GMixIdealI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr>1)
		throw GException("The method needs maximum one parameter.");
	if(args->NbPtr==1)
		sprintf(tmp,"Creating Mixed Groups: Settings=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		sprintf(tmp,"Creating Mixed Groups");
	o->WriteStr(tmp);
	GMixIdealGroups mix(Owner->Session,Owner->Session->GetSubjects()->GetIdealGroups());
	if(args->NbPtr==1)
		mix.SetSettings(args->Tab[0]->GetValue(prg));
	mix.Run(0);
}


//-----------------------------------------------------------------------------
void GFdbksCycleI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	o->WriteStr("Create Feedbacks Cycle");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->FdbksCycle(Owner->AutoSave);
}


//-----------------------------------------------------------------------------
void GCompareIdealI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	o->WriteStr("Compare with Ideal Groups");
	Owner->Session->GetSubjects()->Compare(dynamic_cast<GSlot*>(o));
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


//-----------------------------------------------------------------------------
void GSetSubjectsParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=2)
		throw GException("Method needs two parameters.");
	Owner->Session->GetSubjects()->Set(args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg));
}


//-----------------------------------------------------------------------------
void GSetComputingParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	GProfileCalc* calc;

	if(args->NbPtr!=2)
		throw GException("Method needs two parameters");
	calc=Owner->Session->GetProfilingMng()->GetCurrentMethod();
	if(!calc)
		throw GException("No profiling method selected.");
	calc->GetFactory()->Set(args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg));
}


//-----------------------------------------------------------------------------
void GSetGroupingParamI::Run(R::RPrg* prg,RPrgOutput*,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	GGrouping* calc;

	if(args->NbPtr!=2)
		throw GException("Method needs two parameters.");
	calc=Owner->Session->GetGroupingMng()->GetCurrentMethod();
	if(!calc)
		throw GException("No grouping computing method selected.");
	calc->GetFactory()->Set(args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg));
}


//-----------------------------------------------------------------------------
void GAddIdealI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	o->WriteStr("Create New Ideal Group");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->AddTopic(Owner->AutoSave);
}


//-----------------------------------------------------------------------------
void GAddProfilesI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	o->WriteStr("Adding Profiles");
	Owner->Session->GetSubjects()->Apply();
	sprintf(tmp, "%u new profiles created",Owner->Session->GetSubjects()->AddProfiles(Owner->AutoSave));
	o->WriteStr(tmp);
}


//-----------------------------------------------------------------------------
void GRealLifeI::CommonTasks(RPrgOutput* o) throw(GException)
{
	// Compute Profiles
	o->WriteStr("Compute Profiles: Current Method");
	if(Owner->Session->GetLinkCalcMng()->GetCurrentMethod())
		Owner->Session->GetLinkCalcMng()->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GetProfilingMng()->GetCurrentMethod()->ApplyConfig();
	Owner->Session->CalcProfiles(dynamic_cast<GSlot*>(o),Owner->FirstProfile,Owner->AutoSave);
	if(!Owner->FirstProfile) Owner->FirstProfile=true;

	// Group Profiles
	o->WriteStr("Group Profiles: Current Method");
	if(Owner->Session->GetSubjects()->GetIdealGroups())
	{
		GGrouping* algo=Owner->Session->GetGroupingMng()->GetCurrentMethod();
		algo->SetIdealGroups(Owner->Session->GetSubjects()->GetIdealGroups());
	}
	Owner->Session->GetGroupingMng()->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GetGroupCalcMng()->GetCurrentMethod()->ApplyConfig();
	Owner->Session->GroupingProfiles(dynamic_cast<GSlot*>(o),Owner->FirstGroup,Owner->AutoSave);
	if(!Owner->FirstGroup) Owner->FirstGroup=true;

	// Store History
	if(History)
	{
		sprintf(tmp,"Store History n�%u",Owner->NbHistory);
		o->WriteStr(tmp);
		Owner->Session->SaveMixedGroups(Owner->Session->GetGroups(),Owner->NbHistory++);
	}

	// Compare Ideal
	o->WriteStr("Compare with Ideal Groups");
	Owner->Session->GetSubjects()->Compare(dynamic_cast<GSlot*>(o));
	Owner->Precision=Owner->Session->GetSubjects()->GetPrecision();
	Owner->Recall=Owner->Session->GetSubjects()->GetRecall();
	Owner->Total=Owner->Session->GetSubjects()->GetTotal();
	sprintf(tmp,"Recall: %f  -  Precision: %f  -  Total: %f",Owner->Recall,Owner->Precision,Owner->Total);
	o->WriteStr(tmp);
	if(Owner->OFile)
		(*Owner->OFile)<<Owner->TestName<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
	if(Owner->GOFile)
		(*Owner->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
}


//-----------------------------------------------------------------------------
void GRealLifeI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
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

	if(args->NbPtr!=5)
		throw GException("Method needs five parameters.");
	sprintf(tmp,"Real Life: Settings=\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"",
	        args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg),args->Tab[2]->GetValue(prg),
	        args->Tab[3]->GetValue(prg),args->Tab[4]->GetValue(prg));
	o->WriteStr(tmp);
	Random=Owner->Session->GetRandom();
	MaxStep=atoi(args->Tab[0]->GetValue(prg));
	MinFBStep=atoi(args->Tab[1]->GetValue(prg));
	MaxFBStep=atoi(args->Tab[2]->GetValue(prg))-MinFBStep+1;
	Proba=atof(args->Tab[3]->GetValue(prg));
	History=atoi(args->Tab[4]->GetValue(prg));
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
			o->WriteStr("Create Feedbacks Cycle");
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
		o->WriteStr("Create 1 new profile");
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


//-----------------------------------------------------------------------------
void GAddAssessmentsI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	o->WriteStr("Adding Assessments");
	Owner->Session->GetSubjects()->Apply();
	Owner->Session->GetSubjects()->AddAssessments(Owner->AutoSave);
}


//-----------------------------------------------------------------------------
void GTrackNewProfilesI::Run(R::RPrg* prg,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs one parameter (\"0\" or \"1\") to specify if the profiles must be tracked.");
	if(args->NbPtr)
	{
		if(((args->Tab[0]->GetValue(prg))[0]=='0')||(args->Tab[0]->GetValue(prg)=="true"))
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


//-----------------------------------------------------------------------------
void GClearNewProfilesI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameter.");
	o->WriteStr("Clear New Profiles");
	Owner->Session->GetSubjects()->ClearLastAdded();
}


//-----------------------------------------------------------------------------
void GStoreInHistoryI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameter.");
	sprintf(tmp,"Store History n�%u",Owner->NbHistory);
	o->WriteStr(tmp);
	Owner->Session->SaveMixedGroups(Owner->Session->GetGroups(),Owner->NbHistory, true);
	Owner->Session->SaveHistoricProfiles(Owner->NbHistory++);
}


//-----------------------------------------------------------------------------
void GResetTimeI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameter.");
	o->WriteStr("Reset Time");
	Owner->ClockRef=time(0);
}


//-----------------------------------------------------------------------------
void GComputeTimeI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	time_t end;
	double cpu_time;

	end=time(0);
	if(args->NbPtr)
		throw GException("Method needs no parameter.");
	cpu_time=difftime(end,Owner->ClockRef);
	sprintf(tmp,"Ellapsed Time %f",cpu_time);
	o->WriteStr(tmp);
}



//-----------------------------------------------------------------------------
void GWordsClusteringI::Run(R::RPrg*,RPrgOutput* o,R::RContainer<RPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	o->WriteStr("Create New Concepts");
	Owner->Session->RemoveAssociation();
	Owner->Session->AnalyseAssociation(true);
}



//-----------------------------------------------------------------------------
//
// class GPrgClassSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GPrgClassSession::GPrgClassSession(GSession* s) throw(bad_alloc)
	: RPrgClass("Session"), /*IdealMethod(0),*/ Session(s), OFile(0),
	  GOFile(0), SOFile(0),DSOFile(0), NbHistory(0), AutoSave(false), TrackNewProfiles(false) 
{
	Methods.InsertPtr(new GOutputI(this));
	Methods.InsertPtr(new GGOutputI(this));
	Methods.InsertPtr(new GSOutputI(this));
	Methods.InsertPtr(new GSetLinksMethodI(this));
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
	Methods.InsertPtr(new GStoreInHistoryI(this));
	Methods.InsertPtr(new GResetTimeI(this));
	Methods.InsertPtr(new GComputeTimeI(this));
	Methods.InsertPtr(new GWordsClusteringI(this));
	Methods.InsertPtr(new GSetRandI(this));
};


// //-----------------------------------------------------------------------------
// GIdealGroup* GPrgClassSession::GetIdealMethod(void)
// {
// 	if(!IdealMethod)
// 		IdealMethod=new GIdealGroup(Session);
// 	return(IdealMethod);
// }


//-----------------------------------------------------------------------------
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
/*	if(IdealMethod)
	{
		delete IdealMethod;
		IdealMethod=0;
	}*/
}



//-----------------------------------------------------------------------------
//
// GSessionPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSessionPrg::GSessionPrg(RString f,GSession* s,RPrgOutput* o) throw(bad_alloc,GException)
	: RPrg(f,o)
{
	Classes.InsertPtr(new GPrgClassSession(s));
	Load();
}


//-----------------------------------------------------------------------------
GSessionPrg::~GSessionPrg(void)
{
}
