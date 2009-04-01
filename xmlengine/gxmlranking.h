/*

	GALILEI Research Project

	GXMLRanking.h

	Heuristic using a GGA - Header.

	Copyright 2004-2009 by the Universit�Libre de Bruxelles.

	Authors:
		Faïza Abbaci (fabbaci@ulb.ac.be)

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
#ifndef GXmlRankingH
#define GXmlRankingH


//-----------------------------------------------------------------------------

// include files for GALILEI
#include <gstatscalc.h>
#include <gxmlprom.h>
#include <gxmlfrag.h>
#include <xnodeset.h>
#include <gxmlir.h>

//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

class GXmlRanking
{


protected:
	/**
	* Parameters .
	*/
	GXmlParams Params;
	/**
	* Instance of Promethee algorithm.
	*/
	GXmlProm* ThProm;
	/**
	* Solutions corresponding
	*/
	R::RPromSol** Sols;


public:
	//used from Promethhe
	GALILEI::GStorage *storage;
	GALILEI::GStorageTag *cmdtag;
	GALILEI::GStorageTag *cmdtag_2;

	double TFIDF(int _idfile, RContainer<RString, false, false>& _keywordlist); //calculate TF-IDF factor (traditionnal)
	double TFIDF_Nodes(int _idfile, RContainer<RString, false, false>& _keywordlist); //calculate TF-IEF
	/**
	* Constructor.
	* @param cnset nodes to rank
	*/
	GXmlRanking(GALILEI::GStorage *_storage, GALILEI::GXmlParams _Params);



	/**
	* Compute the statistics.
	* @param xml             XML Structure.
	* @param res             XML tag that will be hold the results.
	*/
//NORMAL
	virtual void Compute(RCursor<XNodeSet> cns, RContainer<RString, false, false>& keywordlist, R::RString Name);
	//virtual void Compute(R::RString Name);

	virtual ~GXmlRanking(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
