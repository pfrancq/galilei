/*

	GALILEI Research Project

	GSessionMySQL.cpp

	GALILEI Session using a MySQL Database - Implementation.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gsessionsmysql/gsessionmysql.h>
using namespace GALILEI;
using namespace RMySQL;



//-----------------------------------------------------------------------------
//
//  GSessionMySQL
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSessionMySQL::GSessionMySQL(const char* host,const char* user,const char* pwd,const char* db,GURLManager* mng) throw(bad_alloc,GException,RMySQLError)
	: RDb(host,user,pwd,db), GSession(GetCount("htmls"),GetCount("users"),GetCount("htmlsbyprofiles"),mng)
{
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSessionMySQL::GetCount(const char* tbl)
{
	char sSql[100];

	sprintf(sSql,"SELECT COUNT(*) FROM %s",tbl);
	RQuery count(this,sSql);
	count.Begin();
	return(strtoul(count[0],0,10));
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSessionMySQL::GetMax(const char* tbl,const char* fld)
{
	char sSql[100];

	sprintf(sSql,"SELECT MAX(%s) FROM %s",fld,tbl);
	RQuery count(this,sSql);
	count.Begin();
	return(strtoul(count[0],0,10));
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSessionMySQL::GetDicNextId(const char* word,const GDict* dict)
{
	char sSql[100];

	// Verify that the word didn't already exist.
	sprintf(sSql,"SELECT kwdid FROM %skwds WHERE kwd='%s'",dict->GetLang()->GetCode(),word);
	RQuery find(this,sSql);
	if(find.GetNbRows())
	{
		find.Begin();
		return(strtoul(find[0],0,10));
	}

	// Insert the new word
	sprintf(sSql,"INSERT INTO %skwds(kwd) VALUES('%s')",dict->GetLang()->GetCode(),word);
	RQuery insert(this,sSql);

	// Get the next id
	sprintf(sSql,"SELECT last_insert_id() FROM %skwds",dict->GetLang()->GetCode());
	RQuery getinsert(this,sSql);
	getinsert.Begin();
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
		count.Begin();
		if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
	}
	if(MaxCount==0) MaxCount=100;
	MaxId=GetMax(tbl,"kwdid");
	if(!MaxId)
		MaxId=100;

	// Create and insert the dictionnary
	if(s)
		Stops.InsertPtr(dict=new GDict(this,tbl,"Stop List",GetLang(code),MaxId,MaxCount,true));
	else
		Dics.InsertPtr(dict=new GDict(this,tbl,"Dictionnary",GetLang(code),MaxId,MaxCount,false));

	// Load the dictionnary from the database
	sprintf(sSql,"SELECT kwdid, kwd  FROM %s",tbl);
	RQuery dicts (this, sSql);
	for(dicts.Begin();dicts.IsMore();dicts++)
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
	w.Begin();
	return(w[0]);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadUsers() throw(bad_alloc,GException)
{
	char sSql[100];
	GUser* usr;
	GProfile* prof;
	GLang* lang;
	GSubProfile* sub;
	unsigned int userid,profileid;

	// Go through the users
	RQuery users(this, "SELECT userid,user,fullname FROM users");
	for(users.Begin();users.IsMore();users++)
	{
		userid=atoi(users[0]);
		sprintf(sSql,"SELECT profileid, description, updated FROM profiles WHERE userid=%u",userid);
		RQuery profiles(this,sSql);
		Users.InsertPtr(usr=new GUser(userid,users[1],users[2],profiles.GetNbRows()));
		for(profiles.Begin();profiles.IsMore();profiles++)
		{
			profileid=atoi(profiles[0]);
			usr->InsertPtr(prof=new GProfile(usr,profileid,profiles[1],GetNbLangs()));
			sprintf(sSql,"SELECT subprofileid,langid FROM subprofiles WHERE profileid=%u",profileid);
			RQuery subprofil (this,sSql);
			for(subprofil.Begin();subprofil.IsMore();subprofil++)
			{
				lang=GetLang(subprofil[1]);
				prof->InsertPtr(sub=new GSubProfile(prof,atoi(subprofil[0]),lang));

				// Load GWordList 'OK'
				sprintf(sSql,"SELECT kwdid FROM %sokkwds WHERE subprofileid=%u",lang->GetCode(),sub->GetId());
				RQuery ok(this,sSql);
				for(ok.Begin();ok.IsMore();ok++)
					sub->GetOK()->InsertPtr(new GIWord(atoi(ok[0])));

				// Load GWordList 'KO'
				sprintf(sSql,"SELECT kwdid FROM %skokwds WHERE subprofileid=%u",lang->GetCode(),sub->GetId());
				RQuery ko(this,sSql);
				for(ko.Begin();ko.IsMore();ko++)
					sub->GetKO()->InsertPtr(new GIWord(atoi(ko[0])));

				// Load GWordList 'Common'
				sprintf(sSql,"SELECT kwdid FROM %scomkwds WHERE subprofileid=%u",lang->GetCode(),sub->GetId());
				RQuery com(this,sSql);
				for(com.Begin();com.IsMore();com++)
					sub->GetCommon()->InsertPtr(new GIWord(atoi(com[0])));
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadFdbks() throw(bad_alloc,GException)
{
	char sSql[200];
	GProfile* prof;
	GUser* usr;
	GDoc* doc;

	sprintf(sSql,"SELECT htmlid, judgement, profiles.profileid, userid, when2 FROM htmlsbyprofiles,profiles WHERE profiles.profileid=htmlsbyprofiles.profileid");
	RQuery fdbks(this,sSql);
	for(fdbks.Begin();fdbks.IsMore();fdbks++)
	{
		usr=Users.GetPtr<unsigned int>(atoi(fdbks[3]));
		if(!usr) continue;
		prof=usr->GetPtr<unsigned int>(atoi(fdbks[2]));
		if(!prof) continue;
		doc=Docs.GetPtr<unsigned int>(atoi(fdbks[0]));
		if(!doc) continue;
		InsertFdbk(prof,doc,fdbks[1][0],fdbks[4]);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadDocs(void) throw(bad_alloc,GException)
{
	GDoc* doc;
	GLang* lang;
	int docid;
	char sSql[100];

	sprintf(sSql,"SELECT htmlid,html,langid,calculated,wordnumtot,wordnumdiff,title,mimetype,updated,calculated FROM htmls");
	RQuery quer (this,sSql);
	for(quer.Begin();quer.IsMore();quer++)
	{
		docid=atoi(quer[0]);
		lang=GetLang(quer[2]);
		Docs.InsertPtr(doc=new GDoc(quer[1],quer[6],docid,lang,Mng->GetMIMEType(quer[7]),quer[8],quer[9],atoi(quer[4]),atoi(quer[5])));
		if(lang)
		{
			sprintf(sSql,"SELECT kwdid,occurs FROM %shtmlsbykwds WHERE htmlid=%u",lang->GetCode(),docid);
			RQuery doc2(this,sSql);
			for(doc2.Begin();doc2.IsMore();doc2++)
				doc->AddWord(atoi(doc2[0]),atoi(doc2[1]));
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadGroups() throw(bad_alloc,GException)
{
	char sSql[100];
	GGroup* group;

	for(Langs.Start();!Langs.End();Langs.Next())
	{
		GGroups* groups=GetGroups(Langs());
		sprintf(sSql,"SELECT groupid, langid  FROM groups WHERE langid=\"%s\"",Langs()->GetCode());
		RQuery group2 (this,sSql);
		for(group2.Begin();group2.IsMore();group2++)
		{
			groups->InsertPtr(group=new GGroup(atoi(group2[0])));
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadGroupsMember(GGroup* grp) throw(bad_alloc,GException)
{
	char sSql[100];

	sprintf(sSql, "SELECT groupid, subprofileid, attach FROM groupsbysubprofiles WHERE groupid=%u", grp->GetId());
	RQuery sub(this,sSql);
	for(sub.Begin();sub.IsMore();sub++)
	{
		for(Users.Start(); !Users.End(); Users.Next())
		{
			GUser* usr = Users();
			for(usr->Start(); !usr->End();usr->Next())
			{
				GProfile* prof=(*usr)();
				GSubProfile* s=prof->GetPtr<unsigned int>(atoi(sub[1]),false);
				if(s)
					grp->InsertPtr(s);
			}
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GSessionMySQL::~GSessionMySQL()
{
}
