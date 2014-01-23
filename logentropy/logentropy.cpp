/*

	GALILEI Research Project

	LogEntropy.cpp

	Log Entropy Feature Weighting Method - Implementation.

	Copyright 2013-2014 by Pascal Francq (pascal@francq.info).

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
#include <logentropy.h>



//------------------------------------------------------------------------------
//
//  SetData
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
LogEntropyData::LogEntropyData(GSession* session,LogEntropy* calc)
	: GDescriptionSetData(session),
	  Entropy(session->GetMaxObjId(pConcept)+1),
	  Sum(session->GetMaxObjId(pConcept)+1),
	  Refs(session->GetMaxObjId(pConceptType)+1),
	  Session(session),
	  Calc(calc)
{
}


//------------------------------------------------------------------------------
void LogEntropyData::Compute(GDescriptionSet* set)
{
	// Reinitialize the vector
	Entropy.ReSize(Session->GetMaxObjId(pConcept)+1,0.0);
	Sum.ReSize(Session->GetMaxObjId(pConcept)+1,0.0);
	Refs.ReSize(Session->GetMaxObjId(pConceptType)+1,0.0);

	// Go through the descriptions
	RCursor<const GDescription> Desc(set->GetDescriptions());
	for(Desc.Start();!Desc.End();Desc.Next())
	{
		RConstCursor<GVector> vectors(Desc()->GetVectors());
		if(!vectors.GetNb()) continue;

		// Init
		Calc->tmpTypes.Init(Session->GetMaxObjId(pConceptType)+1,true);

		// Parse the vectors
		for(vectors.Start();!vectors.End();vectors.Next())
		{
			RConstCursor<GConceptRef> Vector(vectors()->GetRefs());
			if(!Vector.GetNb())
				continue;
			GConceptRef Ref(vectors()->GetMetaConcept(),1.0);
			Calc->Alter(&Ref,true,LogEntropy::tUnknown,&Entropy,&Sum,&Refs,0);
			for(Vector.Start();!Vector.End();Vector.Next())
				Calc->Alter(Vector(),true,LogEntropy::tUnknown,&Entropy,&Sum,&Refs,0);
		}
	}

	// Compute the If
	RNumCursor<double> Cur(Entropy.GetCols());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()==0.0)
			continue;

		double TypeRef(Refs[Session->GetObj(pConcept,Cur.GetPos())->GetType()->GetId()]);
		double S(Sum[Cur.GetPos()]);
		Cur()=1+(((Cur()/S)-log10(S))/(log10(TypeRef+1)));;
	}

	Dirty=false;
}



//------------------------------------------------------------------------------
//
//  LogEntropy
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
LogEntropy::LogEntropy(GSession* session,GPlugInFactory* fac)
	: RObject(fac->GetMng()->GetName()+"|"+fac->GetList()+"|"+fac->GetName()),
	  GMeasure(session,fac),
	  ConceptProd(5,0),
	  ConceptSum(5,0),
	  ConceptEntropy(5,0),
	  ConceptTypeRef(5,0),
	  tmpTypes(0)
{
	// Document notifications
	InsertObserver(HANDLER(LogEntropy::HandleAddDoc),hDocs[oeSelected]);
	InsertObserver(HANDLER(LogEntropy::HandleAddDoc),hDocs[oeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelDoc),hDocs[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelDoc),hDocs[oeDeselected]);
	InsertObserver(HANDLER(LogEntropy::HandleDelDoc),hDocs[oeAboutToBeDeleted]);

	// Topic notifications
	InsertObserver(HANDLER(LogEntropy::HandleAddTopic),hTopics[oeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelTopic),hTopics[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelTopic),hTopics[oeAboutToBeDeleted]);

	// Class notifications
	InsertObserver(HANDLER(LogEntropy::HandleAddClass),hClasses[oeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelClass),hClasses[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelClass),hClasses[oeAboutToBeDeleted]);

	// Profile notifications
	InsertObserver(HANDLER(LogEntropy::HandleAddProfile),hProfiles[oeSelected]);
	InsertObserver(HANDLER(LogEntropy::HandleAddProfile),hProfiles[oeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelProfile),hProfiles[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelProfile),hProfiles[oeDeselected]);
	InsertObserver(HANDLER(LogEntropy::HandleDelProfile),hProfiles[oeAboutToBeDeleted]);

	// Community notifications
	InsertObserver(HANDLER(LogEntropy::HandleAddCommunity),hCommunities[oeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelCommunity),hCommunities[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(LogEntropy::HandleDelCommunity),hCommunities[oeAboutToBeDeleted]);

	// Session notifications
	InsertObserver(HANDLER(LogEntropy::HandleReInit),hReInit,session);

	// Init matrices
	ConceptProd.Init(0.0);
	ConceptSum.Init(0.0);
	ConceptEntropy.Init(NAN);
	ConceptTypeRef.Init(0.0);
}


//------------------------------------------------------------------------------
void LogEntropy::Init(void)
{
	RString Dir(GALILEIApp->GetIndexDir()+RFile::GetDirSeparator()+Session->GetName()+RFile::GetDirSeparator()+GMeasure::GetName()+RFile::GetDirSeparator());
	RDir::CreateDirIfNecessary(Dir,true);
	ConceptsProdFile.Open(Dir+"conceptprods",RGenericMatrix::tNormal);
	ConceptsSumFile.Open(Dir+"conceptsums",RGenericMatrix::tNormal);
	ConceptTypesFile.Open(Dir+"concepttyperefs",RGenericMatrix::tNormal);

	ConceptsProdFile.Load(ConceptProd);
	ConceptsSumFile.Load(ConceptSum);
	ConceptTypesFile.Load(ConceptTypeRef);
	ConceptEntropy.VerifySize(ConceptProd.GetNbLines(),ConceptProd.GetNbCols(),true,NAN);
}


//------------------------------------------------------------------------------
void LogEntropy::Done(void)
{
	if(Session->MustSaveResults())
	{
		ConceptsProdFile.Save(ConceptProd);
		ConceptsSumFile.Save(ConceptSum);
		ConceptTypesFile.Save(ConceptTypeRef);
	}
	ConceptsProdFile.Close();
	ConceptsSumFile.Close();
	ConceptTypesFile.Close();
}


//------------------------------------------------------------------------------
void LogEntropy::Alter(const GConceptRef* ref,bool add,eType idx,RVector* prod,RVector* sum,RVector* typeref,RVector* entropy)
{
	if(ref->GetWeight()==0.0)
		return;

	// Verify if the reference of the concept type must be altered
   // This must be done only once per description added or removed
	GConceptType* type(ref->GetConcept()->GetType());
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

	// Add the product of the frequence and its logarithm
	size_t ConceptId(ref->GetConcept()->GetId());
	if(add)
	{
		(*prod)[ConceptId]+=ref->GetWeight()*log10(ref->GetWeight());
		(*sum)[ConceptId]+=ref->GetWeight();
	}
	else
	{
		(*prod)[ConceptId]-=ref->GetWeight()*log10(ref->GetWeight());
		(*sum)[ConceptId]-=ref->GetWeight();
	}
	if(entropy)
		(*entropy)[ConceptId]=NAN;
	if(Session->MustSaveResults()&&(idx!=tUnknown))
	{
		ConceptsProdFile.Write(idx,ConceptId,ConceptProd(idx,ConceptId));
		ConceptsSumFile.Write(idx,ConceptId,ConceptSum(idx,ConceptId));
	}
}


//------------------------------------------------------------------------------
void LogEntropy::Add(R::RConstCursor<GVector> vectors,eType idx)
{
	// Verify that there is something to do
	if(!vectors.GetNb()) return;

	// Verify the sizes
	ConceptProd.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptSum.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptEntropy.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,NAN);
	ConceptTypeRef.VerifySize(5,Session->GetMaxObjId(pConceptType)+1,true,0.0);
	tmpTypes.Init(Session->GetMaxObjId(pConceptType)+1,true);
	ConceptsProdFile.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptsSumFile.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptTypesFile.VerifySize(5,Session->GetMaxObjId(pConceptType)+1,true,0.0);

	// Parse the vectors
	for(vectors.Start();!vectors.End();vectors.Next())
	{
		RConstCursor<GConceptRef> Vector(vectors()->GetRefs());
		if(!Vector.GetNb())
			continue;
		GConceptRef Ref(vectors()->GetMetaConcept(),1.0);
		Alter(&Ref,true,idx,ConceptProd[idx],ConceptSum[idx],ConceptTypeRef[idx],ConceptEntropy[idx]);
		for(Vector.Start();!Vector.End();Vector.Next())
			Alter(Vector(),true,idx,ConceptProd[idx],ConceptSum[idx],ConceptTypeRef[idx],ConceptEntropy[idx]);
	}
}


//------------------------------------------------------------------------------
void LogEntropy::Del(R::RConstCursor<GVector> vectors,eType idx)
{
	// Verify that there is something to do
	if(!vectors.GetNb()) return;

	// Verify the sizes
	ConceptProd.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptSum.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptEntropy.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,NAN);
	ConceptTypeRef.VerifySize(5,Session->GetMaxObjId(pConceptType)+1,true,0.0);
	tmpTypes.Init(Session->GetMaxObjId(pConceptType)+1,true);
	ConceptsProdFile.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptsSumFile.VerifySize(5,Session->GetMaxObjId(pConcept)+1,true,0.0);
	ConceptTypesFile.VerifySize(5,Session->GetMaxObjId(pConceptType)+1,true,0.0);

	// Parse the vectors
	for(vectors.Start();!vectors.End();vectors.Next())
	{
		RConstCursor<GConceptRef> Vector(vectors()->GetRefs());
		if(!Vector.GetNb())
			continue;
		GConceptRef Ref(vectors()->GetMetaConcept(),1.0);
		Alter(&Ref,false,idx,ConceptProd[idx],ConceptSum[idx],ConceptTypeRef[idx],ConceptEntropy[idx]);
		for(Vector.Start();!Vector.End();Vector.Next())
			Alter(Vector(),false,idx,ConceptProd[idx],ConceptSum[idx],ConceptTypeRef[idx],ConceptEntropy[idx]);
	}
}


//------------------------------------------------------------------------------
double LogEntropy::GetEntropy(const GConcept* concept,eType idx)
{
	if(concept->GetId()>=ConceptEntropy.GetNbCols())
		return(0.0);

	double& Val(ConceptEntropy(idx,concept->GetId()));
	if(Val!=Val)
	{
		double TypeRef(ConceptTypeRef(idx,concept->GetType()->GetId()));
		double Prod(ConceptProd(idx,concept->GetId()));
		double Sum(ConceptSum(idx,concept->GetId()));
		if(TypeRef&&Sum)
			Val=1+(((Prod/Sum)-log10(Sum))/(log10(TypeRef+1)));
		else
			Val=0.0;
	}
	return(Val);
}

//------------------------------------------------------------------------------
void LogEntropy::ComputeEntropy(GDescription* desc,eType idx)
{
	RCursor<GVector> Vector(desc->GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		RCursor<GConceptRef> ptr(Vector()->GetRefs());
		for(ptr.Start();!ptr.End();ptr.Next())
			ptr()->SetWeight(log10(ptr()->GetWeight()+1)*GetEntropy(ptr()->GetConcept(),idx));
	}
}


//------------------------------------------------------------------------------
void LogEntropy::ComputeEntropy(GDescription* desc,GDescriptionSet* set,eType)
{
	// If the description set is dirty -> recompute
	if(!set->GetData())
		set->SetData(new LogEntropyData(Session,this));
	LogEntropyData* Data(dynamic_cast<LogEntropyData*>(set->GetData()));
	if(Data->IsDirty())
		Data->Compute(set);

	// Compute the Log/Entropy
	RCursor<GVector> Vector(desc->GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		RCursor<GConceptRef> ptr(Vector()->GetRefs());
		for(ptr.Start();!ptr.End();ptr.Next())
			ptr()->SetWeight(log10(ptr()->GetWeight()+1)*Data->Entropy[ptr()->GetConcept()->GetId()]);
	}
}


//------------------------------------------------------------------------------
void LogEntropy::Measure(size_t measure,...)
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
			(*res)=GetEntropy(Concept,idx);
			break;
		case 1:
			ComputeEntropy(Desc,idx);
			break;
		case 2:
			ComputeEntropy(Desc,Set,idx);
			break;
		case 3:
			(*res)=ConceptSum(idx,Concept->GetId());
			break;
	}
}


//------------------------------------------------------------------------------
void LogEntropy::Info(size_t info,...)
{
	va_list ap;
	va_start(ap,info);
	RString* res(va_arg(ap,RString*));
	va_end(ap);
	if(info!=0)
		mThrowGException(RString::Number(info)+" is not allowed as information");
	(*res)="Log/entropy";
}


//------------------------------------------------------------------------------
void LogEntropy::HandleAddDoc(const RNotification& notification)
{
	// Verify that the sender is a document and that it is selected
	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
	if(Doc&&Session->IsUsed(Doc))
		Add(Doc->GetVectors(),tDoc);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleDelDoc(const RNotification& notification)
{
	// Verify that the sender is a document and that it is selected
	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
	if(Doc&&Session->IsUsed(Doc))
		Del(Doc->GetVectors(),tDoc);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleAddTopic(const RNotification& notification)
{
	// Verify that the sender is a topic
	GTopic* Topic(dynamic_cast<GTopic*>(notification.GetSender()));
	if(Topic)
		Add(Topic->GetVectors(),tTopic);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleDelTopic(const RNotification& notification)
{
	// Verify that the sender is a topic
	GTopic* Topic(dynamic_cast<GTopic*>(notification.GetSender()));
	if(Topic)
		Del(Topic->GetVectors(),tTopic);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleAddClass(const RNotification& notification)
{
	// Verify that the sender is a class
	GClass* Class(dynamic_cast<GClass*>(notification.GetSender()));
	if(Class)
		Add(Class->GetVectors(),tClass);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleDelClass(const RNotification& notification)
{
	// Verify that the sender is a class
	GClass* Class(dynamic_cast<GClass*>(notification.GetSender()));
	if(Class)
		Del(Class->GetVectors(),tClass);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleAddProfile(const RNotification& notification)
{
	// Verify that the sender is a profile
	GProfile* Profile(dynamic_cast<GProfile*>(notification.GetSender()));
	if(Profile)
		Add(Profile->GetVectors(),tProfile);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleDelProfile(const RNotification& notification)
{
	// Verify that the sender is a profile
	GProfile* Profile(dynamic_cast<GProfile*>(notification.GetSender()));
	if(Profile)
		Del(Profile->GetVectors(),tProfile);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleAddCommunity(const RNotification& notification)
{
	// Verify that the sender is a community
	GCommunity* Community(dynamic_cast<GCommunity*>(notification.GetSender()));
	if(Community)
		Add(Community->GetVectors(),tCommunity);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleDelCommunity(const RNotification& notification)
{
	// Verify that the sender is a community
	GCommunity* Community(dynamic_cast<GCommunity*>(notification.GetSender()));
	if(Community)
		Del(Community->GetVectors(),tCommunity);
}


//------------------------------------------------------------------------------
void LogEntropy::HandleReInit(const R::RNotification& notification)
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
void LogEntropy::ReInit(LogEntropy::eType type)
{
	if(ConceptProd.GetNbLines()>type)
	{
		ConceptProd[type]->Init(0.0);
		ConceptSum[type]->Init(0.0);
		ConceptEntropy[type]->Init(NAN);
		ConceptTypeRef[type]->Init(0.0);
	}
	if(Session->MustSaveResults()&&(ConceptsProdFile.GetNbLines()>type))
	{
		ConceptsProdFile.InitLine(type,0.0);
		ConceptsSumFile.InitLine(type,0.0);
		ConceptTypesFile.InitLine(type,0.0);
	}
}


//------------------------------------------------------------------------------
template<class cObj>
	void LogEntropy::ComputeEntropy(const cObj* obj,LogEntropy::eType type)
{
	RCursor<cObj> Obj(Session->GetObjs(obj));
	for(Obj.Start();!Obj.End();Obj.Next())
		Add(Obj()->GetVectors(),type);
}


//------------------------------------------------------------------------------
void LogEntropy::Reset(void)
{
	ConceptProd.Init(0.0);
	ConceptSum.Init(0.0);
	ConceptEntropy.Init(NAN);
	ConceptTypeRef.Init(0.0);
	if(Session->MustSaveResults())
	{
		ConceptsProdFile.Init(0.0);
		ConceptsSumFile.Init(0.0);
		ConceptTypesFile.Init(0.0);
	}
	ComputeEntropy(pDoc,tDoc);
	ComputeEntropy(pTopic,tTopic);
	ComputeEntropy(pClass,tClass);
	ComputeEntropy(pProfile,tProfile);
	ComputeEntropy(pCommunity,tCommunity);
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Features Evaluation","LogEntropy","LogEntropy",LogEntropy)
