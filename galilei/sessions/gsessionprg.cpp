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
#include <profiles/ggetfeedback.h>
#include <groups/gidealgroup.h>
#include <tests/gmixidealgroups.h>
#include <tests/gstatsimsubprof.h>
#include <tests/gstatsimdoc.h>
#include <sessions/gprginstfor.h>
#include <sessions/gprginstclass.h>
#include <sessions/gprgvarconst.h>
#include <sessions/gprgvarref.h>
using namespace GALILEI;
using namespace RIO;
using namespace RStd;


//-----------------------------------------------------------------------------
//
// Instructions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GSM : public GPrgInst
{
public:
	char tmp[300];
	GPrgClassSession* Owner;
	GSM(const char* name,GPrgClassSession* o)
		: GPrgInst(name), Owner(o) {}
};


//-----------------------------------------------------------------------------
class GOutputI : public GSM
{
public:
	GOutputI(GPrgClassSession* o) : GSM("Output",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		sprintf(tmp,"Create Output file '%s'",Params.Tab[0]->GetValue(prg));
		r->WriteStr(tmp);
		if(Owner->OFile)
		{
			delete Owner->OFile;
			Owner->OFile=0;
		}
		Owner->OFile=new RTextFile(Params.Tab[0]->GetValue(prg),RIO::Create);
		Owner->OFile->SetSeparator("\t");
		(*Owner->OFile)<<"Sets"<<"Recall"<<"Precision"<<"Total"<<endl;
	}
};


//-----------------------------------------------------------------------------
class GGOutputI : public GSM
{
public:
	GGOutputI(GPrgClassSession* o) : GSM("GOutput",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		sprintf(tmp,"Create Graph Output file '%s'",Params.Tab[0]->GetValue(prg));
		r->WriteStr(tmp);
		if(Owner->GOFile)
		{
			delete Owner->GOFile;
			Owner->GOFile=0;
		}
		Owner->GOFile=new RTextFile(Params.Tab[0]->GetValue(prg),RIO::Create);
		Owner->GOFile->SetSeparator("\t");
	}
};


//-----------------------------------------------------------------------------
class GSOutputI : public GSM
{
public:
	GSOutputI(GPrgClassSession* o) : GSM("SOutput",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		sprintf(tmp,"Create Statistics Output file '%s'",Params.Tab[0]->GetValue(prg));
		r->WriteStr(tmp);
		if(Owner->SOFile)
		{
			delete Owner->SOFile;
			Owner->SOFile=0;
		}
		Owner->SOFile=new RTextFile(Params.Tab[0]->GetValue(prg),RIO::Create);
		Owner->SOFile->SetSeparator("\t");
		(*Owner->SOFile)<<"AVGintra"<<"AVGinter"<<"AVGol"<<"tRie"<<endl;
	}
};


