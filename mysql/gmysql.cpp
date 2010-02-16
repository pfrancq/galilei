/*

	GALILEI Research Project

	GMySQL.cpp

	Storage Manager using a MySQL Database - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by Julien Lamoral.
	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for GALILEI
#include <glang.h>
#include <glink.h>
#include <gsuggestion.h>
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
#include <ggalileiapp.h>
#include <gconcept.h>
#include <gxmlindex.h>
#include <gconcepttype.h>
#include <gstatement.h>
#include <gpredicate.h>
#include <gclass.h>
#include <gfdbk.h>



//------------------------------------------------------------------------------
//
//  GStorageMySQL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStorageMySQL::GStorageMySQL(GPlugInFactory* fac)
	: GStorage(fac), Db(0)
{
}


//------------------------------------------------------------------------------
size_t GStorageMySQL::GetCount(RString tbl)
{
	RString c;
	RString sSql("SELECT COUNT(*) FROM "+tbl);

	RQuery Count(Db,sSql);
	Count.Start();
	c=Count[0];
	if(c.IsEmpty()) return(0);
	return(atoi(c));
}


//------------------------------------------------------------------------------
size_t GStorageMySQL::GetMax(RString tbl,RString fld)
{
	RString sSql("SELECT MAX("+fld+") FROM "+tbl);
	RString c;

	RQuery Count(Db,sSql);
	Count.Start();
	c=Count[0];
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
		Db=new RDbMySQL(Database,Host,User,Password,Encoding);
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
	Encoding=Factory->Get("Encoding").Latin1();
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
				return(GetCount("docs"));

			case otUser:
				return(GetCount("users"));

			case otProfile:
				return(GetCount("profiles"));

			case otCommunity:
				return(GetCount("communities"));

			case otTopic:
				return(GetCount("topics"));

			case otFdbk:
				return(GetCount("docsbyprofiles"));

			case otConcept:
				return(GetCount("concepts"));

			default:
				ThrowGException(GetObjType(type,true,true)+" are not supported");
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
	return(Db);
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadSubjects(void)
{
	try
	{
		if(Session->GetSlot())
			Session->GetSlot()->StartJob("Load subjects");

		// Verify the subjects exist and clear them
		Session->Clear(GetCount("subjects"),GetCount("docs"),GetCount("profiles"));

		// Load the subjects
		RQuery sub(Db,"SELECT subjectid,name,used,parent FROM subjects");
		for(sub.Start();!sub.End();sub.Next())
		{
			size_t ParentId(sub[3].ToSizeT());
			GSubject* Parent(0);
			if(ParentId)
				Parent=Session->GetSubject(ParentId);
			Session->InsertSubject(Parent,new GSubject(sub[0].ToSizeT(),sub[1],sub[2].ToBool(true)));
		}

		//  Make Link between documents and subjects
		RQuery docs(Db,"SELECT docid,subjectid,used FROM subjectsbydocs");
		for(docs.Start();!docs.End();docs.Next())
		{
			GDoc* d=Session->GetDoc(docs[0].ToSizeT());
			if(!d) continue;
			size_t UsedId,CatId;

			switch(docs[2].ToSizeT())
			{
				case 0:
					CatId=docs[1].ToSizeT();
					UsedId=cNoRef;
					break;
				case 1:
					CatId=cNoRef;
					UsedId=docs[1].ToSizeT();
					break;
				case 2:
					CatId=UsedId=docs[1].ToSizeT();
					break;
				default:
					throw GException("GStorageMySQL::LoadSubjects(void) : used cannot only takes 0,1 or 2");
			}
			Session->Insert(d,CatId,UsedId);
		}

		// Make links between profiles and subjects
		RQuery profiles(Db,"SELECT profileid,subjectid FROM profiles WHERE subjectid<>0");
		for(profiles.Start();!profiles.End();profiles.Next())
		{
			GProfile* prof=Session->GetProfile(profiles[0].ToSizeT());
			if(!prof) continue;
			Session->Insert(prof,profiles[1].ToSizeT());
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
void GStorageMySQL::SaveSubject(GSubject* subject)
{
	try
	{
		if(subject->GetId()==0)
			return;

		// Clear
		RQuery(Db,"DELETE FROM subjectsbydocs WHERE subjectid="+Num(subject->GetId()));
		RQuery(Db,"UPDATE profiles SET subjectid=0 WHERE subjectid="+Num(subject->GetId()));
		RQuery(Db,"DELETE FROM subjects WHERE subjectid="+Num(subject->GetId()));

		RString sSql;

		if(Session->GetSlot())
			Session->GetSlot()->StartJob("Save subjects");

		size_t parentid;
		if(subject->GetParent())
			parentid=subject->GetParent()->GetId();
		else
			parentid=0;
		RString used;
		if(subject->IsUsed())
			used=",'1')";
		else
			used=",'0')";

		// Insert the topic
		sSql="INSERT INTO subjects(subjectid,name,parent,used) VALUES("+
			Num(subject->GetId())+","+RQuery::SQLValue(subject->GetName())+","+Num(parentid)+used;
		RQuery Ins(Db,sSql);

		// Get all the documents assigned to the subject
		RCursor<GDoc> Docs(subject->GetTotalDocs());
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			// Look if the document is also selected in this subject
			RString used;
			if(subject->IsIn(Docs()))
				used="'2')";  // Categorized and selected
			else
				used="'0')"; // Just categorized

			// Look if the couple(subject,doc) is already in the database
			sSql="INSERT INTO subjectsbydocs(subjectid,docid,used) VALUES("+Num(subject->GetId())+","+Num(Docs()->GetId())+","+used;
			RQuery(Db,sSql);
		}

		// See if some selected documents are not categorized to the subject
		RCursor<GDoc> SelectedDocs(subject->GetObjs(static_cast<GDoc*>(0)));
		for(SelectedDocs.Start();!SelectedDocs.End();SelectedDocs.Next())
		{
			if(subject->IsCategorized(SelectedDocs()))
				continue;

			sSql="INSERT INTO subjectsbydocs(subjectid,docid,used) VALUES("+Num(subject->GetId())+","+Num(SelectedDocs()->GetId())+",'1')";
			RQuery(Db,sSql);
		}

		// Assign the profiles to the topic
		RCursor<GProfile> Profiles(subject->GetObjs(static_cast<GProfile*>(0)));
		for(Profiles.Start();!Profiles.End();Profiles.Next())
		{
			sSql="UPDATE profiles SET subjectid="+Num(subject->GetId())+" WHERE profileid="+Num(Profiles()->GetId());
			RQuery(Db,sSql);
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
			RQuery exec(Db,l);
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
void GStorageMySQL::Clear(tObjType objtype)
{
	try
	{
		// Look What to do
		RString What;
		bool Group(false);
		bool Topic(false);
		bool Doc(false);
		bool ResetAutoIncrement(false);
		switch(objtype)
		{
			case otUser:
				What="users";
				ResetAutoIncrement=true;
				break;
			case otDoc:
			{
				What="docs";
				Doc=true;
				ResetAutoIncrement=true;
				break;
			}
			case otProfile:
			{
				What="profiles";
				ResetAutoIncrement=true;
				break;
			}
			case otCommunity:
			{
				Group=true;
				What="communities";
				ResetAutoIncrement=true;
				break;
			}
			case otTopic:
			{
				Topic=true;
				What="topics";
				ResetAutoIncrement=true;
				break;
			}
			case otFdbk:
				What="docsbyprofiles";
				break;
			case otSubject:
			{
				What="subjects";
				RQuery Del1(Db,"DELETE FROM subjectsbydocs");
				RQuery Del3(Db,"UPDATE profiles SET subjectid=0");
				ResetAutoIncrement=true;
				break;
			}
			case otClass:
				What="classes";
				ResetAutoIncrement=true;
				break;
			default:
				ThrowGException("Cannot clear "+GetObjType(objtype,false,true)+" from storage");
		}

		// Clear the table and reset eventually the auto-increment field
		RQuery Delete(Db,"DELETE FROM "+What);
		if(ResetAutoIncrement)
			RQuery Reset(Db,"ALTER TABLE "+What+" AUTO_INCREMENT = 1");

		// If communities -> All profiles are detached
		if(Group)
			RQuery Update(Db,"UPDATE profiles SET communityid=0,attached='1970-01-01'");

		// If topics -> All documents are detached
		if(Topic)
			RQuery Update(Db,"UPDATE docs SET topicid=0,attached='1970-01-01'");

		if(Doc)
		{
			RQuery Delete1(Db,"DELETE FROM docsbycommunities");
			RQuery Delete2(Db,"DELETE FROM docsbylinks");
			RQuery Delete3(Db,"DELETE FROM docsbyprofiles");
			RQuery Delete4(Db,"DELETE FROM subjectsbydocs");
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
// Knowledge Methods
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::LoadConceptTypes(void)
{
	try
	{
		RQuery Types(Db,"SELECT typeid,name,description,refdocs,refprofiles,refcommunities,reftopics,refclasses FROM concepttypes");
		for(Types.Start();!Types.End();Types.Next())
			Session->InsertConceptType(static_cast<char>(atoi(Types[0])),Types[1],Types[2],
					Types[3].ToSizeT(),Types[4].ToSizeT(),Types[5].ToSizeT(),Types[6].ToSizeT(),Types[7].ToSizeT());
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
		RQuery find(Db,sSql);
		find.Start();
		if(!find.End())
		{
			type->SetId(find[0].ToChar());
			return;
		}

		// Insert the new concept type
		sSql="INSERT INTO concepttypes(name,description) VALUES("+name+","+desc+")";
		RQuery insert(Db,sSql);

		// Get the next id
		sSql=RString("SELECT typeid FROM concepttypes WHERE typeid=LAST_INSERT_ID()");
		RQuery getinsert(Db,sSql);
		getinsert.Start();
		type->SetId(getinsert[0].ToChar());
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadConcepts(void)
{
	try
	{
		size_t XMLIndexId(Session->GetConceptType("XMLIndex",false)->GetId());

		// Create and insert the dictionary
		// Load the dictionary from the database
		RQuery dicts(Db,"SELECT conceptid,name,typeid,"
				"refdocs,indexdocs,"
				"refprofiles,indexprofiles,"
				"refcommunities,indexcommunities,"
				"reftopics,indextopics,"
				"refclasses,indexclasses FROM concepts");
		for(dicts.Start();!dicts.End();dicts.Next())
		{
			size_t TypeId(dicts[2].ToSizeT());
			GConceptType* Type(Session->GetConceptType(TypeId,false));
			if(TypeId==XMLIndexId)
			{
				GXMLIndex w(dicts[0].ToSizeT(),dicts[1],Type,
						dicts[3].ToSizeT(),dicts[4].ToSizeT(),
						dicts[5].ToSizeT(),dicts[6].ToSizeT(),
						dicts[7].ToSizeT(),dicts[8].ToSizeT(),
						dicts[9].ToSizeT(),dicts[10].ToSizeT(),
						dicts[11].ToSizeT(),dicts[12].ToSizeT());
				Session->InsertConcept(&w);
			}
			else
			{
				GConcept w(dicts[0].ToSizeT(),dicts[1],Type,
						dicts[3].ToSizeT(),dicts[4].ToSizeT(),
						dicts[5].ToSizeT(),dicts[6].ToSizeT(),
						dicts[7].ToSizeT(),dicts[8].ToSizeT(),
						dicts[9].ToSizeT(),dicts[10].ToSizeT(),
						dicts[11].ToSizeT(),dicts[12].ToSizeT());
				Session->InsertConcept(&w);
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
void GStorageMySQL::AssignId(GConcept* concept)
{
	try
	{
		// Init some strings
		RString name=RQuery::SQLValue(concept->GetName());
		RString type=Num(concept->GetType()->GetId());

		// Verify that the concept didn't already exist.
		RString sSql="SELECT conceptid,name FROM concepts WHERE typeid="+type+" AND name="+name;
		RQuery find(Db,sSql);
		find.Start();
		if(!find.End())
		{
			concept->SetId(strtoul(find[0],0,10));
			return;
		}

		// Insert the new concept
		RQuery insert(Db,"INSERT INTO concepts(name,typeid) VALUES("+name+","+type+")");

		// Get the next id
		sSql=RString("SELECT conceptid FROM concepts WHERE conceptid=LAST_INSERT_ID()");
		RQuery getinsert(Db,sSql);
		getinsert.Start();
		concept->SetId(getinsert[0].ToSizeT());
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
		RQuery Del(Db,"DELETE FROM concepts WHERE conceptid="+Num(concept->GetId()));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
RString GStorageMySQL::LoadConcept(size_t id)
{
	try
	{
		RString sSql("SELECT name FROM concepts WHERE conceptid="+Num(id));
		RQuery w(Db,sSql);
		w.Start();
		if(!w.End())
			return(w[0]);
		return(RString::Null);
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
		RString sSql("SELECT conceptid FROM concepts WHERE typeid="+Num(type->GetId())+" AND kwd="+RQuery::SQLValue(name));
		RQuery w(Db,sSql);
		w.Start();
		if(!w.End())
			return(w[0].ToSizeT());
		return(0);
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
		RQuery Delete(Db,"DELETE FROM concepts WHERE conceptid="+Num(concept->GetId()));

		// Insert the new word in the database
		Sql="INSERT INTO concepts(conceptid,name,typeid,refprofiles,refcommunities,refdocs,reftopics,refclasses) ";
		Sql+="VALUES("+Num(concept->GetId())+","+RQuery::SQLValue(concept->GetName())+","+
		               Num(concept->GetType()->GetId())+","+
		               Num(concept->GetRef(otProfile))+","+
		               Num(concept->GetRef(otCommunity))+","+
		               Num(concept->GetRef(otDoc))+","+
		               Num(concept->GetRef(otTopic))+","+
		               Num(concept->GetRef(otClass))+")";
		RQuery Insert(Db,Sql);
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
				sSql="UPDATE concepts SET refdocs="+Num(refs)+" WHERE conceptid="+Num(concept->GetId());
				break;
			case otProfile:
				sSql="UPDATE concepts SET refprofiles="+Num(refs)+" WHERE conceptid="+Num(concept->GetId());
				break;
			case otCommunity:
				sSql="UPDATE concepts SET refcommunities="+Num(refs)+" WHERE conceptid="+Num(concept->GetId());
				break;
			case otTopic:
				sSql="UPDATE concepts SET reftopics="+Num(refs)+" WHERE conceptid="+Num(concept->GetId());
				break;
			case otClass:
				sSql="UPDATE concepts SET refclasses="+Num(refs)+" WHERE conceptid="+Num(concept->GetId());
				break;
			default:
				ThrowGException("This type of objects do not have descriptions");
		};
		RQuery(Db,sSql);
	}
	catch(RDbException e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveIndex(const GConcept* concept,tObjType what,size_t indexdocs)
{
	try
	{
		RString sSql;

		switch(what)
		{
			case otDoc:
				sSql="UPDATE concepts SET indexdocs="+RString::Number(indexdocs)+" WHERE conceptid="+Num(concept->GetId());
				break;
/*			case otProfile:
				sSql="UPDATE concepts SET refprofiles="+Num(refs)+
				     " WHERE conceptid="+Num(concept->GetId())+" AND typeid="+Num(concept->GetType()->GetId());
				break;
			case otCommunity:
				sSql="UPDATE concepts SET refcommunities="+Num(refs)+
				     " WHERE conceptid="+Num(concept->GetId())+" AND typeid="+Num(concept->GetType()->GetId());
				break;
			case otTopic:
				sSql="UPDATE concepts SET reftopics="+Num(refs)+
				     " WHERE conceptid="+Num(concept->GetId())+" AND typeid="+Num(concept->GetType()->GetId());
				break;*/
			default:
				ThrowGException("This type of objects do not have index");
		};
		RQuery(Db,sSql);
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
				RQuery(Db,"UPDATE concepttypes SET refdocs="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				if(refs==0)
					RQuery(Db,"UPDATE concepts SET refdocs=0 WHERE typeid="+Num(type->GetId()));
				break;
			}
			case otProfile:
			{
				RQuery(Db,"UPDATE concepttypes SET refprofiles="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				if(refs==0)
					RQuery(Db,"UPDATE concepts SET refcommunities=0 WHERE typeid="+Num(type->GetId()));
				break;
			}
			case otCommunity:
			{
				RQuery(Db,"UPDATE concepttypes SET refcommunities="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				if(refs==0)
					RQuery(Db,"UPDATE concepts SET refcommunities=0 WHERE typeid="+Num(type->GetId()));
				break;
			}
			case otTopic:
			{
				RQuery(Db,"UPDATE concepttypes SET reftopics="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				if(refs==0)
					RQuery(Db,"UPDATE concepts SET reftopics=0 WHERE typeid="+Num(type->GetId()));
				break;
			}
			case otClass:
			{
				RQuery(Db,"UPDATE concepttypes SET refclasses="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				if(refs==0)
					RQuery(Db,"UPDATE concepts SET refclasses=0 WHERE typeid="+Num(type->GetId()));
				break;
			}
			default:
				ThrowGException("This type of objects do not have descriptions");
		};
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadPredicates(void)
{
	try
	{
		RQuery Types(Db,"SELECT predicateid,name,description FROM predicates");
		for(Types.Start();!Types.End();Types.Next())
			Session->InsertPredicate(Types[0].ToSizeT(),Types[1],Types[2]);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GPredicate* predicate)
{
	try
	{
		// Init some strings
		RString name=RQuery::SQLValue(predicate->GetName());

		// Verify that the predicate didn't already exist.
		RString sSql="SELECT predicateid FROM predicates WHERE name="+name;
		RQuery find(Db,sSql);
		find.Start();
		if(!find.End())
		{
			predicate->SetId(find[0].ToSizeT());
			return;
		}

		// Insert the new predicate
		sSql="INSERT INTO predicates(name,description) VALUES("+name+","+RQuery::SQLValue(predicate->GetDescription())+")";
		RQuery insert(Db,sSql);

		// Get the next id
		sSql=RString("SELECT predicateid FROM predicates WHERE predicateid=LAST_INSERT_ID()");
		RQuery getinsert(Db,sSql);
		getinsert.Start();
		predicate->SetId(getinsert[0].ToSizeT());
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadStatements(void)
{
	try
	{
		RQuery Statements(Db,"SELECT statementid,predicate,xi,xitype,xj,xjtype,weight FROM statements");
		for(Statements.Start();!Statements.End();Statements.Next())
			Session->InsertStatement(Statements[0].ToSizeT(),
					Statements[1].ToSizeT(),
					Statements[2].ToSizeT(),static_cast<tObjType>(Statements[3].ToInt()),
					Statements[4].ToSizeT(),static_cast<tObjType>(Statements[5].ToInt()),
					Statements[6].ToDouble());
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GStatement* statement)
{
	try
	{
		// Init some strings
		RString predicate(Num(statement->GetPredicate()->GetId()));
		RString xi(Num(statement->GetXi()->GetId()));
		RString xitype(Num(statement->GetXi()->GetObjType()));
		RString xj(Num(statement->GetXj()->GetId()));
		RString xjtype(Num(statement->GetXj()->GetObjType()));

		// Verify that the statement didn't already exist.
		RString sSql="SELECT statementid FROM statements WHERE xi="+xi+" AND xitype="+xitype+
		             " AND predicate="+predicate+" AND xj="+xj+" AND xjtype="+xjtype;
		RQuery find(Db,sSql);
		find.Start();
		if(!find.End())
		{
			statement->SetId(find[0].ToSizeT());
			return;
		}

		// Insert the new statement
		RQuery insert(Db,"INSERT INTO statements(predicate,xi,xitype,xj,xjtype,weight) VALUES("+
				         predicate+","+xi+","+xitype+","+xj+","+xjtype+","+Num(statement->GetWeight())+")");

		// Get the next id
		sSql=RString("SELECT statementid FROM statements WHERE statementid=LAST_INSERT_ID()");
		RQuery getinsert(Db,sSql);
		getinsert.Start();
		statement->SetId(getinsert[0].ToSizeT());
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadClasses(void)
{
	try
	{
		if(Session->GetSlot())
			Session->GetSlot()->StartJob("Load Classes");

		// Load the classes
		RQuery sub(Db,"SELECT classid,name,parent,blockid FROM classes");
		for(sub.Start();!sub.End();sub.Next())
		{
			size_t ParentId();
			GClass* Parent(Session->GetClass(sub[2].ToSizeT(),true));
			GClass* Class(Session->InsertClass(Parent,sub[0].ToSizeT(),sub[3].ToSizeT(),sub[1]));
			Class->SetState(osNeedLoad);
		}

		if(Session->GetSlot())
			Session->GetSlot()->EndJob();
	}
	catch(RDbException e)
	{
		if(Session->GetSlot())
			Session->GetSlot()->EndJob();
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GClass* theclass)
{
	try
	{
		// Init some strings
		RString name=RQuery::SQLValue(theclass->GetName());
		RString Id;
		if(theclass->GetParent())
			Id=Num(theclass->GetParent()->GetId());
		else
			Id="'0'";
		// Verify that the class didn't already exist.
		RQuery find(Db,"SELECT classid FROM classes WHERE name="+name+" and parent="+Id);
		find.Start();
		if(!find.End())
		{
			theclass->SetId(find[0].ToSizeT());
			return;
		}

		// Insert the new class
		RQuery insert(Db,"INSERT INTO classes(name) VALUES("+name+")");

		// Get the next id
		RQuery getinsert(Db,"SELECT classid FROM classes WHERE classid=LAST_INSERT_ID()");
		getinsert.Start();
		theclass->SetId(getinsert[0].ToSizeT());
	}
	catch(RDbException e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveClass(GClass* theclass)
{
	try
	{
		// Init
		RString id(Num(theclass->GetId()));
		size_t ParentId(0);
		if(theclass->GetParent())
			ParentId=theclass->GetParent()->GetId();

		// Test if the class already exists.
		RQuery Test(Db,"SELECT COUNT(1) FROM classes WHERE classid="+id);
		Test.Start();
		if(!atoi(Test[0]))
		{
			// Insert the class
			RString sSql("INSERT INTO classes(classid,name,parent,blockid) "
			             " VALUES("+id+","+RQuery::SQLValue(theclass->GetName())+","+Num(ParentId)+","+Num(theclass->GetBlockId())+")");
			RQuery Insert(Db,sSql);
		}
		else
		{
			// Update the class
			RString sSql("UPDATE classes "
				 "SET name="+RQuery::SQLValue(theclass->GetName())+",parent="+Num(ParentId)+",blockid="+Num(theclass->GetBlockId())+
			     " WHERE classid="+id);
			RQuery Update(Db,sSql);
		}
		theclass->SetState(osUpToDate);
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
		RString sSql="INSERT INTO docs(doc,title,langid) "
		             "VALUES("+RQuery::SQLValue(doc->GetURL()())+","+RQuery::SQLValue(doc->GetName())+","+Lang(doc->GetLang())+")";
		RQuery Insert(Db,sSql);

		// Get the id and assign it to the document
		sSql=RString("SELECT docid FROM docs WHERE docid=LAST_INSERT_ID()");
		RQuery Get(Db,sSql);
		Get.Start();
		doc->SetId(atoi(Get[0]));
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

		RQuery quer (Db,"SELECT docid,doc,title,mimetype,langid,updated,calculated,topicid,attached,blockid,structid "
		                "FROM docs WHERE docid="+Num(docid));
		quer.Start();
		if(quer.End())
			return(0);

		// Verify that the langague is active
		GLang* lang(GALILEIApp->GetPlugIn<GLang>("Lang",quer[4],false));
		if((!lang)&&(!quer[4].IsEmpty()))
			return(0);

		doc=new GDoc(quer[1],quer[2],docid,quer[9].ToSizeT(),quer[10].ToSizeT(),lang,
				     quer[3],quer[7].ToSizeT(),GetMySQLToDate(quer[6]),GetMySQLToDate(quer[5]),GetMySQLToDate(quer[8]));
		doc->SetState(osNeedLoad);

		// Load the links of the document loaded.
		RQuery querLinks (Db,"SELECT docid,linkid,occurs FROM docsbylinks WHERE docid="+Num(docid));
		for(querLinks.Start();!querLinks.End();querLinks.Next())
			doc->InsertLink(Session->GetDoc(querLinks[1].ToSizeT()),querLinks[2].ToSizeT());

		return(doc);
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
	try
	{
		RString Sql("SELECT docid,doc,title,mimetype,langid,updated,calculated,topicid,attached,blockid,structid FROM docs");
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
			// Verify if its language is active
			GLang* lang(GALILEIApp->GetPlugIn<GLang>("Lang",quer[4],false));
			if((!lang)&&(!quer[4].IsEmpty()))
				continue;
			size_t docid(quer[0].ToSizeT());
			GDoc* doc(new GDoc(quer[1],quer[2],docid,quer[9].ToSizeT(),quer[10].ToSizeT(),lang,
					           quer[3],quer[7].ToSizeT(),GetMySQLToDate(quer[6]),GetMySQLToDate(quer[5]),GetMySQLToDate(quer[8])));
			Session->InsertDoc(doc);
			doc->SetState(osNeedLoad);

			// Load the links of the document loaded.
			RQuery querLinks (Db,"SELECT docid,linkid,occurs FROM docsbylinks "
			                     "WHERE docid="+Num(docid));
			for(querLinks.Start(); !querLinks.End(); querLinks.Next())
				doc->InsertLink(Session->GetDoc(atoi(querLinks[1])), atoi(querLinks[2]));
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
		sSql="SELECT COUNT(1) FROM docs WHERE docid="+Num(doc->GetId());
		RQuery Test(Db,sSql);
		Test.Start();
		if(!atoi(Test[0]))
		{
			// Insert the document
			sSql="INSERT INTO docs(docid,doc,title,mimetype,langid,updated,calculated,topicid,attached,blockid,structid) "
			     "VALUES("+Num(doc->GetId())+","+RQuery::SQLValue(doc->GetURL()())+","+
			     RQuery::SQLValue(doc->GetName())+","+f+","+l+","+RQuery::SQLValue(doc->GetUpdated())+
			     ","+RQuery::SQLValue(doc->GetComputed())+","+Num(doc->GetGroupId())+","+RQuery::SQLValue(doc->GetAttached())+
			     ","+Num(doc->GetBlockId())+","+Num(doc->GetStructId())+")";
			RQuery Insert(Db,sSql);
		}
		else
		{
			// Update the document
			sSql="UPDATE docs SET doc="+RQuery::SQLValue(doc->GetURL()())+",title="+
			     RQuery::SQLValue(doc->GetName())+",mimetype="+f+",langid="+l+
			     ",updated="+RQuery::SQLValue(doc->GetUpdated())+",calculated="+RQuery::SQLValue(doc->GetComputed())+
			     ",topicid="+Num(doc->GetGroupId())+",attached="+RQuery::SQLValue(doc->GetAttached())+
			     ",blockid="+Num(doc->GetBlockId())+",structid="+Num(doc->GetStructId())+
				 " WHERE docid="+Num(doc->GetId());
			RQuery Update(Db,sSql);
		}

		// Update links to others documents
		RQuery deletelinks(Db,"DELETE FROM docsbylinks WHERE docid="+Num(doc->GetId()));
		lcur= doc->GetLinks();
		for ( lcur.Start(); ! lcur.End(); lcur.Next())
		{
			sSql="INSERT INTO docsbylinks(docid,linkid,occurs) VALUES("+Num(doc->GetId())+","+Num(lcur()->GetId())+","+Num(lcur()->GetOccurs())+")";
			RQuery insertkwds(Db,sSql);
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
			RQuery Users(Db,"SELECT userid,user,fullname FROM users");
			for(Users.Start();!Users.End();Users.Next())
				Session->InsertUser(new GUser(atoi(Users[0]),Users[1],Users[2],10));

			// Load profiles
			RString Sql("SELECT profileid,description,social,userid,attached,communityid,updated,calculated,blockid,score,level,profiletype FROM profiles");
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
				GUser* user=Session->GetUser(Profiles[3].ToSizeT());
				size_t groupid(Profiles[5].ToSizeT());
				Session->InsertProfile(prof=new GProfile(user,GetProfileType(atoi(Profiles[11])),Profiles[0].ToSizeT(),Profiles[8].ToSizeT(),Profiles[1],groupid,
						GetMySQLToDate(Profiles[4]),GetMySQLToDate(Profiles[6]),GetMySQLToDate(Profiles[7]),
						Profiles[2].ToBool(false),Profiles[9].ToDouble(),Profiles[10].ToChar(),5));
				prof->SetState(osNeedLoad);
			}

			// Load feedbacks
			RQuery fdbks(Db,"SELECT docid,fdbk,profileid,done,computed FROM docsbyprofiles");
			for(fdbks.Start();!fdbks.End();fdbks.Next())
				Session->AddFdbk(atoi(fdbks[2]),atoi(fdbks[0]),GetFdbkType(atoi(fdbks[1])),GetMySQLToDate(fdbks[3]),GetMySQLToDate(fdbks[4]));
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
		RQuery User(Db,"SELECT userid,user,fullname FROM users WHERE userid="+Num(userid));
		User.Start();
		if(User.End())
			return(0);
		return(new GUser(atoi(User[0]),User[1],User[2],10));
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
		RQuery User(Db,"SELECT userid,user,fullname FROM users WHERE user="+RQuery::SQLValue(name));
		User.Start();
		if(User.End())
			return(0);
		return(new GUser(atoi(User[0]),User[1],User[2],10));
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
		GPlugInManager* Langs=GALILEIApp->GetManager("Lang");

		// Load Profile
		RQuery Profile(Db,"SELECT profileid,description,social,userid,attached,communityid,updated,calculated,blockid,score,level,profiletype "
		                  "FROM profiles WHERE profileid="+Num(profileid));
		Profile.Start();
		if(Profile.End())
			return(0);
		GUser* user=Session->GetUser(Profile[3].ToSizeT());
		if(!user)
			throw GException("Profile "+Profile[0]+" has no parent user");
		size_t groupid=Profile[5].ToSizeT();

		// Create the profile
		GProfile* prof=new GProfile(user,GetProfileType(Profile[11].ToUInt()),Profile[0].ToSizeT(),Profile[8].ToSizeT(),Profile[1],groupid,
				GetMySQLToDate(Profile[4]),GetMySQLToDate(Profile[6]),GetMySQLToDate(Profile[7]),
				Profile[2].ToBool(true),Profile[9].ToDouble(),Profile[10].ToChar(),5);
		prof->SetState(osNeedLoad);

		// Load Feedbacks
		RQuery fdbks(Db,"SELECT docid,fdbk,profileid,done,computed "
		                "FROM docsbyprofiles WHERE profileid="+Num(profileid));
		for(fdbks.Start();!fdbks.End();fdbks.Next())
		{
			Session->AddFdbk(atoi(fdbks[2]),atoi(fdbks[0]),GetFdbkType(atoi(fdbks[1])),RDate(fdbks[3]),RDate(fdbks[4]));
			// Since the profile is not in the session -> we must manually insert the profile.
			GLang* lang(Langs->GetPlugIn<GLang>(fdbks[5],false));
			if(!lang)
				continue;
			prof->AddFdbk(atoi(fdbks[0]),GetFdbkType(atoi(fdbks[1])),RDate(fdbks[3]),RDate(fdbks[4]));
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
void GStorageMySQL::UpdateProfiles(size_t docid)
{
	try
	{
		RQuery Up(Db,"UPDATE docsbyprofiles SET computed=CURDATE() WHERE docid="+Num(docid));
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
		// Reserved an identifier
		RString sSql=RString("INSERT INTO users(user,fullname) VALUES("+RQuery::SQLValue(user->GetName())+","+RQuery::SQLValue(user->GetFullName())+")");
		RQuery Insert(Db,sSql);

		// Get the id and assign it to the user
		sSql=RString("SELECT userid FROM users WHERE userid=LAST_INSERT_ID()");
		RQuery Get(Db,sSql);
		Get.Start();
		user->SetId(atoi(Get[0]));
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
		// Reserved an identifier
		RString sSql=RString("INSERT INTO profiles(userid,description) "
		                     "VALUES("+Num(p->GetUser()->GetId())+","+RQuery::SQLValue(p->GetName())+")");
		RQuery Insert(Db,sSql);

		// Get the id and assign it to the profile
		sSql=RString("SELECT profileid FROM profiles WHERE profileid=LAST_INSERT_ID()");
		RQuery Get(Db,sSql);
		Get.Start();
		p->SetId(atoi(Get[0]));
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
		RQuery Test(Db,sSql);
		Test.Start();
		if(!atoi(Test[0]))
		{
			// Insert the profile (if subjects -> save topicid)
			sSql="INSERT INTO profiles(profileid,description,social,userid,updated,calculated,attached,blockid,score,level,subjectid,profiletype";
			sSql+=") VALUES("+Num(profid)+","+RQuery::SQLValue(prof->GetName())+","+
			      Num(social)+","+Num(prof->GetUser()->GetId())+","+
			      RQuery::SQLValue(prof->GetUpdated())+","+RQuery::SQLValue(prof->GetComputed())+","+RQuery::SQLValue(prof->GetAttached())+
			      ","+Num(prof->GetBlockId())+","+Num(prof->GetConfidenceScore())+","+Num(prof->GetConfidenceLevel())+","+
			      Num(prof->GetProfileType())+")";
			const GSubject* sub(Session->GetIdealGroup(prof));
			if(sub)
				sSql+=","+RString::Number(sub->GetId());
			else
				sSql+=",0";
			sSql+=")";
			RQuery Insert(Db,sSql);
		}
		else
		{
			// Update the profile (if subjects -> save topicid)
			sSql="UPDATE profiles SET description="+RQuery::SQLValue(prof->GetName())+",social="+Num(social)+
			     ",userid="+Num(prof->GetUser()->GetId())+",updated="+RQuery::SQLValue(prof->GetUpdated())+
			     ",calculated="+RQuery::SQLValue(prof->GetComputed())+",attached="+RQuery::SQLValue(prof->GetAttached())+
			     ",blockid="+Num(prof->GetBlockId())+",score="+Num(prof->GetConfidenceScore())+",level="+Num(prof->GetConfidenceLevel())+
			     ",profiletype="+Num(prof->GetProfileType());
			const GSubject* sub(Session->GetIdealGroup(prof));
			if(sub)
				sSql+=",subjectid="+Num(sub->GetId());
			else
				sSql+=",subjectid=0";
			sSql+=" WHERE profileid="+Num(profid);
			RQuery Update(Db,sSql);
		}

		// First insert information from GFdbk
		RCursor<GFdbk> Fdbks=prof->GetFdbks();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			// Clear the feedback
			RQuery Delete(Db,"DELETE FROM docsbyprofiles WHERE profileid="+Num(profid)+" AND docid="+Num(Fdbks()->GetDocId()));

			// Re-Insert all the feedback
			sSql="INSERT INTO docsbyprofiles(docid,fdbk,profileid,done) "
			     "VALUES("+Num(Fdbks()->GetDocId())+",'"+RString::Number(Fdbks()->GetFdbk())+"',"+
			     Num(prof->GetId())+","+RQuery::SQLValue(Fdbks()->GetDone())+")";
			RQuery Insert(Db,sSql);
		}

		// Update other information from the documents
		RQuery Update(Db,"UPDATE docs,docsbyprofiles SET "
				         "docsbyprofiles.langid=docs.langid,"
				         "docsbyprofiles.computed=docs.calculated,"
				         "docsbyprofiles.updated=docs.updated "
		                 "WHERE docsbyprofiles.docid=docs.docid AND profileid="+Num(prof->GetId()));
		prof->SetState(osUpToDate);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AddFdbk(size_t p,size_t d,tFdbkType fdbk,R::RDate date,R::RDate computed)
{
	try
	{
		RString sSql;
		sSql="INSERT INTO docsbyprofiles(docid,fdbk,profileid,done,computed) "
		     "VALUES("+Num(d)+",'"+RString::Number(fdbk)+"',"+Num(p)+","+RQuery::SQLValue(date)+
		     ","+RQuery::SQLValue(computed)+")";
		RQuery Insert(Db,sSql);
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
		RString Sql("SELECT communityid,updated,calculated,name,blockid FROM communities");
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
			group=new GCommunity(Groups[0].ToSizeT(),Groups[4].ToSizeT(),Groups[3],Groups[1],Groups[2]);
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
GCommunity* GStorageMySQL::LoadCommunity(size_t communityid)
{
	try
	{
		RQuery Group(Db,"SELECT communityid,updated,calculated,name,blockid FROM communities WHERE communityid="+RString::Number(communityid));
		Group.Start();
		if(Group.End())
			return(0);
		GCommunity* group=new GCommunity(Group[0].ToSizeT(),Group[4].ToSizeT(),Group[3],Group[1],Group[2]);
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
void GStorageMySQL::UpdateCommunities(size_t subid)
{
	try
	{
		RQuery Up(Db,"UPDATE communities,profiles SET communities.updated=CURDATE() "
	    	         "WHERE communities.communityid=profiles.communityid AND "
	        	     "profiles.profileid="+Num(subid));
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
		RString sSql;
		sSql="INSERT INTO communities(updated,calculated,name) "
		     "VALUES("+RQuery::SQLValue(grp->GetUpdated())+","+
		     RQuery::SQLValue(grp->GetComputed())+","+
		     RQuery::SQLValue(grp->GetName())+")";
		RQuery insert1(Db,sSql);

		// Get the id and assign it to the group
		sSql=RString("SELECT communityid FROM communities WHERE communityid=LAST_INSERT_ID()");
		RQuery Get(Db,sSql);
		Get.Start();
		grp->SetId(atoi(Get[0]));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveCommunity(GCommunity* grp)
{
	RCursor<GWeightInfo> WordCur;
	RString sSql;
	RCursor<GProfile> Sub;

	try
	{
		// Delete group and groups info
		sSql="INSERT INTO communities(communityid,updated,calculated,name,blockid) "
		     "VALUES("+Num(grp->GetId())+","+
		     RQuery::SQLValue(grp->GetUpdated())+","+
		     RQuery::SQLValue(grp->GetComputed())+","+
		     RQuery::SQLValue(grp->GetName())+","+
		     Num(grp->GetBlockId())+")";
		RQuery insert1(Db,sSql);

		// Save Profiles infos
		Sub=grp->GetObjs();
		for(Sub.Start();!Sub.End();Sub.Next())
		{
			sSql="UPDATE profiles SET communityid="+Num(grp->GetId())+","
			     "attached="+RQuery::SQLValue(Sub()->GetAttached())+" "
			     "WHERE profileid="+Num(Sub()->GetId());
			RQuery update(Db,sSql);
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveDocs(const GCommunityDocs& docs)
{
	if(!docs.GetCommunityId()) return;

	// Delete the existing documents
	RQuery Del(Db,"DELETE FROM docsbycommunities WHERE communityid="+Num(docs.GetCommunityId()));

	// Save the new documents
	RCursor<GDocRanking> Doc(docs);
	for(Doc.Start();!Doc.End();Doc.Next())
		RQuery Ins(Db,"INSERT INTO docsbycommunities(communityid,docid,ranking) VALUES("+
				Num(docs.GetCommunityId())+","+Num(Doc()->GetDocId())+","+Num(Doc()->GetRanking())+")");
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadDocs(GCommunityDocs& docs)
{
	RQuery Get(Db,"SELECT docid,ranking FROM docsbycommunities WHERE communityid="+Num(docs.GetCommunityId()));
	for(Get.Start();!Get.End();Get.Next())
		docs.InsertPtr(new GDocRanking(Get[0].ToSizeT(),Get[1].ToDouble()));
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
		RString Sql("SELECT topicid,updated,calculated,name,blockid FROM topics");
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
			group=new GTopic(Groups[0].ToSizeT(),Groups[4].ToSizeT(),Groups[3],Groups[1],Groups[2]);
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
		RQuery Group(Db,"SELECT topicid,updated,calculated,name,blockid FROM topics WHERE topicid="+RString::Number(topicid));
		Group.Start();
		if(Group.End())
			return(0);
		GTopic* group=new GTopic(Group[0].ToSizeT(),Group[4].ToSizeT(),Group[3],Group[1],Group[2]);
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
		RQuery Up(Db,"UPDATE topics,docs SET topics.updated=CURDATE() "
	    	         "WHERE topics.topicid=docs.topicid AND "
	        	     "docs.docid="+Num(subid));
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
		RString sSql;
		sSql="INSERT INTO topics(updated,calculated,name) "
		     "VALUES("+RQuery::SQLValue(grp->GetUpdated())+","+
		     RQuery::SQLValue(grp->GetComputed())+","+
			 RQuery::SQLValue(grp->GetName())+")";
		RQuery insert1(Db,sSql);

		// Get the id and assign it to the group
		sSql=RString("SELECT topicid FROM topics WHERE topicid=LAST_INSERT_ID()");
		RQuery Get(Db,sSql);
		Get.Start();
		grp->SetId(atoi(Get[0]));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveTopic(GTopic* grp)
{
	RCursor<GWeightInfo> WordCur;
	RString sSql;
	RCursor<GDoc> Sub;

	try
	{
		sSql="INSERT INTO topics(topicid,updated,calculated,name,blockid) "
		     "VALUES("+Num(grp->GetId())+","+
		     RQuery::SQLValue(grp->GetUpdated())+","+
		     RQuery::SQLValue(grp->GetComputed())+","+
			 RQuery::SQLValue(grp->GetName())+","+
			 Num(grp->GetBlockId())+")";
		RQuery insert1(Db,sSql);

		// Save documents infos
		Sub=grp->GetObjs();
		for(Sub.Start();!Sub.End();Sub.Next())
		{
			sSql="UPDATE docs SET topicid="+Num(grp->GetId())+","
			     "attached="+RQuery::SQLValue(Sub()->GetAttached())+" "
			     "WHERE docid="+Num(Sub()->GetId());
			RQuery update(Db,sSql);
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
// Topics Methods
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::SaveSugs(const GSugs& sugs,size_t max)
{
	if(!max) return;
	RString Table("sugsby");
	RString Id;
	switch(sugs.GetType())
	{
		case otProfile:
			Table+="profiles";
			Id="profileid";
			break;
		case otCommunity:
			Table+="communities";
			Id="communityid";
			break;
		default:
			ThrowGException("Invalid list of suggestions");
	}
	RQuery Del(Db,"DELETE FROM "+Table+" WHERE "+Id+"="+Num(sugs.GetAddresseeId()));
	RString InsSql("INSERT INTO "+Table+"("+Id+",docid,ranking,proposed,info) VALUES("+Num(sugs.GetAddresseeId())+",");
	RCursor<GSuggestion> Cur(sugs,0,max);
	for(Cur.Start();!Cur.End();Cur.Next())
		RQuery Ins(Db,InsSql+Num(Cur()->GetDocId())+","+Num(Cur()->GetRanking())+","+RQuery::SQLValue(Cur()->GetProposed())+","+RQuery::SQLValue(Cur()->GetInfo())+")");
}



//------------------------------------------------------------------------------
void GStorageMySQL::LoadSugs(GSugs& sugs)
{
	RString Table("sugsby");
	RString Id;
	switch(sugs.GetType())
	{
		case otProfile:
			Table+="profiles";
			Id="profileid";
			break;
		case otCommunity:
			Table+="communities";
			Id="communityid";
			break;
		default:
			ThrowGException("Invalid list of suggestions");
	}

	try
	{
		RQuery Load(Db,"SELECT docid,ranking,proposed,info FROM "+Table+" WHERE "+Id+"="+Num(sugs.GetAddresseeId())+" ORDER BY "+Id+",ranking");
		for(Load.Start();!Load.End();Load.Next())
			sugs.InsertPtr(new GSuggestion(Load[0].ToSizeT(),Load[1].ToDouble(),Load[2],Load[3]));
	}
	catch(RDbException e)
	{
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
	delete Db;
}


//------------------------------------------------------------------------------
CREATE_STORAGE_FACTORY("MySQL","MySQL",GStorageMySQL)
