/*

	GALILEI Research Project

	GMeasureFile.hh

	Binary files to store measures between objects - Implementation

	Copyright 2006-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/


//------------------------------------------------------------------------------
//
// class GMeasureFile<C>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class C>
	GMeasureFile<C>::GMeasureFile(size_t recsize,GStoredMeasure::tType type)
		: RRecFile<C,false>(RString::Null,recsize), Type(type)
{

}


//------------------------------------------------------------------------------
template<class C>
	void GMeasureFile<C>::Open(const RURI& uri)
{
	RRecFile<C,false>::SetURI(uri);
	RRecFile<C,false>::Open(RIO::ReadWrite);
}


//------------------------------------------------------------------------------
template<class C>
	void GMeasureFile<C>::ReadValue(size_t id1,size_t id2,C& val,bool move)
{
	if(move)
	{
		// Compute the position of the file and verify that new "lines" must be added
		size_t recnb=(((id1-1)*(id1-2))/2)+(id2-1);
		if(recnb>RRecFile<C,false>::GetRecNb())
			AddIdentificator(id1);
		RRecFile<C,false>::GoToRec(recnb);
	}

	// Read the record at the current position
	// Read the value : If dirty, re-compute it and save
	long long pos=RRecFile<C,false>::GetPos();
	ReadRec(val);

	// Verify if there is something to update
	if((val.Status!=GStoredMeasure::NeverComputed)&&(val.Status!=GStoredMeasure::ReComputed))
		return;

	MustUpdate(id1,id2,val);

	// Store new record
	val.Status=GStoredMeasure::Computed;
	RRecFile<C,false>::Seek(pos);      // Necessary because new data may be loaded in between
	WriteRec(val);
}


//------------------------------------------------------------------------------
template<class C>
	void GMeasureFile<C>::AddIdentificator(size_t id)
{
	C Null;

	// Look if this object must be added
	size_t actualsize=RRecFile<C,false>::GetRecNb();
	size_t newsize=(id*(id-1))/2;
	if(actualsize>newsize)
		return;

	// Go to end of the file, while the size is not good -> fill it.
	RRecFile<C,false>::GoToRec(actualsize);
	actualsize=newsize-actualsize+1; // Put in actualsize the number of records to add (+1)
	while(--actualsize)
		WriteRec(Null);

	Updated=false;
}


//------------------------------------------------------------------------------
template<class C>
	void GMeasureFile<C>::DirtyIdentificator(size_t id)
{
	C Update;
	size_t nb;

	// Find the right line and dirty all the line.
	if(id>1)
	{
		size_t rec=((id-1)*(id-2))/2;
		RRecFile<C,false>::GoToRec(rec);
		nb=id;
		while(--nb)
		{
			ReadRec(Update);
			if(Update.Status==GStoredMeasure::Computed)
			{
				Update.Status=GStoredMeasure::ReComputed;
				RRecFile<C,false>::Prev();
				WriteRec(Update);
			}
		}
	}

	// For the next lines -> dirty only the column
	nb=id+1;
	size_t nbrecs=RRecFile<C,false>::GetRecNb();
	size_t recnb=(((nb-1)*(nb-2))/2)+(id-1);
	while(recnb<nbrecs)
	{
		RRecFile<C,false>::GoToRec(recnb);
		ReadRec(Update);
		if(Update.Status==GStoredMeasure::Computed)
		{
			Update.Status=GStoredMeasure::ReComputed;
			RRecFile<C,false>::Prev();
			WriteRec(Update);
		}
		nb++;
		recnb=(((nb-1)*(nb-2))/2)+(id-1);
	}

	Updated=false;
}


//------------------------------------------------------------------------------
template<class C>
	void GMeasureFile<C>::DeleteIdentificator(size_t id)
{
	C Update;
	size_t nb;

	// Find the right line and dirty all the line
	if(id>1)
	{
		size_t rec=((id-1)*(id-2))/2;
		RRecFile<C,false>::GoToRec(rec);
		nb=id;
		while(--nb)
		{
			ReadRec(Update);
			if(Update.Status==GStoredMeasure::Computed)
			{
				Update.Status=GStoredMeasure::Nothing;
				Delete(Update);
				RRecFile<C,false>::Prev();
				WriteRec(Update);
			}
		}
	}

	// For the next lines -> dirty only the column
	nb=id+1;
	size_t nbrecs=RRecFile<C,false>::GetRecNb();
	size_t recnb=(((nb-1)*(nb-2))/2)+(id-1);
	while(recnb<nbrecs)
	{
		RRecFile<C,false>::GoToRec(recnb);
		ReadRec(Update);
		if(Update.Status==GStoredMeasure::Computed)
		{
			Update.Status=GStoredMeasure::Nothing;
			Delete(Update);
			RRecFile<C,false>::Prev();
			WriteRec(Update);
		}
		nb++;
		recnb=(((nb-1)*(nb-2))/2)+(id-1);
	}
}
