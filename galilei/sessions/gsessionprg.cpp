               /*

	GALILEI Research Project

	GSessionPrg.cpp

	Program to run on a Session - Implementation.

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsessionprg.h>
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <sessions/gslot.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofiledesc.h>
#include <groups/ggroups.h>
#include <groups/ggroupvector.h>
#include <groups/ggrouping.h>
#include <groups/ggroupingmanager.h>
#include <groups/gsubjecttree.h>
#include <profiles/gprofilecalc.h>
#include <sessions/gprginstfor.h>
#include <sessions/gprginstmethod.h>
#include <sessions/gprgvarconst.h>
#include <sessions/gprgvarref.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// Instructions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void GOutputI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("A filename must be specified.");
	sprintf(tmp,"Create Output file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
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
void GGOutputI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("A filename must be specified.");
	sprintf(tmp,"Create Graph Output file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	if(Owner->GOFile)
	{
		delete Owner->GOFile;
		Owner->GOFile=0;
	}
	Owner->GOFile=new RTextFile(args->Tab[0]->GetValue(prg),Create);
	Owner->GOFile->SetSeparator("\t");
}


//-----------------------------------------------------------------------------
void GSOutputI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("A filename must be specified.");
	sprintf(tmp,"Create Statistics Output file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	if(Owner->SOFile)
	{
		delete Owner->SOFile;
		Owner->SOFile=0;
	}
	Owner->SOFile=new RTextFile(args->Tab[0]->GetValue(prg),Create);
	Owner->SOFile->SetSeparator("\t");
}


//-----------------------------------------------------------------------------
void GSetLinksMethodI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs one parameter to specify the name of the link computing method (or \"None\").");
	sprintf(tmp,"Link Computing Method: %s",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	Owner->Session->GetLinkCalcMng()->SetCurrentMethod(args->Tab[0]->GetValue(prg));
}



//-----------------------------------------------------------------------------
void GSetAutoSaveI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs one parameter (\"0\" or \"1\") to specify if the results must be stored.");
	if((args->Tab[0]->GetValue(prg))[0]=='0')
	{
		r->WriteStr("Set AutoSave: false");
		Owner->AutoSave=false;
	}
	else
	{
		r->WriteStr("Set AutoSave: true");
		Owner->AutoSave=true;
	}
}


//-----------------------------------------------------------------------------
void GTestI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs the name of the test.");
	if(args->NbPtr)
	sprintf(tmp,"Current Test Name '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	Owner->TestName=args->Tab[0]->GetValue(prg);
}



//-----------------------------------------------------------------------------
void GLogI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs the name of the log file.");
	sprintf(tmp,"Create Log file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
}


//-----------------------------------------------------------------------------
void GSqlI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs the name of the SQL file.");
	sprintf(tmp,"Execute Sql file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	Owner->Session->ExecuteData(args->Tab[0]->GetValue(prg));
}


//-----------------------------------------------------------------------------
void GModifyProfilesI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("The method needs no parameters.");
	Owner->FirstProfile=false;
	r->WriteStr("Profiles are considered as modified");
}


//-----------------------------------------------------------------------------
void GComputeProfilesI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr>1)
		throw GException("The method needs maximum one parameter.");
	if(args->NbPtr==1)
		sprintf(tmp,"Compute Profiles: Method=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Compute Profiles: Current Method");
	r->WriteStr(tmp);
	if(args->NbPtr==1)
		Owner->Session->GetProfilingMng()->SetCurrentMethod(args->Tab[0]->GetValue(prg));
	Owner->Session->CalcProfiles(r,Owner->FirstProfile,Owner->AutoSave);
	if(!Owner->FirstProfile) Owner->FirstProfile=true;
}


//-----------------------------------------------------------------------------
void GGroupProfilesI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr>1)
		throw GException("The method needs maximum one parameter.");
	if(args->NbPtr==1)
		sprintf(tmp,"Group Profiles: Method=\"%s\" ",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Group Profiles: Current Method");
	r->WriteStr(tmp);
	if(args->NbPtr==1)
		Owner->Session->GetGroupingMng()->SetCurrentMethod(args->Tab[0]->GetValue(prg));
	if(Owner->Session->GetSubjects()->GetIdealGroups())
	{
		GGrouping* algo=Owner->Session->GetGroupingMng()->GetCurrentMethod();
		algo->SetIdealGroups(Owner->Session->GetSubjects()->GetIdealGroups());
	}
	Owner->Session->GroupingProfiles(r,Owner->FirstGroup,Owner->AutoSave);
	if(!Owner->FirstGroup) Owner->FirstGroup=true;
}


//-----------------------------------------------------------------------------
void GCreateIdealI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	r->WriteStr("Create Ideal Groups");
	Owner->Session->GetSubjects()->CreateIdeal(Owner->AutoSave);
	Owner->FirstGroup=Owner->FirstProfile=false;
}


//-----------------------------------------------------------------------------
void GMixIdealI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr>1)
		throw GException("The method needs maximum one parameter.");
	if(args->NbPtr==1)
		sprintf(tmp,"Creating Mixed Groups: Settings=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		sprintf(tmp,"Creating Mixed Groups");
	r->WriteStr(tmp);
	GMixIdealGroups mix(Owner->Session,Owner->Session->GetSubjects()->GetIdealGroups());
	if(args->NbPtr==1)
		mix.SetSettings(args->Tab[0]->GetValue(prg));
	mix.Run(0);
}


//-----------------------------------------------------------------------------
void GFdbksCycleI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	r->WriteStr("Create Feedbacks Cycle");
	Owner->Session->GetSubjects()->FdbksCycle(Owner->AutoSave);
}


//-----------------------------------------------------------------------------
void GCompareIdealI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	r->WriteStr("Compare with Ideal Groups");
	Owner->Session->GetSubjects()->Compare(r);
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
	r->WriteStr(tmp);
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
void GSetSubjectsParamI::Run(GSessionPrg* prg,GSlot*,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=2)
		throw GException("Method needs two parameters.");
	Owner->Session->GetSubjects()->Set(args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg));
}


//-----------------------------------------------------------------------------
void GSetComputingParamI::Run(GSessionPrg* prg,GSlot*,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
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
void GSetGroupingParamI::Run(GSessionPrg* prg,GSlot*,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
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
void GAddIdealI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	r->WriteStr("Create New Ideal Group");
	Owner->Session->GetSubjects()->AddTopic(Owner->AutoSave);
}


//-----------------------------------------------------------------------------
void GAddProfilesI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	r->WriteStr("Adding Profiles");
	sprintf(tmp, "%u new profiles created",Owner->Session->GetSubjects()->AddProfiles(Owner->AutoSave));
	r->WriteStr(tmp);
}


//-----------------------------------------------------------------------------
void GRealLifeI::CommonTasks(GSlot* r) throw(GException)
{
	// Compute Profiles
	r->WriteStr("Compute Profiles: Current Method");

	Owner->Session->CalcProfiles(r,Owner->FirstProfile,Owner->AutoSave);
	if(!Owner->FirstProfile) Owner->FirstProfile=true;

	// Group Profiles
	r->WriteStr("Group Profiles: Current Method");
	if(Owner->Session->GetSubjects()->GetIdealGroups())
	{
		GGrouping* algo=Owner->Session->GetGroupingMng()->GetCurrentMethod();
		algo->SetIdealGroups(Owner->Session->GetSubjects()->GetIdealGroups());
	}
	Owner->Session->GroupingProfiles(r,Owner->FirstGroup,Owner->AutoSave);

	if(!Owner->FirstGroup) Owner->FirstGroup=true;

	// Store History
	if(History)
	{
		sprintf(tmp,"Store History n°%u",Owner->NbHistory);
		r->WriteStr(tmp);
		Owner->Session->SaveMixedGroups(Owner->Session->GetGroups(),Owner->NbHistory++);
	}

	// Compare Ideal
	r->WriteStr("Compare with Ideal Groups");
	Owner->Session->GetSubjects()->Compare(r);
	Owner->Precision=Owner->Session->GetSubjects()->GetPrecision();
	Owner->Recall=Owner->Session->GetSubjects()->GetRecall();
	Owner->Total=Owner->Session->GetSubjects()->GetTotal();
	sprintf(tmp,"Recall: %f  -  Precision: %f  -  Total: %f",Owner->Recall,Owner->Precision,Owner->Total);
	r->WriteStr(tmp);
	if(Owner->OFile)
		(*Owner->OFile)<<Owner->TestName<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
	if(Owner->GOFile)
		(*Owner->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
}


//-----------------------------------------------------------------------------
void GRealLifeI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
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
	r->WriteStr(tmp);
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
			Owner->TestName=itoa(NbStep);

			// Create Feedbacks
			r->WriteStr("Create Feedbacks Cycle");
			What[0]='F';
			Owner->Session->GetSubjects()->FdbksCycle(Owner->AutoSave);
			CommonTasks(r);

			// Verify Nb Steps
			if(NbStep>MaxStep) break;
		}

		// Verify Nb Steps
		if(NbStep>MaxStep) break;
		NbStep++;

		// Set TestName
		Owner->TestName=itoa(NbStep);

		// Create 1 new profile
		r->WriteStr("Create 1 new profile");
		if(Random->Value()<Proba)
		{
			// Create One profile of an existing topic
			nbminprof=Owner->Session->GetSubjects()->GetUInt("NbProfMin");
			nbmaxprof=Owner->Session->GetSubjects()->GetUInt("NbProfMax");
			Owner->Session->GetSubjects()->Set("NbProfMin",1);
			Owner->Session->GetSubjects()->Set("NbProfMax",1);
			NewProf=Owner->Session->GetSubjects()->AddProfiles(Owner->AutoSave);
			Owner->Session->GetSubjects()->Set("NbProfMin",nbminprof);
			Owner->Session->GetSubjects()->Set("NbProfMax",nbmaxprof);
			if(NewProf)
				What[0]='E';
			else
			{
				What[0]='F';
				Owner->Session->GetSubjects()->FdbksCycle(Owner->AutoSave);
			}
		}
		else
		{
			// Create one profile of a new topic
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
				NewProf=Owner->Session->GetSubjects()->AddProfiles(Owner->AutoSave);
				Owner->Session->GetSubjects()->Set("NbProfMin",nbminprof);
				Owner->Session->GetSubjects()->Set("NbProfMax",nbmaxprof);
				if(NewProf)
					What[0]='E';
				else
				{
					What[0]='F';
					Owner->Session->GetSubjects()->FdbksCycle(Owner->AutoSave);
				}
			}
		}
		CommonTasks(r);

		// Verify Nb Steps
		if(NbStep>MaxStep) break;
	}
}


//-----------------------------------------------------------------------------
void GAddAssessmentsI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	r->WriteStr("Adding Assessments");
	Owner->Session->GetSubjects()->AddAssessments(Owner->AutoSave);
}


//-----------------------------------------------------------------------------
void GTrackNewProfilesI::Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=1)
		throw GException("The method needs one parameter (\"0\" or \"1\") to specify if the profiles must be tracked.");
	if(args->NbPtr)
	{
		if(((args->Tab[0]->GetValue(prg))[0]=='0')||(args->Tab[0]->GetValue(prg)=="true"))
		{
			r->WriteStr("Track New Profiles: false");
			Owner->TrackNewProfiles=false;
		}
		else
		{
			r->WriteStr("Track New Profiles: true");
			Owner->TrackNewProfiles=true;
		}
	}
}


//-----------------------------------------------------------------------------
void GClearNewProfilesI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameter.");
	r->WriteStr("Clear New Profiles");
	Owner->Session->GetSubjects()->ClearLastAdded();
}


//-----------------------------------------------------------------------------
void GStoreInHistoryI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameter.");
	sprintf(tmp,"Store History n°%u",Owner->NbHistory);
	r->WriteStr(tmp);
	Owner->Session->SaveMixedGroups(Owner->Session->GetGroups(),Owner->NbHistory, true);
	Owner->Session->SaveHistoricProfiles(Owner->NbHistory++);
}


//-----------------------------------------------------------------------------
void GResetTimeI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameter.");
	r->WriteStr("Reset Time");
	Owner->ClockRef=time(0);
}


//-----------------------------------------------------------------------------
void GComputeTimeI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	time_t end;
	double cpu_time;

	end=time(0);
	if(args->NbPtr)
		throw GException("Method needs no parameter.");
	cpu_time=difftime(end,Owner->ClockRef);
	sprintf(tmp,"Ellapsed Time %f",cpu_time);
	r->WriteStr(tmp);
}



//-----------------------------------------------------------------------------
void GWordsClusteringI::Run(GSessionPrg*,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
		throw GException("Method needs no parameters.");
	r->WriteStr("Create New Concepts");
	Owner->Session->RemoveAssociation();
	Owner->Session->AnalyseAssociation(true);
}


//-----------------------------------------------------------------------------
//
// class GPrgClassSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgClassSession::GPrgClassSession(GSession* s) throw(bad_alloc)
	: GPrgClass("Session"), /*IdealMethod(0),*/ Session(s), OFile(0),
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
};