//-----------------------------------------------------------------------------
class GSetAutoSaveI : public GSM
{
public:
	GSetAutoSaveI(GPrgClassSession* o) : GSM("SetAutoSave",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		if(Params.NbPtr)
		{
			if((Params.Tab[0]->GetValue(prg))[0]=='0')
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
};


//-----------------------------------------------------------------------------
class GTestI : public GSM
{
public:
	GTestI(GPrgClassSession* o) : GSM("Test",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		sprintf(tmp,"Current Test Name '%s'",Params.Tab[0]->GetValue(prg));
		r->WriteStr(tmp);
		Owner->TestName=Params.Tab[0]->GetValue(prg);
	}
};


//-----------------------------------------------------------------------------
class GLogI : public GSM
{
public:
	GLogI(GPrgClassSession* o) : GSM("Log",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
			sprintf(tmp,"Create Log file '%s'",Params.Tab[0]->GetValue(prg));
			r->WriteStr(tmp);
	}
};


//-----------------------------------------------------------------------------
class GSqlI : public GSM
{
public:
	GSqlI(GPrgClassSession* o) : GSM("ExecSql",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
			sprintf(tmp,"Execute Sql file '%s'",Params.Tab[0]->GetValue(prg));
			r->WriteStr(tmp);
			Owner->Session->ExecuteData(Params.Tab[0]->GetValue(prg));
	}
};


//-----------------------------------------------------------------------------
class GModifyProfilesI : public GSM
{
public:
	GModifyProfilesI(GPrgClassSession* o) : GSM("ModifyProfiles",o) {}
	virtual void Run(GSessionPrg*,GSlot* r) throw(GException)
	{
		Owner->FirstProfile=false;
		r->WriteStr("Profiles are considered as modified");
	}
};


//-----------------------------------------------------------------------------
class GComputeProfilesI : public GSM
{
public:
	GComputeProfilesI(GPrgClassSession* o) : GSM("ComputeProfiles",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		if(Params.NbPtr==2)
			sprintf(tmp,"Compute Profiles: Method=\"%s\"  Settings=\"%s\"",Params.Tab[0]->GetValue(prg),Params.Tab[1]->GetValue(prg));
		else
		if(Params.NbPtr==1)
			sprintf(tmp,"Compute Profiles: Method=\"%s\"  Current Settings",Params.Tab[0]->GetValue(prg));
		else
			strcpy(tmp,"Compute Profiles: Current Method and Settings");
		r->WriteStr(tmp);
		if(Params.NbPtr==1)
			Owner->Session->SetCurrentComputingMethod(Params.Tab[0]->GetValue(prg));
		if(Params.NbPtr==2)
			Owner->Session->SetCurrentComputingMethodSettings(Params.Tab[1]->GetValue(prg));
		Owner->Session->CalcProfiles(r,Owner->FirstProfile,Owner->AutoSave);
		if(!Owner->FirstProfile) Owner->FirstProfile=true;
	}
};


//-----------------------------------------------------------------------------
class GGroupProfilesI : public GSM
{
public:
	GGroupProfilesI(GPrgClassSession* o) : GSM("GroupProfiles",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		if(Params.NbPtr==2)
			sprintf(tmp,"Group Profiles: Method=\"%s\"  Settings=\"%s\"",Params.Tab[0]->GetValue(prg),Params.Tab[1]->GetValue(prg));
		else
		if(Params.NbPtr==1)
			sprintf(tmp,"Group Profiles: Method=\"%s\"  Current Settings",Params.Tab[0]->GetValue(prg));
		else
			strcpy(tmp,"Group Profiles: Current Method and Settings");
		r->WriteStr(tmp);
		if(Params.NbPtr==1)
			Owner->Session->SetCurrentGroupingMethod(Params.Tab[0]->GetValue(prg));
		if(Params.NbPtr==2)
			Owner->Session->SetCurrentGroupingMethodSettings(Params.Tab[1]->GetValue(prg));
		if(Owner->Groups)
		{
			GGrouping* algo=Owner->Session->GetCurrentGroupingMethod();
			algo->SetIdealGroups(Owner->Groups);
		}
		Owner->Session->GroupingProfiles(r,Owner->FirstGroup,Owner->AutoSave);
		if(!Owner->FirstGroup) Owner->FirstGroup=true;
	}
};


//-----------------------------------------------------------------------------
class GLoadIdealI : public GSM
{
public:
	GLoadIdealI(GPrgClassSession* o) : GSM("LoadIdeal",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		if(Params.NbPtr==1)
			throw GException("No file Defined");
		sprintf(tmp,"Load Ideal Groups from file '%s'",Params.Tab[0]->GetValue(prg));
		r->WriteStr(tmp);
		Owner->LoadGroups(Params.Tab[0]->GetValue(prg));
	}
};


//-----------------------------------------------------------------------------
class GCreateIdealI : public GSM
{
	GIdealGroup IdealMethod;
public:
	GCreateIdealI(GPrgClassSession* o) : GSM("CreateIdeal",o),IdealMethod(Owner->Session) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		if(Params.NbPtr==1)
			sprintf(tmp,"Create Ideal Groups: Settings=\"%s\"",Params.Tab[0]->GetValue(prg));
		else
			strcpy(tmp,"Create Ideal Groups");
		r->WriteStr(tmp);
		if(Params.NbPtr==1)
			IdealMethod.SetSettings(Params.Tab[0]->GetValue(prg));
		IdealMethod.CreateJudgement(Owner->Parents,Owner->Groups,Owner->AutoSave);
		Owner->FirstGroup=Owner->FirstProfile=false;
	}
};


//-----------------------------------------------------------------------------
class GMixIdealI : public GSM
{
public:
	GMixIdealI(GPrgClassSession* o) : GSM("MixIdeal",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		if(!Owner->Groups)
			throw GException("No Ideal Groups Defined");
		if(Params.NbPtr==1)
			sprintf(tmp,"Creating Mixed Groups: Settings=\"%s\"",Params.Tab[0]->GetValue(prg));
		else
			sprintf(tmp,"Creating Mixed Groups");
		r->WriteStr(tmp);
		GMixIdealGroups mix(Owner->Session,Owner->Parents,Owner->Groups);
		if(Params.NbPtr==1)
			mix.SetSettings(Params.Tab[0]->GetValue(prg));
		mix.Run();
	}
};


//-----------------------------------------------------------------------------
class GFdbksCycleI : public GSM
{
	GGetFeedback FdbksMethod;
public:
	GFdbksCycleI(GPrgClassSession* o) : GSM("FdbksCycle",o),FdbksMethod(Owner->Session) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		if(Params.NbPtr==1)
			sprintf(tmp,"Create Feedbacks Cycle: Settings=\"%s\"",Params.Tab[0]->GetValue(prg));
		else
			strcpy(tmp,"Create Feedbacks Cycle");
		r->WriteStr(tmp);
		if(Params.NbPtr==1)
			FdbksMethod.SetSettings(Params.Tab[0]->GetValue(prg));
		FdbksMethod.Run(Owner->Parents,Owner->Groups,Owner->AutoSave);
	}
};


