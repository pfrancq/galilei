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
// include files for R
#include <rvectorint.h>


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
#include <gtopic.h>
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
size_t GStorageMySQL::GetCount(RString tbl)
{
	RString c;
	RString sSql("SELECT COUNT(*) FROM "+tbl);

	auto_ptr<RQuery> count(Db->Query(sSql));
	count->Start();
	c=(*count)[0];
	if(c.IsEmpty()) return(0);
	return(atoi(c));
}


//------------------------------------------------------------------------------
size_t GStorageMySQL::GetMax(RString tbl,RString fld)
{
	RString sSql("SELECT MAX("+fld+") FROM "+tbl);
	RString c;

	auto_ptr<RQuery> count(Db->Query(sSql));
	count->Start();
	c=(*count)[0];
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
		Db.reset(RDb::Get(RDb::MySQL,Database,Host,User,Password,Encoding));
	}
	catch(RDbException e)
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
size_t GStorageMySQL::GetNbSaved(tObjType type)
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
	catch(RDbException e)
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
		auto_ptr<RQuery> create(Db->Query(sSql));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> del(Db->Query("DELETE FROM "+name));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddDummyEntry(RString name,size_t id,RString desc,size_t parentid)
{
	RString sSql;

	try
	{
		sSql="INSERT INTO "+name+" (id, parentid, description) VALUES("+RString::Number(id)+","+RString::Number(parentid)+","+RQuery::SQLValue(desc)+")";
		auto_ptr<RQuery> add(Db->Query(sSql));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
RQuery* GStorageMySQL::SelectDummyEntry(RString name,size_t id,RString desc,size_t parentid,size_t filter)
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
		select=Db->Query(sSql);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}

	return(select);
}


