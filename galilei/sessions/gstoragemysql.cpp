/*

	GALILEI Research Project

	GStorageMySQL.cpp

	Storage Manager using a MySQL Database - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>
#include <ctype.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/gdict.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <infos/gword.h>
#include <infos/gwordlist.h>
#include <infos/gweightinfo.h>
#include <infos/gweightinfos.h>
#include <docs/gdocvector.h>
#include <docs/glink.h>
#include <profiles/guser.h>
#include <profiles/gusers.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <profiles/gsubprofilevector.h>
#include <sessions/gstoragemysql.h>
#include <groups/ggroup.h>
#include <groups/ggroupvector.h>
#include <groups/gsubjects.h>
#include <groups/gsubject.h>
#include <sessions/gsession.h>
#include <historic/ggroupshistory.h>
#include <historic/gweightinfoshistory.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GStorageMySQL
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Static variable
#define Ins_spec 1
const char GStorageMySQL::SQLNULL[5]="NULL";


//------------------------------------------------------------------------------
GStorageMySQL::GStorageMySQL(const char* host,const char* user,const char* pwd,const char* db) throw(std::bad_alloc,GException,RMySQLError)
	: RDb(host,user,pwd,db), GStorage(db)
{
}


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetCount(const char* tbl) throw(RMySQLError)
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


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetMax(const char* tbl,const char* fld) throw(RMySQLError)
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


//------------------------------------------------------------------------------
const char* GStorageMySQL::GetDateToMySQL(const RDate& d,char* tmp) throw(RMySQLError)
{
	sprintf(tmp,"'%u-%u-%u'",d.GetYear(),d.GetMonth(),d.GetDay());
	return(tmp);
}


//------------------------------------------------------------------------------
const char* GStorageMySQL::ValidSQLValue(const char* val,char* tmp) throw(RMySQLError)
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


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetNbSaved(tObjType type) throw(GException)
{
	try
	{
		switch(type)
		{
			case otDoc:
				return(GetCount("htmls"));

			case otUser:
				return(GetCount("users"));

			case otProfile:
				return(GetCount("profiles"));

			case otGroup:
				return(GetCount("groups"));

			case otFdbk:
				return(GetCount("htmlsbyprofiles"));

			default:
				return(0);
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GData* data,const GDict* dict) throw(GException)
{
	char sSql[600];
	char tmp[100];

	try
	{
		// Verify that the word didn't already exist.
		sprintf(sSql,"SELECT kwdid FROM %skwds WHERE kwd='%s'",dict->GetLang()->GetCode(),data->GetName().Latin1());
		RQuery find(this,sSql);
		if(find.GetNbRows())
		{
			find.Start();
			data->SetId(strtoul(find[0],0,10));
		}

		// Insert the new word
		sprintf(sSql,"INSERT INTO %skwds(kwd,type) VALUES(%s,%u)",dict->GetLang()->GetCode(),ValidSQLValue(data->GetName(),tmp),data->GetType());
		RQuery insert(this,sSql);

		// Get the next id
		sprintf(sSql,"SELECT kwdid FROM %skwds WHERE kwdid=LAST_INSERT_ID()",dict->GetLang()->GetCode());
		RQuery getinsert(this,sSql);
		getinsert.Start();
		data->SetId(strtoul(getinsert[0],0,10));
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}



//------------------------------------------------------------------------------
void GStorageMySQL::LoadDic(GDict* &dic,GLang* lang,bool s) throw(std::bad_alloc,GException)
{
	unsigned int MaxCount=100;
	unsigned int MaxId=0;
	char sSql[100];
	char tbl[20];

	try
	{
		// Construct the table name
		if(s)
			sprintf(tbl,"%sstopkwds",lang->GetCode());
		else
			sprintf(tbl,"%skwds",lang->GetCode());

		// Search the values to initialise the dictionary
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

		// Create and insert the dictionary
		if(s)
			dic=new GDict(tbl,"Stop List",lang,MaxId,MaxCount,true);
		else
			dic=new GDict(tbl,"Dictionary",lang,MaxId,MaxCount,false);

		// Load the dictionary from the database
		sprintf(sSql,"SELECT kwdid, kwd, type  FROM %s",tbl);
		RQuery dicts (this, sSql);
		for(dicts.Start();!dicts.End();dicts.Next())
		{
			switch(atoi(dicts[2]))
			{
				case infoWord :
					{
						GWord w(atoi(dicts[0]),dicts[1]);
						dic->InsertData(&w);
					}
					break;

				case infoWordList :
					{
						GWordList w(atoi(dicts[0]),dicts[1]);
						LoadWordList(&w,lang);
						dic->InsertData(&w);
					}
					break;
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//--------------------.---------------------------------------------------------
const char* GStorageMySQL::LoadWord(unsigned int id,const char* code) throw(std::bad_alloc,GException)
{
	char sSql[100];

	try
	{
		sprintf(sSql,"SELECT kwd FROM %skwds WHERE kwdid=%i",code,id);
		RQuery w(this,sSql);
		if(!w.GetNbRows())
			return(0);
		w.Start();
		return(w[0]);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadWordList(GWordList* w,GLang* lang) throw(std::bad_alloc,GException)
{
	char sSql[100];

	try
	{
		sprintf(sSql,"SELECT kwdid FROM %skwdsbygroups WHERE grid=%i",lang->GetCode(),dynamic_cast<GData*>(w)->GetId());
		RQuery wl(this,sSql);
		for(wl.Start();!wl.End();wl.Next())
			w->InsertWord(new GWord(atoi(wl[0]),lang->GetDict()->GetData(atoi(wl[0]))->GetName()));
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveWordList(GDict* dic,GWordList* w) throw(GException)
{
	GWordCursor Cur;
	char sSql[600];

	try
	{
		Cur=w->GetWordCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			sprintf(sSql,"INSERT INTO %skwdsbygroups(grid,kwdid) VALUES (%u,%u)",dic->GetLang()->GetCode(),dynamic_cast<GData*>(w)->GetId(),Cur()->GetId());
			try
			{
				RQuery insertword(this,sSql);
			}
			catch(RMySQLError e)
			{
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveSubProfile(GSubProfile* sub) throw(GException)
{
	char sSql[200];
	char scomputed[15];
	char supdated[15];
	GWeightInfoCursor Cur;
	const char* l;

	try
	{
		//update the subprofile;
		sprintf(sSql,"UPDATE subprofiles SET udpated=%s, calculated=%s WHERE subprofileid=%u",
				GetDateToMySQL(sub->GetUpdated(),supdated), GetDateToMySQL(sub->GetComputed(),scomputed), sub->GetId());
		RQuery updatesubprof(this,sSql);

		// Delete keywords
		l=sub->GetLang()->GetCode();

		sprintf(sSql,"DELETE FROM %ssubprofilesbykwds WHERE subprofileid=%u",l,sub->GetId());
		RQuery deletekwds(this,sSql);

		Cur=((GSubProfileVector*)sub)->GetWeightInfoCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			sprintf(sSql,"INSERT INTO  %ssubprofilesbykwds(subprofileid,kwdid,weight) VALUES (%u,%u,'%e')",
						l,sub->GetId(),Cur()->GetId(),Cur()->GetWeight());
			RQuery insertkwds(this,sSql);
		}

		sub->SetState(osUpToDate);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveSubProfileInHistory(GSubProfile* sub,unsigned int historicid) throw(GException)
{
	char sSql[200];
	GWeightInfoCursor Cur;


	try
	{
		// Save subprofiles
		Cur=((GSubProfileVector*)sub)->GetWeightInfoCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			sprintf(sSql,"INSERT INTO  %shistoricsubprofiles(historicid,subprofileid,kwdid,weight) VALUES (%u, %u,%u,'%e')",
						sub->GetLang()->GetCode() ,historicid,sub->GetId(),Cur()->GetId(),Cur()->GetWeight());
			RQuery insertkwds(this,sSql);
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveProfile(GProfile* prof) throw(GException)
{
	char sSql[500];
	char sname[200];
	unsigned int profid;
	unsigned int social;
	GSubProfileCursor CurSub=prof->GetSubProfilesCursor();

	try
	{
		// Init
		profid=prof->GetId();

		// Save the Subprofile
		for(CurSub.Start();!CurSub.End();CurSub.Next())
		{
			if (CurSub()->GetState()==osUpdated)
				SaveSubProfile(CurSub());
		}


		// Update profiles
		if(prof->IsSocial())
			social=1;
		else
			social =0;
		sprintf(sSql,"UPDATE profiles SET description=%s,social=%u WHERE profileid=%u",
				ValidSQLValue(prof->GetName(),sname),social,profid);
		RQuery updateprof(this,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadUsers(GSession* session) throw(std::bad_alloc,GException)
{
	char sSql[100];
	GUser* usr;
	GProfile* prof;
	GFactoryLangCursor langs;
	GLang* lang;
	GSubProfileVector* sub;
	unsigned int userid,profileid,subid;
	GGroup* grp;
	bool Social;
	GSubject* s;

	// Go through the users
	try
	{
		RQuery select(this, "SELECT userid,user,fullname FROM users");
		for(select.Start();!select.End();select.Next())
		{
			userid=atoi(select[0]);
			sprintf(sSql,"SELECT profileid,description,social,topicid FROM profiles WHERE userid=%u",userid);
			RQuery profiles(this,sSql);
			session->InsertUser(usr=new GUser(userid,select[1],select[2],profiles.GetNbRows()));
			for(profiles.Start();!profiles.End();profiles.Next())
			{
				profileid=atoi(profiles[0]);
				#if GALILEITEST
					if(session->GetSubjects())
						s=session->GetSubjects()->GetSubject(atoi(profiles[3]));
					else
						s=0;
				#else
					s=0;
				#endif
				Social=false;
				if(atoi(profiles[2])==1) Social=true;
				session->InsertProfile(prof=new GProfile(usr,profileid,profiles[1],Social,session->GetLangs()->NbPtr));
				#if GALILEITEST
					if(s)
						prof->SetSubject(s);
				#endif
				sprintf(sSql,"SELECT subprofileid,langid,attached,groupid, updated, calculated FROM subprofiles WHERE profileid=%u",profileid);
				RQuery subprofil (this,sSql);
				for(subprofil.Start();!subprofil.End();subprofil.Next())
				{
					lang=session->GetLangs()->GetLang(subprofil[1]);
					subid=atoi(subprofil[0]);
					grp=session->GetGroup(atoi(subprofil[3]));
					session->InsertSubProfile(sub=new GSubProfileVector(prof,subid,lang,grp,subprofil[2], subprofil[4], subprofil[5]));
					#if GALILEITEST
						if((s)&&(sub->GetLang()==s->GetLang()))
						{
							sub->SetSubject(s);
							s->InsertSubProfile(sub);
						}
					#endif
				}
			}
		}

		// Load the subprofile's description
		langs=session->GetLangs()->GetLangsCursor();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			sprintf(sSql,"SELECT subprofileid,kwdid,Weight FROM %ssubprofilesbykwds",lang->GetCode());
			RQuery sel(this,sSql);
			for(sel.Start();!sel.End();sel.Next())
			{
				sub=dynamic_cast<GSubProfileVector*>(session->GetSubProfile(atoi(sel[0]),lang));
				if(sub)
					sub->AddInfo(new GWeightInfo(atoi(sel[1]),atof(sel[2]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()));
			}
		}

		// Update References of the loaded subprofiles.
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			GSubProfileCursor SubProfiles=session->GetSubProfilesCursor(lang);
			for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
				dynamic_cast<GSubProfileVector*>(SubProfiles())->UpdateRefs();
		}

		// Load the ideal Groups.
		LoadIdealGroupment(session);
	}
	catch(RMySQLError& e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadIdealGroupment(GSession* session) throw(std::bad_alloc,GException)
{
	GGroups* groups;
	GGroup* group;
	GFactoryLangCursor langs;
	GLang* lang;
	char sSql[100];
	GSubProfile* subp;

	try
	{
		if(!session->GetSubjects()) return;
		groups=session->GetSubjects()->GetIdealGroups();
		groups->Clear();

		langs=session->GetLangs()->GetLangsCursor();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			sprintf(sSql,"SELECT DISTINCT(groupid) FROM idealgroup WHERE langid='%s'",lang->GetCode());
			RQuery sel(this,sSql);
			for(sel.Start();!sel.End();sel.Next())
			{
				groups->InsertGroup(group=new GGroupVector(atoi(sel[0]),lang,false));
				sprintf(sSql,"SELECT profileid FROM idealgroup where groupid=%u",atoi(sel[0]));
				RQuery sub(this,sSql);
				for(sub.Start();!sub.End();sub.Next())
				{
					subp=session->GetProfile(atoi(sub[0]))->GetSubProfile(lang);
					if(subp)
						group->InsertSubProfile(subp);
				}
				if(!group->GetNbSubProfiles())
					groups->DeleteGroup(group);
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveIdealGroupment(GGroups* idealgroup) throw(GException)
{
	GGroupCursor groups;
	GSubProfileCursor sub;
	char sSql[100];

	try
	{
		sprintf(sSql,"DELETE from idealgroup");
		RQuery del(this,sSql);

		groups=idealgroup->GetGroupsCursor();
		for(groups.Start();!groups.End();groups.Next())
		{
			sub=groups()->GetSubProfilesCursor();
			for(sub.Start();!sub.End();sub.Next())
			{
				sprintf(sSql,"INSERT INTO idealgroup(profileid,langid,groupid) VALUES(%u,'%s',%u)",sub()->GetProfile()->GetId(),groups()->GetLang()->GetCode(),groups()->GetId());
				RQuery insert(this,sSql);
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadSubjectTree(GSession* session) throw(std::bad_alloc,GException)
{
	char sSql[200];
	GSubject* subject;
	GSubject* subsubject;

	try
	{
		if(!session->GetSubjects()) return;
		session->GetSubjects()->Clear();
		RQuery sub(this,"SELECT topicid,name,used,langid FROM topics WHERE parent=0");
		for(sub.Start();!sub.End();sub.Next())
		{
			session->GetSubjects()->AddNode(0,subject=new GSubject(atoi(sub[0]),sub[1],session->GetLangs()->GetLang(sub[3]),atoi(sub[2])));
			sprintf(sSql,"SELECT topicid,name,used,langid FROM topics WHERE parent=%u",atoi(sub[0]));
			RQuery subsub(this,sSql);
			for(subsub.Start();!subsub.End();subsub.Next())
				session->GetSubjects()->AddNode(subject,subsubject=new GSubject(atoi(subsub[0]),subsub[1],session->GetLangs()->GetLang(subsub[3]),atoi(subsub[2])));
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadFdbks(GSession* session) throw(std::bad_alloc,GException)
{
	char sSql[200];
	GProfile* prof;
	GUser* usr;
	GDoc* doc;
	tDocAssessment jug;

	try
	{
		sprintf(sSql,"SELECT htmlid, judgement, profiles.profileid, userid, when2 FROM htmlsbyprofiles,profiles WHERE profiles.profileid=htmlsbyprofiles.profileid");
		RQuery fdbks(this,sSql);
		for(fdbks.Start();!fdbks.End();fdbks.Next())
		{
			usr=session->GetUser(atoi(fdbks[3]));
			if(!usr) continue;
			prof=usr->GetPtr<unsigned int>(atoi(fdbks[2]));
			if(!prof) continue;
			doc=session->GetDoc(atoi(fdbks[0]));
			if(!doc) continue;

			switch(fdbks[1][0])
			{
				case 'O':
					jug=djOK;
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
					jug = tDocAssessment(jug | djHub);
					break;
				case 'A':
					jug = tDocAssessment(jug | djAutority);
					break;
				//case 'U':
					//break;
				default:
					break;
			}
			session->InsertFdbk(prof,doc,jug,fdbks[4]);
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadDocs(GSession* session) throw(std::bad_alloc,GException)
{
	GDocVector* doc;
	GLang* lang;
	int docid;
	char sSql[100];
	GFactoryLangCursor langs;
	GDoc* d;
	GSubject* s;

	try
	{
		sprintf(sSql,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed FROM htmls");
		RQuery quer (this,sSql);
		for(quer.Start();!quer.End();quer.Next())
		{
			docid=atoi(quer[0]);
			lang=session->GetLangs()->GetLang(quer[4]);
			session->InsertDoc(doc=new GDocVector(quer[1],quer[2],docid,lang,quer[3],quer[5],quer[6],atoi(quer[7])));
		}

		// Load the links of the document loaded.
		sprintf(sSql,"SELECT htmlid,linkid,occurs FROM htmlsbylinks");
		RQuery querLinks (this,sSql);
		for (querLinks.Start(); !querLinks.End() ; querLinks.Next())
		{
			session->GetDoc(atoi(querLinks[0]))->InsertLink(session->GetDoc(atoi(querLinks[1])), atoi(querLinks[2]));
		}

		// Load the document description
		langs=session->GetLangs()->GetLangsCursor();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			sprintf(sSql,"SELECT htmlid,kwdid,occurs FROM %shtmlsbykwds",lang->GetCode());
			RQuery sel(this,sSql);
			for(sel.Start();!sel.End();sel.Next())
			{
				doc=dynamic_cast<GDocVector*>(session->GetDoc(atoi(sel[0])));
				if(doc)
					doc->AddInfo(new GWeightInfo(atoi(sel[1]),atof(sel[2]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()));
			}
		}

		//  Make Link between documents and topics
		#if GALILEITEST
		if(session->GetSubjects())
		{
			RQuery subdocs(this,"SELECT htmlid,topicid FROM topicsbyhtmls");
			for(subdocs.Start();!subdocs.End();subdocs.Next())
			{
				d=session->GetDoc(atoi(subdocs[0]));
				if(!d) continue;
				s=session->GetSubjects()->GetSubject(atoi(subdocs[1]));
				if(!s) continue;
				s->InsertDoc(d);
				d->InsertSubject(s);
			}
		}
		#endif

		// Update References of the loaded documents.
		GDocCursor Docs=session->GetDocsCursor();
		for(Docs.Start();!Docs.End();Docs.Next())
			dynamic_cast<GDocVector*>(Docs())->UpdateRefs();
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveFdbks(GSession* session) throw(GException)
{
	char sSql[500];
	//char j;
	char *j = new char[2];
	GProfDocCursor Fdbks;

	try
	{
		// Clear the all feedbacks
		sprintf(sSql,"DELETE FROM htmlsbyprofiles");
		RQuery delete1(this,sSql);

		// Reinsert all the feedbacks
		Fdbks=session->GetProfDocsCursor();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			switch(Fdbks()->GetFdbk() & djMaskJudg)
			{
				case djOK:
					j[0]='O';
					break;
				case djOutScope:
					j[0]='H';
					break;
				case djKO:
					j[0]='K';
					break;
				default:
					throw GException("No Valid Assessment");
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
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveDoc(GDoc* doc) throw(GException)
{
	char sSql[1000];
	const char* l=0;
	unsigned int id;
	const char* f;
	id=doc->GetId();
	const char* fn;
	char slang[5];
	char smime[50];
	char surl[200];
	char sname[200];
	char supdated[15];
	char scomputed[15];
	GWeightInfoCursor Words;
	GLinkCursor lcur;

	try
	{
		// Delete keywords
		if(doc->GetLang())
		{
			l=doc->GetLang()->GetCode();
			sprintf(sSql,"DELETE FROM %shtmlsbykwds WHERE htmlid=%u",l,id);
			RQuery deletekwds(this,sSql);
			Words=dynamic_cast<GDocVector*>(doc)->GetWeightInfoCursor();
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
			fn=ValidSQLValue(f,smime);
		else
			fn=SQLNULL;
		sprintf(sSql,"UPDATE htmls SET "
					"html=%s,title=%s,mimetype=%s,langid=%s,"
					"updated=%s,calculated=%s,"
					"failed=%u WHERE htmlid=%u",
					ValidSQLValue(doc->GetURL(),surl),ValidSQLValue(doc->GetName(),sname),fn,l,
					GetDateToMySQL(doc->GetUpdated(),supdated),GetDateToMySQL(doc->GetComputed(),scomputed),
					doc->GetFailed(),id);
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
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveUpDatedDoc(GDoc* doc,unsigned n) throw(GException)
{
	char sSql[1000];
	const char* l=0;
	unsigned int id;
	const char* f;
	id=doc->GetId();
	const char* fn;
	char slang[5];
	char smime[50];
	char surl[200];
	char sname[200];
	char supdated[15];
	char scomputed[15];
	GWeightInfoCursor Words;

	try
	{
		if(doc->GetLang())
		{
			l=doc->GetLang()->GetCode();
			Words=dynamic_cast<GDocVector*>(doc)->GetWeightInfoCursor();
			for(Words.Start();!Words.End();Words.Next())
			{
		if((Words()->GetId()>=n)&&(Words()->GetWeight()>0))
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
			fn=ValidSQLValue(f,smime);
		else
			fn=SQLNULL;
		sprintf(sSql,"UPDATE htmls SET "
					"html=%s,title=%s,mimetype=%s,langid=%s,"
					"updated=%s,calculated=%s,"
					"failed=%u WHERE htmlid=%u",
					ValidSQLValue(doc->GetURL(),surl),ValidSQLValue(doc->GetName(),sname),fn,l,
					GetDateToMySQL(doc->GetUpdated(),supdated),GetDateToMySQL(doc->GetComputed(),scomputed),
					doc->GetFailed(),id);
		RQuery updatedoc(this,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveGroups(GSession* session) throw(GException)
{
	GWeightInfoCursor WordCur;
	GGroupCursor GroupsCursor;
	char sSql[100];
	GFactoryLangCursor langs;
	GLang* lang;
	char sattached[15];
	GSubProfileCursor Sub;

	try
	{
		RQuery delete1(this,"DELETE FROM groups");
		langs=session->GetLangs()->GetLangsCursor();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			sprintf(sSql,"DELETE FROM %sgroupsbykwds ",lang->GetCode());
			RQuery delete2(this,sSql);
		}

		GroupsCursor=session->GetGroupsCursor();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			sprintf(sSql,"INSERT INTO groups(groupid,langid) VALUES(%u,'%s')",GroupsCursor()->GetId(),GroupsCursor()->GetLang()->GetCode());
			RQuery insert1(this,sSql);

			// Save SubProfiles infos
			Sub=GroupsCursor()->GetSubProfilesCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sprintf(sSql,"UPDATE subprofiles SET groupid=%u,attached=%s WHERE subprofileid=%u",
						GroupsCursor()->GetId(),GetDateToMySQL(Sub()->GetAttached(),sattached),Sub()->GetId());
				RQuery update(this,sSql);
			}

			// Save the description part
			WordCur=dynamic_cast<GGroupVector*>(GroupsCursor());
			for(WordCur.Start();!WordCur.End();WordCur.Next())
			{
				sprintf(sSql,"INSERT INTO %sgroupsbykwds(groupid,kwdid,occurs) VALUES(%u,%u,%f)",
						GroupsCursor()->GetLang()->GetCode(),GroupsCursor()->GetId(),WordCur()->GetId(),WordCur()->GetWeight());
				RQuery InserinfoWord(this,sSql);
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveGroupsHistory(GSession* session) throw(GException)
{
	GGroupCursor GroupsCursor;
	GSubProfileCursor Sub;
	unsigned int historicid;
	char sSql[100];

	try
	{
		// if historic needed, gget the last historic id
		historicid=0;
		RQuery histcount(this,"SELECT MAX(historicid) from historicgroups");
		histcount.Start();
		if(histcount[0])
			historicid=atoi(histcount[0]);
		historicid++;

		// save the groups in history
		GroupsCursor=session->GetGroupsCursor();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			Sub=GroupsCursor()->GetSubProfilesCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sprintf(sSql,"INSERT INTO historicgroups SET historicid=%u, groupid=%u, subprofileid=%u, lang='%s'",
				historicid,GroupsCursor()->GetId(),Sub()->GetId(),GroupsCursor()->GetLang()->GetCode());
				RQuery history(this,sSql);
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveMixedGroups(GGroups* mixedgroups,unsigned int id, bool historic) throw(GException)
{
	char sSql[100];
	char database[20];
	char field[20];
	GGroupCursor grp;
	GSubProfileCursor Sub;

	try
	{
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

		grp=mixedgroups->GetGroupsCursor();
		for(grp.Start();!grp.End();grp.Next())
		{
			Sub=grp()->GetSubProfilesCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sprintf(sSql,"INSERT INTO %s(%s,groupid,lang,subprofileid) VALUES(%u,%u,'%s',%u)",database,field, id,grp()->GetId(),grp()->GetLang()->GetCode(),Sub()->GetId());
				RQuery InsertChromo(this,sSql);
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveHistoricProfiles(GSession* session,unsigned int historicid) throw(GException)
{
	GFactoryLangCursor curLang;
	GLang* lang;

	try
	{
		// Delete historic subprofiles if historicid=0.
		char sSql[200];
		if (historicid==0)
		{
			for (curLang.Start();!curLang.End();curLang.Next())
			{
				lang=curLang()->GetPlugin();
				if(!lang) continue;
				sprintf(sSql,"DELETE FROM %shistoricsubprofiles",lang->GetCode());
				RQuery deletekwds(this,sSql);
			}
		}

		GProfileCursor curProf=session->GetProfilesCursor() ;


		// Save the Subprofile
		for(curProf.Start();!curProf.End();curProf.Next())
		{
			GSubProfileCursor curSub=curProf()->GetSubProfilesCursor();

			for(curSub.Start();!curSub.End();curSub.Next())
				SaveSubProfileInHistory(curSub(), historicid);
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadGroups(GSession* session) throw(std::bad_alloc,GException)
{
	char sSql[100];
	GGroupVector* group;
	GGroupCursor GroupsCursor;
	GLang* lang;
	GFactoryLangCursor langs;

	try
	{
		RQuery group2 (this,"SELECT groupid, langid  FROM groups");
		for(group2.Start();!group2.End();group2.Next())
		{
			lang=session->GetLangs()->GetLang(group2[1]);
			group=new GGroupVector(atoi(group2[0]),lang,true);
			session->InsertGroup(group);
		}

		// Load the groups description
		langs=session->GetLangs()->GetLangsCursor();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			sprintf(sSql,"SELECT groupid,kwdid,occurs FROM %sgroupsbykwds",lang->GetCode());
			RQuery sel(this,sSql);
			for(sel.Start();!sel.End();sel.Next())
			{
				group=dynamic_cast<GGroupVector*>(session->GetGroup(atoi(sel[0])));
				if(!group) continue;
				group->AddInfo(new GWeightInfo(atoi(sel[1]),atof(sel[2]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()));
			}
		}

		// Update References of the loaded groups.
		GroupsCursor=session->GetGroupsCursor();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
			dynamic_cast<GGroupVector*>(GroupsCursor())->UpdateRefs();
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::ExecuteData(const char* filename) throw(GException)
{
	try
	{
		RTextFile Sql(filename);
		RString l;
		while(!Sql.Eof())
		{
			l=Sql.GetLine();
			RQuery exec(this,l);
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
	catch(RString str)
	{
		throw GException(str);
	}
	catch(std::bad_alloc)
	{
		throw GException("Memory Problem");
	}
}


//------------------------------------------------------------------------------
GGroupsHistory* GStorageMySQL::LoadAnHistoricGroups(RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang, unsigned int historicid) throw(std::bad_alloc,GException)
{
	char sSql[200];
	GGroupHistory* grp;
	GGroupsHistory* grps;
	GWeightInfosHistory* historicsubprof;
	unsigned int subprofid;
	unsigned int groupid;
	unsigned int v;

	try
	{
		// Init part
		groupid=cNoRef;
		grps=new GGroupsHistory(lang, historicid);

		//read the groupment.
		sprintf(sSql,"SELECT groupid,subprofileid FROM historicgroups WHERE lang='%s' AND historicid=%u ORDER by historicid,groupid",lang->GetCode(),historicid);
		RQuery grquery(this,sSql);
		for(grquery.Start(),grp=0;!grquery.End();grquery.Next())
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

			// Create the historic subprofile and add it to the group
			subprofid=atoi(grquery[1]);
			historicsubprof=new GWeightInfosHistory(subprofiles->GetPtr(subprofid),100);
			grp->AddSubProfile(historicsubprof);

			// fill the vector of the subprofile
			sprintf(sSql,"SELECT kwdid,weight FROM %shistoricsubprofiles WHERE historicid=%u AND subprofileid=%u",lang->GetCode(), historicid,subprofid);
			RQuery subprofdesc(this,sSql);
			for(subprofdesc.Start();!subprofdesc.End();subprofdesc.Next())
			{
				historicsubprof->InsertPtr(new GWeightInfo(atoi(subprofdesc[0]),double(atof(subprofdesc[1]))));
			}
		}
		return(grps);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetHistorySize(void) throw(GException)
{
	char sSql[200];

	try
	{
		sprintf(sSql,"SELECT COUNT(DISTINCT historicid) from historicgroups");
		RQuery size(this, sSql);
		size.Start();
		return(atoi(size[0]));
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::CreateDummy(const char* name) throw(GException)
{
	char sSql[200];

	try
	{
		// Verify if the table exists
		sprintf(sSql,"SHOW TABLES LIKE '%s'",name);
		RQuery verify(this,sSql);
		if(verify.GetNbRows())
			return;

		// Table not find -> create it.
		sprintf(sSql,"CREATE TABLE %s (id INT(11), parentid INT(11), description TEXT) ",name);
		RQuery create(this, sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddDummyEntry(const char* name, unsigned int id, const char* desc, unsigned int parentid) throw(GException)
{
	char sSql[500];

	try
	{
		sprintf(sSql, "INSERT INTO %s (id, parentid, description) values (%u, %u, '%s')", name, id, parentid, desc);
		RQuery add(this, sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GStorageMySQL::~GStorageMySQL(void) throw(GException)
{
}
