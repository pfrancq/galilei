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
#include <groups/gcomparegrouping.h>
#include <profiles/gprofilecalc.h>
#include <sessions/gprginstfor.h>
#include <sessions/gprginstmethod.h>
#include <sessions/gprgvarconst.h>
#include <sessions/gprgvarref.h>
#include <tests/gquerydocsgroup.h>
#include <tests/gstatsimdocprof.h>
#include <tests/gstatsimdocgrp.h>
using namespace GALILEI;
using namespace RIO;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// Instructions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void GOutputI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(!args->NbPtr)
		throw GException("No filename specified");
	sprintf(tmp,"Create Output file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	if(Owner->OFile)
	{
		delete Owner->OFile;
		Owner->OFile=0;
	}
	Owner->OFile=new RTextFile((args->Tab[0]->GetValue(prg)),RIO::Create);
	Owner->OFile->SetSeparator("\t");
	(*Owner->OFile)<<"Sets"<<"Recall"<<"Precision"<<"Total";
	if(Owner->TrackNewProfiles)
	{
		(*Owner->OFile)<<"TrackNew";
	}
	(*Owner->OFile)<<endl;
}


//-----------------------------------------------------------------------------
void GGOutputI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	sprintf(tmp,"Create Graph Output file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	if(Owner->GOFile)
	{
		delete Owner->GOFile;
		Owner->GOFile=0;
	}
	Owner->GOFile=new RTextFile(args->Tab[0]->GetValue(prg),RIO::Create);
	Owner->GOFile->SetSeparator("\t");
}


//-----------------------------------------------------------------------------
void GSOutputI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	sprintf(tmp,"Create Statistics Output file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	if(Owner->SOFile)
	{
		delete Owner->SOFile;
		Owner->SOFile=0;
	}
	Owner->SOFile=new RTextFile(args->Tab[0]->GetValue(prg),RIO::Create);
	Owner->SOFile->SetSeparator("\t");
}


//-----------------------------------------------------------------------------
void GSetAutoSaveI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr)
	{
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
}


//-----------------------------------------------------------------------------
void GTestI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	sprintf(tmp,"Current Test Name '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	Owner->TestName=args->Tab[0]->GetValue(prg);
}



//-----------------------------------------------------------------------------
void GLogI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	sprintf(tmp,"Create Log file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
}


//-----------------------------------------------------------------------------
void GSqlI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	sprintf(tmp,"Execute Sql file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	Owner->Session->ExecuteData(args->Tab[0]->GetValue(prg));
}


//-----------------------------------------------------------------------------
void GModifyProfilesI::Run(GSessionPrg*,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>*) throw(GException)
{
	Owner->FirstProfile=false;
	r->WriteStr("Profiles are considered as modified");
}


//-----------------------------------------------------------------------------
void GComputeProfilesI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr==2)
		sprintf(tmp,"Compute Profiles: Method=\"%s\"  Settings=\"%s\"",args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg));
	else
	if(args->NbPtr==1)
		sprintf(tmp,"Compute Profiles: Method=\"%s\"  Current Settings",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Compute Profiles: Current Method and Settings");
	r->WriteStr(tmp);
	if(args->NbPtr==1)
		Owner->Session->SetCurrentComputingMethod(args->Tab[0]->GetValue(prg));
	if(args->NbPtr==2)
		Owner->Session->SetCurrentComputingMethodSettings(args->Tab[1]->GetValue(prg));
	Owner->Session->CalcProfiles(r,Owner->FirstProfile,Owner->AutoSave);
	if(!Owner->FirstProfile) Owner->FirstProfile=true;
}


