	/*

	GALILEI Research Project

	GMatrixMeasure.h

	Measure represented by a matrix of values - Header.

	Copyright 2007-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef GMatrixMeasureH
#define GMatrixMeasureH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <robject.h>


//------------------------------------------------------------------------------
// forward declaration
namespace R
{
	template<class C,bool bOrder> class RRecFile;
	class RBinaryFile;
}


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GMatrixMeasure class provides a representation for a measure represented
* by a matrix a value, such as between two elements.
*
* Each measure has different properties:
* - Symmetric (for example between two same type of elements) or not.
* - The values can be stored in memory. This can be memory consuming for huge
*   number of elements.
* - The values can be stored in files. In this case, for each measure 'mes', at
*   least two files are created : mes.main (containing main information) and
*   mes.val1 (each record corresponds to a value). If the measure is not
*   symmetric, a third file mes.val2 is created (with the other part of the
*   matrix). All files are created in a sub-directory called after the name of
*   the world.
* The class supposes that the identifiers of the elements are continuous and
* that the first identifier is one.
* @author Pascal Francq.
* @short Matrix Measure.
*/
class GMatrixMeasure : public R::RObject, public GMeasure
{
	class Measures;    // A list of measure for a given element.
	class MeasureRec;  // Record storing a specific measure.

	/**
	 * Values.
	 */
	R::RContainer<Measures,true,false>* MemValues;

	/**
	 * Main file containing information on the global values.
	 */
	R::RBinaryFile* Main;

	/**
	 * Records of measures (First part of the matrix). This file contains the
	 * values: (1,1), (2,1), (2,2), (3,1), (3,2), (3,3)...
	 */
	R::RRecFile<MeasureRec,false>* RecValues1;

	/**
	 * Records of measures (Second part of the matrix). This file contains the
	 * values: (1,2), (1,3), (2,3), (1,4), (2,4), (3,4)...
	 */
	R::RRecFile<MeasureRec,false>* RecValues2;

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
	 * The status of the memory is dirty.
	 */
	bool MemDirty;

	/**
	 * The status of the file is dirty.
	 */
	bool FileDirty;

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
	 * Used to compute the default minimum of the measure.
	 */
	double DeviationRate;

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

	/**
	 * First call.
	 */
	bool FirstCall;

public:

	/**
	 * Constructor of the measures between two elements of different types.
	 * @param fac             Factory of the plug-in.
	 * @param lines           Type of the elements in the lines.
	 * @param cols            Type of the elements in the columns.
	 * @param sym             Symmetric measure?
	 */
	GMatrixMeasure(GFactoryMeasure* fac,tObjType lines,tObjType cols,bool sym);

	/**
	 * Virtual method inherits from R::RObject and that must be re-implemented
	 * in all child classes.
	 * @return Name of the class.
	 */
	virtual R::RCString GetClassName(void) const {return("GMatrixMeasure");}

	/**
	 * Set the type of the elements.
	 * @param sym             Symmetric measure?
	 * @param lines           Type of the elements in the lines.
	 * @param cols            Type of the elements in the columns.
	 */
	void SetElementsType(bool sym,tObjType lines,tObjType cols);

	/**
	 * Get the type of the objects of the lines.
	 */
	inline tObjType GetLinesType(void) const {return(Lines);}

	/**
	 * Get the type of the objects of the columns.
	 */
	inline tObjType GetColsType(void) const {return(Cols);}

	/**
	 * Get the value corresponding to a value that must be considered as null.
	 */
	inline double GetNullValue(void) const {return(NullLevel);}

	/**
	 * Get the root directory where the measures are stored. By default, it
	 * correspond to "Dir/World/Cat" where :
	 * - Dir is the parameter specified by the user.
	 * - World is the name of the session.
	 * - Cat is the name of the corresponding plug-ins category (where all "/"
	 * characters in the names are replaced by "-").
	 */
	virtual R::RString GetRootDir(void) const;

	/**
	 * Get the name of the files that will be used. By default, it is the name
	 * of the plug-in.
	 */
	virtual R::RString GetFilesName(void) const;

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
	 * Initialize the measure (first call).
	 */
	void Init(void);

	/**
	* The measure must be re-initialized, i.e. all values must be considered
	* as dirty.
	*/
	virtual void ReInit(void);

	/**
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Get a measure between two elements. There are at least one parameters:
	* @param measure         Type of the measure (0).
	*
	* By default, this class supposes that three more parameters are passed:
	* - Identifier of the first element (size_t).
	* - Identifier of the second element (size_t).
	* - A pointer to a variable of type double
	*/
	virtual void Measure(size_t measure,...);

	/**
	* Access to several information related to the matrix of measure, such as
	* the minimum of the measure of two similar elements. There are
	* maximum two parameters.
	* @param info        Type of the information to take.
	*
	* By default, the only information is the minimum of values stored in the
	* matrix. One more parameter must be passed:
	* - A pointer to a variable of type double
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
	 * Open the files.
	 */
	void OpenFiles(void);

	/**
	 * Close the files.
	 */
	void CloseFiles(void);

	/**
	 * Check if the identifiers must be changed.
	 * @param id1
	 * @param id2
	 */
	inline void Check(size_t& id1,size_t& id2) const
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
	 * Dirty the current file position.
	 * @param file           File to dirty.
	 */
	void DirtyCurrentFilePos(R::RRecFile<MeasureRec,false>* file);

	/**
	 * An element has changed and all the measure related to it are not updated
	 * anymore.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line?
	 * @param file           File must be affected too?
	 */
	void DirtyIdentificator(size_t id,bool line,bool file);

	/**
	 * An element is deleted and all the measure related to it are modified.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line?
	 */
	void DeleteIdentificator(size_t id,bool line);

	/**
	 * An element is destroyed and all the measure related to it are modified.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line?
	 */
	void DestroyIdentificator(size_t id,bool line);

	/**
	 * This template method handles the modification of the status of a given
	 * object.
	 * @param notification   Notification received.
	 */
	void Handle(const R::RNotification& notification);

	/**
	 * All the measures must be updated in memory.
	 */
	void UpdateMem(void);

	/**
	 * All the measures must be updated in files.
	 */
	void UpdateFile(void);

	/**
	 * Load a file into memory.
	 */
	void LoadFile(void);

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
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GMatrixMeasure(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
