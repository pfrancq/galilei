/*

	GALILEI Research Project

	GroupDocs.cpp

	k-Means Plug-in for Documents - Implementation

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
// include files for R project
#include <rgroup.h>
#include <rgroups.h>
#include <rmaxvector.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggroupdocs.h>
#include <gdoc.h>
#include <gtopic.h>
#include <gmeasure.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for kMeans
#include <pluginkmeans.h>
#include <rgroupingkmeans.h>
#include <rkernelkmeans.h>


//-----------------------------------------------------------------------------
class GroupsDoc;
class kMeansDocs;
kMeansDocs* Kernel;


//-----------------------------------------------------------------------------
/**
 * The GroupDoc provides a representation for a group holding documents (used
 * by kMeansDoc).
 * @author Pascal Francq
 * @short kMeans Document Group
 */
class GroupDoc : public R::RGroup<GroupDoc,GDoc,GroupsDoc>
{
public:

	/**
	 * Constructor.
	 * @param owner          Owner of the group.
	 * @param id             Identifier of the group.
	 */
	GroupDoc(GroupsDoc* owner,size_t id) : R::RGroup<GroupDoc,GDoc,GroupsDoc>(owner,id) {}
};


//-----------------------------------------------------------------------------
/**
 * A Set of document groups.
 */
class GroupsDoc : public R::RGroups<GroupDoc,GDoc,GroupsDoc>
{
public:
	GroupsDoc(R::RCursor<GDoc> objs,size_t max) : R::RGroups<GroupDoc,GDoc,GroupsDoc>(objs,max) {}
};


//-----------------------------------------------------------------------------
class kMeansDoc : public R::RGroupingKMeans<GroupDoc,GDoc,GroupsDoc>
{
	GMeasure* Measure;
public:
	kMeansDoc(const R::RString& n,R::RRandom& r,R::RCursor<GDoc> objs,double convergence,R::RDebug* debug=0)
		: R::RGroupingKMeans<GroupDoc,GDoc,GroupsDoc>(n,r,objs,convergence,debug)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document Similarities");
	}
	double Similarity(const GDoc* obj1,const GDoc* obj2)
	{
		double d;
		Measure->Measure(0,obj1->GetId(),obj2->GetId(),&d);
		return(d);
	}
};


//-----------------------------------------------------------------------------
class kKernelMeansDoc : public RKernelkMeans<GroupDoc,GDoc,GroupsDoc>
{
	GMeasure* Measure;
	GSession* Session;

public:
	kKernelMeansDoc(GSession* session,const R::RString& n,R::RRandom& r,R::RCursor<GDoc> objs,size_t maxid,double alpha,double convergence,R::RDebug* debug=0)
		: RKernelkMeans<GroupDoc,GDoc,GroupsDoc>(n,r,objs,maxid,alpha,convergence,debug), Session(session)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document Similarities");
	}
	void FillSimilarities(RSparseMatrix& sims);
};


//-----------------------------------------------------------------------------
/**
* The KernelkMeansDocs provides a plug-in for a spherical k-Means on documents.
* @author Pascal Francq
* @short Kernel kMeans for Documents.
*/
class kMeansDocs : public PluginkMeans<GGroupDocs,GDoc,GTopic,GroupDoc,GroupsDoc,kMeansDoc,kKernelMeansDoc>
{
public:

	/**
	* Constructor.
	* @param session        Session.
	* @param f              Factory.
	*/
	kMeansDocs(GSession* session,GPlugInFactory* fac)
		: PluginkMeans<GGroupDocs,GDoc,GTopic,GroupDoc,GroupsDoc,kMeansDoc,kKernelMeansDoc>(session,fac,"Documents Grouping",otDoc,otTopic)
		  {
			Kernel=this;
		  }

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("kMeansDocs");}

	/**
	* Configurations were applied from the factory.
	*/
	void ApplyConfig(void)
	{
		Convergence=FindParam<RParamValue>("Convergence")->GetDouble();
		Alpha=FindParam<RParamValue>("Alpha")->GetDouble();
		Incremental=FindParam<RParamValue>("Incremental")->GetBool();
		Spherical=FindParam<RParamValue>("Spherical")->GetBool();
		InternalRandom=FindParam<RParamValue>("InternalRandom")->GetBool();
		Seed=FindParam<RParamValue>("Seed")->GetInt();
		NbClusters=FindParam<RParamValue>("NbClusters")->GetUInt();
		MaxkMeans=FindParam<RParamValue>("Max kMeans")->GetUInt();
		Cout=FindParam<RParamValue>("Cout")->GetBool();
		RString Tmp(FindParam<RParamValue>("MeasureType")->Get());
		if((Tmp!="Complete")&&(Tmp!="Nearest Neighbors"))
			ThrowGException("'"+Tmp+"' is invalid : Only 'Complete' or 'Nearest Neighbors' are allowed for the type of measure");
		MeasureType=Tmp;
		Tmp=FindParam<RParamValue>("kMeansType")->Get();
		if((Tmp!="Normal")&&(Tmp!="Kernel"))
			ThrowGException("'"+Tmp+"' is invalid : Only 'Normal' or 'Kernel' are allowed for the type of measure");
		kMeansType=Tmp;
	}


protected:

	/**
	 * Get a pointer to the objects to cluster.
	 */
	virtual R::RCursor<GDoc> GetObjs(void) const {return(RCursor<GDoc>(Docs));}

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) {RunGrouping(GetSession()->GetObjs(pTopic),Docs,GetSession()->GetMaxObjId(pDoc));}

public:

	friend class kMeansDoc;
	friend class kKernelMeansDoc;
};



//------------------------------------------------------------------------------
void kKernelMeansDoc::FillSimilarities(RSparseMatrix& sims)
{
	if(Kernel->MeasureType=="Complete")
	{
		// Fill the matrix with all documents
		RCursor<GDoc> Doc(Session->GetObjs(pDoc));
		for(Doc.Start();!Doc.End();Doc.Next())
		{
			RCursor<GDoc> Doc2(Session->GetObjs(pDoc));
			for(Doc2.Start();!Doc2.End();Doc2.Next())
			{
				double mes;
				Measure->Measure(0,Doc()->GetId(),Doc2()->GetId(),&mes);
				sims(Doc()->GetId(),Doc2()->GetId())=mes;
			}
		}
	}
	else
	{
		// Fill the matrix with the nearest neighbors
		RCursor<GDoc> Doc(Session->GetObjs(pDoc));
		for(Doc.Start();!Doc.End();Doc.Next())
		{
			const RMaxVector* Vec;
			Measure->Info(3,Doc()->GetId(),&Vec);
			RCursor<RMaxValue> Cur(*Vec);
			for(Cur.Start();!Cur.End();Cur.Next())
				sims(Doc()->GetId(),Cur()->Id)=Cur()->Value;
		}
	}
}


//------------------------------------------------------------------------------
CREATE_GROUPDOCS_FACTORY("kMeans for Documents","kMeans for Documents",kMeansDocs)
