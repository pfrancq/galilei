/*

	GALILEI Research Project

	GDescriptionSet.cpp

	Description Set - Implementation.

	Copyright 2006-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdescriptionset.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GDescriptionSet
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDescriptionSet::GDescriptionSet(GSession* session)
	: Session(session), Set(200), IF(session->GetNbConcepts()+1),
	  Refs(session->GetNbConceptTypes()+1), Dirty(true),
	  tmpTypes(session->GetNbConceptTypes()+1), tmpConcepts(session->GetNbConcepts()+1)
{
}

//-----------------------------------------------------------------------------
void GDescriptionSet::Clear(void)
{
	Set.Clear();
	Dirty=true;
}


//-----------------------------------------------------------------------------
void GDescriptionSet::InsertDescription(const GDescription* desc)
{
	Set.InsertPtr(desc);
	Dirty=true;
}


//-----------------------------------------------------------------------------
double GDescriptionSet::GetIF(size_t conceptid)
{
	// Look if it must be recomputed
	if(Dirty)
	{
		// Reinitialize the vector
		IF.Init(Session->GetNbConcepts()+1,0.0);
		Refs.Init(Session->GetNbConceptTypes()+1,0.0);


		// Go through the descriptions
		RCursor<const GDescription> Desc(Set);
		for(Desc.Start();!Desc.End();Desc.Next())
		{
			RConstCursor<GVector> Vector(Desc()->GetVectors());
			if(!Vector.GetNb()) continue;

			// Init
			tmpConcepts.Init(Session->GetNbConcepts()+1,true);
			tmpTypes.Init(Session->GetNbConceptTypes()+1,true);

			// Parse the vectors
			for(Vector.Start();!Vector.End();Vector.Next())
			{
				RConstCursor<GConceptRef> ptr(Vector()->GetRefs());
				if(!ptr.GetNb())
					continue;

				GConceptType* type(Vector()->GetMetaConcept()->GetType());
				size_t TypeId(type->GetId());
				if(tmpTypes[TypeId])
				{
					// Yes -> A new object uses this concept type.
					Refs[TypeId]++;
					tmpTypes[TypeId]=false;
				}

				// IncRef for the concept
				size_t ConceptId(Vector()->GetMetaConcept()->GetId());
				if(tmpConcepts[ConceptId])
				{
					// Yes -> A new object uses this concept.
					IF[ConceptId]++;
					tmpConcepts[ConceptId]=false;
				}

				for(ptr.Start();!ptr.End();ptr.Next())
				{
					GConceptType* type(ptr()->GetConcept()->GetType());
					size_t TypeId(type->GetId());
					if(tmpTypes[TypeId])
					{
						// Yes -> A new object uses this concept type.
						Refs[TypeId]++;
						tmpTypes[TypeId]=false;
					}

					// IncRef for the concept
					size_t ConceptId(ptr()->GetConcept()->GetId());
					if(tmpConcepts[ConceptId])
					{
						// Yes -> A new object uses this concept.
						IF[ConceptId]++;
						tmpConcepts[ConceptId]=false;
					}
				}
			}
		}

		// Recompute the If
		RNumCursor<double> Cur(IF);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if(Cur()==0.0)
				Cur()=0;
			else
				Cur()=log10(Refs[Session->GetConcept(Cur.GetPos())->GetType()->GetId()]/Cur());
		}
	}
	if(conceptid>=IF.GetNb())
		throw std::range_error("GDescriptionSetGetIF(size_t) : idx "+RString::Number(conceptid)+" outside range [0,"+RString::Number(IF.GetNb()-1)+"]");
	return(IF[conceptid]);
}
