/*

	GALILEI Research Project

	GProm.cpp

	Document Fragment PROMETHEE Ranking - Implementation.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).
   Copyright 2004-2005 by Jean-Baptiste Valsamis.
	Copyright 2005-2009 by Faïza Abbaci.

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
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rpromsol.h>
#include <rpromcriterion.h>
#include <gdoc.h>
#include <gvector.h>
#include <gsession.h>
#include <gmeasure.h>


//-----------------------------------------------------------------------------
// include files for current project
#include <gprom.h>
#include <genginexml.h>
#include <gresnode.h>
#include <gconceptnode.h>
#include <gresnodes.h>
#include <gquery.h>



//-----------------------------------------------------------------------------
//
// GPromSol
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GPromSol::GPromSol(const size_t id,GResNode* node)
	: RPromSol(id,RString::Number(node->GetNode()->GetSyntacticPos()),5), Node(node)
{

}



//-----------------------------------------------------------------------------
//
// GProm::cDocTfIdf
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GProm::cDocTfIdf
{
public:
	size_t DocId;
	double TfIdf;

	cDocTfIdf(size_t docid) : DocId(docid), TfIdf(0) {}
	int Compare(const cDocTfIdf& doc) const {return(CompareIds(DocId,doc.DocId));}
	int Compare(const size_t doc) const {return(CompareIds(DocId,doc));}
};



//-----------------------------------------------------------------------------
//
// GProm::cOccur
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Class that remember the number of occurrences of a given concept in a fragment
class GProm::cOccur
{
public:
	/**
	 * Identifier of the concept.
	 */
	size_t Id;

	/**
	 * Occurrence of the concept.
    */
	size_t Occur;

	/**
	 * Constructor.
    * @param id             identifier.
    */
	cOccur(size_t id) : Id(id), Occur(0) {}

	int Compare(const cOccur& ref) const {return(CompareIds(Id,ref.Id));}
	int Compare(const size_t ref) const {return(CompareIds(Id,ref));}
};



//-----------------------------------------------------------------------------
//
// GProm
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GProm::GProm(GEngineXML* engine,GQuery* query,GMeasure* weighting)
	: RPromKernel("Structured Search",3,5), Engine(engine), Query(query),
	  TfIdf      (new RPromLinearCriterion(RPromCriterion::Maximize,Engine->TfIdf, "Tf/Idf")),
	  TfIff      (new RPromLinearCriterion(RPromCriterion::Maximize,Engine->TfIff,"Tf/Iff")),
	  Specificity(new RPromLinearCriterion(RPromCriterion::Minimize,Engine->Specificity,"Specificity")),
	  Distance   (new RPromLinearCriterion(RPromCriterion::Minimize,Engine->Distance,"Distance")),
	  Type       (new RPromLinearCriterion(RPromCriterion::Maximize,Engine->Type,"Type")),
	  TfIdfs(1000), TmpOccurs(1000), Weighting(weighting)
{
	if(TfIdf->IsActive())
		AddCriterion(TfIdf);
	if(TfIff->IsActive())
		AddCriterion(TfIff);
	if(Specificity->IsActive())
		AddCriterion(Specificity);
	if(Distance->IsActive())
		AddCriterion(Distance);
	if(Type->IsActive())
		AddCriterion(Type);
}


//-----------------------------------------------------------------------------
void GProm::Add(GResNode* node)
{
	// Create the solution
	GPromSol* Sol(new GPromSol(GetNbSols(),node));
	AddSol(Sol);

	// Assign the criteria
	if(TfIdf->IsActive())
		Assign(Sol,TfIdf,ComputeTfIdf(node));
	if(TfIff->IsActive())
		Assign(Sol,TfIff,ComputeTfIff(node));
	if(Specificity->IsActive())
		Assign(Sol,Specificity,ComputeSpecificity(node));
	if(Distance->IsActive())
		Assign(Sol,Distance,ComputeDistance(node));
	if(Type->IsActive())
		Assign(Sol,Type,ComputeType(node));
}


//-----------------------------------------------------------------------------
RCursor<RPromSol> GProm::Compute(void)
{
	// Compute the solutions
	ComputePrometheeII();
	return(GetSols());
}


//-----------------------------------------------------------------------------
double GProm::ComputeTfIdf(GResNode* node)
{
	// If exist -> return the result
	size_t docid(node->GetParent()->GetDocId());
	cDocTfIdf* TfIdf(TfIdfs.GetPtr(docid));
	if(TfIdf)
		return(TfIdf->TfIdf);

	// Create the entry
	TfIdf=new cDocTfIdf(docid);
	TfIdfs.InsertPtr(TfIdf);
	GDoc* Doc(Engine->GetSession()->GetObj(pDoc,docid));

	// Weighted average of the tf-idf factors of each concept of the query in the document
	double Num(0.0);
	double Den(0.0);
	RConstCursor<GVector> Vector(Doc->GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		double Local(0.0);
		double MaxWeight(0.0);
		RConstCursor<GConceptRef> Ref(Vector()->GetRefs());
		for(Ref.Start();!Ref.End();Ref.Next())
		{
			if(fabs(Ref()->GetWeight())>MaxWeight)
				MaxWeight=fabs(Ref()->GetWeight());
			if(Query->IsIn(Ref()->GetConcept()))
			{
				double iffactor;
				Weighting->Measure(0,Ref()->GetConcept(),otDoc,&iffactor);
				Local+=Ref()->GetWeight()*iffactor;
			}
		}
		Num+=(Vector()->GetNb()*Local/MaxWeight);
		Den+=Vector()->GetNb();
	}
	TfIdf->TfIdf=Num/Den;
	return(TfIdf->TfIdf);
}


