/*

	GALILEI Research Project

	GMatrixMeasure.cpp

	Measure represented by a matrix of values - Implementation.

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



//------------------------------------------------------------------------------
// include files for C/C++ ANSI
#include <cmath>
#include <limits>


//------------------------------------------------------------------------------
// include files for R
#include <rsparsematrix.h>
#include <rmatrix.h>
#include <rsymmetricmatrix.h>
#include <rsparsesymmetricmatrix.h>
#include <rmaxmatrix.h>
#include <rdir.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmatrixmeasure.h>
#include <glang.h>
#include <gsession.h>
#include <gprofile.h>
#include <gdoc.h>
#include <ggroup.h>
#include <ggalileiapp.h>
#include <gcommunity.h>
#include <gtopic.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// Debug dependent variable
//#define DEBUG
#ifdef DEBUG
	RTextFile DebugFile("/home/pfrancq/DebugMeasure.txt");
#endif



//------------------------------------------------------------------------------
//
//  GMatrixMeasure
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMatrixMeasure::GMatrixMeasure(GSession* session,GPlugInFactory* fac,tObjType lines,tObjType cols,bool sym)
	: RObject(fac->GetName()), GMeasure(session,fac), Type(Full), Symmetric(sym), Matrix(0), Storage(),
	  Lines(lines), Cols(cols)
{
	if(Symmetric&&(Lines!=Cols))
		ThrowGException("Symmetric measures are only allowed if the elements are of the same type");
	Name=fac->GetName();

	switch(Lines)
	{
		case otDoc:
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineNew),eNewDoc);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineModified),eDocModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDel),eDelDoc);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDestroy),eDestroyDoc);
			break;

		case otTopic:
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineNew),eNewTopic);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineModified),eTopicModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDel),eDelTopic);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDestroy),eDestroyTopic);
			break;

		case otProfile:
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineNew),eNewProfile);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineModified),eProfileModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDel),eDelProfile);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDestroy),eDestroyProfile);
			break;

		case otCommunity:
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineNew),eNewCommunity);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineModified),eCommunityModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDel),eDelCommunity);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDestroy),eDestroyCommunity);
			break;

		case otClass:
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineNew),eNewClass);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineModified),eClassModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDel),eDelClass);
			InsertObserver(HANDLER(GMatrixMeasure::HandleLineDestroy),eDestroyClass);
			break;

		default:
			ThrowGException(GetObjType(Lines,true,true)+"are not allowed");
	}

	switch(Cols)
	{
		case otDoc:
			InsertObserver(HANDLER(GMatrixMeasure::HandleColNew),eNewDoc);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColModified),eDocModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDel),eDelDoc);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDestroy),eDestroyDoc);
			break;

		case otTopic:
			InsertObserver(HANDLER(GMatrixMeasure::HandleColNew),eNewTopic);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColModified),eTopicModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDel),eDelTopic);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDestroy),eDestroyTopic);
			break;

		case otProfile:
			InsertObserver(HANDLER(GMatrixMeasure::HandleColNew),eNewProfile);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColModified),eProfileModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDel),eDelProfile);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDestroy),eDestroyProfile);
			break;

		case otCommunity:
			InsertObserver(HANDLER(GMatrixMeasure::HandleColNew),eNewCommunity);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColModified),eCommunityModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDel),eDelCommunity);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDestroy),eDestroyCommunity);
			break;

		case otClass:
			InsertObserver(HANDLER(GMatrixMeasure::HandleColNew),eNewClass);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColModified),eClassModified);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDel),eDelClass);
			InsertObserver(HANDLER(GMatrixMeasure::HandleColDestroy),eDestroyClass);
			break;

		default:
			ThrowGException(GetObjType(Cols,true,true)+"are not allowed");
	}

#ifdef DEBUG
	DebugFile.Open(RIO::Create);
#endif
}


//-----------------------------------------------------------------------------
void GMatrixMeasure::SetElementsType(bool sym,tObjType lines,tObjType cols)
{
	if(sym&&(lines!=cols))
		throw GException("Symmetric measures are only allowed if the elements are of the same type");
	Symmetric=sym;
	Lines=lines;
	Cols=cols;
}


//------------------------------------------------------------------------------
RString GMatrixMeasure::GetRootDir(void) const
{
	RString Cat(GetFactory()->GetList());
	Cat.Replace('/','-');
	return(RString(Dir+RFile::GetDirSeparator()+Session->GetName()+RFile::GetDirSeparator()+Cat));
}


//------------------------------------------------------------------------------
RString GMatrixMeasure::GetFilesName(void) const
{
	RString FileName(Name);
	switch(Type)
	{
		case Full:
			if(Symmetric)
				FileName+="_FS";
			else
				FileName+="_F";
			break;
		case Sparse:
			if(Symmetric)
				FileName+="_SS";
			else
				FileName+="_S";
			break;
		case NearestNeighbors:
			FileName+="_NN";
			break;
	}
	return(FileName);
}


//-----------------------------------------------------------------------------
void GMatrixMeasure::SetForceCompute(bool compute)
{
	ForceCompute=compute;
}


//-----------------------------------------------------------------------------
void GMatrixMeasure::ApplyConfig(void)
{
	// Parameters that can always be changed
	DeviationRate=FindParam<RParamValue>("DeviationRate")->GetDouble();
	MinMeasure=FindParam<RParamValue>("MinMeasure")->GetDouble();
	AutomaticMinMeasure=FindParam<RParamValue>("AutomaticMinMeasure")->GetBool();
	CutoffFrequency=FindParam<RParamValue>("Cutoff Frequency")->GetDouble();

	// If matrix managed nearest neighbors and their number have changed -> matrix is dirty
	size_t NewNbNearest(FindParam<RParamValue>("NbNearest")->GetUInt());
	size_t NewNbSamples(FindParam<RParamValue>("NbSamples")->GetUInt());
	if((Session)&&((Type==NearestNeighbors)&&((NewNbNearest!=NbNearest)||(NewNbSamples!=NbSamples))))
	{
		ChangeSize=true;
		DirtyMem=true;
	}
	NbNearest=NewNbNearest;
	NbSamples=NewNbSamples;

	// Parameters needing the reloading of the session
	InMemory=FindParam<RParamValue>("Memory")->GetBool();
	InStorage=FindParam<RParamValue>("Storage")->GetBool();
	Dir=FindParam<RParamValue>("Dir")->Get();
	switch(FindParam<RParamValue>("Type")->GetInt())
	{
		case Full:
			Type=Full;
			break;
		case Sparse:
			Type=Sparse;
			break;
		case NearestNeighbors:
			Type=NearestNeighbors;
			break;
		default:
			ThrowGException("Type '"+FindParam<RParamValue>("Type")->Get()+"' is not a valid type");
	}
	ForceCompute=FindParam<RParamValue>("ForceCompute")->GetBool();
	FastNN=FindParam<RParamValue>("FastNN")->GetBool();
}


//------------------------------------------------------------------------------
void GMatrixMeasure::Init(void)
{
	if(InStorage)
	{
		// Create (if necessary) the directory that will contained the file
		RString place(GetRootDir());
		RDir::CreateDirIfNecessary(place,true);
		place+=RFile::GetDirSeparator()+GetFilesName();

		// Open the file
		RGenericMatrix::tType matrixtype;
		switch(Type)
		{
			case Full:
				if(Symmetric)
					matrixtype=RGenericMatrix::tSymmetric;
				else
					matrixtype=RGenericMatrix::tNormal;
				break;
			case Sparse:
				if(Symmetric)
					matrixtype=RGenericMatrix::tSparseSymmetric;
				else
					matrixtype=RGenericMatrix::tSparse;
				break;
			case NearestNeighbors:
				matrixtype=RGenericMatrix::tMax;
				break;
		}
		Storage.Open(place,matrixtype);

		// Look if the storage contains information
		if(Storage.HasInfo())
		{
			// Yes -> Matrix was computed previously
			size_t LineStorageMatrix(Storage.GetNbLines());
			if(LineStorageMatrix)
				MaxIdLine=LineStorageMatrix-1;  // There are some line -> maximum identifier is line-1 (id=1 -> line=0)
			else
				MaxIdLine=0;     // No elements
			if(Type==NearestNeighbors)
				MaxIdCol=NbNearest;
			else
				MaxIdCol=Storage.GetNbCols()-1;

			Storage.ReadInfo(0,(char*)&NbValues,sizeof(size_t));
			Storage.ReadInfo(sizeof(size_t),(char*)&Mean,sizeof(double));
			Storage.ReadInfo(sizeof(size_t)+sizeof(double),(char*)&Deviation,sizeof(double));
			Storage.ReadInfo(sizeof(size_t)+2*sizeof(double),(char*)&DirtyMem,sizeof(bool));
		}
		else
		{
			// No -> New matrix
			MaxIdLine=NbValues=0;
			Mean=Deviation=0.0;
			if(Type==NearestNeighbors)
				MaxIdCol=NbNearest;
			else
				MaxIdCol=0;
		}
	}
	else
	{
		// New matrix
		MaxIdLine=NbValues=0;
		Mean=Deviation=0.0;
		if(Type==NearestNeighbors)
			MaxIdCol=NbNearest;
		else
			MaxIdCol=0;
	}
	ChangeSize=false;   // Suppose matrix has the correct size.
	DirtyFile=false;    // Suppose the matrix is OK in the storage.
	DirtyMem=false;     // Suppose the matrix is OK in memory.
}


//------------------------------------------------------------------------------
void GMatrixMeasure::ReInit(void)
{
	if(InMemory)
	{
		if(Matrix)
			Matrix->Clear();
		DirtyMem=true;
	}
	if(InStorage)
	{
		if(!InMemory)
			Storage.Clear();  // <- If in memory, the file will be modified at the end
		DirtyFile=true;
	}
	Deviation=Mean=0.0;
	NbValues=0;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::Measure(size_t measure,...)
{
	va_list ap;
	va_start(ap,measure);
	size_t id1(va_arg(ap,size_t));  // Element id1 correspond to line id1-1
	size_t id2(va_arg(ap,size_t));  // Element id2 correspond to column id2-1
	double* res(va_arg(ap,double*));
	va_end(ap);

	if((Type==NearestNeighbors)||((!InMemory)&&(!InStorage)))
	{
		// Recompute the element if not in memory of not in storage, or if it a nearest neighbors matrix
		GObject* obj1=Session->GetObj(Lines,id1);
		GObject* obj2=Session->GetObj(Cols,id2);
		(*res)=Compute(obj1,obj2);
		if(abs(*res)<CutoffFrequency)
			(*res)=0.0;         // High-pass filter
	}
	else if(InMemory)
	{
		// Search in memory

		// If necessary, the matrix is created or changed
		if(!Matrix)
			InitMatrix();
		else if(ChangeSize)
			ChangeMemSize();

		// If the matrix is not full and is dirty -> It must be re-computed first
		if(DirtyMem&&Type!=Full)
			UpdateMem();

		// Take the element from the matrix (static_cast is to ensure the call of the const operator).
		(*res)=(*static_cast<const RGenericMatrix*>(Matrix))(id1-1,id2-1);

		// If NAN or ForceCompute=true -> Recomputing is necessary (Verification in the case of the full matrix)
		if(((*res)!=(*res))||ForceCompute)
		{
			GObject* obj1=Session->GetObj(Lines,id1);
			GObject* obj2=Session->GetObj(Cols,id2);
			(*res)=Compute(obj1,obj2);
			(*Matrix)(id1-1,id2-1)=(*res);
			if(fabs(*res)<CutoffFrequency)
				(*res)=0.0;     // High-pass filter
			AddValue(*res);
			if(InStorage)
				DirtyFile=true;  // <- If in memory, the file will be modified at the end
		}
	}
	else if(InStorage)
	{
		 // Search in the storage

		// If necessary, the storage is extended
		if(ChangeSize)
			ChangeStorageSize();

		// If the matrix is not full and is dirty -> It must be re-computed first
		if(DirtyFile&&Type!=Full)
			UpdateStorage();

		// Read the value from the storage
		(*res)=Storage.Read(id1-1,id2-1);

		// If NAN or ForceCompute=true -> recomputing is necessary (Verification in the case of the full matrix)
		if(((*res)!=(*res))||ForceCompute)
		{
			GObject* obj1=Session->GetObj(Lines,id1);
			GObject* obj2=Session->GetObj(Cols,id2);
			(*res)=Compute(obj1,obj2);
			if(abs(*res)<CutoffFrequency)
				(*res)=0.0;     // High-pass filter
			AddValue(*res);
			Storage.Write(id1-1,id2-1,*res);
		}
	}
}


//------------------------------------------------------------------------------
void GMatrixMeasure::Info(size_t info,...)
{
	va_list ap;
	va_start(ap,info);

	if(info==3)
	{
		size_t id(va_arg(ap,size_t));
		const RMaxVector** Vec(va_arg(ap,const RMaxVector**));
		va_end(ap);

		if(Type!=NearestNeighbors)
			ThrowGException("Cannot ask for nearest neighbors if the matrix is not configured to it");

		if(InMemory)
		{
			// Take the statistics from memory and update it if necessary
			if(!Matrix)
				InitMatrix();
			if(ChangeSize)
				ChangeMemSize();
			if(DirtyMem)
				UpdateMem();
			(*Vec)=((*static_cast<const RMaxMatrix*>(Matrix))[id-1]);
		}
		else
			ThrowGException("Nearest neighbors matrix must be stored in memory");
	}
	else
	{
		double* res(va_arg(ap,double*));
		va_end(ap);

		if(info>2)
			ThrowGException("'"+RString::Number(info)+"' not a valid information");

		if(!AutomaticMinMeasure)
		{
			(*res)=MinMeasure;
			return;
		}

		if(InMemory)
		{
			// Take the statistics from memory and update it if necessary
			if(!Matrix)
				InitMatrix();
			if(ChangeSize)
				ChangeMemSize();
			if(DirtyMem)
				UpdateMem();
		}
		else if(InStorage)
		{
			// Take the statistics from the storage and update it if necessary
			if(ChangeSize)
				ChangeStorageSize();
			if(DirtyFile)
				UpdateStorage();
		}
		else
		{
			// Initialize
			NbValues=0;
			Mean=Deviation=0;
			size_t nblines=Session->GetMaxObjId(Lines)+1;
			if(nblines==1)
			{
				(*res)=0.0;
				return;
			}

			// Go through all elements
			for(size_t i=1;i<nblines;i++)
			{
				GObject* obj1=Session->GetObj(Lines,i,true);
				if(!obj1)
					continue;

				// Compute number of columns for this line
				size_t nbcols;
				if(Symmetric)
					nbcols=i+1;
				else
					nbcols=Session->GetMaxObjId(Cols)+1;

				for(size_t j=1;j<nbcols;j++)
				{
					GObject* obj2=Session->GetObj(Cols,j,true);
					if(!obj2)
						continue;
					NbValues+=1.0;
					double sim(Compute(obj1,obj2));
					if(sim<CutoffFrequency)
						sim=0.0;
					Mean+=sim;
					Deviation+=sim*sim;
				}
			}
			if(NbValues>0.0)
			{
				Mean/=static_cast<double>(NbValues);
				Deviation=(Deviation/static_cast<double>(NbValues))-(Mean*Mean);
			}
		}

		// Look at information needed
		switch(info)
		{
			case 0:
				(*res)=Mean+DeviationRate*sqrt(Deviation);
				if((*res)>1.0)
					(*res)=1.0;
				if((*res)<-1.0)
					(*res)=-1.0;
				break;
			case 1:
				(*res)=Mean;
				break;
			case 2:
				(*res)=sqrt(Deviation);
				break;
		}
	}
}


//------------------------------------------------------------------------------
void GMatrixMeasure::InitMatrix(void)
{
	// Look first if the matrix must be loaded from the storage
	bool LoadStorage;
	size_t InitLine,InitCol;
	if(InStorage&&Storage.GetNbLines()&&Storage.GetNbCols())
	{
		LoadStorage=true;
		InitLine=Storage.GetNbLines();
		InitCol=Storage.GetNbCols();
	}
	else
	{
		LoadStorage=false;
		InitLine=MaxIdLine;
		InitCol=MaxIdCol;
	}

	switch(Type)
	{
		case Full:
			if(Symmetric)
				Matrix=new RSymmetricMatrix(InitLine);
			else
				Matrix=new RMatrix(InitLine,InitCol);
			if(!LoadStorage)
				Matrix->Init(NAN);
			break;
		case Sparse:
			if(Symmetric)
				Matrix=new RSparseSymmetricMatrix(InitLine,true,20);
			else
				Matrix=new RSparseMatrix(InitLine,InitCol,true,20);
			break;
		case NearestNeighbors:
			Matrix=new RMaxMatrix(InitLine,InitCol,true,NbNearest);
			break;
	}

	// If valid storage -> Load the matrix and extend it eventually
	if(LoadStorage)
	{
		Storage.Load(*Matrix);
		if((InitLine!=MaxIdLine)||(InitCol!=NbNearest))
			ChangeMemSize();
		else
			DirtyMem=false;
	}
	else
		DirtyMem=true;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::ChangeMemSize(void)
{
	// Verify if it must be extended
	Matrix->VerifySize(MaxIdLine,MaxIdCol,true);

	// Memory was extended (if needed) and is dirty
	ChangeSize=false;
	DirtyMem=true;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::ChangeStorageSize(void)
{
	// Verify if the matrix must be extended
	Storage.VerifySize(MaxIdLine,NbNearest,true);

	// File was extended (if needed) and is dirty
	ChangeSize=false;
	DirtyFile=true;
}


//------------------------------------------------------------------------------
size_t GMatrixMeasure::GetNbDiffElements(void)
{
	if(Lines==Cols)
		return(Session->GetNbObjs(Lines));
	return(Session->GetNbObjs(Lines)+Session->GetNbObjs(Cols));
}


//------------------------------------------------------------------------------
void GMatrixMeasure::AddIdentificator(size_t id,bool line)
{
	if(line)
	{
		// Verify if a line must be added in memory or in files
		if(MaxIdLine<id)
		{
			MaxIdLine=id;
			if(InMemory)
			{
				if(Matrix&&Matrix->GetNbLines()<=id)
					ChangeSize=true;
			}
			else if(InStorage&&(Storage.GetNbLines()<=id))
				ChangeSize=true;
		}
	}
	else
	{
		if(Type==NearestNeighbors)
			return;       // Nothing to do

		// Verify if a column must be added in memory or in files (only if not nearest neighbors)
		if(MaxIdCol<id)
		{
			MaxIdCol=id;
			if(InMemory)
			{
				if(Matrix&&(Matrix->GetNbCols()<=id))
					ChangeSize=true;
			}
			else if(InStorage&&(Storage.GetNbCols()<=id))
				ChangeSize=true;
		}
	}
}


//------------------------------------------------------------------------------
void GMatrixMeasure::DirtyIdentificator(size_t id,bool line,bool file)
{
	// If the matrix is not fully managed, it is just declare dirty.
	if(Type!=Full)
	{
		DirtyFile=DirtyMem=true;
		return;
	}

	if(line)
	{
		// Line id-1 must be made dirty

		if(InMemory)
		{
			if((!Matrix)||(id>Matrix->GetNbLines()))
				return;  // Nothing to do

			// Identifier correspond to a line in memory -> Line "must" be removed.
			// RGeneric Matrix is a RMatrix (or derived).
			RVector* Line((*static_cast<RMatrix*>(Matrix))[id-1]);
			RNumCursor<double> Cols(Line->GetCols());
			for(Cols.Start();!Cols.End();Cols.Next())
				DeleteValue(Cols());
			DirtyMem=true;
		}
		else if(file&&InStorage&&(id<=Storage.GetNbLines()))
		{
			// Number of columns to dirty in the file
			size_t NbCols(Storage.GetNbCols(id-1));
			for(size_t j=0;j<NbCols;j++)
			{
				if(InMemory)
					Storage.Write(id-1,j,NAN);  // Simply write NAN in the file since it was already modified
				else
				{
					// Must be updated
					double val(Storage.Read(id-1,j));
					if(val==val)
					{
						// Val is not NAN -> Update it
						DeleteValue(val);
						Storage.Write(id-1,j,val);
					}
				}
			}
			DirtyFile=true;
		}
	}
	else
	{
		// Nothing to do for nearest neighbors
		if(Type==NearestNeighbors)
			return;

		// Column id-1 must be made dirty

		// Goes for the rest of lines
		if(InMemory)
		{
			if((!Matrix)||(id>Matrix->GetNbCols()))
					return; // Nothing to do

			// Look where the column "starts"
			size_t start;
			if(Symmetric)
				start=id-1;
			else
				start=0;

			// Identifier correspond to a line in memory -> Line "must" be removed.
			// RGeneric Matrix is a RMatrix (or derived).
			RCursor<RVector> Lines(static_cast<RMatrix*>(Matrix)->GetLines());
			for(Lines.GoTo(start);!Lines.End();Lines.Next())
				DeleteValue((*Lines())[id-1]);
			DirtyMem=true;
		}
		else if(file&&InStorage&&(id<=Storage.GetNbCols()))
		{
			// Look where the column "starts"
			size_t start;
			if(Symmetric)
				start=id-1;
			else
				start=0;

			// Number of lines to dirty in the file
			for(size_t i=start;i<Storage.GetNbLines();i++)
			{
				if(InMemory)
					Storage.Write(i,id-1,NAN);  // Simply write NAN in the file since it was already modified
				else
				{
					// Must be updated
					double val(Storage.Read(i,id-1));
					if(val==val)
					{
						// Val is not NAN -> Update it
						DeleteValue(val);
						Storage.Write(i,id-1,val);
					}
				}
			}
			DirtyFile=true;
		}
	}
}


//------------------------------------------------------------------------------
void GMatrixMeasure::DeleteIdentificator(size_t id,bool line)
{
	// Remove all values
	DirtyIdentificator(id,line,false);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::DestroyIdentificator(size_t id,bool line)
{
	// Remove all values
	DirtyIdentificator(id,line,true);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::HandleLineNew(const RNotification& notification)
{
	GObject* Object(dynamic_cast<GObject*>(notification.GetSender()));
	if(Object)
		AddIdentificator(Object->GetId(),true);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::HandleLineModified(const RNotification& notification)
{
	GObject* Object(dynamic_cast<GObject*>(notification.GetSender()));
	if(Object)
		DirtyIdentificator(Object->GetId(),true,true);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::HandleLineDel(const RNotification& notification)
{
	GObject* Object(dynamic_cast<GObject*>(notification.GetSender()));
	if(Object)
		DeleteIdentificator(Object->GetId(),true);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::HandleLineDestroy(const RNotification& notification)
{
	GObject* Object(dynamic_cast<GObject*>(notification.GetSender()));
	if(Object)
		DestroyIdentificator(Object->GetId(),true);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::HandleColNew(const RNotification& notification)
{
	GObject* Object(dynamic_cast<GObject*>(notification.GetSender()));
	if(Object)
		AddIdentificator(Object->GetId(),false);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::HandleColModified(const RNotification& notification)
{
	GObject* Object(dynamic_cast<GObject*>(notification.GetSender()));
	if(Object)
		DirtyIdentificator(Object->GetId(),false,true);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::HandleColDel(const RNotification& notification)
{
	GObject* Object(dynamic_cast<GObject*>(notification.GetSender()));
	if(Object)
		DirtyIdentificator(Object->GetId(),false,true);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::HandleColDestroy(const RNotification& notification)
{
	GObject* Object(dynamic_cast<GObject*>(notification.GetSender()));
	if(Object)
		DestroyIdentificator(Object->GetId(),false);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::UpdateSparse(void)
{
	// Clear the matrix and the file (if necessary) and re-initialize the statistics
	if(InMemory)
		Matrix->Clear();
	if(InStorage)
		Storage.Clear();
	Deviation=Mean=0.0;
	NbValues=0;

	// Recompute everything
	for(size_t i=0;i<MaxIdLine;i++)
	{
		GObject* obj1=Session->GetObj(Lines,i+1,true);
		if(!obj1)
			continue;
		size_t max;
		if(Symmetric)
			max=i+1;
		else
			max=MaxIdCol;

		for(size_t j=0;j<max;j++)
		{
			GObject* obj2=Session->GetObj(Cols,j+1,true);
			if(!obj2)
				continue;

			// Compute the measure, apply the high-pass filter. If null, nothing is stored
			double res(Compute(obj1,obj2));
			if(abs(res)<CutoffFrequency)
				res=0.0;
			AddValue(res);
			if(res==0.0)
				continue;

			// Store the value in matrix in the file depending
			if(InMemory)
				(*Matrix)(i,j)=res;
			else if(InStorage)
				Storage.Write(i,j,res);
		}
	}
}


//------------------------------------------------------------------------------
// Order a array of RValue by values
int CompareValues(const void* a, const void* b)
{
/*	double da((*((RValue**)a))->Value);
	double db((*((RValue**)b))->Value);*/
	double da(((RValue*)a)->Value);
	double db(((RValue*)b)->Value);

	if(da>db)
		return(-1.0);
	else if(da<db)
		return(1.0);
	else
		return(0);
}


