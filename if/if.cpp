/*

	GALILEI Research Project

	If.cpp

	Tf/Idf Feature Weighting Method - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).

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
#include <math.h>



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>
#include <gtopic.h>
#include <gclass.h>
#include <gprofile.h>
#include <gcommunity.h>
#include <gsession.h>
#include <if.h>



//------------------------------------------------------------------------------
//
//  IfData
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
IfData::IfData(GSession* session,If* calc)
	: GDescriptionSetData(session),
	  IF(session->GetMaxObjId(pConcept)+1),
	  Refs(session->GetMaxObjId(pConceptType)+1),
	  Session(session),
	  Calc(calc)
{
}


//------------------------------------------------------------------------------
void IfData::Compute(GDescriptionSet* set)
{
	// Reinitialize the vector
	IF.ReSize(Session->GetMaxObjId(pConcept)+1,0.0);
	Refs.ReSize(Session->GetMaxObjId(pConceptType)+1,0.0);

	// Go through the descriptions
	RCursor<const GDescription> Desc(set->GetDescriptions());
	for(Desc.Start();!Desc.End();Desc.Next())
	{
		RConstCursor<GVector> vectors(Desc()->GetVectors());
		if(!vectors.GetNb()) continue;

		// Init
		Calc->tmpConcepts.Init(Session->GetMaxObjId(pConcept)+1,true);
		Calc->tmpTypes.Init(Session->GetMaxObjId(pConceptType)+1,true);

		// Parse the vectors
		for(vectors.Start();!vectors.End();vectors.Next())
		{
			RConstCursor<GConceptRef> Vector(vectors()->GetRefs());
			if(!Vector.GetNb())
				continue;
			Calc->Alter(vectors()->GetMetaConcept(),true,If::tUnknown,&IF,&Refs,0);
			for(Vector.Start();!Vector.End();Vector.Next())
				Calc->Alter(Vector()->GetConcept(),true,If::tUnknown,&IF,&Refs,0);
		}
	}

	// Compute the If
	RNumCursor<double> Cur(IF.GetCols());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()!=0.0)
			Cur()=log10(Refs[Session->GetObj(pConcept,Cur.GetPos())->GetType()->GetId()]/Cur());
	}

	Dirty=false;
}



//------------------------------------------------------------------------------
//
//  If
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
If::If(GSession* session,GPlugInFactory* fac)
	: RObject(fac->GetMng()->GetName()+"|"+fac->GetList()+"|"+fac->GetName()),
	  GMeasure(session,fac),
	  ConceptRef(5,0),
	  ConceptIf(5,0),
	  ConceptTypeRef(5,0),
	  tmpTypes(0),
	  tmpConcepts(0)
{
	// Document notifications
	InsertObserver(HANDLER(If::HandleAddDoc),hDocs[oeSelected]);
	InsertObserver(HANDLER(If::HandleAddDoc),hDocs[oeUpdated]);
	InsertObserver(HANDLER(If::HandleDelDoc),hDocs[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(If::HandleDelDoc),hDocs[oeDeselected]);
	InsertObserver(HANDLER(If::HandleDelDoc),hDocs[oeAboutToBeDeleted]);

	// Topic notifications
	InsertObserver(HANDLER(If::HandleAddTopic),hTopics[oeUpdated]);
	InsertObserver(HANDLER(If::HandleDelTopic),hTopics[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(If::HandleDelTopic),hTopics[oeAboutToBeDeleted]);

	// Class notifications
	InsertObserver(HANDLER(If::HandleAddClass),hClasses[oeUpdated]);
	InsertObserver(HANDLER(If::HandleDelClass),hClasses[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(If::HandleDelClass),hClasses[oeAboutToBeDeleted]);

	// Profile notifications
	InsertObserver(HANDLER(If::HandleAddProfile),hProfiles[oeSelected]);
	InsertObserver(HANDLER(If::HandleAddProfile),hProfiles[oeUpdated]);
	InsertObserver(HANDLER(If::HandleDelProfile),hProfiles[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(If::HandleDelProfile),hProfiles[oeDeselected]);
	InsertObserver(HANDLER(If::HandleDelProfile),hProfiles[oeAboutToBeDeleted]);

	// Community notifications
	InsertObserver(HANDLER(If::HandleAddCommunity),hCommunities[oeUpdated]);
	InsertObserver(HANDLER(If::HandleDelCommunity),hCommunities[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(If::HandleDelCommunity),hCommunities[oeAboutToBeDeleted]);

	// Session notifications
	InsertObserver(HANDLER(If::HandleReInit),hReInit,session);

	// Init matrices
	ConceptRef.Init(0.0);
	ConceptIf.Init(NAN);
	ConceptTypeRef.Init(0.0);
}


//------------------------------------------------------------------------------
void If::Init(void)
{
	RString Dir(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Session->GetName()+RFile::GetDirSeparator()+GMeasure::GetName()+RFile::GetDirSeparator());
	RDir::CreateDirIfNecessary(Dir,true);
	ConceptsFile.Open(Dir+"conceptrefs",RGenericMatrix::tNormal);
	ConceptTypesFile.Open(Dir+"concepttyperefs",RGenericMatrix::tNormal);

	ConceptsFile.Load(ConceptRef);
	ConceptTypesFile.Load(ConceptTypeRef);
	ConceptIf.VerifySize(ConceptRef.GetNbLines(),ConceptRef.GetNbCols(),true,NAN);
}


//------------------------------------------------------------------------------
void If::Done(void)
{
	if(Session->MustSaveResults())
	{
		ConceptsFile.Save(ConceptRef);
		ConceptTypesFile.Save(ConceptTypeRef);
	}
	ConceptsFile.Close();
	ConceptTypesFile.Close();
}


//------------------------------------------------------------------------------
void If::Alter(const GConcept* concept,bool add,eType idx,RVector* ref,RVector* typeref,RVector* factors)
{
	// Verify if the reference of the concept type must be altered
   // This must be done only once per description added or removed
	if(!concept)
		mThrowGException("Null concept not allowed");

	GConceptType* type(concept->GetType());
	size_t TypeId(type->GetId());
	if(tmpTypes[TypeId])
	{
		// Yes -> It is the first time current description uses this concept type.
		if(add)
			(*typeref)[TypeId]++;
		else
			(*typeref)[TypeId]--;
		if(Session->MustSaveResults()&&(idx!=tUnknown))
			ConceptTypesFile.Write(idx,TypeId,ConceptTypeRef(idx,TypeId));
		tmpTypes[TypeId]=false;
	}

	// Verify if the reference of the concept must be altered
   // This must be done only once per description added or removed
	size_t ConceptId(concept->GetId());
	if(tmpConcepts[ConceptId])
	{
		// Yes -> It is the first time current description uses this concept.
		if(add)
			(*ref)[ConceptId]++;
		else
			(*ref)[ConceptId]--;
		if(factors)
			(*factors)[ConceptId]=NAN;
		if(Session->MustSaveResults()&&(idx!=tUnknown))
			ConceptsFile.Write(idx,ConceptId,ConceptRef(idx,ConceptId));
		tmpConcepts[ConceptId]=false;
	}
}


//------------------------------------------------------------------------------
void If::Add(R::RConstCursor<GVector> vectors,eType idx)
{
	// Verify that there is something to do
	if(!vectors.GetNb()) return;

	// Verify the sizes
	ConceptRef.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptIf.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,NAN);
	ConceptTypeRef.VerifySize(5,Session->GetMaxObjId(pConceptType)+1,true,0.0);
	tmpTypes.Init(Session->GetMaxObjId(pConceptType)+1,true);
	tmpConcepts.Init(Session->GetMaxObjId(pConcept)+1,true);
	ConceptsFile.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptTypesFile.VerifySize(5,Session->GetMaxObjId(pConceptType)+1,true,0.0);

	// Parse the vectors
	for(vectors.Start();!vectors.End();vectors.Next())
	{
		RConstCursor<GConceptRef> Vector(vectors()->GetRefs());
		if(!Vector.GetNb())
			continue;
		Alter(vectors()->GetMetaConcept(),true,idx,ConceptRef[idx],ConceptTypeRef[idx],ConceptIf[idx]);
		for(Vector.Start();!Vector.End();Vector.Next())
			Alter(Vector()->GetConcept(),true,idx,ConceptRef[idx],ConceptTypeRef[idx],ConceptIf[idx]);
	}
}


//------------------------------------------------------------------------------
void If::Del(R::RConstCursor<GVector> vectors,eType idx)
{
	// Verify that there is something to do
	if(!vectors.GetNb()) return;

	// Verify the sizes
	ConceptRef.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptIf.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,NAN);
	ConceptTypeRef.VerifySize(5,Session->GetMaxObjId(pConceptType)+1,true,0.0);
	tmpTypes.Init(Session->GetMaxObjId(pConceptType)+1,true);
	tmpConcepts.Init(Session->GetMaxObjId(pConcept)+1,true);
	ConceptsFile.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptTypesFile.VerifySize(5,Session->GetMaxObjId(pConceptType)+1,true,0.0);

	// Parse the vectors
	for(vectors.Start();!vectors.End();vectors.Next())
	{
		RConstCursor<GConceptRef> Vector(vectors()->GetRefs());
		if(!Vector.GetNb())
			continue;
		Alter(vectors()->GetMetaConcept(),false,idx,ConceptRef[idx],ConceptTypeRef[idx],ConceptIf[idx]);
		for(Vector.Start();!Vector.End();Vector.Next())
			Alter(Vector()->GetConcept(),false,idx,ConceptRef[idx],ConceptTypeRef[idx],ConceptIf[idx]);
	}
}


//------------------------------------------------------------------------------
double If::GetIf(const GConcept* concept,eType idx)
{
	if(!concept)
		mThrowGException("Null concept not allowed");
	if(concept->GetId()>=ConceptIf.GetNbCols())
		return(0.0);
	double& Val(ConceptIf(idx,concept->GetId()));
	if(Val!=Val)
	{
		double TypeRef(ConceptTypeRef(idx,concept->GetType()->GetId()));
		double Ref(ConceptRef(idx,concept->GetId()));
		if(TypeRef&&Ref)
			Val=log10(TypeRef/Ref);
		else
			Val=0.0;
	}
	return(Val);
}


//------------------------------------------------------------------------------
void If::ComputeTfIdf(GDescription* desc,eType idx)
{
	if(!desc)
		mThrowGException("Null description not allowed");

	RCursor<GVector> Vector(desc->GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		double Max(std::numeric_limits<double>::min());
		RCursor<GConceptRef> ptr(Vector()->GetRefs());
		for(ptr.Start();!ptr.End();ptr.Next())
		{
			if(ptr()->GetWeight()>Max)
				Max=ptr()->GetWeight();
			(*ptr())*=GetIf(ptr()->GetConcept(),idx);
		}
		(*Vector())/=Max;
	}
}


//------------------------------------------------------------------------------
void If::ComputeTfIdf(GDescription* desc,GDescriptionSet* set,eType)
{
	if(!desc)
		mThrowGException("Null description not allowed");

	// If the description set is dirty -> recompute
	if(!set->GetData())
		set->SetData(new IfData(Session,this));
	IfData* Data(dynamic_cast<IfData*>(set->GetData()));
	if(Data->IsDirty())
		Data->Compute(set);

	// Compute the Tf/Idf
	RCursor<GVector> Vector(desc->GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		double Max(std::numeric_limits<double>::min());
		RCursor<GConceptRef> ptr(Vector()->GetRefs());
		for(ptr.Start();!ptr.End();ptr.Next())
		{
			if(ptr()->GetWeight()>Max)
				Max=ptr()->GetWeight();
			(*ptr())*=Data->IF[ptr()->GetConcept()->GetId()];
		}
		(*Vector())/=Max;
	}
}


//------------------------------------------------------------------------------
void If::Measure(size_t measure,...)
{
	// Get the parameters
	va_list ap;
	va_start(ap,measure);
	GConcept* Concept;
	GDescription* Desc;
	GDescriptionSet* Set;
	double* res;
	switch(measure)
	{
		case 0:
			Concept=va_arg(ap,GConcept*);
			break;
		case 1:
			Desc=va_arg(ap,GDescription*);
			break;
		case 2:
			Desc=va_arg(ap,GDescription*);
			break;
		case 3:
			Concept=va_arg(ap,GConcept*);
			break;
		default:
			mThrowGException(RString::Number(measure)+" is not allowed as measure.");
	}
	tObjType type(static_cast<tObjType>(va_arg(ap,int)));
	switch(measure)
	{
		case 0:
			res=va_arg(ap,double*);
			break;
		case 2:
			Set=va_arg(ap,GDescriptionSet*);
			break;
		case 3:
			res=va_arg(ap,double*);
			break;
	}
	va_end(ap);

	// Get the index corresponding to the type
	eType idx;
	switch(type)
	{
		case otDoc:
			idx=tDoc;
			break;
		case otTopic:
			idx=tTopic;
			break;
		case otClass:
			idx=tClass;
			break;
		case otProfile:
			idx=tProfile;
			break;
		case otCommunity:
			idx=tCommunity;
			break;
		default:
			mThrowGException(GetObjType(type,true,false)+" is not allowed for the tf/idf.");
	}

	// Compute the measure
	switch(measure)
	{
		case 0:
			(*res)=GetIf(Concept,idx);
			break;
		case 1:
			ComputeTfIdf(Desc,idx);
			break;
		case 2:
			ComputeTfIdf(Desc,Set,idx);
			break;
		case 3:
			(*res)=ConceptRef(idx,Concept->GetId());
			break;
	}
}


//------------------------------------------------------------------------------
void If::Info(size_t info,...)
{
	va_list ap;
	va_start(ap,info);
	RString* res(va_arg(ap,RString*));
	va_end(ap);
	if(info!=0)
		mThrowGException(RString::Number(info)+" is not allowed as information.");
	(*res)="tf/idf";
}


//------------------------------------------------------------------------------
void If::HandleAddDoc(const RNotification& notification)
{
	// Verify that the sender is a document and that it is selected
	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
	if(Doc&&Session->IsUsed(Doc))
		Add(Doc->GetVectors(),tDoc);
}


//------------------------------------------------------------------------------
void If::HandleDelDoc(const RNotification& notification)
{
	// Verify that the sender is a document and that it is selected
	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
	if(Doc&&Session->IsUsed(Doc))
		Del(Doc->GetVectors(),tDoc);
}


//------------------------------------------------------------------------------
void If::HandleAddTopic(const RNotification& notification)
{
	// Verify that the sender is a topic
	GTopic* Topic(dynamic_cast<GTopic*>(notification.GetSender()));
	if(Topic)
		Add(Topic->GetVectors(),tTopic);
}


//------------------------------------------------------------------------------
void If::HandleDelTopic(const RNotification& notification)
{
	// Verify that the sender is a topic
	GTopic* Topic(dynamic_cast<GTopic*>(notification.GetSender()));
	if(Topic)
		Del(Topic->GetVectors(),tTopic);
}


//------------------------------------------------------------------------------
void If::HandleAddClass(const RNotification& notification)
{
	// Verify that the sender is a class
	GClass* Class(dynamic_cast<GClass*>(notification.GetSender()));
	if(Class)
		Add(Class->GetVectors(),tClass);
}


//------------------------------------------------------------------------------
void If::HandleDelClass(const RNotification& notification)
{
	// Verify that the sender is a class
	GClass* Class(dynamic_cast<GClass*>(notification.GetSender()));
	if(Class)
		Del(Class->GetVectors(),tClass);
}


//------------------------------------------------------------------------------
void If::HandleAddProfile(const RNotification& notification)
{
	// Verify that the sender is a profile
	GProfile* Profile(dynamic_cast<GProfile*>(notification.GetSender()));
	if(Profile)
		Add(Profile->GetVectors(),tProfile);
}


//------------------------------------------------------------------------------
void If::HandleDelProfile(const RNotification& notification)
{
	// Verify that the sender is a profile
	GProfile* Profile(dynamic_cast<GProfile*>(notification.GetSender()));
	if(Profile)
		Del(Profile->GetVectors(),tProfile);
}


//------------------------------------------------------------------------------
void If::HandleAddCommunity(const RNotification& notification)
{
	// Verify that the sender is a community
	GCommunity* Community(dynamic_cast<GCommunity*>(notification.GetSender()));
	if(Community)
		Add(Community->GetVectors(),tCommunity);
}


//------------------------------------------------------------------------------
void If::HandleDelCommunity(const RNotification& notification)
{
	// Verify that the sender is a community
	GCommunity* Community(dynamic_cast<GCommunity*>(notification.GetSender()));
	if(Community)
		Del(Community->GetVectors(),tCommunity);
}


//------------------------------------------------------------------------------
void If::HandleReInit(const R::RNotification& notification)
{
	// Manage only documents
	GSessionMsg& Msg(GetData<GSessionMsg&>(notification));
	switch(Msg.GetType())
	{
		case otDoc:
			ReInit(tDoc);
			break;
		case otTopic:
			ReInit(tTopic);
			break;
		case otClass:
			ReInit(tClass);
			break;
		case otProfile:
			ReInit(tProfile);
			break;
		case otCommunity:
			ReInit(tCommunity);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void If::ReInit(If::eType type)
{
	if(ConceptRef.GetNbLines()>type)
	{
		ConceptRef[type]->Init(0.0);
		ConceptIf[type]->Init(NAN);
		ConceptTypeRef[type]->Init(0.0);
	}
	if(Session->MustSaveResults()&&(ConceptsFile.GetNbLines()>type))
	{
		ConceptsFile.InitLine(type,0.0);
		ConceptTypesFile.InitLine(type,0.0);
	}
}

//------------------------------------------------------------------------------
template<class cObj>
	void If::ComputeIF(const cObj* obj,If::eType type)
{
	RCursor<cObj> Obj(Session->GetObjs(obj));
	for(Obj.Start();!Obj.End();Obj.Next())
		Add(Obj()->GetVectors(),type);
}


//------------------------------------------------------------------------------
void If::Reset(void)
{
	ConceptRef.Init(0.0);
	ConceptIf.Init(NAN);
	ConceptTypeRef.Init(0.0);
	if(Session->MustSaveResults())
	{
		ConceptsFile.Init(0.0);
		ConceptTypesFile.Init(0.0);
	}
	ComputeIF(pDoc,tDoc);
	ComputeIF(pTopic,tTopic);
	ComputeIF(pClass,tClass);
	ComputeIF(pProfile,tProfile);
	ComputeIF(pCommunity,tCommunity);
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Features Evaluation","tfidf","tf/idf",If)
