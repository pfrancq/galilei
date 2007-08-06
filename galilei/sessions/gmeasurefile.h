/*

	GALILEI Research Project

	GMeasureFile.h

	Binary files to store measures between objects - Header

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
#ifndef GMeasureFileH
#define GMeasureFileH



//-----------------------------------------------------------------------------
// include files for R library
#include <rrecfile.h>
#include <ruri.h>


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * This class provides a basic class for a measure that should be stored in a
 * binary to avoid recomputation.
 * @short Stored Measure
 */
class GStoredMeasure
{
public:
	/**
	 * Status of measures stored in a file.
	 */
	enum tStatus
	{
		NeverComputed        /** The measure was never computed.*/,
		ReComputed           /** The measure must be re-computed.*/,
		Computed             /** The measure was computed.*/,
		Nothing              /** No status.*/
	};

	/**
	 * Type of measures stored in a file.
	 */
	enum tType
	{
		OneElement            /** Measure related to only one element (ex: Ranking of a document). */,
		TwoElementsSymetric   /** Symetric measure between two elements (Sim(id1,id2)=Sim(id2,id1)). */,
		TwoElementsNoSymetric /** Non-symetric measure between two elements (Sim(id1,id2)!=Sim(id2,id1)). */
	};

protected:

	/**
	 * Status of the measure.
	 */
	char Status;

public:

	/**
	 * Constructor of a measure.
	 */
	GStoredMeasure(void) : Status(NeverComputed) {}

	/**
	 * Compare two measures.
	 * @param m              Measure.
	 */
	int Compare(const GStoredMeasure& m) const {return(this-(&m));}

	template<class C> friend  class GMeasureFile;
};


//------------------------------------------------------------------------------
/**
 * @param C                  Class representing the measure (must inherit from
 *                           GStoredMeasure).
 *
 * @short Binary file to store measure.
 */
template<class C>
	class GMeasureFile : public RRecFile<C,false>
{
protected:

	/**
	 * At least one record of the file must computed before it is read.
	 */
	bool Updated;

	/**
	 * Type fo the measure stored.
	 */
	GStoredMeasure::tType Type;

public:

	/**
	 * Construct a file that store measure.
	 * @param recsize        Size of a record.
	 */
	GMeasureFile(size_t recsize,GStoredMeasure::tType type);

	/**
	 * Open a specific file with a given uri.
	 * @param uri            URI of the file.
	 */
	void Open(const RURI& uri);

	/**
	 * Read a value of a measure between two elements identified by the
	 * identificators id1 and id2.
	 * @param id1            Identifier of the first element.
	 * @param id2            Identifier of the second element.
	 * @param val            Measure to read.
	 * @param move           Move the file to the right position (true) or read
	 *                       from the current position.
	 */
	void ReadValue(size_t id1,size_t id2,C& val,bool move);

	/**
	 * A given measure between two elements must be updated.
	 * @param id1            Identifier of the first element.
	 * @param id2            Identifier of the second element.
	 * @param val            Measure to update.
	 */
	virtual void MustUpdate(size_t id1,size_t id2,C& val)=0;

	/**
	 * Delete a given measure. This method is called for each measure that is
	 * modified when an element is deleted. It may be used to update statistics
	 * for example.
	 * @param val            Measure to deleted.
	 */
	virtual void Delete(C& val)=0;

	/**
	 * Add a new element to file. The file is enhanced if necessary.
	 * @param id             Identfier of the element.
	 */
	void AddIdentificator(size_t id);

	/**
	 * An element has changed and all the measure related to it are not updated
	 * anymore.
	 * @param id             Identfier of the element.
	 */
	void DirtyIdentificator(size_t id);

	/**
	 * An element is deleted and all the measure related to it are modified.
	 * @param id             Identfier of the element.
	 */
	void DeleteIdentificator(size_t id);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <gmeasurefile.hh>


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