//-----------------------------------------------------------------------------
class GCompareIdealI : public GSM
{
public:
	GCompareIdealI(GPrgClassSession* o) : GSM("CompareIdeal",o) {}
	virtual void Run(GSessionPrg*,GSlot* r) throw(GException)
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
};


//-----------------------------------------------------------------------------
class GStatsProfilesI : public GSM
{
public:
	GStatsProfilesI(GPrgClassSession* o) : GSM("StatsProfiles",o) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		if(!Owner->Groups)
			throw GException("No Ideal Groups Defined");
		if(Params.NbPtr==1)
			sprintf(tmp,"Statistics on Profiles: Settings=\"%s\"",Params.Tab[0]->GetValue(prg));
		else
			strcpy(tmp,"Statistics on Profiles");
		r->WriteStr(tmp);
		GStatSimSubProf ProfStats(Owner->Session,Owner->Groups);
		if(Params.NbPtr==1)
			ProfStats.SetSettings(Params.Tab[0]->GetValue(prg));
		ProfStats.Run();
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  Rie: %f",
		        ProfStats.GetAvgIntra(),ProfStats.GetAvgInter(),ProfStats.GetAVGol(),ProfStats.GetRie());
		r->WriteStr(tmp);
		if(Owner->SOFile)
			(*Owner->SOFile)<<ProfStats.GetAvgIntra()<<ProfStats.GetAvgInter()<<ProfStats.GetAVGol()<<ProfStats.GetRie()<<endl;
	}
};


//-----------------------------------------------------------------------------
class GStatsDocsI : public GSM
{
	GStatSimDoc DocStats;
public:
	GStatsDocsI(GPrgClassSession* o) : GSM("StatsDocs",o), DocStats(Owner->Session) {}
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException)
	{
		if(!Owner->Groups)
			throw GException("No Ideal Groups Defined");
		if(Params.NbPtr==1)
			sprintf(tmp,"Statistics on Documents : Settings=\"%s\"",Params.Tab[0]->GetValue(prg));
		else
			strcpy(tmp,"Statistics on Documents");
		r->WriteStr(tmp);
		if(Params.NbPtr==1)
			DocStats.SetSettings(Params.Tab[0]->GetValue(prg));
		DocStats.Run();
		sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  Rie: %f",
		        DocStats.GetAvgIntra(),DocStats.GetAvgInter(),DocStats.GetAVGol(),DocStats.GetRie());
		r->WriteStr(tmp);
		if(Owner->SOFile)
			(*Owner->SOFile)<<DocStats.GetAvgIntra()<<DocStats.GetAvgInter()<<DocStats.GetAVGol()<<DocStats.GetRie()<<endl;
	}
};


