/*

	GALILEI Research Project

	GDocOptions.h

	Options for the document Analysis - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GDocOptionsH
#define GDocOptionsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GDocOptions provides a representation for the options concerning the
* documents analysis.
* @author Pascal Francq
* @short Document Options.
*/
class GDocOptions
{
public:

	/**
	* Define if the language are static.
	*/
	bool StaticLang;

	/**
	* Minimum percentage of words of a document which must be in the stop list
	* of a language to make the language the current one.
	*/
	double MinStopWords;

	/**
	* Minimum number of characters to have a valid word.
	*/
	unsigned int MinWordSize;

	/**
	* Minimum number of characters to have a valid stem.
	*/
	unsigned int MinStemSize;

	/**
	* Minimum number of occurences needed to insert a valid word in the list of
	* information for a document.
	*/
	unsigned int MinOccur;

	/**
	* Minimum number of documents where a valid word must appear to be compute
	* in the clustering algorithm.
	*/
	unsigned int MinDocs;

	/**
	* Maximum number of documents where a valid word can appear to be compute
	* in the clustering algorithm.
	*/
	unsigned int MaxDocs;


	/**
	* Minimum number of occurences needed in a document to take a valid word
	* in the clustering algorithm.
	*/
	unsigned int MinOccurCluster;

	/**
	* Size of the window use when the distance between words is use.
	*/
	unsigned int WindowSize;

	/**
	* Number of iteration of the words clustering algorithm.
	*/
	unsigned int NbIteration;

	/**
	* Minimum confidence of the clusters of words.
	*/
	double MinConfidence;

	/**
	* Minimum number of documents where a cluster of words must appear.
	*/
	unsigned int NbDocsMin;

	/**
	* Determine if the extracted words may contain other things than letters.
	*/
	bool NonLetterWords;

	/**
	* Determine if the distance between words in a document is use.
	*/
	bool Distance;

	/**
	* Determine if Links are used or not.
	*/
	bool UseLink;                                       

	/**
	* Determine if External links must be treated.
	*/
	bool UseExternalLink;

	/**
	* Determine if redirection must be used.
	*/
	bool UseRedirection;

	/**
	* Default Constructor.
	*/
	GDocOptions(void);

	/**
	* Copy Constructor.
	*/
	GDocOptions(const GDocOptions& opt);

	/**
	* Copy Constructor.
	*/
	GDocOptions(const GDocOptions* opt);

	/**
	* Assignment operator using another options.
	*/
	GDocOptions& operator=(const GDocOptions& opt);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
