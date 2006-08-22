/*

	GALILEI Research Project

	GSatSimElements.cpp

	Template for computing Similarities between elmeents.

	Copyright 2002-2004 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for R
#include <rcursor.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggalileiapp.h>


//------------------------------------------------------------------------------
/**
* The GStatSimElements template provides statistics on the elemets based on
* their similarities.
* @author Pascal Francq.
* @short Similarities between elements.
*/
template<class E1,class E2>
	class GStatSimElements
{
public:

	// Local Statistics
	class LocalStat
	{
	public:
		GSubject* Sub;
		bool Overlap;

		LocalStat(GSubject* s) : Sub(s), Overlap(false) {}
		int Compare(const LocalStat& l) const {return(Sub->Compare(*l.Sub));}
		int Compare(const GSubject& s) const {return(Sub->Compare(s));}
	};

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
	* Global Overlap Factor (Without IF).
	*/
	double GOverlap;

	/**
	* Overlap Factor (Without IF).
	*/
	double Overlap;

	/**
	* Statistics Output file.
	*/
	R::RTextFile* File;

	/**
	* Container of statistics on the subjects.
	*/
	R::RContainer<LocalStat,true,true> Sub;

	/**
	*/
	GMeasure* Measure;

	unsigned int nbElements;

public:

	/**
	* Constructor.
	* @param ses            The  galilei session.
	* @param f              File.
	*/
	GStatSimElements(GSession* ses,R::RTextFile* f);

	virtual R::RCursor<E1> GetE1Cursor(GSubject* sub,GLang* lang)=0;

	virtual R::RCursor<E2> GetE2Cursor(GSubject* sub,GLang* lang)=0;

	/**
	* Compute the similarities statistics.
	*/
	void Run(GStatsCalc* calc,RXMLStruct* xml,RXMLTag* tag);

	/**
	*/
	void ComputeSubject(GStatsCalc* calc,GSubject* sub,GLang* lang,RXMLStruct* xml,RXMLTag* parent);

	/**
	* Destructor for the main view.
	*/
	virtual ~GStatSimElements(void) {}
};



//------------------------------------------------------------------------------
//
// Implementation
//
//------------------------------------------------------------------------------

#include <gstatscalc.h>
//------------------------------------------------------------------------------
template<class E1,class E2>
	GStatSimElements<E1,E2>::GStatSimElements(GSession* ses,RTextFile* f)
	: Session(ses), File(f), Sub(100,50)
{
	if(File)
	{
		(*File)<<"Id";
		(*File)<<"Rie";
		(*File)<<endl;
	}
}


//------------------------------------------------------------------------------
template<class E1,class E2>
	void GStatSimElements<E1,E2>::ComputeSubject(GStatsCalc* calc,GSubject* sub,GLang* lang,RXMLStruct* xml,RXMLTag* parent)
{
	double SimIntra;
	double SimExtra;
	double tmp;
	unsigned int nbIntra;
	unsigned int nbExtra;
	bool Same;
	double MinIntra;
	double MaxExtra;

	// Create tag
	RXMLTag* Tag=new RXMLTag(sub->GetName());
	xml->AddTag(parent,Tag);

	// Init Local Data
	SimIntra=SimExtra=0.0;
	nbIntra=nbExtra=0;
	MinIntra=1.1;
	MaxExtra=-1.1;

	// Go through each element of this subject
	R::RCursor<E1> Cur1(GetE1Cursor(sub,lang));
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		if(Cur1()->GetLang()!=lang) continue;
		if(!Cur1()->IsDefined()) continue;

		// Go trough the other subjects
		R::RCursor<GSubject> Subs2(Session->GetSubjects()->GetNodes());
		for(Subs2.Start();!Subs2.End();Subs2.Next())
		{
			// Look if Same topic
			Same=(sub==Subs2());

			// Go through to other elements
			R::RCursor<E2> Cur2(GetE2Cursor(Subs2(),lang));
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				if(Cur2()->GetLang()!=lang) continue;
				if(!Cur2()->IsDefined()) continue;

				tmp=Measure->GetMeasure(Cur1()->GetId(),Cur2()->GetId());
				if(Same)
				{
					nbIntra++;
					SimIntra+=tmp;
					if(tmp<MinIntra) MinIntra=tmp;
				}
				else
				{
					nbExtra++;
					SimExtra+=tmp;
					if(tmp>MaxExtra) MaxExtra=tmp;
				}
			}
		}
	}

	// Compute Overlap for current subject
	if(MaxExtra>MinIntra)
		Overlap++;

	// Compute Rie for current element
	if(nbIntra)
	{
		SimIntra/=nbIntra;
		MeanIntra+=SimIntra;
		calc->AddTag(xml,Tag,"Min Intra",MinIntra);
		calc->AddTag(xml,Tag,"Mean Intra",SimIntra);
	}

	if(nbExtra)
	{
		SimExtra/=nbExtra;
		MeanExtra+=SimExtra;
		calc->AddTag(xml,Tag,"Max Extra",MaxExtra);
		calc->AddTag(xml,Tag,"Mean Extra",SimExtra);
	}

	if(nbIntra||nbExtra)
		nbElements++;

	if(SimIntra!=0.0)
	{
		tmp=(SimIntra-SimExtra)/SimIntra;
		if(File)
			(*File)<<sub->GetId()<<tmp<<endl;
		calc->AddTag(xml,Tag,"Rie",tmp);
	}

	// Go trough Subtopic
	R::RCursor<GSubject> Cur(sub->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		ComputeSubject(calc,Cur(),lang,xml,Tag);

	if(Tag->IsEmpty())
		xml->DeleteTag(Tag);
}


//------------------------------------------------------------------------------
template<class E1,class E2>
	void GStatSimElements<E1,E2>::Run(GStatsCalc* calc,RXMLStruct* xml,RXMLTag* tag)
{
	RXMLTag* LangTag;


	// Go through the languages
	R::RCursor<GLang> Langs(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());
	R::RCursor<GSubject> Subs1(Session->GetSubjects()->GetTop()->GetNodes());
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		nbElements=0;
		LangTag=new RXMLTag(Langs()->GetName());
		xml->AddTag(tag,LangTag);

		// Initialization
		Overlap=MeanExtra=MeanIntra=0.0;
		Sub.Clear();

		// Go trough the subjects
		for(Subs1.Start();!Subs1.End();Subs1.Next())
			ComputeSubject(calc,Subs1(),Langs(),xml,LangTag);

		// Compute elements statistics
		if(nbElements)
		{
			MeanIntra/=nbElements;
			MeanExtra/=nbElements;
			if(MeanIntra==0.0)
				Rie=0.0;
			else
				Rie=(MeanIntra-MeanExtra)/MeanIntra;
			Overlap/=nbElements;

			calc->AddTag(xml,LangTag,"Mean Intra",MeanIntra);
			calc->AddTag(xml,LangTag,"Mean Extra",MeanExtra);
			calc->AddTag(xml,LangTag,"Rie",Rie);
			calc->AddTag(xml,LangTag,"Overlap",Overlap);
			calc->AddTag(xml,LangTag,"Min Measure",Measure->GetMinMeasure(Langs()));
		}

		if(LangTag->IsEmpty())
			xml->DeleteTag(LangTag);
	}
}
