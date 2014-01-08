/*

	GALILEI Research Project

	GStatSimElements.h

	Template for computing similarities statistics.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
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



//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <rcursor.h>
#include <gcommunity.h>
#include <ggalileiapp.h>
#include <statsims.h>
#include <rmaxvector.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
/**
* The GStatSimElements template provides statistics on the elements based on
* their similarities.
* @author Pascal Francq.
* @short Similarities between elements.
*/
template<class E1,class E2>
	class GStatSimElements
{
protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	*/
	GMeasure* Measure;

	/**
	 * Are the objects compared from the same type?
	 */
	bool SameObjects;

	/**
	 * Type of the first objects.
	 */
	tObjType ObjType1;

	/**
	 * Type of the second objects.
	 */
	tObjType ObjType2;

	/**
	 * Statistics.
	 */
	RWorksheet& Stats;

	/**
	 * First Index.
	 */
	size_t FirstIndex;

public:

	/**
	* Constructor.
	* @param ses            The GALILEI sessi,RTextFile& fon.
	* @param objtype1       Type of the first objects.
	* @param objtype2       Type of the second objects.
	* @param stats          Statistics.
	* @param idx            First index.
	*/
	GStatSimElements(GSession* ses,tObjType objtype1,tObjType objtype2,RWorksheet& stats,size_t idx);

	/**
	 * Cursor over the first type of objects.
	 * @param sub            Subject.
	 */
	virtual R::RCursor<E1> GetE1Cursor(void)=0;

	/**
	 * Cursor over the first type of objects.
	 * @param sub            Subject.
	 */
	virtual R::RCursor<E1> GetE1Cursor(GSubject* sub)=0;

	/**
	 * Cursor over the second type of objects.
	 * @param sub            Subject.
	 */
	virtual R::RCursor<E2> GetE2Cursor(GSubject* sub)=0;

	/**
	 * Compare the vectors of two objects. In practice, each concept describing
	 * the two objects is printed with the current weights and idf.
	 * @param id1            Identifier of the first object.
	 * @param id2            Identifier of the second object.
	 */
	void CompareVectors(size_t id1,size_t id2);

	/**
	* Compute the similarities statistics.
	*/
	void RunComplete(void);

	/**
	* Compute the similarities statistics.
	*/
	void RunNearestNeighbors(void);

	/**
	* Compute the similarities statistics.
	*/
	void Run(const RString& type);

	virtual ~GStatSimElements(void) {}
};



//------------------------------------------------------------------------------
//
// Implementation
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
template<class E1,class E2>
	GStatSimElements<E1,E2>::GStatSimElements(GSession* ses,tObjType objtype1,tObjType objtype2,RWorksheet& stats,size_t idx)
	: Session(ses), SameObjects(objtype1==objtype2),
	  ObjType1(objtype1), ObjType2(objtype2), Stats(stats), FirstIndex(idx)
{
}


//------------------------------------------------------------------------------
template<class E1,class E2>
	void GStatSimElements<E1,E2>::CompareVectors(size_t id1,size_t id2)
{
	E1* Obj1(dynamic_cast<E1*>(Session->GetObj(ObjType1,id1)));
   if(!Obj1)
       mThrowGException("No "+GetObjType(ObjType1,false,false)+" with identifier "+RString::Number(id1));
	E2* Obj2(dynamic_cast<E2*>(Session->GetObj(ObjType2,id2)));
   if(!Obj2)
       mThrowGException("No "+GetObjType(ObjType2,false,false)+" with identifier "+RString::Number(id2));

	// See the common concepts : Parse the vector to found those associated wit the same concept
   RCursor<GVector> Vec1(Obj1->GetVectors());
   RCursor<GVector> Vec2(Obj2->GetVectors());
	for(Vec1.Start(),Vec2.Start();!Vec1.End();Vec1.Next())
	{
		// Parse all the elements of Vec2 with an identifier lower than the current element of Vec1
		while((!Vec2.End())&&(Vec2()->GetMetaConcept()->GetId()<Vec1()->GetMetaConcept()->GetId()))
			Vec2.Next();

		// Verify if both elements are identical
		if((!Vec2.End())&&(Vec2()->GetMetaConcept()->GetId()==Vec1()->GetMetaConcept()->GetId()))
		{
          // Parse the concepts
          RCursor<GConceptRef> Concept1(Vec1()->GetRefs());
          RCursor<GConceptRef> Concept2(Vec2()->GetRefs());
          for(Concept1.Start(),Concept2.Start();!Concept1.End();Concept1.Next())
          {
               // Parse all the elements of Concept2 with an identifier lower than the current element of Concept1
               while((!Concept2.End())&&(Concept2()->GetId()<Concept1()->GetId()))
                  Concept2.Next();

              // Verify if both concepts are identical
              if((!Concept2.End())&&(Concept2()->GetId()==Concept1()->GetId()))
              {
                 // Print the word
                 RString Value1(RString::Number(Concept1()->GetWeight()));
                 RString Value2(RString::Number(Concept2()->GetWeight()));
                 RString Idf;
                 if(SameObjects)
                 {
                    Idf=","+RString::Number(Concept1()->GetConcept()->GetIF(ObjType1));
                    //Idf=","+Idf;
                 }
                 cout<<Concept1()->GetConcept()->GetName()<<": "<<Value1+","+Value2+Idf<<endl;
              }
          }
      }
   }
}


