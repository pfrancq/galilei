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
	Owner->OFile=new RTextFile(args->Tab[0]->GetValue(prg),RIO::Create);
	Owner->OFile->SetSeparator("\t");
	(*Owner->OFile)<<"Sets"<<"Recall"<<"Precision"<<"Total"<<endl;
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
	(*Owner->SOFile)<<"AVGintra"<<"AVGinter"<<"AVGol"<<"tRie"<<"J"<<"AVGGrpol"<<endl;
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
	if(Owner->Groups)
	{
		GGrouping* algo=Owner->Session->GetCurrentGroupingMethod();
		algo->SetIdealGroups(Owner->Groups);    

	}
	Owner->Session->GroupingProfiles(r,Owner->FirstGroup,Owner->AutoSave);
	if(!Owner->FirstGroup) Owner->FirstGroup=true;
}


//-----------------------------------------------------------------------------
void GLoadIdealI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(args->NbPtr==1)
		throw GException("No file Defined");
	sprintf(tmp,"Load Ideal Groups from file '%s'",args->Tab[0]->GetValue(prg));
	r->WriteStr(tmp);
	Owner->LoadGroups(args->Tab[0]->GetValue(prg));
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
		IdealMethod.SetSettings(args->Tab[0]->GetValue(prg));
	IdealMethod.CreateJudgement(Owner->Parents,Owner->Groups,Owner->AutoSave);
	Owner->FirstGroup=Owner->FirstProfile=false;
}


//-----------------------------------------------------------------------------
void GMixIdealI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(!Owner->Groups)
		throw GException("No Ideal Groups Defined");
	if(args->NbPtr==1)
		sprintf(tmp,"Creating Mixed Groups: Settings=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		sprintf(tmp,"Creating Mixed Groups");
	r->WriteStr(tmp);
	GMixIdealGroups mix(Owner->Session,Owner->Parents,Owner->Groups);
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
	FdbksMethod.Run(Owner->Parents,Owner->Groups,Owner->AutoSave);
}


//-----------------------------------------------------------------------------
void GCompareIdealI::Run(GSessionPrg*,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>*) throw(GException)
{
	if(!Owner->Groups)
		throw GException("No Ideal Groups Defined");
	strcpy(tmp,"Compare with Ideal Groups");
	r->WriteStr(tmp);
	GCompareGrouping CompMethod(Owner->Session,Owner->Groups);
	CompMethod.Compare(0);
	Owner->Precision=CompMethod.GetPrecision();
	Owner->Recall=CompMethod.GetRecall();
	Owner->Total=CompMethod.GetTotal();
	sprintf(tmp,"Recall: %f  -  Precision: %f  -  Total: %f",Owner->Recall,Owner->Precision,Owner->Total);
	r->WriteStr(tmp);
	if(Owner->OFile)
		(*Owner->OFile)<<Owner->TestName<<Owner->Recall<<Owner->Precision<<Owner->Total<<endl;
	if(Owner->GOFile)
		(*Owner->GOFile)<<Owner->Recall<<Owner->Precision<<Owner->Total<<endl;
}


//-----------------------------------------------------------------------------
void GStatsProfilesI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(!Owner->Groups)
		throw GException("No Ideal Groups Defined");
	if(args->NbPtr==1)
		sprintf(tmp,"Statistics on Profiles: Settings=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Statistics on Profiles");
	r->WriteStr(tmp);
	GStatSimSubProf ProfStats(Owner->Session,Owner->Groups);
	if(args->NbPtr==1)
		ProfStats.SetSettings(args->Tab[0]->GetValue(prg));
	ProfStats.Run();
	sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  Rie: %f  -  J: %f  - AVGGrpGol: %f",
	        ProfStats.GetAvgIntra(),ProfStats.GetAvgInter(),ProfStats.GetAVGol(),ProfStats.GetRie(),ProfStats.GetJ(),ProfStats.GetAVGGrpol());
	r->WriteStr(tmp);
	if(Owner->SOFile)
		(*Owner->SOFile)<<ProfStats.GetAvgIntra()<<ProfStats.GetAvgInter()<<ProfStats.GetAVGol()<<ProfStats.GetRie()<<ProfStats.GetJ()<<ProfStats.GetAVGGrpol()<<endl;
}