//-----------------------------------------------------------------------------
void GGroupProfilesI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr==2)
		sprintf(tmp,"Group Profiles: Method=\"%s\"  Settings=\"%s\"",args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg));
	else
	if(args->NbPtr==1)
		sprintf(tmp,"Group Profiles: Method=\"%s\"  Current Settings",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Group Profiles: Current Method and Settings");
	r->WriteStr(tmp);
	if(args->NbPtr==1)
		Owner->Session->SetCurrentGroupingMethod(args->Tab[0]->GetValue(prg));
	if(args->NbPtr==2)
		Owner->Session->SetCurrentGroupingMethodSettings(args->Tab[1]->GetValue(prg));
	if(Owner->Session->GetIdealGroups())
	{
		GGrouping* algo=Owner->Session->GetCurrentGroupingMethod();
		algo->SetIdealGroups(Owner->Session->GetIdealGroups());
	}
	Owner->Session->GroupingProfiles(r,Owner->FirstGroup,Owner->AutoSave);
	if(!Owner->FirstGroup) Owner->FirstGroup=true;
}

//-----------------------------------------------------------------------------
void GCreateIdealI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr==1)
		sprintf(tmp,"Create Ideal Groups: Settings=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Create Ideal Groups");
	r->WriteStr(tmp);
	if(args->NbPtr==1)
		Owner->GetIdealMethod()->SetSettings(args->Tab[0]->GetValue(prg));
	Owner->GetIdealMethod()->Run(Owner->AutoSave);
	Owner->FirstGroup=Owner->FirstProfile=false;
}


//-----------------------------------------------------------------------------
void GMixIdealI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr==1)
		sprintf(tmp,"Creating Mixed Groups: Settings=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		sprintf(tmp,"Creating Mixed Groups");
	r->WriteStr(tmp);
	GMixIdealGroups mix(Owner->Session,Owner->Session->GetIdealGroups());
	if(args->NbPtr==1)
		mix.SetSettings(args->Tab[0]->GetValue(prg));
	mix.Run(0);
}


//-----------------------------------------------------------------------------
void GFdbksCycleI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr==1)
		sprintf(tmp,"Create Feedbacks Cycle: Settings=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Create Feedbacks Cycle");
	r->WriteStr(tmp);
	if(args->NbPtr==1)
		FdbksMethod.SetSettings(args->Tab[0]->GetValue(prg));
	FdbksMethod.Run(Owner->AutoSave);
}


//-----------------------------------------------------------------------------
void GCompareIdealI::Run(GSessionPrg*,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>*) throw(GException)
{
	r->WriteStr("Compare with Ideal Groups");
	GCompareGrouping CompMethod(Owner->Session,Owner->Session->GetIdealGroups());
	CompMethod.Compare(r);
	Owner->Precision=CompMethod.GetPrecision();
	Owner->Recall=CompMethod.GetRecall();
	Owner->Total=CompMethod.GetTotal();
	if(Owner->TrackNewProfiles)
	{
		Owner->PercAss=Owner->GetIdealMethod()->ComputePercAss();
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
void GStatsProfilesI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	bool g,l;

	if(args->NbPtr!=2)
		throw GException("Method needs two parameters");
	g=atoi(args->Tab[0]->GetValue(prg));
	l=atoi(args->Tab[1]->GetValue(prg));
	sprintf(tmp,"Statistics on Profiles : Global=%u,Local=%u",g,l);
	r->WriteStr(tmp);
	GStatSimSubProf ProfStats(Owner->Session,Owner->SOFile,g,l);
	ProfStats.Run();
	if(g)
	{
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  AVGGol: %f  -  Rie: %f",
		        ProfStats.GetAvgIntraG(),ProfStats.GetAvgInterG(),ProfStats.GetAVGolG(),ProfStats.GetAVGGolG(),ProfStats.GetRieG());
		r->WriteStr(tmp);
	}
	if(l)
	{
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  AVGGol: %f  -  Rie: %f",
		        ProfStats.GetAvgIntraL(),ProfStats.GetAvgInterL(),ProfStats.GetAVGolL(),ProfStats.GetAVGGolL(),ProfStats.GetRieL());
		r->WriteStr(tmp);
	}
	(*Owner->OFile)<<"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  AVGGol: %f  -  Rie: %f"<<endl;
	if(Owner->OFile&&g)
		(*Owner->OFile)<<ProfStats.GetAvgIntraG()<<ProfStats.GetAvgInterG()<<ProfStats.GetAVGolG()<<ProfStats.GetAVGGolG()<<ProfStats.GetRieG()<<endl;
	if(Owner->OFile&&l)
		(*Owner->OFile)<<ProfStats.GetAvgIntraL()<<ProfStats.GetAvgInterL()<<ProfStats.GetAVGolL()<<ProfStats.GetAVGGolL()<<ProfStats.GetRieL()<<endl;
}