//------------------------------------------------------------------------------
bool IsIn(size_t id,RMaxValue* tab,size_t nb)
{
	size_t i;
	RMaxValue* ptr;
	for(i=nb+1,ptr=tab;--i;ptr++)
		if(ptr->Id==id)
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
void Print(RMaxValue* tab,size_t nb)
{
	size_t i;
	RMaxValue* ptr;
	cout<<"    ";
	for(i=nb+1,ptr=tab;--i;ptr++)
		cout<<ptr->Id<<" - ";
	cout<<endl;
}



//------------------------------------------------------------------------------
void Print(RMaxVector* vec)
{
	cout<<"    ";
	RCursor<RMaxValue> Value(*vec);
	for(Value.Start();!Value.End();Value.Next())
		cout<<Value()->Id<<" - ";
	cout<<endl;
}


//------------------------------------------------------------------------------
bool Verify(RMaxVector* vec)
{
	RCursor<RMaxValue> Value(*vec,0,vec->GetNb()-2);
	for(Value.Start();!Value.End();Value.Next())
	{
		RCursor<RMaxValue> Value2(*vec,Value.GetPos()+1,vec->GetNb()-1);
		for(Value2.Start();!Value2.End();Value2.Next())
			if(Value()->Id==Value2()->Id)
			{
				cout<<"        "<<Value()->Id<<" is twice :"<<Value()->Value<<" and "<<Value2()->Value<<endl;
				return(false);
			}
	}
	return(true);
}


//------------------------------------------------------------------------------
inline size_t Fill(RMaxValue* tab,RNumContainer<size_t,true>& tab2,const RMaxVector* vec)
{
	tab2.Clear();
	RMaxValue* ptr;
	RCursor<RMaxValue> Value(*vec);
	for(Value.Start(),ptr=tab;!Value.End();Value.Next(),ptr++)
	{
		(*ptr)=(*Value());
		tab2.Insert(Value()->Id);
	}
	return(Value.GetNb());
}

//------------------------------------------------------------------------------
inline void Fill(RNumContainer<size_t,true>& tab,const RMaxVector* vec)
{
	tab.Clear();
	RCursor<RMaxValue> Value(*vec);
	for(Value.Start();!Value.End();Value.Next())
		tab.Insert(Value()->Id);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::UpdateNearestNeighborsRAM(void)
{
	// If not in memory -> Not computed
	if(!InMemory)
		throw GException("GMatrixMeasure::UpdateNearestNeighbors(void) : Not implemented to work with storage only");

	// Get the elements
	GObject** Elements;
	size_t NbElements(Session->GetObjs(Cols,Elements,true));

	// Compute the number of nearest neighbor to compute
	if(NbNearest>NbElements-1)
		NbNearest=NbElements-1;
	if(NbSamples<NbNearest)
		NbSamples=2*NbNearest;
	if(NbSamples>NbElements-1)
		NbSamples=NbElements-1;
	if(NbSamples<NbNearest)
		ThrowGException("NbSamples<=NbNearest");

	// Structures
	RMaxValue* CurSample;
	RMaxValue* Samples=new RMaxValue[NbSamples];        // Samples
	RNumContainer<size_t,true> Neighbors(NbSamples);

	// Verify if all elements of Matrix have enough neighbors, if not fill it randomly
	cout<<"Pass 0"<<endl;
	RCursor<RMaxVector> Element(dynamic_cast<RMaxMatrix*>(Matrix)->GetLines());
	for(Element.Start();!Element.End();Element.Next())
	{
		// Valid element ?
		GObject* obj1=Session->GetObj(Lines,Element.GetPos()+1,false);
		if(!obj1)
			continue;

		// Enough neighbors
		if(Element()->GetNb()==NbNearest)
			continue;

		// Fill the rest with randomly chosen elements
		Fill(Neighbors,Element());
		Session->GetRandom().RandOrder(Elements,NbElements);
		GObject** ptr;
		size_t i;
		for(ptr=Elements,i=0;(Element()->GetNb()<NbNearest)&&(i<NbElements);i++,ptr++)
		{
			// A null pointer must be skipped
			if(!(*ptr))
				continue;

			// Get the identifier
			size_t Id(GetId(*ptr,false));

			// Verify that is not the same element and that a same element is twice in the system
			if((Symmetric&&(Id==Element.GetPos()+1))||(Neighbors.IsIn(Id)))
				continue;

			// Compute the measure, apply the high-pass filter.
			GObject* obj2=Session->GetObj(Cols,Id,true);
			double Mes(Compute(obj1,obj2));
			if(abs(Mes)<CutoffFrequency)
				Mes=0.0;
			AddValue(Mes);
			Element()->InsertPtrAt(new RMaxValue(Id,Mes),Element()->GetNb());
			Neighbors.Insert(Id);
		}
		Element()->ReOrder();
	}

	// While changes find new neighbors by comparing with those of already neighbors
	size_t NbPass(1);
	for(bool Cont=true;Cont&&(NbPass<30);)
	{
		// Suppose no changes
		cout<<"Pass "<<NbPass++<<endl;
		Cont=false;

		// Goes to each element of the matrix
		for(Element.Start();!Element.End();Element.Next())
		{
			// Valid element ?
			GObject* obj1=Session->GetObj(Lines,Element.GetPos()+1,false);
			if(!obj1)
				continue;

			// Fill Samples with the NbNearest of element
			size_t CurNbSamples(Fill(Samples,Neighbors,Element()));
			if(CurNbSamples!=NbNearest)
				ThrowGException("Problem "+RString::Number(Element()->GetNb())+"!="+RString::Number(NbNearest));

			// Fill the rest with the nearest neighbors of the nearest neighbors
			size_t CurRank(0);   // Current position treated in each neighbors
			RCursor<RMaxValue> Neighbor(*Element());
			for(Neighbor.Start(),CurSample=&Samples[CurNbSamples];(CurNbSamples<NbSamples)&&(CurRank<NbNearest);)
			{
				size_t Id(((*dynamic_cast<RMaxMatrix*>(Matrix))[Neighbor()->Id-1])->GetValue(CurRank)->Id);

				// Verify if it is a valid new nearest neighbor ?
				if(((Symmetric)&&(Id!=Element.GetPos()+1))&&(!Neighbors.IsIn(Id)))
				{
					Neighbors.Insert(Id);
					CurSample->Id=Id;
					GObject* obj2=Session->GetObj(Cols,Id,true);
					CurSample->Value=Compute(obj1,obj2);
					if(abs(CurSample->Value)<CutoffFrequency)
						CurSample->Value=0.0;
					AddValue(CurSample->Value);
					CurSample++;
					CurNbSamples++;
				}

				// Next nearest neighbor
				Neighbor.Next();
				if(Neighbor.End())
				{
					// All the k first neighbors of the nearest neighbors were treated
					Neighbor.Start();
					CurRank++;
				}
			}

			// Sort in descending order
			qsort(Samples,CurNbSamples,sizeof(RValue),CompareValues);

			// Update the neighbors ?
			for(Neighbor.Start(),CurSample=Samples;!Neighbor.End();Neighbor.Next(),CurSample++)
			{
				// If the nearest neighbors have no changed, the identifiers should be identical
				if(Neighbor()->Id!=CurSample->Id)
				{
					(*Neighbor())=(*CurSample);
					Cont=true;
				}
			}
		}
	}
	cout<<"Finish"<<endl;

	// Deallocation
	delete[] Samples;
	delete[] Elements;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::UpdateNearestNeighborsFast(void)
{
	// If not in memory -> Not computed
	if(!InMemory)
		throw GException("GMatrixMeasure::UpdateNearestNeighbors(void) : Not implemented to work with storage only");

	// Get the elements
	GObject** Elements;
	size_t NbElements(Session->GetObjs(Cols,Elements,true));

	// Compute the number of nearest neighbor to compute
	if(NbNearest>NbElements-1)
		NbNearest=NbElements-1;
	if(NbSamples<NbNearest)
		NbSamples=2*NbNearest;
	if(NbSamples>NbElements-1)
		NbSamples=NbElements-1;
	if(NbSamples<NbNearest)
		ThrowGException("NbSamples<=NbNearest");

	// Structures
	RMaxMatrix Samples(dynamic_cast<RMaxMatrix*>(Matrix)->GetNbLines(),NbElements,true,NbSamples);
	RNumContainer<size_t,true> Neighbors(NbSamples);

	// Parse Matrix and fill Samples, verify if all elements have enough neighbors, if not fill it randomly
	cout<<"Pass 0"<<endl;
	RCursor<RMaxVector> Element(dynamic_cast<RMaxMatrix*>(Matrix)->GetLines());
	RCursor<RMaxVector> Sample(Samples.GetLines());
	for(Element.Start(),Sample.Start();!Element.End();Element.Next(),Sample.Next())
	{
		// Valid element ?
		GObject* obj1=Session->GetObj(Lines,Element.GetPos()+1,false);
		if(!obj1)
			continue;

		// Copy Elements() into Sample()
		(*Sample())=(*Element());

		// Fill the rest with randomly chosen elements
		Fill(Neighbors,Sample());
		Session->GetRandom().RandOrder(Elements,NbElements);
		GObject** ptr;
		size_t i;
		for(ptr=Elements,i=0;(Sample()->GetNb()<NbSamples)&&(i<NbElements);i++,ptr++)
		{
			// A null pointer must be skipped
			if(!(*ptr))
				continue;

			// Get the identifier
			size_t Id(GetId(*ptr,false));

			// Verify that is not the same element and that a same element is not twice in the system
			if((Symmetric&&(Id==Element.GetPos()+1))||(Neighbors.IsIn(Id)))
				continue;

			// Compute the measure, apply the high-pass filter.
			GObject* obj2=Session->GetObj(Cols,Id,true);
			double Mes(Compute(obj1,obj2));
			if(abs(Mes)<CutoffFrequency)
				Mes=0.0;
			AddValue(Mes);
			Sample()->InsertPtrAt(new RMaxValue(Id,Mes),Sample()->GetNb());
			Neighbors.Insert(Id);
		}
		Sample()->ReOrder();
	}

	// While changes find new neighbors by comparing with those of already neighbors
	Element=Samples.GetLines();  // Work now on the samples matrix
	size_t NbPass(1);
	for(bool Cont=true;Cont&&(NbPass<30);)
	{
		// Suppose no changes
		cout<<"Pass "<<NbPass++<<endl;
		Cont=false;

		// Goes to each element of the matrix
		for(Element.Start();!Element.End();Element.Next())
		{
			// Valid element ?
			GObject* obj1=Session->GetObj(Lines,Element.GetPos()+1,false);
			if(!obj1)
				continue;

			// Fill Samples with the NbNearest of element
			Fill(Neighbors,Element());

			// Fill the rest with the nearest neighbors of the nearest neighbors
			size_t CurNbSamples(NbSamples-NbNearest+1); // Current number of samples treated
			size_t CurRank(0);                          // Current position treated in each neighbors
			RCursor<RMaxValue> Neighbor(*Element());    // Cursor over the neighbors of Element()
			for(Neighbor.Start();(--CurNbSamples)&&(CurRank<NbNearest);)
			{
				size_t Id((Samples[Neighbor()->Id-1])->GetValue(CurRank)->Id);

				// Verify if it is a valid new nearest neighbor ?
				if(((Symmetric)&&(Id!=Element.GetPos()+1))&&(!Neighbors.IsIn(Id)))
				{
					// Compute the similarity
					GObject* obj2=Session->GetObj(Cols,Id,true);
					double Value(Compute(obj1,obj2));
					if(abs(Value)<CutoffFrequency)
						Value=0.0;
					AddValue(Value);

					// Replace it a given value
					size_t ReplaceId(Id);
					size_t Pos(Element()->Replace(Value,ReplaceId));
					if(Pos!=cNoRef)
					{
						Neighbors.Delete(ReplaceId);
						Neighbors.Insert(Id);
						if(Pos<NbNearest)
							Cont=true;    // The nearest neighbors have changed
					}
				}

				// Next nearest neighbor
				Neighbor.Next();
				if(Neighbor.End())
				{
					// All the k first neighbors of the nearest neighbors were treated
					Neighbor.Start();
					CurRank++;
				}
			}
		}
	}

	cout<<"Copy Samples into Matrix"<<endl;

	// Copy the first NbNearest of Samples into Matrix
	// Update the neighbors ?
	Element=dynamic_cast<RMaxMatrix*>(Matrix)->GetLines();  // Work now on the real Matrix
	for(Element.Start(),Sample.Start();!Element.End();Element.Next(),Sample.Next())
	{
		Element()->Clear();

		// Valid element ?
		GObject* obj1=Session->GetObj(Lines,Element.GetPos()+1,false);
		if(!obj1)
			continue;

		RCursor<RMaxValue> Val(*Sample(),0,NbNearest-1);
		for(Val.Start();!Val.End();Val.Next())
			Element()->InsertPtrAt(new RMaxValue(*Val()),Val.GetPos());
	}

	// Deallocation
	delete[] Elements;

	cout<<"Finish"<<endl;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::UpdateMem(void)
{
	#ifdef DEBUG
		DebugFile<<"UpdateMem()"<<endl;
	#endif

	// Update the matrix:
	switch(Type)
	{
		case Full:
		{
			#ifdef DEBUG
				DebugFile<<"Full"<<endl;
			#endif
			// Parse the matrix and re-compute all the elements that are dirty
			RCursor<RVector> Cur(static_cast<RMatrix*>(Matrix)->GetLines());
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				GObject* obj1=Session->GetObj(Lines,Cur.GetPos()+1,true);
				if(!obj1)
					continue;
				size_t max;
				if(Symmetric)
					max=Cur.GetPos();
				else
					max=Matrix->GetNbCols()-1;

				RNumCursor<double> Cur2(Cur()->GetCols(0,max));
				for(Cur2.Start();!Cur2.End();Cur2.Next())
				{
					if(Cur2()==Cur2())
						continue;

					// Value must be recomputed
					GObject* obj2=Session->GetObj(Cols,Cur2.GetPos()+1,true);
					if(!obj2)
						continue;

					double res(Compute(obj1,obj2));
					if(fabs(res)<CutoffFrequency)
						res=0.0;
					Cur2()=res;
					AddValue(Cur2());
					#ifdef DEBUG
						DebugFile<<"("<<Cur.GetPos()<<","<<Cur2.GetPos()<<"="<<res<<endl;
					#endif
				}
			}
			break;
		}
		case Sparse:
			UpdateSparse();
			break;
		case NearestNeighbors:
			if(FastNN)
				UpdateNearestNeighborsFast();
			else
				UpdateNearestNeighborsRAM();
			break;
	}
	DirtyMem=false;
	if(InStorage)
		DirtyFile=true;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::UpdateStorage(void)
{
	// Update the file:
	switch(Type)
	{
		case Full:
		{
			// Parse the file and re-compute all the elements that are dirty
			for(size_t i=0;i<Storage.GetNbLines();i++)
			{
				GObject* obj1=Session->GetObj(Lines,i+1,true);
				if(!obj1)
					continue;
				size_t max;
				if(Symmetric)
					max=i+1;
				else
					max=Storage.GetNbCols();

				for(size_t j=0;j<max;j++)
				{
					double res(Storage.Read(i,j));

					if(res==res)
						continue;  // Normal value

					GObject* obj2=Session->GetObj(Cols,j+1,true);
					if(!obj2)
						continue;

					res=Compute(obj1,obj2);
					if(abs(res)<CutoffFrequency)
						res=0.0;

					AddValue(res);
					Storage.Write(i,j,res);
				}
			}
			break;
		}
		case Sparse:
			UpdateSparse();
			break;
		case NearestNeighbors:
			if(FastNN)
				UpdateNearestNeighborsFast();
			else
				UpdateNearestNeighborsRAM();
			break;
	}
	DirtyFile=false;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::AddValue(double val)
{
	if(val!=val)
		return;
	double oldmean(Mean);
	Mean=(static_cast<double>(NbValues)*Mean+val)/static_cast<double>(NbValues+1);
	Deviation=(((Deviation+(oldmean*oldmean))*static_cast<double>(NbValues))+(val*val))/static_cast<double>(NbValues+1)-(Mean*Mean);
	NbValues++;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::DeleteValue(double& val)
{
	if(val!=val)
		return;
	if(NbValues==1)
	{
		NbValues=0;
		Mean=Deviation=0.0;
	}
	else
	{
		double oldmean(Mean);
		Mean=(static_cast<double>(NbValues)*Mean-val)/static_cast<double>(NbValues-1);
		Deviation=(((Deviation+(oldmean*oldmean))*static_cast<double>(NbValues))-(val*val))/static_cast<double>(NbValues-1)-(Mean*Mean);
		NbValues--;
	}
	val=NAN;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::CreateConfig(void)
{
	InsertParam(new RParamValue("Cutoff Frequency",0.000001,"Minimal value for a non-null measure"));
	InsertParam(new RParamValue("MinMeasure",0.05,"Fixed minimal value of the measure"));
	InsertParam(new RParamValue("DeviationRate",1.5,"Deviation rate used to compute automatically the minimal value of the measure"));
	InsertParam(new RParamValue("AutomaticMinMeasure",true,"Should the minimum value of the measure be automatically computed?"));
	InsertParam(new RParamValue("Memory",true,"The measures should be stored in memory?"));
	InsertParam(new RParamValue("Storage",false,"The measures should be stored on disk?"));
	InsertParam(new RParamValue("Dir","/var/galilei","Root directory to store the measures (in practice they are stored in sub-directory named after the session"));
	InsertParam(new RParamValue("Type",static_cast<int>(Full),"Type of the measure"));
	InsertParam(new RParamValue("NbNearest",10,"Number of nearest neighbors to store"));
	InsertParam(new RParamValue("NbSamples",20,"Number of samples used to compute the nearest neighbors"));
	InsertParam(new RParamValue("FastNN",true,"Use a fast computation method for the nearest neighbors (needs more memory)?"));
	InsertParam(new RParamValue("ForceCompute",false,"Must the measure be systematically recomputed?"));
}


//------------------------------------------------------------------------------
GMatrixMeasure::~GMatrixMeasure(void)
{
	if(InStorage)
	{
		if(DirtyFile)
		{
			if(Matrix)
				Storage.Save(*Matrix);
			Storage.WriteInfo(0,(char*)&NbValues,sizeof(size_t));
			Storage.WriteInfo(sizeof(size_t),(char*)&Mean,sizeof(double));
			Storage.WriteInfo(sizeof(size_t)+sizeof(double),(char*)&Deviation,sizeof(double));
			Storage.WriteInfo(sizeof(size_t)+2*sizeof(double),(char*)&DirtyMem,sizeof(bool));
		}
		Storage.Close();
	}
	if(Matrix)
	{
		delete Matrix;
		Matrix=0;
	}
}
