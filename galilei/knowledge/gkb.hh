/*

	GALILEI Research Project

	GKB.hh

	Generic GALILEI Session - inline implementations.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by Julien Lamoral.
	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
//
// class GKB
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
inline size_t GKB::GetNbObjs(const GConceptType*) const
{
	return(ConceptTypes.GetNb());
}


//------------------------------------------------------------------------------
inline size_t GKB::GetMaxObjId(const GConceptType*) const
{
	if(!ConceptTypes.GetNb())
		return(0);
	return(ConceptTypesByIds[ConceptTypesByIds.GetMaxPos()]->Id);
}


//------------------------------------------------------------------------------
inline R::RCursor<GConceptType> GKB::GetObjs(const GConceptType*) const
{
	return(R::RCursor<GConceptType>(ConceptTypes));
}


//------------------------------------------------------------------------------
inline size_t GKB::GetNbObjs(const GConcept*) const
{
	return(ConceptsByIds.GetNb());
}


//------------------------------------------------------------------------------
inline size_t GKB::GetMaxObjId(const GConcept*) const
{
	if(!ConceptsByIds.GetNb())
		return(0);
	return(ConceptsByIds[ConceptsByIds.GetMaxPos()]->Id);
}


//------------------------------------------------------------------------------
inline R::RCursor<GConcept> GKB::GetObjs(const GConcept*) const
{
	return(R::RCursor<GConcept>(ConceptsByIds));
}
