/*

	GALILEI Research Project

	If.cpp

	Inverse Frequencies - Implementation.

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
#include <gsession.h>
#include <if.h>



//------------------------------------------------------------------------------
//
//  If
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
If::If(GSession* session,GPlugInFactory* fac)
	: RObject(fac->GetName()), GMeasure(session,fac), Dirty(false),
	  ConceptRef(Session->GetNbConcepts()+1,5), ConceptIf(Session->GetNbConcepts()+1,5),
	  ConceptTypeRef(Session->GetNbConceptTypes()+1,5),
	  tmpTypes(Session->GetNbConceptTypes()+1),tmpConcepts(Session->GetNbConcepts()+1)
{
	// Document notifications
	InsertObserver(HANDLER(If::HandleAddDoc),hDocs[oeSelected]);
	InsertObserver(HANDLER(If::HandleAddDoc),hDocs[oeUpdated]);
	InsertObserver(HANDLER(If::HandleDelDoc),hDocs[oeAboutToBeUpdated]);
	InsertObserver(HANDLER(If::HandleDelDoc),hDocs[oeDeselected]);
	InsertObserver(HANDLER(If::HandleDelDoc),hDocs[oeAboutToBeDeleted]);

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

	// Verify the sizes
	ConceptRef.VerifySize(Session->GetNbConcepts()+1,5,true,0.0);
	ConceptIf.VerifySize(Session->GetNbConcepts()+1,5,true,NAN);
	ConceptTypeRef.VerifySize(Session->GetNbConceptTypes()+1,5,true,0.0);
	ConceptsFile.VerifySize(Session->GetNbConcepts()+1,5,true,0.0);
	ConceptTypesFile.VerifySize(Session->GetNbConceptTypes()+1,5,true,0.0);
}


//------------------------------------------------------------------------------
void If::Done(void)
{
	ConceptsFile.Save(ConceptRef);
	ConceptTypesFile.Save(ConceptTypeRef);
	ConceptsFile.Close();
	ConceptTypesFile.Close();
}


//------------------------------------------------------------------------------
void If::Add(R::RConstCursor<GVector> vectors,eType idx)
{
	// Verify that there is something to do
	if(!vectors.GetNb()) return;

	// Verify the sizes
	ConceptRef.VerifySize(Session->GetNbConcepts()+1,5,true,0.0);
	ConceptIf.VerifySize(Session->GetNbConcepts()+1,5,true,NAN);
	ConceptTypeRef.VerifySize(Session->GetNbConceptTypes()+1,5,true,0.0);
	tmpTypes.Init(Session->GetNbConceptTypes()+1,true);
	tmpConcepts.Init(Session->GetNbConcepts()+1,true);
	if(Session->MustSaveResults())
	{
		ConceptsFile.VerifySize(Session->GetNbConcepts()+1,5,true,0.0);
		ConceptTypesFile.VerifySize(Session->GetNbConceptTypes()+1,5,true,0.0);
	}

	// Parse the vectors
	for(vectors.Start();!vectors.End();vectors.Next())
	{
		RConstCursor<GConceptRef> ptr(vectors()->GetRefs());
		if(!ptr.GetNb())
			continue;

		GConceptType* type(vectors()->GetMetaConcept()->GetType());
		size_t TypeId(type->GetId());
		if(tmpTypes[TypeId])
		{
			// Yes -> A new object uses this concept type.
			ConceptTypeRef(TypeId,idx)++;
			if(Session->MustSaveResults())
				ConceptTypesFile.Write(TypeId,idx,ConceptTypeRef(TypeId,idx));
			tmpTypes[TypeId]=false;
		}

		// IncRef for the concept
		size_t ConceptId(vectors()->GetMetaConcept()->GetId());
		if(tmpConcepts[ConceptId])
		{
			// Yes -> A new object uses this meta-concept.
			ConceptRef(ConceptId,idx)++;
			ConceptIf(ConceptId,idx)=NAN;
			if(Session->MustSaveResults())
				ConceptsFile.Write(ConceptId,idx,ConceptRef(ConceptId,idx));
			tmpConcepts[ConceptId]=false;
		}

		for(ptr.Start();!ptr.End();ptr.Next())
		{
			GConceptType* type(ptr()->GetConcept()->GetType());
			size_t TypeId(type->GetId());
			if(tmpTypes[TypeId])
			{
				// Yes -> A new object uses this concept type.
				ConceptTypeRef(TypeId,idx)++;
				if(Session->MustSaveResults())
					ConceptTypesFile.Write(TypeId,idx,ConceptTypeRef(TypeId,idx));
				tmpTypes[TypeId]=false;
			}

			// IncRef for the concept
			size_t ConceptId(ptr()->GetConcept()->GetId());
			if(tmpConcepts[ConceptId])
			{
				// Yes -> A new object uses this concept.
				ConceptRef(ConceptId,idx)++;
				ConceptIf(ConceptId,idx)=NAN;
				if(Session->MustSaveResults())
					ConceptsFile.Write(ConceptId,idx,ConceptRef(ConceptId,idx));
				tmpConcepts[ConceptId]=false;
			}
		}
	}
}


//------------------------------------------------------------------------------
void If::Del(R::RConstCursor<GVector> vectors,eType idx)
{
	// Verify that there is something to do
	if(!vectors.GetNb()) return;

	// Verify the sizes
	ConceptRef.VerifySize(Session->GetNbConcepts()+1,5,true,0.0);
	ConceptIf.VerifySize(Session->GetNbConcepts()+1,5,true,0.0);
	ConceptTypeRef.VerifySize(Session->GetNbConceptTypes()+1,5,true,0.0);
	tmpTypes.Init(Session->GetNbConceptTypes()+1,true);
	tmpConcepts.Init(Session->GetNbConcepts()+1,true);

	// Parse the vectors
	for(vectors.Start();!vectors.End();vectors.Next())
	{
		RConstCursor<GConceptRef> ptr(vectors()->GetRefs());
		if(!ptr.GetNb())
			continue;

		// Reference of the concept associated with the vector
		GConceptType* type(vectors()->GetMetaConcept()->GetType());
		size_t TypeId(type->GetId());
		if(tmpTypes[TypeId])
		{
			// Yes -> An old object uses this concept type.
			ConceptTypeRef(TypeId,idx)--;
			if(Session->MustSaveResults())
				ConceptTypesFile.Write(TypeId,idx,ConceptTypeRef(TypeId,idx));
			tmpTypes[TypeId]=false;
		}

		// DecRef for the concept
		size_t ConceptId(vectors()->GetMetaConcept()->GetId());
		if(tmpConcepts[ConceptId])
		{
			// Yes -> An old object uses this meta-concept.
			ConceptRef(ConceptId,idx)--;
			ConceptIf(ConceptId,idx)=NAN;
			if(Session->MustSaveResults())
				ConceptsFile.Write(ConceptId,idx,ConceptRef(ConceptId,idx));
			tmpConcepts[ConceptId]=false;
		}

		for(ptr.Start();!ptr.End();ptr.Next())
		{
			GConceptType* type(ptr()->GetConcept()->GetType());
			size_t TypeId(type->GetId());
			if(tmpTypes[TypeId])
			{
				// Yes -> An old object uses this concept type.
				ConceptTypeRef(TypeId,idx)--;
				if(Session->MustSaveResults())
					ConceptTypesFile.Write(TypeId,idx,ConceptTypeRef(TypeId,idx));
				tmpTypes[TypeId]=false;
			}

			// DecRef for the concept
			size_t ConceptId(ptr()->GetConcept()->GetId());
			if(tmpConcepts[ConceptId])
			{
				// Yes -> An old object uses this concept.
				ConceptRef(ConceptId,idx)--;
				ConceptIf(ConceptId,idx)=NAN;
				if(Session->MustSaveResults())
					ConceptsFile.Write(ConceptId,idx,ConceptRef(ConceptId,idx));
				tmpConcepts[ConceptId]=false;
			}
		}
	}
}


//------------------------------------------------------------------------------
void If::Measure(size_t measure,...)
{
	va_list ap;
	va_start(ap,measure);
	GConcept* concept(va_arg(ap,GConcept*));
	tObjType type(static_cast<tObjType>(va_arg(ap,int)));
	double* res(va_arg(ap,double*));
	va_end(ap);
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

	switch(measure)
	{
		case 0:
		{
			double& Val(ConceptIf(concept->GetId(),idx));
			if(Val!=Val)
			{
				if(ConceptTypeRef(concept->GetType()->GetId(),idx)&&ConceptRef(concept->GetId(),idx))
					Val=log10(ConceptTypeRef(concept->GetType()->GetId(),idx)/ConceptRef(concept->GetId(),idx));
				else
					Val=0.0;
			}
			(*res)=Val;
			break;
		}
		break;
		default:
			mThrowGException(RString::Number(measure)+" is not allowed as measure.");
	}
}


//------------------------------------------------------------------------------
void If::Info(size_t info,...)
{
	va_list ap;
	va_start(ap,info);
	if(info==cNoRef)
	{
		size_t* res(va_arg(ap,size_t*));
		(*res)=2;
	}
	else
	{
		RString* res(va_arg(ap,RString*));
		switch(info)
		{
			case 0:
				(*res)="if";
				break;
			default:
				mThrowGException(RString::Number(2)+" is not allowed as features weight.");
		}
	}
	va_end(ap);
}


//------------------------------------------------------------------------------
void If::HandleAddDoc(const RNotification& notification)
{
	// Verify that the sender is a document and that it is selected
	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
	if(Doc&&Session->IsSelected(Doc))
	{
		cout<<"Add document "<<Doc->GetId()<<endl;
		Add(Doc->GetVectors(),tDoc);
	}
}


//------------------------------------------------------------------------------
void If::HandleDelDoc(const RNotification& notification)
{
	// Verify that the sender is a document and that it is selected
	GDoc* Doc(dynamic_cast<GDoc*>(notification.GetSender()));
	if(Doc&&Session->IsSelected(Doc))
	{
		cout<<"Delete document "<<Doc->GetId()<<endl;
		Del(Doc->GetVectors(),tDoc);
	}
}


//------------------------------------------------------------------------------
void If::ApplyConfig(void)
{
/*	Docs=FindParam<RParamValue>("Docs")->GetBool();
	ProfDoc=FindParam<RParamValue>("ProfDoc")->GetBool();
	GroupDoc=FindParam<RParamValue>("GroupDoc")->GetBool();
	Profiles=FindParam<RParamValue>("Profiles")->GetBool();
	SameDocProf=FindParam<RParamValue>("SameDocProf")->GetBool();
	GroupProf=FindParam<RParamValue>("GroupProf")->GetBool();
	RURI OldName(Results);
	Results=FindParam<RParamValue>("Results")->Get();
	ExportDocsSims=FindParam<RParamValue>("ExportDocsSims")->GetBool();
	DocsSims=FindParam<RParamValue>("DocsSims")->Get();
	ExportDocsIncs=FindParam<RParamValue>("ExportDocsIncs")->GetBool();
	DocsIncs=FindParam<RParamValue>("DocsIncs")->Get();
	RString Tmp(FindParam<RParamValue>("MeasureType")->Get());
	if((Tmp!="Complete")&&(Tmp!="Nearest Neighbors"))
		mThrowGException("'"+Tmp+"' is invalid : Only 'Complete' or 'Nearest Neighbors' are allowed for the type of measure");
	MeasureType=Tmp;*/
	//CurWeights=FindParam<RParamValue>("CurWeights")->GetUInt();
}


//------------------------------------------------------------------------------
void If::CreateConfig(void)
{
/*	InsertParam(new RParamValue("Docs",false));
	InsertParam(new RParamValue("ProfDoc",false));
	InsertParam(new RParamValue("GroupDoc",false));
	InsertParam(new RParamValue("Profiles",false));
	InsertParam(new RParamValue("SameDocProf",false));
	InsertParam(new RParamValue("GroupProf",false));
	InsertParam(new RParamValue("Results",""));
	InsertParam(new RParamValue("ExportDocsSims",false));
	InsertParam(new RParamValue("DocsSims",""));
	InsertParam(new RParamValue("ExportDocsIncs",false));
	InsertParam(new RParamValue("DocsIncs",""));
	InsertParam(new RParamValue("MeasureType","Complete"));*/
	//InsertParam(new RParamValue("CurWeights",0));
	//InsertParam(new RParamValue("MaxWeights",2));
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Features Evaluation","if","Inverse Frequencies",If)