//------------------------------------------------------------------------------
template<class E1,class E2>
	void GStatSimElements<E1,E2>::RunComplete(void)
{
	size_t NbSubjects(0);                   // Number of subjects
	double AvgMinIntra(0.0);                // Average minimum intra-cluster similarity
	double AvgIntra(0.0);                   // Average intra-cluster similarity
	double AvgMaxInter(0.0);                // Average maximum inter-cluster similarity
	double AvgInter(0.0);                   // Average inter-cluster similarity
	double AvgRie(0.0);                     // Average Rie
	double AvgIn(0.0);                      // Average incoherence
	double AvgOv(0.0);                      // Average overlap
	double J(0.0);                          // J Measure
	RContainer<E1,false,false> Centers(100);

	// Go trough the subjects
	R::RCursor<GSubject> Sub(Session->GetObjs(pSubject));
	size_t Line;
	for(Sub.Start(),Line=0;!Sub.End();Sub.Next(),Line++)
	{
		// Add line in the worksheet
		Stats.AddLine(Line,Sub()->GetName());

		size_t NbElements(0);                        // Number of elements in Sub()
		double LocalAvgMinIntra(2.0);                // Average minimum intra-cluster similarity for Sub()
		double LocalAvgIntra(0.0);                   // Average intra-cluster similarity for Sub()
		double LocalAvgMaxInter(-2.0);                // Average maximum inter-cluster similarity for Sub()
		double LocalAvgInter(0.0);                   // Average inter-cluster similarity for Sub()
		double LocalAvgRie(0.0);                     // Average Rie for Sub()
		double LocalAvgIn(0.0);                      // Average incoherence for Sub()
		double LocalAvgOv(0.0);                      // Average overlap for Sub()
		double MaxIntra(-2.0);                       // Maximal intra-similarity
		E1* Center(0);                               // Center of the subject
		double tmp;

		// Go through each element of this subject
		R::RCursor<E1> Cur1(GetE1Cursor(Sub()));
		for(Cur1.Start();!Cur1.End();Cur1.Next())
		{
			if(!Cur1()->IsDefined()) continue;
			NbElements++;

			// Init measure for object
			double MinIntra(2.0);                // Minimum intra-cluster similarity
			double Intra(0.0);                   // Intra-cluster similarity
			double MaxInter(-2.0);               // Maximum inter-cluster similarity
			double Inter(0.0);                   // Inter-cluster similarity
			double In(0.0);                      // Incoherence
			size_t NbIntraElements(0);           // Number of elements inside the subject
			size_t NbInterElements(0);           // Number of elements outside the subject

			// Compute Extra-sim
			R::RCursor<GSubject> Subs2(Session->GetObjs(pSubject));
			for(Subs2.Start();!Subs2.End();Subs2.Next())
			{
				// Look if Same topic
				if(Sub()==Subs2()) continue;

				// Go through to other elements
				R::RCursor<E2> Cur2(GetE2Cursor(Subs2()));
				for(Cur2.Start();!Cur2.End();Cur2.Next())
				{
					// Verify if the object must be treated
					if(!Cur2()->IsDefined()) continue;
					NbInterElements++;

					// Treat the similarity
					Measure->Measure(0,Cur1()->GetId(),Cur2()->GetId(),&tmp);
					if(tmp<0.0)
						mThrowGException("Negative similarity : Similarity ∈ [0,1]");

					Inter+=tmp;
					if(tmp>MaxInter)
						MaxInter=tmp;
				}
			}

			// Compute Intra-sim
			R::RCursor<E2> Cur2(GetE2Cursor(Sub()));
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				// Verify if the object must be treated
				if(!Cur2()->IsDefined()) continue;
				if(SameObjects&&(Cur1()->GetId()==Cur2()->GetId())) continue;
				NbIntraElements++;

				// Treat the similarity
				Measure->Measure(0,Cur1()->GetId(),Cur2()->GetId(),&tmp);
				if(tmp<0.0)
					mThrowGException("Negative similarity : Similarity ∈ [0,1]");

				Intra+=tmp;
				if(tmp<MinIntra)
					MinIntra=tmp;
				if(tmp<MaxInter)
					In+=1.0;
			}

			if(NbIntraElements)
			{
				Intra/=static_cast<double>(NbIntraElements);
				if(Intra>MaxIntra)
				{
					MaxIntra=Intra;
					Center=Cur1();
				}
				In/=static_cast<double>(NbIntraElements);
				LocalAvgIntra+=Intra;
				LocalAvgIn+=In;
				LocalAvgMinIntra+=MinIntra;
			}

			if(NbInterElements)
			{
				Inter/=static_cast<double>(NbInterElements);
				LocalAvgInter+=Inter;
				LocalAvgMaxInter+=MaxInter;
			}

			if((NbIntraElements)&&(NbInterElements))
			{
				LocalAvgRie+=(Intra-Inter)/Intra;
				if(MinIntra<MaxInter)
					LocalAvgOv+=1.0;
			}
		}

		// Subject contains compared elements
		if(SameObjects)
		{
			Centers.InsertPtr(Center);  // Add the center
			J+=MaxIntra;
		}

		if(NbElements)
		{
			LocalAvgMinIntra/=static_cast<double>(NbElements);
			LocalAvgIntra/=static_cast<double>(NbElements);
			LocalAvgInter/=static_cast<double>(NbElements);
			LocalAvgMaxInter/=static_cast<double>(NbElements);
			LocalAvgIn/=static_cast<double>(NbElements);
			LocalAvgOv/=static_cast<double>(NbElements);
			LocalAvgRie/=static_cast<double>(NbElements);

			NbSubjects++;
			Stats.AddLine(Line,Sub()->GetName());
			Stats.SetValue(FirstIndex,Line,LocalAvgMinIntra);
			Stats.SetValue(FirstIndex+1,Line,LocalAvgIntra);
			Stats.SetValue(FirstIndex+2,Line,LocalAvgMaxInter);
			Stats.SetValue(FirstIndex+3,Line,LocalAvgInter);
			Stats.SetValue(FirstIndex+4,Line,LocalAvgRie);
			Stats.SetValue(FirstIndex+5,Line,LocalAvgIn*100.0);
			Stats.SetValue(FirstIndex+6,Line,LocalAvgOv*100.0);
			Stats.InvalidValue(FirstIndex+7,Line);

			AvgMinIntra+=LocalAvgMinIntra;
			AvgIntra+=LocalAvgIntra;
			AvgInter+=LocalAvgInter;
			AvgMaxInter+=LocalAvgMaxInter;
			AvgIn+=LocalAvgIn;
			AvgOv+=LocalAvgOv;
			AvgRie+=LocalAvgRie;
		}
	}

	// Compute elements statistics
	if(NbSubjects)
	{
		AvgMinIntra/=static_cast<double>(NbSubjects);
		AvgIntra/=static_cast<double>(NbSubjects);
		AvgInter/=static_cast<double>(NbSubjects);
		AvgMaxInter/=static_cast<double>(NbSubjects);
		AvgIn/=static_cast<double>(NbSubjects);
		AvgOv/=static_cast<double>(NbSubjects);
		AvgRie/=static_cast<double>(NbSubjects);

		// Compute max inter-similarity and then J
		if(SameObjects)
		{
			R::RCursor<E1> Cur1(Centers);
			size_t nb=Centers.GetNb()-1;
			double max(-2.0);
			for(Cur1.Start();!Cur1.End();Cur1.Next())
			{
				double avg(0.0);
				R::RCursor<E1> Cur2(Centers);
				for(Cur2.Start();!Cur2.End();Cur2.Next())
				{
					if(Cur1()==Cur2()) continue;
					double tmp;
					Measure->Measure(0,Cur1()->GetId(),Cur2()->GetId(),&tmp);
					if(tmp<0.0)
						mThrowGException("Negative similarity : Similarity ∈ [0,1]");
					avg+=tmp;
				}
				avg/=static_cast<double>(nb);
				if(avg>max)
					max=avg;
			}
			J/=(static_cast<double>(NbSubjects)*max);
		}

		Stats.AddLine(Line,"Global");
		Stats.SetValue(FirstIndex,Line,AvgMinIntra);
		Stats.SetValue(FirstIndex+1,Line,AvgIntra);
		Stats.SetValue(FirstIndex+2,Line,AvgMaxInter);
		Stats.SetValue(FirstIndex+3,Line,AvgInter);
		Stats.SetValue(FirstIndex+4,Line,AvgRie);
		Stats.SetValue(FirstIndex+5,Line,AvgIn*100.0);
		Stats.SetValue(FirstIndex+6,Line,AvgOv*100.0);
		Stats.SetValue(FirstIndex+7,Line,J);
	}
}