//-----------------------------------------------------------------------------
//
// class GPrgClassSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GPrgClassSession::GPrgClassSession(GSession* s) throw(bad_alloc)
	: GPrgClass("GSession"), Session(s)
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
}


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
//
// InsType
//
//-----------------------------------------------------------------------------

////-----------------------------------------------------------------------------
class GALILEI::GSessionPrg::InstType
{
public:
	RString Name;
	tInst Type;

	InstType(const char* n,tInst t) : Name(n), Type(t) {}
	int Compare(const InstType* t) const {return(Name.Compare(t->Name));}
	int Compare(const char* t) const {return(Name.Compare(t));}
};



//-----------------------------------------------------------------------------
//
// Inst
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GSessionPrg::Inst
{
public:
	RString Param1;
	RString Param2;
	tInst Type;

	Inst(const char* p1,const char* p2,tInst t) : Type(t)
	{
		if(p1) Param1=p1;
		if(p2) Param2=p2;
	}
	int Compare(const Inst* t) const {return(-1);}
};



//-----------------------------------------------------------------------------
//
// GSessionPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// static variable
RStd::RContainer<GALILEI::GSessionPrg::InstType,unsigned int,true,true> GALILEI::GSessionPrg::InstTypes(30,10);


//-----------------------------------------------------------------------------
GALILEI::GSessionPrg::GSessionPrg(RString f,GSession* s,GSlot* r) throw(bad_alloc,GException)
	: FileName(f), Session(s), Rec(r), Insts(40), OFile(0),
	  GOFile(0), SOFile(0), Groups(0), IdealMethod(0), FdbksMethod(0),
	  Parents(0), AutoSave(false), StatSimSubProf(0), Vars(10,5), Classes(10,5)
{
	Inst* t;
	char tmp[500];

	// Init Part
	IdealMethod=new GIdealGroup(Session);
	FdbksMethod=new GGetFeedback(Session);
	Classes.InsertPtr(new GPrgClassSession(Session));

	// Instructions
	if(!GSessionPrg::InstTypes.NbPtr)
	{
		GSessionPrg::InstTypes.InsertPtr(new InstType("SetOutput",Output));
		GSessionPrg::InstTypes.InsertPtr(new InstType("SetGraphOutput",GOutput));
		GSessionPrg::InstTypes.InsertPtr(new InstType("SetStatsOutput",SOutput));
		GSessionPrg::InstTypes.InsertPtr(new InstType("SetAutoSave",SetAutoSave));
		GSessionPrg::InstTypes.InsertPtr(new InstType("Test",Test));
		GSessionPrg::InstTypes.InsertPtr(new InstType("Log",Log));
		GSessionPrg::InstTypes.InsertPtr(new InstType("ExecSql",Sql));
		GSessionPrg::InstTypes.InsertPtr(new InstType("ComputeProfiles",Comp));
		GSessionPrg::InstTypes.InsertPtr(new InstType("GroupProfiles",Group));
		GSessionPrg::InstTypes.InsertPtr(new InstType("CompareIdeal",CmpIdeal));
		GSessionPrg::InstTypes.InsertPtr(new InstType("CreateIdeal",CreateIdeal));
		GSessionPrg::InstTypes.InsertPtr(new InstType("LoadIdeal",LoadIdeal));
		GSessionPrg::InstTypes.InsertPtr(new InstType("MixIdeal",MixIdeal));
		GSessionPrg::InstTypes.InsertPtr(new InstType("FdbksCycle",Fdbks));
		GSessionPrg::InstTypes.InsertPtr(new InstType("StatsProfiles",StatProf));
		GSessionPrg::InstTypes.InsertPtr(new InstType("StatsDocs",StatDoc));
		GSessionPrg::InstTypes.InsertPtr(new InstType("ModifyProfiles",ModifyProf));
		GSessionPrg::InstTypes.InsertPtr(new InstType("for",forinst));
	}

	// Read Program File
	RTextFile Prg(FileName);
	Prg.SetRem("#");
	while(!Prg.Eof())
	{
		// Read the line
		strcpy(tmp,Prg.GetLine());
		t=AnalyseLine(tmp);
		if(!t) continue;
		Insts.InsertPtr(t);
	}
}