//-----------------------------------------------------------------------------
void GStatsDocsI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	bool g,l;

	if(args->NbPtr!=2)
		throw GException("Method needs two parameters");
	g=atoi(args->Tab[0]->GetValue(prg));
	l=atoi(args->Tab[1]->GetValue(prg));
	sprintf(tmp,"Statistics on Documents : Global=%u,Local=%u",g,l);
	r->WriteStr(tmp);
	GStatSimDoc DocStats(Owner->Session,Owner->SOFile,g,l);
	DocStats.Run();
	if(g)
	{
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  Rie: %f ",
		        DocStats.GetAvgIntraG(),DocStats.GetAvgInterG(),DocStats.GetAVGolG(),DocStats.GetRieG());
		r->WriteStr(tmp);
	}
	if(l)
	{
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  Rie: %f ",
		        DocStats.GetAvgIntraL(),DocStats.GetAvgInterL(),DocStats.GetAVGolL(),DocStats.GetRieL());
		r->WriteStr(tmp);
	}
	if(Owner->OFile&&g)
		(*Owner->OFile)<<DocStats.GetAvgIntraG()<<DocStats.GetAvgInterG()<<DocStats.GetAVGolG()<<DocStats.GetRieG()<<endl;
	if(Owner->OFile&&l)
		(*Owner->OFile)<<DocStats.GetAvgIntraL()<<DocStats.GetAvgInterL()<<DocStats.GetAVGolL()<<DocStats.GetRieL()<<endl;
}


//-----------------------------------------------------------------------------
void GSetComputingParamI::Run(GSessionPrg* prg,GSlot*,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=2)
		throw GException("Method needs two parameters");
	Owner->Session->GetCurrentComputingMethod()->SetParam(args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg));
}


//-----------------------------------------------------------------------------
void GSetGroupingParamI::Run(GSessionPrg* prg,GSlot*,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=2)
		throw GException("Method needs two parameters");
	Owner->Session->GetCurrentGroupingMethod()->SetParam(args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg));
}


//-----------------------------------------------------------------------------
void GRunQueriesI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=4)
		throw GException("Method needs four parameters");
	r->WriteStr("Construct Grouping from Ideal");
	Owner->Session->CopyIdealGroups();
	r->WriteStr("Run Queries");
	GQueryDocsGroup Queries(Owner->Session,Owner->SOFile);
	Queries.Run(atoi(args->Tab[0]->GetValue(prg)),atoi(args->Tab[1]->GetValue(prg)),atoi(args->Tab[2]->GetValue(prg)),atoi(args->Tab[3]->GetValue(prg)));
	sprintf(tmp,"First: %f  -  Second: %f  -  Recall: %f  -  SimIntra: %f  -  SimInter: %f ",
	        Queries.GetFirst(),Queries.GetSecond(),Queries.GetRecall(),Queries.GetSimQueryIntra(),Queries.GetSimQueryInter());
	r->WriteStr(tmp);
}


