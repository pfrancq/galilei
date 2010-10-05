/*

	GALILEI Research Project

	GStatsSims.h

	Similarity Statistics - Header.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
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
#ifndef GStatsSimsH
#define GStatsSimsH


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <ruri.h>
#include <rworksheet.h>
#include <galilei.h>
#include <gtool.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
/**
* The GStatsSims class provides a method to compute different measures on the
* groups.
* @author Pascal Francq
* @short Groups Evaluation.
*/
class GStatsSims : public GTool
{
	/**
	* Statistics on Documents.
	*/
	bool Docs;

	/**
	* Statistics between profiles and documents.
	*/
	bool ProfDoc;

	/**
	* Statistics between groups and documents.
	*/
	bool GroupDoc;

	/**
	* Statistics on profiles.
	*/
	bool Profiles;

	/**
	* Statistics on the number of documents assessed by more than one profile.
	*/
	bool SameDocProf;

	/**
	* Statistics between groups and profiles.
	*/
	bool GroupProf;

	/**
	* Name of the file.
	*/
	RURI Results;

	/**
	 * Export the similarities between documents?
	 */
	bool ExportDocsSims;

	/**
	 * File to store the similarities between documents.
	 */
	RURI DocsSims;

	/**
	 * Export documents inclusions ?
	 */
	bool ExportDocsIncs;

	/**
	 * File to store the documents inclusions.
	 */
	RURI DocsIncs;

	/**
	 * Current measure used.
	 */
	GMeasure* Measure;

	/**
	 * File containing the results.
	 */
	RTextFile File;

	/**
	 * Type of the similarity measure ('Nearest Neighbors' or 'Complete').
	 */
	RString MeasureType;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GStatsSims(GSession* session,GPlugInFactory* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	 * Export the documents similarities in a file.
	 */
	void DoExportDocsSims(void);

	/**
	 * Export the documents inclusions in a file.
	 */
	void DoExportDocsIncs(void);

	/**
	 * Add columns for a given statistics to compute.
	 * @param stats          Statistics.
	 * @param idx            Index of the first column.
	 * @param name           Name of the columns.
	 */
	void AddColumns(RWorksheet& stats,size_t& idx,const RString& name);

	/**
	* Compute the statistics.
	*/
	virtual void Run(GSlot* slot);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	 * @return the type of the measure.
	 */
	RString GetMeasureType(void) const {return(MeasureType);}
};


//-----------------------------------------------------------------------------
#endif
