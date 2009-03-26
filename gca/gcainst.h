/*

	GALILEI Research Project

	GCAInst.h

	Instance - Header

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef GCAInstH
#define GCAInstH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstsc.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gca.h>


//-----------------------------------------------------------------------------
/**
* The GCAThreadData class provides a representation for "thread-dependent" data
* for the IR GA.
* @author Pascal Francq
* @short IR "thread-dependent" Data.
*/
class GCAThreadData : public R::RThreadDataSC<GCAInst,GCAChromo,GCAThreadData,GCAGroup,GCAObj>
{
public:
	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	GCAThreadData(GCAInst* owner);

	/**
	* Destruct the data.
	*/
	virtual ~GCAThreadData(void);
};


//-----------------------------------------------------------------------------
/**
* The instance of the IR problem.
* @author Pascal Francq
* @short IR Instance.
*/
class GCAInst : public R::RInstSC<GCAInst,GCAChromo,GCAThreadData,GCAGroup,GCAObj>
{
	/**
	* Session.
	*/
	GSession* Session;

	/*
	 * Similarity measure to used.
	 */
	GMeasure* Sims;

	/*
	 * Agreement measure to used.
	 */
	GMeasure* Agree;

	/*
	 * Disagreement measure to used.
	 */
	GMeasure* Disagree;

	/**
	 * Type of the elements to group.
	 */
	tObjType Type;

public:

	/**
	* Construct the instance.
	* @param ses            Session.
	* @param objs           The objects to group.
	* @param p              Parameters.
	* @param debug          Debugger.
	* @param type           Type of the element to group.
	* @param mes            Name of the measures to used (ex: "Documents",
	*                       "Profiles", etc.).
	* @param inc            Incremental mode (The GCA constructs the
	*                       chromosomes from existing solutions).
	*/
	GCAInst(GSession* ses,R::RCursor<GCAObj> objs,RParamsSC* p,R::RDebug* debug,tObjType type,const R::RString& mes,bool inc);

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetDisagreementRatio(size_t element1,size_t element2) const;

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetAgreementRatio(size_t element1,size_t element2) const;

	/**
	* Compute the similarity between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetSim(size_t element1,size_t element2) const;

	/**
	* Destruct the instance.
	*/
	virtual ~GCAInst(void);

	// friend classes
	friend class GCAChromo;
	friend class GCAGroup;
	friend class GCAThreadData;
	friend class GCAHeuristic;
};


//-----------------------------------------------------------------------------
#endif

