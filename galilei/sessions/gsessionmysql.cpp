/*

	R Project Library

	gsessionmsql.cpp

	Description - Implementation.

	(C) 2001 by Pascal Francq

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


//---------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdio.h>      // Defining size_t
#include <stdlib.h>
#include <string.h>     // String functions


//-----------------------------------------------------------------------------
//include files for GALILEI
#include "gsessionmysql.h"

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
void GSessionMySQL::LoadUsersFromDB(GSessionMySQL* mysqlsess)
{	
	Users = new GUsers(10,mysqlsess);
	GUser* usr;
    RQuery users (db, "SELECT userid,user FROM users");
	for(users.Begin();users.IsMore();users++)
	{
		usr=new GUser(Users);
		usr->Id=atoi(users[0]);
		usr->Name=RString(users[1]);
		LoadProfilesFromDB(usr);	
		Users->InsertPtr(usr);
	}
}


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadProfilesFromDB (GUser* usr)
{
	// load profiles of this user
	GProfile* prof;
	char sSql[100];
	sprintf(sSql,"SELECT profileid, description FROM profiles WHERE userid=%u",usr->Id);
	RQuery profiles(db,sSql);
	for(profiles.Begin();profiles.IsMore();profiles++)
	{
		prof=new GProfile(usr);
		prof->Id=atoi(profiles[0]);
		prof->Name=RString (profiles[1]);
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
	sprintf(ssql,"SELECT subprofileid, langid FROM subprofiles WHERE profileid=%u",prof->Id);
	RQuery subprofil (db,ssql);
	for(subprofil.Begin();subprofil.IsMore();subprofil++)
	{
		lang=Langs->GetLang(subprofil[1]);
		sub=new GSubProfile(prof,atoi(subprofil[0]),lang);
				

		// Load GWordList 'OK'
		sprintf(sSql,"SELECT kwdid FROM %sokkwds WHERE subprofileid=%u",lang->Code,sub->Id);
		RQuery ok(db,sSql);
		for(ok.Begin();ok.IsMore();ok++)		
			sub->OK->InsertPtr(new GWordRef(atoi(ok[0])));

		// Load GWordList 'KO'
		sprintf(sSql,"SELECT kwdid FROM %skokwds WHERE subprofileid=%u",lang->Code,sub->Id);
		RQuery ko(db,sSql);
		for(ko.Begin();ko.IsMore();ko++)		
		sub->KO->InsertPtr(new GWordRef(atoi(ko[0])));

		// Load GWordList 'Common'
		sprintf(sSql,"SELECT kwdid FROM %scomkwds WHERE subprofileid=%u",lang->Code,sub->Id);
		RQuery com(db,sSql);
		for(com.Begin();com.IsMore();com++)		
			sub->Common->InsertPtr(new GWordRef(atoi(com[0])));

		prof->InsertPtr(sub);
	}
}


//-----------------------------------------------------------------------------
void GSessionMySQL::LoadLangsFromDB()
{
	RQuery compt(db,"SELECT COUNT(*) FROM languages");
	compt.Begin();
	Dics=new GDicts(atoi(compt[0]), this);
	Stops=new GDicts(atoi(compt[0]), this);
	Langs = new GLangs(5);
	RQuery langs(db,"SELECT langid,language FROM languages");
	for(langs.Begin();langs.IsMore();langs++)
	{
		GLang* lang=new GLang (langs[1], langs[0]);
		Langs->InsertPtr(lang);
		RString dictablename=RString (langs[0])+RString("kwds");
		LoadDicFromDB( dictablename,lang) ;
		dictablename=RString (langs[0])+RString("stopkwds");
		LoadStopsFromDB( dictablename,lang) ;
	}
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
		dict=new GDict(this,name,lang,strtoul(max[0],0,10),MaxCount);
	else
		dict=new GDict(this,name,lang,100,MaxCount);

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
		dict=new GDict(this,name,lang,strtoul(max[0],0,10),MaxCount);
	else
		dict=new GDict(this,name,lang,100,MaxCount);

	FillDict(dict);
	Stops->InsertPtr(dict);
}


//-----------------------------------------------------------------------------
void GSessionMySQL::FillDict(GDict* dict)
{
	char ssql[100];
	sprintf(ssql,"SELECT kwdid, kwd  FROM %s", dict->Name());
	RQuery dicts (db, ssql);
	for(dicts.Begin();dicts.IsMore();dicts++)
	{
		GWord* word = new GWord (atoi(dicts[0]), RString(dicts[1]));
		dict->InsertPtr (word);
	}
	
}



//-----------------------------------------------------------------------------
unsigned GSessionMySQL::DicNextId(const RString& word)   // function to get 'next id' for dic. (to devellop!)
{
}



//-----------------------------------------------------------------------------
