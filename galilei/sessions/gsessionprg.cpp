/*

	GALILEI Research Project

	GSessionPrg.cpp

	Program to run on a Session - Implementation.

	(C) 2002 by P. Francq.

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
// include files for R Project
#include <rstd/rcursor.h>
using namespace RStd;


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
using namespace GALILEI;
using namespace RIO;



//-----------------------------------------------------------------------------
//
// InsType
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
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
GALILEI::GSessionPrg::GSessionPrg(RString f,GSession* s,GSlot* r) throw(bad_alloc,GException)
	: FileName(f), Session(s), Rec(r), InstTypes(20), Insts(40), OFile(0),
	  Groups(0), IdealMethod(0), FdbksMethod(0), Parents(0), AutoSave(false)
	
{
	RString l;
	char tmp[500];
	char* ptr;
	char* obj;
	char* name;
	char* param1;
	char* param2;
	InstType* t;

	// Init Part
	IdealMethod=new GIdealGroup(Session);
	FdbksMethod=new GGetFeedback(Session);

	// Instructions
	InstTypes.InsertPtr(new InstType("SetOutput",Output));
	InstTypes.InsertPtr(new InstType("SetAutoSave",SetAutoSave));
	InstTypes.InsertPtr(new InstType("Test",Test));
	InstTypes.InsertPtr(new InstType("Log",Log));
	InstTypes.InsertPtr(new InstType("ExecSql",Sql));
	InstTypes.InsertPtr(new InstType("ComputeProfiles",Comp));
	InstTypes.InsertPtr(new InstType("GroupProfiles",Group));
	InstTypes.InsertPtr(new InstType("CompareIdeal",CmpIdeal));
	InstTypes.InsertPtr(new InstType("CreateIdeal",CreateIdeal));
	InstTypes.InsertPtr(new InstType("LoadIdeal",LoadIdeal));
	InstTypes.InsertPtr(new InstType("FdbksCycle",Fdbks));

	// Read Program File
	RTextFile Prg(FileName);
	Prg.SetRem("#");
	while(!Prg.Eof())
	{
		// Read the line
		strcpy(tmp,Prg.GetLine());
		ptr=tmp;

		// Skip Spaces
		while((*ptr)&&(isspace(*ptr)))
			ptr++;

		// Read the Object
		obj=ptr;
		while((*ptr)&&((*ptr)!='.')&&((*ptr)!='='))
			ptr++;
		(*(ptr++))=0;
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
		t=InstTypes.GetPtr<const char*>(name);
		if(!t) continue;
		Insts.InsertPtr(new Inst(param1,param2,t->Type));
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
			OFile=new RTextFile(i->Param1,RTextFile::Create);
			(*OFile)<<"Sets\tRecall\tPrecision\tTotal"<<endl;
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

		case Comp:
			if(i->Param1.GetLen()&&i->Param2.GetLen())
				sprintf(tmp,"Compute Profiles: Method=\"%s\"  Settings=\"%s\"",i->Param1(),i->Param2());
			else
			if(i->Param1.GetLen())
				sprintf(tmp,"Compute Profiles: Method=\"%s\"  Current Settings",i->Param1());
			else
				strcpy(tmp,"Compute Profiles: Current Method and  Settings");
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
			GCompareGrouping Comp(Session,Groups);
			Comp.Compare(0);
			Precision=Comp.GetPrecision();
			Recall=Comp.GetRecall();
			Total=Comp.GetTotal();
			sprintf(tmp,"Recall: %lf  -  Precision: %lf  -  Total: %lf",Recall,Precision,Total);
			Rec->WriteStr(tmp);
			if(OFile)
				(*OFile)<<TestName<<"\t"<<Recall<<"\t"<<Precision<<"\t"<<Total<<endl;
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
