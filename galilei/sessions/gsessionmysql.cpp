/*

	GALILEI Research Project

	GSessionMySQL.cpp

	GALILEI Session using a MySQL Database - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#include <ctype.h>



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcursor.h>
#include <rstd/rtextfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsessionmysql.h>
#include <langs/glang.h>
#include <langs/gdict.h>
#include <langs/gdicts.h>
#include <langs/gwordlist.h>
#include <infos/giwordlist.h>
#include <infos/giwordweight.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofdoc.h>
#include <docs/gdocvector.h>
#include <docs/gdocs.h>
#include <docs/glink.h>
#include <groups/ggroupvector.h>
#include <historic/ggrouphistory.h>
#include <groups/ggroups.h>
#include <historic/ggroupshistory.h>
#include <tests/ggroupsevaluate.h>
#include <tests/ggroupevaluatedoc.h>
#include <groups/gsubjecttree.h>
#include <filters/gmimefilter.h>
#include <filters/gurlmanager.h>
#include <groups/gsubject.h>
#include <groups/ggroup.h>
#include <groups/gchromoir.h>
#include <groups/ginstir.h>
#include <groups/ggroupir.h>
#include <groups/gobjir.h>
#include <groups/ggroupcalcgravitation.h>
#include <postgroups/gpointofview.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GSessionMySQL
//
//-----------------------------------------------------------------------------

#define Ins_spec 1

//-----------------------------------------------------------------------------
// Static variable
const char GALILEI::GSessionMySQL::SQLNULL[5]="NULL";


//-----------------------------------------------------------------------------
GALILEI::GSessionMySQL::GSessionMySQL(const char* host,const char* user,const char* pwd,const char* db,GURLManager* mng) throw(bad_alloc,GException,RMySQLError)
	: RDb(host,user,pwd,db),
	  GSession(GetCount("htmls"),GetCount("users"),GetCount("profiles"),GetCount("htmlsbyprofiles"),GetCount("groups"),mng)
{
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSessionMySQL::GetCount(const char* tbl)
{
	char sSql[100];
	const char* c;

	sprintf(sSql,"SELECT COUNT(*) FROM %s",tbl);
	RQuery count(this,sSql);
	count.Start();
	c=count[0];
	if(!c) return(0);
	return(atoi(c));
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSessionMySQL::GetMax(const char* tbl,const char* fld)
{
	char sSql[100];
	const char* c;

	sprintf(sSql,"SELECT MAX(%s) FROM %s",fld,tbl);
	RQuery count(this,sSql);
	count.Start();
	c=count[0];
	if(!c) return(0);
	return(atoi(c));
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSessionMySQL::GetDateToMySQL(const RDate* d,char* tmp)
{
	if(d)
		sprintf(tmp,"'%u-%u-%u'",d->GetYear(),d->GetMonth(),d->GetDay());
	else
		strcpy(tmp,"null");
	return(tmp);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSessionMySQL::ValidSQLValue(const char* val,char* tmp)
{
	const char* ptr1=val;
	char* ptr2=tmp;

	(*(ptr2++))='\'';
	while(*ptr1)
	{
		if((*ptr1)=='\'')
			(*(ptr2++))='\'';
		if((*ptr1)=='\\')
			(*(ptr2++))='\\';
		(*(ptr2++))=(*(ptr1++));
	}
	(*(ptr2++))='\'';
	(*ptr2)=0;
	return(tmp);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSessionMySQL::GetDicNextId(const char* word,const GDict* dict)
{

	// preliminary traitement of special words and composite like insert -> reinsert ...
	#if Ins_spec
		if (strstr(word,"insert"))
		{
			return 1;
		}
		if (strstr(word,"delete"))
		{
			return 2;
		}
		if (strstr(word,"update"))
		{
			return 3;
		}
		if (strstr(word,"select"))
		{
			return 4;
		}
	#endif

	char sSql[600];
	char tmp[100];
	// Verify that the word didn't already exist.
	sprintf(sSql,"SELECT kwdid FROM %skwds WHERE kwd='%s'",dict->GetLang()->GetCode(),word);
	RQuery find(this,sSql);
	if(find.GetNbRows())
	{
		find.Start();
		return(strtoul(find[0],0,10));
	}

	// Get the existing word for derivate of 'insert' 'delete' 'update' 'delete'

	// Insert the new word
	sprintf(sSql,"INSERT INTO %skwds(kwd) VALUES(%s)",dict->GetLang()->GetCode(),ValidSQLValue(word,tmp));
	RQuery insert(this,sSql);

	// Get the next id
	sprintf(sSql,"SELECT kwdid FROM %skwds WHERE kwdid=LAST_INSERT_ID()",dict->GetLang()->GetCode());
	RQuery getinsert(this,sSql);
	getinsert.Start();
	return(strtoul(getinsert[0],0,10));
}



//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadDic(const char* code,bool s) throw(bad_alloc,GException)
{
	GDict* dict;
	unsigned int MaxCount=100;
	unsigned int MaxId=0;
	char sSql[100];
	char tbl[20];

	// Construct the table name
	if(s)
		sprintf(tbl,"%sstopkwds",code);
	else
		sprintf(tbl,"%skwds",code);

	// Search the values to initialise the dictionnary
	for(char i='a';i<='z';i++)
	{
		sprintf(sSql,"SELECT COUNT(*) FROM %s WHERE kwd LIKE '%c%%'",tbl,i);
		RQuery count(this,sSql);
		count.Start();
		if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
	}
	if(MaxCount==0) MaxCount=2000;
	MaxId=GetMax(tbl,"kwdid");
	if(!MaxId)
		MaxId=2000;

	// Create and insert the dictionnary
	if(s)
		Stops->InsertPtr(dict=new GDict(this,tbl,"Stop List",GetLang(code),MaxId,MaxCount,true));
	else
	{
		Dics->InsertPtr(dict=new GDict(this,tbl,"Dictionnary",GetLang(code),MaxId,MaxCount,false));
		GetLang(code)->AssignDict(dict);
	}

	// Load the dictionnary from the database
	sprintf(sSql,"SELECT kwdid, kwd  FROM %s",tbl);
	RQuery dicts (this, sSql);
	for(dicts.Start();!dicts.End();dicts.Next())
		dict->Put(atoi(dicts[0]), dicts[1]);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSessionMySQL::LoadWord(const unsigned int id,const char* code)
{
	char sSql[100];

	sprintf(sSql,"SELECT kwd FROM %skwds WHERE kwdid=%i",code,id);
	RQuery w(this,sSql);
	if(!w.GetNbRows())
		return(0);
	w.Start();
	return(w[0]);
}


//-----------------------------------------------------------------------------
GProfile* GALILEI::GSessionMySQL::NewProfile(GUser* usr,const char* desc) throw(bad_alloc,GException)
{
	char sSql[500];
	char sDes[100];
	GProfile* prof;
	unsigned int id;
	GLangCursor Langs;

	sprintf(sSql,"INSERT INTO profiles(userid,description) VALUES(%u,%s)",
		usr->GetId(),ValidSQLValue(desc,sDes));
	RQuery insertprof(this,sSql);

	// Get Id and updated
	sprintf(sSql,"SELECT profileid FROM profiles WHERE profileid=LAST_INSERT_ID()");
	RQuery selectprofile(this,sSql);
	selectprofile.Start();
	id=strtoul(selectprofile[0],0,10);
	InsertProfile(prof=new GProfile(usr,id,desc,true,GetNbLangs()));

	// Construct SubProfiles
	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		sprintf(sSql,"INSERT INTO subprofiles(profileid,langid, state) VALUES(%u,'%s', %u)",id,Langs()->GetCode(), static_cast<int>(osCreated));
		RQuery insertsub(this,sSql);
		sprintf(sSql,"SELECT subprofileid, state from subprofiles WHERE subprofileid=LAST_INSERT_ID())");
		RQuery selectsub(this,sSql);
		selectsub.Start();
		InsertSubProfile(new GSubProfileVector(prof,strtoul(selectsub[0],0,10),Langs(),0,0,(static_cast<tObjState>(atoi(selectsub[1]))),0));
	}

	// Return new created profile
	return(prof);
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GSessionMySQL::NewSubProfile(GProfile* prof,GLang* lang) throw(bad_alloc,GException)
{
	char sSql[500];
	GSubProfile* subprof;
	unsigned int id;

	sprintf(sSql,"INSERT INTO subprofiles(profileid,langid,state) VALUES(%u,'%s',%u)",prof->GetId(),lang->GetCode(),osCreated);
	RQuery insertsubprof(this,sSql);

	// Get Id and updated
	sprintf(sSql,"SELECT subprofileid FROM subprofiles WHERE subprofileid=LAST_INSERT_ID()");
	RQuery selectsubprofile(this,sSql);
	selectsubprofile.Start();
	id=strtoul(selectsubprofile[0],0,10);
	InsertSubProfile(subprof=new GSubProfileVector(prof,id,lang,0,0,osCreated,0));
	prof->InsertPtr(subprof);

	// Return new created subprofile
	return(subprof);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveSubProfile(GSubProfile* sub) throw(GException)
{
	char sSql[200];
	char scomputed[15];
	GIWordWeightCursor Cur;
	const char* l;


	//update the subprofile;
	sprintf(sSql,"UPDATE subprofiles SET state=%u, calculated=%s WHERE subprofileid=%u",
			 static_cast<int>(sub->GetState()), GetDateToMySQL(sub->GetComputed(),scomputed), sub->GetId());
	RQuery updatesubprof(this,sSql);

	// Delete keywords
	l=sub->GetLang()->GetCode();

	sprintf(sSql,"DELETE FROM %ssubprofilesbykwds WHERE subprofileid=%u",l,sub->GetId());
	RQuery deletekwds(this,sSql);

	Cur=((GSubProfileVector*)sub)->GetVectorCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sprintf(sSql,"INSERT INTO  %ssubprofilesbykwds(subprofileid,kwdid,weight) VALUES (%u,%u,'%e')",
		              l,sub->GetId(),Cur()->GetId(),Cur()->GetWeight());
		RQuery insertkwds(this,sSql);
	}

	sub->SetState(osUpToDate);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveSubProfileInHistory(GSubProfile* sub,unsigned int historicid) throw(GException)
{
	char sSql[200];
	GIWordWeightCursor Cur;

	//save subprofiles
	Cur=((GSubProfileVector*)sub)->GetVectorCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		cout <<"courage david"<<endl;
		sprintf(sSql,"INSERT INTO  %shistoricsubprofiles(historicid,subprofileid,kwdid,weight) VALUES (%u, %u,%u,'%e')",
		              sub->GetLang()->GetCode() ,historicid,sub->GetId(),Cur()->GetId(),Cur()->GetWeight());
		cout <<sSql<<endl;
		RQuery insertkwds(this,sSql);
	}
}

//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveProfile(GProfile* prof) throw(GException)
{
	char sSql[500];
	char sname[200];
	unsigned int profid;
	unsigned int social;
	GSubProfileCursor CurSub=prof->GetSubProfilesCursor();

	// Init
	profid=prof->GetId();

	// Save the Subprofile
	for(CurSub.Start();!CurSub.End();CurSub.Next())
	{
		if (CurSub()->GetState()==osUpdated)
			SaveSubProfile(CurSub());
	}


	// Update profiles
	if (prof->IsSocial()) social=1;
	else social =0;
	sprintf(sSql,"UPDATE profiles SET description=%s,social=%u WHERE profileid=%u",
	        ValidSQLValue(prof->GetName(),sname),social,profid);
	RQuery updateprof(this,sSql);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadUsers() throw(bad_alloc,GException)
{
	char sSql[100];
	GUser* usr;
	GProfile* prof;
	GLang* lang;
	GSubProfile* sub;
	unsigned int userid,profileid,subid;
	GGroup* grp;
	GLangCursor Langs;
	bool Social;
	#if GALILEITEST
		GSubject* s;
	#endif

	// Go through the users
	try
	{
		RQuery users(this, "SELECT userid,user,fullname FROM users");
		for(users.Start();!users.End();users.Next())
		{
			userid=atoi(users[0]);
			#if GALILEITEST
				sprintf(sSql,"SELECT profileid,description,social,topicid FROM profiles WHERE userid=%u",userid);
			#else
				sprintf(sSql,"SELECT profileid,description,social FROM profiles WHERE userid=%u",userid);
			#endif
			RQuery profiles(this,sSql);
			InsertUser(usr=new GUser(userid,users[1],users[2],profiles.GetNbRows()));
			for(profiles.Start();!profiles.End();profiles.Next())
			{
				profileid=atoi(profiles[0]);
				#if GALILEITEST
					s=Subjects.GetPtr<unsigned int>(atoi(profiles[3]));
				#endif
				Social=false;
				if(atoi(profiles[2])==1) Social=true;
				InsertProfile(prof=new GProfile(usr,profileid,profiles[1],Social,GetNbLangs()));
				prof->SetSubject(s);
				sprintf(sSql,"SELECT subprofileid,langid,attached,groupid, state, calculated FROM subprofiles WHERE profileid=%u",profileid);
				RQuery subprofil (this,sSql);
				for(subprofil.Start();!subprofil.End();subprofil.Next())
				{
					lang=GetLang(subprofil[1]);
					subid=atoi(subprofil[0]);
					if(lang)
						grp=Groups.GetPtr<const GLang*>(lang)->GetPtr<const unsigned int>(atoi(subprofil[3]));
					else
						grp=0;
					InsertSubProfile(sub=new GSubProfileVector(prof,subid,lang,grp,subprofil[2], (static_cast<tObjState>(atoi(subprofil[4]))), subprofil[5]));
					#if GALILEITEST
						if(sub->GetLang()==s->GetLang())
						{
							sub->SetSubject(s);
							s->InsertSubProfile(sub);
						}
					#endif
				}   
			}
		}

		// Load the subprofile's description
		Langs=GetLangsCursor();
		for(Langs.Start();!Langs.End();Langs.Next())
		{
			sprintf(sSql,"SELECT subprofileid,kwdid,Weight FROM %ssubprofilesbykwds",Langs()->GetCode());
			RQuery sel(this,sSql);
			for(sel.Start();!sel.End();sel.Next())
			{
				sub=GetSubProfile(atoi(sel[0]),Langs());
				if(sub)
				{
					if(GetDic(Langs())->GetElement(atoi(sel[1]))->GetType()==tWordList)
						((GSubProfileVector*)sub)->AddWordList(atoi(sel[1]),atof(sel[2]));
					else
						((GSubProfileVector*)sub)->AddWord(atoi(sel[1]),atof(sel[2]));
				}
			}
		}

		// Update References of the loaded subprofiles.
		for(Langs.Start();!Langs.End();Langs.Next())
		{
			GSubProfileCursor SubProfiles=GetSubProfilesCursor(Langs());
			for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
				((GSubProfileVector*)SubProfiles())->UpdateRefs();
		}
		// Load the ideal Groups.
		LoadIdealGroupment();
	}
	catch(RMySQLError& e)
	{
		throw GException(e.GetError());
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadIdealGroupment()
{
	GGroups* groups;
	GGroup* group;
	GLangCursor Langs;
	char sSql[100];
	GSubProfile* subp;

	IdealGroups->Clear();

	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		IdealGroups->InsertPtr(groups=new GGroups(Langs()));
		sprintf(sSql,"SELECT DISTINCT(groupid) FROM idealgroup WHERE langid='%s'",Langs()->GetCode());
		RQuery sel(this,sSql);
		for(sel.Start();!sel.End();sel.Next())
		{
 			groups->InsertPtr(group=new GGroupVector(atoi(sel[0]),Langs()));
			sprintf(sSql,"SELECT profileid FROM idealgroup where groupid=%u",atoi(sel[0]));
			RQuery sub(this,sSql);
			for(sub.Start();!sub.End();sub.Next())
			{
				subp=GetProfile(atoi(sub[0]))->GetSubProfile(Langs());
				if(subp)
					group->InsertPtr(subp);
			}
			if(!group->NbPtr)
				groups->DeletePtr(group);
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveIdealGroupment(RContainer<GGroups,unsigned int,true,true>* idealgroup)
{
	GGroups* groups;
	GGroup* group;
	GLangCursor Langs;
	char sSql[100];
	
	sprintf(sSql,"DELETE from idealgroup");
	RQuery del(this,sSql);

	Langs=GetLangsCursor();
	for(idealgroup->Start();!idealgroup->End();idealgroup->Next())
	{
		groups=(*idealgroup)();
		for(groups->Start();!groups->End();groups->Next())
		{
			group=(*groups)();
			for(group->Start();!group->End();group->Next())
			{
				sprintf(sSql,"INSERT INTO idealgroup(profileid,langid,groupid) VALUES(%u,'%s',%u)",(*group)()->GetProfile()->GetId(),groups->GetLang()->GetCode(),group->GetId());
				RQuery insert(this,sSql);
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadSubjectTree()
{
	char sSql[200];
	GSubject* subject;
	GSubject* subsubject;

	Subjects.Clear();
	RQuery sub(this,"SELECT topicid,name,used,langid FROM topics WHERE parent=0");
	for(sub.Start();!sub.End();sub.Next())
	{
		Subjects.AddNode(0,subject=new GSubject(atoi(sub[0]),sub[1],GetLang(sub[3]),atoi(sub[2])));
		sprintf(sSql,"SELECT topicid,name,used,langid FROM topics WHERE parent=%u",atoi(sub[0]));
		RQuery subsub(this,sSql);
		for(subsub.Start();!subsub.End();subsub.Next())
			Subjects.AddNode(subject,subsubject=new GSubject(atoi(subsub[0]),subsub[1],GetLang(subsub[3]),atoi(subsub[2])));
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadFdbks() throw(bad_alloc,GException)
{
	char sSql[200];
	GProfile* prof;
	GUser* usr;
	GDoc* doc;
	tDocJudgement jug;

	sprintf(sSql,"SELECT htmlid, judgement, profiles.profileid, userid, when2 FROM htmlsbyprofiles,profiles WHERE profiles.profileid=htmlsbyprofiles.profileid");
	RQuery fdbks(this,sSql);
	for(fdbks.Start();!fdbks.End();fdbks.Next())
	{
		usr=GetUser(atoi(fdbks[3]));
		if(!usr) continue;
		prof=usr->GetPtr<unsigned int>(atoi(fdbks[2]));
		if(!prof) continue;
		doc=GetDoc(atoi(fdbks[0]));
		if(!doc) continue;

		switch(fdbks[1][0])
		{
			case 'O':
				jug=djOK;
				break;
			case 'N':
				jug=djNav;
				break;
			case 'K':
				jug=djKO;
				break;
			case 'H':
				jug= djOutScope;
				break;
			default:
				jug=djUnknow;
				break;
		}
		switch(fdbks[1][1])
		{
			case 'H':
				jug = tDocJudgement(jug | djHub);
				break;
			case 'A':
				jug = tDocJudgement(jug | djAutority);
				break;
			//case 'U':
				//break;
			default:
				break;
		}
		InsertFdbk(prof,doc,jug,fdbks[4]);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadDocs(void) throw(bad_alloc,GException)
{
	GDocVector* doc;
	GLang* lang;
	int docid;
	char sSql[100];
	GLangCursor Langs;
	GDoc* d;
	GSubject* s;

	sprintf(sSql,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed,n,ndiff,v,vdiff FROM htmls");
	RQuery quer (this,sSql);
	for(quer.Start();!quer.End();quer.Next())
	{
		docid=atoi(quer[0]);
		lang=GetLang(quer[4]);
		InsertDoc(doc=new GDocVector(quer[1],quer[2],docid,lang,Mng->GetMIMEType(quer[3]),quer[5],quer[6],atoi(quer[7]),atoi(quer[8]),atoi(quer[9]),atoi(quer[10]),atoi(quer[11])));
	}

	// Load the links of the document loaded.
	sprintf(sSql,"SELECT htmlid,linkid,occurs FROM htmlsbylinks");
	RQuery querLinks (this,sSql);
	for (querLinks.Start(); !querLinks.End() ; querLinks.Next())
	{
		GetDoc(atoi(querLinks[0]) )->InsertLink(GetDoc(atoi( querLinks[1]) ), atoi(querLinks[2]));
	}

	// Load the document's description
	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		sprintf(sSql,"SELECT htmlid,kwdid,occurs FROM %shtmlsbykwds",Langs()->GetCode());
		RQuery sel(this,sSql);
		for(sel.Start();!sel.End();sel.Next())
		{
			doc=dynamic_cast<GDocVector*>(GetDoc(atoi(sel[0])));
			if(doc)
			{
				if(GetDic(Langs())->GetElement(atoi(sel[1]))->GetType()==tWordList)
					doc->AddWordList(atoi(sel[1]),atof(sel[2]));
				else
					doc->AddWord(atoi(sel[1]),atof(sel[2]));
			}
		}
	}

	//  Make Link between documents and topics
	RQuery subdocs(this,"SELECT htmlid,topicid FROM topicsbyhtmls");
	for(subdocs.Start();!subdocs.End();subdocs.Next())
	{
		d=GetDoc(atoi(subdocs[0]));
		if(!d) continue;
		s=Subjects.GetPtr<const unsigned int>(atoi(subdocs[1]));
		if(!s) continue;
		s->InsertDoc(d);
		d->InsertSubject(s);
	}

	// Update References of the loaded documents.
	GDocCursor Docs=GetDocsCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
		dynamic_cast<GDocVector*>(Docs())->UpdateRefs();
}


//-----------------------------------------------------------------------------
GDoc* GALILEI::GSessionMySQL::NewDoc(const char* url,const char* name,const char* mime) throw(GException)
{
	char sSql[1000];
	char surl[1000];
	char sname[1000];
	GDoc *doc;

	// Insert it
	sprintf(sSql,"INSERT INTO htmls(html,title,mimetype,updated) VALUES(%s,%s,'%s',CURDATE())",
	        ValidSQLValue(url,surl),ValidSQLValue(name,sname),mime);
	RQuery insertdoc(this,sSql);

	// Get Id and updated
	sprintf(sSql,"SELECT htmlid,updated FROM htmls WHERE htmlid=LAST_INSERT_ID()");
	RQuery selectdoc(this,sSql);
	selectdoc.Start();
	doc=new GDocVector(url,name,strtoul(selectdoc[0],0,10),0,Mng->GetMIMEType(mime),selectdoc[1],0,0,0,0,0,0);
//	InsertDoc(doc);
	return(doc);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveFdbks(void) throw(GException)
{
	char sSql[500];
	//char j;
	char *j = new char[2];

	// Clear the all feedbacks
	sprintf(sSql,"DELETE FROM htmlsbyprofiles");
	RQuery delete1(this,sSql);

	// Reinsert all the feedbacks
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		switch(Fdbks()->GetFdbk() & djMaskJudg)
		{
			case djOK:
				j[0]='O';
				break;
			case djNav:
				j[0]='N';
				break;
			case djOutScope:
				j[0]='H';
				break;
			case djKO:
				j[0]='K';
				break;
			default:
				throw GException("No Valid Judgement");
		}
		switch(Fdbks()->GetFdbk() & djMaskHubAuto)
		{
			case djHub:
				j[1]='H';
				break;
			case djAutority:
				j[1]='A';
				break;
			default :
				j[1]='U';
				break;
		}
		sprintf(sSql,"INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2) VALUES(%u,'%s',%u,CURDATE())",
		        Fdbks()->GetDoc()->GetId(),j,Fdbks()->GetProfile()->GetId());
		RQuery fdbks(this,sSql);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveDoc(GDoc* doc) throw(GException)
{
	char sSql[1000];
	const char* l=0;
	unsigned int id;
	GMIMEFilter* f;
	id=doc->GetId();
	const char* fn;
	char slang[5];
	char smime[50];
	char surl[200];
	char sname[200];
	char supdated[15];
	char scomputed[15];
	GIWordWeightCursor Words;
	GLinkCursor lcur;

	// Delete keywords
	if(doc->GetLang())
	{
		l=doc->GetLang()->GetCode();
		sprintf(sSql,"DELETE FROM %shtmlsbykwds WHERE htmlid=%u",l,id);
		RQuery deletekwds(this,sSql);
		Words=dynamic_cast<GDocVector*>(doc)->GetWordWeightCursor();
		for(Words.Start();!Words.End();Words.Next())
		{
			sprintf(sSql,"INSERT INTO %shtmlsbykwds(htmlid,kwdid,occurs) VALUES (%u,%u,%f)",l,id,Words()->GetId(),Words()->GetWeight());
			RQuery insertkwds(this,sSql);
		}
		l=ValidSQLValue(l,slang);
	}
	else
	{
		l=SQLNULL;
	}

	// Update document
	f=doc->GetMIMEType();
	if(f)
		fn=ValidSQLValue(f->GetName(),smime);
	else
		fn=SQLNULL;
	sprintf(sSql,"UPDATE htmls SET "
	             "html=%s,title=%s,mimetype=%s,langid=%s,"
	             "updated=%s,calculated=%s,"
	             "n=%u,ndiff=%u,v=%u,vdiff=%u,failed=%u WHERE htmlid=%u",
	             ValidSQLValue(doc->GetURL(),surl),ValidSQLValue(doc->GetName(),sname),fn,l,
	             GetDateToMySQL(doc->GetUpdated(),supdated),GetDateToMySQL(doc->GetComputed(),scomputed),
	             doc->GetN(),doc->GetNdiff(),doc->GetV(),doc->GetVdiff(),doc->GetFailed(),id);
	RQuery updatedoc(this,sSql);

	// Update links to others documents
	sprintf(sSql,"DELETE FROM htmlsbylinks WHERE htmlid=%u",id);
	RQuery deletelinks(this,sSql);
	lcur= doc->GetLinkCursor();
	for ( lcur.Start(); ! lcur.End(); lcur.Next())
	{
		sprintf(sSql,"INSERT INTO htmlsbylinks(htmlid,linkid,occurs) VALUES (%u,%u,%u)",id,lcur()->GetId(),lcur()->GetOccurs() );
		RQuery insertkwds(this,sSql);
	}
	
	
}

//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveWordsGroups(GDict* dict) throw(GException)
{

	GWordList *tmp;
	char sSql[600];
	sprintf(sSql,"DELETE from idealgroup");
	RQuery del(this,sSql);
	for(dict->GroupsList.Start();!dict->GroupsList.End();dict->GroupsList.Next())
	{
		tmp=(dict->GroupsList)();
		for(tmp->List->Start();!tmp->List->End();tmp->List->Next())
		{
			sprintf(sSql,"INSERT INTO %skwdsbygroups(grid,kwdid) VALUES (%u,%u)",dict->GetLang()->GetCode(),tmp->GetId(),
			(*(tmp->List))()->GetId()) ;
 			try
			{
				RQuery insertword(this,sSql);
			}
			catch(RMySQLError e)
			{
//				cout << e.GetError()<< endl;
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadWordsGroups(GDict* dict) throw(GException)
{
	GWordList *tmp;
	char sSql[600];
	unsigned int i=0;
//	cout<<dict->GroupsList.NbPtr<<endl;
	sprintf(sSql,"SELECT grid, kwdid  FROM %skwdsbygroups",dict->GetLang()->GetCode());
	RQuery loadwords (this, sSql);
	for(loadwords.Start();!loadwords.End();loadwords.Next())
	{
		cout<<i++<<" "<<atoi(loadwords[0])<<" "<<atoi(loadwords[1])<<" nb= "<<dict->GroupsList.NbPtr<<" "<<dict->GroupsList.GetPtr(atoi(loadwords[0]))<<endl;
		if(!dict->GroupsList.IsIn(atoi(loadwords[0])))
			dict->GroupsList.InsertPtr(new GWordList(atoi(loadwords[0]),dict->GetWord(atoi(loadwords[0]))));
		tmp=dict->GroupsList.GetPtr(atoi(loadwords[0]));
		cout<<dict->GroupsList.GetPtr(atoi(loadwords[0]))<<" "<<tmp<<" nb= "<<dict->GroupsList.NbPtr<<endl;
		tmp->InsertWord(new GWord(atoi(loadwords[1]),dict->GetWord(atoi(loadwords[1]))));
	}

}
//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveUpDatedDoc(GDoc* doc,unsigned n) throw(GException)
{
	char sSql[1000];
	const char* l=0;
	unsigned int id;
	GMIMEFilter* f;
	id=doc->GetId();
	const char* fn;
	char slang[5];
	char smime[50];
	char surl[200];
	char sname[200];
	char supdated[15];
	char scomputed[15];
	GIWordWeightCursor Words;
	if(doc->GetLang())
	{
		l=doc->GetLang()->GetCode();
		Words=dynamic_cast<GDocVector*>(doc)->GetWordWeightCursor();
		for(Words.Start();!Words.End();Words.Next())
		{
      if(Words()->GetId()>=n)
      {
			  sprintf(sSql,"INSERT INTO %shtmlsbykwds(htmlid,kwdid,occurs) VALUES (%u,%u,%f)",l,id,Words()->GetId(),Words()->GetWeight());
			  RQuery insertkwds(this,sSql);
      }
		}
		l=ValidSQLValue(l,slang);
	}
	else
	{
		l=SQLNULL;
	}

	// Update document
	f=doc->GetMIMEType();
	if(f)
		fn=ValidSQLValue(f->GetName(),smime);
	else
		fn=SQLNULL;
	sprintf(sSql,"UPDATE htmls SET "
	             "html=%s,title=%s,mimetype=%s,langid=%s,"
	             "updated=%s,calculated=%s,"
	             "n=%u,ndiff=%u,v=%u,vdiff=%u,failed=%u WHERE htmlid=%u",
	             ValidSQLValue(doc->GetURL(),surl),ValidSQLValue(doc->GetName(),sname),fn,l,
	             GetDateToMySQL(doc->GetUpdated(),supdated),GetDateToMySQL(doc->GetComputed(),scomputed),
	             doc->GetN(),doc->GetNdiff(),doc->GetV(),doc->GetVdiff(),doc->GetFailed(),id);
	RQuery updatedoc(this,sSql);

}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveGroups(void)
{
	GGroups* groups;
	GGroup* g;
	GIWordWeightCursor WordCur;
	GGroupsCursor GroupsCursor; 
	char sSql[100];
	GLangCursor Langs;
	GSubProfile* sub;
	char sattached[15];

	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		sprintf(sSql,"DELETE FROM %sgroupsbykwds ",Langs()->GetCode());
		RQuery delete2(this,sSql);
	}

	GroupsCursor=this->GetGroupsCursor();
	for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
	{
		groups=GroupsCursor();
		for(groups->Start();!groups->End();groups->Next())
		{
			g=(*groups)();
 			// build point of views
			//      PrepearPoV(g);
			// Save SubProfiles infos
			for(g->Start();!g->End();g->Next())
			{
				sub=(*g)();
				sprintf(sSql,"UPDATE subprofiles SET groupid=%u,attached=%s WHERE subprofileid=%u",g->GetId(),GetDateToMySQL(sub->GetAttached(),sattached),sub->GetId());
				RQuery update(this,sSql);
			}

			// Save the description part
			WordCur=static_cast<GGroupVector*>((*groups)())->GetVectorCursor();
			for(WordCur.Start();!WordCur.End();WordCur.Next())
			{
				sprintf(sSql,"INSERT INTO %sgroupsbykwds(groupid,kwdid,occurs) VALUES(%u,%u,%f)",g->GetLang()->GetCode(),g->GetId(),WordCur()->GetId(),WordCur()->GetWeight());
				RQuery InsertWord(this,sSql);
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveMixedGroups(RContainer<GGroups,unsigned int,true,true>* mixedgroups,unsigned int id, bool historic)
{
	char sSql[100];
	char database[20];
	char field[20];
	GGroups* grps;
	GGroup* grp;

	// set the name of the database and the field 'id' of this database.
	if(historic)
	{
		sprintf(database,"historicgroups");
		sprintf(field,"historicid") ;
	}
	else
	{
		sprintf(database,"tempchromo");
		sprintf(field,"chromoid");
	}
	
	// Delete all the old chromo where the id is id.
	if(!mixedgroups) return;
	if(!id)
	{
		// First chromosome to store, delete all chromosomes
		sprintf(sSql, "DELETE FROM %s",database);
		RQuery delete1(this,sSql);
	}

	for(mixedgroups->Start(); !mixedgroups->End(); mixedgroups->Next())
	{
		grps=(*mixedgroups)();
		for(grps->Start(); !grps->End(); grps->Next())
		{
			grp=(*grps)();
			for(grp->Start(); !grp->End(); grp->Next())
			{
				GSubProfile* sub = (*grp)();
				sprintf(sSql,"INSERT INTO %s(%s,groupid,lang,subprofileid) VALUES(%u,%u,'%s',%u)",database,field, id,grp->GetId(),grp->GetLang()->GetCode(),sub->GetId());
				RQuery InsertChromo(this,sSql);
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveHistoricProfiles(unsigned int historicid)
{

	// Delete historic subprofiles if historicid=0 .
	char sSql[200];
	if (historicid==0)
	{
		GLangCursor curLang=this->GetLangsCursor();
		for (curLang.Start();!curLang.End();curLang.Next())
		{
			sprintf(sSql,"DELETE FROM %shistoricsubprofiles",curLang()->GetCode());
			RQuery deletekwds(this,sSql);
		}
	}

	GProfileCursor curProf=this->GetProfilesCursor() ;


	// Save the Subprofile
	for(curProf.Start();!curProf.End();curProf.Next())
	{
		GSubProfileCursor curSub=curProf()->GetSubProfilesCursor();

		for(curSub.Start();!curSub.End();curSub.Next())
			SaveSubProfileInHistory(curSub(), historicid);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::NewGroup(GLang* lang,GGroup* grp)
{
	char sSql[100];

	// Insert it
	if(!lang) return;
	sprintf(sSql,"INSERT INTO groups(langid) VALUES('%s')",lang->GetCode());
	RQuery insert(this,sSql);

	// Get Id and updated
	sprintf(sSql,"SELECT groupid FROM groups WHERE groupid=LAST_INSERT_ID()");
	RQuery select(this,sSql);
	select.Start();
	grp->SetId(strtoul(select[0],0,10));
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::DeleteGroup(GGroup* grp)
{
	char sSql[100];

	if(!grp) return;
	sprintf(sSql,"UPDATE subprofiles SET groupid=0 WHERE groupid=%u",grp->GetId());
	RQuery up(this,sSql);
	sprintf(sSql,"DELETE FROM groups WHERE groupid=%u",grp->GetId());
	RQuery del(this,sSql);
	grp->SetId(cNoRef);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadGroups() throw(bad_alloc,GException)
{
	char sSql[100];
	GGroupVector* group;
	GLangCursor Langs;
//	GIWordWeight* Word;
	GGroups* groups;
	GGroupsCursor GroupsCursor;

	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		GGroups* groups=GetGroups(Langs());
		sprintf(sSql,"SELECT groupid, langid  FROM groups WHERE langid='%s'",Langs()->GetCode());
		RQuery group2 (this,sSql);
		for(group2.Start();!group2.End();group2.Next())
		{
			group=new GGroupVector(atoi(group2[0]),Langs());
			sprintf(sSql,"SELECT kwdid,occurs FROM %sgroupsbykwds where groupid=%u",Langs()->GetCode(),atoi(group2[0]));
			RQuery sel(this,sSql);
			for(sel.Start();!sel.End();sel.Next())
			{
				if(GetDic(Langs())->GetElement(atoi(sel[0]))->GetType()==tWordList)
					group->AddWordList(atoi(sel[0]),atof(sel[1]));
				else
					group->AddWord(new GIWordWeight(atoi(sel[0]),atof(sel[1])));
			}
			groups->InsertPtr(group);
		}
	}

	// Update References of the loaded groups.
	GroupsCursor=this->GetGroupsCursor();
	for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
	{
		groups=GroupsCursor();
		for(groups->Start();!groups->End();groups->Next())
		{
			static_cast<GGroupVector*>((*groups)())->UpdateRefs();
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::ExecuteData(const char* filename) throw(GException)
{
	RTextFile Sql(filename);
	RString l;

	while(!Sql.Eof())
	{
		l=Sql.GetLine();                                                       
		RQuery exec(this,l);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::ClearStoredChromos(void)
{
	RQuery delete1(this,"DELETE FROM tempchromo");
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveChromo(GChromoIR* chromo,unsigned int id,RObjs<GObjIR>* objs)
{
	char sSql[100];
	unsigned int* tab;
	unsigned int* ptr;
	int GrpId;
	const char* c;

	// Get the language of hte instance.
	c=chromo->Instance->GetLang()->GetCode();

	// Delete all the old chromo where the id is id.
	sprintf(sSql,"DELETE FROM tempchromo WHERE chromoid=%u AND lang='%s'",id,c);
	RQuery delete1(this,sSql);

	// Parse the chromosomes and save them into the database.
	GrpId=0;
	for(chromo->Used.Start();!chromo->Used.End();chromo->Used.Next())
	{
		GrpId++;
		GGroupIR* gr=chromo->Used();
		ptr=tab=gr->GetObjectsId();
		while((*ptr)!=NoObject)
        {
			GObjIR* o=objs->Tab[*(ptr++)];
			sprintf(sSql,"INSERT INTO tempchromo(chromoid,groupid,lang,subprofileid) VALUES(%u,%u,'%s',%u)",id,GrpId,c,o->GetSubProfile()->GetId());
			RQuery InsertChromo(this,sSql);
		}
		delete[] tab;
	}
}


//-----------------------------------------------------------------------------
GInstIR* GALILEI::GSessionMySQL::LoadInstIR(GLang* lang,RObjs<GObjIR>* objs,GIRParams* p)
{
	GInstIR* InstIR;
	char sSql[200];
	unsigned int popsize;
	GChromoIR* chromo;
	GGroupDataIR data;
	GGroupIR* grp;
	unsigned int id;
	unsigned int chromoid;
	unsigned int groupid;
	unsigned int v;

	// Init part
	chromoid=cNoRef;

	// count the number of chromosome in tempchromo to assign popsize;
	sprintf(sSql,"SELECT max(chromoid) FROM tempchromo");
	RQuery count(this,sSql);
	count.Start();
	if(count.End()) return(0);
	popsize=atoi(count[0]);
	if(!popsize) return(0);

	InstIR=new GInstIR(this,lang,0,objs,p,popsize,0);
	InstIR->Init(&data);
	sprintf(sSql,"SELECT chromoid,groupid,subprofileid FROM tempchromo WHERE lang='%s' ORDER by chromoid,groupid",lang->GetCode());
	RQuery GA(this,sSql);
	for(GA.Start();!GA.End();GA.Next())
	{
		// Read Chromosome
		v=atoi(GA[0]);
		if(v!=chromoid)
		{
			chromoid=v;
			groupid=cNoRef;
			grp=0;
			if(chromoid==popsize)
				chromo=InstIR->BestChromosome;
			else
				chromo=InstIR->Chromosomes[chromoid];
		}

		// Read Group
		v=atoi(GA[1]);
		// If group id changed -> new group needed
		if((v!=groupid))
		{
			grp=chromo->ReserveGroup();
			groupid=v;
		}

		// Read SubProfile
		id=atoi(GA[2]);
		for(objs->Start();!objs->End();objs->Next())
		{
			if((*objs)()->GetSubProfile()->GetId()==id)
			{
				grp->Insert((*objs)());
				break;
			}
		}
	}
	return(InstIR);
}

//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveDocSim(void)
{
	char sSql[200];

	// The container of documents.
	RContainer<GGroupsEvaluate,unsigned int,false,false>* GroupsDoc = IdealDocs;

	double tempGlobal;
	double tempnormal;

	for(GroupsDoc->Start();!GroupsDoc->End();GroupsDoc->Next())
	{
		GGroupEvaluateCursor Cur2=(*GroupsDoc)()->GetGroupEvaluateCursor();
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			GGroupEvaluate* Group=Cur2();
			// A vector used to memorise the id of the documents
			int* vector;
			vector=new int[Group->NbPtr()];
			unsigned int i=0;
			for(Group->Start();!Group->End();Group->Next())
			{
				vector[i]=Group->Current();
				i++;
			}
			// For all the document in this group
			for(i=0;i<Group->NbPtr();i++)
			{
				for(unsigned int j=0;j<Group->NbPtr();j++)
				{
					tempGlobal=Group->GlobalSimilarity(vector[i],vector[j]);
					tempnormal=Group->Similarity(vector[i],vector[j]);
 					if((tempGlobal>-10)&&(tempGlobal<10))
					{
						if(vector[i]<vector[j])
						{
							sprintf(sSql,"INSERT INTO statdoc(id1,id2,nsim,gsim,same) VALUES(%u,%u,%f,%f,1)",vector[i],vector[j],tempnormal,tempGlobal);
							RQuery InsertChromo(this,sSql);
						}
					}
				}
			}

			// Calc the similarity for all the document which are not in the current group.
			GGroupEvaluateCursor Cur=(*GroupsDoc)()->GetGroupEvaluateCursor();
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				GGroupEvaluate* GroupExtra=Cur();
				int* vectorextra;
				vectorextra=new int[GroupExtra->NbPtr()];
				int ii=0;
				for(GroupExtra->Start();!GroupExtra->End();GroupExtra->Next())
				{
					vectorextra[ii]=GroupExtra->Current();
					ii++;
				}
				if(GroupExtra->GetId()!=Group->GetId())
				{
					for(i=0;i<Group->NbPtr();i++)
					{
						for(unsigned int j=0;j<GroupExtra->NbPtr();j++)
						{
							tempGlobal=Group->GlobalSimilarity(vectorextra[j],vector[i]);
							tempnormal=Group->Similarity(vectorextra[j],vector[i]);
 							if((tempGlobal>-10)&&(tempGlobal<10))
							{
								if(vector[i]<vectorextra[j])
								{
									sprintf(sSql,"INSERT INTO statdoc(id1,id2,nsim,gsim,same) VALUES(%u,%u,%f,%f,0)",vector[i],vectorextra[j],tempnormal,tempGlobal);
									RQuery InsertChromo(this,sSql);
								}
							}
						}
					}
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
GGroupsHistory* GALILEI::GSessionMySQL::LoadAnHistoricGroups(RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang, unsigned int historicid)
{
	char sSql[200];
	GGroupHistory* grp;
	GGroupsHistory* grps;
	GIWordsWeightsHistory* historicsubprof;
	unsigned int subprofid;
	unsigned int groupid;
	unsigned int v;

	// Init part
	groupid=cNoRef;
	grps=new GGroupsHistory(lang, historicid);

	//read the groupment.
	sprintf(sSql,"SELECT groupid,subprofileid FROM historicgroups WHERE lang='%s' AND historicid=%u ORDER by historicid,groupid",lang->GetCode(),historicid);
	RQuery grquery(this,sSql);
	for(grquery.Start();!grquery.End();grquery.Next())
	{

		// Read Group
		v=atoi(grquery[0]);
		// If group id changed -> new group needed
		if((v!=groupid))
		{
			groupid=v;
			grp=new GGroupHistory(groupid, lang, grps);
			//insert group in the container of groups.
			grps->InsertPtr(grp);
		}

		// Create the historic subprofile     and add it to the group
		subprofid=atoi(grquery[1]);
		historicsubprof=new GIWordsWeightsHistory(subprofiles->GetPtr(subprofid),100);
		grp->AddSubProfile(historicsubprof);

		//fill the vector of the subprofile
		sprintf(sSql,"SELECT kwdid,weight FROM %shistoricsubprofiles WHERE historicid=%u AND subprofileid=%u",lang->GetCode(), historicid,subprofid);
		RQuery subprofdesc(this,sSql);
		for(subprofdesc.Start();!subprofdesc.End();subprofdesc.Next())
		{
			historicsubprof->InsertPtr(new GIWordWeight(atoi(subprofdesc[0]),double(atof(subprofdesc[1]))));
		}
	}
	return(grps);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSessionMySQL::GetHistorySize(void)
{
	char sSql[200];
	sprintf(sSql,"SELECT COUNT(DISTINCT historicid) from historicgroups");
	RQuery size(this, sSql);
	size.Start();
	return(atoi(size[0]));
}

//-----------------------------------------------------------------------------
int GALILEI::GSessionMySQL::Alreadyexist(void)
{
	char sSql[100];

	sprintf(sSql,"SELECT * FROM expertkwds");
	RQuery all(this,sSql);

	if(!all.GetNbRows())
		return(0);

	return(1);

}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::PrepearPoV(GGroup* grp)
{
	GDispatchpov *disp;

	//cout << "bijor je suis le nouveau dispatch"<<endl;
	if(/*!Alreadyexist()*/0)    // WARNING : enlever ! inversion pour debug
	{
		disp->UpdatePovForAGrp();    // exemple / all groups / one group
	}
	else
	{
		disp->GeneratePoVfromScratch(grp);
	}
}


//-----------------------------------------------------------------------------
GALILEI::GSessionMySQL::~GSessionMySQL() throw(GException)
{

}
