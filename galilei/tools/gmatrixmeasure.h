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
// include files for R Project
#include <rgenericmatrix.h>
#include <rmatrixstorage.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmeasure.h>
#include <robject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GMatrixMeasure class provides a representation for a measure, \f$ [M]=m_{i,j} \f$,
* represented by a matrix of values, such as the similarity between two
* elements. A cutoff frequency can be specified.
*
* The class maintains the mean and the deviation of the values computed. A
* minimum value is computed using:
*
* \f$ \min([M])=\overline{[M]}+DeviationRate \cdot \sigma_{[M]} \f$
*
* The measure may be symmetric: \f$ m_{i,j}=m_{j,i} \; \forall i,j \f$
*
* The user has three choices to manage the matrix :
* -# The full matrix is maintained. It is the fastest access but it uses also
*    the most amount of memory. This is particularly useful if most values are
*    not null.
* -# A sparse matrix is maintained. It uses less memory that the full matrix,
*    but is also slower. This is particularly useful if most values are
*    null. If most values are not null, this is never a good choice.
* -# A list of nearest neighbors is maintained for each element (line). The
*    class tries to compute, for each value \f$ i \f$, a given number of
*    minimal values \f$ m_{i,\bullet} \f$. This type needs to have the matrix
*    stored in memory or in a file.
*
* The user may also specified if the matrix is maintained in memory, in a file
* or both. In this latest mode, the matrix is loaded the first time and saved
* at the end. The matrix is created in memory at the first call to it.
*
* If the full matrix is managed, changes (such as a modification of an element)
* imply only the re-computation of the corresponding line and column. In the
* other cases, the whole matrix is re-computed (also if only one element
* changed).
*
* @remarks The class supposes that the identifiers of the elements are continuous and
* that the first identifier is one.
* @author Pascal Francq.
* @short Matrix Measure.
*/
class GMatrixMeasure : public R::RObject, public GMeasure
{
public:

	/**
	 * Type of the matrix managing the measure.
	 */
	enum tType
	{
		Full,              /** Full matrix.*/
		Sparse,            /** Sparse matrix.*/
		NearestNeighbors  /** Nearest neighbors matrix.*/
	};

private:

	/**
	 * Type of the measure.
	 */
	tType Type;

	/**
	 *  Is the measure symmetric, i.e. measure(i,j)=measure(j,i) ?
	 */
	bool Symmetric;

	/**
	 * Matrix in memory (if needed).
	 */
	R::RGenericMatrix* Matrix;

	/**
	 * Storage of the matrix (if needed).
	 */
	R::RMatrixStorage Storage;

	/**
	 * Maximal identifier of new lines to created.
	 */
	size_t MaxIdLine;

	/**
	 * Maximal identifier of new columns to created
	 */
	size_t MaxIdCol;

	/**
	 * Matrix must be extended (in memory or in file).
	 */
	bool MustExtend;

	/**
	 * Matrix is dirty (in memory or file).
	 */
	bool Dirty;

	/**
	 * Mean of the measures.
	 */
	double Mean;

	/**
	 * Number of values computed.
	 */
	size_t NbValues;

	/**
	* Cutoff frequency;
	*/
	double CutoffFrequency;

	/**
	 * Deviation of the measures.
	 */
	double Deviation;

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
	 * Measures in a storage.
	 */
	bool InStorage;

	/**
	 * Name of measures.
	 */
	R::RString Name;

	/**
	 * Directory containing the binary files.
	 */
	R::RString Dir;

	/**
	 * Number of nearest neighbors.
	 */
	size_t NbNearest;

	/**
	 * Number of samples used to computed the neighbors.
	 */
	size_t NbSamples;

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
	inline double GetCutoffFrequency(void) const {return(CutoffFrequency);}

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
	*
	* The storage of the matrix is opened (if necessary).
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect to the session.
	*
	* If the matrix was initialized and a storage exists, the matrix is saved.
	* Then, the matrix is deallocated (if necessary) and the file is closed (if
	* necessary). The statistical elements are eventually saved with the matrix.
	* @param session         Pointer to the session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* The measure must be re-initialized, i.e. all values must be considered
	* as dirty. If the matrix is allocated, it is cleared. If the measures are
	* only managed through the storage, the storage is also cleared.
	*/
	virtual void ReInit(void);

