/*

        R Project Library

        gsessionmsql.cpp

        Description - Implementation.

        (C) 2001 by Pascal Francq

        Version $Revision$

        Last Modify: $Date$

*/







// include files for ANSI C/C++
#include <stdio.h>      // Defining size_t
#include <stdlib.h>
#include <string.h>     // String functions
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

//-----------------------------------------------------------------------------
//include files for GALILEI
#include <glangs/glangen.h>
#include <glangs/glangfr.h>
#include <gsessionsmysql/gsessionmysql.h>
using namespace GALILEI;




//-----------------------------------------------------------------------------
//
//  GSessionMySQL
//
//-----------------------------------------------------------------------------






GSessionMySQL::GSessionMySQL(RDb* _db)
{
                db=_db; 
}


GSessionMySQL::~GSessionMySQL()
{
}


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadAllFromDB()
{
	LoadLangsFromDB();
	LoadUsersFromDB();
	LoadGroupsFromDB();
	LoadDocs();
}
//-----------------------------------------------------------------------------
void GSessionMySQL::LoadUsersFromDB()
{        
	if (Langs)
	{
		Users = new GUsers(10);
		GUser* usr;
		RQuery users (db, "SELECT userid,user FROM users");
		for(users.Begin();users.IsMore();users++)
		{
			usr=new GUser(atoi(users[0]),users[1]);
			LoadProfilesFromDB(usr);
			Users->InsertPtr(usr);
		}
	}
	else
		cout << "please loas Langs (dics) first" <<endl;
}


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadProfilesFromDB (GUser* usr)
{
	// load profiles of this user
	GProfile* prof;
	char sSql[100];
	sprintf(sSql,"SELECT profileid, description, updated FROM profiles WHERE userid=%u",usr->GetId());
        
	RQuery profiles(db,sSql);
	for(profiles.Begin();profiles.IsMore();profiles++)
	{
		prof=new GProfile(usr,atoi(profiles[0]),profiles[1]);
		LoadSubProfilesFromDB(prof);
		usr->InsertPtr(prof);
	}
        
}
        


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadSubProfilesFromDB (GProfile* prof)
{
	GSubProfile* sub ;
	char ssql[100], sSql[100];
	GLang* lang;
	sprintf(ssql,"SELECT subprofileid, langid FROM subprofiles WHERE profileid=%u",prof->GetId());
	RQuery subprofil (db,ssql);
	for(subprofil.Begin();subprofil.IsMore();subprofil++)
	{
        
		lang=Langs->GetLang(subprofil[1]);
		sub=new GSubProfile(prof,atoi(subprofil[0]),lang);
                                

		// Load GWordList 'OK'
		sprintf(sSql,"SELECT kwdid FROM %sokkwds WHERE subprofileid=%u",lang->GetCode(),sub->GetId());
		RQuery ok(db,sSql);
		for(ok.Begin();ok.IsMore();ok++)                
			sub->GetOK()->InsertPtr(new GIWord(atoi(ok[0])));

		// Load GWordList 'KO'
		sprintf(sSql,"SELECT kwdid FROM %skokwds WHERE subprofileid=%u",lang->GetCode(),sub->GetId());
		RQuery ko(db,sSql);
		for(ko.Begin();ko.IsMore();ko++)                
			sub->GetKO()->InsertPtr(new GIWord(atoi(ko[0])));

		// Load GWordList 'Common'
		sprintf(sSql,"SELECT kwdid FROM %scomkwds WHERE subprofileid=%u",lang->GetCode(),sub->GetId());
		RQuery com(db,sSql);
		for(com.Begin();com.IsMore();com++)                
			sub->GetCommon()->InsertPtr(new GIWord(atoi(com[0])));

		prof->InsertPtr(sub);

        }
}


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadDics(void) throw(bad_alloc,GException)
{
	GLang* lang;
	char str[30];

	if(!Dics) Dics=new GDicts(Langs->NbPtr);
	if(!Stops) Stops=new GDicts(Langs->NbPtr);
	for(Langs->Start();!Langs->End();Langs->Next())
	{
		lang=(*Langs)();
		sprintf(str,"%skwds",lang->GetCode());
//		LoadDicFromDB(str,lang,Dics);
		sprintf(str,"%sstopkwds",lang->GetCode());
//		LoadDicFromDb(str,lang,Stops);
	}
}


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadLangsFromDB()
{
	GLang* lang;

	RQuery compt(db,"SELECT COUNT(*) FROM languages");
	compt.Begin();
	Dics=new GDicts(atoi(compt[0]));
	Stops=new GDicts(atoi(compt[0]));
	Langs = new GLangs(atoi(compt[0]));
	lang=new GLangEN();
	Langs->InsertPtr(lang);
	LoadDicFromDB("enkwds",lang) ;
	LoadStopsFromDB("enstopkwds",lang) ;
	lang=new GLangFR();
	Langs->InsertPtr(lang);
	LoadDicFromDB("frkwds",lang) ;
	LoadStopsFromDB("frstopkwds",lang) ;
}


