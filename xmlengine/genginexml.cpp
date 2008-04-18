/*

	GALILEI Research Project

	GEngineGoogle.cpp

	Extractor of results from the Google search engine - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
// include files for ANSI C/C++
#include <ctype.h>
#include <iostream>
#include <ctime>

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <genginexml.h>
#include <gsession.h>
#include <gstorage.h>
#include <xanalyser.h>
#include <gmetaengine.h>
#include <gplugin.h>
#include <ggalileiapp.h>
using namespace R;
using namespace GALILEI;
using namespace std;

#include "xsqlcmd.h"

//______________________________________________________________________________
//------------------------------------------------------------------------------
void fct_print(const char *str)
{
	cout << str << endl;
}


//------------------------------------------------------------------------------
//
// class GEngineXML
//
//------------------------------------------------------------------------------
GEngineXML::GEngineXML(GFactoryEngine *fac) : GEngine(fac), Name(""), NbResults(40), Weight(1.0)
{
//Params = new GXmlParams;
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void GEngineXML::ApplyConfig()
{
	Name=Factory->Get("Name");
	NbResults = Factory->GetUInt("NbResults");
	Weight = Factory->GetDouble("Weight");
	ask_update = Factory->GetBool("Update");
	ask_reset = ask_update && Factory->GetBool("Reset");
	//cout<<"update? "<<ask_update<<" reset? "<<Factory->GetBool("Reset")<<" nbres? "<<NbResults<<" weight? "<<Weight<<endl;
		Params.ParamsDocSc=Factory->FindParam<RParamStruct>("DocSc Criterion");
		//Params->ParamsType.Set(Factory->FindParam<RParamStruct>("Type Criterion"));
		Params.ParamsDis=Factory->FindParam<RParamStruct>("Distance Criterion");
		//Params->ParamsOcc.Set(Factory->FindParam<RParamStruct>("Occurrence Criterion"));
		Params.ParamsSpecif=Factory->FindParam<RParamStruct>("Specificity Criterion");
		Params.ParamsTfief=Factory->FindParam<RParamStruct>("Tfief Criterion");
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void GEngineXML::Connect(GSession *session) throw(GException)
{
	if(Session)
		return;
	GEngine::Connect(session);
	XSQLCmd::Init();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void GEngineXML::Disconnect(GSession *session) throw(GException)
{
	if(!Session)
		return;
	GEngine::Disconnect(session);

}

//______________________________________________________________________________
//------------------------------------------------------------------------------
void GEngineXML::UpdateDb()
{
	RString paths;
	RCursor<RString> curs;
	RContainer<RString, true, false> pathlist(10);
	XAnalyser analyser(Session->GetStorage(), fct_print);

	paths = Factory->Get("Paths");
	paths.Split(pathlist, '.');
	curs.Set(pathlist);
	for (curs.Start(); !curs.End(); curs.Next())
		analyser.AddPath(curs()->Mid(1), curs()->Mid(0, 1) == "1");
	analyser.Update();
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
RString GEngineXML::ConstructQuery(RContainer<RString, false, false> &keyWords)
{
	RCursor<RString> cstr(keyWords);
	RString curr_exp, buff, res;
	RCharCursor curs;
	bool exp_end = false;														// Tells if it is end of curr_exp

	for (cstr.Start(); !cstr.End(); cstr.Next())
	{
		curs.Set(*cstr());
		for (curs.Start(); !curs.End(); curs.Next())							// Looks in the string
		{
			if (curs() == ' ')													// Space
				exp_end = true;													// => curr_exp reaches end
			else if (curs().IsAlNum())											// Alphanumeric character
				curr_exp += curs();
			else if (curs() == '(')												// Parenthesis
				buff = "(";
			else if (curs() == ')')												// Parenthesis
				buff = ")";
			else if (curs() == '|')												// Or
				buff = "OR";
			else if (curs() == '&' || curs() == '+')			 				// And
				buff = "AND";
			else if (curs() == '/')												// Inc
				buff = "INC";
			else if (curs() == '.')												// Sib
				buff = "SIB";
			else if (curs() == '-')												// Not
				buff = "NOT";
			if (exp_end || buff.GetLen())										// curr_exp reaches end ?
			{
				exp_end = false;
				if (curr_exp.GetLen())
					res += curr_exp + " ";
				curr_exp = "";													// Reset curr_exp
			}
			if (buff.GetLen())
			{
				res += buff + " ";
				buff = "";
			}
		}
		if (curr_exp.GetLen())													// Adds the last exp
			res += curr_exp + " ";
		curr_exp = "";															// Reset curr_exp
	}
	cout << "faiza c la requete " << res << endl;
	return res;
}

// ______________________________________________________________________________
//------------------------------------------------------------------------------
void GEngineXML::Process(R::RContainer<R::RString, false, false>&) throw(GException)
{
	XQuery xquery(Session->GetStorage(), Params);
	xquery.rank_results(Name);

}
//_NORMAL_____________________________________________________________________________
//------------------------------------------------------------------------------
// void GEngineXML::Process(R::RContainer<R::RString,false,false> &keyWords) throw(GException)
// {
// 	RCursor<XResult> cres;
// 	unsigned int res = 0, currRank = 0;
// 	RString url, title, description;
// 	XQuery xquery(Session->GetStorage(), Params);
// 	RContainer<RString, true, false> query_list(20);
// 
// 	try
// 	{
// 		if (ask_reset)
// 		{
// 			GStorageTag cmdtag("ClearXMLTables");
// 			Session->GetStorage()->ExecuteCmd(cmdtag, 0);
// 			Factory->SetBool("Reset", false);										// Disable the Resest of DB
// 			UpdateDb();
// 		}
// 		else if (ask_update)
// 			UpdateDb();
// 		ask_update = false;
// 		ask_reset = false;
// /*        RString temp;
//         temp = ConstructQuery(keyWords);
//         cout << temp.Latin1() << endl;
// 		temp.Split(query_list, RChar(' '));*/
//  		ConstructQuery(keyWords).Split(query_list, RChar(' '));					// Same line as the 4 above, query_list
// 		time_t temps_actD;
// 		time_t temps_actF;
// 
// 		time(&temps_actD);
// 		cres = xquery.Query(query_list, Name);										//   is a container of words and symbols
// 		time(&temps_actF);
// 		if (difftime(temps_actF,temps_actD)) 
// 		cout << "temps prispour cette requ�te  " << difftime(temps_actF,temps_actD) << endl;
// 		for (cres.Start(); !cres.End() && currRank < NbResults; cres.Next())
// 		{
// 			url = cres()->GetUrl();
// 			title = "";
// 			description = cres()->GetSnippet();
// 			GALILEIApp->GetManager<GMetaEngineManager>("MetaEngine")->GetCurrentMethod()->AddResult(url, title, description, currRank, Factory->GetName());
// 			currRank++;
// 		}
// 	}
// 	catch(std::bad_alloc e)
// 	{
// 		throw GException(e.what());
// 	}
// 	catch (RException e)
// 	{
// 		throw GException(e.GetMsg());
// 	}
// }
//______________________________________________________________________________
//------------------------------------------------------------------------------
void GEngineXML::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("Name","/home/fgaultier/resultsFile"));
	params->InsertParam(new RParamValue("NbResults", 40));
	params->InsertParam(new RParamValue("Weight", 1.0));
	params->InsertParam(new RParamValue("Update", false));
	params->InsertParam(new RParamValue("Reset", false));
	params->InsertParam(new RParamValue("Paths", ""));
		params->InsertParam(RPromLinearCriterion::CreateParam("DocSc Criterion"));
	//	params->InsertParam(RPromCriterionParams::CreateParam("Type Criterion"));
		params->InsertParam(RPromLinearCriterion::CreateParam("Distance Criterion"));
	//	params->InsertParam(RPromCriterionParams::CreateParam("Occurrence Criterion"));
		params->InsertParam(RPromLinearCriterion::CreateParam("Specificity Criterion"));
		params->InsertParam(RPromLinearCriterion::CreateParam("Tfief Criterion"));
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
GEngineXML::~GEngineXML(void)
{
}

//______________________________________________________________________________
//------------------------------------------------------------------------------
CREATE_ENGINE_FACTORY("XML Engine", GEngineXML)
