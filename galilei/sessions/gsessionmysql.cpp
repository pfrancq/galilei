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
//#include <stdio.h>      // Defining size_t
#include <stdlib.h>
//#include <string.h>     // String functions
//#include <sys/stat.h>
//#include <unistd.h>
//#include <fcntl.h>


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
GALILEI::GSessionMySQL::GSessionMySQL(const char* host,const char* user,const char* pwd,const char* db) throw(bad_alloc,GException,RMySQLError)
	: RDb(host,user,pwd,db), GSession()
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
void GALILEI::GSessionMySQL::LoadUsersFromDB()
{        
	Users = new GUsers(10);
	GUser* usr;
	RQuery users (this, "SELECT userid,user FROM users");
	for(users.Begin();users.IsMore();users++)
	{
		usr=new GUser(atoi(users[0]),users[1]);
		LoadProfilesFromDB(usr);
		Users->InsertPtr(usr);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadProfilesFromDB (GUser* usr)
{
	// load profiles of this user
	GProfile* prof;
	char sSql[100];
	sprintf(sSql,"SELECT profileid, description, updated FROM profiles WHERE userid=%u",usr->GetId());
        
	RQuery profiles(this,sSql);
	for(profiles.Begin();profiles.IsMore();profiles++)
	{
		prof=new GProfile(usr,atoi(profiles[0]),profiles[1]);
		LoadSubProfilesFromDB(prof);
		usr->InsertPtr(prof);
	}
}
        

//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadSubProfilesFromDB (GProfile* prof)
{
	GSubProfile* sub ;
	char ssql[100], sSql[100];
	GLang* lang;

	sprintf(ssql,"SELECT subprofileid, langid FROM subprofiles WHERE profileid=%u",prof->GetId());
	RQuery subprofil (this,ssql);
	for(subprofil.Begin();subprofil.IsMore();subprofil++)
	{
		lang=GetLang(subprofil[1]);
		sub=new GSubProfile(prof,atoi(subprofil[0]),lang);

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

		prof->InsertPtr(sub);

	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadDics(void) throw(bad_alloc,GException)
{
	char str[30];

	if(!Dics) Dics=new GDicts(Langs.NbPtr);
	if(!Stops) Stops=new GDicts(Langs.NbPtr);
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		sprintf(str,"%skwds",Langs()->GetCode());
//        LoadDicFromDB(str,lang,Dics);
		sprintf(str,"%sstopkwds",Langs()->GetCode());
//        LoadDicFromDb(str,lang,Stops);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadLangsFromDB()
{
	Dics=new GDicts(Langs.NbPtr);
	Stops=new GDicts(Langs.NbPtr);
	LoadDicFromDB("enkwds",GetLang("en")) ;
	LoadStopsFromDB("enstopkwds",GetLang("en")) ;
	LoadDicFromDB("frkwds",GetLang("fr")) ;
	LoadStopsFromDB("frstopkwds",GetLang("fr")) ;
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadDicFromDB(const RString &name,GLang *lang)
{
	GDict* dict;
	unsigned int MaxCount=0;
	char sSql[100];

	for(char i='a';i<='z';i++)
	{
		sprintf(sSql,"SELECT COUNT(*) FROM %s WHERE kwd LIKE '%c%%'",name(),i);
		RQuery count(this,sSql);
		count.Begin();
		if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
	}
	if(MaxCount==0) MaxCount=100;
	sprintf(sSql,"SELECT MAX(kwdid) FROM %s",name());
	RQuery max(this,sSql);
	max.Begin();
	if(max[0])
		dict=new GDict(this,name,"Dictionnary",lang,strtoul(max[0],0,10),MaxCount);
	else
		dict=new GDict(this,name,"Dictionnary",lang,100,MaxCount);
	FillDict(dict);
	Dics->InsertPtr(dict);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadStopsFromDB(const RString &name,GLang *lang)
{
	GDict* dict;
	unsigned int MaxCount=0;
	char sSql[100];

	for(char i='a';i<='z';i++)
	{
		sprintf(sSql,"SELECT COUNT(*) FROM %s WHERE kwd LIKE '%c%%'",name(),i);
		RQuery count(this,sSql);
		count.Begin();
		if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
	}
	if(MaxCount==0) MaxCount=100;
	sprintf(sSql,"SELECT MAX(kwdid) FROM %s",name());
	RQuery max(this,sSql);
	max.Begin();
	if(max[0])
		dict=new GDict(this,name,"Dictionnary",lang,strtoul(max[0],0,10),MaxCount);
	else
		dict=new GDict(this,name,"Dictionnary",lang,100,MaxCount);
	FillDict(dict);
	Stops->InsertPtr(dict);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::FillDict(GDict* dict)
{
	char ssql[100];
	sprintf(ssql,"SELECT kwdid, kwd  FROM %s", dict->GetName());
	RQuery dicts (this, ssql);
	for(dicts.Begin();dicts.IsMore();dicts++)
	{
		dict->Put(atoi(dicts[0]), RString(dicts[1]));
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadDocs(void)
{
	GDoc* doc;
	int docid;

	char sSql1[100],sSql2[100],sSql3[100],sSql4[100];
	sprintf(sSql1,"SELECT COUNT(*) FROM htmls" );
	RQuery count(this,sSql1);
	count.Begin();
	Docs = new  GDocs (atoi(count[0]),this);
	sprintf(sSql2,"SELECT htmlid,html,langid,calculated,wordnumtot,wordnumdiff,title  FROM htmls");
	RQuery quer (this,sSql2);
	for(quer.Begin();quer.IsMore();quer++)
	{
		docid=atoi(quer[0]);
		Docs->InsertPtr(doc=new GDoc(quer[1],quer[6],docid,GetLang(quer[2]),atoi(quer[4]),atoi(quer[5])));
		sprintf(sSql3,"SELECT COUNT(*) FROM %shtmlsbykwds WHERE htmlid=%u",quer[2],docid);
		RQuery count(this,sSql3);
		count.Begin();
		sprintf(sSql4,"SELECT kwdid,occurs FROM %shtmlsbykwds WHERE htmlid=%u",quer[2],docid);
		RQuery doc2(this,sSql4);
		for(doc2.Begin();doc2.IsMore();doc2++)
		{
			doc->AddWord(atoi(doc2[0]),atoi(doc2[1]));
		}
	}
	LoadProfilesDocs();
}


//-----------------------------------------------------------------------------
unsigned GALILEI::GSessionMySQL::DicNextId(const RString& word)   // function to get 'next id' for dic. (to devellop!)
{
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadGroupsFromDB()
{
	char sSql[100];

	for(Langs.Start();!Langs.End();Langs.Next())
	{
		GGroups* groups=GetGroups(Langs());
		sprintf(sSql,"SELECT groupid, langid  FROM groups WHERE langid=\"%s\"",Langs()->GetCode());
		RQuery group2 (this,sSql);
		for(group2.Begin();group2.IsMore();group2++)
		{
			GGroup* group=new GGroup(atoi(group2[0]));
			groups->InsertPtr(group);
			sprintf(sSql, "SELECT groupid, subprofileid, attach FROM groupsbysubprofiles WHERE groupid=%u", group->GetId());
			RQuery group3 (this,sSql);
			for(group3.Begin(); group3.IsMore(); group3++)
			{
				for(Users->Start(); !Users->End(); Users->Next())
				{
					GUser* usr = (*Users)();
					for(usr->Start(); !usr->End();usr->Next())
					{
						GProfile* prof=(*usr)();
						GSubProfile* sub=prof->GetPtr<int>(atoi(group3[1]));
						if(sub)
							group->InsertPtr(sub);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadProfilesDocs()
{
	for (Users->Start();!Users->End();Users->Next())
	{
		GUser* user=(*Users)();
		for (user->Start();!user->End();user->Next())
			LoadProfileDocs((*user)());
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadProfileDocs(GProfile* profile)
{
	char ssql[100];
	sprintf(ssql,"SELECT htmlid, judgement  FROM htmlsbyprofiles WHERE profileid=%u", profile->GetId());
	RQuery docs (this, ssql);
	for(docs.Begin();docs.IsMore();docs++)
		profile->AddDocJudged(Docs->GetPtr<const unsigned>(atoi(docs[0])),atoi(docs[1]));
}


//-----------------------------------------------------------------------------
GALILEI::GSessionMySQL::~GSessionMySQL()
{
}
