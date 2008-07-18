/*

	GALILEI Research Project

	GMeasure2Elements.cpp

	Measures between two elements - Implementation.

	Copyright 2007-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include standard api files
#include <math.h>
#include <limits>


//------------------------------------------------------------------------------
// include files for R
#include <rvectorint.h>
#include <rrecfile.h>
#include <rdir.h>

//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure2elements.h>
#include <glang.h>
#include <gsession.h>
#include <gprofile.h>
#include <gdoc.h>
#include <ggroup.h>
#include <ggalileiapp.h>
#include <gcommunity.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GMeasure2Elements::MeasureRec
//
//------------------------------------------------------------------------------

class GMeasure2Elements::MeasureRec
{
public:
	double Value;

	MeasureRec(void) : Value(0.0) {}
	MeasureRec(double val) : Value(val) {}
	void Read(R::RRecFile<MeasureRec,false>& f)  {f>>Value;}
	void Write(R::RRecFile<MeasureRec,false>& f) {f<<Value;}
};



//------------------------------------------------------------------------------
//
// class GMeasure2Elements::IdxRec
//
//------------------------------------------------------------------------------

class GMeasure2Elements::IdxRec
{
public:
	off_t Pos;

	IdxRec(void) : Pos(RIOFile::MaxSize) {}
	void Read(R::RRecFile<IdxRec,false>& f)  {f>>Pos;}
	void Write(R::RRecFile<IdxRec,false>& f) {f<<Pos;}
};



//------------------------------------------------------------------------------
//
// class GMeasure2Elements::BlockRec
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GMeasure2Elements::BlockRec
{
public:
	off_t Pos;
	size_t FirstId;
	size_t LastId;
	off_t Next;

	BlockRec(void) : Pos(RIOFile::MaxSize), FirstId(0), LastId(0), Next(RIOFile::MaxSize) {}
	BlockRec(size_t f,size_t l) : Pos(RIOFile::MaxSize), FirstId(f), LastId(l), Next(RIOFile::MaxSize) {}
	void Read(R::RRecFile<BlockRec,false>& f)  {f>>Pos>>FirstId>>LastId>>Next;}
	void Write(R::RRecFile<BlockRec,false>& f) {f<<Pos<<FirstId<<LastId<<Next;}
};



//------------------------------------------------------------------------------
//
// class GMeasure2Elements::Measures
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GMeasure2Elements::Measures
{
public:
	double* Values;      // Values

	Measures(size_t max,double dirty);
	void Extend(size_t olds,size_t news,double dirty);
	int Compare(const Measures&) const {return(-1);}
	~Measures(void) {delete[] Values;}
};


//------------------------------------------------------------
GMeasure2Elements::Measures::Measures(size_t max,double dirty) : Values(0)
{
	Values=new double[max];
	// Dirty the new columns
	double* col;
	for(max++,col=Values;--max;col++)
		(*col)=dirty;
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Measures::Extend(size_t olds,size_t news,double dirty)
{
	double* tmp=new double[news];
	if(olds)
	{
		memcpy(tmp,Values,sizeof(double)*olds);
		delete[] Values;
	}
	Values=tmp;

	// Dirty the new columns
	double* col;
	size_t j;
	for(j=news-olds+1,col=&Values[olds];--j;col++)
		(*col)=dirty;
}



//------------------------------------------------------------------------------
//
//  GMeasure2Elements
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMeasure2Elements::GMeasure2Elements(GFactoryMeasure* fac,bool sym,tObjType type)
	: GMeasure(fac), GSignalHandler(), MemValues(0), RecValues(0), Idx(0), Blocks(0),
	  MemNbLines(0), MemNbCols(0), FileNbLines(0), FileNbCols(0), MaxIdLine(0), MaxIdCol(0), NbValues(0), Mean(0.0), Deviation(0.0),
	  Symmetric(sym),NullLevel(0.000001), DirtyValue(-2.0), MinMeasure(0.5), AutomaticMinMeasure(true),
	  MinMeasureSense(true), InMemory(true), InFile(false), Lines(type), Cols(type)
{
	GSession::AddHandler(this);
	Name=fac->GetName();
}


//------------------------------------------------------------------------------
GMeasure2Elements::GMeasure2Elements(GFactoryMeasure* fac,tObjType lines,tObjType cols)
	: GMeasure(fac), GSignalHandler(), MemValues(0), RecValues(0), Idx(0), Blocks(0),
	  MemNbLines(0), MemNbCols(0), FileNbLines(0), FileNbCols(0), MaxIdLine(0), MaxIdCol(0), NbValues(0), Mean(0.0), Deviation(0.0),
	  Symmetric(false),NullLevel(0.000001), DirtyValue(-2.0), MinMeasure(0.5), AutomaticMinMeasure(true),
	  MinMeasureSense(true), InMemory(true), InFile(false), Lines(lines), Cols(cols)
{
	GSession::AddHandler(this);
	Name=fac->GetName();
}


//-----------------------------------------------------------------------------
void GMeasure2Elements::SetElementsType(bool sym,tObjType lines,tObjType cols)
{
	if(sym&&(lines!=cols))
		throw GException("Symetric measures are only allowed if the elements are of the same type");
	Symmetric=sym;
	Lines=lines;
	Cols=cols;
}


//-----------------------------------------------------------------------------
void GMeasure2Elements::ApplyConfig(void)
{
	// Nothing can be changed when a session is running
	if(Session)
		return;
	NullLevel=Factory->GetDouble("NullLevel");
	MinMeasure=Factory->GetDouble("MinMeasure");
	AutomaticMinMeasure=Factory->GetBool("AutomaticMinMeasure");
	InMemory=Factory->GetBool("Memory");
	InFile=Factory->GetBool("File");
	Dir=Factory->Get("Dir");
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Connect(GSession* session)
{
	GMeasure::Connect(session);
	if(InFile)
	{
		// Create (if necessary) the directory that will contained the file
		RString place(Dir+RFile::GetDirSeparator()+Session->GetStorage()->GetWorld());
		RDir::CreateDirIfNecessary(place,true);
		place+=RFile::GetDirSeparator()+Name+".";

		// Read the main information
		try
		{
			RBinaryFile Main(place+"main");
			Main.Open(RIO::Read);
			Main>>FileNbLines>>FileNbCols>>NbValues>>Mean>>Deviation;
		}
		catch(...)
		{
			// Suppose the file does not exist
			FileNbLines=FileNbCols=NbValues=0;
			Mean=Deviation=0.0;
		}

		// Create the three files
		RecValues=new RRecFile<MeasureRec,false>(place+"val",sizeof(MeasureRec));
		RecValues->Open(RIO::ReadWrite);
		Idx=new RRecFile<IdxRec,false>(place+"idx",sizeof(IdxRec));
		Idx->Open(RIO::ReadWrite);
		if(!Idx->GetRecNb())
		{
			// Create a first rec corresponding to free blocks
			IdxRec I;
			Idx->WriteRec(I);
		}
		Blocks=new RRecFile<BlockRec,false>(place+"block",sizeof(BlockRec));
		Blocks->Open(RIO::ReadWrite);
	}
	FileMustExtend=MemMustExtend=false;   // Suppose nothing must be added
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Disconnect(GSession* session)
{
	if(InFile)
	{
		// Write the main information
		RBinaryFile Main(Dir+RFile::GetDirSeparator()+Session->GetStorage()->GetWorld()+RFile::GetDirSeparator()+Name+".main");
		Main.Open(RIO::Create);
		Main<<FileNbLines<<FileNbCols<<NbValues<<Mean<<Deviation;
	}

	if(MemValues)
	{
		delete MemValues;
		MemValues=0;
	}
	if(RecValues)
	{
		delete RecValues;
		RecValues=0;
	}
	if(Idx)
	{
		delete Idx;
		Idx=0;
	}
	if(Blocks)
	{
		delete Blocks;
		Blocks=0;
	}
	FileNbLines=FileNbCols=MemNbLines=MemNbCols=MaxIdLine=MaxIdCol=NbValues=0;
	Mean=Deviation=0.0;
	GMeasure::Disconnect(session);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Measure(size_t measure,...)
{
	va_list ap;
	va_start(ap,measure);
	size_t id1=va_arg(ap,size_t);
	size_t id2=va_arg(ap,size_t);
	size_t idx1,idx2;
	double* res=va_arg(ap,double*);
	va_end(ap);

	if(InFile&&FileMustExtend)
		ExtendFile();

	if(InMemory) // Search in memory
	{
		if(MemMustExtend)
			ExtendMem();

		Check(id1,id2);

		idx1=id1-1; // First line is empty (id1==1 -> Line=0)
		idx2=id2-1; // First column is id2=1 -> Col=0
		(*res)=(*MemValues)[idx1]->Values[idx2];

		// Recomputing it necessary
		if((*res)==DirtyValue)
		{
			void* obj1=Session->GetElement(Lines,id1);
			if(obj1)
			{
				void* obj2=Session->GetElement(Cols,id2);
				if(obj2)
					(*res)=Compute(obj1,obj2);
			}
			//cout<<"Compute Mem sim("<<id1<<","<<id2<<")="<<(*res)<<endl;
			(*MemValues)[idx1]->Values[idx2]=(*res);
			AddValue(*res);
			if(InFile)
				WriteValue(id1,id2,*res);
		}
		//cout<<"Mem: sim("<<id1<<","<<id2<<")="<<(*res)<<endl;
	}
	else if(InFile) // Search in binary file
	{
		(*res)=ReadValue(id1,id2);
		//cout<<"File: sim("<<id1<<","<<id2<<")="<<(*res)<<endl;
	}
	else  // Recompute it
	{
		void* obj1=Session->GetElement(Lines,id1);
		if(obj1)
		{
			void* obj2=Session->GetElement(Cols,id2);
			if(obj2)
				(*res)=Compute(obj1,obj2);
			else
				(*res)=0.0;
		}
		else
			(*res)=0.0;
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Info(size_t info,...)
{
	if(!MinMeasureSense)
		return;

	va_list ap;
	va_start(ap,info);
	double* res=va_arg(ap,double*);
	va_end(ap);
	double deviationrate=1.5;

	if(!AutomaticMinMeasure)
	{
		(*res)=MinMeasure;
		return;
	}

	if(InFile&&FileMustExtend)
		ExtendFile();
	if(InMemory)
	{
		if(MemMustExtend)
			ExtendMem();
		UpdateMem();
		(*res)=Mean+deviationrate*sqrt(Deviation);
	}
	else if(InFile)
	{
		if(BlankRecs)
			UpdateFile();
		(*res)=Mean+deviationrate*sqrt(Deviation);
	}
	else
	{
		size_t max=Session->GetMaxElementId(Lines);
		if(!max)
		{
			(*res)=0.0;
			return;
		}

		double NbComp=0.0;
		double sim;

		// Go through all elements
		for(size_t i=0;i<max-1;i++)
		{
			void* obj1=Session->GetElement(Lines,i+2);
			if(!obj1)
				continue;

			for(size_t j=0;j<i+1;j++)
			{
				void* obj2=Session->GetElement(Cols,j+1);
				if(!obj2)
					continue;
				NbComp+=1.0;
				sim=Compute(obj1,obj2);
				if(sim<NullLevel)
					sim=0.0;
				Mean+=sim;
				Deviation+=sim*sim;
			}
		}
		if(NbComp>0.0)
		{
			Mean/=NbComp;
			Deviation=(Deviation/NbComp)-(Mean*Mean);
		}
		(*res)=Mean+deviationrate*sqrt(Deviation);
	}
}


//------------------------------------------------------------------------------
inline double GMeasure2Elements::ReadValue(size_t id1,size_t id2)
{
	MeasureRec M;

	Check(id1,id2);

	// Go to the index file to find the first block
	IdxRec Element;
	if(Symmetric)
		Idx->GoToRec(id1-1);
	else
		Idx->GoToRec(id1);
	Idx->ReadRec(Element);

	// Go to the first block
	BlockRec Block;
	Blocks->GoToRec(Element.Pos);
	Blocks->ReadRec(Block);
	while((id2<Block.FirstId)||(id2>Block.LastId))
	{
		Blocks->GoToRec(Block.Next);
		Blocks->ReadRec(Block);
	}

	// Move to the correct value
	RecValues->GoToRec(Block.Pos+id2-Block.FirstId);

	// Read the record at the current position
	off_t Pos=RecValues->GetPos();
	MeasureRec Mes;
	RecValues->ReadRec(Mes);

	// Verify if there is something to update
	if(Mes.Value!=DirtyValue)
		return(Mes.Value);

	void* obj1=Session->GetElement(Lines,id1);
	if(obj1)
	{
		void* obj2=Session->GetElement(Cols,id2);
		if(obj2)
			Mes.Value=Compute(obj1,obj2);
	}
//	cout<<"Compute for file: Sim("<<id1<<","<<id2<<")="<<Mes.Value<<endl;
	AddValue(Mes.Value);

	// Store new record
	RecValues->Seek(Pos);      // Necessary because new data may be loaded in between
	RecValues->WriteRec(Mes);

	// Return
	return(Mes.Value);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::WriteValue(size_t id1,size_t id2,double val)
{
	Check(id1,id2);

	// Go to the index file to find the first block
	IdxRec Element;
	Idx->GoToRec(id1);
	Idx->ReadRec(Element);

	// Go to the first block
	BlockRec Block;
	Blocks->GoToRec(Element.Pos);
	Blocks->ReadRec(Block);
	while((id2<Block.FirstId)||(id2>Block.LastId))
	{
		Blocks->GoToRec(Block.Next);
		Blocks->ReadRec(Block);
	}

	// Move to the correct value
	RecValues->GoToRec(Block.Pos+id2-Block.FirstId);
	MeasureRec Mes(val);
	RecValues->WriteRec(Mes);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::ExtendMem(void)
{
	// Verify if the MemValues must be created
	if(!MemValues)
		MemValues=new RContainer<Measures,true,false>(MaxIdLine);

	// Verify first if the existing lines must be extended
	if((!Symmetric)&&(MaxIdCol>MemNbCols))
	{
		RCursor<Measures> Lines(*MemValues);
		for(Lines.Start();!Lines.End();Lines.Next())
			Lines()->Extend(MemNbCols,MaxIdCol,DirtyValue);
	}

	// Verify if new lines must be added
	if(MaxIdLine>MemNbLines)
	{
		for(size_t id=MemNbLines+1;id<=MaxIdLine;id++)
		{
			// Create the line
			size_t max;
			if(Symmetric)
				max=id;
			else
				max=MaxIdCol;
			MemValues->InsertPtrAt(new Measures(max,DirtyValue),id-1);
		}
	}
	MemNbLines=MaxIdLine;
	MemNbCols=MaxIdCol;
	MemMustExtend=false;
}


//------------------------------------------------------------------------------
void GMeasure2Elements::ExtendFile(void)
{
	MeasureRec NullMeasure(DirtyValue);
	IdxRec Index;

	// Verify first if the existing lines must be extended
	if((!Symmetric)&&(MaxIdCol>FileNbCols))
	{
		BlockRec NewBlock(FileNbCols+1,MaxIdCol);
		for(Idx->GoToRec(1);!Idx->End();)
		{
			// Read First block
			Idx->ReadRec(Index);

			// Create new dirty values
			NewBlock.Pos=RecValues->GetRecNb();
			RecValues->GoToRec(NewBlock.Pos);
			for(size_t i=MaxIdCol-FileNbCols+1;--i;)
				RecValues->WriteRec(NullMeasure);

			// Add a new block of values
			off_t Pos=Blocks->GetRecNb();
			Blocks->GoToRec(Pos);
			Blocks->WriteRec(NewBlock);

			// Find the last block to assign to another one
			BlockRec Find;
			Blocks->GoToRec(Index.Pos);
			Blocks->ReadRec(Find);
			for(;Find.Next!=RIOFile::MaxSize;)
			{
				Blocks->GoToRec(Find.Next);
				Blocks->ReadRec(Find);
			}
			Blocks->Prev();
			Find.Next=Pos;
			Blocks->WriteRec(Find);
		}
	}

	// Verify if new lines must be added
	if(MaxIdLine>FileNbLines)
	{
		for(size_t id=FileNbLines+1;id<=MaxIdLine;id++)
		{
			if(Symmetric&&id==1)
				continue;

			// Create the line
			size_t max;
			if(Symmetric)
				max=id-1;
			else
				max=MaxIdCol;
			BlockRec NewBlock(1,max);

			// Create new dirty values
			NewBlock.Pos=RecValues->GetRecNb();
			RecValues->GoToRec(NewBlock.Pos);
			for(size_t i=max+1;--i;)
				RecValues->WriteRec(NullMeasure);

			// Add a new block of values
			Index.Pos=Blocks->GetRecNb();
			Blocks->GoToRec(Index.Pos);
			Blocks->WriteRec(NewBlock);

			// Add the line in the index file
			if(Symmetric)
				Idx->GoToRec(id-1);
			else
				Idx->GoToRec(id);
			Idx->WriteRec(Index);
		}
	}
	FileNbLines=MaxIdLine;
	FileNbCols=MaxIdCol;
	FileMustExtend=false;
	BlankRecs=true;
}


//------------------------------------------------------------------------------
size_t GMeasure2Elements::GetNbDiffElements(void)
{
	if(Lines==Cols)
		return(Session->GetNbElements(Lines));
	return(Session->GetNbElements(Lines)+Session->GetNbElements(Cols));
}


//------------------------------------------------------------------------------
void GMeasure2Elements::AddIdentificator(size_t id,bool line)
{
	if(line)
	{
		// Verify if a line must be added in memory or in files
		if(MaxIdLine<id)
			MaxIdLine=id;
		if(Symmetric&&id==1)
			return;
		if(InMemory&&(MaxIdLine>MemNbLines))
			MemMustExtend=true;
		if(InFile&&(MaxIdLine>FileNbLines))
			FileMustExtend=true;
	}
	else
	{
		// Verify if a column must be added in memory or in files
		if(MaxIdCol<id)
			MaxIdCol=id;

		if(Symmetric)       // Nothing to do -> will be modified by a "new line".
			return;

		if(InMemory&&(MaxIdCol>MemNbCols))
			MemMustExtend=true;
		if(InFile&&(MaxIdCol>FileNbCols))
			FileMustExtend=true;
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::DirtyIdentificator(size_t id,bool line,bool file)
{
	if(line)
	{
		if(Symmetric&&id==1)
			return;

		if(file&&InFile&&(id<=MemNbLines))
		{
			// Dirty the measures
			Measures* ptr=(*MemValues)[id-1];
			if(!ptr)
			{
				char tmp[80];
				sprintf(tmp,"GMeasure2Elements::DirtyIdentificator : index %z doesn't exist",id);
				throw std::range_error(tmp);
			}

			// Dirty the line
			size_t max;
			if(Symmetric)
				max=id;
			else
				max=MemNbCols;
			double* col;
			size_t j;
			for(j=max+1,col=ptr->Values;--j;col++)
				DeleteValue(*col);
		}
		if(file&&InFile&&(id<=FileNbLines))
		{
			IdxRec Element;
			BlockRec Block;
			MeasureRec Value;

			// Read the index
			Idx->GoToRec(id);
			Idx->ReadRec(Element);

			Block.Next=Element.Pos;
			while(Block.Next!=RIOFile::MaxSize)
			{
				Blocks->GoToRec(Block.Next);
				Blocks->ReadRec(Block);

				// Read all values
				RecValues->GoToRec(Block.Pos);
				for(size_t j=Block.FirstId;j<=Block.LastId;j++)
				{
					RecValues->ReadRec(Value);

					// If value is already dirty -> Skip it
					if(Value.Value==DirtyValue)
						continue;

					if(!InFile)
						DeleteValue(Value.Value);  // Be sure it is not done twice
					else
						Value.Value=DirtyValue;
					RecValues->Prev();
					RecValues->WriteRec(Value);
				}
			}
		}
	}
	else
	{
		// Goes for the rest of lines
		if(InMemory&&(id<=MemNbCols))
		{
			size_t start;
			if(Symmetric)
				start=id;
			else
				start=0;
			RCursor<Measures> Lines(*MemValues);
			for(Lines.GoTo(start);!Lines.End();Lines.Next())
				DeleteValue(Lines()->Values[id-1]);
		}
		if(file&&InFile&&(id<FileNbCols))
		{
			IdxRec Element;
			BlockRec Block;
			MeasureRec Value;

			size_t start;
			if(Symmetric)
				start=id;
			else
				start=1;

			for(Idx->GoToRec(start);!Idx->End();)
			{
				Idx->ReadRec(Element);
				Blocks->GoToRec(Element.Pos);
				Blocks->ReadRec(Block);
				while((id<Block.FirstId)||(id>Block.LastId))
				{
					Blocks->GoToRec(Block.Next);
					Blocks->ReadRec(Block);
				}

				// If value is already dirty -> Skip it
				RecValues->GoToRec(Block.Pos+id-Block.FirstId);
				RecValues->ReadRec(Value);
				if(Value.Value==DirtyValue)
					continue;
				if(!InFile)
					DeleteValue(Value.Value);  // Be sure it is not done twice
				else
					Value.Value=DirtyValue;
				RecValues->Prev();
				RecValues->WriteRec(Value);
			}
		}
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::DeleteIdentificator(size_t id,bool line)
{
	// Remove all values
	DirtyIdentificator(id,line,false);
}


//------------------------------------------------------------------------------
void GMeasure2Elements::DestroyIdentificator(size_t,bool)
{
	if(InFile)
	{
		RToDo("Move blocks to used");
	}
}


//------------------------------------------------------------------------------
template<class C>
	void GMeasure2Elements::UpdateElement(C* element,tEvent event,bool line)
{
	switch(event)
	{
		case eObjNew:
			AddIdentificator(element->GetId(),line);
			break;
		case eObjModified:
			DirtyIdentificator(element->GetId(),line,true);
			break;
		case eObjDelete:
			DeleteIdentificator(element->GetId(),line);
			break;
		case eObjDestroyed:
			DestroyIdentificator(element->GetId(),line);
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::UpdateMem(void)
{
	// Parse all values and recompute each dirty values
	size_t max,i,j;
	double* vals;
	RCursor<Measures> Rows(*MemValues);
	for(Rows.Start(),i=1;!Rows.End();Rows.Next(),i++)
	{
		void* obj1=Session->GetElement(Lines,i);
		if(!obj1)
			continue;
		if(Symmetric)
			max=i;
		else
			max=MaxIdCol;
		for(j=1,vals=Rows()->Values;j<max;j++,vals++)
		{
			if((*vals)!=DirtyValue)
				continue;
			void* obj2=Session->GetElement(Cols,j);
			if(!obj2)
				continue;
			(*vals)=Compute(obj1,obj2);
			AddValue(*vals);
			if(InFile)
				WriteValue(i,j,*vals);
		}
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::UpdateFile(void)
{
	// Parse all values and recompute each dirty values
	size_t i;
	IdxRec Element;
	BlockRec Block;
	MeasureRec Value;

	if(Symmetric)
		i=2;
	else
		i=1;
	for(Idx->GoToRec(1);!Idx->End();i++)
	{
		// Get a pointer to the object 1 : If no pointer -> goes to next record
		void* obj1=Session->GetElement(Lines,i);
		if(!obj1)
		{
			Idx->Next();
			continue;
		}

		// Get the index
		Idx->ReadRec(Element);

		// Goes to the first block
		Block.Next=Element.Pos;
		while(Block.Next!=RIOFile::MaxSize)
		{
			Blocks->GoToRec(Block.Next);
			Blocks->ReadRec(Block);

			// Read all values
			RecValues->GoToRec(Block.Pos);
			for(size_t j=Block.FirstId;j<=Block.LastId;j++)
			{
				RecValues->ReadRec(Value);

				// If value OK -> Skip it
				if(Value.Value!=DirtyValue)
					continue;

				// Recompute it (if object exists)
				void* obj2=Session->GetElement(Cols,j);
				if(!obj2)
					continue;
				Value.Value=Compute(obj1,obj2);
				AddValue(Value.Value);
				RecValues->Prev();
				RecValues->WriteRec(Value);
			}
		}

	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::AddValue(double val)
{
	if(val==DirtyValue)
		return;
	double oldmean(Mean);
	Mean=(NbValues*Mean+val)/(NbValues+1);
	Deviation=(((Deviation+(oldmean*oldmean))*NbValues)+(val*val))/(NbValues+1)-(Mean*Mean);
	NbValues++;
}


//------------------------------------------------------------------------------
void GMeasure2Elements::DeleteValue(double& val)
{
	if(val==DirtyValue)
		return;
	if(NbValues==1)
	{
		NbValues=0;
		Mean=Deviation=0.0;
	}
	else
	{
		double oldmean(Mean);
		Mean=(NbValues*Mean-val)/(NbValues-1);
		Deviation=(((Deviation+(oldmean*oldmean))*NbValues)-(val*val))/(NbValues-1)-(Mean*Mean);
		NbValues--;
	}
	val=DirtyValue;
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GDoc* doc, tEvent event)
{
	if(InMemory||InFile)
	{
		if(Lines==otDoc)
			UpdateElement(doc,event,true);
		if(Cols==otDoc)
			UpdateElement(doc,event,false);
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GProfile* prof, tEvent event)
{
	if(InMemory||InFile)
	{
		if(Lines==otProfile)
			UpdateElement(prof,event,true);
		if(Cols==otProfile)
			UpdateElement(prof,event,false);
		//	cout<<"Profile "<<prof->GetId()<<" : "<<GetEvent(event)<<" ("<<GetPlugInName()<<")"<<endl;
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::Event(GCommunity* community, tEvent event)
{
	if(InMemory||InFile)
	{
		if(Lines==otCommunity)
			UpdateElement(community,event,true);
		if(Cols==otCommunity)
			UpdateElement(community,event,false);
	}
}


//------------------------------------------------------------------------------
void GMeasure2Elements::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NullLevel",0.00001));
	params->InsertParam(new RParamValue("MinMeasure",0.05));
	params->InsertParam(new RParamValue("AutomaticMinMeasure",true));
	params->InsertParam(new RParamValue("Memory",true));
	params->InsertParam(new RParamValue("File",false));
	params->InsertParam(new RParamValue("Dir","/var/galilei"));
}


//------------------------------------------------------------------------------
GMeasure2Elements::~GMeasure2Elements(void)
{
	GSession::DeleteHandler(this);
}
