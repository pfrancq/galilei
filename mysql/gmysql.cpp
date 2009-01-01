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
#include <math.h>
#include <memory>


//------------------------------------------------------------------------------
// include files for R
#include <rvectorint.h>
#include <rdir.h>
#include <rrecfile.h>


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
#include <gindexer.h>
#include <gwordoccurs.h>
#include <ggalileiapp.h>
#include <gconcept.h>
#include <gxmlindex.h>
#include <gconcepttype.h>
#include <grelation.h>
#include <grelationtype.h>
#include <gdocstruct.h>



//------------------------------------------------------------------------------
//
//  Records to store description and structure files
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Record representing an occurrence of concept of a given type with a given weight
class DescRec
{
public:
	char TypeId;       // Identifier of the type.
	size_t ConceptId;  // Identifier of the concept.
	double Weight;     // Weight of the concepts.

	DescRec(void) : TypeId(0), ConceptId(0), Weight(NAN) {}
	void Set(char tid,size_t cid,double w) {TypeId=tid;ConceptId=cid;Weight=w;}
	void Read(RRecFile<DescRec,false>& f)  {f>>TypeId>>ConceptId>>Weight;}
	void Write(RRecFile<DescRec,false>& f) {f<<TypeId<<ConceptId<<Weight;}
};



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
	double d2(0.0);
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
	Dir=Factory->Get("Dir");
	try
	{
		RDir::CreateDirIfNecessary(Dir+RFile::GetDirSeparator()+Database,true);
		DirDocs=Dir+RFile::GetDirSeparator()+Database+RFile::GetDirSeparator()+"Documents"+RFile::GetDirSeparator();
		RDir::CreateDirIfNecessary(DirDocs,false);
		DirProfiles=Dir+RFile::GetDirSeparator()+Database+RFile::GetDirSeparator()+"Profiles"+RFile::GetDirSeparator();
		RDir::CreateDirIfNecessary(DirProfiles,false);
		DirCommunities=Dir+RFile::GetDirSeparator()+Database+RFile::GetDirSeparator()+"Communities"+RFile::GetDirSeparator();
		RDir::CreateDirIfNecessary(DirCommunities,false);
		DirTopics=Dir+RFile::GetDirSeparator()+Database+RFile::GetDirSeparator()+"Topics"+RFile::GetDirSeparator();
		RDir::CreateDirIfNecessary(DirTopics,false);
	}
	catch(...)
	{
		cerr<<"GStorageMySQL::ApplyConfig: Problems in creating directories in '"<<Dir<<"'"<<endl;
	}
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

			case otTopic:
				return(GetCount("topics"));

			case otFdbk:
				return(GetCount("htmlsbyprofiles"));

			default:
				throw GException("GStorageMySQL::GetNbSaved: '"+GetObjType(type)+"' not supported");
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
			RCursor<GDoc> Docs(Cur()->GetObjs(static_cast<GDoc*>(0)));
			for(Docs.Start();!Docs.End();Docs.Next())
				auto_ptr<RQuery>(Db->Query("INSERT INTO subjectsbyhtmls(subjectid,htmlid) VALUES("+Num(Cur()->GetId())+","+Num(Docs()->GetId())+")"));

			// AsSubPsign the profiles to the topic
			RCursor<GProfile> Profiles(Cur()->GetObjs(static_cast<GProfile*>(0)));
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
		bool Group(false);
		bool Topic(false);
		bool Doc(false);

		switch(objtype)
		{
			case otUser:
				What="users";
				break;
			case otDoc:
			{
				What="htmls";
				RDir DelDir(DirDocs);
				DelDir.Clear();
				Doc=true;
				break;
			}
			case otProfile:
			{
				What="profiles";
				RDir DelDir(DirProfiles);
				DelDir.Clear();
				break;
			}
			case otCommunity:
			{
				Group=true;
				What="groups";
				RDir DelDir(DirCommunities);
				DelDir.Clear();
				break;
			}
			case otTopic:
			{
				Topic=true;
				What="topics";
				RDir DelDir(DirTopics);
				DelDir.Clear();
				break;
			}
			case otFdbk:
				What="htmlsbyprofiles";
				break;
			default:
				throw GException("Cannot clear "+GetObjType(objtype)+" from storage");
		}
		auto_ptr<RQuery> Delete(Db->Query("DELETE FROM "+What));

		// If groups -> All profiles are detached
		if(Group)
			auto_ptr<RQuery> Update(Db->Query("UPDATE profiles SET groupid=0,attached=NULL"));

		// If topics -> All documents are detached
		if(Topic)
			auto_ptr<RQuery> Update(Db->Query("UPDATE htmls SET topicid=0,attached=NULL"));

		if(Doc)
		{
			auto_ptr<RQuery> Delete1(Db->Query("DELETE FROM htmlsbygroups"));
			auto_ptr<RQuery> Delete2(Db->Query("DELETE FROM htmlsbylinks"));
			auto_ptr<RQuery> Delete3(Db->Query("DELETE FROM htmlsbyprofiles"));
			auto_ptr<RQuery> Delete4(Db->Query("DELETE FROM subjectsbyhtmls"));
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadInfos(GWeightInfos& infos,tObjType type,size_t id)
{
	try
	{
		// Find the file to open
		RString File;
		switch(type)
		{
			case otDoc:
				File=DirDocs;
				break;
			case otProfile:
				File=DirProfiles;
				break;
			case otCommunity:
				File=DirCommunities;
				break;
			case otTopic:
				File=DirTopics;
				break;
			default:
				throw GException("This type of objects do not have descriptions");
		};
		File+=RString::Number(id)+".desc";
		if(!RFile::Exists(File)) return;

		// Read the file
		DescRec Rec;
		GConceptType* type(0);
		char tid=0;
		size_t i;
		RRecFile<DescRec,false> Obj(File,sizeof(DescRec));
		Obj.Open(RIO::Read);
		for(i=0;!Obj.End();i++)
		{
			Obj.ReadRec(Rec);
			if(tid!=Rec.TypeId)
			{
				tid=Rec.TypeId;
				type=Session->GetConceptType(tid,false);
			}
			infos.InsertPtrAt(new GWeightInfo(type->GetConcept(Rec.ConceptId),Rec.Weight),i,false);
		}
	}
	catch(RIOException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveInfos(GWeightInfos& infos,tObjType type,size_t id)
{
	try
	{
		// Find the file to open
		RString File;
		switch(type)
		{
			case otDoc:
				File=DirDocs;
				break;
			case otProfile:
				File=DirProfiles;
				break;
			case otCommunity:
				File=DirCommunities;
				break;
			case otTopic:
				File=DirTopics;
				break;
			default:
				throw GException("This type of objects do not have descriptions");
		};
		File+=RString::Number(id)+".desc";

		// Create the file
		DescRec Rec;
		RRecFile<DescRec,false> Obj(File,sizeof(DescRec));
		Obj.Open(RIO::Create);
		RCursor<GWeightInfo> Words(infos);
		for(Words.Start();!Words.End();Words.Next())
		{
			Rec.Set(Words()->GetType()->GetId(),Words()->GetId(),Words()->GetWeight());
			Obj.WriteRec(Rec);
		}
	}
	catch(RIOException e)
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
			Session->InsertConceptType(static_cast<char>(atoi((*Types)[0])),(*Types)[1],(*Types)[2],atoi((*Types)[3]),atoi((*Types)[4]),atoi((*Types)[5]),atoi((*Types)[6]));
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
		find->Start();
		if(!find->End())
		{
			type->SetId(static_cast<char>(strtoul((*find)[0],0,10)));
			return;
		}

		// Insert the new concept type
		sSql="INSERT INTO concepttypes(name,description) VALUES("+name+","+desc+")";
		auto_ptr<RQuery> insert(Db->Query(sSql));

		// Get the next id
		sSql=RString("SELECT typeid FROM concepttypes WHERE typeid=LAST_INSERT_ID()");
		auto_ptr<RQuery> getinsert(Db->Query(sSql));
		getinsert->Start();
		type->SetId(static_cast<char>(strtoul((*getinsert)[0],0,10)));
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
			Session->InsertRelation(atoi((*Rel)[0]),(*Rel)[1],atoi((*Rel)[2]),static_cast<char>(atoi((*Rel)[3])),atoi((*Rel)[4]),atoi((*Rel)[5]),static_cast<char>(atoi((*Rel)[8])),atof((*Rel)[9]));
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
		find->Start();
		if(!find->End())
		{
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
		w->Start();
		if(!w->End())
			res=(*w)[0];
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
		w->Start();
		if(!w->End())
			res=atoi((*w)[0]);
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
		               Num(concept->GetType()->GetId())+","+
		               Num(concept->GetRef(otProfile))+","+
		               Num(concept->GetRef(otCommunity))+","+
		               Num(concept->GetRef(otDoc))+","+
		               Num(concept->GetRef(otTopic))+")";
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
		             "VALUES("+RQuery::SQLValue(doc->GetURL()())+","+RQuery::SQLValue(doc->GetName())+","+Lang(doc->GetLang())+")";
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

	throw GException("GStorageMySQL::LoadIndexer is not implemented correctly");
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

		auto_ptr<RQuery> quer (Db->Query("SELECT htmlid,html,title,mimetype,langid,updated,calculated,owner,topicid,attached,nbnodes,nbcontent "
		                "FROM htmls WHERE htmlid="+Num(docid)));
		quer->Start();
		if(quer->End())
			return(0);

		// Verify that the langague is active
		GLang* lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn((*quer)[4],false);
		if((!lang)&&(!(*quer)[4].IsEmpty()))
			return(0);

		doc=new GDoc((*quer)[1],(*quer)[2],docid,lang,(*quer)[3],atoi((*quer)[8]),GetMySQLToDate((*quer)[6]),GetMySQLToDate((*quer)[5]),GetMySQLToDate((*quer)[9]),
				atoi((*quer)[10]),atoi((*quer)[11]),atoi((*quer)[7]));
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
void GStorageMySQL::LoadStruct(GDocStruct& docstruct,GDoc* doc)
{
	try
	{
		// Read the file
		DescRec Rec;
		GConceptType* ctype;
		size_t tId,pos,nbchild;
		char tTypeId(0),type_id;
		char depth;
		char type;

		RBinaryFile Obj(DirDocs+RString::Number(doc->GetId())+".struct");
		Obj.Open(RIO::Read);

		// Load the size of the LCs
		size_t max=doc->GetNbLCs();
		for(size_t i=0;i<max;i++)
		{
			Obj>>tId;
			docstruct.SetNbLCEntries(i,tId);
		}

		// Load the records
		while(!Obj.End())
		{

			Obj>>tId>>type_id>>type>>pos>>depth>>nbchild;
			if(type_id!=tTypeId)
			{
				tTypeId=type_id;
				ctype=Session->GetConceptType(type_id,false);
			}
			docstruct.AddRecord(ctype->GetConcept(tId),static_cast<GVTDRec::RecType>(type),pos,depth,nbchild);
		}
	}
	catch(RIOException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}

}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveStruct(GDocStruct& docstruct,GDoc* doc)
{
	try
	{
	    if(!docstruct.GetNbRecs())
			return;
		// Create the file
		RBinaryFile Obj(DirDocs+RString::Number(doc->GetId())+".struct");
		Obj.Open(RIO::Create);

		// Save the size of the LCs
		size_t max=docstruct.GetNbLCs();
		for(size_t i=0;i<max;i++)
			Obj<<docstruct.GetNbLCEntries(i);

		// Save the records
		R::RCursor<GVTDRec> Recs(docstruct.GetRecs());
		for(Recs.Start();!Recs.End();Recs.Next())
			Obj<<Recs()->GetConcept()->GetId()<<Recs()->GetConcept()->GetType()->GetId()<<static_cast<char>(Recs()->GetType())<<Recs()->GetPos()<<Recs()->GetDepth()<<docstruct.GetFirstChild(Recs());
	}
	catch(RIOException e)
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
		RString Sql("SELECT htmlid,html,title,mimetype,langid,updated,calculated,owner,topicid,attached,nbnodes,nbcontent FROM htmls");
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
			// Verify if its language is active
			lang=GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIn((*quer)[4],false);
			if((!lang)&&(!(*quer)[4].IsEmpty()))
				continue;

			docid=atoi((*quer)[0]);
			doc=new GDoc((*quer)[1],(*quer)[2],docid,lang,(*quer)[3],atoi((*quer)[8]),GetMySQLToDate((*quer)[6]),GetMySQLToDate((*quer)[5]),
					     GetMySQLToDate((*quer)[9]),atoi((*quer)[10]),atoi((*quer)[11]),atoi((*quer)[7]));
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
		if(doc->GetLang())
			l=Lang(doc->GetLang());
		else
			l="NULL";

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
			sSql="INSERT INTO htmls(htmlid,html,title,mimetype,langid,updated,calculated,topicid,attached,nbnodes,nbcontent) "
			     "VALUES("+Num(doc->GetId())+","+RQuery::SQLValue(doc->GetURL()())+","+
			     RQuery::SQLValue(doc->GetName())+","+f+","+l+","+RQuery::SQLValue(doc->GetUpdated())+
			     ","+RQuery::SQLValue(doc->GetComputed())+","+Num(doc->GetGroupId())+","+RQuery::SQLValue(doc->GetAttached())+
			     ","+Num(doc->GetNbRecs())+","+Num(doc->GetNbLCs())+")";
			auto_ptr<RQuery> Insert(Db->Query(sSql));
		}
		else
		{
			// Update the document
			sSql="UPDATE htmls SET html="+RQuery::SQLValue(doc->GetURL()())+",title="+
			     RQuery::SQLValue(doc->GetName())+",mimetype="+f+",langid="+l+
			     ",updated="+RQuery::SQLValue(doc->GetUpdated())+",calculated="+RQuery::SQLValue(doc->GetComputed())+
			     ",topicid="+Num(doc->GetGroupId())+",attached="+RQuery::SQLValue(doc->GetAttached())+
			     ",nbnodes="+Num(doc->GetNbRecs())+",nbcontent="+Num(doc->GetNbLCs())+
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
		if(User->End())
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
		if(User->End())
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
		if(Profile->End())
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
				sSql+=",subjectid";
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
					sSql+=",subjectid="+Num(sub->GetId());
				else
					sSql+=",subjectid=0";
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

		SaveInfos(*prof,otProfile,prof->GetId());
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
		if(Group->End())
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
			SaveInfos(*GroupsCursor(),otCommunity,GroupsCursor()->GetId());
		}
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
		if(Group->End())
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
			SaveInfos(*GroupsCursor(),otTopic,GroupsCursor()->GetId());
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
	params->InsertParam(new RParamValue("Dir","/var/galilei"));
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