//-----------------------------------------------------------------------------
void GStatsProfilesDocsI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	bool g,l;

	if(args->NbPtr!=2)
		throw GException("Method needs two parameters");
	g=atoi(args->Tab[0]->GetValue(prg));
	l=atoi(args->Tab[1]->GetValue(prg));
	sprintf(tmp,"Statistics on Profiles/Documents : Global=%u,Local=%u",g,l);
	r->WriteStr(tmp);
	GStatSimDocProf ProfDocStats(Owner->Session,Owner->SOFile,g,l);
	ProfDocStats.Run();
	if(g)
	{
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  AVGGol: %f  -  Rie: %f"
		            "  P(R0): %f  -  P(R10): %f  -  P(R20): %f  -  P(R30): %f  -  P(R40): %f  -  P(R50): %f"
		            "  -  P(R60): %f  -  P(R70): %f  -  P(R80): %f  -  P(R90): %f  -  P(R100): %f",
		            ProfDocStats.GetAvgIntraG(),ProfDocStats.GetAvgInterG(),ProfDocStats.GetAVGolG(),ProfDocStats.GetAVGGolG(),ProfDocStats.GetRieG(),
		            ProfDocStats.GetPrecisionG(0),ProfDocStats.GetPrecisionG(1),ProfDocStats.GetPrecisionG(2),ProfDocStats.GetPrecisionG(3),
		            ProfDocStats.GetPrecisionG(4),ProfDocStats.GetPrecisionG(5),ProfDocStats.GetPrecisionG(6),ProfDocStats.GetPrecisionG(7),
		            ProfDocStats.GetPrecisionG(8),ProfDocStats.GetPrecisionG(9),ProfDocStats.GetPrecisionG(10));
		r->WriteStr(tmp);
	}
	if(l)
	{
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  AVGGol: %f  -  Rie: %f"
		            "-  P(R0): %f  -  P(R10): %f  -  P(R20): %f  -  P(R30): %f  -  P(R40): %f  -  P(R50): %f"
		            "  -  P(R60): %f  -  P(R70): %f  -  P(R80): %f  -  P(R90): %f  -  P(R100): %f",
		            ProfDocStats.GetAvgIntraL(),ProfDocStats.GetAvgInterL(),ProfDocStats.GetAVGolL(),ProfDocStats.GetAVGGolL(),ProfDocStats.GetRieL(),
		            ProfDocStats.GetPrecisionL(0),ProfDocStats.GetPrecisionL(1),ProfDocStats.GetPrecisionL(2),ProfDocStats.GetPrecisionL(3),
		            ProfDocStats.GetPrecisionL(4),ProfDocStats.GetPrecisionL(5),ProfDocStats.GetPrecisionL(6),ProfDocStats.GetPrecisionL(7),
		            ProfDocStats.GetPrecisionL(8),ProfDocStats.GetPrecisionL(9),ProfDocStats.GetPrecisionL(10));
		r->WriteStr(tmp);
	}
	if(Owner->OFile&&g)
	{
		(*Owner->OFile)<<ProfDocStats.GetAvgIntraG()<<ProfDocStats.GetAvgInterG();
		(*Owner->OFile)<<ProfDocStats.GetAVGolG()<<ProfDocStats.GetAVGGolG()<<ProfDocStats.GetRieG();
		for(int i=0;i<11;i++)
			(*Owner->OFile)<<ProfDocStats.GetPrecisionG(i);
		(*Owner->OFile)<<endl;
	}
	if(Owner->OFile&&l)
	{
		(*Owner->OFile)<<ProfDocStats.GetAvgIntraL()<<ProfDocStats.GetAvgInterL();
		(*Owner->OFile)<<ProfDocStats.GetAVGolL()<<ProfDocStats.GetAVGGolL()<<ProfDocStats.GetRieL();
		for(int i=0;i<11;i++)
			(*Owner->OFile)<<ProfDocStats.GetPrecisionL(i);
		(*Owner->OFile)<<endl;
	}
}