//-----------------------------------------------------------------------------

void GSessionMySQL::LoadDicFromDB(const RString &name,GLang *lang)
{
        GDict* dict;
        unsigned int MaxCount=0;
        char sSql[100];

        for(char i='a';i<='z';i++)
        {

                sprintf(sSql,"SELECT COUNT(*) FROM %s WHERE kwd LIKE '%c%%'",name(),i);
                RQuery count(db,sSql);
                count.Begin();
                if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
        }
        if(MaxCount==0) MaxCount=100;
        sprintf(sSql,"SELECT MAX(kwdid) FROM %s",name());
        RQuery max(db,sSql);
        max.Begin();
        if(max[0])
                dict=new GDict(this,name,"Dictionnary",lang,strtoul(max[0],0,10),MaxCount);
        else
                dict=new GDict(this,name,"Dictionnary",lang,100,MaxCount);

        FillDict(dict);
        Dics->InsertPtr(dict);
}


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadStopsFromDB(const RString &name,GLang *lang)
{
        GDict* dict;
        unsigned int MaxCount=0;
        char sSql[100];

        for(char i='a';i<='z';i++)
        {

                sprintf(sSql,"SELECT COUNT(*) FROM %s WHERE kwd LIKE '%c%%'",name(),i);
                RQuery count(db,sSql);
                count.Begin();
                if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
        }
        if(MaxCount==0) MaxCount=100;
        sprintf(sSql,"SELECT MAX(kwdid) FROM %s",name());
        RQuery max(db,sSql);
        max.Begin();
        if(max[0])
                dict=new GDict(this,name,"Dictionnary",lang,strtoul(max[0],0,10),MaxCount);
        else
                dict=new GDict(this,name,"Dictionnary",lang,100,MaxCount);

        FillDict(dict);
        Stops->InsertPtr(dict);
}


//-----------------------------------------------------------------------------
void GSessionMySQL::FillDict(GDict* dict)
{
	char ssql[100];
	sprintf(ssql,"SELECT kwdid, kwd  FROM %s", dict->GetName());
	RQuery dicts (db, ssql);
	for(dicts.Begin();dicts.IsMore();dicts++)
	{
	 	/*GWord* word = new GWord (atoi(dicts[0]), RString(dicts[1]));
		dict->PutDirect(word);
		dict->InsertPtr (word);*/
		dict->Put(atoi(dicts[0]), RString(dicts[1]));
	}
        
}