//-----------------------------------------------------------------------------
void GStatsDocsI::Run(GSessionPrg* prg,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException)
{
	if(!Owner->Groups)
		throw GException("No Ideal Groups Defined");
	if(args->NbPtr==1)
		sprintf(tmp,"Statistics on Documents : Settings=\"%s\"",args->Tab[0]->GetValue(prg));
	else
		strcpy(tmp,"Statistics on Documents");
	r->WriteStr(tmp);
	if(args->NbPtr==1)
		DocStats.SetSettings(args->Tab[0]->GetValue(prg));
	DocStats.Run();
	sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  Rie: %f ",
	        DocStats.GetAvgIntra(),DocStats.GetAvgInter(),DocStats.GetAVGol(),DocStats.GetRie());
	r->WriteStr(tmp);
	if(Owner->SOFile)
		(*Owner->SOFile)<<DocStats.GetAvgIntra()<<DocStats.GetAvgInter()<<DocStats.GetAVGol()<<DocStats.GetRie()<<endl;
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
void GRunQueriesI::Run(GSessionPrg* /*prg*/,GSlot* r,RStd::RContainer<GPrgVar,unsigned int,true,false>* /*args*/) throw(GException)
{
	GQueryDocsGroup Queries(Owner->Session);
	Queries.Run();

	sprintf(tmp,"Good: %f%%  -  SimIntra: %f  -  SimInter: %f ",
	        Queries.GetTargets(),Queries.GetSimQueryIntra(),Queries.GetSimQueryInter());
	r->WriteStr(tmp);
}



//-----------------------------------------------------------------------------
//
// class GPrgClassSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgClassSession::GPrgClassSession(GSession* s) throw(bad_alloc)
	: GPrgClass("Session"), Session(s), OFile(0),
	  GOFile(0), SOFile(0), Groups(0), Parents(0), AutoSave(false)
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
	Methods.InsertPtr(new GLoadIdealI(this));
	Methods.InsertPtr(new GCreateIdealI(this));
	Methods.InsertPtr(new GMixIdealI(this));
	Methods.InsertPtr(new GFdbksCycleI(this));
	Methods.InsertPtr(new GCompareIdealI(this));
	Methods.InsertPtr(new GStatsProfilesI(this));
	Methods.InsertPtr(new GStatsDocsI(this));
	Methods.InsertPtr(new GSetComputingParamI(this));
	Methods.InsertPtr(new GSetGroupingParamI(this));
	Methods.InsertPtr(new GRunQueriesI(this));
};


//-----------------------------------------------------------------------------
void GALILEI::GPrgClassSession::LoadGroups(const char* filename) throw(GException)
{
	unsigned int nb;
	unsigned int i,j,id;
	GGroups* groups;
	GGroup* group;
	unsigned int nbprof;
	GLang* lang;
	GProfile* prof;
	GSubProfile* sub;
	GLangCursor CurLang;

	CurLang=Session->GetLangsCursor();
	RTextFile f(filename);
	f>>nb;
	if(Groups)
		Groups->Clear();
	Groups=new RContainer<GGroups,unsigned int,true,true>(nb,nb/2);
	for(CurLang.Start();!CurLang.End();CurLang.Next())
		Groups->InsertPtr(new GGroups(CurLang()));
	for(i=0;i<nb;i++)
	{
		lang=Session->GetLang(f.GetWord());
		f>>nbprof;
		groups=Groups->GetPtr<const GLang*>(lang);
		groups->InsertPtr(group=new GGroupVector(i,lang));
		for(j=nbprof+1;--j;)
		{
			f>>id;
			prof=Session->GetProfile(id);
			if(!prof) continue;
			sub=prof->GetSubProfile(lang);
			if(!sub) continue;
			group->InsertPtr(sub);
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GPrgClassSession::~GPrgClassSession(void)
{
	if(Groups)
		delete Groups;
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
	if(Parents)
		delete Parents;
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
int GALILEI::GSessionPrg::CountTabs(char* line)
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
			GPrgInstFor* f=new GPrgInstFor(ptr);

			// Read the next lines
			tbuf=Prg.GetLine();
			if(!tbuf) return(f);
			strcpy(tmp,tbuf);
			ReadLine=false;
			while((!ReadLine)||(!Prg.Eof()&&(ReadLine)))
			{
				if(CountTabs(tmp)<=tabs) break;
				f->AddInst(AnalyseLine(prg));
				tbuf=prg.GetLine();
				if(!tbuf) return(f);
				strcpy(tmp,tbuf);
				ReadLine=false;
			}
			return(f);
		}
		throw GException(RString("Instruction \"")+RString(name)+"\" does not exist.");
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