//-----------------------------------------------------------------------------
void GStatsGroupsDocsI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	bool g,l;

	if(args->NbPtr!=2)
		throw GException("Method needs two parameters");
	g=atoi(args->Tab[0]->GetValue(prg));
	l=atoi(args->Tab[1]->GetValue(prg));
	r->WriteStr("Construct Grouping from Ideal");
	Owner->Session->CopyIdealGroups();
	sprintf(tmp,"Statistics on Groups/Documents : Global=%u,Local=%u",g,l);
	r->WriteStr(tmp);
	GStatSimDocGrp GrpDocStats(Owner->Session,Owner->SOFile,g,l);
	GrpDocStats.Run();
	if(g)
	{
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  AVGGol: %f  -  Rie: %f"
		            "  P(R0): %f  -  P(R10): %f  -  P(R20): %f  -  P(R30): %f  -  P(R40): %f  -  P(R50): %f"
	                "  -  P(R60): %f  -  P(R70): %f  -  P(R80): %f  -  P(R90): %f  -  P(R100): %f",
		        GrpDocStats.GetAvgIntraG(),GrpDocStats.GetAvgInterG(),GrpDocStats.GetAVGolG(),GrpDocStats.GetAVGGolG(),GrpDocStats.GetRieG(),
		        GrpDocStats.GetPrecisionG(0),GrpDocStats.GetPrecisionG(1),GrpDocStats.GetPrecisionG(2),GrpDocStats.GetPrecisionG(3),
		        GrpDocStats.GetPrecisionG(4),GrpDocStats.GetPrecisionG(5),GrpDocStats.GetPrecisionG(6),GrpDocStats.GetPrecisionG(7),
		        GrpDocStats.GetPrecisionG(8),GrpDocStats.GetPrecisionG(9),GrpDocStats.GetPrecisionG(10));
		r->WriteStr(tmp);
	}
	if(l)
	{
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  AVGGol: %f  -  Rie: %f"
		            "-  P(R0): %f  -  P(R10): %f  -  P(R20): %f  -  P(R30): %f  -  P(R40): %f  -  P(R50): %f"
	                "  -  P(R60): %f  -  P(R70): %f  -  P(R80): %f  -  P(R90): %f  -  P(R100): %f",
		        GrpDocStats.GetAvgIntraL(),GrpDocStats.GetAvgInterL(),GrpDocStats.GetAVGolL(),GrpDocStats.GetAVGGolL(),GrpDocStats.GetRieL(),
		        GrpDocStats.GetPrecisionL(0),GrpDocStats.GetPrecisionL(1),GrpDocStats.GetPrecisionL(2),GrpDocStats.GetPrecisionL(3),
		        GrpDocStats.GetPrecisionL(4),GrpDocStats.GetPrecisionL(5),GrpDocStats.GetPrecisionL(6),GrpDocStats.GetPrecisionL(7),
		        GrpDocStats.GetPrecisionL(8),GrpDocStats.GetPrecisionL(9),GrpDocStats.GetPrecisionL(10));
		r->WriteStr(tmp);
	}
	if(Owner->OFile&&g)
	{
		(*Owner->OFile)<<GrpDocStats.GetAvgIntraG()<<GrpDocStats.GetAvgInterG();
		(*Owner->OFile)<<GrpDocStats.GetAVGolG()<<GrpDocStats.GetAVGGolG()<<GrpDocStats.GetRieG();
		for(int i=0;i<11;i++)
			(*Owner->OFile)<<GrpDocStats.GetPrecisionG(i);
		(*Owner->OFile)<<endl;
	}
	if(Owner->OFile&&l)
	{
		(*Owner->OFile)<<GrpDocStats.GetAvgIntraL()<<GrpDocStats.GetAvgInterL();
		(*Owner->OFile)<<GrpDocStats.GetAVGolL()<<GrpDocStats.GetAVGGolL()<<GrpDocStats.GetRieL();
		for(int i=0;i<11;i++)
			(*Owner->OFile)<<GrpDocStats.GetPrecisionL(i);
		(*Owner->OFile)<<endl;
	}
}


//-----------------------------------------------------------------------------
void GAddIdealI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr==1)
		sprintf(tmp,"Create New Ideal Group: Settings=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Create New Ideal Group");
	r->WriteStr(tmp);
	if(args->NbPtr==1)
		Owner->GetIdealMethod()->SetSettings(args->Tab[0]->GetValue(prg));
	Owner->GetIdealMethod()->AddTopic(Owner->AutoSave);
}