	/**
	* Get a measure between two elements. If the values are in memory, the
	* method verifies if the matrix must be allocated or modified in memory.
	* If the values are only in files, the storage is modified if necessary.
	*
	* If the matrix manages the nearest neighbors, if the second element is not
	* a nearest neighbor of the first one, the measure is null. If the matrix
	* is neither stored in memory nor in a file, the result is always the
	* measure between the two elements.
	* @param measure         Type of the measure (not used since only one value
	*                        is stored for each element).
	* The class supposes that three more parameters are passed:
	* - Identifier of the first element (size_t).
	* - Identifier of the second element (size_t).
	* - A pointer to a variable of type double that will contain the result.
	*/
	virtual void Measure(size_t measure,...);

	/**
	* Access to several information related to the matrix of measure. The
	* matrix is eventually updated (in memory or in file) if necessary.
	*
	* If the matrix is not stored in memory or in file, the statistics are
	* computed on the whole matrix. If the matrix manages the nearest
	* neighbors, the statistics are computed on the pair tested to detect the
	* nearest neighbors.
	* @param info        Type of the information to take. Three values are
	*                    accepted:
	*                    - case '0': The minimum value.
	*                    - case '1': The mean value.
	*                    - case '2': The deviation of the values.
	*
	* The class supposes that one more parameter is passed:
	* - A pointer to a variable of type double that will contain the result.
	*/
	virtual void Info(size_t info,...);

	/**
	 * Compute the measure for two elements. This method must be overloaded.

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
	 * Creates the matrix in memory. If necessary, the matrix is loaded from
	 * the storage
	 */
	void InitMatrix(void);

	/**
	 * Extend the internal structure.
	 */
	void ExtendMem(void);

	/**
	 * Extend the storage.
	 */
	void ExtendStorage(void);

	/**
	 * An element is added. Look and remember if the memory or the storage must
	 * be extended. If the matrix is managed in memory, the storage is not
	 * modified until the de-connection from the session.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line or a column?
	 */
	void AddIdentificator(size_t id,bool line);

	/**
	 * An element is changed. If the matrix is fully managed, the values
	 * corresponding to the line and column are setting to NAN and the matrix
	 * is declared dirty. In the other cases, the matrix is just declare dirty.
	 *
	 * If the matrix is managed in memory and in files, the storage is not
	 * modified until the de-connection from the session.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line?
	 * @param file           File must be affected too?
	 */
	void DirtyIdentificator(size_t id,bool line,bool file);

	/**
	 * An element is deleted and all the measure related to it are modified.
	 * In practice, it calls DirtyIdentificator.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line?
	 */
	void DeleteIdentificator(size_t id,bool line);

	/**
	 * An element is destroyed and all the measure related to it are modified.
	 * In practice, it calls DirtyIdentificator.
	 * @param id             Identifier of the element.
	 * @param line           Element is a line?
	 */
	void DestroyIdentificator(size_t id,bool line);

	/**
	 * This method handles the modification of the status of a given
	 * object.
	 * @param notification   Notification received.
	 */
	void Handle(const R::RNotification& notification);

	/**
	 * Update the sparse matrix. All the measures are re-computed and stored
	 * either in memory or in files (depending the options).
	 */
	void UpdateSparse(void);

	/**
	 * Update the nearest neighbors. All the measures are re-computed and
	 * stored either in memory or in files (depending the options).
	 */
	void UpdateNearestNeighbors(void);

	/**
	 * All the measures must be updated in memory. If the matrix is fully
	 * managed, only the changed values are re-computed. Else, everything is
	 * re-computed.
	 */
	void UpdateMem(void);

	/**
	 * All the measures must be updated in the storage. If the matrix is fully
	 * managed, only the changed values are re-computed. Else, everything is
	 * re-computed.
	 */
	void UpdateStorage(void);

	/**
	 * A value is added to the statistics.
	 * @param val            Value to add.
	 */
	void AddValue(double val);

	/**
	 * A value must be removed from the statistics. The value is set
	 * to NAN.
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
