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
/**
* The GStatSimElements template provides statistics on the elemets based on
* their similarities.
* @author Pascal Francq.
* @short Similarities between elements.
*/
template<class E,class C>
	class GStatSimElements
{
public:

	// Local Statistics
	class LocalStat
	{
	public:
		GSubject* Sub;
		bool OverlapL;
		bool OverlapG;

		LocalStat(GSubject* s) : Sub(s), OverlapL(false), OverlapG(false) {}
		int Compare(const LocalStat* l) const {return(Sub->Compare(l->Sub));}
		int Compare(const GSubject* s) const {return(Sub->Compare(s));}
	};

protected:

	/**
	* Session.
	*/
	GSession* Session;

	/**
	* The Mean of the Mean intra group similarity (Without IF).
	*/
	double MeanIntraML;

	/**
	* The Mean of the Mean extra group similarity (Without IF).
	*/
	double MeanExtraML;

	/**
	* Ratio Rie (Without IF).
	*/
	double RieL;

	/**
	* Global Overlap Factor (Without IF).
	*/
	double GOverlapL;

	/**
	* Overlap Factor (Without IF).
	*/
	double OverlapL;

	/**
	* The Mean of the Mean intra group similarity (With IF).
	*/
	double MeanIntraMG;

	/**
	* The Mean of the Mean extra group similarity (With IF).
	*/
	double MeanExtraMG;

	/**
	* Ratio Rie (With IF).
	*/
	double RieG;

	/**
	* Global Overlap Factor (With IF).
	*/
	double GOverlapG;

	/**
	* Overlap Factor (With IF).
	*/
	double OverlapG;

	/**
	* Similarities with IF must be calculated.
	*/
	bool Global;

	/**
	* Similarities without IF must be calculated.
	*/
	bool Local;

	/**
	* Statistics Output file.
	*/
	R::RTextFile* File;

	/**
	* Container of statistics on the subjects.
	*/
	R::RContainer<LocalStat,true,true> Sub;

public:

	/**
	* Constructor.
	* @param ses            The  galilei session.
	* @param f              File.
	* @param g              Similarities with IF.
	* @param l              Similarities without IF.
	*/
	GStatSimElements(GSession* ses,R::RTextFile* f,bool g,bool l);

	/**
	* Method that must return a cursor of the elements. This method must be
	* overloaded.
	*/
	virtual C GetElementCursor(GLang*)  {C Cur; return(Cur);}

	/**
	* Method that test if two elements are of the subject. This method must be
	* overloaded.
	*/
	virtual bool SameSubject(E*,E*) {return(false);}

	/**
	* Method that verifies if a subject is assigned to a given element. This
	* method must be overloaded.
	*/
	virtual bool HasSubject(E*) {return(false);}

	/**
	* Update local statistics to made all the subjects of an element overlapped.
	* This method must be overloaded.
	*/
	virtual void OverlapTopics(E*,bool) {}

	/**
	* Compute the similarities statistics.
	*/
	void Run(GStatsCalc* calc,RXMLStruct* xml,RXMLTag* tag);

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

//------------------------------------------------------------------------------
template<class E,class C>
	GStatSimElements<E,C>::GStatSimElements(GSession* ses,RTextFile* f,bool g,bool l)
	: Session(ses), Global(g), Local(l), File(f), Sub(100,50)
{
	if(File)
	{
		(*File)<<"Id";
		if(Global) (*File)<<"RieGlobal";
		if(Local) (*File)<<"RieLocal";
		(*File)<<endl;
	}
}


//------------------------------------------------------------------------------
template<class E,class C>
	void GStatSimElements<E,C>::Run(GStatsCalc* calc,RXMLStruct* xml,RXMLTag* tag)
{
	R::RCursor<GFactoryLang> Langs;
	GLang* lang;
	C Cur1;
	C Cur2;
	R::RCursor<GSubject> Sub1;
	double SimIntraL;
	double SimExtraL;
	double SimIntraG;
	double SimExtraG;
	double tmp;
	unsigned int nbIntra;
	unsigned int nbExtra;
	unsigned int nbElements;
	bool Same;
	double MinIntraL;
	double MaxExtraL;
	double MinIntraG;
	double MaxExtraG;
	RXMLTag* LangTag;

	// Go through the languages
	Langs=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		LangTag=0;
		nbElements=0;
		lang=Langs()->GetPlugin();
		if(!lang) continue;

		// Initialization
		OverlapL=MeanExtraML=MeanIntraML=0.0;
		OverlapG=MeanExtraMG=MeanIntraMG=0.0;
		Sub.Clear();
		Cur1=GetElementCursor(lang);
		Cur2=GetElementCursor(lang);

		// Go through each element
		for(Cur1.Start();!Cur1.End();Cur1.Next())
		{
			if(!Cur1()->IsDefined()) continue;
			nbElements++;

			// Init Local Data
			SimIntraG=SimExtraG=SimIntraL=SimExtraL=0.0;
			nbIntra=nbExtra=0;
			MinIntraG=MinIntraL=1.1;
			MaxExtraG=MaxExtraL=-1.1;

			// Go through to other elements
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				if(Cur1()==Cur2()) continue;
				if(!Cur2()->IsDefined()) continue;

				// Look if Same topic
				Same=SameSubject(Cur1(),Cur2());
				if(Same)
					nbIntra++;
				else
					nbExtra++;

				if(Global)
				{
					tmp=Cur1()->SimilarityIFF(Cur2());
					if(Same)
					{
						SimIntraG+=tmp;
						if(tmp<MinIntraG) MinIntraG=tmp;
					}
					else
					{
						SimExtraG+=tmp;
						if(tmp>MaxExtraG) MaxExtraG=tmp;
					}
				}

				if(Local)
				{
					tmp=Cur1()->Similarity(Cur2());
					if(Same)
					{
						SimIntraL+=tmp;
						if(tmp<MinIntraL) MinIntraL=tmp;
					}
					else
					{
						SimExtraL+=tmp;
						if(tmp>MaxExtraL) MaxExtraL=tmp;
					}
				}
			}

			// Compute Overlap
			if(Global&&(MaxExtraG>MinIntraG))
			{
				OverlapG++;
				OverlapTopics(Cur1(),true);
			}
			if(Local&&(MaxExtraL>MinIntraL))
			{
				OverlapL++;
				OverlapTopics(Cur1(),false);
			}

			// Compute Rie for current element
			if(Global)
			{
				if(nbIntra)
					SimIntraG/=nbIntra;
				else
					SimIntraG=1.0;
				if(nbExtra)
					SimExtraG/=nbExtra;
				else
					SimExtraG=1.0;
				MeanIntraMG+=SimIntraG;
				MeanExtraMG+=SimExtraG;
			}
			if(Local)
			{
				if(nbIntra)
					SimIntraL/=nbIntra;
				else
					SimIntraL=1.0;
				if(nbExtra)
					SimExtraL/=nbExtra;
				else
					SimExtraL=1.0;
				MeanIntraML+=SimIntraL;
				MeanExtraML+=SimExtraL;
			}
			if(File)
			{
				(*File)<<Cur1()->GetId();
				if(Global)
				{
					if(SimIntraL==0.0)
						tmp=0.0;
					else
						tmp=(SimIntraL-SimExtraL)/SimIntraL;
					(*File)<<tmp;
				}
				if(Local)
				{
					if(SimIntraL==0.0)
						tmp=0.0;
					else
						tmp=(SimIntraL-SimExtraL)/SimIntraL;
					(*File)<<tmp;
				}
				(*File)<<endl;
			}
		}

