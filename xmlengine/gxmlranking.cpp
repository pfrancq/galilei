/*

	GALILEI Research Project

	GGroupingGGA.cpp

	Heuristic using a GGA - Implementation

	Copyright 2002-2005 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------





// include files for R Project

#include <stdlib.h>
#include <iostream.h>
#include <rcursor.h>
#include <rdebug.h>
#include <rdebug.h>
#include <gxmlranking.h>
#include <gxmlfrag.h>
#include <gxmlir.h>
#include <gxmlprom.h>
#include <gsession.h>
#include <gxmlir.h>
#include <xnodeset.h>
#include <math.h>
using namespace GALILEI;
using namespace R;
using namespace std;
//-----------------------------------------------------------------------------
// include files for GALILEI





//-----------------------------------------------------------------------------
//
//  GGroupingGGA
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GXmlRanking::GXmlRanking(GStorage *_storage, GXmlParams _Params) 	
{
storage = _storage;
Params = _Params;
}
//______________________________________________________________________________
//------------------------------------------------------------------------------
double GXmlRanking::TFIDF(int _idfile,  RContainer<RString, false, false> &_keywordlist)
{
int tfidf=0;
RCursor<RString> curs(_keywordlist);
//� voir !!!  XSQLComputeTF() * XSQLComputeIDF()


double tf, idf, idfTot = 0;
int d =0;
int D=1;
char str[20]  = "";
 	//calcul de N : nombre total des documents de la collection
	cmdtag = new GALILEI::GStorageTag("ComputeD");
	storage->ExecuteCmd(*cmdtag, &D);
	delete cmdtag;
	for(curs.Start();!curs.End();curs.Next())
	{	//tf TF d'un terme de la requ�te
		cmdtag = new GALILEI::GStorageTag("ComputeTF");
  		cmdtag->InsertAttr("idfile", RString::Number(_idfile));
		cmdtag->InsertAttr("word", *curs());
		storage->ExecuteCmd(*cmdtag, &tf); 
		sprintf(str, "%f", tf);
		cout << "tf : " << str<< endl;
		//calcul de n : nombre de document contenant ti
		cmdtag_2 = new GALILEI::GStorageTag("Compute_d");
		cmdtag_2->InsertAttr("word", *curs());
		storage->ExecuteCmd(*cmdtag_2, &d);
		cout << "n : " << d << endl;
		idf = fabs(log(D/d));
		idfTot += tf * idf;
		sprintf(str, "%f", idfTot);
		cout << "idfTot : " << str<< endl;
		delete cmdtag;
		delete cmdtag_2;
	}
	
  	//cmdtag->InsertAttr("word", word);
  	

return idfTot;
}
//------------------------------------------------------------------------------
double GXmlRanking::TFIDF_Nodes(int _idfile,  RContainer<RString, false, false> &_keywordlist)
{
int tfidf=0;
RCursor<RString> curs(_keywordlist);
//� voir !!!  XSQLComputeTF() * XSQLComputeIDF()


double tf, idf, idfTot = 0;
double n =0;
int N=1;
char str[20]  = "";
 	//calcul de N : nombre total des documents de la collection
	cmdtag = new GALILEI::GStorageTag("ComputeN");
	storage->ExecuteCmd(*cmdtag, &N);
	delete cmdtag;
	for(curs.Start();!curs.End();curs.Next())
	{	//tf TF d'un terme de la requ�te
		cmdtag = new GALILEI::GStorageTag("ComputeTF");
  		cmdtag->InsertAttr("idfile", RString::Number(_idfile));
		cmdtag->InsertAttr("word", *curs());
		storage->ExecuteCmd(*cmdtag, &tf); 
		//calcul de n : nombre d'elements contenant ti
		cmdtag_2 = new GALILEI::GStorageTag("Compute_n");
		cmdtag_2->InsertAttr("word", *curs());
		storage->ExecuteCmd(*cmdtag_2, &n);
		//cout << "n : " << n << endl;
		idf = fabs(log(N/n));
		idfTot += tf * idf;
		delete cmdtag;
		delete cmdtag_2;
	}
	
  	//cmdtag->InsertAttr("word", word);
  	

return idfTot;
}
//-----------------------------------------------------------------------------
void GXmlRanking::Compute(R::RString Name) 
{
	bool find_at_least_one_ele=0;
	RPromSol *sol;
	RString line;
	RString name_data_file="/home/fgaultier/Documents/CollectionsXML/Inex05/Inex05Experiments/AllValues2.xml";
	RTextFile src(name_data_file);
	RTextFile Dest(Name);
	Dest.Open(RIO::Append);
	double criteria_values[4]; 
	src.Open(RIO::Read);
	int Nb=0;
	ThProm=new GXmlProm(&Params);
	int i =1;
	char str[10];
	//RContainer<RString, true, false> elements(3941,1);
	RString elementName;
		Dest << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl; 
 		Dest << "<inex-submission participant-id=\"24\" run-id=\""<< Name.Mid(74) << "\" task=\"COS.Thorough\" query=\"automatic\" submission-type=\"adhoc\">" << endl; 
		Dest << "<topic-fields title=\"yes\" castitle=\"no\" description=\"no\" narrative=\"no\" ontopic_keywords=\"no\"/>" << endl; 
		Dest << "<description>run based son sectoin index with best entry point based on first retrieved element</description>" << endl;
		Dest << "<collections>" << endl;
  		Dest << "<collection>ieee</collection>" << endl;
		Dest << "</collections>" << endl;
	while(!src.Eof())
	{
		line=src.GetLine();
		if (line.FindStr("<topic",0,true)!=-1){//debut topic
			Dest <<  line << endl;
			i=1;
			find_at_least_one_ele=0;
		}
		else if (line.FindStr("</topic>",0,true)!=-1){ 
			if (find_at_least_one_ele){
			ThProm->ComputePrometheeII();
			Sols=ThProm->GetSols();
			for(int j=0; j<=Nb-1; j++){
				sprintf(str, "%f", (*Sols)->GetFi());
				Dest << (*Sols)->GetName() << str << "</rsv></result>"<< endl;
				Sols++;
			}
			Nb=0;
			i=1;
			ThProm->ClearSols();
			Dest <<  line << endl;
			}else {
				Nb=0;
				i=1;
				Dest <<  line << endl;
			}
		}else if (line.FindStr("</rsv>",0,true)!=-1){
			i=1;
		}else {
			if (i==1 )	
				if (line != ""){
					elementName=line; 
					find_at_least_one_ele = 1;
				}
			if (i==2 )		//tfidf
				criteria_values[0]= atof ( line.Latin1());
			if (i==3 )//tfief
				criteria_values[1]= atof ( line.Latin1());
			if (i==4 ){//type
			}	
			if (i==5 ) //dis
				criteria_values[2]= atof ( line.Latin1());
			if (i==6 ){ //specif
				criteria_values[3]= atof ( line.Latin1());
				sol=ThProm->NewSol(elementName.Latin1());
				ThProm->Assign(sol,criteria_values);
				Nb++;
			}else	i++;
		}
	}
	Dest << "</inex-submission>" << endl;
	cout << "fini " << Name.Mid(74) << endl;
}
/*
//-----------------------------------------------------------------------------
void GXmlRanking::Compute(R::RString Name) 
{
	bool find_at_least_one_ele=0;
	RPromSol *sol;
	RString line;
	RString name_data_file="/home/fgaultier/Documents/CollectionsXML/Inex05/Inex05Experiments/AllValues2.xml";
	RTextFile src(name_data_file);
	RTextFile Dest(Name);
	Dest.Open(RIO::Append);
	double criteria_values[5]; 
	src.Open(RIO::Read);
	int Nb=0;
	ThProm=new GXmlProm(&Params);
	int i =1;
	char str[10];
	RContainer<RString, true, false> elements(3941,1);
	while(!src.Eof())
	{
		line=src.GetLine();
		if (line.FindStr("<topic",0,true)!=-1){//debut topic
			Dest <<  line << endl;
			i=1;
			find_at_least_one_ele=0;
		}
		else if (line.FindStr("</topic>",0,true)!=-1){ 
			if (find_at_least_one_ele){
			ThProm->ComputePrometheeII();
			Sols=ThProm->GetSols();
			for(int j=0; j<=Nb-1; j++){
				sprintf(str, "%f", (*Sols)->GetFi());
				Dest << elements[j]->Latin1() << str << "</rsv></result>"<< endl;
				Sols++;
			}
			Nb=0;
			i=1;
			ThProm->ClearSols();
			elements.Clear();
			Dest <<  line << endl;
			}else {
				Nb=0;
				i=1;
				Dest <<  line << endl;
			}
		}else if (line.FindStr("</rsv>",0,true)!=-1){
			i=1;
		}else {
			if (i==1 )	
				if (line != ""){
					elements.InsertPtr(new RString(line)); 
					find_at_least_one_ele = 1;
				}
			if (i==2 )
				criteria_values[0]= atof ( line.Latin1());
			if (i==3 )
				criteria_values[1]= atof ( line.Latin1());
			if (i==4 )
				criteria_values[2]= atof ( line.Latin1());		
			if (i==5 )
				criteria_values[3]= atof ( line.Latin1());	
			if (i==6 ){
				criteria_values[4]= atof ( line.Latin1());
				sol=ThProm->NewSol();
				ThProm->Assign(sol,criteria_values);
				Nb++;
			}else	i++;
		}
	}
}*/

