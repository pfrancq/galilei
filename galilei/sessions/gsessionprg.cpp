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
#include <groups/ggroup.h>
#include <groups/ggrouping.h>
#include <groups/gcomparegrouping.h>
#include <profiles/gprofilecalc.h>
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
// Ins
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GSessionPrg::Inst
{
public:
	RString Param1;
	RString Param2;
	tInst Type;

	Inst(const char* p1,const char* p2,tInst t) : Param1(p1), Type(t) {if(p2) Param2=p2;}
	int Compare(const Inst* t) const {return(Type-t->Type);}
	int Compare(const tInst t) const {return(Type-t);}
};



//-----------------------------------------------------------------------------
//
// Proc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GSessionPrg::Proc : public RContainer<GALILEI::GSessionPrg::Inst,unsigned int,true,false>
{
public:
	RString Name;

	Proc(const char* n) : RContainer<Inst,unsigned int,true,false>(10,5), Name(n) {}
	int Compare(const Proc*) const {return(-1);}
	int Compare(const Proc&) const {return(-1);}
};



//-----------------------------------------------------------------------------
//
// GSessionPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// GSessionPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSessionPrg::GSessionPrg(RString f,GSession* s,GSlot* r) throw(bad_alloc,GException)
	: FileName(f), Session(s), Rec(r), InstTypes(20), Procs(20),
	  OutputName("Coucou"), OFile(0), Groups(0)
	
{
	RString l;
	char tmp[500];
	char* ptr;
	char* obj;
	char* name;
	char* param1;
	char* param2;
	InstType* t;
	Proc* p;

	p=0;
	InstTypes.InsertPtr(new InstType("Output",Output));
	InstTypes.InsertPtr(new InstType("Test",Test));
	InstTypes.InsertPtr(new InstType("Log",Log));
	InstTypes.InsertPtr(new InstType("Sql",Sql));
	InstTypes.InsertPtr(new InstType("ComputeProfiles",Comp));
	InstTypes.InsertPtr(new InstType("GroupProfiles",Group));
	InstTypes.InsertPtr(new InstType("CompareIdeal",Ideal));
	RTextFile Prg(FileName);
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
		if(!strcmp(obj,"Output"))
		{
			OutputName=param1;
			OFile=new RTextFile(OutputName,RTextFile::Create);
				(*OFile)<<"Sets\tRecall\tPrecision\tTotal"<<endl;
		}
		else
			if(!strcmp(name,"Test"))
			{
				Procs.InsertPtr(p=new Proc(obj));
			}
			else
			{
				t=InstTypes.GetPtr<const char*>(name);
				if((!t)||(!p)) continue;
				p->InsertPtr(new Inst(param1,param2,t->Type));
			}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionPrg::LoadGroups(const char* filename)
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
		groups->InsertPtr(group=new GGroup(i,lang));
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
void GALILEI::GSessionPrg::Run(tInst t,Proc* p) throw(GException)
{
	Inst* i=p->GetPtr<const tInst>(t);
	char tmp[300];

	if(!i) return;
	switch(t)
	{
		case Test:
			break;
		case Log:
			sprintf(tmp,"Create Log file '%s'",i->Param1());
			Rec->WriteStr(tmp);
			break;
		case Sql:
			sprintf(tmp,"Execute Sql file '%s'",i->Param1());
			Rec->WriteStr(tmp);
			break;
		case Comp:
			sprintf(tmp,"Compute Profiles: Method=\"%s\"  Settings=\"%s\"",i->Param1(),i->Param2());
			Rec->WriteStr(tmp);
			Session->SetCurrentComputingMethod(i->Param1());
			Session->SetCurrentComputingMethodSettings(i->Param2());
 			Session->CalcProfiles(Rec,false);
			break;
		case Group:
			sprintf(tmp,"Group Profiles: Method=\"%s\"  Settings=\"%s\"",i->Param1(),i->Param2());
			Rec->WriteStr(tmp);
			Session->SetCurrentGroupingMethod(i->Param1());
			Session->SetCurrentGroupingMethodSettings(i->Param2());
 			Session->GroupingProfiles(Rec,false);
			break;
		case Ideal:
			sprintf(tmp,"Compare with ideal groupement from file '%s'",i->Param1());
			Rec->WriteStr(tmp);
			LoadGroups(i->Param1());
			GCompareGrouping Comp(Session,Groups);
			Comp.Compare(0);
			Precision=Comp.GetPrecision();
			Recall=Comp.GetRecall();
			Total=Comp.GetTotal();
			sprintf(tmp,"Recall: %lf  -  Precision: %lf  -  Total: %lf",Recall,Precision,Total);
			Rec->WriteStr(tmp);
			break;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionPrg::Exec(void) throw(GException)
{
	char tmp[300];

	if(!Rec) return;
	sprintf(tmp,"Create Output file '%s'",OutputName());
	Rec->WriteStr(tmp);
	Rec->WriteStr("");
	for(Procs.Start();!Procs.End();Procs.Next())
	{
		sprintf(tmp,"Create Test: %s",Procs()->Name());
		Rec->WriteStr(tmp);
		Run(Log,Procs());
		Run(Sql,Procs());
		Run(Comp,Procs());
		Run(Group,Procs());
		Run(Ideal,Procs());
		sprintf(tmp,"Write results of Test '%s'",Procs()->Name());
		Rec->WriteStr(tmp);
		if(OFile)
			(*OFile)<<Procs()->Name<<"\t"<<Recall<<"\t"<<Precision<<"\t"<<Total<<endl;
		Rec->WriteStr("");
	}
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
}
