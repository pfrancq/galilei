/*

  gwordsclustering.h

  Description - Header.

  (c) 2002 by N. Kumps (nkumps@ulb.ac.be).

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
#include <rio/rrecfile.h>
#include <rstd/rcontainer.h>

//---------------------------------------------------------------------------
// include files for GALILEI
#include <infos/giwordsweights.h>
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------
/**
* This class provides methods to cluster words.
*/
class GWordsClustering
{
	class GWordDocs
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

	/**
	* Container of all the words contain in the documents.
	*/
	RContainer<GWordDocs,unsigned,true,true>* Words;
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
	* Container of all the documents to compute.
	*/
	RStd::RContainer<GDoc,unsigned int,false,false>* Doc;

	/**
	* Dictionnary.
	*/
	GDict* Dic;

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
	*/
	GWordsClustering(GDict* dic,unsigned mindocs,unsigned maxdocs,unsigned minocc);

	/**
	* Insert a document to the list.
	* @param doc                    document to add.
	*/
	void AddDoc(GDocVector* doc) ;

	/**
	* Sort the dictionnary.
	*/
	void Sort();

	/*
	* Clean the words which don't satisfy the conditions.
	*/
	void CleanWords();

	/**
	* Create the associations of words.
	* @param n                      number of the iteration.
	*/
	bool OrdreByDocs(unsigned n);

	void View();

	/**
	* Update the dictionnary of the document.
	* @param doc                    document to update.
	*/
	void UpdateDoc(GDocVector* doc);

	/**
	* Save all the association in the global dictionnary and in the documents of the list.
	*/
	void SaveAssociations(unsigned n,bool save);

	/**
	* Clear the local dictionnary and the list of documents.
	*/
	void Clear();

	/**
	* Destructor.
	*/
	virtual ~GWordsClustering(void) ;

};

}
//---------------------------------------------------------------------------
#endif
