/*

	GALILEI Research Project

	GMeasure2Elements.h

	Measures between two elements - Header.

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
#ifndef GMeasure2ElementsH
#define GMeasure2ElementsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <gsignalhandler.h>


//------------------------------------------------------------------------------
// forward declaration
namespace R
{
	template<class C,bool bOrder> class RRecFile;
}


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GMeasure2Elements class provides a representation for a measure between
* two elements. Each measure has different properties:
* - Symmetric (for example between two same type of elements) or not.
* - The values can be stored in memory. This can be memory consuming for huge
*    number of elements.
* - The values can be stored in files. In this case, for each measure 'mes', 4
*   files are created : mes.main (containing main information), mes.idx (each
*   record corresponds to one "line" element), mes.block (each record
*   corresponds to a block of values) and mes.val (each record corresponds to
*   a value). All files are created in a sub-directory called after the name of
*   the world.
* The class supposes that the identifiers of the elements are continuous and
* that the first identifier is one.
* @author Pascal Francq.
* @short Measures Between Two Elements.
*/
class GMeasure2Elements : public GMeasure, public GSignalHandler
{
	class Measures;    // A list of measure for a given element.
	class MeasureRec;  // Record storing a specific measure.
	class IdxRec;      // Record storing the first block of each element.
	class BlockRec;    // Record storing a reference to a block of values.

	/**
	 * Values.
	 */
	R::RContainer<Measures,true,false>* MemValues;

	/**
	 * Records of measures.
	 */
	R::RRecFile<MeasureRec,false>* RecValues;

	/**
	 * Records of index.
	 */
	R::RRecFile<IdxRec,false>* Idx;

	/**
	 * Record of blocks.
	 */
	R::RRecFile<BlockRec,false>* Blocks;

	/**
	 * Number of lines in memory.
	 */
	size_t MemNbLines;

	/**
	 * Number of columns in memory.
	 */
	size_t MemNbCols;

	/**
	 * Number of lines in files.
	 */
	size_t FileNbLines;

	/**
	 * Number of columns in files
	 */
	size_t FileNbCols;

	/**
	 * Maximal identifier of new lines to created.
	 */
	size_t MaxIdLine;

	/**
	 * Maximal identifier of new columns to created
	 */
	size_t MaxIdCol;

	/**
	 * Number of values computed.
	 */
	size_t NbValues;

	/**
	 * Structures in memory must be extended.
	 */
	bool MemMustExtend;

	/**
	 * Structures in files must be extended.
	 */
	bool FileMustExtend;

	/**
	 * Mean of the measures.
	 */
	double Mean;

	/**
	 * Deviation of the measures.
	 */
	double Deviation;

	/**
	 *  Is the measure symmetric, i.e. measure(i,j)=measure(j,i) ?
	 */
	bool Symmetric;

	/**
	* Level under which a measure is considered as null;
	*/
	double NullLevel;

	/**
	 * Value representing a dirty one. For example, for a measure in [-1,+1],
	 * it may be -2 (default).
	 */
	double DirtyValue;

	/**
	* Static minimum of measure.
	*/
	double MinMeasure;

	/**
	 * Compute automatically minimum of measure.
	 */
	bool AutomaticMinMeasure;

	/**
	 * Has a minimum for the measure a sense.
	 */
	bool MinMeasureSense;

	/**
	 * Measures in memory.
	 */
	bool InMemory;

	/**
	 * Measures in a file.
	 */
	bool InFile;

	/**
	 * Name of measures.
	 */
	R::RString Name;

	/**
	 * Directory containing the binary files.
	 */
	R::RString Dir;

	/**
	 * Type of the elements representing the lines.
	 */
	tObjType Lines;

	/**
	 * Type of the elements representing the columns.
	 */
	tObjType Cols;

public:

	/**
	* Constructor of the measures between two elements of the same type. The
	* measures may be symmetric or not.
	* @param fac             Factory of the plug-in.
	* @param sym             Symmetric measure?
	* @param type            Type of the elements in the lines.
	*/
	GMeasure2Elements(GFactoryMeasure* fac,bool sym,tObjType type);

	/**
	 * Constructor of the measures between two elements of different types.
	 * @param fac             Factory of the plug-in.
	 * @param lines           Type of the elements in the lines.
	 * @param cols            Type of the elements in the columns.
	 */
	GMeasure2Elements(GFactoryMeasure* fac,tObjType lines,tObjType cols);

