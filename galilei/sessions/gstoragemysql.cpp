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
#include <historic/giwordsweightshistory.h>
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
const char GALILEI::GStorageMySQL::SQLNULL[5]="NULL";


//------------------------------------------------------------------------------
GALILEI::GStorageMySQL::GStorageMySQL(const char* host,const char* user,const char* pwd,const char* db) throw(bad_alloc,GException,RMySQLError)
	: RDb(host,user,pwd,db), GStorage(db)
{
}


//------------------------------------------------------------------------------
unsigned int GALILEI::GStorageMySQL::GetCount(const char* tbl)
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
unsigned int GALILEI::GStorageMySQL::GetMax(const char* tbl,const char* fld)
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
const char* GALILEI::GStorageMySQL::GetDateToMySQL(const RDate& d,char* tmp)
{
	sprintf(tmp,"'%u-%u-%u'",d.GetYear(),d.GetMonth(),d.GetDay());
	return(tmp);
}


//------------------------------------------------------------------------------
const char* GALILEI::GStorageMySQL::ValidSQLValue(const char* val,char* tmp)
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
void GALILEI::GStorageMySQL::AssignId(GData* data,const GDict* dict)
{

	// preliminary traitement of special words and composite like insert -> reinsert ...
	#if Ins_spec
		if (strstr(data->GetName(),"insert"))
		{
			data->SetId(1);
			return;
		}
		if (strstr(data->GetName(),"delete"))
		{
			data->SetId(2);
			return;
		}
		if (strstr(data->GetName(),"update"))
		{
			data->SetId(3);
			return;
		}
		if (strstr(data->GetName(),"select"))
		{
			data->SetId(4);
			return;
		}
	#endif

	char sSql[600];
	char tmp[100];
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



//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::LoadDic(GDict* &dic,GLang* lang,bool s) throw(bad_alloc,GException)
{
	unsigned int MaxCount=100;
	unsigned int MaxId=0;
	char sSql[100];
	char tbl[20];

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


//--------------------.---------------------------------------------------------
const char* GALILEI::GStorageMySQL::LoadWord(unsigned int id,const char* code)
{
	char sSql[100];

	sprintf(sSql,"SELECT kwd FROM %skwds WHERE kwdid=%i",code,id);
	RQuery w(this,sSql);
	if(!w.GetNbRows())
		return(0);
	w.Start();
	return(w[0]);
}


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::LoadWordList(GWordList* w,GLang* lang) throw(bad_alloc,GException)
{
	char sSql[100];

	sprintf(sSql,"SELECT kwdid FROM %skwdsbygroups WHERE grid=%i",lang->GetCode(),dynamic_cast<GData*>(w)->GetId());
	RQuery wl(this,sSql);
	for(wl.Start();!wl.End();wl.Next())
		w->InsertWord(new GWord(atoi(wl[0]),lang->GetDict()->GetData(atoi(wl[0]))->GetName()));
}


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveWordList(GDict* dic,GWordList* w) throw(bad_alloc,GException)
{
	GWordCursor Cur;
	char sSql[600];

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveSubProfile(GSubProfile* sub) throw(GException)
{
	char sSql[200];
	char scomputed[15];
	GWeightInfoCursor Cur;
	const char* l;


	//update the subprofile;
	sprintf(sSql,"UPDATE subprofiles SET state=%u, calculated=%s WHERE subprofileid=%u",
			 static_cast<int>(sub->GetState()), GetDateToMySQL(sub->GetComputed(),scomputed), sub->GetId());
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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveSubProfileInHistory(GSubProfile* sub,unsigned int historicid) throw(GException)
{
	char sSql[200];
	GWeightInfoCursor Cur;

	//save subprofiles
	Cur=((GSubProfileVector*)sub)->GetWeightInfoCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sprintf(sSql,"INSERT INTO  %shistoricsubprofiles(historicid,subprofileid,kwdid,weight) VALUES (%u, %u,%u,'%e')",
		              sub->GetLang()->GetCode() ,historicid,sub->GetId(),Cur()->GetId(),Cur()->GetWeight());
		RQuery insertkwds(this,sSql);
	}
}


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveProfile(GProfile* prof) throw(GException)
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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::LoadUsers(GSession* session) throw(bad_alloc,GException)
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
					s=session->GetSubjects()->GetSubject(atoi(profiles[3]));
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
				sprintf(sSql,"SELECT subprofileid,langid,attached,groupid, state, calculated FROM subprofiles WHERE profileid=%u",profileid);
				RQuery subprofil (this,sSql);
				for(subprofil.Start();!subprofil.End();subprofil.Next())
				{
					lang=session->GetLangs()->GetLang(subprofil[1]);
					subid=atoi(subprofil[0]);
					grp=session->GetGroup(atoi(subprofil[3]));
					session->InsertSubProfile(sub=new GSubProfileVector(prof,subid,lang,grp,subprofil[2], (static_cast<tObjState>(atoi(subprofil[4]))), subprofil[5]));
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
		throw GException(e.GetError());
	}
}


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::LoadIdealGroupment(GSession* session)
{
	GGroups* groups;
	GGroup* group;
	GFactoryLangCursor langs;
	GLang* lang;
	char sSql[100];
	GSubProfile* subp;

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveIdealGroupment(GGroups* idealgroup)
{
	GGroupCursor groups;
	GSubProfileCursor sub;
	char sSql[100];

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::LoadSubjectTree(GSession* session)
{
	char sSql[200];
	GSubject* subject;
	GSubject* subsubject;

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::LoadFdbks(GSession* session) throw(bad_alloc,GException)
{
	char sSql[200];
	GProfile* prof;
	GUser* usr;
	GDoc* doc;
	tDocAssessment jug;

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::LoadDocs(GSession* session) throw(bad_alloc,GException)
{
	GDocVector* doc;
	GLang* lang;
	int docid;
	char sSql[100];
	GFactoryLangCursor langs;
	GDoc* d;
	GSubject* s;

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
		session->GetDoc(atoi(querLinks[0]) )->InsertLink(session->GetDoc(atoi( querLinks[1]) ), atoi(querLinks[2]));
	}

	// Load the document's description
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
	RQuery subdocs(this,"SELECT htmlid,topicid FROM topicsbyhtmls");
	for(subdocs.Start();!subdocs.End();subdocs.Next())
	{
		d=session->GetDoc(atoi(subdocs[0]));
		if(!d) continue;
		if(session->GetSubjects())
			s=session->GetSubjects()->GetSubject(atoi(subdocs[1]));
		else
			s=0;
		if(!s) continue;
		s->InsertDoc(d);
		d->InsertSubject(s);
	}

	// Update References of the loaded documents.
	GDocCursor Docs=session->GetDocsCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
		dynamic_cast<GDocVector*>(Docs())->UpdateRefs();
}


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveFdbks(GSession* session) throw(GException)
{
	char sSql[500];
	//char j;
	char *j = new char[2];
	GProfDocCursor Fdbks;

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveDoc(GDoc* doc) throw(GException)
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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveUpDatedDoc(GDoc* doc,unsigned n) throw(GException)
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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveGroups(GSession* session)
{
	GWeightInfoCursor WordCur;
	GGroupCursor GroupsCursor;
	char sSql[100];
	GFactoryLangCursor langs;
	GLang* lang;
	char sattached[15];
	GSubProfileCursor Sub;

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveMixedGroups(GGroups* mixedgroups,unsigned int id, bool historic)
{
	char sSql[100];
	char database[20];
	char field[20];
	GGroupCursor grp;
	GSubProfileCursor Sub;

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::SaveHistoricProfiles(GSession* session,unsigned int historicid)
{
	GFactoryLangCursor curLang;
	GLang* lang;

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::LoadGroups(GSession* session) throw(bad_alloc,GException)
{
	char sSql[100];
	GGroupVector* group;
	GGroupCursor GroupsCursor;
	GLang* lang;
	GFactoryLangCursor langs;

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


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::ExecuteData(const char* filename) throw(GException)
{
	RTextFile Sql(filename);
	RString l;

	while(!Sql.Eof())
	{
		l=Sql.GetLine();
		RQuery exec(this,l);
	}
}


//------------------------------------------------------------------------------
GGroupsHistory* GALILEI::GStorageMySQL::LoadAnHistoricGroups(RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang, unsigned int historicid)
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

		// Create the historic subprofile     and add it to the group
		subprofid=atoi(grquery[1]);
		historicsubprof=new GIWordsWeightsHistory(subprofiles->GetPtr(subprofid),100);
		grp->AddSubProfile(historicsubprof);

		//fill the vector of the subprofile
		sprintf(sSql,"SELECT kwdid,weight FROM %shistoricsubprofiles WHERE historicid=%u AND subprofileid=%u",lang->GetCode(), historicid,subprofid);
		RQuery subprofdesc(this,sSql);
		for(subprofdesc.Start();!subprofdesc.End();subprofdesc.Next())
		{
			historicsubprof->InsertPtr(new GWeightInfo(atoi(subprofdesc[0]),double(atof(subprofdesc[1]))));
		}
	}
	return(grps);
}


//------------------------------------------------------------------------------
unsigned int GALILEI::GStorageMySQL::GetHistorySize(void)
{
	char sSql[200];
	sprintf(sSql,"SELECT COUNT(DISTINCT historicid) from historicgroups");
	RQuery size(this, sSql);
	size.Start();
	return(atoi(size[0]));
}


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::CreateDummy(const char* name) throw(GException)
{
	char sSql[200];
	sprintf(sSql,"CREATE TABLE %s (id INT(11), parentid INT(11), description TEXT) ",name);
	RQuery create(this, sSql);
}


//------------------------------------------------------------------------------
void GALILEI::GStorageMySQL::AddDummyEntry(const char* name, unsigned int id, const char* desc, unsigned int parentid)
{
	char sSql[500];
	sprintf(sSql, "INSERT INTO %s (id, parentid, description) values (%u, %u, '%s')", name, id, parentid, desc);
	RQuery add(this, sSql);
}


//------------------------------------------------------------------------------
GALILEI::GStorageMySQL::~GStorageMySQL(void) throw(GException)
{
}