// //-----------------------------------------------------------------------------
// GIdealGroup* GALILEI::GPrgClassSession::GetIdealMethod(void)
// {
// 	if(!IdealMethod)
// 		IdealMethod=new GIdealGroup(Session);
// 	return(IdealMethod);
// }


//-----------------------------------------------------------------------------
GALILEI::GPrgClassSession::~GPrgClassSession(void)
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
GALILEI::GSessionPrg::GSessionPrg(RString f,GSession* s,GSlot* r) throw(bad_alloc,GException)
	: FileName(f), Rec(r), Insts(40), Vars(10,5), Classes(10,5), Prg(FileName)
{
	GPrgInst* i;

	// Init Part
	Classes.InsertPtr(new GPrgClassSession(s));

	Prg.SetRem("#");
	ReadLine=true;
	while((!ReadLine)||(!Prg.Eof()&&(ReadLine)))
	{
		i=AnalyseLine(Prg);
		if(i)
			Insts.InsertPtr(i);
	}
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSessionPrg::CountTabs(char* line)
{
	int tabs;
	char* ptr;

	// Count tabs
	ptr=line;
	tabs=0;
	while((*ptr)&&((*ptr)=='\t'))
	{
		tabs++;
		ptr++;
	}
	return(tabs);
}


//-----------------------------------------------------------------------------
GPrgInst* GALILEI::GSessionPrg::AnalyseLine(RTextFile& prg) throw(bad_alloc,GException)
{
	RString l;
	char* ptr;
	char* obj;
	char* line;
	char* name;
	char what;
	char tabs;
	GPrgVar* r;
	char buf[200];
	char* tbuf;

	// Read the line
	if(ReadLine)
	{
		tbuf=prg.GetLine();
		strcpy(tmp,tbuf);
	}
	line=tmp;

	// Skip Spaces and count tabs
	tabs=CountTabs(line);
	ptr=line;
	while((*ptr)&&(isspace(*ptr)))
	{
		ptr++;
	}

	// Read if it is an Object or instruction
	obj=ptr;
	while((*ptr)&&((*ptr)!='.')&&((*ptr)!='=')&&((*ptr)!='('))
		ptr++;
	what=(*ptr);
	(*(ptr++))=0;

	// Look if instruction
	if(what=='(')
	{
		// If is "for"
		if(!strcmp(obj,"for"))
		{
			GPrgInstFor* f=new GPrgInstFor(ptr,tabs);

			// Read the next lines
			tbuf=Prg.GetLine();
			if(!tbuf) return(f);
			strcpy(tmp,tbuf);
			ReadLine=false;
			while((!ReadLine)||(!Prg.Eof()&&(ReadLine)))
			{
				if(CountTabs(tmp)<=f->GetTabs())
					break;
				f->AddInst(AnalyseLine(prg));
				if(ReadLine)
				{
					tbuf=prg.GetLine();
					if(!tbuf) return(f);
					strcpy(tmp,tbuf);
					ReadLine=false;
				}
			}
			return(f);
		}
		throw GException(RString("Instruction \"")+RString(obj)+"\" does not exist.");
	}

	// Look if call to an object
	if(what=='.')
	{
		// Look if the object is known
		GPrgClass* c=Classes.GetPtr<const char*>(obj);
		if(!c)
			throw GException(RString("Object \"")+obj+"\" unknown");

		// Read the methods name
		name=ptr;
		while((*ptr)&&((*ptr)!='('))
			ptr++;
		(*(ptr++))=0;
		GPrgFunc* method=c->GetMethod(name);
		if(!method)
		{
			sprintf(buf,"Method \"%s\" unknown for object \"%s\"",name,obj);
			throw GException(buf);
		}

		// Create the instruction
		GPrgInstMethod* inst=new GPrgInstMethod(method);
		while((*ptr))
		{
			r=GSessionPrg::AnalyseParam(ptr);
			if(r)
				inst->AddParam(r);
		}
		ReadLine=true;
		return(inst);
	}

	ReadLine=true;
	return(0);
}


//-----------------------------------------------------------------------------
GPrgVar* GALILEI::GSessionPrg::AnalyseParam(char* &param) throw(bad_alloc,GException)
{
	char* ptr;

	// Skip before something
	while((*param)&&((*param)!='"')&&(!isalpha(*param)))
		param++;
	if(!(*param))
		return(0);

	if((*param)=='"')
	{
		// Skip "
		param++;

		// Read Value and skip " and next thing
		ptr=param;
		while((*param)&&((*param)!='"'))
			param++;
		(*(param++))=0;
		param++;

		// Value
		return(new GPrgVarConst(ptr));
	}
	else
	{
		// Look until , or )
		ptr=param;
		while((*param)&&((*param)!=',')&&((*param)!=')'))
			param++;
		(*(param++))=0;

		// Ref
		return(new GPrgVarRef(ptr));
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionPrg::Exec(void) throw(GException)
{
	if(!Rec) return;
	for(Insts.Start();!Insts.End();Insts.Next())
		Insts()->Run(this,Rec);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionPrg::AddVar(GPrgVar* var) throw(bad_alloc,GException)
{
	Vars.InsertPtr(var);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionPrg::DelVar(GPrgVar* var) throw(bad_alloc,GException)
{
	Vars.DeletePtr(var);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSessionPrg::GetValue(const char* var) throw(GException)
{
	GPrgVar* v=Vars.GetPtr<const char*>(var);

	if(!v) return(0);
	return(v->GetValue(this));
}


//-----------------------------------------------------------------------------
GALILEI::GSessionPrg::~GSessionPrg(void)
{
}
