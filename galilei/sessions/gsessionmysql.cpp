/*

	GALILEI Research Project

	GSessionMySQL.cpp

	GALILEI Session using a MySQL Database - Implementation.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>
#include <ctype.h>



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <sessions/gsessionmysql.h>
#include <langs/glang.h>
#include <langs/gdict.h>
#include <langs/gdicts.h>
#include <infos/giwordlist.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofdoc.h>
#include <docs/gdoc.h>
#include <docs/gdocs.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
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
	char sSql[600];

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
		Stops->InsertPtr(dict=new GDict(this,tbl,"Stop List",GetLang(code),MaxId,MaxCount,true));
	else
	{
		Dics->InsertPtr(dict=new GDict(this,tbl,"Dictionnary",GetLang(code),MaxId,MaxCount,false));
		GetLang(code)->AssignDict(dict);
	}

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
	GLangCursor Langs;

	sprintf(sSql,"INSERT INTO profiles(userid,description,updated) VALUES(%u,%s,CURDATE())",
		usr->GetId(),ValidSQLValue(desc,sDes));
	RQuery insertprof(this,sSql);

	// Get Id and updated
	sprintf(sSql,"SELECT profileid,updated FROM profiles WHERE profileid=LAST_INSERT_ID()");
	RQuery selectprofile(this,sSql);
	selectprofile.Begin();
	id=strtoul(selectprofile[0],0,10);
	InsertProfile(prof=new GProfile(usr,id,desc,selectprofile[1],0,GetNbLangs()));

	// Construct SubProfiles
	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		sprintf(sSql,"INSERT INTO subprofiles(profileid,langid) VALUES(%u,'%s')",id,Langs()->GetCode());
		RQuery insertsub(this,sSql);
		sprintf(sSql,"SELECT subprofileid from subprofiles WHERE subprofileid=LAST_INSERT_ID())");
		RQuery selectsub(this,sSql);
		selectsub.Begin();
		InsertSubProfile(new GSubProfileVector(prof,strtoul(selectsub[0],0,10),Langs(),0,0));
	}

	// Return new created profile
	return(prof);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveSubProfile(GSubProfile* sub) throw(GException)
{
	char sSql[200];
	char sattached[15];
	unsigned int grpid;
	GIWordWeightCursor Cur;
	const char* l;

	// Delete keywords
	l=sub->GetLang()->GetCode();
	sprintf(sSql,"DELETE FROM %ssubprofilesbykwds WHERE subprofileid=%u",l,sub->GetId());
	RQuery deletekwds(this,sSql);
	Cur=((GSubProfileVector*)sub)->GetVectorCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sprintf(sSql,"INSERT INTO  %ssubprofilesbykwds(subprofileid,kwdid,weight) VALUES (%u,%u,'%e')",
		              l,sub->GetId(),Cur()->GetId(),Cur()->GetWeight());
		RQuery insertkwds(this,sSql);
	}

	if(sub->GetGroup())
		grpid=sub->GetGroup()->GetId();
	else
		grpid=0;
	sprintf(sSql,"UPDATE subprofiles SET groupid=%u,attached=%s WHERE subprofileid=%u",grpid,GetDateToMySQL(sub->GetAttached(),sattached),sub->GetId());
	RQuery update(this,sSql);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveProfile(GProfile* prof) throw(GException)
{
	char sSql[500];
	char sname[200];
	char supdated[15];
	char scomputed[15];
	unsigned int profid;
	GSubProfileCursor CurSub=prof->GetSubProfilesCursor();

	// Init
	profid=prof->GetId();

	// Save the Subprofile
	for(CurSub.Start();!CurSub.End();CurSub.Next())
		SaveSubProfile(CurSub());

	// Update profiles
	sprintf(sSql,"UPDATE profiles SET description=%s,updated=%s,calculated=%s WHERE profileid=%u",
	        ValidSQLValue(prof->GetName(),sname),GetDateToMySQL(prof->GetUpdated(),supdated),
	        GetDateToMySQL(prof->GetComputed(),scomputed),profid);
	RQuery updateprof(this,sSql);
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
	GLangCursor Langs;

	// Go through the users
	try
	{
		RQuery users(this, "SELECT userid,user,fullname FROM users");
		for(users.Begin();users.IsMore();users++)
		{
			userid=atoi(users[0]);
			sprintf(sSql,"SELECT profileid,description,updated,calculated FROM profiles WHERE userid=%u",userid);
			RQuery profiles(this,sSql);
			InsertUser(usr=new GUser(userid,users[1],users[2],profiles.GetNbRows()));
			for(profiles.Begin();profiles.IsMore();profiles++)
			{
				profileid=atoi(profiles[0]);
				InsertProfile(prof=new GProfile(usr,profileid,profiles[1],profiles[2],profiles[3],GetNbLangs()));
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
					InsertSubProfile(sub=new GSubProfileVector(prof,subid,lang,grp,subprofil[2]));
				}
			}
		}

		// Load the subprofile's description
		Langs=GetLangsCursor();
		for(Langs.Start();!Langs.End();Langs.Next())
		{
			sprintf(sSql,"SELECT subprofileid,kwdid,Weight FROM %ssubprofilesbykwds",Langs()->GetCode());
			RQuery sel(this,sSql);
			for(sel.Begin();sel.IsMore();sel++)
			{
				sub=GetSubProfile(atoi(sel[0]),Langs());
				if(sub)
				{
					((GSubProfileVector*)sub)->AddWord(atoi(sel[1]),atof(sel[2]));
				}
			}
		}

		// Update References of the loaded subprofiles.
		for(Langs.Start();!Langs.End();Langs.Next())
		{
			GSubProfileCursor SubProfiles=GetSubProfilesCursor(Langs());
			for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
				((GSubProfileVector*)SubProfiles())->UpdateRefs();
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
		usr=GetUser(atoi(fdbks[3]));
		if(!usr) continue;
		prof=usr->GetPtr<unsigned int>(atoi(fdbks[2]));
		if(!prof) continue;
		doc=GetDoc(atoi(fdbks[0]));
		if(!doc) continue;
		switch(fdbks[1][0])
		{
			case 'O':
				InsertFdbk(prof,doc,djOK,fdbks[4]);
				break;
			case 'N':
				InsertFdbk(prof,doc,djNav,fdbks[4]);
				break;
			case 'K':
				InsertFdbk(prof,doc,djKO,fdbks[4]);
				break;
			case 'H':
				InsertFdbk(prof,doc,djOutScope,fdbks[4]);
				break;
			default:
				InsertFdbk(prof,doc,djUnknow,fdbks[4]);
				break;
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::LoadDocs(void) throw(bad_alloc,GException)
{
	GDoc* doc;
	GLang* lang;
	int docid;
	char sSql[100];
	GLangCursor Langs;

	sprintf(sSql,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,failed,n,ndiff,v,vdiff FROM htmls");
	RQuery quer (this,sSql);
	for(quer.Begin();quer.IsMore();quer++)
	{
		docid=atoi(quer[0]);
		lang=GetLang(quer[4]);
		InsertDoc(doc=new GDoc(quer[1],quer[2],docid,lang,Mng->GetMIMEType(quer[3]),quer[5],quer[6],atoi(quer[7]),atoi(quer[8]),atoi(quer[9]),atoi(quer[10]),atoi(quer[11])));
	}

	// Load the document's description
	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		sprintf(sSql,"SELECT htmlid,kwdid,occurs FROM %shtmlsbykwds",Langs()->GetCode());
		RQuery sel(this,sSql);
		for(sel.Begin();sel.IsMore();sel++)
		{
			doc=GetDoc(atoi(sel[0]));
			if(doc)
				doc->AddWord(atoi(sel[1]),atof(sel[2]));
		}
	}

	// Update References of the loaded documents.
	GDocCursor Docs=GetDocsCursor();
	for(Docs.Start();!Docs.End();Docs.Next())
		Docs()->UpdateRefs();
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
	InsertDoc(doc);
	return(doc);
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveFdbks(void) throw(GException)
{
	char sSql[500];
	char j;

	// Clear the all feedbacks
	sprintf(sSql,"DELETE FROM htmlsbyprofiles");
	RQuery delete1(this,sSql);

	// Reinsert all the feedbacks
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		switch(Fdbks()->GetFdbk())
		{
			case djOK:
				j='O';
				break;
			case djNav:
				j='N';
				break;
			case djOutScope:
				j='H';
				break;
			case djKO:
				j='K';
				break;
			default:
				throw GException("No Valid Judgement");
		}
		sprintf(sSql,"INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2) VALUES(%u,%c,%u,CURDATE())",
		        Fdbks()->GetDoc()->GetId(),j,Fdbks()->GetProfile()->GetId());
		RQuery fdbks(this,sSql);

	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionMySQL::SaveDoc(GDoc* doc) throw(GException)
{
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
	GIWordWeightCursor Words;

	// Delete keywords
	if(doc->GetLang())
	{
		l=doc->GetLang()->GetCode();
		sprintf(sSql,"DELETE FROM %shtmlsbykwds WHERE htmlid=%u",l,id);
		RQuery deletekwds(this,sSql);
		Words=doc->GetWordWeightCursor();
		for(Words.Start();!Words.End();Words.Next())
		{
			sprintf(sSql,"INSERT INTO %shtmlsbykwds(htmlid,kwdid,occurs) VALUES (%u,%u,%lf)",l,id,Words()->GetId(),Words()->GetWeight());
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
void GALILEI::GSessionMySQL::NewGroup(GLang* lang,GGroup* grp)
{
	char sSql[100];

	// Insert it
	if(!lang) return;
	sprintf(sSql,"INSERT INTO groups(langid) VALUES('%s')",lang->GetCode());
	RQuery insert(this,sSql);

	// Get Id and updated
	sprintf(sSql,"SELECT groupid FROM groups WHERE groupid=LAST_INSERT_ID()");
	RQuery select(this,sSql);
	select.Begin();
	grp->SetId(strtoul(select[0],0,10));
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
	GLangCursor Langs;

	Langs=GetLangsCursor();
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
void GALILEI::GSessionMySQL::ExecuteData(const char* filename) throw(GException)
{
	RTextFile Sql(filename);
	RString l;

	while(!Sql.Eof())
	{
		l=Sql.GetLine();
		RQuery exec(this,l);
	}
}


//-----------------------------------------------------------------------------
GALILEI::GSessionMySQL::~GSessionMySQL()
{
}