// //-NORMAL----------------------------------------------------------------------------
// void GXmlRanking::Compute(RCursor<XNodeSet> cns, RContainer<RString, false, false> &keywordlist) 
// {
// 		double tf_idf, tf_ief;
// 		RPromSol *sol;
// 		GXmlFrag *frag;
// 		unsigned int Nb=0; //number of solutions
// 		unsigned int i=0;
// 		
// 		bool ThereAreSolutons = false;
// 		RCursor<XNode> cn;
// 		
// 		RString res, file;
// 		unsigned int idfile;
// 	
// 		//remplir param ne fonction de ce qui vient de kde (refaire les fenetres de gxmlengine pour saisir les criteres de promethee
// 		ThProm=new GXmlProm(&Params);
// 		
// 		
// 	for (cns.Start(); !cns.End(); cns.Next())
// 	{
// 		ThereAreSolutons = true;
// 		idfile = cns()->GetIdFile();
// 		// compute tf*idf
// 		tf_idf = TFIDF(idfile, keywordlist);
// 		tf_ief = TFIDF_Nodes(idfile, keywordlist);
// 		//cout << "tf-idf idfile " << idfile << "-" << tf_idf << endl;
// 		cn.Set(*cns());
// 		if (cn.GetNb())				//pour chaque noeud on calcule les crit�res
// 			for (cn.Start(); !cn.End(); cn.Next())
// 			{
// //				cout << " faiza le noeud " << cn()->GetIdNode() << " file " << idfile << " ";
// 				cn()->Compute_Promethee_Criteria(tf_idf, tf_ief); //tf_idf est calcul� ici et pour plus d'homgeneit� du code on l'attribue � DocSc dans Compute_Promethee_Criteria
// 				//cout << " pour le noeud " << cn()->GetIdNode() << " on cree la solution " << Nb << endl;
// 				sol=ThProm->NewSol();
// 				ThProm->Assign(sol,cn());
// 				cn()->SetRank(tf_idf);
// 				//cout << "sol : docsc " << cn()->CritDocSc <<endl; //<<": type " << cn()->CritType <<": dis " << cn()->CritDis << endl; 
// 				Nb++;
// 			}
// 	}
// 	if (ThereAreSolutons){
// 		ThProm->ComputePrometheeII();	
// 		Sols=ThProm->GetSols();
// 
// 		// Nous allons remplir le champ Rank de chaque noeud 'solution')
// 		i = 0;
// 		for (cns.Start(); !cns.End(); cns.Next())
// 		{
// 			cn.Set(*cns());
// 			if (cn.GetNb())			
// 				for (cn.Start(); !cn.End(); cn.Next())
// 				{	int j=0;
// 					bool found = false;
// 					Sols=ThProm->GetSols();
// 					//chercher la solution qui correspond � cn()
// 					while(!found && j<Nb){
// 						if ((*Sols)->GetId() == i) { //on a trouv� la solution qui correspond a notre noeud qui au au niv i de cn
// 							
// 							cn()->SetRank((*Sols)->GetFi());
// 							//cout << "le rank de la sol " << (*Sols)->GetId() << " est donn�e au noeud " << cn()->GetIdNode() << endl;
// 							found = true;
// 						}
// 						Sols++;
// 						j++;
// 						
// 					}
// 					i++;
// 				}
// 		}
// 	}
// 	Sols=ThProm->GetSols();
// 	//for(i=0; i<Nb; i++){
// 		//cout << "fi de la solution " << (*Sols)->GetId() << ": " << (*Sols)->GetFi() << ", " << (*Sols)->GetFiPlus() << ", " << (*Sols)->GetFiMinus() << endl;
// 		//Sols++;
// 	//}
// // 			Line=src.GetLine();		
// // 			cout << Line << endl;
// // 			
// // 			frag =new GXmlFrag(Nb);
// // 			if (i==1 )
// // 				frag->CritDocSc = atof ( Line );	
// // 			if (i==2 )
// // 				frag->CritType = atof ( Line );		
// // 			if (i==3 )
// // 				frag->CritDis = atof ( Line );	
// // 			if (i==4 )
// // 				frag->CritOcc = atof ( Line );
// // 			if (i==5 )
// // 				frag->CritSpecif = atof ( Line );
// // 			if (i==6 ){
// // 				i=0;
// // 				frag->CritHier = atof ( Line );
// // 				sol=ThProm->NewSol();
// // 				ThProm->Assign(sol,frag);
// // 				delete frag;
// // 				Nb++;
// // 			}
// // 			i++;
// // 			}
// // 
// // 		ThProm->ComputePrometheeII();
// // 		Sols=ThProm->GetSols();
// // 	
// // 		RTextFile Dest(NameOut);
// // 		Dest.Open(RIO::Append);
// // 		
// // 		for(i=Nb-1; i>0; i--){
// // 			Dest << "fi de la solution " << (*Sols)->GetId() << ": " << (*Sols)->GetFi() << ", " << (*Sols)->GetFiPlus() << ", " << (*Sols)->GetFiMinus() << endl;
// // 			Sols++;
// // 		}
// // 		Dest << "--------------------------------------------------------------------------------------------------------------------"<< endl; 
// 		//Juste un exemple : cout << Params.ParamsDocSc.P << endl;
// 	
// }




//-----------------------------------------------------------------------------
GXmlRanking::~GXmlRanking(void)
{
	
}