//-----------------------------------------------------------------------------
void GSessionMySQL::LoadDocs(void)
{
	GDoc* doc;
	int docid;

	char sSql1[100],sSql2[100],sSql3[100],sSql4[100];
	sprintf(sSql1,"SELECT COUNT(*) FROM htmls" );
	RQuery count(db,sSql1);
	count.Begin();
	Docs = new  GDocs (atoi(count[0]),this);
	sprintf(sSql2,"SELECT htmlid,html,langid,calculated,wordnumtot,wordnumdiff,title  FROM htmls");
	RQuery quer (db,sSql2);
	for(quer.Begin();quer.IsMore();quer++)
	{
		docid=atoi(quer[0]);


		Docs->InsertPtr(doc=new GDoc(quer[1],quer[6],docid,Langs->GetLang(quer[2]),atoi(quer[4]),atoi(quer[5])));
		sprintf(sSql3,"SELECT COUNT(*) FROM %shtmlsbykwds WHERE htmlid=%u",quer[2],docid);
		RQuery count(db,sSql3);
		count.Begin();
		sprintf(sSql4,"SELECT kwdid,occurs FROM %shtmlsbykwds WHERE htmlid=%u",quer[2],docid);
		RQuery doc2(db,sSql4);
		for(doc2.Begin();doc2.IsMore();doc2++)
		{
			doc->AddWord(atoi(doc2[0]),atoi(doc2[1]));
		}
	}
	LoadProfilesDocs();
}





//-----------------------------------------------------------------------------
unsigned GSessionMySQL::DicNextId(const RString& word)   // function to get 'next id' for dic. (to devellop!)
{
	return(0);
}


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadGroupsFromDB()
{        
        GroupsLangs = new GGroupsLangs(this);
        char ssql[100], ssql2[100], ssql3[100];
        sprintf(ssql, "SELECT langid FROM languages");
        RQuery group1 (db,ssql);
        for(group1.Begin();group1.IsMore();group1++)
        {
                GLang* lang = Langs->GetLang(group1[0]);
                GGroups* groups=new GGroups(GroupsLangs, lang);
                GroupsLangs->InsertPtr(groups);
                sprintf(ssql2,"SELECT groupid, langid  FROM groups WHERE langid=\"%s\"",group1[0] );

                RQuery group2 (db, ssql2);
                for(group2.Begin();group2.IsMore();group2++)
                {
                        GGroup* group=new GGroup(groups);
                        group->Id=atoi(group2[0]);
                        groups->InsertPtr(group);
                        sprintf(ssql3, "SELECT groupid, subprofileid, attach FROM groupsbysubprofiles WHERE groupid=%u", group->Id);
                        RQuery group3 (db, ssql3);
                        for(group3.Begin(); group3.IsMore(); group3++)
                        {
                                for (Users->Start(); !Users->End(); Users->Next())
                                {
                                        GUser* usr = (*Users)();
                                        for (usr->Start(); !usr->End();usr->Next())
                                        {
                                                GProfile* prof=(*usr)();
                                                GSubProfile* sub=prof->GetPtr<int>(atoi(group3[1]));
                                                if (sub)
                                                        group->InsertPtr(sub);
                                        }
                                }
                        }
                }
        }
        
}

//-----------------------------------------------------------------------------
void GSessionMySQL::LoadProfilesDocs()
{
	for (Users->Start();!Users->End();Users->Next())
	{
		GUser* user=(*Users)();
		for (user->Start();!user->End();user->Next())
		{
			LoadProfileDocs((*user)());
		}
	}
	
}


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadProfileDocs(GProfile* profile)
{
	
	char ssql[100];
	sprintf(ssql,"SELECT htmlid, judgement  FROM htmlsbyprofiles WHERE profileid=%u", profile->GetId());
	RQuery docs (db, ssql);

	for(docs.Begin();docs.IsMore();docs++)
		profile->AddDocJudged(Docs->GetPtr<const unsigned>(atoi(docs[0])),atoi(docs[1]));
}


//-----------------------------------------------------------------------------
void GSessionMySQL::AddDocJudgement(GDoc* doc, GProfile* profile, char fdbk)
{
}
