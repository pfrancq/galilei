/*

	GALILEI Research Project

	ClusteringEval.h

	Clustering Evaluation - Template Implementation

	Copyright 2007-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

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
// class ClusteringEval
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	ClusteringEval<cGroup,cObj>::ClusteringEval(GSession* session,GPlugInFactory* fac,tObjType grouptype,tObjType objtype)
		: GMeasure(session,fac), GroupType(grouptype), ObjType(objtype), ClustersScore(100,50)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj>
	void ClusteringEval<cGroup,cObj>::Dirty(void)
{
	DirtyRecallPrecision=true;
	DirtyAdjustedRandIndex=true;
	DirtyJ=true;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj>
	void ClusteringEval<cGroup,cObj>::Init(void)
{
	Dirty();
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj>
	void ClusteringEval<cGroup,cObj>::Measure(size_t measure,...)
{
	va_list ap;
	va_start(ap,measure);
	size_t id(va_arg(ap,size_t));  // Element i correspond to line i-1
	double* res(va_arg(ap,double*));
	va_end(ap);

	cGroup* Cluster(static_cast<cGroup*>(Session->GetObj(GroupType,id)));
	if(!Cluster)
		ThrowGException("'"+RString::Number(id)+"' is not a valid "+GetObjType(GroupType,false,false));
	ClusterScore<cGroup>* g(ClustersScore.GetPtr(Cluster));

	switch(measure)
	{
		case 0:
			if(DirtyRecallPrecision)
				ComputeRecallPrecision();
			if(!g)
				(*res)=0.0;
			else
				(*res)=g->Recall;
			break;
		case 1:
			if(DirtyRecallPrecision)
				ComputeRecallPrecision();
			if(!g)
				(*res)=0.0;
			else
				(*res)=g->Precision;
			break;
		default:
			throw GException("ClusteringEval::Measure(size_t,...) : Only 0 and 1 are allowed as measure");
	};
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj>
	void ClusteringEval<cGroup,cObj>::Info(size_t measure,...)
{
	va_list ap;
	va_start(ap,measure);
	double* res(va_arg(ap,double*));
	va_end(ap);

	switch(measure)
	{
		case 0:
			if(DirtyRecallPrecision)
				ComputeRecallPrecision();
			(*res)=Recall;
			break;
		case 1:
			if(DirtyRecallPrecision)
				ComputeRecallPrecision();
			(*res)=Precision;
			break;
		case 2:
			if(DirtyAdjustedRandIndex)
				ComputeAdjustedRandIndex();
			(*res)=AdjustedRandIndex;
			break;
		case 3:
			if(DirtyJ)
				ComputeJ();
			(*res)=J;
			break;
		default:
			throw GException("ClusteringEval::Info(size_t,...) : Only 0,1,2 and 3 are allowed as measure");
	};
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj>
	bool ClusteringEval<cGroup,cObj>::IsObjAloneInIdealGroup(cObj* obj)
{
	const GSubject* ThGrp(Session->GetSubject(obj));
	if(!ThGrp)
		throw GException("ClusteringEval<cGroup,cObj>::IsObjAloneInIdealGroup(cObj*): ThGrp cannot be null");
	return(ThGrp->GetNbObjs(ObjType)==1);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj>
	void ClusteringEval<cGroup,cObj>::ComputeBestLocalRecallPrecision(RCursor<cObj>& objs,ClusterScore<cGroup>* grp,size_t ingroup)
{
	for(objs.Start();!objs.End();objs.Next())
	{
		const GSubject* ThGrp(Session->GetSubject(objs()));
		if(!ThGrp)
			throw GException("ClusteringEval<cGroup,cObj>::ComputeBestLocalRecallPrecision(cObj*,ClusterScore<cGroup>*,size_t): ThGrp cannot be null");
		size_t InThGrp(ThGrp->GetNbObjs(ObjType));
		if(InThGrp==1)
		{
			// Precision is null
			grp->Recall+=1.0;
		}
		else
		{
			size_t ElseInThGrp(ThGrp->GetNbObjs(grp->Group)-1);
			grp->Precision+=((double)(ElseInThGrp))/((double)(ingroup-1));
			size_t ElseInGrp(grp->Group->GetNbObjs(ThGrp)-1);
			grp->Recall+=((double)(ElseInGrp))/((double)(InThGrp-1));
		}
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj>
	void ClusteringEval<cGroup,cObj>::ComputeRecallPrecision(void)
{
	size_t InGrp;           // Number of objects in the computed group.
	size_t NbObjs(0);       // Number of objects grouped.

	// Re-create the classes for the score of each cluster
	ClustersScore.Clear();
	RCursor<cGroup> Cur(GetClusters());
	for(Cur.Start();!Cur.End();Cur.Next())
		ClustersScore.InsertPtr(new ClusterScore<cGroup>(Cur()));

	// Compute the recall and the precision
	Precision=Recall=0.0;
	RCursor<ClusterScore<cGroup> > Grps(ClustersScore);
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		InGrp=Grps()->Group->GetNbObjs();
		NbObjs+=InGrp;
		Grps()->Precision=Grps()->Recall=0.0;
		RAssert(InGrp);
		RCursor<cObj> Objs(Grps()->Group->GetObjs());
		if(InGrp==1)
		{
			Objs.Start();
			if(IsObjAloneInIdealGroup(Objs()))
				Grps()->Recall=1.0;
			Grps()->Precision=1.0;
			Precision+=Grps()->Precision;
			Recall+=Grps()->Recall;
		}
		else
		{
			ComputeBestLocalRecallPrecision(Objs,Grps(),InGrp);
			Precision+=Grps()->Precision;
			Recall+=Grps()->Recall;
			Grps()->Precision/=(double)InGrp;
			Grps()->Recall/=(double)InGrp;
		}
	}
	if(NbObjs)
	{
		Precision/=(double)NbObjs;
		Recall/=(double)NbObjs;
	}
	DirtyRecallPrecision=false;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj>
	void ClusteringEval<cGroup,cObj>::ComputeAdjustedRandIndex(void)
{
	cGroup* GroupComputed;                  // Pointer to a computed group
	size_t NbRows,NbCols;                   // Rows and Cols for the matrix
	size_t MaxRows,MaxCols;                 // Maximal Rows and Cols for matrix allocation
	size_t NbObjs(0);                       // Total Number of objects
	size_t NbTot;
	size_t col;
	double a(0.0),b(0.0),c(0.0),d(0.0),num,den,subtotal;
	double* VectorRows;                           // Sum of the rows of the matrix
	double* VectorCols;                           // Sum of the columns of the matrix
	double* VectorColsTemp;                       // Temporary sum of the columns of the matrix
	double* ptr;

	// If only one element and one group -> Adjusted Rand Index=1.
	if((Session->GetNbObjs(ObjType)==1)&&(Session->GetNbObjs(GroupType)==1))
	{
		AdjustedRandIndex=1.0;
		return;
	}

	// Initialization part
	AdjustedRandIndex=0.0;

	// Go through the languages to define the maximal sizes and allocate the matrix
	MaxRows=MaxCols=0;
	NbRows=Session->GetNbIdealGroups(ObjType);
	NbCols=Session->GetNbObjs(GroupType);
	if((!NbRows)||(!NbCols))
		return;
	if(NbRows>MaxRows) MaxRows=NbRows;
	if(NbCols>MaxCols) MaxCols=NbCols;
	if((!MaxRows)||(!MaxCols))
		return;
	VectorRows=new double[MaxRows];
	VectorCols=new double[MaxCols];
	VectorColsTemp=new double[MaxCols];

	// Construction of the container for relation between id and column in the matrix.
	RContainer<ClusterId,true,true> ClustersId(NbCols,NbCols/2);
	RCursor<cGroup> Grps(GetClusters());
	for(Grps.Start(),col=0;!Grps.End();Grps.Next())
		ClustersId.InsertPtr(new ClusterId(Grps()->GetId(),col++));

	// Initialization of the vectors
	memset(VectorRows,0,NbRows*sizeof(double));
	memset(VectorCols,0,NbCols*sizeof(double));

	// For each group of ideal group and for each object in this group
	// -> Compute the different terms of the total
	size_t row(0),position;
	RCursor<GSubject> GroupsIdeal(Session->GetSubjects());
	for(GroupsIdeal.Start(),NbTot=0;!GroupsIdeal.End();GroupsIdeal.Next())
	{
		if(!GroupsIdeal()->GetNbObjs(ObjType))
			continue;
		memset(VectorColsTemp,0,NbCols*sizeof(double));
		RCursor<cObj> Prof(GroupsIdeal()->GetObjs(static_cast<cObj*>(0)));
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			if(Prof()->GetGroupId()==cNoRef)
				continue;
			VectorRows[row]++;
			NbTot++;
			GroupComputed=static_cast<cGroup*>(Session->GetObj(GroupType,Prof()->GetGroupId()));
			if(!GroupComputed)
				continue;
			position=ClustersId.GetPtr(GroupComputed->GetId())->position;
			VectorCols[position]++;
			VectorColsTemp[position]++;
		}
		row++;
		for(col=NbCols+1,ptr=VectorColsTemp;--col;ptr++)
			a+=(((*ptr)*((*ptr)-1))/2);
	}

	for(col=NbCols+1,ptr=VectorCols;--col;ptr++)
		b+=(((*ptr)*((*ptr)-1))/2);
	for(row=NbRows+1,ptr=VectorRows;--row;ptr++)
		c+=(((*ptr)*((*ptr)-1))/2);
	d=(static_cast<double>(NbTot)*(static_cast<double>(NbTot)-1))/2;
	num=a-((b*c)/d);
	den=(0.5*(b+c))-(b*c/d);
	if(den)
		subtotal=num/den;
	else
		subtotal=1.0;
	NbObjs+=NbTot;
	AdjustedRandIndex+=subtotal*static_cast<double>(NbTot);

	// Compute Total
	if(NbObjs)
		AdjustedRandIndex=AdjustedRandIndex/static_cast<double>(NbObjs);
	else
		AdjustedRandIndex=0.0;

	//delete the vectors
	if (VectorRows) delete[] VectorRows;
	if (VectorCols) delete[] VectorCols;
	if (VectorColsTemp) delete[] VectorColsTemp;
	DirtyAdjustedRandIndex=false;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	void ClusteringEval<cGroup,cObj>::ComputeJ(void) //tObjType objtype,tObjType grptype,R::RCursor<cGroup>& grps,double& J)
{
	R::RString mes;
	bool Neg(false);          // Correct negative similarities (a priori, no negative similarities)

	// Init
	R::RContainer<cObj,false,false> Centers(Session->GetNbObjs(GroupType));
	J=0;
	switch(ObjType)
	{
		case otDoc:
			mes="Documents";
			break;
		case otProfile:
			mes="Profiles";
			break;
		default:
			ThrowGException(GetObjType(ObjType,true,true)+" are not elements that are grouped");
	}
	GMeasure* Sim(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures",mes+" Similarities"));

	// Compute the average intra-similarity and the centers
	RCursor<cGroup> Grps(GetClusters());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		if(!Grps()->GetNbObjs())
			continue;

		// Verify if the info is updated -> If yes -> continue
		if(Grps()->GetData()->Centroid)
		{
			J+=Grps()->GetData()->AvgSim;
			Centers.InsertPtr(Grps()->GetData()->Centroid);
			continue;
		}

		RCursor<cObj> Objs1(Grps()->GetObjs());
		if(Grps()->GetNbObjs()==1)
		{
			J+=1.0;
			Objs1.Start();
			Grps()->GetData()->AvgSim=1.0;
			Grps()->GetData()->Centroid=Objs1();
			Centers.InsertPtr(Objs1());
			continue;
		}
		Grps()->GetData()->AvgSim=-2.0;
		Grps()->GetData()->Centroid=0;
		RCursor<cObj> Objs2(Grps()->GetObjs());
		for(Objs1.Start();!Objs1.End();Objs1.Next())
		{
			if(!Objs1()->IsDefined()) continue;
			double intrasim(0.0);
			size_t nbintra(0);
			for(Objs2.Start();!Objs2.End();Objs2.Next())
			{
				double tmp;
				if(!Objs2()->IsDefined()) continue;
				if(Objs1()==Objs2()) continue;
				nbintra++;
				Sim->Measure(0,Objs1()->GetId(),Objs2()->GetId(),&tmp);
				if(tmp<0.0)
					Neg=true;
				intrasim+=tmp;
			}
			intrasim/=static_cast<double>(nbintra);
			if(intrasim>Grps()->GetData()->AvgSim)
			{
				Grps()->GetData()->AvgSim=intrasim;
				Grps()->GetData()->Centroid=Objs1();
			}
		}
		J+=Grps()->GetData()->AvgSim;
		if(!Grps()->GetData()->Centroid)
			throw GException("ClusteringEval<cGroup,cObj>::ComputeJ(void) : Cluster "+RString::Number(Grps()->GetId())+" has no objects");
		Centers.InsertPtr(Grps()->GetData()->Centroid);
	}

	// Compute the maximal similarity between the centers
	RCursor<cObj> Objs1(Centers);
	RCursor<cObj> Objs2(Centers);
	size_t nb(Centers.GetNb()-1);
	double max(-2.0);
	for(Objs1.Start();!Objs1.End();Objs1.Next())
	{
		double avg(0.0);
		for(Objs2.Start();!Objs2.End();Objs2.Next())
		{
			double tmp;
			if(Objs1()==Objs2()) continue;
			Sim->Measure(0,Objs1()->GetId(),Objs2()->GetId(),&tmp);
			if(tmp<0.0)
				Neg=true;
			avg+=tmp;
		}
		avg/=static_cast<double>(nb);
		if(avg>max)
			max=avg;
	}

	// Correction if negative values
	if(Neg)
	{
		max=(max+1.0)/2.0;
		J=(J+1.0)/2.0;
	}

	// Compute J
	J/=(static_cast<double>(Centers.GetNb())*max);
	DirtyJ=false;
}
