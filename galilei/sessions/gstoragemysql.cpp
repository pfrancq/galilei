/*

	GALILEI Research Project

	GStorageMySQL.cpp

	Storage Manager using a MySQL Database - Implementation.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
#include <infos/gword.h>
#include <infos/gwordlist.h>
#include <infos/gweightinfo.h>
#include <infos/gweightinfos.h>
#include <docs/gdocproxy.h>
#include <docs/glink.h>
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
		sSql=RString("INSERT INTO kwds(kwd,kwdid,type,langid) SELECT ")+RQuery::SQLValue(data->GetName())+",IFNULL(MAX(kwdid)+1,1),"+RQuery::SQLValue(itou(data->GetType()))+",'"+dict->GetLang()->GetCode()+"' FROM kwds WHERE langid='"+dict->GetLang()->GetCode()+"'";
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
	GWordCursor Cur;

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
	GWeightInfoCursor Cur;
	RString l;

	try
	{
		//test if the subprofile already exists.
		sSql="SELECT COUNT(*) FROM subprofiles WHERE subprofileid="+itou(sub->GetId());
		RQuery existsubprof(Db,sSql);
		existsubprof.Start();
		if(!atoi(existsubprof[0]))
		{
			//insert the subprofile;
			sSql="INSERT INTO subprofiles SET profileid="+itou(sub->GetProfile()->GetId());
			sSql+=",langid='"+RString(sub->GetLang()->GetCode())+"',attached=CURDATE(),state="+itou(sub->GetState());
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
	GWeightInfoCursor Cur;

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
	GUser* usr;
	GProfile* prof;
	GFactoryLangCursor langs;
	GLang* lang;
	GSubProfile* sub;
	unsigned int userid,profileid,subid;
	GGroup* grp;
	bool Social;
	GSubject* s;

	// Go through the users
	try
	{
		RQuery select(Db, "SELECT userid,user,fullname FROM users");
		for(select.Start();!select.End();select.Next())
		{
			userid=atoi(select[0]);
			sSql="SELECT profileid,description,social,topicid FROM profiles WHERE userid="+itou(userid);
			RQuery profiles(Db,sSql);
			session->InsertUser(usr=new GUser(userid,select[1],select[2],profiles.GetNbRows()));
			for(profiles.Start();!profiles.End();profiles.Next())
			{
				profileid=atoi(profiles[0]);
				if(session->GetSubjects())
					s=session->GetSubjects()->GetSubject(atoi(profiles[3]));
				else
					s=0;
				Social=false;
				if(atoi(profiles[2])==1) Social=true;
				session->InsertProfile(prof=new GProfile(usr,profileid,profiles[1],Social,session->GetLangs()->NbPtr));
				if(s)
					prof->SetSubject(s);
				sSql="SELECT subprofileid,langid,attached,groupid, updated, calculated FROM subprofiles WHERE profileid="+itou(profileid);
				RQuery subprofil (Db,sSql);
				for(subprofil.Start();!subprofil.End();subprofil.Next())
				{
					lang=session->GetLangs()->GetLang(subprofil[1]);
					if (!lang)
						throw GException(RString("Error in loading subprofiles: no language defined with code '")+subprofil[1]+RString("'"));
					subid=atoi(subprofil[0]);
					grp=session->GetGroup(atoi(subprofil[3]));
					session->InsertSubProfile(sub=new GSubProfile(prof,subid,lang,grp,subprofil[2], GetMySQLToDate(subprofil[4]), GetMySQLToDate(subprofil[5])));
					if((s)&&(sub->GetLang()==s->GetLang()))
					{
						sub->SetSubject(s);
						s->InsertSubProfile(sub);
					}
				}
			}
		}

		// Load the subprofile's description
		langs=session->GetLangs()->GetLangsCursor();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			sSql="SELECT subprofileid,kwdid,Weight FROM subprofilesbykwds WHERE langid='"+RString(lang->GetCode())+"'";
			RQuery sel(Db,sSql);
			for(sel.Start();!sel.End();sel.Next())
			{
				sub=session->GetSubProfile(atoi(sel[0]),lang);
				if(sub)
					sub->InsertInfo(new GWeightInfo(atoi(sel[1]),atof(sel[2]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()));
			}
		}

		// Update References of the loaded subprofiles.
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			GSubProfileCursor SubProfiles=session->GetSubProfilesCursor(lang);
			for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
				SubProfiles()->UpdateRefs();
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
	RString sSql;
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
	GGroupCursor groups;
	GSubProfileCursor sub;
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
			session->GetSubjects()->AddNode(0,subject=new GSubject(atoi(sub[0]),sub[1],session->GetLangs()->GetLang(sub[3]),atoi(sub[2])));
			sSql="SELECT topicid,name,used,langid FROM topics WHERE parent="+sub[0];
			RQuery subsub(*Db,sSql);
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
	tDocAssessment jug;

	try
	{
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
	int docid;
	GFactoryLangCursor langs;
	GDoc* d;
	GSubject* s;
	unsigned int i,idx;
	RContainer<GWeightInfo,false,true> Infos(1000,500);

	try
	{
		RQuery quer (Db,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed FROM htmls");
		for(quer.Start();!quer.End();quer.Next())
		{
			docid=atoi(quer[0]);
			lang=session->GetLangs()->GetLang(quer[4]);
			session->InsertDoc(doc=new GDoc(quer[1],quer[2],docid,lang,quer[3],GetMySQLToDate(quer[5]),GetMySQLToDate(quer[6]),atoi(quer[7])));
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
			lang=session->GetLangs()->GetLang(sel[2]);

			// If not the same -> new doc
			if(idx!=docid)
			{
				// If valid document -> assign the information to it
				if(docid!=cNoRef)
				{
					doc=session->GetDoc(docid);
					if(doc)
						doc->Update(doc->GetLang(),&Infos);
				}

				// New doc
				docid=idx;
				i=0;
			}

			Infos.InsertPtrAt(new GWeightInfo(atoi(sel[1]),atof(sel[3]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()),i,false);
		}
		if(docid!=cNoRef)
		{
			doc=session->GetDoc(docid);
			if(doc)
				doc->Update(doc->GetLang(),&Infos);
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
void GStorageMySQL::LoadNewDocs(GSession* session) throw(std::bad_alloc,GException)
{
	GDoc* doc;
	GLang* lang;
	int docid;
	GFactoryLangCursor langs;

	try
	{
		dynamic_cast<GDocs*>(session)->Clear();
		RQuery quer (Db,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed FROM htmls WHERE calculated < updated");
		for(quer.Start();!quer.End();quer.Next())
		{
			docid=atoi(quer[0]);
			lang=session->GetLangs()->GetLang(quer[4]);
			session->InsertDoc(doc=new GDoc(quer[1],quer[2],docid,lang,quer[3],GetMySQLToDate(quer[5]),GetMySQLToDate(quer[6]),atoi(quer[7])));
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
		for(Profiles.Start();!Profiles.End();Profiles.End())
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
				sSql="INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2) VALUES("+itou(Fdbks()->GetDoc()->GetId())+"'"+j+"',"+itou(Profiles()->GetId())+","+RQuery::SQLValue(Fdbks()->GetUpdated());
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
		for(Profiles.Start();!Profiles.End();Profiles.End())
		{
			Fdbks=Profiles()->GetFdbks();
			for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
			{
				switch(Fdbks()->GetFdbk() & djMaskHubAuto)
				{
					case djHub:
						j="H";
						break;
					case djAutority:
						j="A";
						break;
					default:
						continue;
				}
				sSql="INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2) VALUES("+itou(Fdbks()->GetDoc()->GetId())+"'"+j+"',"+itou(Profiles()->GetId())+RQuery::SQLValue(Fdbks()->GetUpdated());
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
	GWeightInfoCursor Words;
	GLinkCursor lcur;

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
	GWeightInfoCursor Words;

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
	GWeightInfoCursor WordCur;
	GGroupCursor GroupsCursor;
	RString sSql;
	GFactoryLangCursor langs;
	GLang* lang;
	GSubProfileCursor Sub;

	try
	{
		RQuery delete1(Db,"DELETE FROM groups");
		langs=session->GetLangs()->GetLangsCursor();
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
	GGroupCursor GroupsCursor;
	GSubProfileCursor Sub;
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
	GGroupCursor grp;
	GSubProfileCursor Sub;

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
		GProfileCursor curProf=session->GetProfilesCursor() ;

		// Save the Subprofile
		for(curProf.Start();!curProf.End();curProf.Next())
		{
			GSubProfileCursor curSub=curProf()->GetSubProfilesCursor();
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
	GGroupCursor GroupsCursor;
	GLang* lang;
	GFactoryLangCursor langs;

	try
	{
		RQuery group2(Db,"SELECT groupid,langid FROM groups");
		for(group2.Start();!group2.End();group2.Next())
		{
			lang=session->GetLangs()->GetLang(group2[1]);
			group=new GGroup(atoi(group2[0]),lang,true);
			session->InsertGroup(group);
		}

		// Load the groups description
		langs=session->GetLangs()->GetLangsCursor();
		for(langs.Start();!langs.End();langs.Next())
		{
			lang=langs()->GetPlugin();
			if(!lang) continue;
			RQuery sel(Db,"SELECT groupid,kwdid,occurs FROM groupsbykwds WHERE langid='"+RString(lang->GetCode())+"'");
			for(sel.Start();!sel.End();sel.Next())
			{
				group=session->GetGroup(atoi(sel[0]));
				if(!group) continue;
				group->InsertInfo(new GWeightInfo(atoi(sel[1]),atof(sel[2]),lang->GetDict()->GetData(atoi(sel[1]))->GetType()));
			}
		}

		// Update References of the loaded groups.
		GroupsCursor=session->GetGroupsCursor();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
			GroupsCursor()->UpdateRefs();
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
			lang=session->GetLangs()->GetLang(grquery[2]);

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
	RTextFile text(filename,R::Create);
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
