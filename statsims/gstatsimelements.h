/*

	GALILEI Research Project

	GStatSimElements.h

	Template for computing similarities statistics.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
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
#include <gstatscalc.h>
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
	* The Mean of the Mean intra group similarity.
	*/
	double MeanIntra;

	/**
	* The Mean of the Mean extra group similarity.
	*/
	double MeanExtra;

	/**
	* Ratio Rie.
	*/
	double Rie;

	/**
	* Local Ratio Rie.
	*/
	double LocalRie;

	/**
	* Ratio Frac.
	*/
	double Frac;

	/**
	* Local Ratio Frac.
	*/
	double LocalFrac;

	/**
	* Overlap Factor.
	*/
	double Overlap;

	/**
	 * J Measure.
	 */
	double J;

	/**
	* Statistics Output file.
	*/
	R::RTextFile* Results;

	/**
	*/
	GMeasure* Measure;

	/**
	 * Number of elements compared.
	 */
	size_t NbElements;

	/**
	 * Number of subjects containing elements compared.
	 */
	size_t NbSubjects;

	/**
	 * Was the tile written.
	 */
	bool WriteTitle;

	/**
	 * Centers of each subject (only if same objects).
	 */
	R::RContainer<E1,false,false> Centers;

	/**
	 * Are the objects compared from the same type?
	 */
	bool SameObjects;

	/**
	 * Negative values for similarities?
	 */
	bool Neg;

public:

	/**
	* Constructor.
	* @param ses            The GALILEI session.
	* @param same           Same objects?
	* @param f              File.
	*/
	GStatSimElements(GSession* ses,bool same,R::RTextFile* f);

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
	* Compute the similarities statistics.
	*/
	void Run(GStatsCalc* calc,RXMLStruct* xml,RXMLTag* tag);

	/**
	 * Compute the similarities for a given subject.
	 */
	void ComputeSubject(GStatsCalc* calc,GSubject* sub,RXMLStruct* xml,RXMLTag* parent);

	/**
	* Destruct.
	*/
	virtual ~GStatSimElements(void) {}
};



//------------------------------------------------------------------------------
//
// Implementation
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
template<class E1,class E2>
	GStatSimElements<E1,E2>::GStatSimElements(GSession* ses,bool same,RTextFile* f)
	: Session(ses), Results(f), Centers(50), SameObjects(same)
{
}


