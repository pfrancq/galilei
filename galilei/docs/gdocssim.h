/*

	GALILEI Research Project

	GDocsSim.h

	Similarities between documents - Header

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
#ifndef GDocsSimH
#define GDocsSimH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rrecfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GDocsSim class provides a representation for all the similarities between
* the documents. Two documents of different languages are supposed to have a
* null similarity.
* @author Pascal Francq
* @short Documents Similarities.
*/
class GDocsSim
{
public:

	/**
	* Class representing a specific similarity between two documents.
	*/
	class GSim
	{
	public:
		/**
		* Identificator of the first document.
		*/
		unsigned int Id1;

		/**
		* Identificator of the second document.
		*/
		unsigned int Id2;

		/**
		* Similarity.
		*/
		double Sim;

		/**
		* Default constructor.
		*/
		GSim(void);

		/**
		* Copy constructor.
		*/
		GSim(const GSim& r);

		/**
		* Constructor.
		*/
		GSim(unsigned int id1,unsigned id2,double s);

		/**
		* Compare method needed by R::RRecFile.
		*/
		int Compare(const GSim* rec) const;

		/**
		* Compare method needed by R::RRecFile.
		*/
		int Compare(const GSim& rec) const;

		/**
		* Assignment operator.
		*/
		GSim& operator=(const GSim& src);

		/**
		* Write a record to a R::RRecFile.
		*/
		void Write(R::RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true>& f) throw(R::RString);

		/**
		* Read a record from a R::RRecFile.
		*/
		void Read(R::RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true>& f) throw(R::RString);
	};

private:

	/**
	* The similarities.
	*/
	R::RRecFile<GSim,sizeof(double)+2*sizeof(unsigned int),true>* Sims;

public:

	/**
	* Constructor.
	* @param filename       Name of the file containing the similarities.
	* @param d              Documents of the system.
	* @param global         Global approach.
	*/
	GDocsSim(const char* filename,GDocs* d,bool global) throw(bad_alloc,R::RString);

	/**
	* Constructor.
	* @param filename       Name of the file containing the similarities.
	* @param d              documents of the system.
	* @param global         Global approach.
	*/
	GDocsSim(const char* filename,GDocs& d,bool global) throw(bad_alloc,R::RString);

protected:

	/**
	* Compute the similarities.
	* @param filename       Name of the file containing the similarities.
	* @param Cur1           First cursor.
	* @param Cur2           Second cursor.
	* @param global         Global approach.
	*/
	void ComputeSims(const char* filename,GDocCursor& Cur1,GDocCursor& Cur2,bool global) throw(bad_alloc,R::RString);

public:

	/**
	* Get the similarities between two documents.
	* @param i              Identificator of the first document.
	* @param j              Identificator of the second document.
	* @return double.
	*/
	double GetSim(unsigned int i,unsigned int j);

	/**
	* Get the similarities between two documents.
	* @param d1             Pointer to the first document.
	* @param d2             Pointer to second document.
	* @return double.
	*/
	double GetSim(const GDoc* d1,const GDoc* d2);

	/**
	* Destructor.
	*/
	~GDocsSim(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
