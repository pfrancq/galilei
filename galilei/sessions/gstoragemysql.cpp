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
#include <infos/gdict.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <infos/ginfo.h>
#include <infos/ginfolist.h>
#include <infos/gword.h>
#include <infos/gwordlist.h>
#include <infos/gweightinfo.h>
#include <infos/gweightinfos.h>
#include <docs/gdocproxy.h>
#include <docs/glink.h>
#include <docs/gsugs.h>
#include <profiles/guser.h>
#include <profiles/gusers.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <sessions/gstoragemysql.h>
#include <sessions/gslot.h>
#include <groups/ggroup.h>
#include <groups/gsubjects.h>
#include <groups/gsubject.h>
#include <sessions/gsession.h>
#include <historic/ggroupshistory.h>
#include <historic/gweightinfoshistory.h>
#include <engines/gindexer.h>
#include <engines/gwordoccurs.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GStorageMySQL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStorageMySQL::GStorageMySQL(RString host,RString user,RString pwd,RString db,RString coding) throw(std::bad_alloc,GException)
	: GStorage(db), Db(0)
{
	try
	{
		Db.reset(new RDb(host,user,pwd,db,coding));
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetCount(RString tbl) throw(RMySQLError)
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
unsigned int GStorageMySQL::GetMax(RString tbl,RString fld) throw(RMySQLError)
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
void GStorageMySQL::LoadDic(GDict* &dic,GLang* lang,bool s) throw(std::bad_alloc,GException)
{
	unsigned int MaxCount=100;
	unsigned int MaxId=0;
	RString sSql;
	RString tbl;

	try
	{
		// If language is not defined -> do nothing
		if(!lang) return;

		// Construct the table name
		if(s)
			tbl=RString("stopkwds");
		else
			tbl=RString("kwds");

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

		// Create and insert the dictionary
		if(s)
			dic=new GDict(tbl,"Stop List",lang,MaxId,MaxCount,true);
		else
			dic=new GDict(tbl,"Dictionary",lang,MaxId,MaxCount,false);

		// Load the dictionary from the database
		sSql="SELECT kwdid,kwd,type FROM "+tbl+" WHERE langid='"+lang->GetCode()+"'";
		RQuery dicts (Db, sSql);
		RString tmp;
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
RString GStorageMySQL::LoadWord(unsigned int id,const char* code) throw(std::bad_alloc,GException)
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
unsigned int GStorageMySQL::LoadWord(const R::RString word,const char* code) throw(std::bad_alloc,GException)
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
void GStorageMySQL::LoadWordList(GWordList* w,GLang* lang) throw(std::bad_alloc,GException)
{
	try
	{
		// If language is not defined -> do nothing
		if(!lang) return;

		RString sSql("SELECT kwdid FROM kwdsbygroups WHERE langid='"+RString(lang->GetCode())+"' AND grid="+itou(dynamic_cast<GData*>(w)->GetId()));
		RQuery wl(Db,sSql);
		for(wl.Start();!wl.End();wl.Next())
			w->InsertWord(dynamic_cast <GWord*>(lang->GetDict()->GetData(atoi(wl[0]))));
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveWordList(GDict* dic,GWordList* w) throw(GException)
{
	R::RCursor<GWord> Cur;

	try
	{
		Cur=w->GetWordCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			RString sSql("INSERT INTO kwdsbygroups(grid,kwdid,langid) VALUES ('"+itou(dynamic_cast<GData*>(w)->GetId())+"','"+itou(Cur()->GetId())+"','"+RString(dic->GetLang()->GetCode())+"')");
			try
			{
				RQuery insertword(Db,sSql);
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
	RString sSql;
	RCursor<GWeightInfo> Cur;
	RString l;

	try
	{
		//test if the subprofile already exists.
		sSql="SELECT COUNT(1) FROM subprofiles WHERE subprofileid="+itou(sub->GetId());
		RQuery existsubprof(Db,sSql);
		existsubprof.Start();
		if(!atoi(existsubprof[0]))
		{
			//insert the subprofile;
			sSql="INSERT INTO subprofiles SET profileid="+itou(sub->GetProfile()->GetId());
			sSql+=",langid='"+RString(sub->GetLang()->GetCode())+"',attached=CURDATE()";
			sSql+=",updated="+RQuery::SQLValue(sub->GetUpdated())+",calculated="+RQuery::SQLValue(sub->GetComputed())+",subprofileid="+itou(sub->GetId());
		}
		else
		{
			//update the subprofile;
			sSql="UPDATE subprofiles SET updated="+RQuery::SQLValue(sub->GetUpdated())+",calculated="+RQuery::SQLValue(sub->GetComputed())+" WHERE subprofileid="+itou(sub->GetId());
		}
		RQuery updatesubprof(Db,sSql);

		// Delete keywords
		l=sub->GetLang()->GetCode();

		sSql="DELETE FROM subprofilesbykwds WHERE langid='"+l+"' AND subprofileid="+itou(sub->GetId());
		RQuery deletekwds(Db,sSql);

		Cur=sub->GetWeightInfoCursor();
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
void GStorageMySQL::SaveSubProfileInHistory(GSubProfile* sub,unsigned int historicID) throw(GException)
{
	RCursor<GWeightInfo> Cur;

	try
	{
		// Save subprofiles
		Cur=sub->GetWeightInfoCursor();
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
void GStorageMySQL::SaveProfile(GProfile* prof) throw(GException)
{
	unsigned int profid;
	unsigned int social;
	RCursor<GSubProfile> CurSub=prof->GetSubProfilesCursor();

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
		RString sSql="UPDATE profiles SET description="+RQuery::SQLValue(prof->GetName())+",social="+itou(social)+" WHERE profileid="+itou(profid);
		RQuery updateprof(Db,sSql);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadUsers(GSession* session) throw(std::bad_alloc,GException)
{
	RString sSql;
	GProfile* prof;
	R::RCursor<GFactoryLang> langs;
	GLang* lang;
	GSubProfile* sub;
	GSubject* s;
	unsigned int i,idx,subprofileid;
	RContainer<GWeightInfo,false,true> Infos(1000,500);

	// Go through the users
	try
	{
		// Load users
		RQuery Users(Db, "SELECT userid,user,fullname FROM users");
		for(Users.Start();!Users.End();Users.Next())
            session->InsertUser(new GUser(atoi(Users[0]),Users[1],Users[2],10));

		// Load profiles
		RQuery Profiles(Db,"SELECT profileid,description,social,topicid,userid FROM profiles");
		for(Profiles.Start();!Profiles.End();Profiles.Next())
		{
			session->InsertProfile(prof=new GProfile(session->GetUser(atoi(Profiles[4])),atoi(Profiles[0]),Profiles[1],(atoi(Profiles[2])==1),5));
			if(session->GetSubjects())
			{
				s=session->GetSubjects()->GetSubject(atoi(Profiles[3]));
				if(s)
					prof->SetSubject(s);
			}
		}

		// Load subprofiles
		RQuery SubProfiles(Db,"SELECT subprofileid,langid,attached,groupid, updated, calculated, profileid FROM subprofiles");
		for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
		{
			lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(SubProfiles[1]);
			if(!lang)
				throw GException(RString("Error in loading subprofiles: no language defined with code '")+SubProfiles[1]+RString("'"));
			prof=session->GetProfile(atoi(SubProfiles[6]));
			if(!prof)
				throw GException("Subprofile "+SubProfiles[0]+" has no parent profile");
			session->InsertSubProfile(sub=new GSubProfile(prof,atoi(SubProfiles[0]),lang,
			                                     session->GetGroup(atoi(SubProfiles[3])),SubProfiles[2],
			                                     GetMySQLToDate(SubProfiles[4]),GetMySQLToDate(SubProfiles[5])));
			s=session->GetSubjects()->GetSubject(atoi(SubProfiles[6]));
			if((s)&&(sub->GetLang()==s->GetLang()))
			{
				sub->SetSubject(s);
				s->InsertSubProfile(sub);
			}
		}

		// Load subprofiles description
		RQuery sel(Db,"SELECT subprofileid,kwdid,langid,weight FROM subprofilesbykwds ORDER BY subprofileid,kwdid");
		for(sel.Start(),subprofileid=cNoRef;!sel.End();sel.Next(),i++)
		{
			// Get the id
			idx=atoi(sel[0]);
			lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(sel[2]);

			// If not the same -> new subprofile
			if(idx!=subprofileid)
			{
				// If valid subprofile -> assign the information to it
				if(subprofileid!=cNoRef)
				{
					sub=session->GetSubProfile(subprofileid);
					session->AddModifiedProfile(sub);
					if(sub)
						sub->Update(&Infos,false);
				}

				// New doc
				subprofileid=idx;
				i=0;
			}
			if(lang)
				Infos.InsertPtrAt(new GWeightInfo(atoi(sel[1]),atof(sel[3]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()),i,false);
		}
		if(subprofileid!=cNoRef)
		{
			sub=session->GetSubProfile(subprofileid);
			session->AddModifiedProfile(sub);
			if(sub)
				sub->Update(&Infos,false);
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
	R::RCursor<GFactoryLang> langs;
	GLang* lang;
	RString sSql;
	GSubProfile* subp;

	try
	{
		if(!session->GetSubjects()) return;
		groups=session->GetSubjects()->GetIdealGroups();
		groups->ClearGroups();

		langs=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLangsCursor();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			sSql="SELECT DISTINCT(groupid) FROM idealgroup WHERE langid='"+RString(lang->GetCode())+"'";
			RQuery sel(Db,sSql);
			for(sel.Start();!sel.End();sel.Next())
			{
				groups->InsertGroup(group=new GGroup(atoi(sel[0]),lang,false));
				sSql="SELECT profileid FROM idealgroup where groupid="+sel[0];
				RQuery sub(Db,sSql);
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
	R::RCursor<GGroup> groups;
	RCursor<GSubProfile> sub;
	RString sSql;

	try
	{
		RQuery del(Db,"DELETE from idealgroup");

		groups=idealgroup->GetGroupsCursor();
		for(groups.Start();!groups.End();groups.Next())
		{
			sub=groups()->GetSubProfilesCursor();
			for(sub.Start();!sub.End();sub.Next())
			{
				sSql="INSERT INTO idealgroup(profileid,langid,groupid) VALUES("+itou(sub()->GetProfile()->GetId())+",'"+groups()->GetLang()->GetCode()+"',"+itou(groups()->GetId())+")";
				RQuery insert(Db,sSql);
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
	RString sSql;
	GSubject* subject;
	GSubject* subsubject;

	try
	{
		if(!session->GetSubjects()) return;
		session->GetSubjects()->Clear();
		RQuery sub(Db,"SELECT topicid,name,used,langid FROM topics WHERE parent=0");
		for(sub.Start();!sub.End();sub.Next())
		{
			session->GetSubjects()->AddNode(0,subject=new GSubject(atoi(sub[0]),sub[1],(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(sub[3]),atoi(sub[2])));
			sSql="SELECT topicid,name,used,langid FROM topics WHERE parent="+sub[0];
			RQuery subsub(*Db,sSql);
			for(subsub.Start();!subsub.End();subsub.Next())
				session->GetSubjects()->AddNode(subject,subsubject=new GSubject(atoi(subsub[0]),subsub[1],(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(subsub[3]),atoi(subsub[2])));
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
	tDocAssessment jug;

	try
	{
		// Load feedbacks
		RQuery fdbks(Db,"SELECT htmlid,judgement,profileid,when2 FROM htmlsbyprofiles");
		for(fdbks.Start();!fdbks.End();fdbks.Next())
		{
			switch(fdbks[1][0].Unicode())
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
			switch(fdbks[1][1].Unicode())
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
			session->InsertFdbk(atoi(fdbks[2]),atoi(fdbks[0]),jug,RDate(fdbks[3]));
		}

		// Update all the profiles to dispatch the feedbacks
		R::RCursor<GProfile> Profiles=session->GetProfilesCursor();
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Profiles()->Update();
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadDocs(GSession* session) throw(std::bad_alloc,GException)
{
	GDoc* doc;
	GLang* lang;
	R::RCursor<GFactoryLang> langs;
	GDoc* d;
	GSubject* s;
	unsigned int i,idx,docid;
	RContainer<GWeightInfo,false,true> Infos(1000,500);

	try
	{
		RQuery quer (Db,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed, owner FROM htmls");
		for(quer.Start();!quer.End();quer.Next())
		{
			docid=atoi(quer[0]);
			lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(quer[4]);
			session->InsertDoc(doc=new GDoc(quer[1],quer[2],docid,lang,quer[3],GetMySQLToDate(quer[5]),GetMySQLToDate(quer[6]),atoi(quer[7]),atoi(quer[8])));
		}

		// Load the links of the document loaded.
		RQuery querLinks (Db,"SELECT htmlid,linkid,occurs FROM htmlsbylinks");
		for (querLinks.Start(); !querLinks.End() ; querLinks.Next())
		{
			session->GetDoc(atoi(querLinks[0]))->InsertLink(session->GetDoc(atoi(querLinks[1])), atoi(querLinks[2]));
		}

		// Load the document description
		RQuery sel(Db,"SELECT htmlid,kwdid,langid,occurs FROM htmlsbykwds ORDER BY htmlid,kwdid");
		for(sel.Start(),docid=cNoRef;!sel.End();sel.Next(),i++)
		{
			// Get the id
			idx=atoi(sel[0]);
			if(idx==3982 || idx==4057) continue;
			lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(sel[2]);

			// If not the same -> new doc
			if(idx!=docid)
			{
				// If valid document -> assign the information to it
				if(docid!=cNoRef)
				{
					doc=session->GetDoc(docid);
					if(doc)
						doc->Update(doc->GetLang(),&Infos,false);
				}

				// New doc
				docid=idx;
				i=0;
			}
			if(lang)
			{
				if(lang->GetDict())
					Infos.InsertPtrAt(new GWeightInfo(atoi(sel[1]),atof(sel[3]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()),i,false);
				else
					Infos.InsertPtrAt(new GWeightInfo(atoi(sel[1]),atof(sel[3]),infoNothing),i,false);
			}
		}
		if(docid!=cNoRef)
		{
			doc=session->GetDoc(docid);
			if(doc)
				doc->Update(doc->GetLang(),&Infos,false);
		}

		//  Make Link between documents and topics
		if(session->GetSubjects())
		{
			RQuery subdocs(Db,"SELECT htmlid,topicid FROM topicsbyhtmls");
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
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}

}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadDocs(GSession* session,GInfoList& list,GLang* lang) throw(std::bad_alloc,GException)
{
	GDoc* doc;
//	GLang* lang;
	R::RCursor<GFactoryLang> langs;
	unsigned int i,idx,docid;
	RContainer<GWeightInfo,false,true> Infos(1000,500);
 	RString sSql;

	if((!list.GetNb())||(!lang))
		return;
	try
	{
		// Create a SQL clause with all documents id having all the information contained
		//select distinct(htmlid),count(htmlid) as t  from htmlsbykwds where langid='en' and (kwdid=37 or kwdid=2016)  group by htmlid having t=2
		sSql="SELECT DISTINCT(htmlid), count(htmlid) as t from htmlsbykwds WHERE langid='"+RString(lang->GetCode())+"' and (";
		RCursor<GInfo> List(list);
		for(List.Start();!List.End();)
		{
			sSql+="kwdid="+itou(List()->GetId());
			List.Next();
			if(!List.End())
				sSql+=" or ";
		}
		sSql+=")  group by htmlid having t="+itou(list.GetNb());
		RQuery DocsId(Db,sSql);
		sSql=" WHERE ";
		for(DocsId.Start();!DocsId.End();)
		{
			sSql+="htmlid="+DocsId[0];
			DocsId.Next();
			if(!DocsId.End())
				sSql+=" or ";
		}

		//
		RQuery quer (Db,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed, ownerid FROM htmls"+sSql);
		for(quer.Start();!quer.End();quer.Next())
		{
			docid=atoi(quer[0]);
			lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(quer[4]);
			session->InsertDoc(doc=new GDoc(quer[1],quer[2],docid,lang,quer[3],GetMySQLToDate(quer[5]),GetMySQLToDate(quer[6]),atoi(quer[7]),atoi(quer[8])));
		}

		// Load the links of the document loaded.
		RQuery querLinks (Db,"SELECT htmlid,linkid,occurs FROM htmlsbylinks"+sSql);
		for (querLinks.Start(); !querLinks.End() ; querLinks.Next())
		{
			session->GetDoc(atoi(querLinks[0]))->InsertLink(session->GetDoc(atoi(querLinks[1])), atoi(querLinks[2]));
		}

		// Load the document description
		RQuery sel(Db,"SELECT htmlid,kwdid,langid,occurs FROM htmlsbykwds"+sSql+" ORDER BY htmlid,kwdid");
		for(sel.Start(),docid=cNoRef;!sel.End();sel.Next(),i++)
		{
			// Get the id
			idx=atoi(sel[0]);

			// If not the same -> new doc
			if(idx!=docid)
			{
				// If valid document -> assign the information to it
				if(docid!=cNoRef)
				{
					doc=session->GetDoc(docid);
					if(doc)
						doc->Update(doc->GetLang(),&Infos,false);
				}

				// New doc
				docid=idx;
				i=0;
			}
			if(lang->GetDict())
				Infos.InsertPtrAt(new GWeightInfo(atoi(sel[1]),atof(sel[3]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()),i,false);
			else
				Infos.InsertPtrAt(new GWeightInfo(atoi(sel[1]),atof(sel[3]),infoNothing),i,false);
		}
		if(docid!=cNoRef)
		{
			doc=session->GetDoc(docid);
			if(doc)
				doc->Update(doc->GetLang(),&Infos,false);
		}

		//  Make Link between documents and topics
/*		if(session->GetSubjects())
		{
			RQuery subdocs(Db,"SELECT htmlid,topicid FROM topicsbyhtmls"+sSql);
			for(subdocs.Start();!subdocs.End();subdocs.Next())
			{
				d=session->GetDoc(atoi(subdocs[0]));
				if(!d) continue;
				s=session->GetSubjects()->GetSubject(atoi(subdocs[1]));
				if(!s) continue;
				s->InsertDoc(d);
				d->InsertSubject(s);
			}
		}*/
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadNewDocs(GSession* session) throw(std::bad_alloc,GException)
{
	GDoc* doc;
	GLang* lang;
	int docid;
	R::RCursor<GFactoryLang> langs;

	try
	{
		dynamic_cast<GDocs*>(session)->Clear();
		RQuery quer (Db,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed, ownerid FROM htmls WHERE calculated < updated");
		for(quer.Start();!quer.End();quer.Next())
		{
			docid=atoi(quer[0]);
			lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(quer[4]);
			session->InsertDoc(doc=new GDoc(quer[1],quer[2],docid,lang,quer[3],GetMySQLToDate(quer[5]),GetMySQLToDate(quer[6]),atoi(quer[7]),atoi(quer[8])));
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveFdbks(GSession* session) throw(GException)
{
	RString sSql;
	RString j;
	RCursor<GFdbk> Fdbks;

	try
	{
		// Clear the all feedbacks
		RQuery delete1(Db,"DELETE FROM htmlsbyprofiles");

		// Reinsert all the feedbacks
		RCursor<GProfile> Profiles=session->GetProfilesCursor();
		for(Profiles.Start();!Profiles.End();Profiles.Next())
		{
			Fdbks=Profiles()->GetFdbks();
			for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
			{
				switch(Fdbks()->GetFdbk() & djMaskJudg)
				{
					case djOK:
						j="O";
						break;
					case djOutScope:
						j="H";
						break;
					case djKO:
						j="K";
						break;
					default:
						throw GException("No Valid Assessment");
				}
				sSql="INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2) VALUES("+itou(Fdbks()->GetDoc()->GetId())+",'"+j+"',"+itou(Profiles()->GetId())+","+RQuery::SQLValue(Fdbks()->GetUpdated())+")";
				RQuery fdbks(Db,sSql);
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveLinks(GSession* session) throw(GException)
{
	RString sSql;
	RString j;
	RCursor<GFdbk> Fdbks;

	try
	{
		// Clear the all the links (where judg= Hub or Authority)
		RQuery delete1(Db,"DELETE FROM htmlsbyprofiles WHERE judgement='H' OR judgement='A'");

		// Reinsert all the feedbacks
		RCursor<GProfile> Profiles=session->GetProfilesCursor();
		for(Profiles.Start();!Profiles.End();Profiles.Next())
		{
			Fdbks=Profiles()->GetFdbks();
			for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
			{
				switch(Fdbks()->GetFdbk() & djMaskHubAuto)
				{
					case djHub:
						j="OH";
						break;
					case djAutority:
						j="OA";
						break;
					default:
						continue;
				}
				sSql="INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2) VALUES("+itou(Fdbks()->GetDoc()->GetId())+",'"+j+"',"+itou(Profiles()->GetId())+","+RQuery::SQLValue(Fdbks()->GetUpdated())+")";
				RQuery fdbks(Db,sSql);
			}
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
			Words=doc->GetWeightInfoCursor();
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
		lcur= doc->GetLinkCursor();
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
void GStorageMySQL::SaveUpDatedDoc(GDoc* doc,unsigned n) throw(GException)
{
	RString sSql;
	RString l;
	RString id;
	RString f;
	RCursor<GWeightInfo> Words;

	try
	{
		id=itou(doc->GetId());
		if(doc->GetLang())
		{
			l=doc->GetLang()->GetCode();
			Words=doc->GetWeightInfoCursor();
			for(Words.Start();!Words.End();Words.Next())
			{
				if((Words()->GetId()>=n)&&(Words()->GetWeight()>0))
				{
						sSql="INSERT INTO htmlsbykwds(htmlid,kwdid,occurs,langid) VALUES("+id+","+itou(Words()->GetId())+",'"+dtou(Words()->GetWeight())+"','"+l+"')";
						RQuery insertkwds(Db,sSql);
				}
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
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveGroups(GSession* session) throw(GException)
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
		langs=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLangsCursor();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			sSql="DELETE FROM groupsbykwds WHERE langid='"+RString(lang->GetCode())+"'";
			RQuery delete2(Db,sSql);
		}

		GroupsCursor=session->GetGroupsCursor();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			sSql="INSERT INTO groups(groupid,langid) VALUES("+itou(GroupsCursor()->GetId())+",'"+RString(GroupsCursor()->GetLang()->GetCode())+"')";
			RQuery insert1(Db,sSql);

			// Save SubProfiles infos
			Sub=GroupsCursor()->GetSubProfilesCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sSql="UPDATE subprofiles SET groupid="+itou(GroupsCursor()->GetId())+",attached="+RQuery::SQLValue(Sub()->GetAttached())+" WHERE subprofileid="+itou(Sub()->GetId());
				RQuery update(Db,sSql);
			}

			// Save the description part
			WordCur.Set(GroupsCursor());
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
void GStorageMySQL::SaveGroupsHistory(GSession* session) throw(GException)
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
		GroupsCursor=session->GetGroupsCursor();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			Sub=GroupsCursor()->GetSubProfilesCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sSql="INSERT INTO historicgroups SET date=CURDATE(), historicID="+itou(historicID)+",groupid="+itou(GroupsCursor()->GetId())+
						", subprofileid="+itou(Sub()->GetId())+",lang='"+RString(GroupsCursor()->GetLang()->GetCode())+"'";
				RQuery history(Db,sSql);
			}
		}

		//save the profiles history if needed.
		if(session->GetSessionParams()->GetBool("SaveProfilesHistory"))
			SaveHistoricProfiles(session, historicID);
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveMixedGroups(GGroups* mixedgroups,unsigned int id, bool historic) throw(GException)
{
	RString sSql;
	RString database;
	RString field;
	R::RCursor<GGroup> grp;
	RCursor<GSubProfile> Sub;

	try
	{
		// set the name of the database and the field 'id' of this database.
		if(historic)
		{
			database="historicgroups";
			field="historicID";
		}
		else
		{
			database="tempchromo";
			field="chromoid";
		}

		// Delete all the old chromo where the id is id.
		if(!mixedgroups) return;
		if(!id)
		{
			// First chromosome to store, delete all chromosomes
			RQuery delete1(Db,"DELETE FROM "+database);
		}

		grp=mixedgroups->GetGroupsCursor();
		for(grp.Start();!grp.End();grp.Next())
		{
			Sub=grp()->GetSubProfilesCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sSql="INSERT INTO "+database+"("+field+",groupid,lang,subprofileid) VALUES("+
						itou(id)+","+itou(grp()->GetId())+",'"+RString(grp()->GetLang()->GetCode())+"',"+itou(Sub()->GetId());
				RQuery InsertChromo(Db,sSql);
			}
		}
	}
	catch(RMySQLError e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveHistoricProfiles(GSession* session,unsigned int historicID) throw(GException)
{
	try
	{
		R::RCursor<GProfile> curProf=session->GetProfilesCursor() ;

		// Save the Subprofile
		for(curProf.Start();!curProf.End();curProf.Next())
		{
			RCursor<GSubProfile> curSub=curProf()->GetSubProfilesCursor();
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
void GStorageMySQL::LoadGroups(GSession* session) throw(std::bad_alloc,GException)
{
	GGroup* group;
	R::RCursor<GGroup> GroupsCursor;
	GLang* lang;
	R::RCursor<GFactoryLang> langs;
	unsigned int groupid,idx,i;
	RContainer<GWeightInfo,false,true> Infos(1000,500);

	try
	{
		RQuery group2(Db,"SELECT groupid,langid FROM groups");
		for(group2.Start();!group2.End();group2.Next())
		{
			lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(group2[1]);
			group=new GGroup(atoi(group2[0]),lang,true);
			session->InsertGroup(group);
		}

		RQuery sel(Db,"SELECT groupid,kwdid,langid,occurs FROM groupsbykwds ORDER BY groupid,kwdid");
		for(sel.Start(),groupid=cNoRef;!sel.End();sel.Next(),i++)
		{
			// Get the id
			idx=atoi(sel[0]);
			lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(sel[2]);

			// If not the same -> new group
			if(idx!=groupid)
			{
				// If valid group -> assign the information to it
				if(groupid!=cNoRef)
				{
					group=session->GetGroup(groupid);
					if(group)
						group->Update(&Infos,false);
				}

				// New group
				groupid=idx;
				i=0;
			}
			if(lang)
				Infos.InsertPtrAt(new GWeightInfo(atoi(sel[1]),atof(sel[3]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()),i,false);
		}
		if(groupid!=cNoRef)
		{
			group=session->GetGroup(groupid);
			if(group)
				group->Update(&Infos,false);
		}

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
GGroupsHistory* GStorageMySQL::LoadAnHistoricGroups(GSession* session,unsigned int historicID) throw(std::bad_alloc,GException)
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
			lang=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLang(grquery[2]);

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
			historicsubprof=new GWeightInfosHistory(session->GetSubProfile(subprofid),100);
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
void GStorageMySQL::LoadHistoricGroupsByDate(GSession* session, R::RString mindate, R::RString maxdate)
{
	RString sSql;

	sSql="SELECT DISTINCT historicID from historicgroups where date>'"+mindate+"' AND date<'"+maxdate+"' ORDER BY date,historicID";
	RQuery ids(Db,sSql);
	for (ids.Start(); !ids.End(); ids.Next())
                session->GetGroupsHistoryManager()->InsertGroupsHistory(LoadAnHistoricGroups(session, atoi(ids[0])));
}


//------------------------------------------------------------------------------
unsigned int GStorageMySQL::GetHistorySize(void) throw(GException)
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
		sSql="DELETE FROM sugsbyprofiles WHERE test="+RQuery::SQLValue(name);
		RQuery create3(Db,sSql);
		sSql="DELETE FROM sugsbygroups WHERE test="+RQuery::SQLValue(name);
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
		sSql="SELECT profileid,htmlid,rank,test FROM sugsbyprofiles ORDER BY profileid,rank WHERE test="+RQuery::SQLValue(name);
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
void GStorageMySQL::CreateDummy(RString name) throw(GException)
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
void GStorageMySQL::ClearDummy(RString name) throw(GException)
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
void GStorageMySQL::AddDummyEntry(RString name,unsigned int id,RString desc,unsigned int parentid) throw(GException)
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
RQuery* GStorageMySQL::SelectDummyEntry(RString name,unsigned int id,RString desc,unsigned int parentid,unsigned int filter) throw(GException)
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
void GStorageMySQL::ClearDummyEntry(RString name,unsigned int id,RString desc, unsigned int parentid,unsigned int filter) throw(GException)
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


//-----------------------------------------------------------------------------
void GALILEI::GStorageMySQL::ExportMatrix(GSession* sess,GSlot* rec,RString type,RString filename,GLang* lang,bool label)
{
	double* wvector;
	unsigned int* vector;
	RString sSql;
	char tmp[5];
	int maxid;

	// create the file
	RTextFile text(filename);
	text.Open(R::RIO::Create);
	text.SetSeparator("");

	//get all the words
	sSql="SELECT max(kwdid) FROM kwds WHERE langid='"+RString(lang->GetCode())+"'";
	R::RQuery maxd(Db,sSql);
	for(maxd.Begin();maxd.IsMore();maxd++)
		maxid=atoi(maxd[0]);

	//initialize the table of words;
	vector=new unsigned int[maxid+1];
	vector[0]=0;
	memset(vector,0,(maxid+1)*sizeof(unsigned int));

	// check the words used in vector description.
	if(type=="Profiles")
		sSql="SELECT DISTINCT kwdid FROM subprofilesbykwds WHERE langid='"+RString(lang->GetCode())+"'";
	else if(type=="Documents")
		sSql="SELECT DISTINCT kwdid FROM htmlsbykwds WHERE langid='"+RString(lang->GetCode())+"'";
	else if(type=="Groups")
		sSql="SELECT DISTINCT kwdid FROM groupsbykwds WHERE langid='"+RString(lang->GetCode())+"'";
	R::RQuery presentkwds(Db,sSql);
	for (presentkwds.Start(); !presentkwds.End(); presentkwds.Next())
		vector[atoi(presentkwds[0])]=1;

	//if labels are needed set word id as label
	if (label)
 		for(int i=1;i<maxid+1;i++)
 			if((vector[i]==1))
 				(text)<<" \""<<i<<"\"";

	// export the vectors;
	if(type=="Profiles")
		sSql="SELECT subprofileid FROM subprofiles WHERE langid='"+RString(lang->GetCode())+"' ORDER BY subprofileid";
	else if(type=="Documents")
		sSql="SELECT htmlid FROM htmls WHERE langid='"+RString(lang->GetCode())+"' ORDER BY htmlid";
	else if(type=="Groups")
		sSql="SELECT groupid FROM groups WHERE langid='"+RString(lang->GetCode())+"' ORDER BY groupid";
	R::RQuery entities(Db,sSql);
	for(entities.Begin();entities.IsMore();entities++)
	{
		wvector=new double[maxid+1];
		memset(wvector,0,(maxid+1)*sizeof(double));
		if(type=="Profiles")
		{
			rec->receiveNextProfileExport(sess->GetSubProfile(atoi(entities[0]))->GetProfile());
			sSql="SELECT weight,kwdid FROM subprofilesbykwds WHERE langid='"+RString(lang->GetCode())+"' AND subprofileid="+entities[0];
		}
		else if(type=="Documents")
		{
			rec->receiveNextDocumentExport(sess->GetDoc(atoi(entities[0])));
			sSql="SELECT occurs,kwdid FROM htmlsbykwds WHERE langid='"+RString(lang->GetCode())+"' AND htmlid="+entities[0];
		}
		else if(type=="Groups")
		{
			rec->receiveNextGroupExport(sess->GetGroup(atoi(entities[0])));
			sSql="SELECT occurs,kwdid FROM groupsbykwds WHERE langid='"+RString(lang->GetCode())+"' AND groupid="+entities[0];
		}
		R::RQuery query(Db,sSql);
		for(query.Begin();query.IsMore();query++)
			wvector[atoi(query[1])]=atof(query[0]);
		if (label)
			(text)<<"\""<<entities[0]<<"\"";
		for(int i=1;i<maxid+1;i++)
		{
			if(vector[i]==1)
			{
				sprintf(tmp,"%.0f", wvector[i]);
				(text)<<" "<<tmp;
			}
		}
		(text)<<" \""<<entities[0]<<"\""<<endl;
	}

	// detele used tables
	delete[] vector;
	delete[] wvector;
}


//------------------------------------------------------------------------------
GStorageMySQL::~GStorageMySQL(void)
{
}
