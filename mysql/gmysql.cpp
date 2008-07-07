/*

	GALILEI Research Project

	GStorageMySQL.cpp

	Storage Manager using a MySQL Database - Implementation.

	Copyright 2001-2008 by the Université libre de Bruxelles.

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
#include <glang.h>
#include <gweightinfo.h>
#include <gweightinfos.h>
#include <glink.h>
#include <gsugs.h>
#include <guser.h>
#include <gprofile.h>
#include <gmysql.h>
#include <gslot.h>
#include <gcommunity.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <gsession.h>
#include <gcommunitieshistory.h>
#include <gweightinfoshistory.h>
#include <gindexer.h>
#include <gwordoccurs.h>
#include <ggalileiapp.h>
#include <gconcept.h>
#include <gxmlindex.h>
#include <gconcepttype.h>
#include <grelation.h>
#include <grelationtype.h>
#include <gdocstruct.h>
using namespace GALILEI;
using namespace R;
using namespace std;



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
RString GStorageMySQL::Num(double d)
{
	RString res("'"+RString::Number(d)+"'");
	int i=res.Find(',');
	if(i!=-1)
		res=res.Mid(0,i)+"."+res.Mid(i+1);
	return(res);
}


//------------------------------------------------------------------------------
double GStorageMySQL::atof(const R::RString& str)
{
	double d2;
	RString res(str);
	double d1=std::atof(str);
	int i=res.Find(',');
	if(i!=-1)
	{
		res=res.Mid(0,i)+"."+res.Mid(i+1);
		d2=std::atof(res);
	}
	else
	{
		int i=res.Find('.');
		if(i!=-1)
		{
			res=res.Mid(0,i)+","+res.Mid(i+1);
			d2=std::atof(res);
		}
	}
	if(d1==0.0)
		return(d2);
	return(d1);
}



//------------------------------------------------------------------------------
//
// General Methods
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RString GStorageMySQL::GetWorld(void) const
{
	return(Database);
}


//-----------------------------------------------------------------------------
void GStorageMySQL::InitAccess(void)
{
	try
	{
		Db.reset(new RDb(Host,User,Password,Database,Encoding));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//-----------------------------------------------------------------------------
void GStorageMySQL::ApplyConfig(void)
{
	Host=Factory->Get("Host");
	User=Factory->Get("User");
	Password=Factory->Get("Password");
	Database=Factory->Get("Database");
	Filter.SetDate(Factory->Get("Filter"));
	LoadAll=Factory->GetBool("All");
	Encoding=Factory->Get("Encoding");
	Filtering=Factory->GetBool("Filtering");
	GStorage::ApplyConfig();
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

			case otCommunity:
				return(GetCount("groups"));

			case otFdbk:
				return(GetCount("htmlsbyprofiles"));

			default:
				return(0);
		}
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//-----------------------------------------------------------------------------
void* GStorageMySQL::GetInfos(void)
{
	return(Db.get());
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
		cerr<<e.GetMsg()<<endl;
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
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddDummyEntry(RString name,unsigned int id,RString desc,unsigned int parentid)
{
	RString sSql;

	try
	{
		sSql="INSERT INTO "+name+" (id, parentid, description) VALUES("+RString::Number(id)+","+RString::Number(parentid)+","+RQuery::SQLValue(desc)+")";
		RQuery add(Db,sSql);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
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
			sSql="SELECT * FROM "+name+" WHERE id="+RString::Number(id);
			break;
		case 2: //filter on description
			sSql= "SELECT * FROM "+name+" WHERE description="+RQuery::SQLValue(desc);
			break;
		case 3: //filter on id & desc
			sSql="SELECT * FROM "+name+" WHERE id="+RString::Number(id)+" AND description="+RQuery::SQLValue(desc);
			break;
		case 4: //filter on parentid
			sSql="SELECT * FROM "+name+" WHERE parentid="+RString::Number(parentid);
			break;
		case 5: //filter on id & parentid
			sSql="SELECT * FROM "+name+" WHERE id="+RString::Number(id)+" AND parentid="+RString::Number(parentid);
			break;
		case 6: //filter on desc & parentid
			sSql="SELECT * FROM "+name+" WHERE description="+RQuery::SQLValue(desc)+" AND parentid="+RString::Number(parentid);
			break;
		case 7: //filter on id &desc & parentid
			sSql="SELECT * FROM "+name+" WHERE id="+RString::Number(id)+" AND description="+RQuery::SQLValue(desc)+" AND parentid="+RString::Number(parentid);
			break;
	}
	try
	{
		select=new RQuery(Db,sSql);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
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
			sSql="DELETE FROM "+name+" WHERE id="+RString::Number(id);
			break;
		case 2: //filter on description
			sSql= "DELETE FROM "+name+" WHERE description="+RQuery::SQLValue(desc);
			break;
		case 3: //filter on id & desc
			sSql="DELETE FROM "+name+" WHERE id="+RString::Number(id)+" AND description="+RQuery::SQLValue(desc);
			break;
		case 4: //filter on parentid
			sSql="DELETE FROM "+name+" WHERE parentid="+RString::Number(parentid);
			break;
		case 5: //filter on id & parentid
			sSql="DELETE FROM "+name+" WHERE id="+RString::Number(id)+" AND parentid="+RString::Number(parentid);
			break;
		case 6: //filter on desc & parentid
			sSql="DELETE FROM "+name+" WHERE description="+RQuery::SQLValue(desc)+" AND parentid="+RString::Number(parentid);
			break;
		case 7: //filter on id &desc & parentid
			sSql="DELETE FROM "+name+" WHERE id="+RString::Number(id)+" AND description="+RQuery::SQLValue(desc)+" AND parentid="+RString::Number(parentid);
			break;
	}

	try
	{
		RQuery select(Db,sSql);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
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
		GSubject* Top=Session->GetSubjects()->GetTop();
		RQuery sub(Db,"SELECT topicid,name,used FROM topics WHERE parent=0");
		for(sub.Start();!sub.End();sub.Next())
		{
			Session->GetSubjects()->InsertNode(Top,subject=new GSubject(atoi(sub[0]),sub[1],atoi(sub[2])));
			sSql="SELECT topicid,name,used FROM topics WHERE parent="+sub[0];
			RQuery subsub(*Db,sSql);
			for(subsub.Start();!subsub.End();subsub.Next())
				Session->GetSubjects()->InsertNode(subject,subsubject=new GSubject(atoi(subsub[0]),subsub[1],atoi(subsub[2])));
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
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveSubjects(void)
{
	try
	{
		// Verify the subjects exists
		if(!Session->GetSubjects()) return;
		RString sSql;

		if(Session->GetSlot())
			Session->GetSlot()->StartJob("Save subjects");

		// Clear all subjects information
		RQuery Del1(Db,"DELETE FROM topicsbyhtmls");
		RQuery Del2(Db,"DELETE FROM topics");
		RQuery Del3(Db,"UPDATE profiles SET topicid=0");

		RCursor<GSubject> Cur(Session->GetSubjects()->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			size_t parent(0);
			GSubject* ptr=Cur()->GetParent();
			if(ptr)
				parent=ptr->GetId();
			if(!ptr) continue;
			RString used;
			if(Cur()->IsUsed())
				used=",'1')";
			else
				used=",'0')";

			// Insert the topic
			sSql="INSERT INTO topics(topicid,name,parent,used) VALUES("+
				Num(Cur()->GetId())+",'"+Cur()->GetName()+"',"+Num(parent)+used;
			RQuery Ins1(Db,sSql);

			// Assign the documents to the topic
			RCursor<GDoc> Docs(Cur()->GetDocs());
			for(Docs.Start();!Docs.End();Docs.Next())
				RQuery(Db,"INSERT INTO topicsbyhtmls(topicid,htmlid) VALUES("+Num(Cur()->GetId())+","+Num(Docs()->GetId())+")");

			// AsSubPsign the profiles to the topic
			RCursor<GProfile> Profiles(Cur()->GetProfiles());
			for(Profiles.Start();!Profiles.End();Profiles.Next())
				RQuery(Db,"UPDATE profiles SET topicid="+Num(Cur()->GetId())+" WHERE profileid="+Num(Profiles()->GetId()));
		}
	}
	catch(RMySQLError e)
	{
		if(Session->GetSlot())
			Session->GetSlot()->EndJob();
		cerr<<e.GetMsg()<<endl;
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
		while(!Sql.End())
		{
			l=Sql.GetLine();
			RQuery exec(Db,l);
		}
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
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
		cerr<<e.GetMsg()<<endl;
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
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::Clear(tObjType objtype)
{
	try
	{
		RString What;
		bool Desc(false);
		bool Group(false);

		switch(objtype)
		{
			case otUser:
				What="users";
				break;
			case otProfile:
				What="profiles";
				Desc=true;
				break;
			case otCommunity:
				Group=true;
				What="groups";
				Desc=true;
				break;
			case otFdbk:
				What="htmlsbyprofiles";
				break;
			default:
				throw GException("Cannot clear "+GetObjType(objtype)+" from storage");
		}
		RQuery Delete(Db,"DELETE FROM "+What);
		if(Desc)
			RQuery DeleteInfos(Db,"DELETE FROM "+What+"byconcepts");
		// If groups -> All profiles are detached
		if(Group)
			RQuery Update(Db,"UPDATE profiles SET groupid=0,attached=NULL");
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}



//------------------------------------------------------------------------------
//
// Knowledge Methods
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::LoadConceptTypes(void)
{
	try
	{
		RQuery Types(Db,"SELECT typeid,name,description,refdocs,refprofiles,refgroups FROM concepttypes");
		for(Types.Start();!Types.End();Types.Next())
			Session->InsertConceptType(atoi(Types[0]),Types[1],Types[2],atoi(Types[3]),atoi(Types[4]),atoi(Types[5]));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GConceptType* type)
{
	try
	{
		// Init some strings
		RString name=RQuery::SQLValue(type->GetName());
		RString desc;
		if(type->GetDescription().IsEmpty())
			desc=name;
		else
			desc=RQuery::SQLValue(type->GetDescription());

		// Verify that the concepttype didn't already exist.
		RString sSql="SELECT typeid FROM concepttypes WHERE name="+name;
		RQuery find(Db,sSql);
		if(find.GetNb())
		{
			find.Start();
			type->SetId(strtoul(find[0],0,10));
			return;
		}

		// Insert the new concept type
		sSql="INSERT INTO concepttypes(name,description) VALUES("+name+","+desc+")";
		RQuery insert(Db,sSql);

		// Get the next id
		sSql=RString("SELECT typeid FROM concepttypes WHERE typeid=LAST_INSERT_ID()");
		RQuery getinsert(Db,sSql);
		getinsert.Start();
		type->SetId(strtoul(getinsert[0],0,10));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadRelationTypes(void)
{
	try
	{
		RQuery Types(Db,"SELECT typeid,name,description FROM relationtypes");
		for(Types.Start();!Types.End();Types.Next())
			Session->InsertRelationType(atoi(Types[0]),Types[1],Types[2]);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadRelations(void)
{
	try
	{
		RQuery Rel(Db,"SELECT relationid,name,subjecttid,subjecttypeid,typeid,objectid,objecttypeid,weight FROM relations");
		for(Rel.Start();!Rel.End();Rel.Next())
			Session->InsertRelation(atoi(Rel[0]),Rel[1],atoi(Rel[2]),atoi(Rel[3]),atoi(Rel[4]),atoi(Rel[5]),atoi(Rel[8]),atof(Rel[9]));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadInfos(RContainer<GWeightInfo,false,true>& infos,tObjType type,size_t id)
{
	try
	{
		size_t i;
		RString sSql,table;

		switch(type)
		{
			case otDoc:
				table="html";
				break;
			case otProfile:
				table="profile";
				break;
			case otCommunity:
				table="group";
				break;
			default:
				throw GException("This type of objects do not have descriptions");
		};
		sSql="SELECT conceptid,typeid,weight FROM "+table+"sbyconcepts "
		     "WHERE "+table+"id="+Num(id)+" ORDER BY typeid,conceptid";
		GConceptType* type;
		size_t tid=0;
		RQuery sel(Db,sSql);
		for(sel.Start(),i=0;!sel.End();sel.Next(),i++)
		{
			if(tid!=static_cast<size_t>(atol(sel[1])))
			{
				tid=atol(sel[1]);
				type=Session->GetConceptType(tid,false);
			}
			infos.InsertPtrAt(new GWeightInfo(type->GetConcept(atoi(sel[0])),atof(sel[2])),i,false);
		}
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadConcepts(GConceptType* type)
{
	unsigned int MaxCount=100;
	unsigned int MaxId=0;
	RString sSql;

	try
	{
		bool Index=(type->GetName()=="XMLIndex");
		// Construct the table name
		if(GSession::Get()&&GSession::Get()->GetSlot())
			GSession::Get()->GetSlot()->StartJob("Load Dictionnary ("+type->GetDescription()+")");

		// Search the values to initialise the dictionary
		for(char i='a';i<='z';i++)
		{
			sSql="SELECT COUNT(*) FROM concepts WHERE typeid="+Num(type->GetId())+" AND name LIKE '"+RString(i)+"%'";
			RQuery count(Db,sSql);
			count.Start();
			if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
		}
		if(MaxCount==0) MaxCount=2000;
		sSql="SELECT MAX(conceptid) FROM concepts WHERE typeid="+Num(type->GetId());
		RQuery count(Db,sSql);
		count.Start();
		RString c=count[0];
		if(c.IsEmpty())
			MaxId=2000;
		else
			MaxId=atoi(c);

		// Create and insert the dictionary
		// Load the dictionary from the database
		sSql="SELECT conceptid,name,refdocs,refprofiles,refgroups FROM concepts WHERE typeid="+Num(type->GetId());
		RQuery dicts(Db, sSql);
		for(dicts.Start();!dicts.End();dicts.Next())
		{
			if(Index)
			{
				GXMLIndex w(atoi(dicts[0]),dicts[1],type,atoi(dicts[2]),atoi(dicts[3]),atoi(dicts[4]));
				type->InsertConcept(&w);
			}
			else
			{
				GConcept w(atoi(dicts[0]),dicts[1],type,atoi(dicts[2]),atoi(dicts[3]),atoi(dicts[4]));
				type->InsertConcept(&w);
			}
		}
		if(GSession::Get()&&GSession::Get()->GetSlot())
			GSession::Get()->GetSlot()->EndJob();
	}
	catch(RMySQLError e)
	{
		if(GSession::Get()&&GSession::Get()->GetSlot())
			GSession::Get()->GetSlot()->EndJob();
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GConcept* concept)
{
	try
	{
		// Init some strings
		RString name=RQuery::SQLValue(concept->GetName());
		RString type=Num(concept->GetType()->GetId());

		// Verify that the concept didn't already exist.
		RString sSql="SELECT conceptid FROM concepts WHERE typeid="+type+" AND name="+name;
		RQuery find(Db,sSql);
		if(find.GetNb())
		{
			find.Start();
			concept->SetId(strtoul(find[0],0,10));
			return;
		}

		// Insert the new concept
		sSql="INSERT INTO concepts(name,conceptid,typeid) SELECT "+name+",IFNULL(MAX(conceptid),0)+1,"+type+
			 " FROM concepts WHERE typeid="+type;
		RQuery insert(Db,sSql);

		// Get the next id
		sSql=RString("SELECT conceptid FROM concepts WHERE conceptautoid=LAST_INSERT_ID()");
		RQuery getinsert(Db,sSql);
		getinsert.Start();
		concept->SetId(strtoul(getinsert[0],0,10));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::DeleteConcept(GConcept* concept)
{
	try
	{
		RQuery Del(Db,"DELETE FROM concepts WHERE typeid="+
		              Num(concept->GetType()->GetId())+" AND conceptid="+Num(concept->GetId()));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
RString GStorageMySQL::LoadConcept(unsigned int id,GConceptType* type)
{
	try
	{
		RString res;
		RString sSql("SELECT name FROM concepts WHERE typeid="+Num(type->GetId())+" AND conceptid="+Num(id));
		RQuery w(Db,sSql);
		if(w.GetNb())
		{
			w.Start();
			res=w[0];
		}
		return(res);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
size_t GStorageMySQL::LoadConcept(const RString name,GConceptType* type)
{
	try
	{
		unsigned int res=0;
		RString sSql("SELECT conceptid FROM concepts WHERE typeid="+Num(type->GetId())+" AND kwd="+RQuery::SQLValue(name));
		RQuery w(Db,sSql);
		if(w.GetNb())
		{
			w.Start();
			res=atoi(w[0].Latin1());
		}
		return(res);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveConcept(GConcept* concept)
{
	try
	{
		RString Sql;

		// Delete the old word from the database
		RQuery Delete(Db,"DELETE FROM concepts WHERE "
		                 "conceptid="+Num(concept->GetId())+" AND "
		                 "typeid="+Num(concept->GetType()->GetId()));

		// Insert the new word in the database
		Sql="INSERT INTO concepts(conceptid,name,typeid,refprofiles,refgroups,refdocs) ";
		Sql+="VALUES("+Num(concept->GetId())+","+RQuery::SQLValue(concept->GetName())+","+
		     Num(concept->GetType()->GetId())+",0,0,0)";
		RQuery Insert(Db,Sql);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveRefs(const GConcept* concept,tObjType what,size_t refs)
{
	try
	{
		RString sSql;

		switch(what)
		{
			case otDoc:
				sSql="UPDATE concepts SET refdocs="+Num(refs)+
				     " WHERE conceptid="+Num(concept->GetId())+" AND typeid="+Num(concept->GetType()->GetId());
				break;
			case otProfile:
				sSql="UPDATE concepts SET refprofiles="+Num(refs)+
				     " WHERE conceptid="+Num(concept->GetId())+" AND typeid="+Num(concept->GetType()->GetId());
				break;
			case otCommunity:
				sSql="UPDATE concepts SET refgroups="+Num(refs)+
				     " WHERE conceptid="+Num(concept->GetId())+" AND typeid="+Num(concept->GetType()->GetId());
				break;
			default:
				throw GException("This type of objects do not have descriptions");
		};
		RQuery(Db,sSql);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveRefs(GConceptType* type,tObjType what,size_t refs)
{
	try
	{
		switch(what)
		{
			case otDoc:
			{
				RQuery(Db,"UPDATE concepttypes SET refdocs="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				if(refs==0)
					RQuery(Db,"UPDATE concepts SET refdocs=0 WHERE typeid="+Num(type->GetId()));
				break;
			}
			case otProfile:
			{
				RQuery(Db,"UPDATE concepttypes SET refprofiles="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				if(refs==0)
					RQuery(Db,"UPDATE concepts SET refgroups=0 WHERE typeid="+Num(type->GetId()));
				break;
			}
			case otCommunity:
			{
				RQuery(Db,"UPDATE concepttypes SET refgroups="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				if(refs==0)
					RQuery(Db,"UPDATE concepts SET refgroups=0 WHERE typeid="+Num(type->GetId()));
				break;
			}
			default:
				throw GException("This type of objects do not have descriptions");
		};
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}



//------------------------------------------------------------------------------
//
// Documents Methods
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GDoc* doc)
{
	try
	{
		// Reserved an identificator
		RString sSql="INSERT INTO htmls(html,title,langid) "
		             "VALUES("+RQuery::SQLValue(doc->GetURL())+","+RQuery::SQLValue(doc->GetName())+","+Lang(doc->GetLang())+")";
		RQuery Insert(Db,sSql);

		// Get the id and assign it to the document
		sSql=RString("SELECT htmlid FROM htmls WHERE htmlid=LAST_INSERT_ID()");
		RQuery Get(Db,sSql);
		Get.Start();
		doc->SetId(atoi(Get[0]));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
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
			sSql="SELECT COUNT(*) FROM concepts WHERE name LIKE '"+RString(i)+"%'";
			RQuery count(Db,sSql);
			count.Start();
			if(strtoul(count[0],0,10)>MaxCount) MaxCount=strtoul(count[0],0,10);
		}
		if(MaxCount==0) MaxCount=2000;

		// Create and insert the dictionary
		indexer=new GIndexer(MaxCount,langs);

		// Load the stems from the database
		sSql="SELECT name FROM concepts WHERE typeid='1'";
		RQuery stems(Db, sSql);
		for(stems.Start();!stems.End();stems.Next())
		{
			indexer->InsertWord(stems[0]);
		}

		// Index the documents
		RQuery sel(Db,"SELECT htmlid,name FROM htmlsbyconcepts,concepts "
		              "WHERE concepts.conceptid=htmlsbyconcepts.conceptid AND "
		              "concepts.typeid=htmlsbyconcepts.typeid");
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
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GDoc* GStorageMySQL::LoadDoc(unsigned int docid)
{
	try
	{
		GDoc* doc;

		RQuery quer (Db,"SELECT htmlid,html,title,mimetype,langid,updated,calculated,owner "
		                "FROM htmls WHERE htmlid="+Num(docid));
		quer.Start();
		if(!quer.GetNb())
			return(0);

		// Verify that the langague is active
		GLang* lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn(quer[4],false);
		if((!lang)&&(!quer[4].IsEmpty()))
			return(0);

		doc=new GDoc(quer[1],quer[2],docid,lang,quer[3],GetMySQLToDate(quer[5]),GetMySQLToDate(quer[6]),atoi(quer[7]));
		doc->SetState(osNeedLoad);

		// Load the links of the document loaded.
		RQuery querLinks (Db,"SELECT htmlid,linkid,occurs FROM htmlsbylinks WHERE htmlid="+Num(docid));
		for(querLinks.Start(); !querLinks.End(); querLinks.Next())
			doc->InsertLink(Session->GetDoc(atoi(querLinks[1])), atoi(querLinks[2]));

		return(doc);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadNode(RQuery& nodes,RQuery& content,const GDoc* doc,GDocStruct* docstruct,GDocStructNode* parent/*,size_t tnodes,size_t nbnodes,size_t contents,size_t nbcontents*/)
{
	try
	{
		size_t NbSubNodes(atoi(nodes[3]));
		size_t NbContent(atoi(nodes[5]));
//		cout<<"Treat node "<<nodes[8]<<" - Nodes: "<<nodes[2]<<"("<<NbSubNodes<<") - Content: "<<nodes[5]<<"("<<NbContent<<")"<<endl;

		// Create the node in the structure
		GConceptType* type(Session->GetConceptType(atoi(nodes[1]),false));
		GConcept* concept=type->GetConcept(atoi(nodes[0]));
		GDocStructNode* ptr=new GDocStructNode(doc->GetPtr(concept),atoi(nodes[6]),static_cast<GDocStructNode::NodeType>(atoi(nodes[7])));
		docstruct->InsertNode(parent,ptr);

		// Go to the next node
		nodes.Next();

		// Load its content
		for(size_t i=NbContent+1;--i;content.Next())
		{
			GConceptType* type(Session->GetConceptType(atoi(content[1]),false));
			GConcept* concept=type->GetConcept(atoi(content[0]));
			docstruct->InsertContent(ptr,doc->GetPtr(concept),atoi(content[2]));
		}

		// Load SubNodes
		for(size_t i=NbSubNodes+1;--i;)
			LoadNode(nodes,content,doc,docstruct,ptr);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}



//------------------------------------------------------------------------------
GDocStruct* GStorageMySQL::LoadStruct(const GDoc* doc)
{
	try
	{
		if(doc->GetId()==cNoRef)
			return(0);
		RQuery Find(Db,"SELECT totalnodes,nodes,nbnodes,content,nbcontent FROM htmls WHERE htmlid="+Num(doc->GetId()));
		Find.Start();
		if(!Find.GetNb())
			return(0);
		GDocStruct* docstruct=new GDocStruct(strtoul(Find[0],0,10));

		// Create the queries for all nodes and contents
		size_t FirstNode(atoi(Find[1]));
		size_t OutNode(FirstNode+atoi(Find[0]));
		RQuery Nodes(Db,"SELECT conceptid,typeid,childs,nbchilds,contents,nbcontents,pos,what,nodeid FROM nodes "
				 "WHERE nodeid>="+Find[1]+" AND nodeid<"+Num(OutNode)+" ORDER BY nodeid");
		Nodes.Start();
		size_t FirstContent(atoi(Find[3]));
		size_t OutContent(FirstContent+atoi(Find[4]));
		RQuery Content(Db,"SELECT conceptid,typeid,pos FROM content "
				"WHERE contentid>="+Find[3]+" AND contentid<"+Num(OutContent)+" ORDER BY contentid");
		Content.Start();
		size_t NbSubNodes(atoi(Find[2]));

		for(size_t i=NbSubNodes+1;--i;)
			LoadNode(Nodes,Content,doc,docstruct,docstruct->GetTop()/*,FirstNode,NbSubNodes,0,0*/);
		return(docstruct);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
size_t GStorageMySQL::SaveNode(GDocStructNode* node,size_t& topcontent,size_t& nbcontent)
{
	size_t first(0);
	size_t id;

	// Insert all the subnodes first
	RCursor<GDocStructNode> Nodes(node->GetNodes());
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		// Insert the node in the table
		RString Sql("INSERT INTO nodes(conceptid,typeid,pos,what) VALUES("+
				Num(Nodes()->GetInfo()->GetId())+","+Num(Nodes()->GetInfo()->GetTypeId())+","+
				Num(Nodes()->GetPos())+","+Num(static_cast<size_t>(Nodes()->GetType()))+")");
		RQuery InsNode(Db,Sql);

		// Look if the id of the node must be get
		if(!first)
		{
			RQuery GetId(Db,"SELECT nodeid FROM nodes WHERE nodeid=LAST_INSERT_ID()");
			GetId.Start();
			first=strtoul(GetId[0],0,10);
		}
	}

	// Insert now all information related to each node
	for(Nodes.Start(),id=first;!Nodes.End();Nodes.Next(),id++)
	{
		// Save their content
		size_t contents(0);
		RCursor<GOccurInfo> Content(Nodes()->GetContent());
		nbcontent+=Content.GetNb();
		for(Content.Start();!Content.End();Content.Next())
		{
			RString Sql("INSERT INTO content(conceptid,typeid,pos) VALUES("+
									Num(Content()->GetInfo()->GetId())+","+Num(Content()->GetInfo()->GetTypeId())+","+
									Num(Content()->GetPos())+")");
			RQuery InsCont(Db,Sql);

			if(!contents)
			{
				RQuery GetId(Db,"SELECT contentid FROM content WHERE contentid=LAST_INSERT_ID()");
				GetId.Start();
				contents=strtoul(GetId[0],0,10);
			}

			if(!topcontent)
				topcontent=contents;
		}

		// Save their child nodes
		size_t childs=SaveNode(Nodes(),topcontent,nbcontent);

		// Update node
		RQuery Update(Db,"UPDATE nodes SET childs="+Num(childs)+",nbchilds="+Num(Nodes()->GetNbNodes())+
					",contents="+Num(contents)+",nbcontents="+Num(Content.GetNb())+" WHERE nodeid="+Num(id));
	}

	return(first);
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveStruct(GDocStruct* docstruct,size_t docid)
{
	try
	{
		// Remove the old structure
		RQuery Info(Db,"SELECT nodes,totalnodes,content,nbcontent FROM htmls WHERE htmlid="+Num(docid));
		Info.Start();
		if(Info.GetNb())
		{
			size_t LastId=atoi(Info[0])+atoi(Info[1]);
			RQuery DelNodes(Db,"DELETE FROM nodes WHERE nodeid>="+Info[0]+" AND nodeid<"+Num(LastId));
			LastId=atoi(Info[2])+atoi(Info[3]);
			RQuery DelContent(Db,"DELETE FROM content WHERE contentid>="+Info[2]+" AND contentid<"+Num(LastId));
		}

		if((!docstruct)||(!docstruct->GetTop()->GetNbNodes()))
			return;

		// Save the structure
		size_t topcontent(0);
		size_t nbcontent(0);
		size_t topnode(SaveNode(docstruct->GetTop(),topcontent,nbcontent));
		RString Sql("UPDATE htmls SET "
				"nodes="+Num(topnode)+
				",nbnodes="+Num(docstruct->GetTop()->GetNbNodes())+
				",totalnodes="+Num(docstruct->GetNbNodes()-1)+
				",content="+Num(topcontent)+
				",nbcontent="+Num(nbcontent)+
				" WHERE htmlid="+Num(docid));
		RQuery UpdateDoc(Db,Sql);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadDocs(void)
{
	GDoc* doc;
	GLang* lang;
	unsigned int docid;

	try
	{
		RString Sql("SELECT htmlid,html,title,mimetype,langid,updated,calculated,owner FROM htmls");
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
			lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn(quer[4],false);
			if((!lang)&&(!quer[4].IsEmpty()))
				continue;

			docid=atoi(quer[0]);
			doc=new GDoc(quer[1],quer[2],docid,lang,quer[3],GetMySQLToDate(quer[5]),GetMySQLToDate(quer[6]),atoi(quer[7]));
			Session->InsertDoc(doc);
			doc->SetState(osNeedLoad);

			// Load the links of the document loaded.
			RQuery querLinks (Db,"SELECT htmlid,linkid,occurs FROM htmlsbylinks "
			                     "WHERE htmlid="+Num(docid));
			for(querLinks.Start(); !querLinks.End(); querLinks.Next())
				doc->InsertLink(Session->GetDoc(atoi(querLinks[1])), atoi(querLinks[2]));
		}
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
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
		// Delete keywords
		if(doc->GetLang())
		{
			sSql="DELETE FROM htmlsbyconcepts "
			     "WHERE htmlid="+Num(doc->GetId());
			RQuery deletekwds(Db,sSql);
			Words=doc->GetInfos();
			for(Words.Start();!Words.End();Words.Next())
			{
				sSql="INSERT INTO htmlsbyconcepts(htmlid,conceptid,weight,typeid) "
				     "VALUES("+Num(doc->GetId())+","+Num(Words()->GetId())+","+Num(Words()->GetWeight())+","+
				     Num(Words()->GetType()->GetId())+")";
				RQuery insertkwds(Db,sSql);
			}
			l=Lang(doc->GetLang());
		}
		else
		{
			l="NULL";
		}

		// Mime type
		f=doc->GetMIMEType();
		if(f.IsEmpty())
			f="NULL";
		else
			f=RQuery::SQLValue(f);

		// Test if the document already exists.
		sSql="SELECT COUNT(1) FROM htmls WHERE htmlid="+Num(doc->GetId());
		RQuery Test(Db,sSql);
		Test.Start();
		if(!atoi(Test[0]))
		{
			// Insert the document
			sSql="INSERT INTO htmls(htmlid,html,title,mimetype,langid,updated,calculated) "
			     "VALUES("+Num(doc->GetId())+","+RQuery::SQLValue(doc->GetURL())+","+
			     RQuery::SQLValue(doc->GetName())+","+f+","+l+","+RQuery::SQLValue(doc->GetUpdated())+
			     ","+RQuery::SQLValue(doc->GetComputed())+")";
			RQuery Insert(Db,sSql);
		}
		else
		{
			// Update the document
			sSql="UPDATE htmls SET html="+RQuery::SQLValue(doc->GetURL())+",title="+
			     RQuery::SQLValue(doc->GetName())+",mimetype="+f+",langid="+l+
			     ",updated="+RQuery::SQLValue(doc->GetUpdated())+",calculated="+RQuery::SQLValue(doc->GetComputed())+
				 " WHERE htmlid="+Num(doc->GetId());
			RQuery Update(Db,sSql);
		}

		// Update links to others documents
		RQuery deletelinks(Db,"DELETE FROM htmlsbylinks WHERE htmlid="+Num(doc->GetId()));
		lcur= doc->GetLinks();
		for ( lcur.Start(); ! lcur.End(); lcur.Next())
		{
			sSql="INSERT INTO htmlsbylinks(htmlid,linkid,occurs) VALUES("+Num(doc->GetId())+","+Num(lcur()->GetId())+","+Num(lcur()->GetOccurs())+")";
			RQuery insertkwds(Db,sSql);
		}
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}



//------------------------------------------------------------------------------
//
// Users/Profiles Methods
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::LoadUsers(void)
{
	GProfile* prof;

	try
	{
		if(IsAllInMemory()) // If everything must be in memory -> load all the users and the profiles
		{
			// Load users
			RQuery Users(Db, "SELECT userid,user,fullname FROM users");
			for(Users.Start();!Users.End();Users.Next())
				Session->InsertUser(new GUser(atoi(Users[0]),Users[1],Users[2],10));

			// Load profiles
			RString Sql("SELECT profileid,description,social,userid,attached,groupid,updated,calculated FROM profiles");
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
			RQuery Profiles(Db,Sql);
			for(Profiles.Start();!Profiles.End();Profiles.Next())
			{
				GUser* user=Session->GetUser(atoi(Profiles[3]));
				unsigned int groupid=atoi(Profiles[5]);
				if(!groupid)
					groupid=cNoRef;
				Session->InsertProfile(prof=new GProfile(user,atoi(Profiles[0]),Profiles[1],groupid,GetMySQLToDate(Profiles[4]),GetMySQLToDate(Profiles[6]),GetMySQLToDate(Profiles[7]),(atoi(Profiles[2])==1),5));
				prof->SetState(osNeedLoad);
			}

			// Load feedbacks
			RQuery fdbks(Db,"SELECT htmlid,judgement,profileid,when2,computed FROM htmlsbyprofiles");
			for(fdbks.Start();!fdbks.End();fdbks.Next())
				Session->InsertFdbk(atoi(fdbks[2]),atoi(fdbks[0]),GetAssessmentType(fdbks[1]),GetMySQLToDate(fdbks[3]),GetMySQLToDate(fdbks[4]));
		}
	}
	catch(RMySQLError& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GUser* GStorageMySQL::LoadUser(unsigned int userid)
{
	try
	{
		RQuery User(Db,"SELECT userid,user,fullname FROM users WHERE userid="+Num(userid));
		User.Start();
		if(!User.GetNb())
			return(0);
		return(new GUser(atoi(User[0]),User[1],User[2],10));
	}
	catch(RMySQLError& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GUser* GStorageMySQL::LoadUser(const R::RString name)
{
	try
	{
		RQuery User(Db,"SELECT userid,user,fullname FROM users WHERE user="+RQuery::SQLValue(name));
		User.Start();
		if(!User.GetNb())
			return(0);
		return(new GUser(atoi(User[0]),User[1],User[2],10));
	}
	catch(RMySQLError& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GProfile* GStorageMySQL::LoadProfile(unsigned int profileid)
{
	try
	{
		GLangManager* Langs=GALILEIApp->GetManager<GLangManager>("Lang");

		// Load Profile
		RQuery Profile(Db,"SELECT profileid,description,social,userid,attached,groupid,updated,calculated "
		                  "FROM profiles WHERE profileid="+Num(profileid));
		Profile.Start();
		if(!Profile.GetNb())
			return(0);
		GUser* user=Session->GetUser(atoi(Profile[3]));
		if(!user)
			throw GException("Profile "+Profile[0]+" has no parent user");
		unsigned int groupid=atoi(Profile[5]);
		if(!groupid)
			groupid=cNoRef;

		// Create the profile
		GProfile* prof=new GProfile(user,atoi(Profile[0]),Profile[1],groupid,GetMySQLToDate(Profile[4]),GetMySQLToDate(Profile[6]),GetMySQLToDate(Profile[7]),(atoi(Profile[2])==1),5);
		prof->SetState(osNeedLoad);

		// Load Feedbacks
		RQuery fdbks(Db,"SELECT htmlid,judgement,profileid,when2,computed "
		                "FROM htmlsbyprofiles WHERE profileid="+Num(profileid));
		for(fdbks.Start();!fdbks.End();fdbks.Next())
		{
			Session->InsertFdbk(atoi(fdbks[2]),atoi(fdbks[0]),GetAssessmentType(fdbks[1]),RDate(fdbks[3]),RDate(fdbks[4]));
			// Since the profile is not in the session -> we must manually insert the profile.
			GLang* lang=Langs->GetPlugIn(fdbks[5],false);
			if(!lang)
				continue;
			prof->InsertFdbk(atoi(fdbks[0]),GetAssessmentType(fdbks[1]),RDate(fdbks[3]),RDate(fdbks[4]));
		}
		return(prof);
	}
	catch(RMySQLError& e)
	{
		cerr<<e.GetMsg()<<endl;
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
		sSql="SELECT profileid,htmlid,rank,test FROM sugsbyprofiles "
		     "WHERE test="+RQuery::SQLValue(name)+" ORDER BY profileid,rank";
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
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::UpdateProfiles(unsigned int docid)
{
	try
	{
		RQuery Up(Db,"UPDATE htmlsbyprofiles SET computed=CURDATE() WHERE htmlid="+Num(docid));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GUser* user)
{
	try
	{
		// Reserved an identificator
		RString sSql=RString("INSERT INTO users(user,fullname) VALUES("+RQuery::SQLValue(user->GetName())+","+RQuery::SQLValue(user->GetFullName())+")");
		RQuery Insert(Db,sSql);

		// Get the id and assign it to the user
		sSql=RString("SELECT userid FROM users WHERE userid=LAST_INSERT_ID()");
		RQuery Get(Db,sSql);
		Get.Start();
		user->SetId(atoi(Get[0]));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GProfile* p)
{
	try
	{
		// Reserved an identificator
		RString sSql=RString("INSERT INTO profiles(userid,description) "
		                     "VALUES("+Num(p->GetUser()->GetId())+","+RQuery::SQLValue(p->GetName())+")");
		RQuery Insert(Db,sSql);

		// Get the id and assign it to the profile
		sSql=RString("SELECT profileid FROM profiles WHERE profileid=LAST_INSERT_ID()");
		RQuery Get(Db,sSql);
		Get.Start();
		p->SetId(atoi(Get[0]));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveUser(GUser* user)
{
	unsigned int userid;
	RString sSql;

	try
	{
		userid=user->GetId();

		// Test if the user already exists.
		sSql="SELECT COUNT(1) FROM users WHERE userid="+Num(userid);
		RQuery Test(Db,sSql);
		Test.Start();
		if(!atoi(Test[0]))
		{
			// Insert the user
			sSql="INSERT INTO users(userid,user,fullname) "
			     "VALUES("+Num(userid)+","+RQuery::SQLValue(user->GetName())+","+RQuery::SQLValue(user->GetFullName())+")";
			RQuery Insert(Db,sSql);
		}
		else
		{
			// Update the user
			sSql="UPDATE users SET user="+RQuery::SQLValue(user->GetName())+",fullname="+RQuery::SQLValue(user->GetFullName())+
			     " WHERE userid="+Num(userid);
			RQuery Update(Db,sSql);
		}
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveProfile(GProfile* prof)
{
	unsigned int profid;
	unsigned int social;
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
		sSql="SELECT COUNT(1) FROM profiles WHERE profileid="+Num(profid);
		RQuery Test(Db,sSql);
		Test.Start();
		if(!atoi(Test[0]))
		{
			// Insert the profile (if subjects -> save topicid)
			sSql="INSERT INTO profiles(profileid,description,social,userid";
			if(Session->GetSubjects(false))
				sSql+=",topicid";
			sSql+=") VALUES("+Num(profid)+","+RQuery::SQLValue(prof->GetName())+","+
			      Num(social)+","+Num(prof->GetUser()->GetId());
			if(Session->GetSubjects(false))
			{
				GSubject* sub=Session->GetSubjects(false)->GetSubject(prof);
				if(sub)
					sSql+=","+RString::Number(sub->GetId());
				else
					sSql+=",0";
			}
			sSql+=")";
			RQuery Insert(Db,sSql);
		}
		else
		{
			// Update the profile (if subjects -> save topicid)
			sSql="UPDATE profiles SET description="+RQuery::SQLValue(prof->GetName())+",social="+Num(social)+
			     ",userid="+Num(prof->GetUser()->GetId());
			if(Session->GetSubjects(false))
			{
				GSubject* sub=Session->GetSubjects(false)->GetSubject(prof);
				if(sub)
					sSql+=",topicid="+Num(sub->GetId());
				else
					sSql+=",topicid=0";
			}
			sSql+=" WHERE profileid="+Num(profid);
			RQuery Update(Db,sSql);
		}

		// First insert information from GFdbk
		RCursor<GFdbk> Fdbks=prof->GetFdbks();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Clear the feedback
			RQuery Delete(Db,"DELETE FROM htmlsbyprofiles WHERE profileid="+Num(profid)+" AND htmlid="+Num(Fdbks()->GetDocId()));

			// Re-Insert all the feedback
			sSql="INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2) "
			     "VALUES("+Num(Fdbks()->GetDocId())+",'"+GetAssessmentCode(Fdbks()->GetFdbk())+"',"+
			     Num(prof->GetId())+","+RQuery::SQLValue(Fdbks()->GetWhen())+")";
			RQuery Insert(Db,sSql);
		}

		// Update other information from the documents
		RQuery Update(Db,"UPDATE htmls,htmlsbyprofiles SET htmlsbyprofiles.langid=htmls.langid,htmlsbyprofiles.computed=htmls.calculated "
		                 "WHERE htmlsbyprofiles.htmlid=htmls.htmlid AND profileid="+Num(prof->GetId()));

		// Delete old description
		sSql="DELETE FROM profilesbyconcepts "
		     "WHERE profileid="+Num(prof->GetId());
		RQuery deletekwds(Db,sSql);

		// Insert new description
		RCursor<GWeightInfo> Cur(prof->GetInfos());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			sSql="INSERT INTO profilesbyconcepts(profileid,conceptid,weight,typeid) "
			     "VALUES("+Num(prof->GetId())+","+Num(Cur()->GetId())+","+Num(Cur()->GetWeight())+
			     ","+Num(Cur()->GetType()->GetId())+")";
			RQuery insertkwds(Db,sSql);
		}
		prof->SetState(osUpToDate);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveProfileInHistory(GProfile* sub,unsigned int historicID)
{
	RCursor<GWeightInfo> Cur;

	try
	{
		// Save profiles
		Cur=sub->GetInfos();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			RString sSql("INSERT INTO historicprofiles(historicID,profileid,kwdid,weight, date) VALUES("+
					     Num(historicID)+","+Num(sub->GetId())+","+Num(Cur()->GetId())+","+
					     Num(Cur()->GetWeight())+",CURDATE()");
			RQuery insertkwds(Db,sSql);
		}
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveHistoricProfiles(unsigned int /*historicID*/)
{
	try
	{
		RToImplement();
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddSugsProfile(const R::RString& name,unsigned int profileid,unsigned int docid,unsigned int rank)
{
	RString sSql;

	try
	{
		sSql="INSERT INTO sugsbyprofiles(profileid,htmlid,rank,test) "
		     "VALUES("+Num(profileid)+","+Num(docid)+","+Num(rank)+","+RQuery::SQLValue(name)+")";
		RQuery create(Db,sSql);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddFdbk(unsigned int p,unsigned int d,tDocAssessment assess,R::RDate date,R::RDate computed)
{
	try
	{
		RString sSql;
		sSql="INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2,computed) "
		     "VALUES("+Num(d)+",'"+GetAssessmentCode(assess)+"',"+Num(p)+","+RQuery::SQLValue(date)+
		     ","+RQuery::SQLValue(computed)+")";
		RQuery Insert(Db,sSql);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}



//------------------------------------------------------------------------------
//
// Groups Methods
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::LoadCommunities(void)
{
	GCommunity* group;
	R::RCursor<GCommunity> GroupsCursor;

	try
	{
		RString Sql("SELECT groupid,updated,calculated FROM groups");
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
			group=new GCommunity(atoi(Groups[0]),Groups[1],Groups[2]);
			group->SetState(osNeedLoad);
			Session->InsertCommunity(group);
		}
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GCommunity* GStorageMySQL::LoadCommunity(unsigned int groupid)
{
	try
	{
		RQuery Group(Db,"SELECT groupid,updated,calculated FROM groups WHERE groupid="+RString::Number(groupid));
		Group.Start();
		if(!Group.GetNb())
			return(0);
		GCommunity* group=new GCommunity(atoi(Group[0]),Group[1],Group[2]);
		group->SetState(osNeedLoad);

		return(group);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GCommunitiesHistory* GStorageMySQL::LoadHistoricCommunities(unsigned int historicID)
{
	RString sSql;
	GCommunityHistory* grp;
	GCommunitiesHistory* grps;
	GWeightInfosHistory* historicsubprof;
	unsigned int subprofid;
	unsigned int groupid;
	unsigned int v;

	try
	{
		// Init part
		groupid=cNoRef;

		sSql="SELECT date FROM historicgroups WHERE historicID="+Num(historicID);
		RQuery date(Db,sSql);
		date.Start();
		grps=new GCommunitiesHistory(historicID, date[0]);

		// Read the groupment.
		sSql="SELECT groupid,profileid,date FROM historicgroups "
		     "WHERE historicID="+Num(historicID)+" ORDER by historicID,groupid";
		RQuery grquery(Db,sSql);
		for(grquery.Start(),grp=0;!grquery.End();grquery.Next())
		{
			// Read Group
			v=atoi(grquery[0]);

			// If group id changed -> new group needed
			if((v!=groupid))
			{
				groupid=v;
				grp=new GCommunityHistory(groupid,grps);
				//insert group in the container of groups.
				grps->InsertPtr(grp);
			}

			// Create the historic profile and add it to the group
			subprofid=atoi(grquery[1]);
			historicsubprof=new GWeightInfosHistory(Session->GetProfile(0,subprofid),100);
			grp->AddProfile(historicsubprof);
			historicsubprof->SetParent(grp);

			// Fill the vector of the subprofile


			sSql="SELECT conceptid,typeid,weight FROM historicprofiles "
			     "WHERE profileid="+Num(subprofid)+" AND historicID="+Num(historicID)+" ORDER BY typeid,conceptid";
			GConceptType* type;
			size_t tid=0,i;
			RQuery sel(Db,sSql);
			for(sel.Start(),i=0;!sel.End();sel.Next(),i++)
			{
				if(tid!=static_cast<size_t>(atol(sel[1])))
				{
					tid=atol(sel[1]);
					type=Session->GetConceptType(tid,false);
				}
				historicsubprof->InsertPtrAt(new GWeightInfo(type->GetConcept(atoi(sel[0])),atof(sel[2])),i,false);
			}
		}
		return(grps);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadHistoricCommunitiesByDate(R::RString mindate, R::RString maxdate)
{
	try
	{
		RString sSql="SELECT DISTINCT historicID from historicgroups "
		             "WHERE date>'"+mindate+"' AND date<'"+maxdate+"' ORDER BY date,historicID";
		RQuery ids(Db,sSql);
		for (ids.Start(); !ids.End(); ids.Next())
			Session->GetCommunitiesHistoryManager()->InsertCommunitiesHistory(LoadHistoricCommunities(atoi(ids[0])));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::GetSugsCommunities(const R::RString& name,R::RContainer<GSugs,true,false>& res)
{
	RString sSql;
	unsigned int groupid,idx;
	GSugs* sugs;

	try
	{
		res.Clear();
		sSql="SELECT groupid,htmlid,rank,test "
		     "FROM sugsbygroups ORDER BY groupid,rank WHERE test="+RQuery::SQLValue(name);
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
				sugs=new GSugs(otCommunity,groupid,50);
			}
			sugs->AddSugs(atoi(load[1]));
		}
		if(groupid!=cNoRef)
			res.InsertPtr(sugs);

	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
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
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::UpdateCommunities(unsigned int subid)
{
	try
	{
		RQuery Up(Db,"UPDATE groups,profiles SET groups.updated=CURDATE() "
	    	         "WHERE groups.groupid=profiles.groupid AND "
	        	     "profiles.profileid="+Num(subid));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GCommunity* grp)
{
	try
	{
		// Reserved an identificator
		RString sSql="INSERT INTO groups(groupid) "
		             "SELECT IFNULL(MAX(groupid),0)+1 "
		             "FROM groups";
		RQuery Insert(Db,sSql);

		// Get the id and assign it to the group
		sSql=RString("SELECT groupid FROM groups WHERE groupautoid=LAST_INSERT_ID()");
		RQuery Get(Db,sSql);
		Get.Start();
		grp->SetId(atoi(Get[0]));
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveCommunities(void)
{
	RCursor<GWeightInfo> WordCur;
	R::RCursor<GCommunity> GroupsCursor;
	RString sSql;
	RCursor<GProfile> Sub;

	try
	{
		// Delete groups and goups info
		RQuery delete1(Db,"DELETE FROM groupsbyconcepts;");
		RQuery delete2(Db,"DELETE FROM groups");


		GroupsCursor=Session->GetCommunities();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			sSql="INSERT INTO groups(groupid,updated,calculated) "
			     "VALUES("+Num(GroupsCursor()->GetId())+","+
			     RQuery::SQLValue(GroupsCursor()->GetUpdated())+","+
			     RQuery::SQLValue(GroupsCursor()->GetComputed())+")";
			RQuery insert1(Db,sSql);

			// Save Profiles infos
			Sub=GroupsCursor()->GetObjs();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sSql="UPDATE profiles SET groupid="+Num(GroupsCursor()->GetId())+","
				     "attached="+RQuery::SQLValue(Sub()->GetAttached())+" "
				     "WHERE profileid="+Num(Sub()->GetId());
				RQuery update(Db,sSql);
			}

			// Save the description part
			WordCur.Set(*GroupsCursor());
			for(WordCur.Start();!WordCur.End();WordCur.Next())
			{
				sSql="INSERT INTO groupsbyconcepts(groupid,conceptid,weight,typeid) "
				     "VALUES("+Num(GroupsCursor()->GetId())+","+Num(WordCur()->GetId())+","+
				     Num(WordCur()->GetWeight())+","+Num(WordCur()->GetType()->GetId())+")";
				RQuery InserinfoWord(Db,sSql);
			}
		}
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveCommunitiesHistory(void)
{
	R::RCursor<GCommunity> GroupsCursor;
	RCursor<GProfile> Sub;
	unsigned int historicID;
	RString sSql;

	try
	{
		// If historic needed, gget the last historic id
		historicID=0;
		RQuery histcount(Db,"SELECT MAX(historicID) from historicgroups");
		histcount.Start();
		if(histcount[0])
			historicID=atoi(histcount[0]);
		historicID++;

		// Save the groups in history
		GroupsCursor=Session->GetCommunities();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			Sub=GroupsCursor()->GetObjs();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sSql="INSERT INTO historicgroups "
				     "SET date=CURDATE(), historicID="+Num(historicID)+",groupid="+Num(GroupsCursor()->GetId())+
					 ",profileid="+Num(Sub()->GetId());
				RQuery history(Db,sSql);
			}
		}

		// Save the profiles history if needed.
		SaveHistoricProfiles(historicID);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddSugsCommunity(const R::RString& name,unsigned int groupid,unsigned int docid,unsigned int rank)
{
	RString sSql;

	try
	{
		sSql="INSERT INTO sugsbygroups(groupid,htmlid,rank,test) "
		     "VALUES("+Num(groupid)+","+Num(docid)+","+Num(rank)+","+RQuery::SQLValue(name)+")";
		RQuery create(Db,sSql);
	}
	catch(RMySQLError e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}



//------------------------------------------------------------------------------
//
// End of the PlugIn
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("Host","127.0.0.1"));
	params->InsertParam(new RParamValue("User","root"));
	params->InsertParam(new RParamValue("Password",""));
	params->InsertParam(new RParamValue("Database",""));
	params->InsertParam(new RParamValue("Filtering",false));
	params->InsertParam(new RParamValue("Filter",""));
	params->InsertParam(new RParamValue("Encoding","Latin1"));
	params->InsertParam(new RParamValue("All",true));
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