//-----------------------------------------------------------------------------
void GProm::CollectOccurs(GConceptNode* node)
{
	RNodeCursor<GConceptTree,GConceptNode> Child(node);
	for(Child.Start();!Child.End();Child.Next())
	{
		TmpOccurs.GetInsertPtr(Child()->GetConceptId())->Occur++;
		CollectOccurs(Child());
	}
}

//-----------------------------------------------------------------------------
double GProm::ComputeTfIff(GResNode* node)
{
	// If it has no children -> The whole fragment that match the query
	if(!node->GetNbChildren())
		return(1000.0);
	TmpOccurs.Clear(node->GetNbChildren());

	// Average of the tf-iff factors of each keyword of the query in the fragment

	// 1. Compute the number of occurrences of each different concept in the fragment
	CollectOccurs(node->GetNode());

	// 2. Compute the average weights multiply by the iff factor of each concept of the query
	double Sum(0.0);
	double Max(0.0);
	RCursor<cOccur> Occur(TmpOccurs);
	for(Occur.Start();!Occur.End();Occur.Next())
	{
		double Weight(Occur()->Occur);
		if(Weight>Max)
			Max=Weight;

		// Takes only the the concept of the query in account
		if(Query->IsIn(Engine->GetSession()->GetObj(pConcept,Occur()->Id)))
			Sum+=Weight*Engine->GetIff(Occur()->Id,node->GetNode()->GetConceptId());
	}

	// 3. Divide it by the number of concepts in the fragment and the maximum weight
	Sum/=(static_cast<double>(TmpOccurs.GetNb())*Max);
	return(Sum);
}


//-----------------------------------------------------------------------------
double GProm::ComputeSpecificity(GResNode* node)
{
	// Spec(e)=1/|e| -> |e| number of child nodes
	size_t Nb(0);
	RNodeCursor<GConceptTree,GConceptNode> Child(node->GetNode());
	for(Child.Start();!Child.End();Child.Next())
		Nb+=Child()->GetNbTotalNodes();
	return(static_cast<double>(Nb));
}


//-----------------------------------------------------------------------------
double GProm::ComputeDistance(GResNode* node)
{
	// If less than two keywords -> distance is null
	if(Query->GetNbConcepts()<2)
		return(0.0);

	// Compute the average distance
	RCursor<GConceptNode> Node(node->GetChildren());   // All child nodes
	size_t NbPairs(0);                                 // Number of keyword pairs
	double Sum(0.0);                                   // Sum of distance
	RCursor<GConcept> Concept(Query->GetConcepts(0,Query->GetNbConcepts()-2));
	for(Concept.Start();!Concept.End();Concept.Next())
	{
		// Only textual concept
		if(Concept()->GetType()->GetCategory()!=ccText)
			continue;
		RCursor<GConcept> Concept2(Query->GetConcepts(Concept.GetPos()+1));
		for(Concept2.Start();!Concept2.End();Concept2.Next())
		{
			// Only textual concept
			if(Concept2()->GetType()->GetCategory()!=ccText)
				continue;

			// A new pair to compare -> Find the minimum and maximum syntactic position of the keywords
			NbPairs++;
			size_t Min(cNoRef);
			size_t Max(0);
			for(Node.Start();!Node.End();Node.Next())
			{
				if((Node()->GetConceptId()!=Concept()->GetId())&&(Node()->GetConceptId()!=Concept2()->GetId()))
					continue;
				if(Node()->GetSyntacticPos()>Max)
					Max=Node()->GetSyntacticPos();
				if(Node()->GetSyntacticPos()<Min)
					Min=Node()->GetSyntacticPos();
			}

			// Compare with the root node
			if(node->GetNode()->GetSyntacticPos()>Max)
				Max=node->GetNode()->GetSyntacticPos();
			if(node->GetNode()->GetSyntacticPos()<Min)
				Min=node->GetNode()->GetSyntacticPos();

			// Distance is the size
			Sum+=Max-Min+1;
		}
	}

	// If no pairs -> distance is null
	if(!NbPairs)
		return(0.0);
	return(Sum/static_cast<double>(NbPairs));
}


//-----------------------------------------------------------------------------
double GProm::ComputeType(GResNode* node)
{
	double Ret;
	switch(node->GetNode()->GetType())
	{
		case ttLink:
			Ret=1;
			break;
		case ttSemantic:
		case ttDivision:
			Ret=2;
			break;
		case ttAttribute:
			Ret=3;
			break;
		case ttMetadata:
			Ret=4;
			break;
		default:
			Ret=0;
			break;
	}
	return(Ret);
}


//-----------------------------------------------------------------------------
GProm::~GProm(void)
{
}
