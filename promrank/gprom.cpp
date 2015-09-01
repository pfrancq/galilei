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
#include <gdoc.h>
#include <gvector.h>
#include <gsession.h>
#include <gmeasure.h>
#include <gconceptnode.h>


//-----------------------------------------------------------------------------
// include files for current project
#include <gprom.h>
#include <gpromrank.h>
#include <gsearchquery.h>
#include <gsearchquerynode.h>



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



//------------------------------------------------------------------------------
//
// Iterators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GProm::Iterator : public GConceptRecordIterator
{
	GProm* Prom;
public:
	Iterator(GDoc* doc,const GConceptRecord& rec,GProm* prom)
		: GConceptRecordIterator(doc,rec), Prom(prom)
		{}

	virtual void Treat(GConceptRecord& child)
	{
		Prom->TmpOccurs.GetInsertPtr(child.GetConceptId())->Occur++;
		Iterator(GetDoc(),child,Prom).Parse();
	}
};


//------------------------------------------------------------------------------
class GProm::Iterator2 : public GConceptRecordIterator
{
	double& Nb;
public:
	Iterator2(GDoc* doc,const GConceptRecord& rec,double& nb)
		: GConceptRecordIterator(doc,rec), Nb(nb)
		{}

	virtual void Treat(GConceptRecord& child)
	{
		Nb+=GetDoc()->GetSession()->GetNbTotalChildRecords(GetDoc(),child);
	}
};


//-----------------------------------------------------------------------------
//
// GProm
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GProm::GProm(GPromRank* rank,GMeasure* weighting)
	: RPromKernel("Structured Search",3,5), Rank(rank),
	  TfIdf      (new RPromLinearCriterion(RPromCriterion::Maximize,rank->TfIdf, "Tf/Idf")),
	  TfIff      (new RPromLinearCriterion(RPromCriterion::Maximize,rank->TfIff,"Tf/Iff")),
	  Specificity(new RPromLinearCriterion(RPromCriterion::Minimize,rank->Specificity,"Specificity")),
	  Distance   (new RPromLinearCriterion(RPromCriterion::Minimize,rank->Distance,"Distance")),
	  Type       (new RPromLinearCriterion(RPromCriterion::Maximize,rank->Type,"Type")),
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
void GProm::Add(GDocFragment* node)
{
	// Create the solution
	GPromSol* Sol;
	if(node->GetRecord())
		Sol=new GPromSol(GetNbSols(),node->GetRecord()->GetSyntacticPos(),node);
	else
		Sol=new GPromSol(GetNbSols(),0,node);

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
double GProm::ComputeTfIdf(GDocFragment* node)
{
	// If exist -> return the result
	size_t docid(node->GetDoc()->GetId());
	cDocTfIdf* TfIdf(TfIdfs.GetPtr(docid));
	if(TfIdf)
		return(TfIdf->TfIdf);

	// Create the entry
	TfIdf=new cDocTfIdf(docid);
	TfIdfs.InsertPtr(TfIdf);
	GDoc* Doc(Rank->GetSession()->GetObj(pDoc,docid));

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
			if(Rank->Query->IsIn(Ref()->GetConcept()))
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
void GProm::CollectOccurs(GDoc* doc,const GConceptRecord* rec)
{
	Iterator(doc,*rec,this).Parse();
}

//-----------------------------------------------------------------------------
double GProm::ComputeTfIff(GDocFragment* node)
{
	// If it has no children -> The whole fragment matches the query
	if(!node->GetNbChildren())
		return(1000.0);
	TmpOccurs.Clear(node->GetNbChildren());

	// Average of the tf-iff factors of each keyword of the query in the fragment

	// 1. Compute the number of occurrences of each different concept in the fragment

	CollectOccurs(node->GetDoc(),node->GetRecord());

	// 2. Compute the average weights multiply by the iff factor of each concept of the query
	double Sum(0.0);
	double Max(0.0);
	RCursor<cOccur> Occur(TmpOccurs);
	for(Occur.Start();!Occur.End();Occur.Next())
	{
		double Weight(Occur()->Occur);
		if(Weight>Max)
			Max=Weight;

		// Takes only the concepts of the query in account
		if(Rank->Query->IsIn(Rank->GetSession()->GetObj(pConcept,Occur()->Id)))
			Sum+=Weight*Rank->GetIff(Occur()->Id,node->GetRecord()->GetConceptId());
	}

	// 3. Divide it by the number of concepts in the fragment and the maximum weight
	Sum/=(static_cast<double>(TmpOccurs.GetNb())*Max);
	return(Sum);
}


//-----------------------------------------------------------------------------
double GProm::ComputeSpecificity(GDocFragment* node)
{
	// Spec(e)=1/|e| -> |e| number of child nodes
	double Nb(0.0);

	if(node->GetRecord())
	{
		Iterator2(node->GetDoc(),*node->GetRecord(),Nb).Parse();
	}
	else
	{
		// The child nodes are those that select the concept node
		RCursor<const GConceptRecord> Child(node->GetChildren());
		for(Child.Start();!Child.End();Child.Next())
		{
			Iterator2(node->GetDoc(),*Child(),Nb).Parse();
		}
	}
	return(Nb);
}


//-----------------------------------------------------------------------------
double GProm::ComputeDistance(GDocFragment* node)
{
	// If less than two keywords -> distance is null
	if(Rank->Query->GetNbConcepts()<2)
		return(0.0);

	// Compute the average distance
	RCursor<const GConceptRecord> Node(node->GetChildren());   // All child nodes
	size_t NbPairs(0);                                 // Number of keyword pairs
	double Sum(0.0);                                   // Sum of distance
	RCursor<GConcept> Concept(Rank->Query->GetConcepts(0,Rank->Query->GetNbConcepts()-2));
	for(Concept.Start();!Concept.End();Concept.Next())
	{
		// Only textual concept
		if(Concept()->GetType()->GetCategory()!=ccText)
			continue;
		RCursor<GConcept> Concept2(Rank->Query->GetConcepts(Concept.GetPos()+1));
		for(Concept2.Start();!Concept2.End();Concept2.Next())
		{
			// Only textual concept
			if(Concept2()->GetType()->GetCategory()!=ccText)
				continue;

			// A new pair to compare -> Find the minimum and maximum syntactic position of the keywords
			NbPairs++;
			size_t Min(cNoRef);
			size_t Max(0);
			//Iterator3(node->GetDoc(),const GConceptRecord& rec,GConcept* concept,GConcept* concept2,size_t& min,size_t& max)
			for(Node.Start();!Node.End();Node.Next())
			{
				if((Node()->GetConceptId()!=Concept()->GetId())&&(Node()->GetConceptId()!=Concept2()->GetId()))
					continue;
				if(Node()->GetSyntacticPos()>Max)
					Max=Node()->GetSyntacticPos();
				if(Node()->GetSyntacticPos()<Min)
					Min=Node()->GetSyntacticPos();
			}

			// Compare with the root node if any
			if(node->GetRecord())
			{
				if(node->GetRecord()->GetSyntacticPos()>Max)
					Max=node->GetRecord()->GetSyntacticPos();
				if(node->GetRecord()->GetSyntacticPos()<Min)
					Min=node->GetRecord()->GetSyntacticPos();
			}

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
double GProm::ComputeType(GDocFragment* node)
{
	double Ret;
	if(!node->GetRecord())
		return(0);
	switch(node->GetRecord()->GetType())
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
