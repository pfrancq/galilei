/*

	GALILEI Research Project

	GMatrixMeasure.cpp

	Measure represented by a matrix of values - Implementation.

	Copyright 2007-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warr
		if(Matrix)anty of
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
GMatrixMeasure::GMatrixMeasure(GPlugInFactory* fac,tObjType lines,tObjType cols,bool sym)
	: RObject(fac->GetName()), GMeasure(fac), Symmetric(sym), Matrix(0), Storage(),
	  MinMeasureSense(true), Lines(lines), Cols(cols)
{
	if(Symmetric&&(Lines!=Cols))
		throw GException("Symmetric measures are only allowed if the elements are of the same type");
	Name=fac->GetName();
	InsertObserver(HANDLER(GMatrixMeasure::Handle),"ObjectChanged");
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
	return(RString(Dir+RFile::GetDirSeparator()+Session->GetStorage()->GetWorld()+RFile::GetDirSeparator()+Cat));
}


//------------------------------------------------------------------------------
RString GMatrixMeasure::GetFilesName(void) const
{
	return(Name);
}


//-----------------------------------------------------------------------------
void GMatrixMeasure::ApplyConfig(void)
{
	// Parameters that can always be changed
	DeviationRate=Factory->GetDouble("DeviationRate");
	MinMeasure=Factory->GetDouble("MinMeasure");
	AutomaticMinMeasure=Factory->GetBool("AutomaticMinMeasure");
	CutoffFrequency=Factory->GetDouble("Cutoff Frequency");

	// If matrix managed nearest neighbors and their number have changed -> matrix is dirty
	double NewNbNearest(Factory->GetUInt("NbNearest"));
	bool NewNbSamples(Factory->GetUInt("NbSamples"));
	if(NewNbSamples<NewNbNearest)
		NewNbSamples=2*NewNbNearest;
	if((Session)&&(Type==NearestNeighbors)&&((NewNbNearest!=NbNearest)||(NewNbSamples!=NbSamples)))
			Dirty=true;
	NbNearest=NewNbNearest;
	NbSamples=NewNbSamples;

	if(Session)
		return;

	// Parameters needing the reloading of the session
	InMemory=Factory->GetBool("Memory");
	InStorage=Factory->GetBool("Storage");
	Dir=Factory->Get("Dir");
	Type=static_cast<tType>(Factory->GetInt("Type"));
}


//------------------------------------------------------------------------------
void GMatrixMeasure::Connect(GSession* session)
{
	GMeasure::Connect(session);
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
				matrixtype=RGenericMatrix::tSparse;
				break;
		}
		Storage.Open(place,matrixtype);

		// Look if the storage contains information
		if(Storage.HasInfo())
		{
			// Yes -> Matrix was computed previously
			MaxIdLine=Storage.GetNbLines()-1;
			MaxIdCol=Storage.GetNbCols()-1;
			Storage.ReadInfo(0,(char*)&NbValues,sizeof(size_t));
			Storage.ReadInfo(sizeof(size_t),(char*)&Mean,sizeof(double));
			Storage.ReadInfo(sizeof(size_t)+sizeof(double),(char*)&Deviation,sizeof(double));
			Storage.WriteInfo(sizeof(size_t)+2*sizeof(double),(char*)&Dirty,sizeof(bool));
		}
		else
		{
			// No -> New matrix
			MaxIdLine=MaxIdCol=NbValues=0;
			Mean=Deviation=0.0;
		}
	}
	else
	{
		// New matrix
		MaxIdLine=MaxIdCol=NbValues=0;
		Mean=Deviation=0.0;
	}
	MustExtend=false;   // Suppose matrix is correct
}


//------------------------------------------------------------------------------
void GMatrixMeasure::Disconnect(GSession* session)
{
	if(InStorage)
	{
		if(Matrix)
			Storage.Save(*Matrix);
		Storage.WriteInfo(0,(char*)&NbValues,sizeof(size_t));
		Storage.WriteInfo(sizeof(size_t),(char*)&Mean,sizeof(double));
		Storage.WriteInfo(sizeof(size_t)+sizeof(double),(char*)&Deviation,sizeof(double));
		Storage.WriteInfo(sizeof(size_t)+2*sizeof(double),(char*)&Dirty,sizeof(bool));
		Storage.Close();
	}
	if(Matrix)
	{
		delete Matrix;
		Matrix=0;
	}
	MaxIdLine=MaxIdCol=NbValues=0;
	Mean=Deviation=0.0;
	GMeasure::Disconnect(session);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::ReInit(void)
{
	if(InMemory)
	{
		if(Matrix)
			Matrix->Clear();
	}
	else if(InStorage)
		Storage.Clear();
	Dirty=true;
	Deviation=Mean=0.0;
	NbValues=0;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::Measure(size_t measure,...)
{
	va_list ap;
	va_start(ap,measure);
	size_t id1(va_arg(ap,size_t));  // Element i correspond to line i-1
	size_t id2(va_arg(ap,size_t));  // Element j correspond to column j-1
	double* res(va_arg(ap,double*));
	va_end(ap);

	if(InMemory)
	{
		// Search in memory

		// If necessary, the matrix is created or extended
		if(!Matrix)
			InitMatrix();
		else if(MustExtend)
			ExtendMem();

		// If the matrix is not full and is dirty -> It must be re-computed first
		if(Dirty&&Type!=Full)
			UpdateMem();

		// Take the element from the matrix (static_cast is to ensure the call of the const operator).
		(*res)=(*static_cast<const RGenericMatrix*>(Matrix))(id1-1,id2-1);

		// If NAN -> Recomputing is necessary (Verification in the case of the full matrix)
		if((*res)!=(*res))
		{
			void* obj1=Session->GetElement(Lines,id1);
			void* obj2=Session->GetElement(Cols,id2);
			(*res)=Compute(obj1,obj2);
			(*Matrix)(id1-1,id2-1)=(*res);
			if(fabs(*res)<CutoffFrequency)
				(*res)=0.0;     // High-pass filter
			AddValue(*res);
		}
	}
	else if(InStorage)
	{
		 // Search in the storage

		// If necessary, the storage is extended
		if(MustExtend)
			ExtendStorage();

		// If the matrix is not full and is dirty -> It must be re-computed first
		if(Dirty&&Type!=Full)
			UpdateStorage();

		// Read the value from the storage
		(*res)=Storage.Read(id1-1,id2-1);

		// If NAN -> recomputing is necessary (Verification in the case of the full matrix)
		if((*res)!=(*res))
		{
			void* obj1=Session->GetElement(Lines,id1);
			void* obj2=Session->GetElement(Cols,id2);
			(*res)=Compute(obj1,obj2);
			if(abs(*res)<CutoffFrequency)
				(*res)=0.0;     // High-pass filter
			AddValue(*res);
			Storage.Write(id1-1,id2-1,*res);
		}
	}
	else
	{
		// Recompute the element
		void* obj1=Session->GetElement(Lines,id1);
		void* obj2=Session->GetElement(Cols,id2);
		(*res)=Compute(obj1,obj2);
		if(abs(*res)<CutoffFrequency)
			(*res)=0.0;         // High-pass filter
	}
}


//------------------------------------------------------------------------------
void GMatrixMeasure::Info(size_t info,...)
{
	va_list ap;
	va_start(ap,info);
	double* res(va_arg(ap,double*));
	va_end(ap);

	if(info>2)
		throw GException("GMatrixMeasure: '"+RString::Number(info)+"' not a valid information");

	if(!MinMeasureSense)
	{
		(*res)=NAN;
		return;
	}

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
		if(MustExtend)
			ExtendMem();
		if(Dirty)
			UpdateMem();
	}
	else if(InStorage)
	{
		// Take the statistics from the storage and update it if necessary
		if(MustExtend)
			ExtendStorage();
		if(Dirty)
			UpdateStorage();
	}
	else
	{
		// Initialize
		NbValues=0;
		Mean=Deviation=0;
		size_t nblines=Session->GetMaxElementId(Lines);
		if(!nblines)
		{
			(*res)=0.0;
			Dirty=false;
			return;
		}

		// Go through all elements
		for(size_t i=0;i<nblines;i++)
		{
			void* obj1=Session->GetElement(Lines,i+1,true);
			if(!obj1)
				continue;

			// Compute number of columns for this line
			size_t nbcols;
			if(Symmetric)
				nbcols=i+1;
			else
				nbcols=Session->GetMaxElementId(Cols);

			for(size_t j=0;j<i;j++)
			{
				void* obj2=Session->GetElement(Cols,j+1,true);
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
		Dirty=false;
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
			Matrix=new RSparseMatrix(InitLine,InitCol,true,NbNearest);
			break;
	}

	// If valid storage -> Load the matrix and extend it eventually
	if(LoadStorage)
	{
		Storage.Load(*Matrix);
		if((InitLine!=MaxIdLine)||(InitCol!=MaxIdCol))
		{
			ExtendMem();
			Dirty=true;
		}
		else
			Dirty=false;
	}
	else
		Dirty=true;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::ExtendMem(void)
{
	// Verify if it must be extended
	Matrix->VerifySize(MaxIdLine,MaxIdCol,true);

	// Memory was extended (if needed) and is dirty
	MustExtend=false;
	Dirty=true;
}


//------------------------------------------------------------------------------
void GMatrixMeasure::ExtendStorage(void)
{
	// Verify if the matrix must be extended
	Storage.VerifySize(MaxIdLine,MaxIdCol,true);

	// File was extended (if needed) and is dirty
	MustExtend=false;
	Dirty=true;
}


//------------------------------------------------------------------------------
size_t GMatrixMeasure::GetNbDiffElements(void)
{
	if(Lines==Cols)
		return(Session->GetNbElements(Lines));
	return(Session->GetNbElements(Lines)+Session->GetNbElements(Cols));
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
					MustExtend=true;
			}
			else if(InStorage&&(Storage.GetNbLines()<=id))
				MustExtend=true;
		}
	}
	else
	{
		// Verify if a column must be added in memory or in files
		if(MaxIdCol<id)
		{
			MaxIdCol=id;
			if(InMemory)
			{
				if(Matrix&&(Matrix->GetNbCols()<=id))
					MustExtend=true;
			}
			else if(InStorage&&(Storage.GetNbCols()<=id))
				MustExtend=true;
		}
	}
}


//------------------------------------------------------------------------------
void GMatrixMeasure::DirtyIdentificator(size_t id,bool line,bool file)
{
	// If the matrix is not fully managed, it is just declare dirty.
	if(Type!=Full)
	{
		Dirty=true;
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
			RMatrixLine* Line((*static_cast<RMatrix*>(Matrix))[id-1]);
			RNumCursor<double> Cols(Line->GetCols());
			for(Cols.Start();!Cols.End();Cols.Next())
				DeleteValue(Cols());
			Dirty=true;
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
			Dirty=true;
		}
	}
	else
	{
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
			RCursor<RMatrixLine> Lines(static_cast<RMatrix*>(Matrix)->GetLines());
			for(Lines.GoTo(start);!Lines.End();Lines.Next())
				DeleteValue((*Lines())[id-1]);
			Dirty=true;
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
			Dirty=true;
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
void GMatrixMeasure::Handle(const RNotification& notification)
{
	GEvent& Event(GetData<GEvent&>(notification));
//    cout<<"From GMatrixMeasure ("<<Name<<"): "<<endl;
//    cout<<"    Event: "<<GetEvent(Event.Event)<<endl;
//    cout<<"    Object ("<<GetObjType(Event.Object->GetObjType())<<": "<<Event.Object->GetId()<<endl;

    bool DoLines(Lines==Event.Object->GetObjType());
    bool DoCols(Cols==Event.Object->GetObjType());
	switch(Event.Event)
	{
		case GEvent::eObjNew:
			if(DoLines)
				AddIdentificator(Event.Object->GetId(),true);
			if(DoCols)
				AddIdentificator(Event.Object->GetId(),false);
			break;
		case GEvent::eObjModified:
			if(DoLines)
				DirtyIdentificator(Event.Object->GetId(),true,true);
			if(DoCols)
				DirtyIdentificator(Event.Object->GetId(),false,true);
			break;
		case GEvent::eObjDelete:
			if(DoLines)
				DeleteIdentificator(Event.Object->GetId(),true);
			if(DoCols)
				DeleteIdentificator(Event.Object->GetId(),false);
			break;
		case GEvent::eObjDestroyed:
			if(DoLines)
				DestroyIdentificator(Event.Object->GetId(),true);
			if(DoCols)
				DestroyIdentificator(Event.Object->GetId(),false);
			break;
		default:
			break;
	}
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
		void* obj1=Session->GetElement(Lines,i+1,true);
		if(!obj1)
			continue;
		size_t max;
		if(Symmetric)
			max=i+1;
		else
			max=MaxIdCol;

		for(size_t j=0;j<max;j++)
		{
			void* obj2=Session->GetElement(Cols,j+1,true);
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
	double da=static_cast<const RValue*>(a)->Value;
	double db=static_cast<const RValue*>(b)->Value;
	if(da>db)
		return(-1.0);
	else if(da<db)
		return(1.0);
	else
		return(0);
}


//------------------------------------------------------------------------------
// Class representing the nearest neighbor of an element
class NN
{
public:
	RValue* Neighbors;

	NN(size_t nb) : Neighbors(new RValue[nb]) {}
	int Compare(const NN&) const {return(-1);}
	void Clear(void) {delete[] Neighbors;}
	~NN(void) {}
};


//------------------------------------------------------------------------------
bool IsIn(size_t id,RValue* tab,size_t nb)
{
	size_t i;
	RValue* ptr;
	for(i=nb+1,ptr=tab;--i;ptr++)
		if(ptr->Id==id)
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
void GMatrixMeasure::UpdateNearestNeighbors(void)
{
	size_t i,j,k,l;

	// If not in memory -> Not computed
	if(!InMemory)
		throw GException("GMatrixMeasure::UpdateNearestNeighbors(void) : Not implemented to work with storage only");

	// Clear the matrix and the file (if necessary) and re-initialize the statistics
	if(InMemory)
		Matrix->Clear();
	if(InStorage)
		Storage.Clear();
	Deviation=Mean=0.0;
	NbValues=0;

	// Compute the number of nearest neighbor to compute
	if(NbNearest>MaxIdCol)
		NbNearest=MaxIdCol;
	if(NbSamples<NbNearest)
		NbSamples=2*NbNearest;
	if(NbSamples>MaxIdCol-1)
		NbSamples=MaxIdCol-1;
	if(NbSamples<=NbNearest)
		throw GException("GMatrixMeasure::UpdateNearestNeighbors(void) : NbSamples<=NbNearest");

	// Structures
	RValue* Samples=new RValue[NbSamples];              // Samples
	RContainer<NN,true,false> Neighbors(MaxIdLine);     // Neighbors
	RValue* CurSample;
	RValue* CurNeighbor;
	size_t* Ids=new size_t[MaxIdLine];
	size_t* ptr;

	// Initialization
	for(i=0,ptr=Ids;i<MaxIdLine;i++)
	{
		void* obj1=Session->GetElement(Lines,i+1,true);
		if(!obj1)
			continue;

		Neighbors.InsertPtrAt(new NN(NbNearest),i,true);
		(*(ptr++))=i;
	}

	// Compute randomly the nearest neighbors of each element
	RCursor<NN> Elements(Neighbors);
	for(Elements.Start();!Elements.End();Elements.Next())
	{
		void* obj1=Session->GetElement(Lines,Elements.GetPos()+1,true);
		if(!obj1)
			cerr<<"GMatrixMeasure::UpdateNearestNeighbors(void) : Problem"<<endl;

		// Fill Samples with randomize measures.
		Session->GetRandom()->RandOrder(Ids,MaxIdLine);
		for(j=0,ptr=Ids,CurSample=Samples;j<NbSamples;j++,CurSample++,ptr++)
		{
			if((*ptr)==Elements.GetPos())
				ptr++;         // An element cannot be its own neighbor

			void* obj2=Session->GetElement(Cols,(*ptr)+1,true);
			if(!obj2)
				cerr<<"GMatrixMeasure::UpdateNearestNeighbors(void) : Problem"<<endl;

			// Compute the measure, apply the high-pass filter.
			CurSample->Id=(*ptr);
			CurSample->Value=Compute(obj1,obj2);
			if(abs(CurSample->Value)<CutoffFrequency)
				CurSample->Value=0.0;
			AddValue(CurSample->Value);
		}

		// Order Values (samples) by values. The first NbNearest are the neighbors.
		qsort(Samples,NbSamples,sizeof(RValue),CompareValues);
		for(j=0,CurSample=Samples,CurNeighbor=Elements()->Neighbors;j<NbNearest;j++,CurSample++,CurNeighbor++)
			(*CurNeighbor)=(*CurSample);
	}

	// While changes find new neighbors by comparing with those of already neighbors
//	size_t NbPass(1);
	for(bool Cont=true;Cont;)
	{
		// Suppose no changes
//		cout<<"Pass "<<NbPass++<<endl;
		Cont=false;

		// Goes to each element
		for(Elements.Start();!Elements.End();Elements.Next())
		{
			void* obj1=Session->GetElement(Lines,Elements.GetPos()+1,true);
			if(!obj1)
				cerr<<"GMatrixMeasure::UpdateNearestNeighbors(void) : Problem"<<endl;

			// Put in Samples the current neighbors
			for(j=0,CurSample=Samples,CurNeighbor=Elements()->Neighbors;j<NbNearest;j++,CurSample++,CurNeighbor++)
				(*CurSample)=(*CurNeighbor);

			// Add in Samples the neighbors of the current neighbors
			// 'k' contains the position of the current position treated in each neighbors
			// 'l' contains the current neighbor treated
			for(j=NbNearest,k=0,CurNeighbor=Elements()->Neighbors,l=0;j<NbSamples;)
			{
				// Current element treated is CurNeighbor[l][k]
				CurSample->Id=Neighbors[CurNeighbor->Id]->Neighbors[k].Id;

				// Look if Id is already a nearest neighbor
				if((CurSample->Id!=Elements.GetPos())&&(!IsIn(CurSample->Id,Samples,j)))
				{
					// No -> Add the value
					void* obj2=Session->GetElement(Cols,CurSample->Id+1,true);
					if(!obj2)
						cerr<<"GMatrixMeasure::UpdateNearestNeighbors(void) : Problem"<<endl;
					CurSample->Value=Compute(obj1,obj2);
					if(abs(CurSample->Value)<CutoffFrequency)
						CurSample->Value=0.0;
					AddValue(CurSample->Value);
					j++;
					CurSample++;
				}

				// Go to the next nearest neighbor
				l++;
				CurNeighbor++;
				if(l==NbNearest)
				{
					l=0;
					CurNeighbor=Elements()->Neighbors;
					k++;
					if(k==NbNearest)
						throw GException("GMatrixMeasure::UpdateNearestNeighbors(void) : Big problem");
				}
			}

			// Order Values (samples) by values. The first NbNearest are the neighbors.
			qsort(Samples,NbSamples,sizeof(RValue),CompareValues);
			for(j=0,CurSample=Samples,CurNeighbor=Elements()->Neighbors;j<NbNearest;j++,CurSample++,CurNeighbor++)
			{
				// If the nearest neighbors have no changed, the identifiers should be identical
				if(CurNeighbor->Id!=CurSample->Id)
				{
					(*CurNeighbor)=(*CurSample);
					Cont=true;
				}
			}
		}
	}

	// Copy all nearest neighbor in matrix.
	for(Elements.Start();!Elements.End();Elements.Next())
	{
		size_t i(Elements.GetPos());
		for(j=0,CurNeighbor=Elements()->Neighbors;j<NbNearest;j++,CurNeighbor++)
		{
			(*Matrix)(i,CurNeighbor->Id)=CurNeighbor->Value;
			#ifdef DEBUG
				DebugFile<<"("<<i<<","<<CurNeighbor->Id<<"="<<CurNeighbor->Value<<endl;
			#endif
		}
		Elements()->Clear();
	}

	// Deallocation
	delete[] Ids;
	delete[] Samples;
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
			RCursor<RMatrixLine> Cur(static_cast<RMatrix*>(Matrix)->GetLines());
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				void* obj1=Session->GetElement(Lines,Cur.GetPos()+1,true);
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
					void* obj2=Session->GetElement(Cols,Cur2.GetPos()+1,true);
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
			UpdateNearestNeighbors();
			break;
	}
	Dirty=false;
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
				void* obj1=Session->GetElement(Lines,i+1,true);
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

					void* obj2=Session->GetElement(Cols,j+1,true);
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
			UpdateNearestNeighbors();
			break;
	}
	Dirty=false;
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
void GMatrixMeasure::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("Cutoff Frequency",0.000001));
	params->InsertParam(new RParamValue("MinMeasure",0.05));
	params->InsertParam(new RParamValue("DeviationRate",1.5));
	params->InsertParam(new RParamValue("AutomaticMinMeasure",true));
	params->InsertParam(new RParamValue("Memory",true));
	params->InsertParam(new RParamValue("Storage",false));
	params->InsertParam(new RParamValue("Dir","/var/galilei"));
	params->InsertParam(new RParamValue("Type",static_cast<int>(Full)));
	params->InsertParam(new RParamValue("NbNearest",10));
	params->InsertParam(new RParamValue("NbSamples",20));
}


//------------------------------------------------------------------------------
GMatrixMeasure::~GMatrixMeasure(void)
{
}
