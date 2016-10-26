/*

	GALILEI Research Project

	GMySQL.cpp

	Storage Manager using a MySQL Database - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
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
#include <gsugs.h>
#include <guser.h>
#include <gprofile.h>
#include <gmysql.h>
#include <gslot.h>
#include <gcommunity.h>
#include <gtopic.h>
#include <gsubject.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gstatement.h>
#include <gclass.h>
#include <gfdbk.h>



//------------------------------------------------------------------------------
//
//  GStorageMySQL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStorageMySQL::GStorageMySQL(GSession* session,GPlugInFactory* fac)
	: GStorage(session,fac), Db(0)
{
}


//------------------------------------------------------------------------------
size_t GStorageMySQL::GetCount(const RString& tbl,const R::RString& condition)
{
	RString c;
	RString sSql("SELECT COUNT(*) FROM "+tbl);

	if(condition!=RString::Null)
		sSql+=" WHERE "+condition;
	RQuery Count(Db,sSql);
	Count.Start();
	c=Count[0];
	if(c.IsEmpty()) return(0);
	return(atoi(c));
}


//------------------------------------------------------------------------------
size_t GStorageMySQL::GetMax(const RString& tbl,const RString& fld)
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
RString GStorageMySQL::GetMySQLToDate(const RString& date)
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
void GStorageMySQL::Init(void)
{
	if(Db)
	{
		delete Db;
		Db=0;
	}
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
	Host=FindParam<RParamValue>("Host")->Get();
	User=FindParam<RParamValue>("User")->Get();
	Password=FindParam<RParamValue>("Password")->Get();
	Database=FindParam<RParamValue>("Database")->Get();
	Filter.SetDate(FindParam<RParamValue>("Filter")->Get());
	LoadAll=FindParam<RParamValue>("All")->GetBool();
	Encoding=FindParam<RParamValue>("Encoding")->Get().ToLatin1();
	Filtering=FindParam<RParamValue>("Filtering")->GetBool();
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
				mThrowGException(GetObjType(type,true,true)+" are not supported");
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
void GStorageMySQL::LoadObjs(const GSubject*)
{
	try
	{
		// Load the subjects
		Session->VerifyTab(otSubject,GetCount("subjects"));
		RQuery sub(Db,"SELECT subjectid,name,used,parent FROM subjects");
		for(sub.Start();!sub.End();sub.Next())
		{
			size_t ParentId(sub[3].ToSizeT());
			GSubject* Parent(0);
			if(ParentId)
				Parent=Session->GetObj(pSubject,ParentId);
			GSubject* Subject(new GSubject(Session,sub[0].ToSizeT(),sub[1]));
			Session->InsertObj(Parent,Subject);
			if(sub[2].ToBool(true))
				 Session->SetUsed(Subject,true);
		}

		//  Make Link between documents and subjects
		RQuery docs(Db,"SELECT docid,subjectid,used FROM subjectsbydocs");
		for(docs.Start();!docs.End();docs.Next())
		{
			GDoc* d(Session->GetObj(pDoc,docs[0].ToSizeT()));
			if(!d) continue;
			GSubject* s(Session->GetObj(pSubject,docs[1].ToSizeT()));
			if(!s) continue;
			Session->InsertObj(s,d);
			if(docs[2].ToBool(true))
				Session->SetUsed(d,s,true);
		}

		// Make links between profiles and subjects
		RQuery profiles(Db,"SELECT profileid,subjectid FROM subjectsbyprofiles");
		for(profiles.Start();!profiles.End();profiles.Next())
		{
			GProfile* prof=Session->GetObj(pProfile,profiles[0].ToSizeT());
			if(!prof) continue;
			GSubject* s(Session->GetObj(pSubject,profiles[1].ToSizeT()));
			if(!s) continue;
			Session->InsertObj(s,prof);
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveObj(GSubject* subject)
{
	try
	{
		if(subject->GetId()==0)
			return;

		// Clear
		RQuery(Db,"DELETE FROM subjectsbydocs WHERE subjectid="+Num(subject->GetId()));
		RQuery(Db,"DELETE FROM subjectsbyprofiles WHERE subjectid="+Num(subject->GetId()));
		RQuery(Db,"DELETE FROM subjects WHERE subjectid="+Num(subject->GetId()));

		RString sSql;

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

		// Insert the subject
		sSql="INSERT INTO subjects(subjectid,name,parent,used) VALUES("+
			Num(subject->GetId())+","+RQuery::SQLValue(subject->GetName())+","+Num(parentid)+used;
		RQuery Ins(Db,sSql);

		// Get all the documents assigned to the subject
		RCursor<GDoc> Docs(subject->GetObjs(pDoc));
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			// Look if the document is also selected in this subject
			RString used;
			if(subject->IsUsed(Docs()))
				used="'1')";  // Categorized and selected
			else
				used="'0')"; // Just categorized

			// Insert the couple(subject,doc) in the database
			sSql="INSERT INTO subjectsbydocs(subjectid,docid,used) VALUES("+Num(subject->GetId())+","+Num(Docs()->GetId())+","+used;
			RQuery(Db,sSql);
		}

		// Get all the profiles assigned to subject
		RCursor<GProfile> Profiles(subject->GetObjs(pProfile));
		for(Profiles.Start();!Profiles.End();Profiles.Next())
		{
			// Insert the couple(subject,profile) in the database
			sSql="INSERT INTO subjectsbyprofiles(subjectid,profileid,used) VALUES("+Num(subject->GetId())+","+Num(Profiles()->GetId())+",'1')";
			RQuery(Db,sSql);
		}
	}
	catch(RDbException e)
	{
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
		bool Profile(false);
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
				Profile=true;
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
				RQuery Del2(Db,"DELETE FROM subjectsbyprofiles");
				ResetAutoIncrement=true;
				break;
			}
			case otClass:
				What="classes";
				ResetAutoIncrement=true;
				break;
			default:
				mThrowGException("Cannot clear "+GetObjType(objtype,false,true)+" from storage");
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
			RQuery Delete2(Db,"DELETE FROM docsbyprofiles");
			RQuery Delete3(Db,"DELETE FROM subjectsbydocs");
		}

		if(Profile)
		{
			RQuery Delete2(Db,"DELETE FROM docsbyprofiles");
			RQuery Delete3(Db,"DELETE FROM subjectsbyprofiles");
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
void GStorageMySQL::LoadObjs(const GConceptType*)
{
	try
	{
		Session->VerifyTab(otConceptType,GetCount("concepttypes"));
		RQuery Types(Db,"SELECT typeid,name,description,catid FROM concepttypes");
		for(Types.Start();!Types.End();Types.Next())
			Session->InsertObj(pConceptType,ConceptCat_cast(Types[3].ToChar()),static_cast<char>(atoi(Types[0])),Types[1],Types[2]);
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

		// Verify that the concept type didn't already exist.
		RString sSql="SELECT typeid FROM concepttypes WHERE name="+name;
		RQuery find(Db,sSql);
		find.Start();
		if(!find.End())
		{
			type->SetId(find[0].ToChar());
			return;
		}

		// Insert the new concept type
		sSql="INSERT INTO concepttypes(name,description,catid) VALUES("+name+","+desc+","+RString::Number(type->GetCategory())+")";
		RQuery insert(Db,sSql);

		// Get the next id
		sSql=RString("SELECT typeid FROM concepttypes WHERE typeid=LAST_INSERT_ID()");
		RQuery getinsert(Db,sSql);
		getinsert.Start();
		type->SetId(getinsert[0].ToSizeT());
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObjs(const GConcept*)
{
	try
	{
		Session->VerifyTab(otConcept,GetCount("concepts"));

		// Create and insert the dictionary
		// Load the dictionary from the database
		RQuery dicts(Db,"SELECT conceptid,name,typeid,"
				"indexdocs,"
				"indexprofiles,"
				"indexcommunities,"
				"indextopics,"
				"indexclasses,"
				"note FROM concepts");
		for(dicts.Start();!dicts.End();dicts.Next())
		{
			GConceptType* Type(Session->GetObj(pConceptType,dicts[2].ToSizeT(),false));
			GConcept w(Session,dicts[0].ToSizeT(),dicts[1],Type,
						dicts[3].ToSizeT(),
						dicts[4].ToSizeT(),
						dicts[5].ToSizeT(),
						dicts[6].ToSizeT(),
						dicts[7].ToSizeT(),
						dicts[8]);
				Session->InsertObj(w);
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
void GStorageMySQL::DeleteObj(GConcept* concept)
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
RString GStorageMySQL::LoadObj(const GConcept*,size_t id)
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
size_t GStorageMySQL::LoadObj(const GConcept*,const RString name,GConceptType* type)
{
	try
	{
		RString sSql("SELECT conceptid FROM concepts WHERE typeid="+Num(type->GetId())+" AND name="+RQuery::SQLValue(name));
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
void GStorageMySQL::SaveObj(GConcept* concept)
{
	try
	{
		RString Sql;

		// Delete the old word from the database
		RQuery Delete(Db,"DELETE FROM concepts WHERE conceptid="+Num(concept->GetId()));

		// Insert the new word in the database
		Sql="INSERT INTO concepts("
			    "conceptid,"
			    "name,"
			    "typeid,"
				 "indexdocs,"
				 "indexprofiles,"
				 "indexcommunities,"
				 "indextopics,"
				 "indexclasses,"
				 "note) "
		    "VALUES("+
		       Num(concept->GetId())+","+
			    RQuery::SQLValue(concept->GetName())+","+
		       Num(concept->GetType()->GetId())+
			    Num(concept->GetIndex(otDoc))+
			    Num(concept->GetIndex(otProfile))+
			    Num(concept->GetIndex(otCommunity))+
			    Num(concept->GetIndex(otTopic))+
			    Num(concept->GetIndex(otClass))+
				 ","+RQuery::SQLValue(concept->GetNote())+
			 ")";
		RQuery Insert(Db,Sql);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveIndex(const GConcept* concept,tObjType what,size_t index)
{
	try
	{
		RString sSql;
		sSql.SetLen(200);
		sSql.SetLen(0);

		switch(what)
		{
			case otDoc:
				sSql="UPDATE concepts SET indexdocs="+Num(index)+" WHERE conceptid="+Num(concept->GetId());
				break;
			case otProfile:
				sSql="UPDATE concepts SET indexprofiles="+Num(index)+" WHERE conceptid="+Num(concept->GetId());
				break;
			case otCommunity:
				sSql="UPDATE concepts SET indexcommunities="+Num(index)+" WHERE conceptid="+Num(concept->GetId());
				break;
			case otTopic:
				sSql="UPDATE concepts SET indextopics="+Num(index)+" WHERE conceptid="+Num(concept->GetId());
				break;
			case otClass:
				sSql="UPDATE concepts SET indexclasses="+Num(index)+" WHERE conceptid="+Num(concept->GetId());
				break;
			default:
				mThrowGException("This type of objects do not have index");
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
void GStorageMySQL::ClearIndex(tObjType what)
{
	try
	{
		RString sSql;

		switch(what)
		{
			case otDoc:
				sSql="UPDATE concepts SET indexdocs='0'";
				break;
			case otProfile:
				sSql="UPDATE concepts SET indexprofiles='0'";
				break;
			case otCommunity:
				sSql="UPDATE concepts SET indexcommunities='0'";
				break;
			case otTopic:
				sSql="UPDATE concepts SET indextopics='0'";
				break;
			case otClass:
				sSql="UPDATE concepts SET indexclasses='0'";
				break;
			default:
				mThrowGException("This type of objects do not have index");
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
				break;
			}
			case otProfile:
			{
				RQuery(Db,"UPDATE concepttypes SET refprofiles="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				break;
			}
			case otCommunity:
			{
				RQuery(Db,"UPDATE concepttypes SET refcommunities="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				break;
			}
			case otTopic:
			{
				RQuery(Db,"UPDATE concepttypes SET reftopics="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				break;
			}
			case otClass:
			{
				RQuery(Db,"UPDATE concepttypes SET refclasses="+Num(refs)+" WHERE typeid="+Num(type->GetId()));
				break;
			}
			default:
				mThrowGException("This type of objects do not have descriptions");
		};
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::ClearRefs(tObjType what)
{
	try
	{
		switch(what)
		{
			case otDoc:
			{
				RQuery(Db,"UPDATE concepttypes SET refdocs='0'");
				RQuery(Db,"UPDATE concepts SET refdocs='0'");
				break;
			}
			case otProfile:
			{
				RQuery(Db,"UPDATE concepttypes SET refprofiles='0'");
				RQuery(Db,"UPDATE concepts SET refprofiles='0'");
				break;
			}
			case otCommunity:
			{
				RQuery(Db,"UPDATE concepttypes SET refcommunities='0'");
				RQuery(Db,"UPDATE concepts SET refcommunities='0'");
				break;
			}
			case otTopic:
			{
				RQuery(Db,"UPDATE concepttypes SET reftopics='0'");
				RQuery(Db,"UPDATE concepts SET reftopics='0'");
				break;
			}
			case otClass:
			{
				RQuery(Db,"UPDATE concepttypes SET refclasses='0'");
				RQuery(Db,"UPDATE concepts SET refclasses='0'");
				break;
			}
			default:
				mThrowGException("This type of objects do not have descriptions");
		};
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObjs(const GStatement*)
{
	try
	{
		// Adapt sizes
		Session->VerifyTab(otStatement,GetCount("statements"));
		GConcept* IsA(Session->GetIsA());
		IsA->VerifyStatements(sePredicate,GetCount("statements","predicateid="+Num(IsA->GetId())+" AND predicatetype="+Num(IsA->GetObjType())));
		GConcept* Synonym(Session->GetSynonym());
		Synonym->VerifyStatements(sePredicate,GetCount("statements","predicateid="+Num(Synonym->GetId())+" AND predicatetype="+Num(Synonym->GetObjType())));
		GConcept* PartOf(Session->GetPartOf());
		PartOf->VerifyStatements(sePredicate,GetCount("statements","predicateid="+Num(PartOf->GetId())+" AND predicatetype="+Num(PartOf->GetObjType())));


		//                                0          1          2            3           4          5       6        7
		RQuery Statements(Db,"SELECT statementid,subjectid,subjecttype,predicateid,predicatetype,valueid,valuetype,weight FROM statements ORDER by statementid");
		for(Statements.Start();!Statements.End();Statements.Next())
		{
			GObject* Subject(Session->GetObj(static_cast<tObjType>(Statements[2].ToSizeT()),Statements[1].ToSizeT(),true));
			GObject* Predicate(Session->GetObj(static_cast<tObjType>(Statements[4].ToSizeT()),Statements[3].ToSizeT(),true));
			GObject* Value(Session->GetObj(static_cast<tObjType>(Statements[6].ToSizeT()),Statements[5].ToSizeT(),true));
			Session->InsertObj(pStatement,Statements[0].ToSizeT(),Subject,Predicate,Value,Statements[7].ToDouble());
		}
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
		RString subjectid(Num(statement->GetSubject()->GetId()));
		RString subjecttype(Num(statement->GetSubject()->GetObjType()));
		RString predicateid(Num(statement->GetPredicate()->GetId()));
		RString predicatetype(Num(statement->GetPredicate()->GetObjType()));
		RString valueid(Num(statement->GetValue()->GetId()));
		RString valuetype(Num(statement->GetValue()->GetObjType()));

		// Verify that the statement didn't already exist.
		RString sSql="SELECT statementid FROM statements WHERE subjectid="+subjectid+" AND subjecttype="+subjecttype+
		             " AND predicateid="+predicateid+" AND predicatetype="+predicatetype+
		             " AND valueid="+valueid+" AND valuetype="+valuetype;
		RQuery find(Db,sSql);
		find.Start();
		if(!find.End())
		{
			statement->SetId(find[0].ToSizeT());
			return;
		}

		// Insert the new statement
		RQuery insert(Db,"INSERT INTO statements(subjectid,subjecttype,predicateid,predicatetype,valueid,valuetype,weight) VALUES("+
				         subjectid+","+subjecttype+","+predicateid+","+predicatetype+","+valueid+","+valuetype+","+Num(statement->GetWeight())+")");

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
void GStorageMySQL::LoadObj(GClass* &classp,size_t classid)
{
	try
	{
		RQuery quer(Db,"SELECT classid,name,parent,blockid,calculated FROM classes WHERE classid="+Num(classid));
		quer.Start();
		if(quer.End())
		{
			classp=0;
			return;
		}
		GClass* Parent(Session->GetObj(pClass,quer[2].ToSizeT(),true,true));
		classp=new GClass(Session,quer[0].ToSizeT(),quer[3].ToSizeT(),quer[1],GetMySQLToDate(quer[4]));
		Session->InsertObj(Parent,classp);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObj(GClass* &classp,const RString& name)
{
	try
	{
		RQuery quer(Db,"SELECT classid,name,parent,blockid,calculated FROM classes WHERE name='"+name+"'");
		quer.Start();
		if(quer.End())
		{
			classp=0;
			return;
		}
		GClass* Parent(Session->GetObj(pClass,quer[2].ToSizeT(),true,true));
		classp=new GClass(Session,quer[0].ToSizeT(),quer[3].ToSizeT(),quer[1],GetMySQLToDate(quer[4]));
		Session->InsertObj(Parent,classp);
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObjs(const GClass*)
{
	try
	{
		Session->VerifyTab(otClass,GetCount("classes"));
		// Load the classes
		RQuery sub(Db,"SELECT classid,name,parent,blockid,calculated FROM classes");
		for(sub.Start();!sub.End();sub.Next())
		{
			GClass* Parent(Session->GetObj(pClass,sub[2].ToSizeT(),true,true));
			GClass* Class(new GClass(Session,sub[0].ToSizeT(),sub[3].ToSizeT(),sub[1],GetMySQLToDate(sub[4])));
			Session->InsertObj(Parent,Class);
		}
	}
	catch(RDbException e)
	{
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::AssignId(GClass* theclass)
{
	try
	{
		// Init some strings
		RString name(RQuery::SQLValue(theclass->GetName()));
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
		RQuery insert(Db,"INSERT INTO classes(name,parent,calculated,blockid) VALUES("+
			name+","+Id+","+RQuery::SQLValue(theclass->GetComputed())+","+Num(theclass->GetBlockId())+")");

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
void GStorageMySQL::SaveObj(GClass* theclass)
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
			RString sSql("INSERT INTO classes(classid,name,calculated,parent,blockid) "
			             " VALUES("+id+","+RQuery::SQLValue(theclass->GetName())+","+Num(ParentId)+","+RQuery::SQLValue(theclass->GetComputed())+","+Num(theclass->GetBlockId())+")");
			RQuery Insert(Db,sSql);
		}
		else
		{
			// Update the class
			RString sSql("UPDATE classes "
				 "SET name="+RQuery::SQLValue(theclass->GetName())+",parent="+Num(ParentId)+",blockid="+Num(theclass->GetBlockId())+",calculated="+RQuery::SQLValue(theclass->GetComputed())+
			     " WHERE classid="+id);
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
		             "VALUES("+RQuery::SQLValue(doc->GetName())+","+RQuery::SQLValue(doc->GetTitle())+","+Lang(doc->GetLang())+")";
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
void GStorageMySQL::LoadObj(GDoc* &doc,size_t docid)
{
	try
	{
		RQuery quer (Db,"SELECT docid,doc,title,mimetype,langid,updated,calculated,topicid,attached,blockid,structid,description "
		                "FROM docs WHERE docid="+Num(docid));
		quer.Start();
		if(quer.End())
		{
			doc=0;
			return;
		}

		// Verify that the langague is active
		GLang* lang(GALILEIApp->GetPlugIn<GLang>("Lang",quer[4],false));
		if((!lang)&&(!quer[4].IsEmpty()))
		{
			doc=0;
			return;
		}

		doc=new GDoc(Session,quer[1],quer[2],quer[11],docid,quer[9].ToSizeT(),quer[10].ToSizeT(),lang,
				     quer[3],quer[7].ToSizeT(),GetMySQLToDate(quer[6]),GetMySQLToDate(quer[5]),GetMySQLToDate(quer[8]));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObj(GDoc* &doc,const R::RString& url)
{
	try
	{
		RQuery quer (Db,"SELECT docid,doc,title,mimetype,langid,updated,calculated,topicid,attached,blockid,structid,description "
		                "FROM docs WHERE doc="+url);
		quer.Start();
		if(quer.End())
		{
			doc=0;
			return;
		}

		// Verify that the langague is active
		GLang* lang(GALILEIApp->GetPlugIn<GLang>("Lang",quer[4],false));
		if((!lang)&&(!quer[4].IsEmpty()))
		{
			doc=0;
			return;
		}

		doc=new GDoc(Session,quer[1],quer[2],quer[11],quer[0].ToSizeT(),quer[9].ToSizeT(),quer[10].ToSizeT(),lang,
				     quer[3],quer[7].ToSizeT(),GetMySQLToDate(quer[6]),GetMySQLToDate(quer[5]),GetMySQLToDate(quer[8]));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObjs(const GDoc*)
{
	try
	{
		Session->VerifyTab(otDoc,GetCount("docs"));
		RString Sql("SELECT docid,doc,title,mimetype,langid,updated,calculated,topicid,attached,blockid,structid,description FROM docs");
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
			GDoc* doc(new GDoc(Session,quer[1],quer[2],quer[11],docid,quer[9].ToSizeT(),quer[10].ToSizeT(),lang,
					           quer[3],quer[7].ToSizeT(),GetMySQLToDate(quer[6]),GetMySQLToDate(quer[5]),GetMySQLToDate(quer[8])));
			Session->InsertObj(doc);
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}

}


//------------------------------------------------------------------------------
void GStorageMySQL::SaveObj(GDoc* doc)
{
	RString sSql;
	RString l;
	RString id;
	RString f;

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
			sSql="INSERT INTO docs(docid,doc,title,mimetype,langid,updated,calculated,topicid,attached,blockid,structid,description) "
			     "VALUES("+Num(doc->GetId())+","+RQuery::SQLValue(doc->GetName())+","+
			     RQuery::SQLValue(doc->GetTitle())+","+f+","+l+","+RQuery::SQLValue(doc->GetUpdated())+
			     ","+RQuery::SQLValue(doc->GetComputed())+","+Num(doc->GetGroupId())+","+RQuery::SQLValue(doc->GetAttached())+
			     ","+Num(doc->GetBlockId())+","+Num(doc->GetStructId())+","+RQuery::SQLValue(doc->GetDescription())+")";
			RQuery Insert(Db,sSql);
		}
		else
		{
			// Update the document
			sSql="UPDATE docs SET doc="+RQuery::SQLValue(doc->GetName())+",title="+
			     RQuery::SQLValue(doc->GetTitle())+",mimetype="+f+",langid="+l+
			     ",updated="+RQuery::SQLValue(doc->GetUpdated())+",calculated="+RQuery::SQLValue(doc->GetComputed())+
			     ",topicid="+Num(doc->GetGroupId())+",attached="+RQuery::SQLValue(doc->GetAttached())+
			     ",blockid="+Num(doc->GetBlockId())+",structid="+Num(doc->GetStructId())+
				  ",description="+RQuery::SQLValue(doc->GetDescription())+
				 " WHERE docid="+Num(doc->GetId());
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
//
// Users/Profiles Methods
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::LoadObjs(const GUser*)
{
	GProfile* prof;

	try
	{
		if(IsAllInMemory()) // If everything must be in memory -> load all the users and the profiles
		{
			// Load users
			Session->VerifyTab(otUser,GetCount("users"));
			RQuery Users(Db,"SELECT userid,user,fullname FROM users");
			for(Users.Start();!Users.End();Users.Next())
				Session->InsertObj(new GUser(Session,atoi(Users[0]),Users[1],Users[2],10));

			// Load profiles
			Session->VerifyTab(otProfile,GetCount("profiles"));
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
				GUser* user=Session->GetObj(pUser,Profiles[3].ToSizeT());
				size_t groupid(Profiles[5].ToSizeT());
				Session->InsertObj(prof=new GProfile(Session,user,GetProfileType(atoi(Profiles[11])),Profiles[0].ToSizeT(),Profiles[8].ToSizeT(),Profiles[1],groupid,
						GetMySQLToDate(Profiles[4]),GetMySQLToDate(Profiles[6]),GetMySQLToDate(Profiles[7]),
						Profiles[2].ToBool(false),Profiles[9].ToDouble(),Profiles[10].ToChar(),5));
			}

			// Load feedbacks
			RQuery fdbks(Db,"SELECT docid,fdbk,profileid,done FROM docsbyprofiles");
			for(fdbks.Start();!fdbks.End();fdbks.Next())
				Session->InsertFdbk(atoi(fdbks[2]),atoi(fdbks[0]),GetFdbkType(atoi(fdbks[1])),GetMySQLToDate(fdbks[3]),true);
		}
	}
	catch(RDbException& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObj(GUser* &user,size_t userid)
{
	try
	{
		RQuery User(Db,"SELECT userid,user,fullname FROM users WHERE userid="+Num(userid));
		User.Start();
		if(User.End())
		{
			user=0;
			return;
		}
		user=new GUser(Session,atoi(User[0]),User[1],User[2],10);
	}
	catch(RDbException& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObj(GUser* &user,const R::RString& name)
{
	try
	{
		RQuery User(Db,"SELECT userid,user,fullname FROM users WHERE user="+RQuery::SQLValue(name));
		User.Start();
		{
			user=0;
			return;
		}
		user=new GUser(Session,atoi(User[0]),User[1],User[2],10);
	}
	catch(RDbException& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObj(GProfile* &profile,size_t profileid)
{
	try
	{
		// Load Profile
		RQuery Profile(Db,"SELECT profileid,description,social,userid,attached,communityid,updated,calculated,blockid,score,level,profiletype "
		                  "FROM profiles WHERE profileid="+Num(profileid));
		Profile.Start();
		if(Profile.End())
		{
			profile=0;
			return;
		}
		GUser* user=Session->GetObj(pUser,Profile[3].ToSizeT());
		if(!user)
			throw GException("Profile "+Profile[0]+" has no parent user");
		size_t groupid=Profile[5].ToSizeT();

		// Create the profile
		profile=new GProfile(Session,user,GetProfileType(Profile[11].ToUInt()),Profile[0].ToSizeT(),Profile[8].ToSizeT(),Profile[1],groupid,
				GetMySQLToDate(Profile[4]),GetMySQLToDate(Profile[6]),GetMySQLToDate(Profile[7]),
				Profile[2].ToBool(true),Profile[9].ToDouble(),Profile[10].ToChar(),5);

		// Load Feedbacks
		RQuery fdbks(Db,"SELECT docid,fdbk,profileid,done "
		                "FROM docsbyprofiles WHERE profileid="+Num(profileid));
		for(fdbks.Start();!fdbks.End();fdbks.Next())
			Session->InsertFdbk(atoi(fdbks[2]),atoi(fdbks[0]),GetFdbkType(atoi(fdbks[1])),RDate(fdbks[3]),true);
	}
	catch(RDbException& e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObj(GProfile* &profile,const R::RString& name)
{
	try
	{
		// Load Profile
		RQuery Profile(Db,"SELECT profileid,description,social,userid,attached,communityid,updated,calculated,blockid,score,level,profiletype "
		                  "FROM profiles WHERE description="+RQuery::SQLValue(name));
		Profile.Start();
		if(Profile.End())
		{
			profile=0;
			return;
		}
		GUser* user=Session->GetObj(pUser,Profile[3].ToSizeT());
		if(!user)
			throw GException("Profile "+Profile[0]+" has no parent user");
		size_t groupid=Profile[5].ToSizeT();

		// Create the profile
		profile=new GProfile(Session,user,GetProfileType(Profile[11].ToUInt()),Profile[0].ToSizeT(),Profile[8].ToSizeT(),Profile[1],groupid,
				GetMySQLToDate(Profile[4]),GetMySQLToDate(Profile[6]),GetMySQLToDate(Profile[7]),
				Profile[2].ToBool(true),Profile[9].ToDouble(),Profile[10].ToChar(),5);

		// Load Feedbacks
		RQuery fdbks(Db,"SELECT docid,fdbk,profileid,done "
		                "FROM docsbyprofiles WHERE profileid="+Num(profile->GetId()));
		for(fdbks.Start();!fdbks.End();fdbks.Next())
			Session->InsertFdbk(atoi(fdbks[2]),atoi(fdbks[0]),GetFdbkType(atoi(fdbks[1])),RDate(fdbks[3]),true);
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
		RQuery Up(Db,"UPDATE docsbyprofiles,profiles SET profiles.updated=CURDATE() WHERE profiles.profileid=docsbyprofiles.profileid AND docsbyprofiles.docid="+Num(docid));
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
void GStorageMySQL::SaveObj(GUser* user)
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
void GStorageMySQL::SaveObj(GProfile* prof)
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
			sSql="INSERT INTO profiles(profileid,description,social,userid,updated,calculated,attached,blockid,score,level,profiletype";
			sSql+=") VALUES("+Num(profid)+","+RQuery::SQLValue(prof->GetName())+","+
			      Num(social)+","+Num(prof->GetUser()->GetId())+","+
			      RQuery::SQLValue(prof->GetUpdated())+","+RQuery::SQLValue(prof->GetComputed())+","+RQuery::SQLValue(prof->GetAttached())+
			      ","+Num(prof->GetBlockId())+","+Num(prof->GetConfidenceScore())+","+Num(prof->GetConfidenceLevel())+","+
			      Num(prof->GetProfileType())+")";
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
			sSql+=" WHERE profileid="+Num(profid);
			RQuery Update(Db,sSql);
		}

		// Delete all the feedbacks of this profile
		RQuery Delete(Db,"DELETE FROM docsbyprofiles WHERE profileid="+Num(profid));

		// First insert information from GFdbk
		RCursor<GFdbk> Fdbks=prof->GetFdbks();
		for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
		{
			sSql="INSERT INTO docsbyprofiles(docid,fdbk,profileid,done) "
			     "VALUES("+Num(Fdbks()->GetDocId())+",'"+RString::Number(Fdbks()->GetFdbk())+"',"+
			     Num(prof->GetId())+","+RQuery::SQLValue(Fdbks()->GetDone())+")";
			RQuery Insert(Db,sSql);
		}

		// Update other information from the documents
		RQuery Update(Db,"UPDATE docs,docsbyprofiles SET "
				         "docsbyprofiles.langid=docs.langid,"
				         "docsbyprofiles.calculated=docs.calculated,"
				         "docsbyprofiles.updated=docs.updated "
		                 "WHERE docsbyprofiles.docid=docs.docid AND profileid="+Num(prof->GetId()));
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::UpdateFdbk(GProfile* prof,GDoc* doc,tFdbkType fdbk,R::RDate done)
{
	try
	{
		RString sSql;

		// Delete first the existing feedback
		sSql="DELETE FROM docsbyprofiles WHERE docid="+Num(doc->GetId())+" AND profileid="+Num(prof->GetId());
		RQuery Del(Db,sSql);

		// Insert it.
		RString Lang;
		if(doc->GetLang())
			Lang=doc->GetLang()->GetCode();
		else
			Lang="00";
		sSql="INSERT INTO docsbyprofiles(docid,fdbk,profileid,done,langid,calculated,updated) "
	     "VALUES("+Num(doc->GetId())+","+Num(fdbk)+","+Num(prof->GetId())+","+RQuery::SQLValue(done)+","
			 +RQuery::SQLValue(Lang)+","+RQuery::SQLValue(doc->GetComputed())+","+RQuery::SQLValue(doc->GetUpdated())+")";
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
void GStorageMySQL::LoadObjs(const GCommunity*)
{
	GCommunity* group;
	R::RCursor<GCommunity> GroupsCursor;

	try
	{
		Session->VerifyTab(otCommunity,GetCount("communities"));
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
			group=new GCommunity(Session,Groups[0].ToSizeT(),Groups[4].ToSizeT(),Groups[3],Groups[1],Groups[2]);
			Session->InsertObj(group);
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObj(GCommunity* &community,size_t groupid)
{
	try
	{
		RQuery Group(Db,"SELECT communityid,updated,calculated,name,blockid FROM communities WHERE communityid="+RString::Number(groupid));
		Group.Start();
		if(Group.End())
		{
			community=0;
			return;
		}

		community=new GCommunity(Session,Group[0].ToSizeT(),Group[4].ToSizeT(),Group[3],Group[1],Group[2]);
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
void GStorageMySQL::SaveObj(GCommunity* grp)
{
	RString sSql;
	RCursor<GProfile> Sub;

	try
	{
		// Verify if the topic already exist
		sSql="SELECT communityid FROM communities WHERE communityid="+Num(grp->GetId());
		RQuery find(Db,sSql);
		find.Start();
		if(!find.End())
		{
			sSql="UPDATE communities SET "
				"updated="+RQuery::SQLValue(grp->GetUpdated())+","+
				"calculated="+RQuery::SQLValue(grp->GetComputed())+","+
				"name="+RQuery::SQLValue(grp->GetName())+","+
				"blockid="+Num(grp->GetBlockId());
			sSql+=" WHERE communityid="+Num(grp->GetId());
			RQuery update(Db,sSql);
		}
		else
		{
			sSql="INSERT INTO communities(communityid,updated,calculated,name,blockid) "
				  "VALUES("+Num(grp->GetId())+","+
				  RQuery::SQLValue(grp->GetUpdated())+","+
				  RQuery::SQLValue(grp->GetComputed())+","+
				  RQuery::SQLValue(grp->GetName())+","+
				  Num(grp->GetBlockId())+")";
			RQuery insert1(Db,sSql);
		}

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
	RCursor<GDocFragmentRank> Doc(docs);
	for(Doc.Start();!Doc.End();Doc.Next())
		RQuery Ins(Db,"INSERT INTO docsbycommunities(communityid,docid,ranking) VALUES("+
			Num(docs.GetCommunityId())+","+Num(Doc()->GetFragment()->GetDoc()->GetId())+","+Num(Doc()->GetRanking())+")");
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadDocs(GCommunityDocs& docs)
{
	RQuery Get(Db,"SELECT docid,ranking FROM docsbycommunities WHERE communityid="+Num(docs.GetCommunityId()));
	for(Get.Start();!Get.End();Get.Next())
	{
		GDoc* Doc(Session->GetObj(pDoc,Get[0].ToSizeT()));
		bool Exist;
		docs.InsertPtr(new GDocFragmentRank(new GDocFragment(Doc,0,0),Get[1].ToDouble(),"",true));
	}
}



//------------------------------------------------------------------------------
//
// Topics Methods
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GStorageMySQL::LoadObjs(const GTopic*)
{
	GTopic* group;
	R::RCursor<GTopic> GroupsCursor;

	try
	{
		Session->VerifyTab(otTopic,GetCount("topics"));
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
			group=new GTopic(Session,Groups[0].ToSizeT(),Groups[4].ToSizeT(),Groups[3],Groups[1],Groups[2]);
			Session->InsertObj(group);
		}
	}
	catch(RDbException e)
	{
		cerr<<e.GetMsg()<<endl;
		throw GException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
void GStorageMySQL::LoadObj(GTopic* &topic,size_t topicid)
{
	try
	{
		RQuery Group(Db,"SELECT topicid,updated,calculated,name,blockid FROM topics WHERE topicid="+RString::Number(topicid));
		Group.Start();
		if(Group.End())
		{
			topic=0;
			return;
		}
		topic=new GTopic(Session,Group[0].ToSizeT(),Group[4].ToSizeT(),Group[3],Group[1],Group[2]);
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
		// Verify if the topic already exist
		RString sSql="SELECT topicid FROM topics WHERE name="+RQuery::SQLValue(grp->GetName());
		RQuery find(Db,sSql);
		find.Start();
		if(!find.End())
		{
			grp->SetId(find[0].ToSizeT());
			return;
		}

		// Insert a new one
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
void GStorageMySQL::SaveObj(GTopic* grp)
{
	RString sSql;
	RCursor<GDoc> Sub;

	try
	{
		// Verify if the topic already exist
		sSql="SELECT topicid FROM topics WHERE topicid="+Num(grp->GetId());
		RQuery find(Db,sSql);
		find.Start();
		if(!find.End())
		{
			sSql="UPDATE topics SET "
				"updated="+RQuery::SQLValue(grp->GetUpdated())+","+
				"calculated="+RQuery::SQLValue(grp->GetComputed())+","+
				"name="+RQuery::SQLValue(grp->GetName())+","+
				"blockid="+Num(grp->GetBlockId());
			sSql+=" WHERE topicid="+Num(grp->GetId());
			RQuery update(Db,sSql);
		}
		else
		{
			sSql="INSERT INTO topics(topicid,updated,calculated,name,blockid) "
				"VALUES("+Num(grp->GetId())+","+
				RQuery::SQLValue(grp->GetUpdated())+","+
				RQuery::SQLValue(grp->GetComputed())+","+
				RQuery::SQLValue(grp->GetName())+","+
				Num(grp->GetBlockId())+")";
			RQuery insert(Db,sSql);
		}

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
// Suggestions Methods
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
			mThrowGException("Invalid list of suggestions");
	}
	RQuery Del(Db,"DELETE FROM "+Table+" WHERE "+Id+"="+Num(sugs.GetAddresseeId()));
	RString InsSql("INSERT INTO "+Table+"("+Id+",docid,ranking,proposed,info) VALUES("+Num(sugs.GetAddresseeId())+",");
	RCursor<GDocFragmentRank> Cur(sugs,0,max);
	for(Cur.Start();!Cur.End();Cur.Next())
		RQuery Ins(Db,InsSql+Num(Cur()->GetFragment()->GetDoc()->GetId())+","+Num(Cur()->GetRanking())+","+RQuery::SQLValue(Cur()->GetFragment()->GetProposed())+RQuery::SQLValue(Cur()->GetInfo())+")");
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
			mThrowGException("Invalid list of suggestions");
	}

	try
	{
		RQuery Load(Db,"SELECT docid,ranking,proposed,info FROM "+Table+" WHERE "+Id+"="+Num(sugs.GetAddresseeId())+" ORDER BY "+Id+",ranking");
		for(Load.Start();!Load.End();Load.Next())
		{
			GDoc* Doc(Session->GetObj(pDoc,Load[0].ToSizeT()));
			sugs.InsertPtr(new GDocFragmentRank(new GDocFragment(Doc,0,0,Load[2]),Load[1].ToDouble(),Load[3],true));
		}
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
void GStorageMySQL::CreateConfig(void)
{
	InsertParam(new RParamValue("Host","127.0.0.1"));
	InsertParam(new RParamValue("User","root"));
	InsertParam(new RParamValue("Password",""));
	InsertParam(new RParamValue("Database",""));
	InsertParam(new RParamValue("Filtering",false));
	InsertParam(new RParamValue("Filter",""));
	InsertParam(new RParamValue("Encoding","utf8"));
	InsertParam(new RParamValue("All",true));
}


//------------------------------------------------------------------------------
GStorageMySQL::~GStorageMySQL(void)
{
	delete Db;
}


//------------------------------------------------------------------------------
CREATE_STORAGE_FACTORY("MySQL","MySQL",GStorageMySQL)
