/*

	GALILEI Research Project

	PluginkMeans.hh

	Generic Plug-in for Kernel kMeans - Template Implementation

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
//
//  class PluginkMeans
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cGrp,class cGrps,class kMeans,class KernelkMeans>
	PluginkMeans<cPlugin,cObj,cGroup,cGrp,cGrps,kMeans,KernelkMeans>::PluginkMeans(GSession* session,GPlugInFactory* fac,const RString& name,tObjType objtype,tObjType grouptype)
		: cPlugin(session,fac), RObject(name), ObjType(objtype), GroupType(grouptype)
{
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cGrp,class cGrps,class kMeans,class KernelkMeans>
	void PluginkMeans<cPlugin,cObj,cGroup,cGrp,cGrps,kMeans,KernelkMeans>::ConstructResults(GSession* session,R::RCursor<cGrp> Sol)
{
	size_t i;
	RString Label(GetObjType(GroupType,true,false)+" ");

	session->Reset(GroupType);
	for(Sol.Start(),i=1;!Sol.End();Sol.Next(),i++)
	{
		cGroup* g(new cGroup(session,Label+RString::Number(i)));
		session->AssignId(g);
		RCursor<cObj> Cur(Sol()->GetObjs());
		for(Cur.Start();!Cur.End();Cur.Next())
			g->InsertObj(Cur());
		session->InsertObj(g);
	}
}


//-----------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cGrp,class cGrps,class kMeans,class KernelkMeans>
	void PluginkMeans<cPlugin,cObj,cGroup,cGrp,cGrps,kMeans,KernelkMeans>::RunGrouping(R::RCursor<cGroup> groups,R::RCursor<cObj> Objs,size_t maxid)
{
	if(Cout)
		cout<<"Do "<<kMeansType<<" kMeans for "<<Objs.GetNb()<<" "<<GetObjType(ObjType,false,true)<<endl;
	RRandom Rand(RRandom::Good,1);
	if(InternalRandom)
		Rand.Reset(12345);
	else
		Rand.Reset(Seed);

	typename kMeans::tInitial start;
	cGrps Sol(Objs,NbClusters);
	Sol.Init();
	if(Incremental&&groups.GetNb())
	{
		if(Cout)
			cout<<"Prepare incremental kMeans"<<endl;
		for(groups.Start();!groups.End();groups.Next())
		{
			cGrp* grp(0);
			RCursor<cObj> Cur(groups()->GetObjs());
			if(Cur.GetNb())
				grp=Sol.ReserveGroup();
			for(Cur.Start();!Cur.End();Cur.Next())
				grp->Insert(Cur());
		}
		start=kMeans::Incremental;
	}
	else
		start=kMeans::Random;//start=kMeans::kMeansPlusPlus;
	if(Cout)
		cout<<"Run "<<kMeansType<<" kMeans ("<<NbClusters<<" clusters, convergence: "<<Convergence<<")"<<endl;
	if(kMeansType=="Normal")
	{
		kMeans Instance("kMeansObj",Rand,Objs,Convergence);
		Instance.Run(&Sol,MaxkMeans,NbClusters,start);
		if(Cout)
			cout<<kMeansType<<" kMeans iterates "<<Instance.GetNbIterations()<<" times (max="<<MaxkMeans<<")"<<endl;
	}
	else
	{
		KernelkMeans Instance(Session,"KernelkMeansObj",Rand,Objs,maxid,Alpha,Convergence);
		Instance.Run(&Sol,MaxkMeans,NbClusters,Spherical);
		if(Cout)
			cout<<kMeansType<<" kMeans iterates "<<Instance.GetNbIterations()<<" times (max="<<MaxkMeans<<")"<<endl;
	}
	ConstructResults(Session,Sol);
}


//------------------------------------------------------------------------------
template<class cPlugin,class cObj,class cGroup,class cGrp,class cGrps,class kMeans,class KernelkMeans>
	void PluginkMeans<cPlugin,cObj,cGroup,cGrp,cGrps,kMeans,KernelkMeans>::CreateConfig(void)
{
	InsertParam(new RParamValue("Alpha",0.9,"It is the parameter used to compute the kernel: D-αA"));
	InsertParam(new RParamValue("Convergence",0.001,"The convergence level used"));
	InsertParam(new RParamValue("NbClusters",20,"Number of clusters to determine"));
	InsertParam(new RParamValue("Max kMeans",30,"Maximal number of iterations"));
	InsertParam(new RParamValue("Incremental",true,"Should the algorithm be incremental"));
	InsertParam(new RParamValue("InternalRandom",true,"Use an internal random number generator"));
	InsertParam(new RParamValue("Seed",static_cast<int>(12345),"Internal random number generator seed"));
	InsertParam(new RParamValue("Cout",false,"Cout some information?"));
	InsertParam(new RParamValue("MeasureType","Complete"));
	InsertParam(new RParamValue("kMeansType","Normal"));
	InsertParam(new RParamValue("Spherical",true));
}
