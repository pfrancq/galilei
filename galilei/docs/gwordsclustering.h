/*

	GALILEI Research Project

	gwordsclustering.h

  Create concepts - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Nicolas Kumps (nkumps@ulb.ac.be).

  Version $Revision$

  Last Modify: $Date$

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



//-----------------------------------------------------------------------------
#ifndef GWordsClusteringH
#define GWordsClusteringH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcursor.h>
#include <rstd/rrecfile.h>
#include <rstd/rcontainer.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <infos/giwordsweights.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* This class provides methods to cluster words.
* @author Nicolas Kumps
* @short Create concept.
*/
class GWordsClustering
{
	class GWordDocs : public GIWordsWeights
	{
			unsigned Id;
			double Idf;
		public :
			RContainer<GDoc,unsigned,false,true>* Docs;
			GWordDocs();
			GWordDocs(unsigned id);
			int Compare(const GWordDocs* n) const;
			int Compare(const GWordDocs& n) const;
			int Compare(const unsigned id) const;
			unsigned GetId() const;
			double GetIdf() const;
			void SetIdf(double idf) ;
			static int sortOrder(const void *a,const void *b);
			void AddDoc(GDoc* doc);
			virtual ~GWordDocs();
	};

//-----------------------------------------------------------------------------
/**
* The GWordWeightCursor class provides a way to go trough a vector of pairs
* (keyword/weight).
* @short Word Weight Cursor
*/
CLASSCURSOR(GWordDocsCursor,GWordDocs,unsigned int)


	/**
	* Container of all the words contain in the documents.
	*/
	RContainer<GWordDocs,unsigned,true,true>* Words;

	/**
	* Container of the documents to analyse.
	*/
	RContainer<GDoc,unsigned,false,false>* Docs;

	/**
	* Container of all the groups of words.
	*/
	RContainer<GIWordList,unsigned,true,true>* Groups;

	/**
	* Number of documents to compute.
	*/
	unsigned NbDocs;

	/**
	* Minimum number of documents where a word must appear
	* to be compute.
	*/
	unsigned NbMinDocs;

	/**
	* Maximum number of documents where a word can appear
	* to be compute.
	*/
	unsigned NbMaxDocs;

	/**
	* Minimum of occurrence of a word in the documents
	* to be compute.
	*/
	unsigned NbMinOcc;

	/**
	* Size of the window use to analyse the document structure.
	*/
	unsigned WindowSize;

	/**
	* Minimum of confidence.
	*/
	double MinConf;

	/**
	* Minimum of documents where the clusters must appear.
	*/
	unsigned NbDocsMin;

	/**
	* Dictionnary.
	*/
	GDict* Dic;

	/**
	* Name of the database.
	*/
	RString DbName;

	/**
	* Vector of words ordered by Idf factor.
	*/
	GWordDocs** Order;



public:

	/**
	* Constructor.
	* @param dic                    global dictionnary.
	* @param mindocs                minimum number of documents.
	* @param maxdocs                maximum number of documents.
	* @param minocc                 minimum occurence of the words to treat.
	* @param s                      size of the window.
	* @param c                      minimum confidence.
	* @param ndm                    minimum appears of a clusters.
	* @param name                   name of the database.
	*/
	GWordsClustering(GDict* dic,unsigned mindocs,unsigned maxdocs,unsigned minocc,unsigned s,double c,unsigned ndm,RString name);

	/**
	* Insert a document to the list.
	* @param doc                    document to add.
	*/
	void AddDoc(GDocVector* doc) ;

	/**
	* Return the name of file where is the document structure.
	*/
	RString GetFileName(GDoc *doc);

	/**
	* Create the association matrix.
	*/
	void InsertNeighbor();

	/**
	* Create the associations of words with the neighbor.
	*/
	void OrderByNeighbor();

	/**
	* Sort the dictionnary.
	*/
	void Sort();

	/*
	* Clean the words which don't satisfy the conditions.
	*/
	void CleanWords();

	/**
	* Create the conceptspace.
	*/
	void ConceptSpace();

	/**
	* Create the associations of words.
	* @param n                      number of the iteration.
	*/
	bool OrdreByDocs(unsigned n);

	/**
	* To View the concept.
	*/
	void View();

	/**
	* Update the dictionnary of the document.
	* @param doc                    document to update.
	*/
	void UpdateDoc(GDocVector* doc);

	/**
	* Update the dictionnary of the document with the second method.
	* @param doc                    document to update.
	*/
	void UpdateDocbis(GDocVector* doc);
	/**
	* Remove the concepts from the dictionnary of the document.
	* @param doc                    document to update.
	*/
	void ReverseUpdateDoc(GDocVector* doc);

	/**
	* Update the dictionnary of the document (delete the words of the concepts).
	* @param doc                    document to update.
	*/
	void UpdateDocter(GDocVector* doc);

	/**
	* Save all the association in the global dictionnary and in the documents of the list.
	*/
	void SaveAssociations(bool save);

	/**
	* Save the conceptspace.
	*/
	void SaveConceptSpace(bool save);

	/**
	* Clear the local dictionnary and the list of documents.
	*/
	void Clear();

	/**
	* Destructor.
	*/
	virtual ~GWordsClustering(void) ;

};


}  //-------- End of namespace GALILEI ----------------------------------------


//---------------------------------------------------------------------------
#endif
