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
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gsessionsmysql/gsessionmysql.h>
#include <glangs/glang.h>
#include <ginfos/giwordlist.h>
#include <ginfos/giwordoccur.h>
#include <ginfos/giwordoccurs.h>
#include <gprofiles/guser.h>
#include <gprofiles/gprofile.h>
#include <gprofiles/gsubprofile.h>
#include <gprofiles/gprofdoc.h>
#include <gdocs/gdoc.h>
#include <gdocs/gdocs.h>
#include <ggroups/ggroup.h>
#include <ggroups/ggroups.h>
#include <filters/gmimefilter.h>
#include <urlmanagers/gurlmanager.h>
using namespace GALILEI;
using namespace RMySQL;
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
//  GSessionMySQL
//
//-----------------------------------------------------------------------------

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
	count.Begin();
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
	count.Begin();
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
		(*(ptr2++))=(*(ptr1++));
	}
	(*(ptr2++))='\'';
	(*ptr2)=0;
	return(tmp);
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
	sprintf(sSql,"SELECT kwdid FROM %skwds WHERE kwdid=LAST_INSERT_ID()",dict->GetLang()->GetCode());
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
	if(MaxCount==0) MaxCount=2000;
	MaxId=GetMax(tbl,"kwdid");
	if(!MaxId)
		MaxId=2000;

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
GProfile* GALILEI::GSessionMySQL::NewProfile(GUser* usr,const char* desc) throw(bad_alloc,GException)
{
	char sSql[500];
	char sDes[100];
	GProfile* prof;
	unsigned int id;

	sprintf(sSql,"INSERT INTO profiles(userid,description,updated) VALUES(%u,%s,CURDATE())",
		usr->GetId(),ValidSQLValue(desc,sDes));
	RQuery insertprof(this,sSql);

	// Get Id and updated
	sprintf(sSql,"SELECT profileid,updated FROM profiles WHERE profileid=LAST_INSERT_ID()");
	RQuery selectprofile(this,sSql);
	selectprofile.Begin();
	id=strtoul(selectprofile[0],0,10);
	Profiles->InsertPtr(prof=new GProfile(usr,id,desc,selectprofile[1],0,GetNbLangs()));

	// Construct SubProfiles
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		sprintf(sSql,"INSERT INTO subprofiles(profileid,langid) VALUES(%u,'%s')",id,Langs()->GetCode());
		RQuery insertsub(this,sSql);
		sprintf(sSql,"SELECT subprofileid from subprofiles WHERE subprofileid=LAST_INSERT_ID())");
		RQuery selectsub(this,sSql);
		selectsub.Begin();
		SubProfiles->InsertPtr(new GSubProfile(prof,strtoul(selectsub[0],0,10),Langs(),0,0));
	}

	// Return new created profile
	return(prof);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::Save(GIWordList* list,unsigned int id,const char* name,const char* lang) throw(GException)
{
	char sSql[100];

	// Delete list
	sprintf(sSql,"delete FROM %s%skwds WHERE subprofileid=%u",lang,name,id);
	RQuery del(this,sSql);

	// Insert It
	for(list->Start();!list->End();list->Next())
	{
		sprintf(sSql,"INSERT INTO %s%skwds(subprofileid,kwdid) VALUES(%u,%u)",lang,name,id,(*list)()->GetId());
		RQuery ins(this,sSql);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::Save(const GSubProfile* sub) throw(GException)
{
	char sSql[200];
	char sattached[15];
	unsigned int grpid;

	if(sub->GetGroup())
		grpid=sub->GetGroup()->GetId();
	else
		grpid=0;
	sprintf(sSql,"UPDATE subprofiles SET groupid=%u,attached=%s WHERE subprofileid=%u",grpid,GetDateToMySQL(sub->GetAttached(),sattached),sub->GetId());
	RQuery update(this,sSql);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::Save(const GProfile* prof) throw(GException)
{
	char sSql[500];
	char sname[200];
	char supdated[15];
	char scomputed[15];
	const char* lang;
	unsigned int profid,subid;
	RContainerCursor<GSubProfile,unsigned,false,true> CurSub(prof);

	// Init
	profid=prof->GetId();

	// Save the Subprofile
	for(CurSub.Start();!CurSub.End();CurSub.Next())
	{
		Save(CurSub());
		lang=CurSub()->GetLang()->GetCode();
		subid=CurSub()->GetId();
		Save(CurSub()->GetOK(),subid,"ok",lang);
		Save(CurSub()->GetKO(),subid,"ko",lang);
		Save(CurSub()->GetCommon(),subid,"com",lang);
	}

	// Update profiles
	sprintf(sSql,"UPDATE profiles SET description=%s,updated=%s,calculated=%s WHERE profileid=%u",
	        ValidSQLValue(prof->GetName(),sname),GetDateToMySQL(prof->GetUpdated(),supdated),
	        GetDateToMySQL(prof->GetComputed(),scomputed),profid);
	RQuery updateprof(this,sSql);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::Load(GIWordList* list,unsigned int id,const char* name,const char* lang) throw(GException)
{
	char sSql[200];

	sprintf(sSql,"SELECT kwdid FROM %s%skwds WHERE subprofileid=%u",lang,name,id);
	RQuery select(this,sSql);
	for(select.Begin();select.IsMore();select++)
		list->InsertPtr(new GIWord(atoi(select[0])));
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

	// Go through the users
	try
	{
		RQuery users(this, "SELECT userid,user,fullname FROM users");
		for(users.Begin();users.IsMore();users++)
		{
			userid=atoi(users[0]);
			sprintf(sSql,"SELECT profileid,description,updated,calculated FROM profiles WHERE userid=%u",userid);
			RQuery profiles(this,sSql);
			Users.InsertPtr(usr=new GUser(userid,users[1],users[2],profiles.GetNbRows()));
			for(profiles.Begin();profiles.IsMore();profiles++)
			{
				profileid=atoi(profiles[0]);
				Profiles->InsertPtr(prof=new GProfile(usr,profileid,profiles[1],profiles[2],profiles[3],GetNbLangs()));
				sprintf(sSql,"SELECT subprofileid,langid,attached,groupid FROM subprofiles WHERE profileid=%u",profileid);
				RQuery subprofil (this,sSql);
				for(subprofil.Begin();subprofil.IsMore();subprofil++)
				{
					lang=GetLang(subprofil[1]);
					subid=atoi(subprofil[0]);
					if(lang)
						grp=Groups.GetPtr<const GLang*>(lang)->GetPtr<const unsigned int>(atoi(subprofil[3]));
					else
						grp=0;
					SubProfiles->InsertPtr(sub=new GSubProfile(prof,subid,lang,grp,subprofil[2]));
					Load(sub->GetOK(),subid,"ok",lang->GetCode());
					Load(sub->GetKO(),subid,"ko",lang->GetCode());
					Load(sub->GetCommon(),subid,"com",lang->GetCode());
				}
			}
		}
	}
	catch(RMySQLError& e)
	{
		throw GException(e.GetError());
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

	sprintf(sSql,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed,n,ndiff,v,vdiff FROM htmls");
	RQuery quer (this,sSql);
	for(quer.Begin();quer.IsMore();quer++)
	{
		docid=atoi(quer[0]);
		lang=GetLang(quer[4]);
		Docs.InsertPtr(doc=new GDoc(quer[1],quer[2],docid,lang,Mng->GetMIMEType(quer[3]),quer[5],quer[6],atoi(quer[7]),atoi(quer[8]),atoi(quer[9]),atoi(quer[10]),atoi(quer[11])));
	}
	
	// Load the document's description
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		sprintf(sSql,"SELECT htmlid,kwdid,occurs FROM %shtmlsbykwds",Langs()->GetCode());
		RQuery sel(this,sSql);
		for(sel.Begin();sel.IsMore();sel++)
		{
			doc=Docs.GetPtr<const unsigned int>(atoi(sel[0]));
			if(doc)
				doc->AddWord(atoi(sel[1]),atoi(sel[2]));
		}
	}
}


//-----------------------------------------------------------------------------
GDoc* GALILEI::GSessionMySQL::NewDoc(const char* url,const char* name,const char* mime) throw(GException)
{
	char sSql[1000];
	char surl[200];
	char sname[200];
	GDoc *doc;

	// Insert it
	sprintf(sSql,"INSERT INTO htmls(html,title,mimetype,updated) VALUES(%s,%s,'%s',CURDATE())",
	        ValidSQLValue(url,surl),ValidSQLValue(name,sname),mime);
	RQuery insertdoc(this,sSql);

	// Get Id and updated
	sprintf(sSql,"SELECT htmlid,updated FROM htmls WHERE htmlid=LAST_INSERT_ID()");
	RQuery selectdoc(this,sSql);
	selectdoc.Begin();
	doc=new GDoc(url,name,strtoul(selectdoc[0],0,10),0,Mng->GetMIMEType(mime),selectdoc[1],0,0,0,0,0,0);
	Docs.InsertPtr(doc);
	return(doc);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::Save(GDoc* doc) throw(GException)
{
	GIWordOccur* w;
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

	// Delete keywords
	if(doc->GetLang())
	{
		l=doc->GetLang()->GetCode();
		sprintf(sSql,"DELETE FROM %shtmlsbykwds WHERE htmlid=%u",l,id);
		RQuery deletekwds(this,sSql);
		for(doc->WordsStart();!doc->WordsEnd();doc->WordsNext())
		{
			w=doc->GetCurWords();
			sprintf(sSql,"INSERT INTO %shtmlsbykwds(htmlid,kwdid,occurs) VALUES (%u,%u,%u)",l,id,w->GetId(),w->GetNbOccurs());
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
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GSessionMySQL::NewGroup(GLang* lang)
{
	char sSql[100];

	// Insert it
	if(!lang) return(0);
	sprintf(sSql,"INSERT INTO groups(langid) VALUES('%s')",lang->GetCode());
	RQuery insert(this,sSql);

	// Get Id and updated
	sprintf(sSql,"SELECT groupid FROM groups WHERE groupid=LAST_INSERT_ID()");
	RQuery select(this,sSql);
	select.Begin();
	return(new GGroup(strtoul(select[0],0,10),lang));
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::DeleteGroup(GGroup* grp)
{
	char sSql[100];

	if(!grp) return;
	sprintf(sSql,"DELETE FROM groups WHERE groupid=%u",grp->GetId());
	RQuery del(this,sSql);
	grp->SetId(cNoRef);
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
			groups->InsertPtr(group=new GGroup(atoi(group2[0]),Langs()));
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GSessionMySQL::~GSessionMySQL()
{
}