	/**
	 * Set the type of the elements.
	 * @param sym             Symmetric measure?
	 * @param lines           Type of the elements in the lines.
	 * @param cols            Type of the elements in the columns.
	 */
	void SetElementsType(bool sym,tObjType lines,tObjType cols);

	/**
	 * Get the value corresponding to a value that must be considered as null.
	 */
	inline double GetNullValue(void) const {return(NullLevel);}

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Get a measure between two elements. There are three parameters.
	* @param measure         Type of the measure (0).
	* @param id1             Identifier of the first element.
	* @param id2             Identifier of the second element.
	*/
	virtual void Measure(size_t measure,...);

	/**
	* Access to the minmum of the measure of two similar elements. There are
	* maximum two parameters.
	* @param measure         Type of the measure (0).
	 */
	virtual void Info(size_t info,...);

	/**
	 * Compute the measure for two elements.

	 * @param obj1            Pointer to the first element.
	 * @param obj2            Pointer to the second element.
	 * @code
	 * double MyMeasure::Compute(void* obj1,void* obj2)
	 * {
	 *    return(static_cast<GProfile*>(obj1)->SimilarityIFF(*static_cast<GProfile*>(obj2),otSubProfile));
	 * }
	 * @endcode
	 */
	virtual double Compute(void* obj1,void* obj2)=0;

	/**
	 * Return the total number of different elements. If the measure is related
	 * to one type only of elements (for example profiles), the implementation
	 * of the method looks like:
	 * @code
	 * size_t MyMeasure::GetNbElements(void)
	 * {
	 *    return(Session->GetNbProfiles());
	 * }
	 *
	 * If the measure is related to two types of elements (for example profiles
	 * and documents), the implementation of the method looks like:
	 * @code
	 * size_t MyMeasure::GetNbElements(void)
	 * {
	 *    return(Session->GetNbDocs()+Session->GetNbProfiles());
	 * }
	 * @endcode
	 */
	size_t GetNbDiffElements(void);

private:

	/**
	 * Check if the identifiers must be changed.
	 * @param id1
	 * @param id2
	 */
	inline void Check(size_t& id1,size_t id2) const
	{
		if((Symmetric)&&(id1<id2))
		{
			size_t tmp(id1);
			id1=id2;
			id2=tmp;
		}
	}

	/**
	 * Read a value of a measure between two elements identified by the
	 * identifiers id1 and id2.
	 * @param id1            Identifier of the first element.
	 * @param id2            Identifier of the second element.
	 */
	inline double ReadValue(size_t id1,size_t id2);

	/**
	 * Write a given value in the file.
	 * @param id1            Identifier of the first element.
	 * @param id2            Identifier of the second element.
	 * @param val            Value to write.
	 */
	void WriteValue(size_t id1,size_t id2,double val);

	/**
	 * Extend the internal structure.
	 */
	void ExtendMem(void);

	/**
	 * Extend the files structure.
	 */
	void ExtendFile(void);

	/**
	 * An element was added and all the measure related to it must be created.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line?
	 */
	void AddIdentificator(size_t id,bool line);

	/**
	 * An element has changed and all the measure related to it are not updated
	 * anymore.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line?
	 */
	void DirtyIdentificator(size_t id,bool line);

	/**
	 * An element is deleted and all the measure related to it are modified.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line?
	 */
	void DeleteIdentificator(size_t id,bool line);

	/**
	 * This template method handles the modification of the status of a given
	 * element. This method is used by all the Event methods.
	 * @param C              Class of the element.
	 * @param element        Pointer to the element.
	 * @param event          Event (type of modification).
	 * @param line           Element is a line?
	 */
	template<class C> void UpdateElement(C* element,tEvent event,bool line);

	/**
	 * All the measures must be updated in memory.
	 */
	void UpdateMem(void);

	/**
	 * All the measures must be updated in files.
	 */
	void UpdateFile(void);

	/**
	 * Add a new value.
	 * @param val            Value to add.
	 */
	void AddValue(double val);

	/**
	 * Delete a new value.
	 * @param val            Value to delete.
	 */
	void DeleteValue(double& val);

public:

	/**
	* A specific document has changed.
	* @param doc             Document.
	* @param event           Event.
	*/
	virtual void Event(GDoc* doc, tEvent event);

	/**
	* A specific profile has changed.
	* @param prof            Profile.
	* @param event           Event.
	*/
	virtual void Event(GProfile* prof, tEvent event);

	/**
	* A specific group has changed.
	* @param community       Community.
	* @param event           Event.
	*/
	virtual void Event(GCommunity* community, tEvent event);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GMeasure2Elements(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
