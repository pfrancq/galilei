/*

	GALILEI Research Project

	GStatsSims.h

	Groups Evaluation - Header.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GStatsSimsH
#define GStatsSimsH


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <ruri.h>
#include <galilei.h>
#include <gstatscalc.h>
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
class GStatsSims : public GStatsCalc
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
	* Should the results of the computation be stored in a file.
	*/
	bool SaveResults;

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
	*/
	GMeasure* Measure;

	/**
	 * File containing the results.
	 */
	RTextFile* ResultsFile;

public:

	/**
	* Constructor.
	* @param fac             Factory.
	*/
	GStatsSims(GFactoryStatsCalc* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	 * Export the documents similarities in a file.
	 */
	void DoExportDocsSims(void);

	/**
	 * Export the documents inclusions in a file.
	 */
	void DoExportDocsIncs(void);

	/**
	* Compute the statistics.
	* @param res             XML tag that will be hold the results.
	*/
	virtual void Compute(R::RXMLStruct* xml,R::RXMLTag& res);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

public:

	/**
	* Destruct.
	*/
	virtual ~GStatsSims(void);
};


//-----------------------------------------------------------------------------
#endif