//------------------------------------------------------------------------------
template<class E1,class E2>
	void GStatSimElements<E1,E2>::ComputeSubject(GStatsCalc* calc,GSubject* sub,RXMLStruct* xml,RXMLTag* parent)
{
	double SimIntra(0.0);         // Intra-similarity of the cluster
	double SimExtra(0.0);         // Extra-similarity of the cluster
	double SimEIntra(0.0);        // Intra-similarity of the element
	double SimEExtra(0.0);        // Extra-similarity of the element
	double MaxSimIntra(-2.0);     // Maximal intra-similarity
	double tmp;
	double LRie(0.0),LLocalRie(0.0),LFrac(0.0),LLocalFrac(0.0);
	size_t nbIntra(0);            // Number of element Intra of the cluster
	size_t nbExtra(0);            // Number of element Extra of the cluster
	size_t nbEIntra;              // Number of element Intra of the element
	size_t nbEExtra;              // Number of element Extra of the element
	double MinIntra(2.0);
	double MaxExtra(-2.0);
	double LOverlap(0.0);
	size_t LNbElements(0);
	E1* Center(0);                // Center of the subject

	// Go through each element of this subject
	R::RCursor<E1> Cur1(GetE1Cursor(sub));
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		if(!Cur1()->IsDefined()) continue;

		// Number of elements treated here
		double min(1.1),max(-1.1);
		NbElements++;
		LNbElements++;
		SimEIntra=SimEExtra=0.0;
		nbEIntra=nbEExtra=0;

		// Compute Intra-sim
		R::RCursor<E2> Cur2(GetE2Cursor(sub));
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			if(!Cur2()->IsDefined()) continue;
			if(SameObjects&&(Cur1()->GetId()==Cur2()->GetId())) continue;
			Measure->Measure(0,Cur1()->GetId(),Cur2()->GetId(),&tmp);
			if(tmp<0.0)
				Neg=true;
			nbIntra++;
			nbEIntra++;
			SimIntra+=tmp;
			SimEIntra+=tmp;
			if(tmp<MinIntra) MinIntra=tmp;
			if(tmp<min) min=tmp;
		}

		// Compute Extra-sim
		R::RCursor<GSubject> Subs2(Session->GetSubjects());
		for(Subs2.Start();!Subs2.End();Subs2.Next())
		{
			// Look if Same topic
			if(sub==Subs2()) continue;

			// Go through to other elements
			R::RCursor<E2> Cur2(GetE2Cursor(Subs2()));
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				if(!Cur2()->IsDefined()) continue;
				Measure->Measure(0,Cur1()->GetId(),Cur2()->GetId(),&tmp);
				if(tmp<0.0)
					Neg=true;
				nbExtra++;
				nbEExtra++;
				SimExtra+=tmp;
				SimEExtra+=tmp;
				if(tmp>MaxExtra) MaxExtra=tmp;
				if(tmp>max) max=tmp;
			}
		}

		// Compute Overlap for current element
		if(max>min)
		{
			LOverlap++;
			Overlap++;
		}

		// Compute local means
		if(nbEIntra)
			SimEIntra/=static_cast<double>(nbEIntra);
		if(nbEExtra)
			SimEExtra/=static_cast<double>(nbEExtra);

		// Verify if the actual element may be the center
		if(SimEIntra>MaxSimIntra)
		{
			MaxSimIntra=SimEIntra;
			Center=Cur1();
		}

		// Compute Rie local
		if(SimEIntra!=0.0)
		{
			LocalRie+=((SimEIntra-SimEExtra)/SimEIntra);
			LLocalRie+=((SimEIntra-SimEExtra)/SimEIntra);
		}

		// Compute MeanIntra/(MeanIntra+MeanExtra) local
		if(SimEIntra+SimEExtra!=0.0)
		{
			LocalFrac+=(SimEIntra/(SimEExtra+SimEIntra));
			LLocalFrac+=(SimEIntra/(SimEExtra+SimEIntra));
		}

	}

	if(!LNbElements)
		return;

	// Subject contains compared elements
	NbSubjects++;
	if(SameObjects)
	{
		Centers.InsertPtr(Center);  // Add the center
		J+=MaxSimIntra;
	}

	// Create tag
	RXMLTag* Tag=new RXMLTag(sub->GetName());
	xml->AddTag(parent,Tag);

	// Compute indicators for current subject
	if(nbIntra)
	{
		SimIntra/=static_cast<double>(nbIntra);
		MeanIntra+=SimIntra;
		calc->AddTag(xml,Tag,"Min Intra",MinIntra);
		calc->AddTag(xml,Tag,"Mean Intra",SimIntra);
	}

	if(nbExtra)
	{
		SimExtra/=static_cast<double>(nbExtra);
		MeanExtra+=SimExtra;
		calc->AddTag(xml,Tag,"Max Extra",MaxExtra);
		calc->AddTag(xml,Tag,"Mean Extra",SimExtra);
	}

	if(SimIntra!=0.0)
	{
		LRie=(SimIntra-SimExtra)/SimIntra;
		calc->AddTag(xml,Tag,"Rie",LRie);
	}
	LLocalRie/=static_cast<double>(LNbElements);
	calc->AddTag(xml,Tag,"LocalRie",LLocalRie);

	if(SimIntra+SimExtra!=0.0)
	{
		LFrac=SimIntra/(SimIntra+SimExtra);
		calc->AddTag(xml,Tag,"Frac",LFrac);
	}
	LLocalFrac/=static_cast<double>(LNbElements);
	calc->AddTag(xml,Tag,"LocalFrac",LLocalFrac);

	LOverlap/=static_cast<double>(LNbElements);
	calc->AddTag(xml,Tag,"Overlap",LOverlap);

	// File
	if(Results)
	{
		if(WriteTitle)
		{
			RString n1("Name"); n1.SetLen(25," ");
			RString n2("Min Intra"); n2.SetLen(15," ");
			RString n3("Mean Intra"); n3.SetLen(15," ");
			RString n4("Max Extra"); n4.SetLen(15," ");
			RString n5("Mean Extra"); n5.SetLen(15," ");
			RString n6("Rie"); n6.SetLen(15," ");
			RString n7("LocalRie"); n7.SetLen(15," ");
			RString n8("Frac"); n8.SetLen(15," ");
			RString n9("LocalFrac"); n9.SetLen(15," ");
			RString n10("Overlap"); n10.SetLen(15," ");
			RString n11("J"); n11.SetLen(15," ");
			(*Results)<<n1+n2+n3+n4+n5+n6+n7+n8+n9+n10+n11<<endl;
			WriteTitle=false;
		}

		RString n1(sub->GetName()); n1.SetLen(25," ");
		RString n2(RString::Number(MinIntra,"%.5E")); n2.SetLen(15," ");
		RString n3(RString::Number(SimIntra,"%.5E")); n3.SetLen(15," ");
		RString n4(RString::Number(MaxExtra,"%.5E")); n4.SetLen(15," ");
		RString n5(RString::Number(SimExtra,"%.5E")); n5.SetLen(15," ");
		RString n6(RString::Number(LRie,"%.5E")); n6.SetLen(15," ");
		RString n7(RString::Number(LLocalRie,"%.5E")); n7.SetLen(15," ");
		RString n8(RString::Number(LFrac,"%.5E")); n8.SetLen(15," ");
		RString n9(RString::Number(LLocalFrac,"%.5E")); n9.SetLen(15," ");
		RString n10(RString::Number(LOverlap,"%.5E")); n10.SetLen(15," ");
		RString n11("------"); n11.SetLen(15," ");
		(*Results)<<n1+n2+n3+n4+n5+n6+n7+n8+n9+n10+n11<<endl;
	}

	if(Tag->IsEmpty())
		xml->DeleteTag(Tag);
}


