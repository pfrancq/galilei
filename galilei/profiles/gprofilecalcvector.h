/*

	GALILEI Research Project

	GProfileCalcVector.h

	Vector Computing Method - Header.

	Copyright 2001-2002 by the Universit� Libre de Bruxelles.

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
#ifndef GProfileCalcVectorH
#define GProfileCalcVectorH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gprofilecalc.h>
#include <profiles/gcalcparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
/**
* The GStatisticalParam represents all the parameter used in the CalcVector module.
* @short GStatistical Parameters.
*/
class GStatisticalParams : public GCalcParams
{
public:

	/**
	* Maximal number of the non-zero weights in the vector.
	*/
	unsigned int MaxNonZero;

	/**
	* Must the idf factor be computed.
	*/
	bool IdfFactor;

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*);

	/**
	* Assignment operator.
	* @param p              Parameters used as source.
	*/
	GStatisticalParams& operator=(const GStatisticalParams& src);

	/**
	* Constructor.
	*/
	GStatisticalParams(void);
};


//-----------------------------------------------------------------------------
/**
* The GProfileCalcVector class provides an implementation of the computing
* method "Statistical", which is based on the idea that the profile vector
* is the one obtain from the documents judged as relevant.
*
* The different parameters of the method:
* @param MaxNonZero         Maximal number of weights of the profile vector not
*                           to be zero. If this paramater is null, all the
*                           weights are left.
* @param IdfFactor          Specify if for the weights of documents the idf
*                           factor of the vector model theory must be computed.
* @author Pascal Francq
* @short Statistical Profile Computing Method.
*/
class GProfileCalcVector : public GProfileCalc
{
protected:
	class GNbDocsLangs;
	class InternVector;

	/**
	* Statistical parameters.
	*/
	GStatisticalParams* Params;

	/**
	* Occurences of the index terms in the "OK" and "N" documents for the
	* different languages.
	*/
	RStd::RContainer<InternVector,unsigned int,true,true> Vector;

	/**
	* Number of documents where each index term of the "OK" and "N" documents
	* for the different languages appears.
	*/
	RStd::RContainer<InternVector,unsigned int,true,true> NbDocsWords;

	/**
	* Number of documents per languages.
	*/
	RStd::RContainer<GNbDocsLangs,unsigned int,true,true> NbDocsLangs;

	/**
	* Ordered vector for current computed profile.
	*/
	GIWordWeight** Order;

	/**
	* Maximal size allocate for a profile.
	*/
	unsigned int MaxOrderSize;


public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GProfileCalcVector(GSession* session, GStatisticalParams* p) throw(bad_alloc);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* "MaxNonZero Idf"
	* @param s              C string coding the settings.
	*/
	virtual void SetSettings(const char* s);

	/**
	* Computes the OK lists for a subprofile.
	* @param profile        Profile to compute.
	*/
	void ComputeGlobal(GSubProfile* subprofile) throw(bad_alloc);

	/**
	* Computes the subprofile of a profile.
	* @param s              SubProfile to compute.
	*/
	void ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc);

	/**
	* Compute a profile.
	* @param profile        Profile to compute.
	*/
	virtual void Compute(GSubProfile* subprofile);

	/**
	* Get the type of the method implemented. This method is used for the
	* vector model.
	* @returns tSubProfileDesc enum type.
	*/
	virtual tSubProfileDesc GetType(void) const {return(sdVector);}

	/**
	* Destructor.
	*/
	virtual ~GProfileCalcVector(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

