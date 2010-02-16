/*

	GALILEI Research Project

	ClassesEval.cpp

	Classes Evaluation - Implementation.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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
#include <cmath>


//-----------------------------------------------------------------------------
// include files for R Project
#include <robject.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gclass.h>
#include <gsubject.h>
#include <gsubjects.h>
#include <gsession.h>
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
class ClassSubject
{
public:
	GClass* Class;
	GSubject* Subject;

	ClassSubject(GClass* theclass,GSubject* subject) : Class(theclass), Subject(subject) {}
	int Compare(const ClassSubject&) const {return(-1);}
};



//------------------------------------------------------------------------------
//
//  ClassesEval
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class ClassesEval : public R::RObject, public GMeasure
{
	bool DirtyRankTree;
	double RankTree;
	RContainer<ClassSubject,true,false> LeafNodes;

public:
	ClassesEval(GPlugInFactory* fac);
	virtual R::RCString GetClassName(void) const {return("ClassesEval");}
	void Handle(const R::RNotification& notification);

	void Dirty(void);

	/**
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session);

	/**
	* This methods returns zero.
	* @param info        Unused parameter (but to specify).
	*
	* The class supposes that one more parameter is passed:
	* - A pointer to a variable of type double that will contain null.
	*/
	virtual void Measure(size_t measure,...);

	/**
	* Access to several information related to the clustering evaluation. If
	* necessary, the measures are updated.
	* @param info        Type of the information to take. One value is
	*                    accepted:
	*                    - case '0': The RankTree.
	*
	* The class supposes that one more parameter is passed:
	* - A pointer to a variable of type double that will contain the result.
	*/
	virtual void Info(size_t measure,...);

	void ComputeRankTree(void);
	static void CreateParams(R::RConfig*) {}
};


//------------------------------------------------------------------------------
ClassesEval::ClassesEval(GPlugInFactory* fac)
	: GMeasure(fac), LeafNodes(3000)
{
	InsertObserver(HANDLER(ClassesEval::Handle),"ObjectChanged");
}


//------------------------------------------------------------------------------
void ClassesEval::Handle(const RNotification& notification)
{
	GEvent& Event(GetData<GEvent&>(notification));
    if(otClass==Event.Object->GetObjType())
    	Dirty();
}


//------------------------------------------------------------------------------
void ClassesEval::Dirty(void)
{
	DirtyRankTree=true;
}


//------------------------------------------------------------------------------
void ClassesEval::Connect(GSession* session)
{
	GMeasure::Connect(session);
	Dirty();
}


//------------------------------------------------------------------------------
void ClassesEval::Measure(size_t measure,...)
{
	va_list ap;
	va_start(ap,measure);
	double* res(va_arg(ap,double*));
	va_end(ap);
	(*res)=0.0;
}


//------------------------------------------------------------------------------
void ClassesEval::Info(size_t measure,...)
{
	va_list ap;
	va_start(ap,measure);
	double* res(va_arg(ap,double*));
	va_end(ap);

	switch(measure)
	{
		case 0:
			if(DirtyRankTree)
				ComputeRankTree();
			(*res)=RankTree;
			break;
		default:
			ThrowGException("Only 0 is allowed as measure");
	};
}


//------------------------------------------------------------------------------
void ClassesEval::ComputeRankTree(void)
{
	// Look at all leaf class and the corresponding subject
	LeafNodes.Clear();
	RCursor<GClass> Classes(Session->GetClasses());
	for(Classes.Start();!Classes.End();Classes.Next())
	{
		if(Classes()->GetNbNodes()) continue;
		RString Name(Classes()->GetName());
		int Pos(Name.Find('('));
		RString SubjectName;
		if(Pos!=-1)
			SubjectName=Name.Mid(Pos+1,Name.GetLen()-Pos-2);
		else
			SubjectName=Name;
		GSubject* Subject(Session->GetSubject(SubjectName));
		if(!Subject)
			ThrowGException("No correspondence between the class '"+SubjectName+"' and a subject");
		LeafNodes.InsertPtr(new ClassSubject(Classes(),Subject));
	}
	cout<<LeafNodes.GetNb()<<" leaf nodes to compare"<<endl;

	// Compute the RankTree
	double num(0.0), den(0.0);
	RCursor<ClassSubject> u(LeafNodes);
	RCursor<ClassSubject> v(LeafNodes);
	for(u.Start();!u.End();u.Next())
	{
		for(v.Start();!v.End();v.Next())
		{
			if(u()==v())
				continue;
			double Up(Session->GetUpOperationsCost(u()->Subject,v()->Subject));
			double Ud(Session->GetClassTree()->GetUpOperationsCost(u()->Class,v()->Class));
			num+=abs(Up-Ud);
			den+=Up;
		}
	}
	cout<<"num="<<num<<" ; den="<<den<<endl;
	RankTree=1.0-num/den;
	cout<<"RankTree="<<RankTree<<endl;
	DirtyRankTree=false;
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Classes Evaluation","TreeRank","TreeRank",ClassesEval)