//------------------------------------------------------------------------------
template<class E1,class E2>
	void GStatSimElements<E1,E2>::Run(GStatsCalc* calc,RXMLStruct* xml,RXMLTag* tag)
{
	// Initialization
	Neg=false;
	WriteTitle=true;
	NbElements=NbSubjects=0;
	Overlap=MeanExtra=MeanIntra=0.0;
	J=Rie=LocalRie=Frac=LocalFrac=0.0;

	// Compute minimum similarity
	if(SameObjects)
	{
		double minsim,avgsim,devsim;
		Measure->Info(0,&minsim);
		Measure->Info(1,&avgsim);
		Measure->Info(2,&devsim);
		calc->AddTag(xml,tag,"MinSim",minsim);
		calc->AddTag(xml,tag,"AvgSim",avgsim);
		calc->AddTag(xml,tag,"DevSim",devsim);
		if(Results)
		{
			RString n1(""); n1.SetLen(25," ");
			RString n2("Min Sim"); n2.SetLen(15," ");
			RString n3("Avg Sim"); n3.SetLen(15," ");
			RString n4("Dev Sim"); n4.SetLen(15," ");
			(*Results)<<n1+n2+n3+n4<<endl;
			n1="Global"; n1.SetLen(24," ");
			n2=RString::Number(minsim,"%.5E"); n2.SetLen(15," ");
			n3=RString::Number(avgsim,"%.5E"); n3.SetLen(15," ");
			n4=RString::Number(devsim,"%.5E"); n4.SetLen(15," ");
			(*Results)<<n1+n2+n3+n4<<endl;
			(*Results)<<"--------------"<<endl;
		}
	}

	// Go trough the subjects
	Centers.Clear();
	R::RCursor<GSubject> Sub(Session->GetSubjects());
	for(Sub.Start();!Sub.End();Sub.Next())
		ComputeSubject(calc,Sub(),xml,tag);
	if(SameObjects)
	{
		// Compute max inter-similarity and then J
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
					Neg=true;
				avg+=tmp;
			}
			avg/=static_cast<double>(nb);
			if(avg>max)
				max=avg;
		}
		if(Neg)
		{
			J=(J+1.0)/2.0;
			max=(max+1.0)/2.0;
		}
		J/=(static_cast<double>(NbSubjects)*max);
	}

	// Compute elements statistics
	if(NbElements)
	{
		MeanIntra/=static_cast<double>(NbSubjects);
		MeanExtra/=static_cast<double>(NbSubjects);
		if(MeanIntra!=0.0)
			Rie=(MeanIntra-MeanExtra)/MeanIntra;
		if(MeanIntra+MeanExtra!=0.0)
			Frac=MeanIntra/(MeanExtra+MeanIntra);
		Overlap/=static_cast<double>(NbElements);
		LocalRie/=static_cast<double>(NbElements);
		LocalFrac/=static_cast<double>(NbElements);

		calc->AddTag(xml,tag,"Mean Intra",MeanIntra);
		calc->AddTag(xml,tag,"Mean Extra",MeanExtra);
		calc->AddTag(xml,tag,"Rie",Rie);
		calc->AddTag(xml,tag,"LocalRie",LocalRie);
		calc->AddTag(xml,tag,"Frac",Frac);
		calc->AddTag(xml,tag,"LocalFrac",LocalFrac);
		calc->AddTag(xml,tag,"Overlap",Overlap);
		calc->AddTag(xml,tag,"J",J);

		if(Results)
		{
			RString n1("Global"); n1.SetLen(25," ");
			RString n2("------"); n2.SetLen(15," ");
			RString n3(RString::Number(MeanIntra,"%.5E")); n3.SetLen(15," ");
			RString n4("------"); n4.SetLen(15," ");
			RString n5(RString::Number(MeanExtra,"%.5E")); n5.SetLen(15," ");
			RString n6(RString::Number(Rie)); n6.SetLen(15," ");
			RString n7(RString::Number(LocalRie)); n7.SetLen(15," ");
			RString n8(RString::Number(Frac)); n8.SetLen(15," ");
			RString n9(RString::Number(LocalFrac)); n9.SetLen(15," ");
			RString n10(RString::Number(Overlap,"%.5E")); n10.SetLen(15," ");
			RString n11(RString::Number(J,"%.5E")); n11.SetLen(15," ");
			(*Results)<<n1+n2+n3+n4+n5+n6+n7+n8+n9+n10+n11<<endl;
			(*Results)<<"--------------"<<endl;
		}
	}
}
