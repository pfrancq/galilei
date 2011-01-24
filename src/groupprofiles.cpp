/*

	GALILEI Research Project

	GroupProfiles.cpp

	k-Means Plug-in for Profiles - Implementation

	Copyright 2008-2011 by Pascal Francq (pascal@francq.info).

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
#include <ggroupprofiles.h>
#include <gprofile.h>
#include <gcommunity.h>
#include <gmeasure.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for kMeans
#include <pluginkmeans.h>
#include <rgroupingkmeans.h>
#include <rkernelkmeans.h>


//-----------------------------------------------------------------------------
class GroupsProfile;
class kMeansProfiles;
kMeansProfiles* Kernel;


//-----------------------------------------------------------------------------
/**
 * The GroupDoc provides a representation for a group holding documents (used
 * by kMeansProfile).
 * @author Pascal Francq
 * @short kMeans Document Group
 */
class GroupProfile : public R::RGroup<GroupProfile,GProfile,GroupsProfile>
{
public:

	/**
	 * Constructor.
	 * @param owner          Owner of the group.
	 * @param id             Identifier of the group.
	 */
	GroupProfile(GroupsProfile* owner,size_t id) : R::RGroup<GroupProfile,GProfile,GroupsProfile>(owner,id) {}
};


//-----------------------------------------------------------------------------
/**
 * A Set of document groups.
 */
class GroupsProfile : public R::RGroups<GroupProfile,GProfile,GroupsProfile>
{
public:
	GroupsProfile(R::RCursor<GProfile> objs,size_t max) : R::RGroups<GroupProfile,GProfile,GroupsProfile>(objs,max) {}
};


//-----------------------------------------------------------------------------
class kMeansProfile : public R::RGroupingKMeans<GroupProfile,GProfile,GroupsProfile>
{
	GMeasure* Measure;
public:
	kMeansProfile(const R::RString& n,R::RRandom& r,R::RCursor<GProfile> objs,double convergence,R::RDebug* debug=0)
		: R::RGroupingKMeans<GroupProfile,GProfile,GroupsProfile>(n,r,objs,convergence,debug)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profiles Similarities");
	}
	double Similarity(const GProfile* obj1,const GProfile* obj2)
	{
		double d;
		Measure->Measure(0,obj1->GetId(),obj2->GetId(),&d);
		return(d);
	}
};


//-----------------------------------------------------------------------------
class kKernelMeansProfile : public RKernelkMeans<GroupProfile,GProfile,GroupsProfile>
{
	GMeasure* Measure;
	GSession* Session;

public:
	kKernelMeansProfile(GSession* session,const R::RString& n,R::RRandom& r,R::RCursor<GProfile> objs,size_t maxid,double alpha,double convergence,R::RDebug* debug=0)
		: RKernelkMeans<GroupProfile,GProfile,GroupsProfile>(n,r,objs,maxid,alpha,convergence,debug), Session(session)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profiles Similarities");
	}
	void FillSimilarities(RSparseMatrix& sims);
};


//-----------------------------------------------------------------------------
/**
* The KernelkMeansProfiles provides a plug-in for a spherical k-Means on documents.
* @author Pascal Francq
* @short Kernel kMeans for Profiles.
*/
class kMeansProfiles : public PluginkMeans<GGroupProfiles,GProfile,GCommunity,GroupProfile,GroupsProfile,kMeansProfile,kKernelMeansProfile>
{
public:

	/**
	* Constructor.
	* @param session        Session.
	* @param f              Factory.
	*/
	kMeansProfiles(GSession* session,GPlugInFactory* fac)
		: PluginkMeans<GGroupProfiles,GProfile,GCommunity,GroupProfile,GroupsProfile,kMeansProfile,kKernelMeansProfile>(session,fac,"Profiles Grouping",otProfile,otCommunity)
		  {
			Kernel=this;
		  }

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("kMeansProfiles");}

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
	virtual R::RCursor<GProfile> GetObjs(void) const {return(RCursor<GProfile>(Profiles));}

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) {RunGrouping(GetSession()->GetCommunities(),Profiles,GetSession()->GetMaxObjectId(otProfile));}

public:

	friend class kMeansProfile;
	friend class kKernelMeansProfile;
};



//------------------------------------------------------------------------------
void kKernelMeansProfile::FillSimilarities(RSparseMatrix& sims)
{
	if(Kernel->MeasureType=="Complete")
	{
		// Fill the matrix with the nearest neighbors
		RCursor<GProfile> Doc(Session->GetProfiles());
		for(Doc.Start();!Doc.End();Doc.Next())
		{
			RCursor<GProfile> Doc2(Session->GetProfiles());
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
		RCursor<GProfile> Doc(Session->GetProfiles());
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
CREATE_GROUPPROFILES_FACTORY("kMeans for Profiles","kMeans for Profiles",kMeansProfiles)
