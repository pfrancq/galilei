/*

	GALILEI Research Project

	GStatsSims.cpp

	Groups Evaluation - Implementation.

	Copyright 2002-2004 by the Universit�Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for R Library
#include <rstd/rxmlstruct.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/glangmanager.h>
#include <infos/glang.h>
#include <sessions/gsession.h>
#include <groups/gsubject.h>
#include <groups/gsubjects.h>
#include <docs/gdoc.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include statistics files
#include <gstatsimelements.h>
#include <gstatsimdocs.h>
#include <gstatssims.h>
#include <gstatprofdoc.h>
#include <gstatsimprofgrp.h>



//------------------------------------------------------------------------------
//
// Instantiation of the templates
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Statistics between documents
class GStatSimDoc : public GStatSimElements<GDoc,R::RCursor<GDoc> >
{
public:
	GStatSimDoc(GSession* ses,R::RTextFile* f,bool g,bool l)
		: GStatSimElements<GDoc,R::RCursor<GDoc> >(ses,f,g,l) {}

	R::RCursor<GDoc> GetElementCursor(GLang* Lang)
	{ return(Session->GetDocsCursor(Lang));}

	bool SameSubject(GDoc* doc1,GDoc* doc2)
	{
		R::RCursor<GSubject> Subjects(doc1->GetSubjectCursor());
		for(Subjects.Start();!Subjects.End();Subjects.Next())
			if(doc2->IsFromSubject(Subjects()))
				return(true);
		return(false);
	}

	bool HasSubject(GDoc* doc)
	{return(doc->GetNbSubjects());}

	virtual void OverlapTopics(GDoc* doc,bool global)
	{
		R::RCursor<GSubject> Subjects=doc->GetSubjectCursor();
		for(Subjects.Start();!Subjects.End();Subjects.Next())
		{
			LocalStat* t=Sub.GetInsertPtr<GSubject*>(Subjects());
			if(global)
				t->OverlapG=true;
			else
				t->OverlapL=true;
		}
	}
};


//------------------------------------------------------------------------------
// Statistics between subprofiles
class GStatSimSubProf : public GStatSimElements<GSubProfile,RCursor<GSubProfile> >
{
public:
	GStatSimSubProf(GSession* ses,R::RTextFile* f,bool g,bool l)
		: GStatSimElements<GSubProfile,RCursor<GSubProfile> >(ses,f,g,l) {}

	RCursor<GSubProfile> GetElementCursor(GLang* Lang)
	{return(Session->GetSubProfilesCursor(Lang));}

	bool SameSubject(GSubProfile* sub1,GSubProfile* sub2)
	{return(sub1->GetSubject()==sub2->GetSubject());}

	bool HasSubject(GSubProfile* sub)
	{return(sub->GetSubject());}

	virtual void OverlapTopics(GSubProfile* sub,bool global)
	{
		LocalStat* t=Sub.GetInsertPtr<GSubject*>(sub->GetSubject());
		if(global)
			t->OverlapG=true;
		else
			t->OverlapL=true;
	}
};


//------------------------------------------------------------------------------
// Statistics between documents/groups
class GStatSimDocGrp : public GStatSimDocs<GGroup,R::RCursor<GGroup> >
{
public:

	GStatSimDocGrp(GSession* ses,R::RTextFile* f,bool g,bool l)
		: GStatSimDocs<GGroup,R::RCursor<GGroup> >(ses,f,g,l) {}

	R::RCursor<GGroup> GetElementCursor(GLang* Lang)
	{ return(Session->GetGroupsCursor(Lang));}

	bool HasSubject(GGroup* grp)
	{return(!grp->IsEmpty());}

	bool SameSubject(GGroup* grp,GDoc* doc)
	{
		// Suppose subject of the group is subject of the first subprofile
		// contained
		RCursor<GSubProfile> Subp=grp->GetSubProfilesCursor();
		Subp.Start();
		return(doc->IsFromSubject(Subp()->GetSubject()));
	}

	virtual void OverlapTopics(GGroup* grp,bool global)
	{
		// Suppose subject of the group is subject of the first subprofile
		// contained
		RCursor<GSubProfile> Subp=grp->GetSubProfilesCursor();
		Subp.Start();
		LocalStat* t=Sub.GetInsertPtr<GSubject*>(Subp()->GetSubject());
		if(global)
			t->OverlapG=true;
		else
			t->OverlapL=true;
	}
};


//------------------------------------------------------------------------------
// Statistics between documents/groups
class GStatSimDocProf : public GStatSimDocs<GSubProfile,RCursor<GSubProfile> >
{
public:

	GStatSimDocProf(GSession* ses,R::RTextFile* f,bool g,bool l)
		: GStatSimDocs<GSubProfile,RCursor<GSubProfile> >(ses,f,g,l) {}

	RCursor<GSubProfile> GetElementCursor(GLang* Lang)
	{return(Session->GetSubProfilesCursor(Lang));}

	bool HasSubject(GSubProfile* sub)
	{return(sub->GetSubject());}

	bool SameSubject(GSubProfile* sub,GDoc* doc)
	{
		return(doc->IsFromSubject(sub->GetSubject()));
	}

	virtual void OverlapTopics(GSubProfile* sub,bool global)
	{
		LocalStat* t=Sub.GetInsertPtr<GSubject*>(sub->GetSubject());
		if(global)
			t->OverlapG=true;
		else
			t->OverlapL=true;
	}
};



//------------------------------------------------------------------------------
//
//  class GStatsSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStatsSims::GStatsSims(GFactoryStatsCalc* fac) throw(bad_alloc)
	: GStatsCalc(fac)
{
}


//------------------------------------------------------------------------------
void GStatsSims::ApplyConfig(void)
{
	Docs=Factory->GetBool("Docs");
	ProfDoc=Factory->GetBool("ProfDoc");
	GroupDoc=Factory->GetBool("GroupDoc");
	Profiles=Factory->GetBool("Profiles");
	SameDocProf=Factory->GetBool("SameDocProf");
	GroupProf=Factory->GetBool("GroupProf");
	File=Factory->GetBool("File");
	Name=Factory->GetString("Name");
	WithFactors=Factory->GetBool("WithFactors");
	WithoutFactors=Factory->GetBool("WithoutFactors");
}


//------------------------------------------------------------------------------
void GStatsSims::Connect(GSession* session) throw(GException)
{
	GStatsCalc::Connect(session);
}


//------------------------------------------------------------------------------
void GStatsSims::Disconnect(GSession* session) throw(GException)
{
	GStatsCalc::Disconnect(session);
}


//------------------------------------------------------------------------------
void GStatsSims::Compute(R::RXMLStruct* xml,R::RXMLTag& res) throw(GException)
{
	RXMLTag* tag;
	RXMLTag* tag2;
	RString str;
	RTextFile* Details=0;

	// Init Main XML Tag
	tag=new RXMLTag(Factory->GetName());
	xml->AddTag(&res,tag);

	// Create Details File if necessary
	if(File)
	{
		try
		{
			Details=new RTextFile(Name);
			Details->Open(R::Create);
		}
		catch(...)
		{
			Details=0;
		}
	}

	// Compute Statistics
	if(Docs)
	{
		tag2=new RXMLTag("Documents");
		xml->AddTag(tag,tag2);
		GStatSimDoc Stat(Session,Details,WithFactors,WithoutFactors);
		Stat.Run(this,xml,tag2);
	}
	if(ProfDoc)
	{
		tag2=new RXMLTag("Documents-Profiles");
		xml->AddTag(tag,tag2);
		GStatSimDocProf Stat(Session,Details,WithFactors,WithoutFactors);
		Stat.Run(this,xml,tag2);
	}
	if(GroupDoc)
	{
		tag2=new RXMLTag("Documents-Groups");
		xml->AddTag(tag,tag2);
		GStatSimDocGrp Stat(Session,Details,WithFactors,WithoutFactors);
		Stat.Run(this,xml,tag2);
	}
	if(Profiles)
	{
		tag2=new RXMLTag("Profiles");
		xml->AddTag(tag,tag2);
		GStatSimSubProf Stat(Session,Details,WithFactors,WithoutFactors);
		Stat.Run(this,xml,tag2);
	}
	if(SameDocProf)
	{
		tag2=new RXMLTag("Profiles/Common Documents");
		xml->AddTag(tag,tag2);
		GStatProfDoc Stat(Session,Details);
		Stat.Run(this,xml,tag2);
	}
	if(GroupProf)
	{
		tag2=new RXMLTag("Profiles/Groups");
		xml->AddTag(tag,tag2);
		GStatSimProfGrp Stat(Session,Session->GetSubjects()->GetIdealGroups(),Details,WithFactors,WithoutFactors);
		Stat.Run(this,xml,tag2);
	}

	// Desallocate Details File if necessary
	if(Details)
		delete Details;
}


//------------------------------------------------------------------------------
void GStatsSims::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamBool("Docs",false));
	params->InsertPtr(new GParamBool("ProfDoc",false));
	params->InsertPtr(new GParamBool("GroupDoc",false));
	params->InsertPtr(new GParamBool("Profiles",false));
	params->InsertPtr(new GParamBool("SameDocProf",false));
	params->InsertPtr(new GParamBool("GroupProf",false));
	params->InsertPtr(new GParamBool("File",false));
	params->InsertPtr(new GParamString("Name",""));
	params->InsertPtr(new GParamBool("WithFactors",true));
	params->InsertPtr(new GParamBool("WithoutFactors",true));
}


//------------------------------------------------------------------------------
GStatsSims::~GStatsSims(void)
{
}


//------------------------------------------------------------------------------
CREATE_STATSCALC_FACTORY("Similarity Statistics",GStatsSims)