		// Compute elements statistics
		if(!nbElements) continue;
		if(Global)
		{
			MeanIntraMG/=nbElements;
			MeanExtraMG/=nbElements;
			if(MeanIntraMG==0.0)
				RieG=0.0;
			else
				RieG=(MeanIntraMG-MeanExtraMG)/MeanIntraMG;
			OverlapG/=nbElements;

			if(!LangTag)
			{
				LangTag=new RXMLTag(lang->GetName());
				xml->AddTag(tag,LangTag);
			}
			RXMLTag *t=new RXMLTag("With IF");
			xml->AddTag(LangTag,t);
			calc->AddTag(xml,t,"MeanIntra",MeanIntraMG);
			calc->AddTag(xml,t,"MeanExtra",MeanExtraMG);
			calc->AddTag(xml,t,"Rie",RieG);
			calc->AddTag(xml,t,"Global Overlap",OverlapG);
		}
		if(Local)
		{
			MeanIntraML/=nbElements;
			MeanExtraML/=nbElements;
			if(MeanIntraML==0.0)
				RieL=0.0;
			else
				RieL=(MeanIntraML-MeanExtraML)/MeanIntraML;
			OverlapL/=nbElements;

			if(!LangTag)
			{
				LangTag=new RXMLTag(lang->GetName());
				xml->AddTag(tag,LangTag);
			}
			RXMLTag *t=new RXMLTag("Without IF");
			xml->AddTag(LangTag,t);
			calc->AddTag(xml,t,"MeanIntra",MeanIntraML);
			calc->AddTag(xml,t,"MeanExtra",MeanExtraML);
			calc->AddTag(xml,t,"Rie",RieL);
			calc->AddTag(xml,t,"Global Overlap",OverlapL);
		}
	}

	// Compute topics statistics
	R::RCursor<LocalStat> LocalStats(Sub);
	if(!LocalStats.GetNb()) return;
	LangTag=new RXMLTag("Topics");
	xml->AddTag(tag,LangTag);
	for(LocalStats.Start(),GOverlapG=0,GOverlapL=0;!LocalStats.End();LocalStats.Next())
	{
		RXMLTag *t=new RXMLTag(LocalStats()->Sub->GetName());
		xml->AddTag(LangTag,t);
		if(LocalStats()->OverlapG)
		{
			GOverlapG++;
			calc->AddTag(xml,t,"Overlap with IF","true");
		}
		else
			calc->AddTag(xml,t,"Overlap with IF","false");
		if(LocalStats()->OverlapL)
		{
			GOverlapL++;
			calc->AddTag(xml,t,"Overlap without IF","true");
		}
		else
			calc->AddTag(xml,t,"Overlap without IF","false");
	}
	GOverlapG/=LocalStats.GetNb();
	GOverlapL/=LocalStats.GetNb();
	calc->AddTag(xml,LangTag,"Overlap with IF",GOverlapG);
	calc->AddTag(xml,LangTag,"Overlap without IF",GOverlapL);
}