//-----------------------------------------------------------------------------
void GAddProfilesI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=2)
		throw GException("Method needs two parameters");
	sprintf(tmp,"Adding Profile: Settings=\"%s\",\"%s\"",args->Tab[0]->GetValue(prg),args->Tab[1]->GetValue(prg));
	r->WriteStr(tmp);
//<<<<<<< gsessionprg.cpp
//	if(args->NbPtr==1)
//		Owner->GetIdealMethod()->SetSettings(args->Tab[0]->GetValue(prg));
////	cout <<atoi(args->Tab[0]->GetValue(prg)) <<endl;
////	cout <<atoi(args->Tab[1]->GetValue(prg)) <<endl;
//
//=======
//>>>>>>> 1.43
	sprintf(tmp, "%u new profiles created",Owner->GetIdealMethod()->AddProfiles(atoi(args->Tab[0]->GetValue(prg)), atoi(args->Tab[1]->GetValue(prg)),Owner->AutoSave));
	r->WriteStr(tmp);
}


//-----------------------------------------------------------------------------
void GRealLifeI::CommonTasks(GSlot* r) throw(GException)
{
	// Compute Profiles
	r->WriteStr("Compute Profiles: Current Method and Settings");

	Owner->Session->CalcProfiles(r,Owner->FirstProfile,Owner->AutoSave);
	if(!Owner->FirstProfile) Owner->FirstProfile=true;

	// Group Profiles
	r->WriteStr("Group Profiles: Current Method and Settings");
	if(Owner->Session->GetIdealGroups())
	{
		GGrouping* algo=Owner->Session->GetCurrentGroupingMethod();
		algo->SetIdealGroups(Owner->Session->GetIdealGroups());
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
	GCompareGrouping CompMethod(Owner->Session,Owner->Session->GetIdealGroups());
	CompMethod.Compare(r);
	Owner->Precision=CompMethod.GetPrecision();
	Owner->Recall=CompMethod.GetRecall();
	Owner->Total=CompMethod.GetTotal();
	sprintf(tmp,"Recall: %f  -  Precision: %f  -  Total: %f",Owner->Recall,Owner->Precision,Owner->Total);
	r->WriteStr(tmp);
	if(Owner->OFile)
		(*Owner->OFile)<<Owner->TestName<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
	if(Owner->GOFile)
		(*Owner->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->Total<<What<<endl;
}


//-----------------------------------------------------------------------------
void GRealLifeI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	unsigned int MaxStep;
	unsigned int MinFBStep;
	unsigned int MaxFBStep;
	unsigned int NbStep;
	double Proba;
	RRandom* Random;
	unsigned int nb;
	unsigned int NewProf;

	if(args->NbPtr!=5)
		throw GException("Method needs five parameters");
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
			FdbksMethod.Run(Owner->AutoSave);
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
			NewProf=Owner->GetIdealMethod()->AddProfiles(1,1,Owner->AutoSave);
			if(NewProf)
				What[0]='E';
			else
			{
				What[0]='F';
				FdbksMethod.Run(Owner->AutoSave);
			}
		}
		else
		{
			// Create one profile of a new topic
			if(Owner->GetIdealMethod()->AddTopic(Owner->AutoSave))
			{
				What[0]='N';
			}
			else
			{
				Proba=1.0;
				NewProf=Owner->GetIdealMethod()->AddProfiles(1,1,Owner->AutoSave);
				if(NewProf)
					What[0]='E';
				else
				{
					What[0]='F';
					FdbksMethod.Run(Owner->AutoSave);
				}
			}
		}
		CommonTasks(r);

		// Verify Nb Steps
		if(NbStep>MaxStep) break;
	}
}