//------------------------------------------------------------------------------
template<class E1,class E2>
	void GStatSimElements<E1,E2>::RunNearestNeighbors(void)
{
	// Global statistics
	size_t NbSubjects(0);         // Number of subjects.
	double AvgCols(0.0);          // Average number of nearest neighbors.
	double AvgIn(0.0);            // Average incoherence (average percentage of nearest neighbors to be in the same subject).
	double AvgOv(0.0);            // Average overlap (average number of elements having one nearest neighbor not from the same subject).

	size_t Line;
	R::RCursor<GSubject> Sub(Session->GetObjs(pSubject));
	for(Sub.Start(),Line=0;!Sub.End();Sub.Next(),Line++)
	{
		// Local statistics
		size_t LocalNbLines(0);          // Number of "lines" elements in Sub().
		double LocalAvgCols(0.0);        // Average number of nearest neighbors in Sub().
		double LocalAvgIn(0.0);          // Average incoherence in Sub() (average percentage of nearest neighbors to be in the same subject).
		double LocalAvgOv(0.0);          // Average overlap in Sub() (average number of elements having one nearest neighbor not from the same subject).

		// Go through each element of Sub()
		R::RCursor<E1> Lines(GetE1Cursor(Sub()));
		for(Lines.Start();!Lines.End();Lines.Next())
		{
			// Increment the number of elements
			LocalNbLines++;

			// Initialize the values
			size_t NbCols(0);          // Number of nearest neighbors.
			double In(0.0);            // Incoherence.
			bool Overlap(false);       // Suppose no overlap

			// Compute the nearest neighbors of the same subject
			const RMaxVector* Vec;
			Measure->Info(3,Lines()->GetId(),&Vec);
			RCursor<RMaxValue> Cur(*Vec);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				NbCols++;  // Increase the number of nearest neighbors

				// Get the subject of Cur()
				E2* Col(static_cast<E2*>(Session->GetObj(ObjType2,Cur()->Id)));
				const GSubject* Subject2(Session->GetObj(pSubject,Col));

				if(Sub()==Subject2)
					In+=1.0;
				else
					Overlap=true;
			}
			if(NbCols)
				In/=NbCols;
			LocalAvgIn+=In;
			LocalAvgCols+=NbCols;
			if(Overlap)
				LocalAvgOv+=1.0;
		}

		if(LocalNbLines)
		{
			LocalAvgIn/=static_cast<double>(LocalNbLines);
			LocalAvgCols/=static_cast<double>(LocalNbLines);
			LocalAvgOv/=static_cast<double>(LocalNbLines);
			AvgIn+=LocalAvgIn;
			AvgOv+=LocalAvgOv;
			AvgCols+=LocalAvgCols;
			NbSubjects++;
			Stats.AddLine(Line,Sub()->GetName());
			Stats.SetValue(FirstIndex,Line,LocalAvgIn*100.0);
			Stats.SetValue(FirstIndex+1,Line,LocalAvgOv*100.0);
			Stats.SetValue(FirstIndex+2,Line,LocalAvgCols);
		}
	}

	if(NbSubjects)
	{
		AvgCols/=static_cast<double>(NbSubjects);
		AvgOv/=static_cast<double>(NbSubjects);
		AvgIn/=static_cast<double>(NbSubjects);

		Stats.AddLine(Line,"Global");
		Stats.SetValue(FirstIndex,Line,AvgIn*100.0);
		Stats.SetValue(FirstIndex+1,Line,AvgOv*100.0);
		Stats.SetValue(FirstIndex+2,Line,AvgCols);
	}
}


//------------------------------------------------------------------------------
template<class E1,class E2>
	void GStatSimElements<E1,E2>::Run(const RString& type)
{
	if(type=="Complete")
		RunComplete();
	else if(type=="Nearest Neighbors")
		RunNearestNeighbors();
	else
		mThrowGException("'"+type+"' is invalid : Only 'Complete' or 'Nearest Neighbors' are allowed for the type of measure");
}
