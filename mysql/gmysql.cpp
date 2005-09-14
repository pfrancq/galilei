/*

	GALILEI Research Project

	GStorageMySQL.cpp

	Storage Manager using a MySQL Database - Implementation.

	Copyright 2001-2005 by the Université libre de Bruxelles.

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
#include <ctype.h>



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdict.h>
#include <glang.h>
#include <ginfo.h>
#include <ginfolist.h>
#include <gword.h>
#include <gwordlist.h>
#include <gweightinfo.h>
#include <gweightinfos.h>
#include <glink.h>
#include <gsugs.h>
#include <guser.h>
#include <gprofile.h>
#include <gsubprofile.h>
#include <gmysql.h>
#include <gslot.h>
#include <ggroup.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <gsession.h>
#include <ggroupshistory.h>
#include <gweightinfoshistory.h>
#include <gindexer.h>
#include <gwordoccurs.h>
#include <gpluginmanagers.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GStorageMySQL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStorageMySQL::GStorageMySQL(GFactoryStorage* fac)
	: GStorage(fac), Db(0)
{
}

//-----------------------------------------------------------------------------
void GStorageMySQL::ApplyConfig(void)
{
	bool Changes=false;

	RString host=Factory->GetString("Host");
	if(Host!=host)
	{
		Host=host;
		Changes=true;
	}
	RString user=Factory->GetString("User");
	if(User!=user)
	{
		User=user;
		Changes=true;
	}
	RString password=Factory->GetString("Password");
	if(Password!=password)
	{
		Password=password;
		Changes=true;
	}
	RString database=Factory->GetString("Database");
	if(Database!=database)
	{
		Database=database;
		Changes=true;
	}
	RDate filter(Factory->GetString("Filter"));
	if(Filter!=filter)
	{
		Filter=filter;
		Changes=true;
	}
	bool all=Factory->GetBool("All");
	if(LoadAll!=all)
	{
		LoadAll=all;
		Changes=true;
	}
	RString encoding=Factory->GetString("Encoding");
	if(Encoding!=encoding)
	{
		Encoding=encoding;
		Changes=true;
	}
	bool filtering=Factory->GetBool("Filtering");
	if(Filtering!=filtering)
	{
		Filtering=filtering;
		Changes=true;
	}
	if(!Changes)
		return;
	GStorage::ApplyConfig();
	try
	{
		Db.reset(new RDb(Host,User,Password,Database,Encoding));
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetCount(RString tbl)
{
	RString c;
	RString sSql("SELECT COUNT(*) FROM "+tbl);

	RQuery count(Db,sSql);
	count.Start();
	c=count[0];
	if(c.IsEmpty()) return(0);
	return(atoi(c));
}


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetMax(RString tbl,RString fld)
{
	RString sSql("SELECT MAX("+fld+") FROM "+tbl);
	RString c;

	RQuery count(Db,sSql);
	count.Start();
	c=count[0];
	if(c.IsEmpty()) return(0);
	return(atoi(c));
}


//------------------------------------------------------------------------------
RString GStorageMySQL::GetMySQLToDate(RString date)
{
	if(date.IsEmpty())
		return("1970-01-01");
	return(date);
}


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetNbSaved(tObjType type)
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

			case otLang:
				return(GetCount("languages"));

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
void GStorageMySQL::AssignId(GData* data,const GDict* dict)
{
	RString sSql;

	try
	{
		// Verify that the word didn't already exist.
		sSql=RString("SELECT kwdid FROM kwds WHERE kwd="+RQuery::SQLValue(data->GetName())+" and langid='"+dict->GetLang()->GetCode())+"'";
		RQuery find(Db,sSql);
		if(find.GetNbRows())
		{
			find.Start();
			data->SetId(strtoul(find[0],0,10));
			return;
		}

		// Insert the new word
		sSql=RString("INSERT INTO kwds(kwd,kwdid,type,langid) SELECT ")+RQuery::SQLValue(data->GetName())+",MAX(IFNULL(kwdid,0))+1,"+RQuery::SQLValue(itou(data->GetType()))+",'"+dict->GetLang()->GetCode()+"' FROM kwds WHERE langid='"+dict->GetLang()->GetCode()+"'";
		RQuery insert(Db,sSql);

		// Get the next id
		sSql=RString("SELECT kwdid FROM kwds WHERE kwdautoid=LAST_INSERT_ID()");

		RQuery getinsert(Db,sSql);
		getinsert.Start();
		data->SetId(strtoul(getinsert[0],0,10));
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GDoc* doc)
{
	RString sSql;

	try
	{
		#pragma Implement this method.
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadDic(GDict* &dic,GLang* lang,bool s)
{
	unsigned int MaxCount=100;
	unsigned int MaxId=0;
	unsigned int refdocs,refsubprofiles,refgroups;
	RString sSql;
	RString tbl;

	try
	{
		// If language is not defined -> do nothing
		if(!lang) return;

		// Construct the table name
		if(s)
		{
			tbl=RString("stopkwds");
			if(GSession::Get()&&GSession::Get()->GetSlot())
				GSession::Get()->GetSlot()->StartJob("Load "+lang->GetName().ToLower()+" stoplist");
		}
		else
		{
			tbl=RString("kwds");
			if(GSession::Get()&&GSession::Get()->GetSlot())
				GSession::Get()->GetSlot()->StartJob("Load "+lang->GetName().ToLower()+" dictionnary");
		}

		// Search the values to initialise the dictionary
		for(char i='a';i<='z';i++)
		{
			sSql="SELECT COUNT(*) FROM "+tbl+" WHERE langid='"+lang->GetCode()+"' AND kwd LIKE '"+RString(i)+"'";
			RQuery count(Db,sSql);
			count.Start();
			if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
		}
		if(MaxCount==0) MaxCount=2000;
		MaxId=GetMax(tbl,"kwdid");
		if(!MaxId)
			MaxId=2000;

		// Search the number of total references for the dictionary
		if(s)
		{
			refdocs=refsubprofiles=refgroups=0;
		}
		else
		{
			RQuery nb(Db,"SELECT refdocs,refsubprofiles,refgroups FROM languages WHERE langid='"+RString(lang->GetCode())+"'");
			nb.Start();
			if(nb.GetNb())
			{
				refdocs=atoi(nb[0]);
				refsubprofiles=atoi(nb[1]);
				refgroups=atoi(nb[2]);
			}
			else
			{
				// New language in the database -> create an entry
				refdocs=refsubprofiles=refgroups=0;
				RQuery newlang(Db,"INSERT INTO languages(langid,language,refdocs,refsubprofiles,refgroups) VALUES('"+RString(lang->GetCode())+"','"+lang->GetName()+"',0,0,0)");
			}
		}

		// Create and insert the dictionary
		if(s)
			dic=new GDict(tbl,"Stop List",lang,MaxId,MaxCount,true,refdocs,refsubprofiles,refgroups);
		else
			dic=new GDict(tbl,"Dictionary",lang,MaxId,MaxCount,false,refdocs,refsubprofiles,refgroups);

		// Load the dictionary from the database
		sSql="SELECT kwdid,kwd,type";
		if(!s)
			sSql+=",refdocs,refsubprofiles,refgroups";
		sSql+=" FROM "+tbl+" WHERE langid='"+lang->GetCode()+"'";
		RQuery dicts (Db, sSql);
		RString tmp;
		for(dicts.Start();!dicts.End();dicts.Next())
		{
			if(!s)
			{
				refdocs=atoi(dicts[3]);
				refsubprofiles=atoi(dicts[4]);
				refgroups=atoi(dicts[5]);
			}

			switch(atoi(dicts[2]))
			{
				case infoWord :
					{
						GWord w(atoi(dicts[0]),dicts[1],refdocs,refsubprofiles,refgroups);
						dic->InsertData(&w);
					}
					break;

				case infoWordList :
					{
						GWordList w(atoi(dicts[0]),dicts[1],refdocs,refsubprofiles,refgroups);
						sSql="SELECT kwdid FROM kwdsbygroups WHERE langid='"+RString(lang->GetCode())+"' AND grid="+itou(w.GetId());
						RQuery wl(Db,sSql);
						for(wl.Start();!wl.End();wl.Next())
							w.InsertWord(dynamic_cast <GWord*>(lang->GetDict()->GetData(atoi(wl[0]))));
						dic->InsertData(&w);
					}
					break;
			}
		}
		if(GSession::Get()&&GSession::Get()->GetSlot())
			GSession::Get()->GetSlot()->EndJob();
	}
	catch(RMySQLError e)
	{
		if(GSession::Get()&&GSession::Get()->GetSlot())
			GSession::Get()->GetSlot()->EndJob();
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadIndexer(GIndexer* &indexer,GLangManager* langs)
{
	unsigned int MaxCount=100;
	RString sSql;
	RString tbl;
	GWordOccurs* ptr;

	try
	{
		// Search the values to initialise the indexer
		for(char i='a';i<='z';i++)
		{
			sSql="SELECT COUNT(*) FROM kwds WHERE kwd LIKE '"+RString(i)+"'";
			RQuery count(Db,sSql);
			count.Start();
			if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
		}
		if(MaxCount==0) MaxCount=2000;

		// Create and insert the dictionary
		indexer=new GIndexer(MaxCount,langs);

		// Load the stems from the database
		sSql="SELECT kwd FROM kwds WHERE type='"+itou(infoWord)+"'";
		RQuery stems(Db, sSql);
		for(stems.Start();!stems.End();stems.Next())
		{
			indexer->InsertWord(stems[0]);
		}

		// Index the documents
		RQuery sel(Db,"SELECT htmlid,kwd FROM htmlsbykwds,kwds WHERE kwds.kwdid=htmlsbykwds.kwdid");
		for(sel.Start(),ptr=0;!sel.End();sel.Next())
		{
			// If not the same -> new wors
			if((!ptr)||(ptr->Compare(sel[1])!=0))
			{
				// If valid document -> assign the information to it
				ptr=indexer->GetWord(sel[1]);
			}
			ptr->AddDoc(atoi(sel[0]));
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
RString GStorageMySQL::LoadWord(unsigned int id,const char* code)
{
	try
	{
		RString res;
		RString sSql("SELECT kwd FROM kwds WHERE langid='"+RString(code)+"' AND kwdid="+itou(id));
		RQuery w(Db,sSql);
		if(w.GetNbRows())
		{
			w.Start();
			res=w[0];
		}
		return(res);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::LoadWord(const R::RString word,const char* code)
{
	try
	{
		unsigned int res=0;
		RString sSql("SELECT kwdid FROM kwds WHERE langid='"+RString(code)+"' AND kwd="+RQuery::SQLValue(word));
		RQuery w(Db,sSql);
		if(w.GetNbRows())
		{
			w.Start();
			res=atoi(w[0].Latin1());
		}
		return(res);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveData(GData* data,GLang* lang)
{
	RString Sql;

	// Delete the old word from the database
	RQuery Delete(Db,"DELETE FROM kwds WHERE langid='"+RString(lang->GetCode())+"' AND kwdid="+itou(data->GetId()));

	// Insert the new word in the database
	Sql="INSERT INTO kwds(kwdid,kwd,langid,type,refsubprofiles,refgroups,refdocs) ";
	Sql+="VALUES("+itou(data->GetId())+","+RQuery::SQLValue(data->GetName())+",'"+RString(lang->GetCode())+"',"+itou(data->GetType())+",0,0,0)";
	RQuery Insert(Db,Sql);

	// If wordlist -> save the rest
	if(data->GetType()==infoWordList)
	{
		GWordList* w=dynamic_cast<GWordList*>(data);
		R::RCursor<GWord> Cur(w->GetWords());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			Sql="INSERT INTO kwdsbygroups(grid,kwdid,langid) ";
			Sql+="VALUES ('"+itou(dynamic_cast<GData*>(w)->GetId())+"','"+itou(Cur()->GetId())+"','"+RString(lang->GetCode())+"')";
			RQuery insertword(Db,Sql);
		}
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GSubProfile* sub)
{
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveSubProfile(GSubProfile* sub)
{
	RString sSql;
	RCursor<GWeightInfo> Cur;
	RString l;

	try
	{
		// Test if the subprofile already exists.
		sSql="SELECT COUNT(1) FROM subprofiles WHERE subprofileid="+itou(sub->GetId());
		RQuery existsubprof(Db,sSql);
		existsubprof.Start();
		if(!atoi(existsubprof[0]))
		{
			// Insert the subprofile;
			sSql="INSERT INTO subprofiles SET profileid="+itou(sub->GetProfile()->GetId());
			sSql+=",langid='"+RString(sub->GetLang()->GetCode())+"',attached=CURDATE()";
			sSql+=",updated="+RQuery::SQLValue(sub->GetUpdated())+",calculated="+RQuery::SQLValue(sub->GetComputed())+",subprofileid="+itou(sub->GetId());
		}
		else
		{
			// Update the subprofile;
			sSql="UPDATE subprofiles SET updated="+RQuery::SQLValue(sub->GetUpdated())+",calculated="+RQuery::SQLValue(sub->GetComputed())+" WHERE subprofileid="+itou(sub->GetId());
		}
		RQuery updatesubprof(Db,sSql);

		// Delete old description
		l=sub->GetLang()->GetCode();
		sSql="DELETE FROM subprofilesbykwds WHERE langid='"+l+"' AND subprofileid="+itou(sub->GetId());
		RQuery deletekwds(Db,sSql);

		// Insert new description
		Cur=sub->GetInfos();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			sSql="INSERT INTO subprofilesbykwds(subprofileid,kwdid,weight, langid) VALUES("+itou(sub->GetId())+","+itou(Cur()->GetId())+",'"+dtou(Cur()->GetWeight())+"','"+l+"')";
			RQuery insertkwds(Db,sSql);
		}
		sub->SetState(osUpToDate);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveSubProfileInHistory(GSubProfile* sub,unsigned int historicID)
{
	RCursor<GWeightInfo> Cur;

	try
	{
		// Save subprofiles
		Cur=sub->GetInfos();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			RString sSql("INSERT INTO historicsubprofiles(historicID,subprofileid,kwdid,weight, date, langid) VALUES("+
					itou(historicID)+","+itou(sub->GetId())+","+itou(Cur()->GetId())+"'"+dtou(Cur()->GetWeight())+"',CURDATE()),'"+RString(sub->GetLang()->GetCode())+"'");
			RQuery insertkwds(Db,sSql);
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GUser* user)
{
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GProfile* p)
{
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveProfile(GProfile* prof)
{
	unsigned int profid;
	unsigned int social;
	RCursor<GSubProfile> CurSub=prof->GetSubProfiles();
	RString sSql;

	try
	{
		// Init
		profid=prof->GetId();
		if(prof->IsSocial())
			social=1;
		else
			social=0;

		// Test if the profile already exists.
		sSql="SELECT COUNT(1) FROM profiles WHERE profileid="+itou(profid);
		RQuery Test(Db,sSql);
		Test.Start();
		if(!atoi(Test[0]))
		{
			// Insert the profile (if subjects -> save topicid)
			sSql="INSERT INTO profiles(profileid,description,social";
			if(Session->GetSubjects(false))
				sSql+=",topicid";
			sSql+=" VALUES("+itou(profid)+","+RQuery::SQLValue(prof->GetName())+","+itou(social);
			if(Session->GetSubjects(false))
			{
				GSubject* sub=Session->GetSubjects(false)->GetSubject(prof);
				if(sub)
					sSql+=",topicid="+itou(sub->GetId());
				else
					sSql+=",topicid=0";
			}
			sSql+=")";
		}
		else
		{
			// Update the profile (if subjects -> save topicid)
			sSql="UPDATE profiles SET description="+RQuery::SQLValue(prof->GetName())+",social="+itou(social);
			if(Session->GetSubjects(false))
			{
				GSubject* sub=Session->GetSubjects(false)->GetSubject(prof);
				if(sub)
					sSql+=",topicid="+itou(sub->GetId());
				else
					sSql+=",topicid=0";
			}
			sSql+=" WHERE profileid="+itou(profid);
			RQuery Update(Db,sSql);
		}

		// First insert information from GFdbk
		RCursor<GFdbk> Fdbks=prof->GetFdbks();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Clear the feedback
			RQuery Delete(Db,"DELETE FROM htmlsbyprofiles WHERE profileid="+itou(profid)+" AND htmlid="+itou(Fdbks()->GetDocId()));
			// Re-Insert all the feedback
			sSql="INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2) ";
			sSql+="VALUES("+itou(Fdbks()->GetDocId())+",'"+GetAssessmentCode(Fdbks()->GetFdbk())+"',"+itou(prof->GetId())+","+RQuery::SQLValue(Fdbks()->GetWhen())+")";
			RQuery Insert(Db,sSql);
		}
		// Update other information from the documents
		RQuery Update(Db,"UPDATE htmls,htmlsbyprofiles SET htmlsbyprofiles.langid=htmls.langid, htmlsbyprofiles.computed=htmls.calculated WHERE htmlsbyprofiles.htmlid=htmls.htmlid AND profileid="+itou(prof->GetId()));
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GUser* GStorageMySQL::LoadUser(unsigned int userid)
{
	RQuery User(Db, "SELECT userid,user,fullname FROM users WHERE userid="+itou(userid));
	User.Start();
	if(!User.GetNb())
		return(0);
	return(new GUser(atoi(User[0]),User[1],User[2],10));
}


//------------------------------------------------------------------------------
GProfile* GStorageMySQL::LoadProfile(unsigned int profileid)
{
	GLangManager* Langs=GPluginManagers::GetManager<GLangManager>("Lang");

	// Load Profile
	RQuery Profile(Db,"SELECT profileid,description,social,userid FROM profiles WHERE profileid="+itou(profileid));
	Profile.Start();
	if(!Profile.GetNb())
		return(0);
	GUser* user=Session->GetUser(atoi(Profile[3]));
	if(!user)
		throw GException("Profile "+Profile[0]+" has no parent user");

	// Create the profile
	GProfile* prof=new GProfile(user,atoi(Profile[0]),Profile[1],(atoi(Profile[2])==1),5);

	// Load Feedbacks
	RQuery fdbks(Db,"SELECT htmlid,judgement,profileid,when2,computed,langid FROM htmlsbyprofiles WHERE profileid="+itou(profileid));
	for(fdbks.Start();!fdbks.End();fdbks.Next())
	{
		Session->InsertFdbk(atoi(fdbks[2]),atoi(fdbks[0]),Langs->GetPlugIn(fdbks[5]),GetAssessmentType(fdbks[1]),RDate(fdbks[3]),RDate(fdbks[4]));
		// Since the profile is not in the session -> we must manually insert the profile.
		prof->InsertFdbk(atoi(fdbks[0]),Langs->GetPlugIn(fdbks[5]),GetAssessmentType(fdbks[1]),RDate(fdbks[3]),RDate(fdbks[4]));
	}

	return(prof);
}


//------------------------------------------------------------------------------
GSubProfile* GStorageMySQL::LoadSubProfile(unsigned int subprofileid)
{
	RQuery SubProfile(Db,"SELECT subprofileid,langid,attached,groupid,updated,calculated,profileid FROM subprofiles WHERE subprofileid="+itou(subprofileid));
	SubProfile.Start();
	if(!SubProfile.GetNb())
		return(0);
	GLang *lang=GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIn(SubProfile[1]);
	if(!lang)
		return(0);
	GProfile* prof=Session->GetProfile(atoi(SubProfile[6]));
	if(!prof)
		throw GException("Subprofile "+SubProfile[0]+" has no parent profile");
	unsigned int groupid=atoi(SubProfile[3]);
	if(!groupid)
		groupid=cNoRef;
	GSubProfile* sub=new GSubProfile(prof,atoi(SubProfile[0]),lang,groupid,SubProfile[2],GetMySQLToDate(SubProfile[4]),GetMySQLToDate(SubProfile[5]));
	sub->SetState(osNeedLoad);
	return(sub);
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadUsers(void)
{
	GProfile* prof;
	GLang* lang;
	GSubProfile* sub;
	GLangManager* Langs=GPluginManagers::GetManager<GLangManager>("Lang");

	try
	{
		if(IsAllInMemory()) // If everything must be in memory -> load all the users and the profiles
		{
			// Load users
			RQuery Users(Db, "SELECT userid,user,fullname FROM users");
			for(Users.Start();!Users.End();Users.Next())
				Session->InsertUser(new GUser(atoi(Users[0]),Users[1],Users[2],10));

			// Load profiles
			RQuery Profiles(Db,"SELECT profileid,description,social,userid FROM profiles");
			for(Profiles.Start();!Profiles.End();Profiles.Next())
			{
				GUser* user=Session->GetUser(atoi(Profiles[3]));
				Session->InsertProfile(new GProfile(user,atoi(Profiles[0]),Profiles[1],(atoi(Profiles[2])==1),5));
			}

			// Load feedbacks
			RQuery fdbks(Db,"SELECT htmlid,judgement,profileid,when2,computed,langid FROM htmlsbyprofiles");
			for(fdbks.Start();!fdbks.End();fdbks.Next())
				Session->InsertFdbk(atoi(fdbks[2]),atoi(fdbks[0]),Langs->GetPlugIn(fdbks[5]),GetAssessmentType(fdbks[1]),RDate(fdbks[3]),RDate(fdbks[4]));
		}

		// Load subprofiles
		RString Sql("SELECT subprofileid,langid,attached,groupid,updated,calculated,profileid FROM subprofiles");
		if(!LoadAll)
			Sql+=" WHERE calculated<updated";
		if(Filtering)
		{
			if(LoadAll)
				Sql+=" WHERE ";
			else
				Sql+=" OR ";
			Sql+="((updated>="+RQuery::SQLValue(Filter)+") OR (calculated>="+RQuery::SQLValue(Filter)+") OR (attached>="+RQuery::SQLValue(Filter)+"))";
		}
		RQuery SubProfiles(Db,Sql);
		for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
		{
			lang=Langs->GetPlugIn(SubProfiles[1]);
			if(!lang)
				continue;
			prof=Session->GetProfile(atoi(SubProfiles[6]));
			if(!prof)
				throw GException("Subprofile "+SubProfiles[0]+" has no parent profile");
			unsigned int groupid=atoi(SubProfiles[3]);
			if(!groupid)
				groupid=cNoRef;
			Session->InsertSubProfile(sub=new GSubProfile(prof,atoi(SubProfiles[0]),lang,groupid,SubProfiles[2],GetMySQLToDate(SubProfiles[4]),GetMySQLToDate(SubProfiles[5])));
			sub->SetState(osNeedLoad);
		}
	}
	catch(RMySQLError& e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadSubjects(void)
{
	RString sSql;
	GSubject* subject;
	GSubject* subsubject;

	try
	{
		// Verify the subjects exist and clear them
		if(!Session->GetSubjects()) return;

		if(Session->GetSlot())
			Session->GetSlot()->StartJob("Load subjects");

		Session->GetSubjects()->Clear();

		// Load the subjects
		RQuery sub(Db,"SELECT topicid,name,used FROM topics WHERE parent=0");
		for(sub.Start();!sub.End();sub.Next())
		{
			Session->GetSubjects()->InsertNode(0,subject=new GSubject(Session->GetSubjects(),atoi(sub[0]),sub[1],atoi(sub[2])));
			sSql="SELECT topicid,name,used FROM topics WHERE parent="+sub[0];
			RQuery subsub(*Db,sSql);
			for(subsub.Start();!subsub.End();subsub.Next())
				Session->GetSubjects()->InsertNode(subject,subsubject=new GSubject(Session->GetSubjects(),atoi(subsub[0]),subsub[1],atoi(subsub[2])));
		}

		//  Make Link between documents and subjects
		RQuery docs(Db,"SELECT htmlid,topicid FROM topicsbyhtmls");
		for(docs.Start();!docs.End();docs.Next())
		{
			GDoc* d=Session->GetDoc(atoi(docs[0]));
			if(!d) continue;
			Session->GetSubjects()->InsertDocSubject(d,atoi(docs[1]));
		}

		// Make links between profiles and subjects
		RQuery profiles(Db,"SELECT profileid,topicid FROM profiles");
		for(profiles.Start();!profiles.End();profiles.Next())
		{
			GProfile* prof=Session->GetProfile(atoi(profiles[0]));
			if(!prof) continue;
			Session->GetSubjects()->InsertProfileSubject(prof,atoi(profiles[1]));
		}
		if(Session->GetSlot())
			Session->GetSlot()->EndJob();
	}
	catch(RMySQLError e)
	{
		if(Session->GetSlot())
			Session->GetSlot()->EndJob();
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveRefs(tObjType type,GLang* lang,size_t id,size_t refs)
{
	RString sSql;

	switch(type)
	{
		case otDoc:
			sSql="UPDATE kwds SET refdocs="+itou(refs)+" WHERE kwdid="+itou(id)+" AND langid='"+lang->GetCode()+"'";
			break;
		case otSubProfile:
			sSql="UPDATE kwds SET refsubprofiles="+itou(refs)+" WHERE kwdid="+itou(id)+" AND langid='"+lang->GetCode()+"'";
			break;
		case otGroup:
			sSql="UPDATE kwds SET refgroups="+itou(refs)+" WHERE kwdid="+itou(id)+" AND langid='"+lang->GetCode()+"'";
			break;
		default:
			throw GException("This type of objects do not have descriptions");
	};
	RQuery(Db,sSql);
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveRefs(tObjType type,GLang* lang,size_t refs)
{
	RString sSql;

	switch(type)
	{
		case otDoc:
			sSql="UPDATE languages SET refdocs="+itou(refs)+" WHERE langid='"+lang->GetCode()+"'";
			break;
		case otSubProfile:
			sSql="UPDATE languages SET refsubprofiles="+itou(refs)+" WHERE langid='"+lang->GetCode()+"'";
			break;
		case otGroup:
			sSql="UPDATE languages SET refgroups="+itou(refs)+" WHERE langid='"+lang->GetCode()+"'";
			break;
		default:
			throw GException("This type of objects do not have descriptions");
	};
	RQuery(Db,sSql);
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadInfos(RContainer<GWeightInfo,false,true>& infos,GLang* lang,tObjType type,size_t id)
{
	size_t i,kwdid;
	tInfoType InfoType;
	RString sSql;

	switch(type)
	{
		case otDoc:
			sSql="SELECT kwdid,occurs FROM htmlsbykwds WHERE htmlid="+itou(id)+" ORDER BY kwdid";
			break;
		case otSubProfile:
			sSql="SELECT kwdid,weight FROM subprofilesbykwds WHERE subprofileid="+itou(id)+" ORDER BY kwdid";
			break;
		case otGroup:
			sSql="SELECT kwdid,occurs FROM groupsbykwds WHERE groupid="+itou(id)+" ORDER BY kwdid";
			break;
		default:
			throw GException("This type of objects do not have descriptions");
	};
	RQuery sel(Db,sSql);
	for(sel.Start(),i=0;!sel.End();sel.Next(),i++)
	{
		kwdid=atoi(sel[0]);
		if(lang->GetDict())
		{
			GData* data=lang->GetDict()->GetData(kwdid);
			if(data)
				InfoType=data->GetType();
			else
				InfoType=infoNothing;
		}
		else
			InfoType=infoNothing;
		infos.InsertPtrAt(new GWeightInfo(kwdid,atof(sel[1]),InfoType),i,false);
	}
}


//------------------------------------------------------------------------------
GDoc* GStorageMySQL::LoadDoc(unsigned int docid)
{
	GDoc* doc;

	RQuery quer (Db,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed,owner FROM htmls WHERE htmlid="+itou(docid));
	quer.Start();
	if(!quer.GetNb())
		return(0);

	// Verify that the langague is active
	GLang* lang=GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIn(quer[4]);
	if((!lang)&&(!quer[4].IsEmpty()))
		return(0);

	doc=new GDoc(quer[1],quer[2],docid,lang,quer[3],GetMySQLToDate(quer[5]),GetMySQLToDate(quer[6]),atoi(quer[7]),atoi(quer[8]));
	doc->SetState(osNeedLoad);

	// Load the links of the document loaded.
	RQuery querLinks (Db,"SELECT htmlid,linkid,occurs FROM htmlsbylinks WHERE htmlid="+itou(docid));
	for(querLinks.Start(); !querLinks.End() ; querLinks.Next())
		doc->InsertLink(Session->GetDoc(atoi(querLinks[1])), atoi(querLinks[2]));

	return(doc);
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadDocs(void)
{
	GDoc* doc;
	GLang* lang;
	unsigned int docid;

	try
	{
		RString Sql("SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed,owner FROM htmls");
		if(!LoadAll)
			Sql+=" WHERE calculated<updated";
		if(Filtering)
		{
			if(LoadAll)
				Sql+=" WHERE ";
			else
				Sql+=" OR ";
			Sql+="((updated>="+RQuery::SQLValue(Filter)+") OR (calculated>="+RQuery::SQLValue(Filter)+"))";
		}
		RQuery quer(Db,Sql);
		for(quer.Start();!quer.End();quer.Next())
		{
			// Verify that the langague is active
			lang=GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIn(quer[4]);
			if((!lang)&&(!quer[4].IsEmpty()))
				continue;

			docid=atoi(quer[0]);
			doc=new GDoc(quer[1],quer[2],docid,lang,quer[3],GetMySQLToDate(quer[5]),GetMySQLToDate(quer[6]),atoi(quer[7]),atoi(quer[8]));
			Session->InsertDoc(doc);
			doc->SetState(osNeedLoad);

			// Load the links of the document loaded.
			RQuery querLinks (Db,"SELECT htmlid,linkid,occurs FROM htmlsbylinks WHERE htmlid="+itou(docid));
			for(querLinks.Start(); !querLinks.End() ; querLinks.Next())
				doc->InsertLink(Session->GetDoc(atoi(querLinks[1])), atoi(querLinks[2]));
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}

}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveDoc(GDoc* doc)
{
	RString sSql;
	RString l;
	RString id;
	RString f;
	RCursor<GWeightInfo> Words;
	R::RCursor<GLink> lcur;

	try
	{
		id=itou(doc->GetId());
		// Delete keywords
		if(doc->GetLang())
		{
			l=doc->GetLang()->GetCode();
			sSql="DELETE FROM htmlsbykwds WHERE langid='"+l+"' AND htmlid="+id;
			RQuery deletekwds(Db,sSql);
			Words=doc->GetInfos();
			for(Words.Start();!Words.End();Words.Next())
			{
				sSql="INSERT INTO htmlsbykwds(htmlid,kwdid,occurs, langid) VALUES("+id+","+itou(Words()->GetId())+",'"+dtou(Words()->GetWeight())+"','"+l+"')";
				RQuery insertkwds(Db,sSql);
			}
			l=RQuery::SQLValue(l);
		}
		else
		{
			l="NULL";
		}

		// Update document
		f=doc->GetMIMEType();
		if(f.IsEmpty())
			f="NULL";
		else
			f=RQuery::SQLValue(f);
		sSql="UPDATE htmls SET html="+RQuery::SQLValue(doc->GetURL())+",title="+RQuery::SQLValue(doc->GetName())+",mimetype="+f+
				",langid="+l+",updated="+RQuery::SQLValue(doc->GetUpdated())+",calculated="+RQuery::SQLValue(doc->GetComputed())+
				",failed="+itou(doc->GetFailed())+" WHERE htmlid="+id;
		RQuery updatedoc(Db,sSql);

		// Update links to others documents
		RQuery deletelinks(Db,"DELETE FROM htmlsbylinks WHERE htmlid="+id);
		lcur= doc->GetLinks();
		for ( lcur.Start(); ! lcur.End(); lcur.Next())
		{
			sSql="INSERT INTO htmlsbylinks(htmlid,linkid,occurs) VALUES("+id+","+itou(lcur()->GetId())+","+itou(lcur()->GetOccurs())+")";
			RQuery insertkwds(Db,sSql);
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GGroup* grp)
{
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveGroups(void)
{
	RCursor<GWeightInfo> WordCur;
	R::RCursor<GGroup> GroupsCursor;
	RString sSql;
	R::RCursor<GFactoryLang> langs;
	GLang* lang;
	RCursor<GSubProfile> Sub;

	try
	{
		RQuery delete1(Db,"DELETE FROM groups");
		langs=GPluginManagers::GetManager<GLangManager>("Lang")->GetFactories();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			sSql="DELETE FROM groupsbykwds WHERE langid='"+RString(lang->GetCode())+"'";
			RQuery delete2(Db,sSql);
		}

		GroupsCursor=Session->GetGroups();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			sSql="INSERT INTO groups(groupid,langid,updated,calculated) VALUES("+itou(GroupsCursor()->GetId())+",'"+RString(GroupsCursor()->GetLang()->GetCode())+"',"+RQuery::SQLValue(GroupsCursor()->GetUpdated())+","+RQuery::SQLValue(GroupsCursor()->GetComputed())+")";
			RQuery insert1(Db,sSql);

			// Save SubProfiles infos
			Sub=GroupsCursor()->GetSubProfiles();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sSql="UPDATE subprofiles SET groupid="+itou(GroupsCursor()->GetId())+",attached="+RQuery::SQLValue(Sub()->GetAttached())+" WHERE subprofileid="+itou(Sub()->GetId());
				RQuery update(Db,sSql);
			}

			// Save the description part
			WordCur.Set(*GroupsCursor());
			for(WordCur.Start();!WordCur.End();WordCur.Next())
			{
				sSql="INSERT INTO groupsbykwds(groupid,kwdid,occurs,langid) VALUES("+itou(GroupsCursor()->GetId())+","+itou(WordCur()->GetId())+",'"+dtou(WordCur()->GetWeight())+"','"+RString(GroupsCursor()->GetLang()->GetCode())+"')";
				RQuery InserinfoWord(Db,sSql);
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveGroupsHistory(void)
{
	R::RCursor<GGroup> GroupsCursor;
	RCursor<GSubProfile> Sub;
	unsigned int historicID;
	RString sSql;

	try
	{
		// if historic needed, gget the last historic id
		historicID=0;
		RQuery histcount(Db,"SELECT MAX(historicID) from historicgroups");
		histcount.Start();
		if(histcount[0])
			historicID=atoi(histcount[0]);
		historicID++;

		// save the groups in history
		GroupsCursor=Session->GetGroups();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			Sub=GroupsCursor()->GetSubProfiles();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sSql="INSERT INTO historicgroups SET date=CURDATE(), historicID="+itou(historicID)+",groupid="+itou(GroupsCursor()->GetId())+
						", subprofileid="+itou(Sub()->GetId())+",lang='"+RString(GroupsCursor()->GetLang()->GetCode())+"'";
				RQuery history(Db,sSql);
			}
		}

		//save the profiles history if needed.
		SaveHistoricProfiles(historicID);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveHistoricProfiles(unsigned int historicID)
{
	try
	{
		R::RCursor<GProfile> curProf=Session->GetProfiles() ;

		// Save the Subprofile
		for(curProf.Start();!curProf.End();curProf.Next())
		{
			RCursor<GSubProfile> curSub=curProf()->GetSubProfiles();
			for(curSub.Start();!curSub.End();curSub.Next())
				SaveSubProfileInHistory(curSub(), historicID);
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::UpdateProfiles(unsigned int docid,GLang* lang)
{
	RQuery Up(Db,"UPDATE htmlsbyprofiles SET computed=CURDATE(),langid="+RQuery::SQLValue(lang->GetCode())+" WHERE htmlid="+itou(docid));
}


//------------------------------------------------------------------------------
void GStorageMySQL::UpdateGroups(unsigned int subid)
{
	RQuery Up(Db,"UPDATE groups,subprofiles SET groups.updated=CURDATE() WHERE groups.groupid=subprofiles.groupid AND subprofiles.subprofileid="+itou(subid));
}


//------------------------------------------------------------------------------
GGroup* GStorageMySQL::LoadGroup(unsigned int groupid)
{
	RQuery Group(Db,"SELECT groupid,langid,updated,calculated FROM groups WHERE groupid="+itou(groupid));
	Group.Start();
	if(!Group.GetNb())
		return(0);
	GLang* lang=GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIn(Group[1]);
	if(!lang)
		return(0);
	GGroup* group=new GGroup(atoi(Group[0]),lang,true,Group[2],Group[3]);
	group->SetState(osNeedLoad);
	return(group);
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadGroups(void)
{
	GGroup* group;
	R::RCursor<GGroup> GroupsCursor;
	GLang* lang;
	R::RCursor<GFactoryLang> langs;

	try
	{
		RString Sql("SELECT groupid,langid,updated,calculated FROM groups");
		if(!LoadAll)
			Sql+=" WHERE calculated<updated";
		if(Filtering)
		{
			if(LoadAll)
				Sql+=" WHERE ";
			else
				Sql+=" OR ";
			Sql+="((updated>="+RQuery::SQLValue(Filter)+") OR (calculated>="+RQuery::SQLValue(Filter)+"))";
		}
		RQuery Groups(Db,Sql);
		for(Groups.Start();!Groups.End();Groups.Next())
		{
			lang=GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIn(Groups[1]);
			if(!lang)
				continue;
			group=new GGroup(atoi(Groups[0]),lang,true,Groups[2],Groups[3]);
			group->SetState(osNeedLoad);
			Session->InsertGroup(group);
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::ExecuteData(const R::RString& filename)
{
	try
	{
		RString l;
		RTextFile Sql(filename);
		Sql.Open(RIO::Read);
		while(!Sql.Eof())
		{
			l=Sql.GetLine();
			RQuery exec(Db,l);
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
GGroupsHistory* GStorageMySQL::LoadAnHistoricGroups(unsigned int historicID)
{
	RString sSql;
	GGroupHistory* grp;
	GGroupsHistory* grps;
	GWeightInfosHistory* historicsubprof;
	unsigned int subprofid;
	unsigned int groupid;
	unsigned int v;
	GLang* lang;

	try
	{
		// Init part
		groupid=cNoRef;

		sSql="SELECT date FROM historicgroups WHERE historicID="+itou(historicID);
		RQuery date(Db,sSql);
		date.Start();
		grps=new GGroupsHistory(historicID, date[0]);

		//read the groupment.
		sSql="SELECT groupid,subprofileid,lang,date FROM historicgroups WHERE historicID="+itou(historicID)+" ORDER by historicID,groupid";
		RQuery grquery(Db,sSql);
		for(grquery.Start(),grp=0;!grquery.End();grquery.Next())
		{
			// Read Group
			v=atoi(grquery[0]);

			//get lang
			lang=GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIn(grquery[2]);

			// If group id changed -> new group needed
			if((v!=groupid))
			{
				groupid=v;
				grp=new GGroupHistory(groupid,lang, grps);
				//insert group in the container of groups.
				grps->InsertPtr(grp);
			}

			// Create the historic subprofile and add it to the group
			subprofid=atoi(grquery[1]);
			historicsubprof=new GWeightInfosHistory(Session->GetSubProfile(subprofid,0),100);
			grp->AddSubProfile(historicsubprof);
			historicsubprof->SetParent(grp);

			// fill the vector of the subprofile
			sSql="SELECT kwdid,weight FROM historicsubprofiles WHERE langid='"+RString(lang->GetCode())+"' AND historicID="+itou(historicID)+" AND subprofileid="+itou(subprofid);
			RQuery subprofdesc(*Db,sSql);
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
void GStorageMySQL::LoadHistoricGroupsByDate(R::RString mindate, R::RString maxdate)
{
	RString sSql;

	sSql="SELECT DISTINCT historicID from historicgroups where date>'"+mindate+"' AND date<'"+maxdate+"' ORDER BY date,historicID";
	RQuery ids(Db,sSql);
	for (ids.Start(); !ids.End(); ids.Next())
		Session->GetGroupsHistoryManager()->InsertGroupsHistory(LoadAnHistoricGroups(atoi(ids[0])));
}


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetHistorySize(void)
{
	try
	{
		RQuery size(Db,"SELECT COUNT(DISTINCT historicID) from historicgroups");
		size.Start();
		return(atoi(size[0]));
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::CreateSugs(const R::RString& name)
{
	RString sSql;

	try
	{
		// Create table if it doesn't already exist
		sSql="CREATE TABLE IF NOT EXISTS sugsbyprofiles (profileid INT(11), htmlid INT(11), rank INT(11), test TEXT)";
		RQuery create(Db,sSql);
		sSql="CREATE TABLE IF NOT EXISTS sugsbygroups (groupid INT(11), htmlid INT(11), rank INT(11), test TEXT)";
		RQuery create2(Db,sSql);
		sSql="DELETE FROM sugsbyprofiles";
		if(!name.IsEmpty())
			sSql+=" WHERE test="+RQuery::SQLValue(name);
		RQuery create3(Db,sSql);
		sSql="DELETE FROM sugsbygroups";
		if(!name.IsEmpty())
			sSql+=" WHERE test="+RQuery::SQLValue(name);
		RQuery create4(Db,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddSugsProfile(const R::RString& name,unsigned int profileid,unsigned int docid,unsigned int rank)
{
	RString sSql;

	try
	{
		sSql="INSERT INTO sugsbyprofiles(profileid,htmlid,rank,test) VALUES("+
			itou(profileid)+","+itou(docid)+","+itou(rank)+","+RQuery::SQLValue(name)+")";
		RQuery create(Db,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddSugsGroup(const R::RString& name,unsigned int groupid,unsigned int docid,unsigned int rank)
{
	RString sSql;

	try
	{
		sSql="INSERT INTO sugsbygroups(groupid,htmlid,rank,test) VALUES("+
			itou(groupid)+","+itou(docid)+","+itou(rank)+","+RQuery::SQLValue(name)+")";
		RQuery create(Db,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::GetSugsProfiles(const R::RString& name,R::RContainer<GSugs,true,false>& res)
{
	RString sSql;
	unsigned int profileid,idx;
	GSugs* sugs;

	try
	{
		res.Clear();
		sSql="SELECT profileid,htmlid,rank,test FROM sugsbyprofiles WHERE test="+RQuery::SQLValue(name)+" ORDER BY profileid,rank";
		RQuery load(Db,sSql);
		for(load.Start(),profileid=cNoRef;!load.End();load.Next())
		{
			idx=atoi(load[0]);

			// If not the same -> new profile
			if(idx!=profileid)
			{
				// If valid profile -> assign the information to it
				if(profileid!=cNoRef)
					res.InsertPtr(sugs);
				profileid=idx;
				sugs=new GSugs(otProfile,profileid,50);
			}
			sugs->AddSugs(atoi(load[1]));
		}
		if(profileid!=cNoRef)
			res.InsertPtr(sugs);

	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::GetSugsGroups(const R::RString& name,R::RContainer<GSugs,true,false>& res)
{
	RString sSql;
	unsigned int groupid,idx;
	GSugs* sugs;

	try
	{
		res.Clear();
		sSql="SELECT groupid,htmlid,rank,test FROM sugsbygroups ORDER BY groupid,rank WHERE test="+RQuery::SQLValue(name);
		RQuery load(Db,sSql);
		for(load.Start(),groupid=cNoRef;!load.End();load.Next())
		{
			idx=atoi(load[0]);

			// If not the same -> new group
			if(idx!=groupid)
			{
				// If valid group -> assign the information to it
				if(groupid!=cNoRef)
					res.InsertPtr(sugs);
				groupid=idx;
				sugs=new GSugs(otGroup,groupid,50);
			}
			sugs->AddSugs(atoi(load[1]));
		}
		if(groupid!=cNoRef)
			res.InsertPtr(sugs);

	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::GetSugsTests(R::RContainer<R::RString,true,true>& res)
{
	RString sSql;

	try
	{
		res.Clear();
		sSql="SELECT DISTINCT(test) FROM sugsbyprofiles";
		RQuery profilestests(Db,sSql);
		for(profilestests.Start();!profilestests.End();profilestests.Next())
			res.GetInsertPtr<RString>(profilestests[0]);
		sSql="SELECT DISTINCT(test) FROM sugsbygroups";
		RQuery groupstests(Db,sSql);
		for(groupstests.Start();!groupstests.End();groupstests.Next())
			res.GetInsertPtr<RString>(groupstests[0]);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::CreateDummy(RString name)
{
	RString sSql;

	try
	{
		// Create table if it doesn't already exist
		sSql="CREATE TABLE IF NOT EXISTS "+name+" (id INT(11), parentid INT(11), description TEXT)";
		RQuery create(Db,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::ClearDummy(RString name)
{
	try
	{
		// Delete the table
		RQuery del(Db,"DELETE FROM "+name);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddDummyEntry(RString name,unsigned int id,RString desc,unsigned int parentid)
{
	RString sSql;

	try
	{
		sSql="INSERT INTO "+name+" (id, parentid, description) VALUES("+itou(id)+","+itou(parentid)+","+RQuery::SQLValue(desc)+")";
		RQuery add(Db,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
RQuery* GStorageMySQL::SelectDummyEntry(RString name,unsigned int id,RString desc,unsigned int parentid,unsigned int filter)
{
	RString sSql;
	RQuery* select;

	switch(filter)
	{
		case 0: // no filter
			sSql="SELECT * FROM "+name;
			break;
		case 1: //filter on id
			sSql="SELECT * FROM "+name+" WHERE id="+itou(id);
			break;
		case 2: //filter on description
			sSql= "SELECT * FROM "+name+" WHERE description="+RQuery::SQLValue(desc);
			break;
		case 3: //filter on id & desc
			sSql="SELECT * FROM "+name+" WHERE id="+itou(id)+" AND description="+RQuery::SQLValue(desc);
			break;
		case 4: //filter on parentid
			sSql="SELECT * FROM "+name+" WHERE parentid="+itou(parentid);
			break;
		case 5: //filter on id & parentid
			sSql="SELECT * FROM "+name+" WHERE id="+itou(id)+" AND parentid="+itou(parentid);
			break;
		case 6: //filter on desc & parentid
			sSql="SELECT * FROM "+name+" WHERE description="+RQuery::SQLValue(desc)+" AND parentid="+itou(parentid);
			break;
		case 7: //filter on id &desc & parentid
			sSql="SELECT * FROM "+name+" WHERE id="+itou(id)+" AND description="+RQuery::SQLValue(desc)+" AND parentid="+itou(parentid);
			break;
	}
	try
	{
		select=new RQuery(Db,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}

	return(select);
}


//------------------------------------------------------------------------------
void GStorageMySQL::ClearDummyEntry(RString name,unsigned int id,RString desc, unsigned int parentid,unsigned int filter)
{
	RString sSql;

	switch(filter)
	{
		case 0: // no filter
			sSql="DELETE FROM "+name;
			break;
		case 1: //filter on id
			sSql="DELETE FROM "+name+" WHERE id="+itou(id);
			break;
		case 2: //filter on description
			sSql= "DELETE FROM "+name+" WHERE description="+RQuery::SQLValue(desc);
			break;
		case 3: //filter on id & desc
			sSql="DELETE FROM "+name+" WHERE id="+itou(id)+" AND description="+RQuery::SQLValue(desc);
			break;
		case 4: //filter on parentid
			sSql="DELETE FROM "+name+" WHERE parentid="+itou(parentid);
			break;
		case 5: //filter on id & parentid
			sSql="DELETE FROM "+name+" WHERE id="+itou(id)+" AND parentid="+itou(parentid);
			break;
		case 6: //filter on desc & parentid
			sSql="DELETE FROM "+name+" WHERE description="+RQuery::SQLValue(desc)+" AND parentid="+itou(parentid);
			break;
		case 7: //filter on id &desc & parentid
			sSql="DELETE FROM "+name+" WHERE id="+itou(id)+" AND description="+RQuery::SQLValue(desc)+" AND parentid="+itou(parentid);
			break;
	}

	try
	{
		RQuery select(Db,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamString("Host","127.0.0.1"));
	params->InsertPtr(new GParamString("User","root"));
	params->InsertPtr(new GParamString("Password",""));
	params->InsertPtr(new GParamString("Database",""));
	params->InsertPtr(new GParamBool("Filtering",false));
	params->InsertPtr(new GParamString("Filter",""));
	params->InsertPtr(new GParamString("Encoding","Latin1"));
	params->InsertPtr(new GParamBool("All",true));
}


//------------------------------------------------------------------------------
GStorageMySQL::~GStorageMySQL(void)
{
}


//------------------------------------------------------------------------------
CREATE_STORAGE_FACTORY("MySQL",GStorageMySQL)


//------------------------------------------------------------------------------
void TheFactory::UpdateSchema(const R::RXMLStruct&)
{
}