//-----------------------------------------------------------------------------
void GAddAssessmentsI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	unsigned int nbDocs;

	if(args->NbPtr!=1)
		throw GException("Method needs one parameters");
	nbDocs=atoi(args->Tab[0]->GetValue(prg));
	sprintf(tmp,"Adding Assessments: Settings=\"%u\"",nbDocs);
	r->WriteStr(tmp);
	Owner->GetIdealMethod()->AddAssessments(Owner->AutoSave,nbDocs);
}


//-----------------------------------------------------------------------------
void GTrackNewProfilesI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
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
void GClearNewProfilesI::Run(GSessionPrg*,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=0)
		throw GException("Method needs no parameter");
	r->WriteStr("Clear New Profiles");
	Owner->GetIdealMethod()->ClearLastAdded();
}


//-----------------------------------------------------------------------------
void GStoreInHistoryI::Run(GSessionPrg*,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=0)
		throw GException("Method needs no parameter");
	sprintf(tmp,"Store History n°%u",Owner->NbHistory);
	r->WriteStr(tmp);
	Owner->Session->SaveMixedGroups(Owner->Session->GetGroups(),Owner->NbHistory, true);
	Owner->Session->SaveHistoricProfiles(Owner->NbHistory++);
}


//-----------------------------------------------------------------------------
void GResetTimeI::Run(GSessionPrg*,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr!=0)
		throw GException("Method needs no parameter");
	r->WriteStr("Reset Time");
	Owner->ClockRef=clock();
}


//-----------------------------------------------------------------------------
void GComputeTimeI::Run(GSessionPrg*,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	clock_t end;
	double cpu_time;

	end=clock();
	if(args->NbPtr!=0)
		throw GException("Method needs no parameter");
	cpu_time=static_cast<double>(end-Owner->ClockRef)/CLOCKS_PER_SEC;
	sprintf(tmp,"Ellapsed Time %f",cpu_time);
	r->WriteStr(tmp);
}



//-----------------------------------------------------------------------------
//
// class GPrgClassSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgClassSession::GPrgClassSession(GSession* s) throw(bad_alloc)
	: GPrgClass("Session"), IdealMethod(0), Session(s), OFile(0),
	  GOFile(0), SOFile(0), NbHistory(0), AutoSave(false), TrackNewProfiles(false) 
{
	Methods.InsertPtr(new GOutputI(this));
	Methods.InsertPtr(new GGOutputI(this));
	Methods.InsertPtr(new GSOutputI(this));
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
	Methods.InsertPtr(new GStatsProfilesI(this));
	Methods.InsertPtr(new GStatsDocsI(this));
	Methods.InsertPtr(new GSetComputingParamI(this));
	Methods.InsertPtr(new GSetGroupingParamI(this));
	Methods.InsertPtr(new GRunQueriesI(this));
	Methods.InsertPtr(new GStatsProfilesDocsI(this));
	Methods.InsertPtr(new GStatsGroupsDocsI(this));
	Methods.InsertPtr(new GAddIdealI(this));
	Methods.InsertPtr(new GAddProfilesI(this));
	Methods.InsertPtr(new GRealLifeI(this));
	Methods.InsertPtr(new GAddAssessmentsI(this));
	Methods.InsertPtr(new GTrackNewProfilesI(this));
	Methods.InsertPtr(new GClearNewProfilesI(this));
	Methods.InsertPtr(new GStoreInHistoryI(this));
};


//-----------------------------------------------------------------------------
GIdealGroup* GALILEI::GPrgClassSession::GetIdealMethod(void)
{
	if(!IdealMethod)
		IdealMethod=new GIdealGroup(Session);
	return(IdealMethod);
}


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
	if(IdealMethod)
	{
		delete IdealMethod;
		IdealMethod=0;
	}
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
GPrgInst* GALILEI::GSessionPrg::AnalyseLine(RIO::RTextFile& prg) throw(bad_alloc,GException)
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
	cout<<"Analyse: "<<line<<endl;
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
				{
					cout<<"Skip: "<<tmp<<endl;
					break;
				}
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