//------------------------------------------------------------------------------
void GStorageMySQL::ClearDummyEntry(RString name,size_t id,RString desc,size_t parentid,size_t filter)
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
		auto_ptr<RQuery> select(Db->Query(sSql));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> sub(Db->Query("SELECT subjectid,name,used FROM subjects WHERE parent=0"));
		for(sub->Start();!sub->End();sub->Next())
		{
			Session->GetSubjects()->InsertNode(Top,subject=new GSubject(atoi((*sub)[0]),(*sub)[1],atoi((*sub)[2])));
			sSql="SELECT subjectid,name,used FROM subjects WHERE parent="+(*sub)[0];
			auto_ptr<RQuery> subsub(Db->Query(sSql));
			for(subsub->Start();!subsub->End();subsub->Next())
				Session->GetSubjects()->InsertNode(subject,subsubject=new GSubject(atoi((*subsub)[0]),(*subsub)[1],atoi((*subsub)[2])));
		}

		//  Make Link between documents and subjects
		auto_ptr<RQuery> docs(Db->Query("SELECT htmlid,subjectid FROM subjectsbyhtmls"));
		for(docs->Start();!docs->End();docs->Next())
		{
			GDoc* d=Session->GetDoc(atoi((*docs)[0]));
			if(!d) continue;
			Session->GetSubjects()->InsertDocSubject(d,atoi((*docs)[1]));
		}

		// Make links between profiles and subjects
		auto_ptr<RQuery> profiles(Db->Query("SELECT profileid,subjectid FROM profiles"));
		for(profiles->Start();!profiles->End();profiles->Next())
		{
			GProfile* prof=Session->GetProfile(atoi((*profiles)[0]));
			if(!prof) continue;
			Session->GetSubjects()->InsertProfileSubject(prof,atoi((*profiles)[1]));
		}
		if(Session->GetSlot())
			Session->GetSlot()->EndJob();
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Del1(Db->Query("DELETE FROM subjectsbyhtmls"));
		auto_ptr<RQuery> Del2(Db->Query("DELETE FROM subjects"));
		auto_ptr<RQuery> Del3(Db->Query("UPDATE profiles SET subjectid=0"));

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
			sSql="INSERT INTO subjects(subjectid,name,parent,used) VALUES("+
				Num(Cur()->GetId())+",'"+Cur()->GetName()+"',"+Num(parent)+used;
			auto_ptr<RQuery> Ins1(Db->Query(sSql));

			// Assign the documents to the topic
			RCursor<GDoc> Docs(Cur()->GetDocs());
			for(Docs.Start();!Docs.End();Docs.Next())
				auto_ptr<RQuery>(Db->Query("INSERT INTO subjectsbyhtmls(subjectid,htmlid) VALUES("+Num(Cur()->GetId())+","+Num(Docs()->GetId())+")"));

			// AsSubPsign the profiles to the topic
			RCursor<GProfile> Profiles(Cur()->GetProfiles());
			for(Profiles.Start();!Profiles.End();Profiles.Next())
				auto_ptr<RQuery>(Db->Query("UPDATE profiles SET subjectid="+Num(Cur()->GetId())+" WHERE profileid="+Num(Profiles()->GetId())));
		}
	}
	catch(RDbException e)
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
			auto_ptr<RQuery> exec(Db->Query(l));
		}
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> create(Db->Query(sSql));
		sSql="CREATE TABLE IF NOT EXISTS sugsbygroups (groupid INT(11), htmlid INT(11), rank INT(11), test TEXT)";
		auto_ptr<RQuery> create2(Db->Query(sSql));
		sSql="DELETE FROM sugsbyprofiles";
		if(!name.IsEmpty())
			sSql+=" WHERE test="+RQuery::SQLValue(name);
		auto_ptr<RQuery> create3(Db->Query(sSql));
		sSql="DELETE FROM sugsbygroups";
		if(!name.IsEmpty())
			sSql+=" WHERE test="+RQuery::SQLValue(name);
		auto_ptr<RQuery> create4(Db->Query(sSql));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> profilestests(Db->Query(sSql));
		for(profilestests->Start();!profilestests->End();profilestests->Next())
			res.GetInsertPtr<RString>((*profilestests)[0]);
		sSql="SELECT DISTINCT(test) FROM sugsbygroups";
		auto_ptr<RQuery> groupstests(Db->Query(sSql));
		for(groupstests->Start();!groupstests->End();groupstests->Next())
			res.GetInsertPtr<RString>((*groupstests)[0]);
	}
	catch(RDbException e)
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
		bool Topic(false);

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
			case otTopic:
				Topic=true;
				What="topics";
				Desc=true;
				break;
			case otFdbk:
				What="htmlsbyprofiles";
				break;
			default:
				throw GException("Cannot clear "+GetObjType(objtype)+" from storage");
		}
		auto_ptr<RQuery> Delete(Db->Query("DELETE FROM "+What));
		if(Desc)
			auto_ptr<RQuery> DeleteInfos(Db->Query("DELETE FROM "+What+"byconcepts"));
		// If groups -> All profiles are detached
		if(Group)
			auto_ptr<RQuery> Update(Db->Query("UPDATE profiles SET groupid=0,attached=NULL"));
		// If topics -> All documents are detached
		if(Group)
			auto_ptr<RQuery> Update(Db->Query("UPDATE htmls SET topicid=0,attached=NULL"));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Types(Db->Query("SELECT typeid,name,description,refdocs,refprofiles,refgroups,reftopics FROM concepttypes"));
		for(Types->Start();!Types->End();Types->Next())
			Session->InsertConceptType(atoi((*Types)[0]),(*Types)[1],(*Types)[2],atoi((*Types)[3]),atoi((*Types)[4]),atoi((*Types)[5]),atoi((*Types)[6]));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> find(Db->Query(sSql));
		if(find->GetNbRows())
		{
			find->Start();
			type->SetId(strtoul((*find)[0],0,10));
			return;
		}

		// Insert the new concept type
		sSql="INSERT INTO concepttypes(name,description) VALUES("+name+","+desc+")";
		auto_ptr<RQuery> insert(Db->Query(sSql));

		// Get the next id
		sSql=RString("SELECT typeid FROM concepttypes WHERE typeid=LAST_INSERT_ID()");
		auto_ptr<RQuery> getinsert(Db->Query(sSql));
		getinsert->Start();
		type->SetId(strtoul((*getinsert)[0],0,10));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Types(Db->Query("SELECT typeid,name,description FROM relationtypes"));
		for(Types->Start();!Types->End();Types->Next())
			Session->InsertRelationType(atoi((*Types)[0]),(*Types)[1],(*Types)[2]);
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Rel(Db->Query("SELECT relationid,name,subjecttid,subjecttypeid,typeid,objectid,objecttypeid,weight FROM relations"));
		for(Rel->Start();!Rel->End();Rel->Next())
			Session->InsertRelation(atoi((*Rel)[0]),(*Rel)[1],atoi((*Rel)[2]),atoi((*Rel)[3]),atoi((*Rel)[4]),atoi((*Rel)[5]),atoi((*Rel)[8]),atof((*Rel)[9]));
	}
	catch(RDbException e)
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
			case otTopic:
				table="topic";
				break;
			default:
				throw GException("This type of objects do not have descriptions");
		};
		sSql="SELECT conceptid,typeid,weight FROM "+table+"sbyconcepts "
		     "WHERE "+table+"id="+Num(id)+" ORDER BY typeid,conceptid";
		GConceptType* type;
		size_t tid=0;
		auto_ptr<RQuery> sel(Db->Query(sSql));
		for(sel->Start(),i=0;!sel->End();sel->Next(),i++)
		{
			if(tid!=static_cast<size_t>(atol((*sel)[1])))
			{
				tid=atol((*sel)[1]);
				type=Session->GetConceptType(tid,false);
			}
			infos.InsertPtrAt(new GWeightInfo(type->GetConcept(atoi((*sel)[0])),atof((*sel)[2])),i,false);
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadConcepts(GConceptType* type)
{
	size_t MaxCount=100;
	size_t MaxId=0;
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
			auto_ptr<RQuery> count(Db->Query(sSql));
			count->Start();
			if(strtoul((*count)[0],0,10)>MaxCount) MaxCount=strtoul((*count)[0],0,10);
		}
		if(MaxCount==0) MaxCount=2000;
		sSql="SELECT MAX(conceptid) FROM concepts WHERE typeid="+Num(type->GetId());
		auto_ptr<RQuery> count(Db->Query(sSql));
		count->Start();
		RString c=(*count)[0];
		if(c.IsEmpty())
			MaxId=2000;
		else
			MaxId=atoi(c);

		// Create and insert the dictionary
		// Load the dictionary from the database
		sSql="SELECT conceptid,name,refdocs,refprofiles,refgroups,reftopics FROM concepts WHERE typeid="+Num(type->GetId());
		auto_ptr<RQuery> dicts(Db->Query(sSql));
		for(dicts->Start();!dicts->End();dicts->Next())
		{
			if(Index)
			{
				GXMLIndex w(atoi((*dicts)[0]),(*dicts)[1],type,atoi((*dicts)[2]),atoi((*dicts)[3]),atoi((*dicts)[4]),atoi((*dicts)[5]));
				type->InsertConcept(&w);
			}
			else
			{
				GConcept w(atoi((*dicts)[0]),(*dicts)[1],type,atoi((*dicts)[2]),atoi((*dicts)[3]),atoi((*dicts)[4]),atoi((*dicts)[5]));
				type->InsertConcept(&w);
			}
		}
		if(GSession::Get()&&GSession::Get()->GetSlot())
			GSession::Get()->GetSlot()->EndJob();
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> find(Db->Query(sSql));
		if(find->GetNbRows())
		{
			find->Start();
			concept->SetId(strtoul((*find)[0],0,10));
			return;
		}

		// Insert the new concept
		sSql="INSERT INTO concepts(name,conceptid,typeid) SELECT "+name+",IFNULL(MAX(conceptid),0)+1,"+type+
			 " FROM concepts WHERE typeid="+type;
		auto_ptr<RQuery> insert(Db->Query(sSql));

		// Get the next id
		sSql=RString("SELECT conceptid FROM concepts WHERE conceptautoid=LAST_INSERT_ID()");
		auto_ptr<RQuery> getinsert(Db->Query(sSql));
		getinsert->Start();
		concept->SetId(strtoul((*getinsert)[0],0,10));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Del(Db->Query("DELETE FROM concepts WHERE typeid="+
		              Num(concept->GetType()->GetId())+" AND conceptid="+Num(concept->GetId())));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
RString GStorageMySQL::LoadConcept(size_t id,GConceptType* type)
{
	try
	{
		RString res;
		RString sSql("SELECT name FROM concepts WHERE typeid="+Num(type->GetId())+" AND conceptid="+Num(id));
		auto_ptr<RQuery> w(Db->Query(sSql));
		if(w->GetNbRows())
		{
			w->Start();
			res=(*w)[0];
		}
		return(res);
	}
	catch(RDbException e)
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
		size_t res=0;
		RString sSql("SELECT conceptid FROM concepts WHERE typeid="+Num(type->GetId())+" AND kwd="+RQuery::SQLValue(name));
		auto_ptr<RQuery> w(Db->Query(sSql));
		if(w->GetNbRows())
		{
			w->Start();
			res=atoi((*w)[0]);
		}
		return(res);
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Delete(Db->Query("DELETE FROM concepts WHERE "
		                 "conceptid="+Num(concept->GetId())+" AND "
		                 "typeid="+Num(concept->GetType()->GetId())));

		// Insert the new word in the database
		Sql="INSERT INTO concepts(conceptid,name,typeid,refprofiles,refgroups,refdocs,reftopics) ";
		Sql+="VALUES("+Num(concept->GetId())+","+RQuery::SQLValue(concept->GetName())+","+
		     Num(concept->GetType()->GetId())+",0,0,0,0)";
		auto_ptr<RQuery> Insert(Db->Query(Sql));
	}
	catch(RDbException e)
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
			case otTopic:
				sSql="UPDATE concepts SET reftopics="+Num(refs)+
				     " WHERE conceptid="+Num(concept->GetId())+" AND typeid="+Num(concept->GetType()->GetId());
				break;
			default:
				throw GException("This type of objects do not have descriptions");
		};
		auto_ptr<RQuery>(Db->Query(sSql));
	}
	catch(RDbException e)
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
				auto_ptr<RQuery>(Db->Query("UPDATE concepttypes SET refdocs="+Num(refs)+" WHERE typeid="+Num(type->GetId())));
				if(refs==0)
					auto_ptr<RQuery>(Db->Query("UPDATE concepts SET refdocs=0 WHERE typeid="+Num(type->GetId())));
				break;
			}
			case otProfile:
			{
				auto_ptr<RQuery>(Db->Query("UPDATE concepttypes SET refprofiles="+Num(refs)+" WHERE typeid="+Num(type->GetId())));
				if(refs==0)
					auto_ptr<RQuery>(Db->Query("UPDATE concepts SET refgroups=0 WHERE typeid="+Num(type->GetId())));
				break;
			}
			case otCommunity:
			{
				auto_ptr<RQuery>(Db->Query("UPDATE concepttypes SET refgroups="+Num(refs)+" WHERE typeid="+Num(type->GetId())));
				if(refs==0)
					auto_ptr<RQuery>(Db->Query("UPDATE concepts SET refgroups=0 WHERE typeid="+Num(type->GetId())));
				break;
			}
			case otTopic:
			{
				auto_ptr<RQuery>(Db->Query("UPDATE concepttypes SET reftopics="+Num(refs)+" WHERE typeid="+Num(type->GetId())));
				if(refs==0)
					auto_ptr<RQuery>(Db->Query("UPDATE concepts SET reftopics=0 WHERE typeid="+Num(type->GetId())));
				break;
			}
			default:
				throw GException("This type of objects do not have descriptions");
		};
	}
	catch(RDbException e)
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
		// Reserved an identifier
		RString sSql="INSERT INTO htmls(html,title,langid) "
		             "VALUES("+RQuery::SQLValue(doc->GetURL())+","+RQuery::SQLValue(doc->GetName())+","+Lang(doc->GetLang())+")";
		auto_ptr<RQuery> Insert(Db->Query(sSql));

		// Get the id and assign it to the document
		sSql=RString("SELECT htmlid FROM htmls WHERE htmlid=LAST_INSERT_ID()");
		auto_ptr<RQuery> Get(Db->Query(sSql));
		Get->Start();
		doc->SetId(atoi((*Get)[0]));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadIndexer(GIndexer* &indexer,GLangManager* langs)
{
	size_t MaxCount=100;
	RString sSql;
	RString tbl;
	GWordOccurs* ptr;

	try
	{
		// Search the values to initialize the indexer
		for(char i='a';i<='z';i++)
		{
			sSql="SELECT COUNT(*) FROM concepts WHERE name LIKE '"+RString(i)+"%'";
			auto_ptr<RQuery> count(Db->Query(sSql));
			count->Start();
			if(strtoul((*count)[0],0,10)>MaxCount) MaxCount=strtoul((*count)[0],0,10);
		}
		if(MaxCount==0) MaxCount=2000;

		// Create and insert the dictionary
		indexer=new GIndexer(MaxCount,langs);

		// Load the stems from the database
		sSql="SELECT name FROM concepts WHERE typeid='1'";
		auto_ptr<RQuery> stems(Db->Query(sSql));
		for(stems->Start();!stems->End();stems->Next())
		{
			indexer->InsertWord((*stems)[0]);
		}

		// Index the documents
		auto_ptr<RQuery> sel(Db->Query("SELECT htmlid,name FROM htmlsbyconcepts,concepts "
		              "WHERE concepts.conceptid=htmlsbyconcepts.conceptid AND "
		              "concepts.typeid=htmlsbyconcepts.typeid"));
		for(sel->Start(),ptr=0;!sel->End();sel->Next())
		{
			// If not the same -> new wors
			if((!ptr)||(ptr->Compare((*sel)[1])!=0))
			{
				// If valid document -> assign the information to it
				ptr=indexer->GetWord((*sel)[1]);
			}
			ptr->AddDoc(atoi((*sel)[0]));
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GDoc* GStorageMySQL::LoadDoc(size_t docid)
{
	try
	{
		GDoc* doc;

		auto_ptr<RQuery> quer (Db->Query("SELECT htmlid,html,title,mimetype,langid,updated,calculated,owner,topicid,attached "
		                "FROM htmls WHERE htmlid="+Num(docid)));
		quer->Start();
		if(!quer->GetNbRows())
			return(0);

		// Verify that the langague is active
		GLang* lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn((*quer)[4],false);
		if((!lang)&&(!(*quer)[4].IsEmpty()))
			return(0);

		doc=new GDoc((*quer)[1],(*quer)[2],docid,lang,(*quer)[3],atoi((*quer)[8]),GetMySQLToDate((*quer)[6]),GetMySQLToDate((*quer)[5]),GetMySQLToDate((*quer)[9]),atoi((*quer)[7]));
		doc->SetState(osNeedLoad);

		// Load the links of the document loaded.
		auto_ptr<RQuery> querLinks (Db->Query("SELECT htmlid,linkid,occurs FROM htmlsbylinks WHERE htmlid="+Num(docid)));
		for(querLinks->Start();!querLinks->End();querLinks->Next())
			doc->InsertLink(Session->GetDoc(atoi((*querLinks)[1])), atoi((*querLinks)[2]));

		return(doc);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadNode(auto_ptr<RQuery>& nodes,auto_ptr<RQuery>& content,const GDoc* doc,GDocStruct* docstruct,GDocStructNode* parent,size_t nbnodes)
{
	try
	{
		// Vectors to manage the sub-nodes information
		RVectorInt<size_t,false> NbNodes(nbnodes);
		RVectorInt<size_t,false> NbContent(nbnodes);
		RContainer<GDocStructNode,false,false> Nodes(nbnodes);
		size_t i;
		GDocStructNode* ptr;

		// Load Nodes and create them
		for(i=nbnodes+1;--i;nodes->Next())
		{
			cout<<"Treat node "<<(*nodes)[8]<<" - Nodes: "<<(*nodes)[2]<<"("<<(*nodes)[3]<<") - Content: "<<(*nodes)[4]<<"("<<(*nodes)[5]<<")"<<endl;
			NbNodes.Insert(atoi((*nodes)[3]));
			NbContent.Insert(atoi((*nodes)[5]));

			// Create the node in the structure
			GConceptType* type(Session->GetConceptType(atoi((*nodes)[1]),false));
			GConcept* concept=type->GetConcept(atoi((*nodes)[0]));
			Nodes.InsertPtr(ptr=new GDocStructNode(doc->GetPtr(concept),atoi((*nodes)[6]),static_cast<GDocStructNode::NodeType>(atoi((*nodes)[7]))));
			docstruct->InsertNode(parent,ptr);
		}

		// Read their content and subnodes.
		RCursor<GDocStructNode> Cur(Nodes);
		for(i=nbnodes+1,NbNodes.Start(),NbContent.Start(),Cur.Start();--i;NbNodes.Next(),NbContent.Next(),Cur.Next())
		{
			// Read the content of the node
			for(size_t j=NbContent()+1;--j;content->Next())
			{
				GConceptType* type(Session->GetConceptType(atoi((*content)[1]),false));
				GConcept* concept=type->GetConcept(atoi((*content)[0]));
				docstruct->InsertContent(Cur(),doc->GetPtr(concept),atoi((*content)[2]));
			}

			// Read the sub-nodes
			if(NbNodes())
				LoadNode(nodes,content,doc,docstruct,Cur(),NbNodes());
		}
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Find(Db->Query("SELECT totalnodes,nodes,nbnodes,content,nbcontent FROM htmls WHERE htmlid="+Num(doc->GetId())));
		Find->Start();
		if(!Find->GetNbRows())
			return(0);
		GDocStruct* docstruct=new GDocStruct(strtoul((*Find)[0],0,10));

		// Create the queries for all nodes and contents
		size_t FirstNode(atoi((*Find)[1]));
		size_t OutNode(FirstNode+atoi((*Find)[0]));
		auto_ptr<RQuery> Nodes(Db->Query("SELECT conceptid,typeid,childs,nbchilds,contents,nbcontents,pos,what,nodeid FROM nodes "
				 "WHERE nodeid>="+(*Find)[1]+" AND nodeid<"+Num(OutNode)+" ORDER BY nodeid"));
		Nodes->Start();
		size_t FirstContent(atoi((*Find)[3]));
		size_t OutContent(FirstContent+atoi((*Find)[4]));
		auto_ptr<RQuery> Content(Db->Query("SELECT conceptid,typeid,pos FROM content "
				"WHERE contentid>="+(*Find)[3]+" AND contentid<"+Num(OutContent)+" ORDER BY contentid"));
		Content->Start();
		size_t NbSubNodes(atoi((*Find)[2]));

		LoadNode(Nodes,Content,doc,docstruct,docstruct->GetTop(),NbSubNodes);
		return(docstruct);
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> InsNode(Db->Query(Sql));

		// Look if the id of the node must be get
		if(!first)
		{
			auto_ptr<RQuery> GetId(Db->Query("SELECT nodeid FROM nodes WHERE nodeid=LAST_INSERT_ID()"));
			GetId->Start();
			first=strtoul((*GetId)[0],0,10);
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
			auto_ptr<RQuery> InsCont(Db->Query(Sql));

			if(!contents)
			{
				auto_ptr<RQuery> GetId(Db->Query("SELECT contentid FROM content WHERE contentid=LAST_INSERT_ID()"));
				GetId->Start();
				contents=strtoul((*GetId)[0],0,10);
			}

			if(!topcontent)
				topcontent=contents;
		}

		// Save their child nodes
		size_t childs=SaveNode(Nodes(),topcontent,nbcontent);

		// Update node
		auto_ptr<RQuery> Update(Db->Query("UPDATE nodes SET childs="+Num(childs)+",nbchilds="+Num(Nodes()->GetNbNodes())+
					",contents="+Num(contents)+",nbcontents="+Num(Content.GetNb())+" WHERE nodeid="+Num(id)));
	}

	return(first);
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveStruct(GDocStruct* docstruct,size_t docid)
{
	try
	{
		// Remove the old structure
		auto_ptr<RQuery> Info(Db->Query("SELECT nodes,totalnodes,content,nbcontent FROM htmls WHERE htmlid="+Num(docid)));
		Info->Start();
		if(Info->GetNbRows())
		{
			size_t LastId=atoi((*Info)[0])+atoi((*Info)[1]);
			auto_ptr<RQuery> DelNodes(Db->Query("DELETE FROM nodes WHERE nodeid>="+(*Info)[0]+" AND nodeid<"+Num(LastId)));
			LastId=atoi((*Info)[2])+atoi((*Info)[3]);
			auto_ptr<RQuery> DelContent(Db->Query("DELETE FROM content WHERE contentid>="+(*Info)[2]+" AND contentid<"+Num(LastId)));
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
		auto_ptr<RQuery> UpdateDoc(Db->Query(Sql));
	}
	catch(RDbException e)
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
	size_t docid;

	try
	{
		RString Sql("SELECT htmlid,html,title,mimetype,langid,updated,calculated,owner,topicid,attached FROM htmls");
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
		auto_ptr<RQuery> quer(Db->Query(Sql));
		for(quer->Start();!quer->End();quer->Next())
		{
			// Verify that the langague is active
			lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn((*quer)[4],false);
			if((!lang)&&(!(*quer)[4].IsEmpty()))
				continue;

			docid=atoi((*quer)[0]);
			doc=new GDoc((*quer)[1],(*quer)[2],docid,lang,(*quer)[3],atoi((*quer)[8]),GetMySQLToDate((*quer)[6]),GetMySQLToDate((*quer)[5]),GetMySQLToDate((*quer)[9]),atoi((*quer)[7]));
			Session->InsertDoc(doc);
			doc->SetState(osNeedLoad);

			// Load the links of the document loaded.
			auto_ptr<RQuery> querLinks (Db->Query("SELECT htmlid,linkid,occurs FROM htmlsbylinks "
			                     "WHERE htmlid="+Num(docid)));
			for(querLinks->Start(); !querLinks->End(); querLinks->Next())
				doc->InsertLink(Session->GetDoc(atoi((*querLinks)[1])), atoi((*querLinks)[2]));
		}
	}
	catch(RDbException e)
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
			auto_ptr<RQuery> deletekwds(Db->Query(sSql));
			Words=doc->GetInfos();
			for(Words.Start();!Words.End();Words.Next())
			{
				sSql="INSERT INTO htmlsbyconcepts(htmlid,conceptid,weight,typeid) "
				     "VALUES("+Num(doc->GetId())+","+Num(Words()->GetId())+","+Num(Words()->GetWeight())+","+
				     Num(Words()->GetType()->GetId())+")";
				auto_ptr<RQuery> insertkwds(Db->Query(sSql));
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
		auto_ptr<RQuery> Test(Db->Query(sSql));
		Test->Start();
		if(!atoi((*Test)[0]))
		{
			// Insert the document
			sSql="INSERT INTO htmls(htmlid,html,title,mimetype,langid,updated,calculated,topicid,attached) "
			     "VALUES("+Num(doc->GetId())+","+RQuery::SQLValue(doc->GetURL())+","+
			     RQuery::SQLValue(doc->GetName())+","+f+","+l+","+RQuery::SQLValue(doc->GetUpdated())+
			     ","+RQuery::SQLValue(doc->GetComputed())+","+Num(doc->GetGroupId())+","+RQuery::SQLValue(doc->GetAttached())+")";
			auto_ptr<RQuery> Insert(Db->Query(sSql));
		}
		else
		{
			// Update the document
			sSql="UPDATE htmls SET html="+RQuery::SQLValue(doc->GetURL())+",title="+
			     RQuery::SQLValue(doc->GetName())+",mimetype="+f+",langid="+l+
			     ",updated="+RQuery::SQLValue(doc->GetUpdated())+",calculated="+RQuery::SQLValue(doc->GetComputed())+
			     ",topicid="+Num(doc->GetGroupId())+",attached="+RQuery::SQLValue(doc->GetAttached())+
				 " WHERE htmlid="+Num(doc->GetId());
			auto_ptr<RQuery> Update(Db->Query(sSql));
		}

		// Update links to others documents
		auto_ptr<RQuery> deletelinks(Db->Query("DELETE FROM htmlsbylinks WHERE htmlid="+Num(doc->GetId())));
		lcur= doc->GetLinks();
		for ( lcur.Start(); ! lcur.End(); lcur.Next())
		{
			sSql="INSERT INTO htmlsbylinks(htmlid,linkid,occurs) VALUES("+Num(doc->GetId())+","+Num(lcur()->GetId())+","+Num(lcur()->GetOccurs())+")";
			auto_ptr<RQuery> insertkwds(Db->Query(sSql));
		}
	}
	catch(RDbException e)
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
			auto_ptr<RQuery> Users(Db->Query("SELECT userid,user,fullname FROM users"));
			for(Users->Start();!Users->End();Users->Next())
				Session->InsertUser(new GUser(atoi((*Users)[0]),(*Users)[1],(*Users)[2],10));

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
			auto_ptr<RQuery> Profiles(Db->Query(Sql));
			for(Profiles->Start();!Profiles->End();Profiles->Next())
			{
				GUser* user=Session->GetUser(atoi((*Profiles)[3]));
				size_t groupid=atoi((*Profiles)[5]);
				if(!groupid)
					groupid=cNoRef;
				Session->InsertProfile(prof=new GProfile(user,atoi((*Profiles)[0]),(*Profiles)[1],groupid,GetMySQLToDate((*Profiles)[4]),GetMySQLToDate((*Profiles)[6]),GetMySQLToDate((*Profiles)[7]),(atoi((*Profiles)[2])==1),5));
				prof->SetState(osNeedLoad);
			}

			// Load feedbacks
			auto_ptr<RQuery> fdbks(Db->Query("SELECT htmlid,judgement,profileid,when2,computed FROM htmlsbyprofiles"));
			for(fdbks->Start();!fdbks->End();fdbks->Next())
				Session->InsertFdbk(atoi((*fdbks)[2]),atoi((*fdbks)[0]),GetAssessmentType((*fdbks)[1]),GetMySQLToDate((*fdbks)[3]),GetMySQLToDate((*fdbks)[4]));
		}
	}
	catch(RDbException& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GUser* GStorageMySQL::LoadUser(size_t userid)
{
	try
	{
		auto_ptr<RQuery> User(Db->Query("SELECT userid,user,fullname FROM users WHERE userid="+Num(userid)));
		User->Start();
		if(!User->GetNbRows())
			return(0);
		return(new GUser(atoi((*User)[0]),(*User)[1],(*User)[2],10));
	}
	catch(RDbException& e)
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
		auto_ptr<RQuery> User(Db->Query("SELECT userid,user,fullname FROM users WHERE user="+RQuery::SQLValue(name)));
		User->Start();
		if(!User->GetNbRows())
			return(0);
		return(new GUser(atoi((*User)[0]),(*User)[1],(*User)[2],10));
	}
	catch(RDbException& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GProfile* GStorageMySQL::LoadProfile(size_t profileid)
{
	try
	{
		GLangManager* Langs=GALILEIApp->GetManager<GLangManager>("Lang");

		// Load Profile
		auto_ptr<RQuery> Profile(Db->Query("SELECT profileid,description,social,userid,attached,groupid,updated,calculated "
		                  "FROM profiles WHERE profileid="+Num(profileid)));
		Profile->Start();
		if(!Profile->GetNbRows())
			return(0);
		GUser* user=Session->GetUser(atoi((*Profile)[3]));
		if(!user)
			throw GException("Profile "+(*Profile)[0]+" has no parent user");
		size_t groupid=atoi((*Profile)[5]);
		if(!groupid)
			groupid=cNoRef;

		// Create the profile
		GProfile* prof=new GProfile(user,atoi((*Profile)[0]),(*Profile)[1],groupid,GetMySQLToDate((*Profile)[4]),GetMySQLToDate((*Profile)[6]),GetMySQLToDate((*Profile)[7]),(atoi((*Profile)[2])==1),5);
		prof->SetState(osNeedLoad);

		// Load Feedbacks
		auto_ptr<RQuery> fdbks(Db->Query("SELECT htmlid,judgement,profileid,when2,computed "
		                "FROM htmlsbyprofiles WHERE profileid="+Num(profileid)));
		for(fdbks->Start();!fdbks->End();fdbks->Next())
		{
			Session->InsertFdbk(atoi((*fdbks)[2]),atoi((*fdbks)[0]),GetAssessmentType((*fdbks)[1]),RDate((*fdbks)[3]),RDate((*fdbks)[4]));
			// Since the profile is not in the session -> we must manually insert the profile.
			GLang* lang=Langs->GetPlugIn((*fdbks)[5],false);
			if(!lang)
				continue;
			prof->InsertFdbk(atoi((*fdbks)[0]),GetAssessmentType((*fdbks)[1]),RDate((*fdbks)[3]),RDate((*fdbks)[4]));
		}
		return(prof);
	}
	catch(RDbException& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::GetSugsProfiles(const R::RString& name,R::RContainer<GSugs,true,false>& res)
{
	RString sSql;
	size_t profileid,idx;
	GSugs* sugs;

	try
	{
		res.Clear();
		sSql="SELECT profileid,htmlid,rank,test FROM sugsbyprofiles "
		     "WHERE test="+RQuery::SQLValue(name)+" ORDER BY profileid,rank";
		auto_ptr<RQuery> load(Db->Query(sSql));
		for(load->Start(),profileid=cNoRef;!load->End();load->Next())
		{
			idx=atoi((*load)[0]);

			// If not the same -> new profile
			if(idx!=profileid)
			{
				// If valid profile -> assign the information to it
				if(profileid!=cNoRef)
					res.InsertPtr(sugs);
				profileid=idx;
				sugs=new GSugs(otProfile,profileid,50);
			}
			sugs->AddSugs(atoi((*load)[1]));
		}
		if(profileid!=cNoRef)
			res.InsertPtr(sugs);

	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::UpdateProfiles(size_t docid)
{
	try
	{
		auto_ptr<RQuery> Up(Db->Query("UPDATE htmlsbyprofiles SET computed=CURDATE() WHERE htmlid="+Num(docid)));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Insert(Db->Query(sSql));

		// Get the id and assign it to the user
		sSql=RString("SELECT userid FROM users WHERE userid=LAST_INSERT_ID()");
		auto_ptr<RQuery> Get(Db->Query(sSql));
		Get->Start();
		user->SetId(atoi((*Get)[0]));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Insert(Db->Query(sSql));

		// Get the id and assign it to the profile
		sSql=RString("SELECT profileid FROM profiles WHERE profileid=LAST_INSERT_ID()");
		auto_ptr<RQuery> Get(Db->Query(sSql));
		Get->Start();
		p->SetId(atoi((*Get)[0]));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveUser(GUser* user)
{
	size_t userid;
	RString sSql;

	try
	{
		userid=user->GetId();

		// Test if the user already exists.
		sSql="SELECT COUNT(1) FROM users WHERE userid="+Num(userid);
		auto_ptr<RQuery> Test(Db->Query(sSql));
		Test->Start();
		if(!atoi((*Test)[0]))
		{
			// Insert the user
			sSql="INSERT INTO users(userid,user,fullname) "
			     "VALUES("+Num(userid)+","+RQuery::SQLValue(user->GetName())+","+RQuery::SQLValue(user->GetFullName())+")";
			auto_ptr<RQuery> Insert(Db->Query(sSql));
		}
		else
		{
			// Update the user
			sSql="UPDATE users SET user="+RQuery::SQLValue(user->GetName())+",fullname="+RQuery::SQLValue(user->GetFullName())+
			     " WHERE userid="+Num(userid);
			auto_ptr<RQuery> Update(Db->Query(sSql));
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveProfile(GProfile* prof)
{
	size_t profid;
	size_t social;
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
		auto_ptr<RQuery> Test(Db->Query(sSql));
		Test->Start();
		if(!atoi((*Test)[0]))
		{
			// Insert the profile (if subjects -> save topicid)
			sSql="INSERT INTO profiles(profileid,description,social,userid,updated,calculated,attached";
			if(Session->GetSubjects(false))
				sSql+=",topicid";
			sSql+=") VALUES("+Num(profid)+","+RQuery::SQLValue(prof->GetName())+","+
			      Num(social)+","+Num(prof->GetUser()->GetId())+","+
			      RQuery::SQLValue(prof->GetUpdated())+","+RQuery::SQLValue(prof->GetComputed())+","+RQuery::SQLValue(prof->GetAttached());
			if(Session->GetSubjects(false))
			{
				GSubject* sub=Session->GetSubjects(false)->GetSubject(prof);
				if(sub)
					sSql+=","+RString::Number(sub->GetId());
				else
					sSql+=",0";
			}
			sSql+=")";
			auto_ptr<RQuery> Insert(Db->Query(sSql));
		}
		else
		{
			// Update the profile (if subjects -> save topicid)
			sSql="UPDATE profiles SET description="+RQuery::SQLValue(prof->GetName())+",social="+Num(social)+
			     ",userid="+Num(prof->GetUser()->GetId())+",updated="+RQuery::SQLValue(prof->GetUpdated())+
			     ",calculated="+RQuery::SQLValue(prof->GetComputed())+",attached="+RQuery::SQLValue(prof->GetAttached());
			if(Session->GetSubjects(false))
			{
				GSubject* sub=Session->GetSubjects(false)->GetSubject(prof);
				if(sub)
					sSql+=",topicid="+Num(sub->GetId());
				else
					sSql+=",topicid=0";
			}
			sSql+=" WHERE profileid="+Num(profid);
			auto_ptr<RQuery> Update(Db->Query(sSql));
		}

		// First insert information from GFdbk
		RCursor<GFdbk> Fdbks=prof->GetFdbks();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Clear the feedback
			auto_ptr<RQuery> Delete(Db->Query("DELETE FROM htmlsbyprofiles WHERE profileid="+Num(profid)+" AND htmlid="+Num(Fdbks()->GetDocId())));

			// Re-Insert all the feedback
			sSql="INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2) "
			     "VALUES("+Num(Fdbks()->GetDocId())+",'"+GetAssessmentCode(Fdbks()->GetFdbk())+"',"+
			     Num(prof->GetId())+","+RQuery::SQLValue(Fdbks()->GetWhen())+")";
			auto_ptr<RQuery> Insert(Db->Query(sSql));
		}

		// Update other information from the documents
		auto_ptr<RQuery> Update(Db->Query("UPDATE htmls,htmlsbyprofiles SET htmlsbyprofiles.langid=htmls.langid,htmlsbyprofiles.computed=htmls.calculated "
		                 "WHERE htmlsbyprofiles.htmlid=htmls.htmlid AND profileid="+Num(prof->GetId())));

		// Delete old description
		sSql="DELETE FROM profilesbyconcepts "
		     "WHERE profileid="+Num(prof->GetId());
		auto_ptr<RQuery> deletekwds(Db->Query(sSql));

		// Insert new description
		RCursor<GWeightInfo> Cur(prof->GetInfos());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			sSql="INSERT INTO profilesbyconcepts(profileid,conceptid,weight,typeid) "
			     "VALUES("+Num(prof->GetId())+","+Num(Cur()->GetId())+","+Num(Cur()->GetWeight())+
			     ","+Num(Cur()->GetType()->GetId())+")";
			auto_ptr<RQuery> insertkwds(Db->Query(sSql));
		}
		prof->SetState(osUpToDate);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveProfileInHistory(GProfile* sub,size_t historicID)
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
			auto_ptr<RQuery> insertkwds(Db->Query(sSql));
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveHistoricProfiles(size_t /*historicID*/)
{
	try
	{
		RToImplement();
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddSugsProfile(const R::RString& name,size_t profileid,size_t docid,size_t rank)
{
	RString sSql;

	try
	{
		sSql="INSERT INTO sugsbyprofiles(profileid,htmlid,rank,test) "
		     "VALUES("+Num(profileid)+","+Num(docid)+","+Num(rank)+","+RQuery::SQLValue(name)+")";
		auto_ptr<RQuery> create(Db->Query(sSql));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddFdbk(size_t p,size_t d,tDocAssessment assess,R::RDate date,R::RDate computed)
{
	try
	{
		RString sSql;
		sSql="INSERT INTO htmlsbyprofiles(htmlid,judgement,profileid,when2,computed) "
		     "VALUES("+Num(d)+",'"+GetAssessmentCode(assess)+"',"+Num(p)+","+RQuery::SQLValue(date)+
		     ","+RQuery::SQLValue(computed)+")";
		auto_ptr<RQuery> Insert(Db->Query(sSql));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}



//------------------------------------------------------------------------------
//
// Communities Methods
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
		auto_ptr<RQuery> Groups(Db->Query(Sql));
		for(Groups->Start();!Groups->End();Groups->Next())
		{
			group=new GCommunity(atoi((*Groups)[0]),(*Groups)[1],(*Groups)[2]);
			group->SetState(osNeedLoad);
			Session->InsertCommunity(group);
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GCommunity* GStorageMySQL::LoadCommunity(size_t groupid)
{
	try
	{
		auto_ptr<RQuery> Group(Db->Query("SELECT groupid,updated,calculated FROM groups WHERE groupid="+RString::Number(groupid)));
		Group->Start();
		if(!Group->GetNbRows())
			return(0);
		GCommunity* group=new GCommunity(atoi((*Group)[0]),(*Group)[1],(*Group)[2]);
		group->SetState(osNeedLoad);

		return(group);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GCommunitiesHistory* GStorageMySQL::LoadHistoricCommunities(size_t historicID)
{
	RString sSql;
	GCommunityHistory* grp;
	GCommunitiesHistory* grps;
	GWeightInfosHistory* historicsubprof;
	size_t subprofid;
	size_t groupid;
	size_t v;

	try
	{
		// Init part
		groupid=cNoRef;

		sSql="SELECT date FROM historicgroups WHERE historicID="+Num(historicID);
		auto_ptr<RQuery> date(Db->Query(sSql));
		date->Start();
		grps=new GCommunitiesHistory(historicID, (*date)[0]);

		// Read the groupment.
		sSql="SELECT groupid,profileid,date FROM historicgroups "
		     "WHERE historicID="+Num(historicID)+" ORDER by historicID,groupid";
		auto_ptr<RQuery> g(Db->Query(sSql));
		for(g->Start(),grp=0;!g->End();g->Next())
		{
			// Read Group
			v=atoi((*g)[0]);

			// If group id changed -> new group needed
			if((v!=groupid))
			{
				groupid=v;
				grp=new GCommunityHistory(groupid,grps);
				//insert group in the container of groups.
				grps->InsertPtr(grp);
			}

			// Create the historic profile and add it to the group
			subprofid=atoi((*g)[1]);
			historicsubprof=new GWeightInfosHistory(Session->GetProfile(0,subprofid),100);
			grp->AddProfile(historicsubprof);
			historicsubprof->SetParent(grp);

			// Fill the vector of the subprofile


			sSql="SELECT conceptid,typeid,weight FROM historicprofiles "
			     "WHERE profileid="+Num(subprofid)+" AND historicID="+Num(historicID)+" ORDER BY typeid,conceptid";
			GConceptType* type;
			size_t tid=0,i;
			auto_ptr<RQuery> sel(Db->Query(sSql));
			for(sel->Start(),i=0;!sel->End();sel->Next(),i++)
			{
				if(tid!=static_cast<size_t>(atol((*sel)[1])))
				{
					tid=atol((*sel)[1]);
					type=Session->GetConceptType(tid,false);
				}
				historicsubprof->InsertPtrAt(new GWeightInfo(type->GetConcept(atoi((*sel)[0])),atof((*sel)[2])),i,false);
			}
		}
		return(grps);
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> ids(Db->Query(sSql));
		for(ids->Start();!ids->End();ids->Next())
			Session->GetCommunitiesHistoryManager()->InsertCommunitiesHistory(LoadHistoricCommunities(atoi((*ids)[0])));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::GetSugsCommunities(const R::RString& name,R::RContainer<GSugs,true,false>& res)
{
	RString sSql;
	size_t groupid,idx;
	GSugs* sugs;

	try
	{
		res.Clear();
		sSql="SELECT groupid,htmlid,rank,test "
		     "FROM sugsbygroups ORDER BY groupid,rank WHERE test="+RQuery::SQLValue(name);
		auto_ptr<RQuery> load(Db->Query(sSql));
		for(load->Start(),groupid=cNoRef;!load->End();load->Next())
		{
			idx=atoi((*load)[0]);

			// If not the same -> new group
			if(idx!=groupid)
			{
				// If valid group -> assign the information to it
				if(groupid!=cNoRef)
					res.InsertPtr(sugs);
				groupid=idx;
				sugs=new GSugs(otCommunity,groupid,50);
			}
			sugs->AddSugs(atoi((*load)[1]));
		}
		if(groupid!=cNoRef)
			res.InsertPtr(sugs);

	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
size_t GStorageMySQL::GetHistorySize(void)
{
	try
	{
		auto_ptr<RQuery> size(Db->Query("SELECT COUNT(DISTINCT historicID) from historicgroups"));
		size->Start();
		return(atoi((*size)[0]));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::UpdateCommunities(size_t subid)
{
	try
	{
		auto_ptr<RQuery> Up(Db->Query("UPDATE groups,profiles SET groups.updated=CURDATE() "
	    	         "WHERE groups.groupid=profiles.groupid AND "
	        	     "profiles.profileid="+Num(subid)));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> Insert(Db->Query(sSql));

		// Get the id and assign it to the group
		sSql=RString("SELECT groupid FROM groups WHERE groupautoid=LAST_INSERT_ID()");
		auto_ptr<RQuery> Get(Db->Query(sSql));
		Get->Start();
		grp->SetId(atoi((*Get)[0]));
	}
	catch(RDbException e)
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
		auto_ptr<RQuery> delete1(Db->Query("DELETE FROM groupsbyconcepts;"));
		auto_ptr<RQuery> delete2(Db->Query("DELETE FROM groups"));


		GroupsCursor=Session->GetCommunities();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			sSql="INSERT INTO groups(groupid,updated,calculated) "
			     "VALUES("+Num(GroupsCursor()->GetId())+","+
			     RQuery::SQLValue(GroupsCursor()->GetUpdated())+","+
			     RQuery::SQLValue(GroupsCursor()->GetComputed())+")";
			auto_ptr<RQuery> insert1(Db->Query(sSql));

			// Save Profiles infos
			Sub=GroupsCursor()->GetObjs();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sSql="UPDATE profiles SET groupid="+Num(GroupsCursor()->GetId())+","
				     "attached="+RQuery::SQLValue(Sub()->GetAttached())+" "
				     "WHERE profileid="+Num(Sub()->GetId());
				auto_ptr<RQuery> update(Db->Query(sSql));
			}

			// Save the description part
			WordCur.Set(*GroupsCursor());
			for(WordCur.Start();!WordCur.End();WordCur.Next())
			{
				sSql="INSERT INTO groupsbyconcepts(groupid,conceptid,weight,typeid) "
				     "VALUES("+Num(GroupsCursor()->GetId())+","+Num(WordCur()->GetId())+","+
				     Num(WordCur()->GetWeight())+","+Num(WordCur()->GetType()->GetId())+")";
				auto_ptr<RQuery> InserinfoWord(Db->Query(sSql));
			}
		}
	}
	catch(RDbException e)
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
	size_t historicID;
	RString sSql;

	try
	{
		// If historic needed, gget the last historic id
		historicID=0;
		auto_ptr<RQuery> histcount(Db->Query("SELECT MAX(historicID) from historicgroups"));
		histcount->Start();
		if((*histcount)[0])
			historicID=atoi((*histcount)[0]);
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
				auto_ptr<RQuery> history(Db->Query(sSql));
			}
		}

		// Save the profiles history if needed.
		SaveHistoricProfiles(historicID);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddSugsCommunity(const R::RString& name,size_t groupid,size_t docid,size_t rank)
{
	RString sSql;

	try
	{
		sSql="INSERT INTO sugsbygroups(groupid,htmlid,rank,test) "
		     "VALUES("+Num(groupid)+","+Num(docid)+","+Num(rank)+","+RQuery::SQLValue(name)+")";
		auto_ptr<RQuery> create(Db->Query(sSql));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}



//------------------------------------------------------------------------------
//
// Topics Methods
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::LoadTopics(void)
{
	GTopic* group;
	R::RCursor<GTopic> GroupsCursor;

	try
	{
		RString Sql("SELECT topicid,updated,calculated FROM topics");
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
		auto_ptr<RQuery> Groups(Db->Query(Sql));
		for(Groups->Start();!Groups->End();Groups->Next())
		{
			group=new GTopic(atoi((*Groups)[0]),(*Groups)[1],(*Groups)[2]);
			group->SetState(osNeedLoad);
			Session->InsertTopic(group);
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
GTopic* GStorageMySQL::LoadTopic(size_t topicid)
{
	try
	{
		auto_ptr<RQuery> Group(Db->Query("SELECT topicid,updated,calculated FROM topics WHERE topicid="+RString::Number(topicid)));
		Group->Start();
		if(!Group->GetNbRows())
			return(0);
		GTopic* group=new GTopic(atoi((*Group)[0]),(*Group)[1],(*Group)[2]);
		group->SetState(osNeedLoad);

		return(group);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::UpdateTopics(size_t subid)
{
	try
	{
		auto_ptr<RQuery> Up(Db->Query("UPDATE topics,htmls SET topics.updated=CURDATE() "
	    	         "WHERE topics.topicid=htmls.topicid AND "
	        	     "htmls.htmlid="+Num(subid)));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GTopic* grp)
{
	try
	{
		// Reserved an identificator
		RString sSql="INSERT INTO topics(topicid) "
		             "SELECT IFNULL(MAX(topicid),0)+1 "
		             "FROM topics";
		auto_ptr<RQuery> Insert(Db->Query(sSql));

		// Get the id and assign it to the group
		sSql=RString("SELECT topicid FROM topics WHERE topicautoid=LAST_INSERT_ID()");
		auto_ptr<RQuery> Get(Db->Query(sSql));
		Get->Start();
		grp->SetId(atoi((*Get)[0]));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveTopics(void)
{
	RCursor<GWeightInfo> WordCur;
	R::RCursor<GTopic> GroupsCursor;
	RString sSql;
	RCursor<GDoc> Sub;

	try
	{
		// Delete topics and topics info
		auto_ptr<RQuery> delete1(Db->Query("DELETE FROM topicsbyconcepts;"));
		auto_ptr<RQuery> delete2(Db->Query("DELETE FROM topics"));


		GroupsCursor=Session->GetTopics();
		for(GroupsCursor.Start();!GroupsCursor.End();GroupsCursor.Next())
		{
			sSql="INSERT INTO topics(topicid,updated,calculated) "
			     "VALUES("+Num(GroupsCursor()->GetId())+","+
			     RQuery::SQLValue(GroupsCursor()->GetUpdated())+","+
			     RQuery::SQLValue(GroupsCursor()->GetComputed())+")";
			auto_ptr<RQuery> insert1(Db->Query(sSql));

			// Save documents infos
			Sub=GroupsCursor()->GetObjs();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				sSql="UPDATE htmls SET topicid="+Num(GroupsCursor()->GetId())+","
				     "attached="+RQuery::SQLValue(Sub()->GetAttached())+" "
				     "WHERE htmlid="+Num(Sub()->GetId());
				auto_ptr<RQuery> update(Db->Query(sSql));
			}

			// Save the description part
			WordCur.Set(*GroupsCursor());
			for(WordCur.Start();!WordCur.End();WordCur.Next())
			{
				sSql="INSERT INTO topicsbyconcepts(topicid,conceptid,weight,typeid) "
				     "VALUES("+Num(GroupsCursor()->GetId())+","+Num(WordCur()->GetId())+","+
				     Num(WordCur()->GetWeight())+","+Num(WordCur()->GetType()->GetId())+")";
				auto_ptr<RQuery> InserinfoWord(Db->Query(sSql));
			}
		}
	}
	catch(RDbException e)
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