//-----------------------------------------------------------------------------
GSessionPrg::Inst* GALILEI::GSessionPrg::AnalyseLine(char* line) throw(bad_alloc,GException)
{
	RString l;
	InstType* t;
	char* ptr;
	char* obj;
	char* name;
	char* param1;
	char* param2;

	ptr=line;

	// Skip Spaces
	while((*ptr)&&(isspace(*ptr)))
		ptr++;

	// Read the Object
	obj=ptr;
	while((*ptr)&&((*ptr)!='.')&&((*ptr)!='=')&&((*ptr)!='('))
		ptr++;
	(*(ptr++))=0;

	// Is the object not a "for"
	if(!strcmp(obj,"for"))
	{
		GPrgInstFor f(ptr);
		throw GException("for not implemented for "+RString(name));
	}

	// The object must be "Session"
	if(strcmp(obj,"Session"))
		throw GException(RString("Object \"")+obj+"\" unknown");

	// Read the instruction
	name=ptr;
	while((*ptr)&&((*ptr)!='('))
		ptr++;
	(*(ptr++))=0;

	// Read the first parameter if exists
	if((*ptr)==')')
		param1=0;
	else
	{
		ptr++; // Skip "
		param1=ptr;
		while((*ptr)&&((*ptr)!='"'))
			ptr++;
		(*(ptr++))=0;
		ptr++;  // Skip "
	}

	// Read the second parameter if exists
	if((*ptr)==')')
		param2=0;
	else
	{
		ptr++; // Skip "
		param2=ptr;
		while((*ptr)&&((*ptr)!='"'))
			ptr++;
		(*(ptr++))=0;
		ptr++;  // Skip "
	}

	// Create the instructions
	t=GSessionPrg::InstTypes.GetPtr<const char*>(name);
	if(!t) return(0);
	return(new Inst(param1,param2,t->Type));
}


//-----------------------------------------------------------------------------
GPrgVar* GALILEI::GSessionPrg::AnalyseParam(GPrgVar* owner,char* &param) throw(bad_alloc,GException)
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
		return(new GPrgVarConst(owner,ptr));
	}
	else
	{
		// Look until , or )
		ptr=param;
		while((*param)&&((*param)!=',')&&((*param)!=')'))
			param++;
		(*(param++))=0;

		// Ref
		return(new GPrgVarRef(owner,ptr));
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionPrg::LoadGroups(const char* filename) throw(GException)
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
void GALILEI::GSessionPrg::Run(const Inst* i) throw(GException)
{
	char tmp[300];
	GCompareGrouping* CompMethod;
	GStatSimSubProf* ProfStats;
	GStatSimDoc* DocStats;

	if(!i) return;
	switch(i->Type)
	{
		case Output:
			sprintf(tmp,"Create Output file '%s'",i->Param1());
			Rec->WriteStr(tmp);
			if(OFile)
			{
				delete OFile;
				OFile=0;
			}
			OFile=new RTextFile(i->Param1,RIO::Create);
			OFile->SetSeparator("\t");
			(*OFile)<<"Sets"<<"Recall"<<"Precision"<<"Total"<<endl;
			break;

		case GOutput:
			sprintf(tmp,"Create Graph Output file '%s'",i->Param1());
			Rec->WriteStr(tmp);
			if(OFile)
			{
				delete GOFile;
				GOFile=0;
			}
			GOFile=new RTextFile(i->Param1,RIO::Create);
			GOFile->SetSeparator("\t");
			break;

		case SOutput:
			sprintf(tmp,"Create Statistics Output file '%s'",i->Param1());
			Rec->WriteStr(tmp);
			if(SOFile)
			{
				delete SOFile;
				SOFile=0;
			}
			SOFile=new RTextFile(i->Param1,RIO::Create);
			SOFile->SetSeparator("\t");
			(*SOFile)<<"AVGintra"<<"AVGinter"<<"AVGol"<<"tRie"<<endl;
			break;

		case SetAutoSave:
			if(i->Param1.GetLen()==1)
			{
				if((i->Param1())[0]=='0')
				{
					Rec->WriteStr("Set AutoSave: false");
					AutoSave=false;
				}
				else
				{
					Rec->WriteStr("Set AutoSave: true");
					AutoSave=true;
				}
			}
			break;

		case Test:
			sprintf(tmp,"Current Test Name '%s'",i->Param1());
			Rec->WriteStr(tmp);
			TestName=i->Param1();
			break;

		case Log:
			sprintf(tmp,"Create Log file '%s'",i->Param1());
			Rec->WriteStr(tmp);
			break;

		case Sql:
			sprintf(tmp,"Execute Sql file '%s'",i->Param1());
			Rec->WriteStr(tmp);
			Session->ExecuteData(i->Param1());
			break;

		case ModifyProf:
			FirstProfile=false;
			Rec->WriteStr("Profiles are considered as modified");
			break;

		case Comp:
			if(i->Param1.GetLen()&&i->Param2.GetLen())
				sprintf(tmp,"Compute Profiles: Method=\"%s\"  Settings=\"%s\"",i->Param1(),i->Param2());
			else
			if(i->Param1.GetLen())
				sprintf(tmp,"Compute Profiles: Method=\"%s\"  Current Settings",i->Param1());
			else
				strcpy(tmp,"Compute Profiles: Current Method and Settings");
			Rec->WriteStr(tmp);
			if(i->Param1.GetLen())
				Session->SetCurrentComputingMethod(i->Param1());
			if(i->Param2.GetLen())
				Session->SetCurrentComputingMethodSettings(i->Param2());
			Session->CalcProfiles(Rec,FirstProfile,AutoSave);
			if(!FirstProfile) FirstProfile=true;
			break;

		case Group:
			if(i->Param1.GetLen()&&i->Param2.GetLen())
				sprintf(tmp,"Group Profiles: Method=\"%s\"  Settings=\"%s\"",i->Param1(),i->Param2());
			else
			if(i->Param1.GetLen())
				sprintf(tmp,"Group Profiles: Method=\"%s\"  Current Settings",i->Param1());
			else
				strcpy(tmp,"Group Profiles: Current Method and Settings");
			Rec->WriteStr(tmp);
			if(i->Param1.GetLen())
				Session->SetCurrentGroupingMethod(i->Param1());
			if(i->Param2.GetLen())
				Session->SetCurrentGroupingMethodSettings(i->Param2());
			if(Groups)
			{
				GGrouping* algo=Session->GetCurrentGroupingMethod();
				algo->SetIdealGroups(Groups);
			}
			Session->GroupingProfiles(Rec,FirstGroup,AutoSave);
			if(!FirstGroup) FirstGroup=true;
			break;

		case LoadIdeal:
			if(!i->Param1.GetLen())
				throw GException("No file Defined");
			sprintf(tmp,"Load Ideal Groups from file '%s'",i->Param1());
			Rec->WriteStr(tmp);
			LoadGroups(i->Param1());
			break;
 
		case CreateIdeal:
			if(i->Param1.GetLen())
					sprintf(tmp,"Create Ideal Groups: Settings=\"%s\"",i->Param1());
				else
					strcpy(tmp,"Create Ideal Groups");
			Rec->WriteStr(tmp);
			if(i->Param1.GetLen())
				IdealMethod->SetSettings(i->Param1);
			IdealMethod->CreateJudgement(Parents,Groups,AutoSave);
			FirstGroup=FirstProfile=false;
			break;

		case MixIdeal:
			if(!Groups)
				throw GException("No Ideal Groups Defined");
			if(i->Param1.GetLen())
				sprintf(tmp,"Creating Mixed Groups: Settings=\"%s\"",i->Param1());
			else
				sprintf(tmp,"Creating Mixed Groups");
			Rec->WriteStr(tmp);
			GMixIdealGroups* mix;
			mix = new GMixIdealGroups(Session,Parents,Groups);
			if(i->Param1.GetLen())
				mix->SetSettings(i->Param1());
			mix->Run();
			delete mix;
			break;
		
        case Fdbks:
			if(i->Param1.GetLen())
				sprintf(tmp,"Create Feedbacks Cycle: Settings=\"%s\"",i->Param1());
			else
				strcpy(tmp,"Create Feedbacks Cycle");
			Rec->WriteStr(tmp);
			if(i->Param1.GetLen())
				FdbksMethod->SetSettings(i->Param1);
			FdbksMethod->Run(Parents,Groups,AutoSave);
			break;

		case CmpIdeal:
			if(!Groups)
				throw GException("No Ideal Groups Defined");
			strcpy(tmp,"Compare with Ideal Groups");
			Rec->WriteStr(tmp);
			CompMethod=new GCompareGrouping(Session,Groups);
			CompMethod->Compare(0);
			Precision=CompMethod->GetPrecision();
			Recall=CompMethod->GetRecall();
			Total=CompMethod->GetTotal();
			sprintf(tmp,"Recall: %f  -  Precision: %f  -  Total: %f",Recall,Precision,Total);
			Rec->WriteStr(tmp);
			if(OFile)
				(*OFile)<<TestName<<Recall<<Precision<<Total<<endl;
			if(GOFile)
				(*GOFile)<<Recall<<Precision<<Total<<endl;
			delete CompMethod;
			break;

		case StatProf:
			if(!Groups)
				throw GException("No Ideal Groups Defined");
			if(i->Param1.GetLen())
					sprintf(tmp,"Statistics on Profiles: Settings=\"%s\"",i->Param1());
				else
					strcpy(tmp,"Statistics on Profiles");
			Rec->WriteStr(tmp);
			ProfStats=new GStatSimSubProf(Session,Groups);
			if(i->Param1.GetLen())
				ProfStats->SetSettings(i->Param1);
			ProfStats->Run();
			sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  Rie: %f",
			        ProfStats->GetAvgIntra(),ProfStats->GetAvgInter(),ProfStats->GetAVGol(),ProfStats->GetRie());
			Rec->WriteStr(tmp);
			if(SOFile)
				(*SOFile)<<ProfStats->GetAvgIntra()<<ProfStats->GetAvgInter()<<ProfStats->GetAVGol()<<ProfStats->GetRie()<<endl;
			delete ProfStats;
			break;

		case StatDoc:
			if(!Groups)
				throw GException("No Ideal Groups Defined");
			if(i->Param1.GetLen())
					sprintf(tmp,"Statistics on Documents : Settings=\"%s\"",i->Param1());
				else
					strcpy(tmp,"Statistics on Documents");
			Rec->WriteStr(tmp);
			DocStats=new GStatSimDoc(Session);
			if(i->Param1.GetLen())
				DocStats->SetSettings(i->Param1);
			DocStats->Run();
			sprintf(tmp,"AVGintra: %f  -  AVGinter: %f  -  AVGol: %f  -  Rie: %f",
			        DocStats->GetAvgIntra(),DocStats->GetAvgInter(),DocStats->GetAVGol(),DocStats->GetRie());
			Rec->WriteStr(tmp);
			if(SOFile)
				(*SOFile)<<DocStats->GetAvgIntra()<<DocStats->GetAvgInter()<<DocStats->GetAVGol()<<DocStats->GetRie()<<endl;
			delete ProfStats;
			break;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionPrg::Exec(void) throw(GException)
{
	if(!Rec) return;
	for(Insts.Start();!Insts.End();Insts.Next())
		Run(Insts());
	if(OFile)
	{
		delete OFile;
		OFile=0;
	}
}


//-----------------------------------------------------------------------------
GALILEI::GSessionPrg::~GSessionPrg(void)
{
	if(Groups)
		delete Groups;
	if(OFile)
		delete OFile;
	if(IdealMethod)
		delete IdealMethod;
	if(FdbksMethod)
		delete FdbksMethod;
	if(Parents)
		delete Parents;
}
